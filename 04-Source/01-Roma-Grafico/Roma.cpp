// Roma.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <initguid.h>
#include "srvROMA.h"

#include "Estruturas.h"

#include "Roma.h"

#include "Library.h"

//#include "srvROMA_i.c"

#include "ServerRomaConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL LerConfiguracoes();

STRU_VARIAVEIS_GLOBAIS g_struVariaveisGlobais;

HRESULT ObjMQRoute( HANDLE *hEvFinish);

BOOL wt_run = 0;

HANDLE	g_hMutex;

BEGIN_OBJECT_MAP(ObjectMap)

END_OBJECT_MAP()

CServiceModule _Module;

/////////////////////////////////////////////////////////////////////////////
// CRomaApp

BEGIN_MESSAGE_MAP(CRomaApp, CWinApp)
	//{{AFX_MSG_MAP(CRomaApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRomaApp construction

CRomaApp::CRomaApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CRomaApp::~CRomaApp()
{
	
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CRomaApp object

CRomaApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRomaApp initialization

BOOL CRomaApp::InitInstance()
{
	// Declarações de variáveis
    TCHAR szValue[_MAX_PATH];
    DWORD dwLen = _MAX_PATH;
	LPCTSTR lpszToken;
    TCHAR szTokens[] = _T("-/");

	
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	Enable3dControls();			// Call this when using MFC in a shared DLL

	/************************************************************************************
	/*
	/* Código do SERVIÇO DO ROMA
	/*
	/***********************************************************************************/
    m_lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT

	// Limpa a estrutura de variáveis globais

    _Module.Init(ObjectMap, m_hInstance, IDS_SERVICENAME, &LIBID_SRVROMALib);
    _Module.m_bService = TRUE;

	lpszToken = FindOneOf(m_lpCmdLine, szTokens);
    while (lpszToken != NULL)
    {
        if (lstrcmpi(lpszToken, _T("UnregServer")) == 0)
            return _Module.UnregisterServer();

        // Register as Local Server
        if (lstrcmpi(lpszToken, _T("RegServer")) == 0)
            return _Module.RegisterServer(TRUE, FALSE);
        
        // Register as Service
        if (lstrcmpi(lpszToken, _T("Service")) == 0)
            return _Module.RegisterServer(TRUE, TRUE);

		if (lstrcmpi(lpszToken, _T("Config")) == 0)
		{
			CServerRomaConfig dlg;

			m_pMainWnd = &dlg;

			LerConfiguracoes();

			dlg._Module = &_Module;

			dlg.DoModal();
			
			return TRUE;
		}
        
        lpszToken = FindOneOf(lpszToken, szTokens);
    }

    // Are we Service or Local Server
    CRegKey keyAppID;
    LONG lRes = keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_READ);
    if (lRes != ERROR_SUCCESS)
        return lRes;

    CRegKey key;
    lRes = key.Open(keyAppID, _T("{9049DC8A-D32B-426F-B4D1-A152DED669A4}"), KEY_READ);
    if (lRes != ERROR_SUCCESS)
	{
		AfxMessageBox(_T("O serviço Roma não está registrado!"), MB_ICONSTOP);

		CServerRomaConfig dlg;
		m_pMainWnd = &dlg;

		LerConfiguracoes();
		
		dlg._Module = &_Module;

		dlg.DoModal();

		return TRUE;
	}

	lRes = key.QueryValue(szValue, _T("LocalService"), &dwLen);

	g_hMutex = CreateMutex(NULL, FALSE, NULL);

    _Module.m_bService = FALSE;
    if (lRes == ERROR_SUCCESS)
        _Module.m_bService = TRUE;

    _Module.Start();

	CloseHandle(g_hMutex);
    // When we get here, the service has been stopped
    return _Module.m_status.dwWin32ExitCode;

	/************************************************************************************
	/*
	/* Código do SERVIÇO DO ROMA
	/*
	/***********************************************************************************/
}

// Although some of these functions are big they are declared inline since they are only used once

inline HRESULT CServiceModule::RegisterServer(BOOL bRegTypeLib, BOOL bService)
{
	SECURITY_ATTRIBUTES SecAttr;
	DWORD dwDisposition;

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return hr;

    // Remove any previous service since it may point to
    // the incorrect file
    Uninstall();

    // Add service entries
    UpdateRegistryFromResource(IDR_SrvROMA, TRUE);

    // Adjust the AppID for Local Server or Service
    CRegKey keyAppID;
    LONG lRes = keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_WRITE);
    if (lRes != ERROR_SUCCESS)
        return lRes;

