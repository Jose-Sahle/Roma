// WaCServer.cpp: implementation of the CWaCServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaCServer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define EXPIRATION_DATE	_T("99999999")


HANDLE	g_hThreadWaitConnection;
bool	g_isListeningActivated;
CString g_strExpirationDate;

WaitConnectionParams *g_pParams;

extern HANDLE	g_hMutex;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaCServer::CWaCServer()
{
	g_isListeningActivated	= false;
	m_bIsListening			= false;
}

CWaCServer::~CWaCServer()
{
	WSACleanup();
}

bool CWaCServer::StartListen(unsigned short nPort, GETCONNECTEDSOCKET pFunction)
{
	bool b_Ret;

	if(m_bIsListening)
		return false;

	g_pParams = new WaitConnectionParams;

	g_pParams->nPort	= nPort;
	g_pParams->Function	= pFunction;
	g_pParams->pParent	= this;

	g_hThreadWaitConnection = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadWaitConnection, (LPVOID) g_pParams, CREATE_SUSPENDED, &m_dwThreadId);

	b_Ret = (ResumeThread(g_hThreadWaitConnection) != 0xFFFFFFFF);

	m_bIsListening = b_Ret;

	return b_Ret;
}

bool CWaCServer::StopListen()
{
	bool bRet;

	if(!m_bIsListening)
		return false;

	bRet = TerminateThread(g_hThreadWaitConnection, 1) ? true : false;

	closesocket(m_ListenSocket);

	WSACleanup();

	delete g_pParams;
	
	m_bIsListening = false;

	CloseHandle(g_hMutex);

	return bRet;
}

TCHAR * CWaCServer::WaCGetLastErrorText()
{
	return m_szLastErrorText;
}

void ThreadWaitConnection(LPVOID pParam)
{
	WaitConnectionParams*		pParams = (WaitConnectionParams*) pParam;
	AcceptedConnectionParams*	pAcceptedParams;

	struct	sockaddr_in local, from;
	SOCKET	MsgSock;
	int		fromlen;
	DWORD	dwLoopMutex;

	CTime	dtNow;
	CString	strNow;

	if (WSAStartup(0x202,&pParams->pParent->m_wsaData) == SOCKET_ERROR)
	{
		wsprintf(pParams->pParent->m_szLastErrorText, _T("WSAStartup failed with error %d"), WSAGetLastError());
		ExitThread(1);
	}

	if (pParams->nPort == 0)
	{
		wsprintf(pParams->pParent->m_szLastErrorText, _T("Invalid port number: %d"), pParams->nPort);
		WSACleanup();
		ExitThread(1);
	}
 
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;

	/*  
	 * Port MUST be in Network Byte Order 
	 */ 
	local.sin_port = htons(pParams->nPort);
	 
	pParams->pParent->m_ListenSocket = socket(AF_INET, SOCK_STREAM, 0); // TCP socket 

	if (pParams->pParent->m_ListenSocket == INVALID_SOCKET)
	{
		wsprintf(pParams->pParent->m_szLastErrorText, _T("socket() failed with error %d"), WSAGetLastError());
		WSACleanup();
		ExitThread(1);
	}

	// 
	// bind() associates a local address and port combination with the 
	// socket just created. This is most useful when the application is a  
	// server that has a well-known port that clients know about in advance. 
	// 
	 
	if (bind(pParams->pParent->m_ListenSocket, (struct sockaddr*) &local, sizeof(local) ) == SOCKET_ERROR)
	{
		wsprintf(pParams->pParent->m_szLastErrorText, _T("bind() failed with error %d"), WSAGetLastError());
		WSACleanup();
		ExitThread(1);
	}
// 
// So far, everything we did was applicable to TCP as well as UDP. 
// However, there are certain steps that do not work when the server is 
// using UDP. 
// 
	if (listen(pParams->pParent->m_ListenSocket, 5) == SOCKET_ERROR)
	{
		wsprintf(pParams->pParent->m_szLastErrorText, _T("listen() failed with error %d"), WSAGetLastError());
		WSACleanup();
		ExitThread(1);
	}

	// printf("%s: 'Listening' on port %d, protocol %s",argv[0],port, (socket_type == SOCK_STREAM)?"TCP":"UDP");

	g_isListeningActivated = true;

	while (g_isListeningActivated)
	{ 
		fromlen = sizeof(from);
		// 
		// accept() doesn't make sense on UDP, since we do not listen() 
		// 

		dtNow  = CTime::GetCurrentTime();
		strNow = dtNow.Format(_T("%Y%m%d"));

		MsgSock = accept(pParams->pParent->m_ListenSocket, (struct sockaddr*) &from, &fromlen);
		if (MsgSock == INVALID_SOCKET)
		{
			dwLoopMutex = WaitForSingleObject(g_hMutex, INFINITE);
			wsprintf(pParams->pParent->m_szLastErrorText, _T("accept() error %d"), WSAGetLastError());
			WSACleanup();
			ReleaseMutex(g_hMutex);
			ExitThread(1);
		}

		dwLoopMutex = WaitForSingleObject(g_hMutex, INFINITE);

		pAcceptedParams = new AcceptedConnectionParams;

		pAcceptedParams->Address = from;
		pAcceptedParams->Socket  = MsgSock;

		pParams->Function((void*) pAcceptedParams);

		delete pAcceptedParams;

		ReleaseMutex(g_hMutex);

		Sleep(0);
	}

	delete pParams;

	ExitThread(0);
}