/*    CRegKey key;
    lRes = key.Open(keyAppID, _T("{9049DC8A-D32B-426F-B4D1-A152DED669A4}"), KEY_WRITE);*/
    CRegKey key;
	SecAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	SecAttr.lpSecurityDescriptor = NULL;
	SecAttr.bInheritHandle = TRUE;

	lRes = key.Create(keyAppID, 
		              _T("{9049DC8A-D32B-426F-B4D1-A152DED669A4}"), 
					   NULL, 
					   REG_OPTION_NON_VOLATILE, 
					   KEY_WRITE, &SecAttr, &dwDisposition);
    if (lRes != ERROR_SUCCESS)
        return lRes;
    key.DeleteValue(_T("LocalService"));
    
    if (bService)
    {
        key.SetValue(_T("ROMA"), _T("LocalService"));
        key.SetValue(_T("-Service"), _T("ServiceParameters"));

        // Create service
        lRes = Install();
    }

    // Add object entries
    hr = CComModule::RegisterServer(bRegTypeLib);
	if(lRes)
	{
		LogEvent(bService?_T("Instalação do Serviço foi bem-sucedida"):_T("TypeLib Installation Succeeded"), EVENTLOG_INFORMATION_TYPE, _T("Roma"));
	}

    CoUninitialize();
    return hr;
}

inline HRESULT CServiceModule::UnregisterServer()
{
	BOOL bService=FALSE;
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return hr;

    // Remove service entries
    UpdateRegistryFromResource(IDR_SrvROMA, FALSE);
    // Remove service
    bService = Uninstall();
    // Remove object entries
    CComModule::UnregisterServer(TRUE);
	LogEvent(bService?_T("Remoção do Serviço foi bem-sucedida"):_T("TypeLib Unsucceeded"), EVENTLOG_INFORMATION_TYPE, _T("Roma"));

    CoUninitialize();
    return S_OK;
}

inline void CServiceModule::Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, UINT nServiceNameID, const GUID* plibid)
{
    CComModule::Init(p, h, plibid);

    m_bService = TRUE;

    LoadString(h, nServiceNameID, m_szServiceName, sizeof(m_szServiceName) / sizeof(TCHAR));

    // set up the initial service status 
    m_hServiceStatus = NULL;

    m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_status.dwCurrentState = SERVICE_STOPPED;
    m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    m_status.dwWin32ExitCode = 0;
    m_status.dwServiceSpecificExitCode = 0;
    m_status.dwCheckPoint = 0;
    m_status.dwWaitHint = 0;
}

LONG CServiceModule::Unlock()
{
    LONG l = CComModule::Unlock();
    if (l == 0 && !m_bService)
        PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
    return l;
}

BOOL CServiceModule::IsInstalled()
{
    BOOL bResult = FALSE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM != NULL)
    {
        SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_QUERY_CONFIG);
        if (hService != NULL)
        {
            bResult = TRUE;
            ::CloseServiceHandle(hService);
        }
        ::CloseServiceHandle(hSCM);
    }
    return bResult;
}

inline BOOL CServiceModule::Install()
{
    if (IsInstalled())
        return TRUE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Serviço não pode ser aberto!"), m_szServiceName, MB_OK);
        return FALSE;
    }

    // Get the executable file path
    TCHAR szFilePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, _MAX_PATH);

    SC_HANDLE hService = ::CreateService(
        hSCM, m_szServiceName, m_szServiceName,
        SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
        SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
        szFilePath, NULL, NULL, _T("RPCSS\0MSMQ\0"), NULL, NULL);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Serviço não pode ser criado!"), m_szServiceName, MB_OK);
        return FALSE;
    }

	//Sets this service textual description (**BBA)
	SERVICE_DESCRIPTION svd;
	TCHAR txBuff[_MAX_PATH];
	svd.lpDescription = txBuff;
	LoadString(NULL, IDS_SERVICEDESC, svd.lpDescription, sizeof(txBuff) / sizeof(TCHAR));
	ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &svd);

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return TRUE;
}

inline BOOL CServiceModule::Uninstall()
{
    if (!IsInstalled())
        return TRUE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Couldn't open service manager"), m_szServiceName, MB_OK);
        return FALSE;
    }

    SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_STOP | DELETE);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't open service"), m_szServiceName, MB_OK);
        return FALSE;
    }
    SERVICE_STATUS status;
    ::ControlService(hService, SERVICE_CONTROL_STOP, &status);

    BOOL bDelete = ::DeleteService(hService);
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    if (bDelete)
        return TRUE;

    MessageBox(NULL, _T("Service could not be deleted"), m_szServiceName, MB_OK);
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Service startup and registration
inline void CServiceModule::Start()
{
    SERVICE_TABLE_ENTRY st[] =
    {
        { m_szServiceName, _ServiceMain },
        { NULL, NULL }
    };
    if (m_bService && !::StartServiceCtrlDispatcher(st))
    {
        m_bService = FALSE;
    }
    if (m_bService == FALSE)
        Run();
}

inline void CServiceModule::ServiceMain(DWORD /* dwArgc */, LPTSTR* /* lpszArgv */)
{
    // Register the control request handler
    m_status.dwCurrentState = SERVICE_START_PENDING;

    m_hServiceStatus = RegisterServiceCtrlHandler(m_szServiceName, _Handler);
    if (m_hServiceStatus == NULL)
    {
        LogEvent(_T("RegisterServiceCtrlHandler falhou"), EVENTLOG_ERROR_TYPE, _T("Roma"));
        return;
    }
    SetServiceStatus(SERVICE_START_PENDING);

    m_status.dwWin32ExitCode = S_OK;
    m_status.dwCheckPoint = 0;
    m_status.dwWaitHint = 0;

    // When the Run function returns, the service has stopped.
    Run();

    SetServiceStatus(SERVICE_STOPPED);
}

inline void CServiceModule::Handler(DWORD dwOpcode)
{
    switch (dwOpcode)
    {
    case SERVICE_CONTROL_STOP:
        SetServiceStatus(SERVICE_STOP_PENDING);
        PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
        break;
    case SERVICE_CONTROL_PAUSE:
        break;
    case SERVICE_CONTROL_CONTINUE:
		SetServiceStatus(SERVICE_RUNNING);
        break;
    case SERVICE_CONTROL_INTERROGATE:
		SetServiceStatus(SERVICE_RUNNING);
        break;
    case SERVICE_CONTROL_SHUTDOWN:
        break;
    default:
        LogEvent(_T("Bad service request"), EVENTLOG_ERROR_TYPE, _T("Roma"));
    }
}

void WINAPI CServiceModule::_ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
    _Module.ServiceMain(dwArgc, lpszArgv);
}
void WINAPI CServiceModule::_Handler(DWORD dwOpcode)
{
    _Module.Handler(dwOpcode); 
}

void CServiceModule::SetServiceStatus(DWORD dwState)
{
    m_status.dwCurrentState = dwState;
    ::SetServiceStatus(m_hServiceStatus, &m_status);
}

void CServiceModule::Run()
{
	DWORD thdID;
	HANDLE hthd;
	DWORD cLoops = 0, ExitCode;

    _Module.dwThreadID = GetCurrentThreadId();

    HRESULT hr = CoInitialize(NULL);

//  If you are running on NT 4.0 or higher you can use the following call
//  instead to make the EXE free threaded.
//  This means that calls come in on a random RPC thread
//  HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    _ASSERTE(SUCCEEDED(hr));

    // This provides a NULL DACL which will allow access to everyone.
    CSecurityDescriptor sd;
    sd.InitializeFromThreadToken();
    hr = CoInitializeSecurity(sd, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    _ASSERTE(SUCCEEDED(hr));

    hr = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER | CLSCTX_REMOTE_SERVER, REGCLS_MULTIPLEUSE);

    _ASSERTE(SUCCEEDED(hr));

    LogEvent(_T("Serviço iniciado"), EVENTLOG_SUCCESS, _T("Roma"));

    if (m_bService)
        SetServiceStatus(SERVICE_RUNNING);	

	HANDLE hEvStop = CreateEvent( NULL,FALSE, FALSE, NULL);
	hthd = CreateThread(NULL,0, (LPTHREAD_START_ROUTINE)ObjMQRoute, &hEvStop,0, &thdID);

    MSG msg;
	BOOL bGetMessage = GetMessage(&msg, NULL, NULL, NULL);
    while (bGetMessage)
	{
        DispatchMessage(&msg);

		bGetMessage = GetMessage(&msg, NULL, NULL, NULL);
	}

	LogEvent(_T("Serviço parado\n"), EVENTLOG_INFORMATION_TYPE, _T("Roma"));
	SetEvent(hEvStop);

	//Waiting thd to stop
	while(cLoops++ < 20)  // Wait for 20 secs
	{
		GetExitCodeThread( hthd, &ExitCode);
		if(ExitCode != STILL_ACTIVE)
			break;
		Sleep(1000); //Wait 1 second
	}

	CloseHandle(hthd);
    _Module.RevokeClassObjects();

    CoUninitialize();
}
