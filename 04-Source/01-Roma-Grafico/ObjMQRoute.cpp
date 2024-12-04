////////////////////////////////////////////////////////////////////
//////////////  ObjMQRoute - Route Objects from Messaging Assyncronous
//////////Oswaldo Bernice Jr.    MCS - 2001
///////////
#define WINVER 0x0500
#define  _WIN32_WINNT   WINVER

#include "stdafx.h"

#include <mq.h>
#include <conio.h>
#include "srvRoma2.h"

#include "ServerRomaConfig.h"
#include "Library.h"

#include "CommunicationStructs.h"

#include "WASocket.h"

#include "Estruturas.h"

#define NUMBEROFPROPERTIES 4

#define MAX_BUFFER 2048

extern HANDLE	g_hMutex;

CRITICAL_SECTION  gpCriticalSection;

extern STRU_VARIAVEIS_GLOBAIS g_struVariaveisGlobais;

/// Dispara metodos no ActiveX via interface Idispatch
//////////////////////////////////////
#include <Resapi.h>

static int gsux=0;

void GetSocketConnection(void * pParam);
int Receive(const SOCKET Socket, void * pBuffer, const int nBufferSize, unsigned long * plBytes);
int Send(const SOCKET Socket, void * pBuffer, const int nBufferSize, unsigned long * plBytes);
void ThreadSyncSocket(LPVOID pParam);
void ResetConnection(short nConnectionNumber);
/************************************************************************
 *
 * LerConfiguracoes
 * Le as configurações do serviço
 *
 ************************************************************************/
BOOL LerConfiguracoes()
{
	BOOL bRetorno = TRUE;
	TCHAR szReturnString[255];

	TCHAR *lpBackSlash;
	TCHAR szConfigFile[_MAX_PATH];

	GetModuleFileName(NULL, szConfigFile, _MAX_PATH);

	lpBackSlash = _tcsrchr( szConfigFile, _T('\\'));
	*lpBackSlash= _T('\0');

	_tcscat( szConfigFile, _T("\\ROMA.cfg"));

	HANDLE hHeap = GetProcessHeap();

	CString strConfigFile = CString(szConfigFile);

	if (!ArquivoExiste(strConfigFile))
	{
		CString strMensagem;
		strMensagem.Format(_T("Arquivo \"%s\" não foi encontrado!\n"), strConfigFile);
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));

		return FALSE;
	}

	g_struVariaveisGlobais.g_Connections = NULL;
	/////////////////////////////////////////////////////
	///
	/// INICIO CONFIGURAÇÕES DO SERVICO
	///
	/////////////////////////////////////////////////////
	ZeroMemory(szReturnString, 255);
	if (GetPrivateProfileString(_T("SERVICO"), _T("timeout"), _T("20"), szReturnString, 254, strConfigFile) == 0)
		g_struVariaveisGlobais.dwServicoTimeOut = 20;
	else
		Val(CString(szReturnString), g_struVariaveisGlobais.dwServicoTimeOut);

	/////////////////////////////////////////////////////
	///
	/// FINAL CONFIGURAÇÕES DO SERVICO
	///
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	///
	/// INICIO LEITURA DAS FILAS
	///
	/////////////////////////////////////////////////////
	// Lê quantidade de fila
	ZeroMemory(szReturnString, 255);
	if (GetPrivateProfileString(_T("FILAS"), _T("queuecount"), _T("0"), szReturnString, 254, strConfigFile) == 0)
	{
		CString strMensagem;
		strMensagem = _T("Não foi possível ler quantidade de filas? (queuecount)\n");
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));

		bRetorno = FALSE;
	}
	else
		Val(CString(szReturnString), g_struVariaveisGlobais.dwTotalFilas);

	// Se quantidade de fila for zero, também para o serviço
	if( g_struVariaveisGlobais.dwTotalFilas == 0)
	{
		CString strMensagem;
		strMensagem = _T("Total inválido de filas\n");
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
		bRetorno = FALSE;
	}

	// Verifica se o número de filas é maior que o máximo permitido
	if (g_struVariaveisGlobais.dwTotalFilas > MAX_COUNT_RX_QUEUE )
	{
		CString strMensagem;
		strMensagem.Format(_T("Quantidade de fila no Roma.ini: %d\nMáximo permitido:%d\n"), g_struVariaveisGlobais.dwTotalFilas, MAX_COUNT_RX_QUEUE);
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
		g_struVariaveisGlobais.dwTotalFilas = MAX_COUNT_RX_QUEUE;
	}

	//Lê as filas
	for (int nContador = 1; nContador <= (int)g_struVariaveisGlobais.dwTotalFilas; nContador++)
	{
		CString strFila;
		strFila.Format(_T("queue%d"), nContador);
		ZeroMemory(szReturnString, 255);

		if (GetPrivateProfileString(_T("FILAS"), strFila, _T(""), szReturnString, 254, strConfigFile) > 0)
		{
			int nTamanho = strlen(szReturnString) + 1;

			g_struVariaveisGlobais.wsaQueueNames[nContador-1] = (WCHAR*)HeapAlloc( hHeap, 0, nTamanho * 2 );			

			MultiByteToWideChar(CP_ACP, 0, szReturnString, -1, g_struVariaveisGlobais.wsaQueueNames[nContador-1], nTamanho);
		}
		else
		{
			CString strMensagem;
			strMensagem.Format(_T("Item %s não encontrado no arquivo ROMA.INI\n"), strFila);
			LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
		}
	}

	// Lê o nome da fila de Erro do Roma
	ZeroMemory(szReturnString, 255);
	if (GetPrivateProfileString(_T("FILAS"), _T("queueerror"), _T(""), szReturnString, 254, strConfigFile) == 0)
		wcscpy(g_struVariaveisGlobais.wsErrorQueueName, DEFAULT_ERROR_QUEUE_NAME);
	else
	{
		int nTamanho = strlen(szReturnString);

		MultiByteToWideChar(CP_ACP, 0, szReturnString, -1, g_struVariaveisGlobais.wsErrorQueueName, nTamanho);		 
	}

	// Lê o TimeOut para finalizar filas
	ZeroMemory(szReturnString, 255);
	if (GetPrivateProfileString(_T("FILAS"), _T("timeoutmsmq"), _T("2000"), szReturnString, 254, strConfigFile) == 0)
		g_struVariaveisGlobais.dwFilaTimeOut = 20;
	else
		Val(CString(szReturnString), g_struVariaveisGlobais.dwFilaTimeOut);

	/////////////////////////////////////////////////////
	///
	/// FINAL LEITURA DAS FILAS
	///
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	///
	/// INICIO CONFIGURAÇÕES PARA ACESSO REMOTO
	///
	/////////////////////////////////////////////////////

	// Lê se o serviço deve ou não ser inicializado
	ZeroMemory(szReturnString, 255);
	if (GetPrivateProfileString(_T("REMOTO"), _T("IniciarServico"), _T("FALSE"), szReturnString, 254, strConfigFile) > 0)
	{
		CString strIniciarServico;

		strIniciarServico = CString(szReturnString);
		strIniciarServico.MakeUpper();

		if (strIniciarServico == _T("TRUE"))
			g_struVariaveisGlobais.bIniciarServico = TRUE;
		else
			g_struVariaveisGlobais.bIniciarServico = FALSE;
	}
	else
		g_struVariaveisGlobais.bIniciarServico = FALSE;

	// Lê o número da porta de comunicação
	ZeroMemory(szReturnString, 255);
	if (GetPrivateProfileString(_T("REMOTO"), _T("Porta"), _T("0"), szReturnString, 254, strConfigFile) > 0)
		Val(CString(szReturnString), g_struVariaveisGlobais.dwPortaDeComunicacao);
	else
		g_struVariaveisGlobais.dwPortaDeComunicacao = 0;

	// Lê o número de conexões simultãneas
	ZeroMemory(szReturnString, 255);
	if (GetPrivateProfileString(_T("REMOTO"), _T("Simultaneas"), _T("0"), szReturnString, 254, strConfigFile) > 0)
		Val(CString(szReturnString), g_struVariaveisGlobais.dwQuantidadeConexoesSimultaneas);
	else
		g_struVariaveisGlobais.dwQuantidadeConexoesSimultaneas = 0;

	// Lê o tempo sem resposta
	ZeroMemory(szReturnString, 255);
	if (GetPrivateProfileString(_T("REMOTO"), _T("TimeOut"), _T("1"), szReturnString, 254, strConfigFile) > 0)
		Val(CString(szReturnString), g_struVariaveisGlobais.dwTempoSemResposta);
	else
		g_struVariaveisGlobais.dwTempoSemResposta = 0;

	/////////////////////////////////////////////////////
	///
	/// FINAL CONFIGURAÇÕES PARA ACESSO REMOTO
	///
	/////////////////////////////////////////////////////

	return bRetorno;
}


///////////////////
HRESULT SendMQmsg(WCHAR* wszMQPathName, WCHAR *wszLabel, WCHAR *wszExtension, ULONG ulAppSpecific, WCHAR *wszBody)
{

	int cProps = 0;
	WCHAR wszPathFormatName[256];
	QUEUEHANDLE  hQueue;
	HRESULT res;
	ULONG lSizPath = sizeof(wszPathFormatName);
	MSGPROPID  aPropID[NUMBEROFPROPERTIES] ; 
	CString strMensagem;

	MQPROPVARIANT   aPropVar[NUMBEROFPROPERTIES];

	aPropID[cProps] = PROPID_M_LABEL;
	aPropVar[cProps].pwszVal = wszLabel;
	aPropVar[cProps].vt = VT_LPWSTR;
	cProps++;

	aPropID[cProps] = PROPID_M_BODY;
	aPropVar[cProps].caub.pElems  = (LPBYTE) wszBody;
	aPropVar[cProps].caub.cElems = wcslen(wszBody)*2;
	aPropVar[cProps].vt = VT_VECTOR | VT_UI1;
	cProps++;

	aPropID[cProps] = PROPID_M_EXTENSION;
	aPropVar[cProps].caub.pElems  = (LPBYTE) wszExtension;
	aPropVar[cProps].caub.cElems = wcslen(wszExtension)*2;
	aPropVar[cProps].vt = VT_VECTOR | VT_UI1;
	cProps++;

	aPropID[cProps] = PROPID_M_APPSPECIFIC;
	aPropVar[cProps].ulVal = ulAppSpecific;
	aPropVar[cProps].vt = VT_UI4;
	cProps++;

	MQMSGPROPS tMsgProps;
	tMsgProps.aPropID = aPropID;
	tMsgProps.aPropVar = aPropVar;
	tMsgProps.cProp = cProps;
	HRESULT aMsgStatus[NUMBEROFPROPERTIES];
	tMsgProps.aStatus = aMsgStatus;

  	res = MQPathNameToFormatName( wszMQPathName, wszPathFormatName, &lSizPath);
	
	if (FAILED(res))
	{
		CString strMensagem;
		long nTamanho = wcslen((WCHAR*)wszMQPathName)+1;

		strMensagem.Format(_T("Falhou ao tentar mapear a fila \"%s\", erro = 0x%x\n%s"), wszMQPathName, res, ErrorMessage(res));

		delete szMQPathName;

		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
	}

	res = MQOpenQueue(wszPathFormatName, MQ_SEND_ACCESS, MQ_DENY_NONE, &hQueue);

	if (FAILED(res))
	{
		CString strMensagem;

		strMensagem.Format(_T("Falha ao abrir a fila destino, erro = 0x%x\n%s"), res, ErrorMessage(res));
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
	}

	res = MQSendMessage( hQueue, &tMsgProps, NULL);

	if (FAILED(res))
	{
		DispErrDesc(res, _T("Roma"));
	}

	res = MQCloseQueue(hQueue);

	if (FAILED(res))
	{
		CString strMensagem;
		strMensagem.Format(_T("Falhou ao fechar a fila destino, erro = 0x%x\n%s"), res, ErrorMessage(res));
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
	}

	lSizPath++;

	return res;
}
////////////////////////////////////////////////
///////////////////////////////////////////
///////////////
HRESULT SendMQErrorMsg(WCHAR* wszMQPathName, MQMSGPROPS *tMsgProps)
{

	int cProps = 0;
	WCHAR wszPathFormatName[256];
	QUEUEHANDLE  hQueue;
	HRESULT res;
	ULONG lSizPath = sizeof(wszPathFormatName);

  	res = MQPathNameToFormatName( wszMQPathName, wszPathFormatName, &lSizPath);
	
	if (FAILED(res))
	{
		CString strMensagem;

		strMensagem.Format(_T("Falhou ao mapear a fila \"%s\", erro = 0x%x\n%s"), wszMQPathName, res, ErrorMessage(res));

		delete szMQPathName;

		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
	}

	res = MQOpenQueue(wszPathFormatName, MQ_SEND_ACCESS, MQ_DENY_NONE, &hQueue);

	if (FAILED(res))
	{
		CString strMensagem;

		strMensagem.Format(_T("Falha ao abrir a fila destino, erro = 0x%x\n%s"), res, ErrorMessage(res));
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
	}

	res = MQSendMessage( hQueue, tMsgProps, NULL);

	if (FAILED(res))
	{
		CString strMensagem;
		strMensagem.Format(_T("Falhou ao enviar mensagem para a fila destino, erro = 0x%x\n%s"), res, ErrorMessage(res));
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
	}

	res = MQCloseQueue(hQueue);

	if (FAILED(res))
	{
		CString strMensagem;
		strMensagem.Format(_T("Falhou ao fechar a fila destino, erro = 0x%x\n%s"), res, ErrorMessage(res));
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
	}

	lSizPath++;
	return res;
}
////////////////////////////////////////////////
///////////////////////////////////////
//////////// Instanciate the Obj and Call the Methode
/////////
HRESULT ObjInvoke(ROMAOBJCALL* obj)
{

	DISPPARAMS dps;
	VARIANTARG  VariantResult;
	VariantResult.bstrVal = NULL;
	EXCEPINFO ExcepInfo;

	VARIANTARG  *lpVarResult = &VariantResult;

	BSTR   txx = SysAllocString(obj->Argument);

	VARIANTARG vars[1];
	vars[0].bstrVal = txx;
	vars[0].vt = VT_BSTR;

	IDispatch FAR* pdisp = (IDispatch FAR*)NULL;
	LONG  error_num;

	//*** Sets the arguements array
	dps.rgvarg = vars; 
	dps.cArgs = 1;
	dps.cNamedArgs =0;
	dps.rgdispidNamedArgs = NULL;

	OLECHAR FAR* szMember = obj->Methode;

	GUID gg;
	HRESULT hresult=0;
	IUnknown FAR* punk = NULL;

	//*** Instancing the OBJ
	hresult = CLSIDFromProgID( obj->LibDotClass , &gg);
	DISPID dispid;
	try
	{
		hresult = CoCreateInstance(gg, NULL, CLSCTX_SERVER, IID_IUnknown, (void FAR* FAR*)&punk);
		if(!SUCCEEDED(hresult))
		{
			CString strMensagem;

			strMensagem.Format(_T("Falhou CoCreateInstance - \"%s\", erro = 0x%x\n%s"), CString(obj->LibDotClass), hresult, ErrorMessage(hresult));
			LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));

			throw hresult;
		}
		hresult = punk->QueryInterface(IID_IDispatch,
						(void FAR* FAR*)&pdisp);
		if(!SUCCEEDED(hresult))
		{
			CString strMensagem;

			strMensagem.Format(_T("Falhou QueryInterface - \"%s\", erro = 0x%x\n%s"), CString(obj->LibDotClass), hresult, ErrorMessage(hresult));
			LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));

			throw hresult;
		}

		hresult = pdisp->GetIDsOfNames(IID_NULL, &szMember, 1, LOCALE_USER_DEFAULT, &dispid);
		if(!SUCCEEDED(hresult))
		{
			CString strMensagem;

			strMensagem.Format(_T("Falhou GetIDsOfNames - \"%s.%s\", erro = 0x%x\n%s"), CString(obj->LibDotClass), CString(obj->Methode), hresult, ErrorMessage(hresult));

			LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));

			throw hresult;
		}

		//*** do call the Object's Methode 
		hresult = pdisp->Invoke(
				dispid,
				IID_NULL,
				LOCALE_USER_DEFAULT,
				DISPATCH_METHOD,
				&dps, lpVarResult, &ExcepInfo, NULL);

		if(!SUCCEEDED(hresult))
		{
			if(hresult ==DISP_E_EXCEPTION)
			{
				CString strMensagem;

				strMensagem.Format(_T("Falhou Invoke - \"%s.%s\", erro = 0x%x\n%s"), CString(obj->LibDotClass), CString(obj->Methode), hresult, ErrorMessage(hresult));
				LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
			}
			else
				DispErrDesc(GetLastError(), _T("Roma"));

 			error_num = GetLastError();

			{
				WCHAR* wszMQPathName;
				WCHAR wcsExtension[255];
				CString strErrorMessage;
				WCHAR wcsErrorMessage[1024];

				swprintf( wcsExtension, L"%s.%s", obj->LibDotClass, obj->Methode);

				error_num = ExcepInfo.wCode;

				strErrorMessage = CString(ExcepInfo.bstrDescription); //ErrorMessage(error_num);

				_tcscpy(wcsErrorMessage, strErrorMessage);

				WaitForSingleObject(g_hMutex, INFINITE);
				wszMQPathName = g_struVariaveisGlobais.wsErrorQueueName;
				ReleaseMutex(g_hMutex);

				SendMQmsg( wszMQPathName, wcsErrorMessage, wcsExtension , error_num, obj->Argument );
			}

			//throw error_num;
		}
		else
			if(lpVarResult->bstrVal  != NULL)
			{
			}
		obj->VarResult = lpVarResult;
	}
	catch(COleException &e)
	{ 
		error_num = e.m_sc;
		e.Delete();
		throw error_num;
	}
	catch(CException *e)
	{
		CString strMensagem;
		strMensagem.Format(_T("%s(%d): OLE Execption caught: SCODE = %x\n%s"), __FILE__, __LINE__, COleException::Process(e), ErrorMessage(GetLastError()));
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));

		e->Delete();

		error_num = GetLastError();
		throw error_num;
	}
	catch(LONG error_number)
	{
		WCHAR* wszMQPathName;
		WCHAR wcsExtension[255];
		CString strErrorMessage;
		WCHAR wcsErrorMessage[1024];

		error_num = error_number; 

		swprintf( wcsExtension, L"%s.%s", obj->LibDotClass, obj->Methode);

		strErrorMessage = ErrorMessage(error_num);

		_tcscpy(wcsErrorMessage, strErrorMessage);
		
		WaitForSingleObject(g_hMutex, INFINITE);
		wszMQPathName = g_struVariaveisGlobais.wsErrorQueueName;
		ReleaseMutex(g_hMutex);

		SendMQmsg( wszMQPathName, wcsErrorMessage, wcsExtension , error_num, obj->Argument );
	}

	//*** Normal exit from this routine
	if(pdisp != NULL)
		pdisp->Release(); 
	if(punk != NULL)
		punk->Release(); 

	WaitForSingleObject(g_hMutex, INFINITE);

	g_struVariaveisGlobais.bObjInvokeThreadActive[obj->ActivityIx] = FALSE;
 	g_struVariaveisGlobais.dwObjInstNum--;   

	SysFreeString(txx);
	
	HANDLE hHeap = GetProcessHeap();

	HeapFree(hHeap, NULL, obj->LibDotClass);
	HeapFree(hHeap, NULL, obj->Argument);

	if(lpVarResult->bstrVal != NULL)
		SysFreeString(lpVarResult->bstrVal); 

	HeapFree(hHeap, NULL, obj); 

	ReleaseMutex(g_hMutex);

	return hresult;
}
///////////////////////////////////////
/////// Thread Pool Arbitrator
/////// Gets a free Thread from the ThreadPool and starts 
/////// a new instance of a object with it
HANDLE ThreadArbitrator(ROMAOBJCALL* lpObj)
{
	int		nContador;
	DWORD	dwthID;
	HANDLE	hThrObj;

	EnterCriticalSection( &gpCriticalSection);

	// Somente MAX_OBJ_THREAD_COUNTER threads podem ser executadas simultaneamente.
	// Nesse loop procura um elemento da matriz que controla as threads, e que estja NULL, ou seja
	// livre para o uso de uma nova thread

	nContador = 0;
	do 
	{
		DWORD ExitCode=0;

		WaitForSingleObject(g_hMutex, INFINITE);
		if(g_struVariaveisGlobais.bObjInvokeThreadActive[nContador] == FALSE)
		{
			if (g_struVariaveisGlobais.hwndObjInvokeThread[nContador] != NULL)
				CloseHandle(g_struVariaveisGlobais.hwndObjInvokeThread[nContador]);

			g_struVariaveisGlobais.hwndObjInvokeThread[nContador] = NULL;
			g_struVariaveisGlobais.dwObjThreadID[nContador] = NULL;
			g_struVariaveisGlobais.bObjInvokeThreadActive[nContador] = TRUE;
			ReleaseMutex(g_hMutex);
			break;
		}
		ReleaseMutex(g_hMutex);

		Sleep(1);          //Wait 1 mSecs to avoid CPU lock up

		nContador++;

		if(nContador > (MAX_OBJ_THREAD_COUNTER-1))
			nContador = 0;

	}while(TRUE);

	///// Warning: Memory allocated in RouteObj should be deallocated in ObjInvoke
	hThrObj = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ObjInvoke, lpObj, 0 , &dwthID);

	if(hThrObj != NULL)
	{
		WaitForSingleObject(g_hMutex, INFINITE);
		g_struVariaveisGlobais.dwObjThreadID[nContador] = dwthID;
		g_struVariaveisGlobais.hwndObjInvokeThread[nContador] = hThrObj;
		g_struVariaveisGlobais.dwObjInstNum++;								//Quantidade de Intâncias ativa

		lpObj->ActivityIx = nContador;
		ReleaseMutex(g_hMutex);
	}

	LeaveCriticalSection( &gpCriticalSection);

	return hThrObj;
}

////////////////////////////////////////////
////////////////
HRESULT RouteObj(MQMSGPROPS * pmsgprops)
{
	HANDLE hHeap = GetProcessHeap();

	ROMAOBJCALL* obj = (ROMAOBJCALL*)HeapAlloc( hHeap, HEAP_ZERO_MEMORY, sizeof(ROMAOBJCALL));

	WCHAR*  extension = (WCHAR*)HeapAlloc( hHeap, HEAP_ZERO_MEMORY, pmsgprops->aPropVar[3].caub.cElems +1);

	memcpy( extension, pmsgprops->aPropVar[3].caub.pElems, pmsgprops->aPropVar[3].caub.cElems);

	//////////
	//The extension should come in a "Library.Class.Methode" form.
	WCHAR * FoundDot = wcsstr(extension , L"."); //jumps the first DOT
	try
	{
		if(FoundDot == NULL)
			throw(-1);

		FoundDot = wcsstr(++FoundDot , L".");       //finds the second DOT

		if(FoundDot == NULL)						//Replace the 2nd dot with '\0'
			throw(-1);
	}
	catch(...)
	{

		HeapFree( hHeap, 0, obj);
		HeapFree( hHeap, 0, extension);

		return -1;
	}

	*FoundDot = L'\0';

	obj->LibDotClass = extension;
	obj->Methode = ++FoundDot;					//The methode name follows the 2nd dot

	WCHAR* msgbody = (WCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, pmsgprops->aPropVar[0].caub.cElems +1);

	memcpy( msgbody, pmsgprops->aPropVar[0].caub.pElems, pmsgprops->aPropVar[0].caub.cElems);

	obj->Argument = msgbody;

	/////  these pointers allocated in RouteObj have to be deallocated in ObjInvoke
	if(FAILED(ThreadArbitrator(obj)))
	{
		//PRINTF_DEBUG("Arbitrator failed\n");
		HeapFree( hHeap, 0, extension);
		HeapFree( hHeap, 0, msgbody);

		HeapFree( hHeap, 0, obj);

		return -1;
	}
	return 0;
}



/////////////////////////////////////

//////////////////////////////////
// Receive MQmsg syncronously
//////////////
HRESULT  MQSyncRcv( void* MQPathName)
{ 
	HRESULT hr;
	QUEUEHANDLE hQueue = 0;
	WCHAR szwFormatNameBuffer[256];

	WaitForSingleObject(g_hMutex, INFINITE);
	DWORD dwTimeOutMSMQ = g_struVariaveisGlobais.dwFilaTimeOut;
	ReleaseMutex(g_hMutex);

	DWORD dwDestFormatNameBufferLength = sizeof(szwFormatNameBuffer);

	hr = MQPathNameToFormatName((WCHAR*)MQPathName,
                              szwFormatNameBuffer,
                              &dwDestFormatNameBufferLength);

	hr = MQOpenQueue(
		 szwFormatNameBuffer,						// Format Name of the queue to be opened
		 MQ_RECEIVE_ACCESS,							// Access rights to the Queue
		 0,											// No receive Exclusive
		 &hQueue									// OUT: handle to the opened Queue
	);
 
	if (FAILED(hr))
	{
		CString strMensagem, strErro;
		long nTamanhoErro;
		int nIndice;

		WaitForSingleObject(g_hMutex, INFINITE);
		for (nIndice = 0; nIndice < g_struVariaveisGlobais.dwTotalFilas; nIndice++)
		{
			if (MQPathName == g_struVariaveisGlobais.wsaQueueNames[nIndice])
				break;
		}

		strErro = ErrorMessage(hr);

		nTamanhoErro = strErro.GetLength() + 1;
		strMensagem.Format(_T("Erro ao abrir a fila \"%s\"\n%s"), MQPathName, strErro);

		g_struVariaveisGlobais.szQueueError[nIndice] = new TCHAR[nTamanhoErro];

		_tcscpy(g_struVariaveisGlobais.szQueueError[nIndice], strErro);

		ReleaseMutex(g_hMutex);

		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));

		WaitForSingleObject(g_hMutex, INFINITE);
		g_struVariaveisGlobais.dwTotalTentativasDeAbertura++;
		ReleaseMutex(g_hMutex);

		return hr;
	}

	WaitForSingleObject(g_hMutex, INFINITE);
	g_struVariaveisGlobais.dwTotalTentativasDeAbertura++;
	g_struVariaveisGlobais.dwTotalFilasAbertas++;
	ReleaseMutex(g_hMutex);
 
	MQMSGPROPS * pmsgprops;
	MQPROPVARIANT *paVariant;
	MSGPROPID * paPropId;
	DWORD dwcPropId = 0;
	WCHAR *wszLabelBuffer; 
	WCHAR *wszBodyBuffer;
	UCHAR* wszExtentionBuffer;
	HANDLE hHeap;
	BOOL bProgramFinishRequest = FALSE;

	do
	{
		//  The output parameters to an asynchronous call to MQReceiveMessage 
		//  should be kept intact until the operation completes, you should 
		//  not free or reuse them until the operation is complete.
		//

		hHeap = GetProcessHeap ();
		pmsgprops = (MQMSGPROPS *)HeapAlloc (hHeap, HEAP_ZERO_MEMORY, sizeof(MQMSGPROPS));

		paVariant = (MQPROPVARIANT*)HeapAlloc (hHeap, HEAP_ZERO_MEMORY,sizeof(MQPROPVARIANT)*10);

		paPropId = (MSGPROPID*)HeapAlloc (hHeap, HEAP_ZERO_MEMORY,sizeof(MSGPROPID)*10);

		wszLabelBuffer = (WCHAR*)HeapAlloc (hHeap, HEAP_ZERO_MEMORY,sizeof(WCHAR)*256);

		wszBodyBuffer = (WCHAR*)HeapAlloc (hHeap, HEAP_ZERO_MEMORY,sizeof(WCHAR)*MSG_BODY_LEN);

		wszExtentionBuffer = (UCHAR*)HeapAlloc (hHeap, HEAP_ZERO_MEMORY,sizeof(UCHAR)*MSG_EXT_LEN);
    
		//////////////////////////////////////////////////
		// Prepare the message properties to be retrieved.
		/////////////////////////////////////////////////
 
		dwcPropId = 0;

		// Set the PROPID_M_BODY property.
		paPropId[dwcPropId] = PROPID_M_BODY;                //PropId
		paVariant[dwcPropId].vt = VT_VECTOR|VT_UI1;         //Type
		paVariant[dwcPropId].caub.pElems = (UCHAR*)wszBodyBuffer;
		paVariant[dwcPropId].caub.cElems = MSG_BODY_LEN ;    //Value
 
		dwcPropId++;
	
		paPropId[dwcPropId] = PROPID_M_LABEL;           // Property ID
		paVariant[dwcPropId].vt = VT_LPWSTR;            // Type indicator
		paVariant[dwcPropId].pwszVal = wszLabelBuffer;  // Value
		dwcPropId++;

		paPropId[dwcPropId] = PROPID_M_LABEL_LEN;       // Property ID
		paVariant[dwcPropId].vt=VT_UI4;                 // Type indicator
		paVariant[dwcPropId].ulVal= 256;                // Value
		dwcPropId++;

		paPropId[dwcPropId] = PROPID_M_EXTENSION;       // Property ID
		paVariant[dwcPropId].vt=VT_VECTOR|VT_UI1;       // Type indicator
		paVariant[dwcPropId].caub.cElems = MSG_EXT_LEN; // Value
		paVariant[dwcPropId].caub.pElems =wszExtentionBuffer;
		dwcPropId++;

		paPropId[dwcPropId] = PROPID_M_EXTENSION_LEN;       // Property ID
		paVariant[dwcPropId].vt=VT_UI4;						// Type indicator
		paVariant[dwcPropId].ulVal= 256;					// Value
		dwcPropId++;

		paPropId[dwcPropId] = PROPID_M_BODY_SIZE;			// Property ID
		paVariant[dwcPropId].vt=VT_UI4;						// Type indicator
		dwcPropId++;

		paPropId[dwcPropId] = PROPID_M_BODY_TYPE;       // Property ID
		paVariant[dwcPropId].vt=VT_UI4;                 // Type indicator
		dwcPropId++;


		////////////////////////////////
		// Initialize the MQMSGPROPS structure
		///////////////////////////////
		pmsgprops->cProp = dwcPropId;			//Number of properties
		pmsgprops->aPropID = paPropId;			//Ids of properties
		pmsgprops->aPropVar = paVariant;		//Values of properties
		pmsgprops->aStatus  = NULL;				//No Error report
 
		///////////////////////////////////////////////
		hr = MQReceiveMessage(
			hQueue,                     // Handle to the Queue
			dwTimeOutMSMQ,    			// Max time (msec) to wait
			MQ_ACTION_RECEIVE,          // Action
			pmsgprops,                  // Properties to retrieve
			NULL,                       // No OVERLAPPED structure
			NULL,						// Callback function
			NULL,                       // No Cursor
			NULL                        // No transaction
				  );

		if (hr != MQ_ERROR_IO_TIMEOUT && FAILED(hr) )   // Check error other than Timeout
		{
			DispErrDesc(hr, _T("Roma"));
		}

		// Process message.
		if(hr!=MQ_ERROR_IO_TIMEOUT)
		{
			if(RouteObj(pmsgprops))
			{
				paPropId[1] = PROPID_M_LABEL;					// Property ID
				paVariant[1].vt = VT_LPWSTR;					// Type indicator
				paVariant[1].pwszVal = L"Conteúdo inválido";	// Value

				WaitForSingleObject(g_hMutex, INFINITE);
				WCHAR * wsErrorQueueName = g_struVariaveisGlobais.wsErrorQueueName;
				ReleaseMutex(g_hMutex);

				SendMQErrorMsg( wsErrorQueueName, pmsgprops);
			}
		}

		HeapFree(hHeap, 0, pmsgprops);
		HeapFree(hHeap, 0, paVariant);
		HeapFree(hHeap, 0, paPropId);
		HeapFree(hHeap, 0, wszLabelBuffer);
		HeapFree(hHeap, 0, wszBodyBuffer);
		HeapFree(hHeap, 0, wszExtentionBuffer);

		WaitForSingleObject(g_hMutex, INFINITE);
		bProgramFinishRequest = g_struVariaveisGlobais.bProgramFinishRequest;
		ReleaseMutex(g_hMutex);

	}while(!bProgramFinishRequest);
	

	hr = MQCloseQueue(hQueue);
	
	return MQ_OK;
}
////////////////////////////////////////////

/// Main ENTRY  ////////

HRESULT ObjMQRoute( HANDLE *hEvStop)
{
	DWORD thId;
	int nContador = 0, keyCode = 0;
	DWORD nLoops = 0;
	BOOL bThdStillActive = FALSE;

	// Verifica se o aquivo de configuração existe, caso não exista encerra o serviço!
	if (!LerConfiguracoes())
	{
		CServerRomaConfig dlg;

		dlg._Module = &_Module;

		dlg.DoModal();

		_Module.SetServiceStatus(SERVICE_STOP_PENDING);
		PostThreadMessage(_Module.dwThreadID, WM_QUIT, 0, 0);
	}

	// Se deve iniciar Servidor 
	WaitForSingleObject(g_hMutex, INFINITE);
	if (g_struVariaveisGlobais.bIniciarServico)
	{
		if(!g_struVariaveisGlobais.g_Server.StartListen(g_struVariaveisGlobais.dwPortaDeComunicacao, GetSocketConnection))
		{
			CString strPrompt;

			strPrompt.Format(_T("Erro ao iniciar o Servidor do Servico!\n\n%s"), g_struVariaveisGlobais.g_Server.WaCGetLastErrorText());
			LogEvent(strPrompt, EVENTLOG_ERROR_TYPE, _T("Roma"));
		}

		g_struVariaveisGlobais.g_Connections = new EnumConnections[g_struVariaveisGlobais.dwQuantidadeConexoesSimultaneas];
	}
	ReleaseMutex(g_hMutex);

	WaitForSingleObject(g_hMutex, INFINITE);
	DWORD dwTimeOut = g_struVariaveisGlobais.dwServicoTimeOut;
	ReleaseMutex(g_hMutex);

	InitializeCriticalSection( &gpCriticalSection);
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	WaitForSingleObject(g_hMutex, INFINITE);
	g_struVariaveisGlobais.hEvSynObj = CreateEvent(NULL, TRUE, FALSE, NULL);
	ReleaseMutex(g_hMutex);

	WaitForSingleObject(g_hMutex, INFINITE);
	for(nContador = 0; nContador < (int)g_struVariaveisGlobais.dwTotalFilas ; nContador++)
	{
		if (g_struVariaveisGlobais.wsaQueueNames[nContador] != NULL)
		{
			WCHAR * wsaQueueNames = g_struVariaveisGlobais.wsaQueueNames[nContador];

			g_struVariaveisGlobais.szQueueError[nContador] = NULL;

			g_struVariaveisGlobais.hObjQueueThread[nContador] = CreateThread(NULL,NULL, (LPTHREAD_START_ROUTINE)MQSyncRcv, (void*)wsaQueueNames, 0, &thId);
		}
	}
	ReleaseMutex(g_hMutex);

	//Wait all the MQRx threads to finish
	do
	{	//***  if EvStop is Signed then procede Stopping
		//*** else after 2 secs procede cleaning Thread flags
		if(WaitForSingleObject( *hEvStop, 2000) == WAIT_OBJECT_0)
			break;

		WaitForSingleObject(g_hMutex, INFINITE);
		if (g_struVariaveisGlobais.dwTotalFilasAbertas == 0 && g_struVariaveisGlobais.dwTotalTentativasDeAbertura == g_struVariaveisGlobais.dwTotalFilas)
		{
			LogEvent(_T("Nenhuma fila foi aberta!"), EVENTLOG_ERROR_TYPE, _T("Roma"));
			_Module.SetServiceStatus(SERVICE_STOP_PENDING);
			PostThreadMessage(_Module.dwThreadID, WM_QUIT, 0, 0);
		}
		ReleaseMutex(g_hMutex);

		WaitForSingleObject(g_hMutex, INFINITE);
		for(nContador = 0; nContador < MAX_OBJ_THREAD_COUNTER; nContador++)
		{
			DWORD ExitCode = 0;

			if((g_struVariaveisGlobais.hwndObjInvokeThread[nContador] != NULL) && (g_struVariaveisGlobais.bObjInvokeThreadActive[nContador] == FALSE)) 
			{
				HRESULT ret = GetExitCodeThread(g_struVariaveisGlobais.hwndObjInvokeThread[nContador], &ExitCode);	

				if(ret)
				{
					if((ExitCode != STILL_ACTIVE))
					{
						CloseHandle(g_struVariaveisGlobais.hwndObjInvokeThread[nContador]);
						g_struVariaveisGlobais.hwndObjInvokeThread[nContador] = NULL;
					}
				}
			}
		}
		ReleaseMutex(g_hMutex);

	}while(TRUE);

	WaitForSingleObject(g_hMutex, INFINITE);
	g_struVariaveisGlobais.bProgramFinishRequest = TRUE;
	ReleaseMutex(g_hMutex);

	// Se iniciou o servidor dever pará-lo aqui
	if (g_struVariaveisGlobais.bIniciarServico)
	{
		if(!g_struVariaveisGlobais.g_Server.StopListen())
		{
			CString strPrompt;

			strPrompt.Format(_T("Erro ao iniciar o Servidor do Servico!\n\n%s"), g_struVariaveisGlobais.g_Server.WaCGetLastErrorText());
			LogEvent(strPrompt, EVENTLOG_ERROR_TYPE, _T("Roma"));
		}

		delete g_struVariaveisGlobais.g_Connections;
	}

	// Aguarda até que as Threads de componentes finalizem suas atividades
	nLoops = 0;
	bThdStillActive = FALSE;
	do 
	{
		DWORD ExitCode = 0;
		for(nContador = 0; nContador < MAX_OBJ_THREAD_COUNTER; nContador++)
		{
			WaitForSingleObject(g_hMutex, INFINITE);
			BOOL bThreadAtiva = g_struVariaveisGlobais.bObjInvokeThreadActive[nContador];
			ReleaseMutex(g_hMutex);

			if (bThreadAtiva)
			{
				WaitForSingleObject(g_hMutex, INFINITE);
				HRESULT ret = GetExitCodeThread(g_struVariaveisGlobais.hwndObjInvokeThread[nContador], &ExitCode);	
				ReleaseMutex(g_hMutex);

				if(SUCCEEDED(ret))
					if(ExitCode == STILL_ACTIVE)
						bThdStillActive = TRUE;
					else
					{
						WaitForSingleObject(g_hMutex, INFINITE);
						CloseHandle(g_struVariaveisGlobais.hwndObjInvokeThread[nContador]);
						g_struVariaveisGlobais.bObjInvokeThreadActive[nContador] = FALSE;
						ReleaseMutex(g_hMutex);

						bThdStillActive = FALSE;
					}
				else
					bThdStillActive = FALSE;
			}
		}
		Sleep(1000);
	}while((++nLoops < (dwTimeOut)) && bThdStillActive);

	// Se nLoops for >= g_struVariaveisGlobais.nFilaTimeOut, então saiu por TimeOut
	if (nLoops >= dwTimeOut)
	{
		CString strMensagem;

		strMensagem.Format(_T("Nem todas as Threads que invocam componentes foram finalizadas com sucesso\ne foram finalizadas por TIMEOUT\nTempo de Espera: %d\nTempo Configurado:%d"), nLoops, dwTimeOut);

		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
	}

	// Aguarda até que as Threads que verificam filas finalizem suas atividades
	nLoops = 0;
	bThdStillActive = FALSE;
	do
	{
		WaitForSingleObject(g_hMutex, INFINITE);
		int nTotalFilas = g_struVariaveisGlobais.dwTotalFilas;
		ReleaseMutex(g_hMutex);

		DWORD ExitCode = 0;

		for(nContador = 0; nContador < nTotalFilas; nContador++)
		{
			HRESULT ret;
			
			WaitForSingleObject(g_hMutex, INFINITE);
			HANDLE hrThread = g_struVariaveisGlobais.hObjQueueThread[nContador];
			ReleaseMutex(g_hMutex);

			if (hrThread != NULL)
			{
				WaitForSingleObject(g_hMutex, INFINITE);
				ret = GetExitCodeThread(g_struVariaveisGlobais.hObjQueueThread[nContador], &ExitCode);
				ReleaseMutex(g_hMutex);

				if(SUCCEEDED(ret))
					if(ExitCode == STILL_ACTIVE)
						bThdStillActive = TRUE;
					else
					{
						WaitForSingleObject(g_hMutex, INFINITE);
						CloseHandle(g_struVariaveisGlobais.hObjQueueThread[nContador]);
						g_struVariaveisGlobais.hObjQueueThread[nContador] = NULL;
						bThdStillActive = FALSE;
						ReleaseMutex(g_hMutex);
					}
				else
					bThdStillActive = FALSE;
			}
			else
				bThdStillActive = FALSE;

		}
		Sleep(1000);

	}while((++nLoops < ( dwTimeOut)) && bThdStillActive);

	// Se nLoops for >= g_struVariaveisGlobais.nFilaTimeOut, então saiu por TimeOut
	if (nLoops >= ( dwTimeOut))
	{
		CString strMensagem;

		strMensagem.Format(_T("Nem todas as Threads que verificam Filas foram finalizadas com sucesso\ne foram finalizadas por TIMEOUT\nTempo de Espera: %d\nTempo de Configuração: %d"), nLoops, dwTimeOut);

		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
	}

	// Libera os nomes das filas que foram alocadas
	HANDLE hHeap = GetProcessHeap();
	for (nContador = 0; nContador < (int)g_struVariaveisGlobais.dwTotalFilas; nContador++)
	{
		if (g_struVariaveisGlobais.wsaQueueNames[nContador] != NULL)
			HeapFree(hHeap, NULL, g_struVariaveisGlobais.wsaQueueNames[nContador]);

		if (g_struVariaveisGlobais.szQueueError[nContador] != NULL)
			delete g_struVariaveisGlobais.szQueueError[nContador];
	}

	DeleteCriticalSection(&gpCriticalSection);
	CoUninitialize();

	return 0;   
}

void GetSocketConnection(void * pParam)
{
	AcceptedConnectionParams*	pParams;
	short						nPos;

	nPos = -1;

	// Este ponteiro é destruído no final da execução da Thread posteriormente executada
	pParams = new AcceptedConnectionParams;

	CopyMemory(pParams, pParam, sizeof(AcceptedConnectionParams));

	WaitForSingleObject(g_hMutex, INFINITE);
	if(g_struVariaveisGlobais.g_nConnectionCount == g_struVariaveisGlobais.dwQuantidadeConexoesSimultaneas)
	{
		LogEvent("O número máximo de conexões foi atingido", EVENTLOG_ERROR_TYPE, _T("Roma"));
		return;
	}
	ReleaseMutex(g_hMutex);

	for(short n = 0; n < g_struVariaveisGlobais.dwQuantidadeConexoesSimultaneas; n++)
	{
		if(!g_struVariaveisGlobais.g_Connections[n].bUsed)
		{
			nPos = n;
			break;
		}
	}

	pParams->nConnectionNumber = nPos;

	g_struVariaveisGlobais.g_Connections[nPos].hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadSyncSocket, (LPVOID) pParams, CREATE_SUSPENDED, &g_struVariaveisGlobais.g_Connections[nPos].dwThreadID);
	g_struVariaveisGlobais.g_Connections[nPos].tmLastResponse = time(&g_struVariaveisGlobais.g_Connections[nPos].tmLastResponse);

	ResumeThread(g_struVariaveisGlobais.g_Connections[nPos].hThread);
	g_struVariaveisGlobais.g_Connections[nPos].bUsed = true;
	g_struVariaveisGlobais.g_nConnectionCount++;
}

int Receive(const SOCKET Socket, void * pBuffer, const int nBufferSize, unsigned long * plBytes)
{
	int nRes = recv(Socket, (char*) pBuffer, nBufferSize, 0);

	if(nRes > 0)
		*plBytes += nRes;

	return nRes;
}

int Send(const SOCKET Socket, void * pBuffer, const int nBufferSize, unsigned long * plBytes)
{
	int nRes = send(Socket, (char*) pBuffer, nBufferSize, 0);

	if(nRes > 0)
		*plBytes += nRes;

	return nRes;
}


void ThreadSyncSocket(LPVOID pParam)
{
	AcceptedConnectionParams*	pParams = (AcceptedConnectionParams*) pParam;
	CMSSocket					RomaSocket(pParams->Socket, pParams->nConnectionNumber);
	short						nConnectioNumber = pParams->nConnectionNumber;
				
	int							nState; 
	int							nSocketAction; 

	TCHAR						szBuffer[MAX_BUFFER];

	int							nBufferCount;

	BOOL						bLoop;

	int							nRepetidor;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	VALIDAÇÃO DE USUÁRIO		///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	COMUNICAÇÃO					///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////

	nBufferCount = 0;
	nState = COMANDO_IDLE;
	nSocketAction = SOCKET_READ_STATE;
	bLoop =	TRUE;
	nRepetidor = 0;

	while (bLoop) 
	{

		// Envia/Recebe informações da rede
		switch (nSocketAction) 
		{
			case SOCKET_WRITE_STATE:
				if(!RomaSocket.Write(szBuffer, nBufferCount))
					goto SocketError;

				nSocketAction = SOCKET_READ_STATE;
				nBufferCount = 0;
				nState = COMANDO_IDLE;

				break;

			case SOCKET_READ_STATE:
				nBufferCount = RomaSocket.ReadEx(szBuffer, SOCKET_BUFFER_SIZE);
				if (nBufferCount == -1 )
				{	
					nRepetidor++;

					if (nRepetidor == 20) 
						goto SocketError;
				}
				break;
		}


		nSocketAction = SOCKET_IDLE;

		if (nBufferCount > 0)
			nState = (szBuffer[2] * 256) + szBuffer[3];

		// Funções que deverão ser tratadas
		switch (nState)
		{
			case COMANDO_TOTAL_THREADS_ATIVAS:
				{
					int nContador = 0;				

					WaitForSingleObject(g_hMutex, INFINITE);
					for (int n = 0; n < MAX_OBJ_THREAD_COUNTER; n++)
					{
						if(g_struVariaveisGlobais.bObjInvokeThreadActive[n])
							nContador++;

						Sleep(1);
					}
					ReleaseMutex(g_hMutex);

					memset(szBuffer, 0, MAX_BUFFER);

					szBuffer[0] = (4 / 256); 
					szBuffer[1] = (4 % 256); 

					szBuffer[2] = (nContador / 256);
					szBuffer[3] = (nContador % 256);

					nBufferCount = 4;
					nSocketAction = SOCKET_WRITE_STATE;
				}

				nRepetidor = 0;
				break;

			case COMANDO_TOTAL_FILAS:
				{
					memset(szBuffer, 0, MAX_BUFFER);

					WaitForSingleObject(g_hMutex, INFINITE);
					szBuffer[0] = (4 / 256); 
					szBuffer[1] = (4 % 256); 
					szBuffer[2] = (g_struVariaveisGlobais.dwTotalFilas / 256);
					szBuffer[3] = (g_struVariaveisGlobais.dwTotalFilas % 256);
					ReleaseMutex(g_hMutex);

					nBufferCount = 4;
					nSocketAction = SOCKET_WRITE_STATE;
				}

				nRepetidor = 0;
				break;

			case COMANDO_TOTAL_FILAS_ABERTAS:
				{
					memset(szBuffer, 0, MAX_BUFFER);

					WaitForSingleObject(g_hMutex, INFINITE);
					szBuffer[0] = (4 / 256); 
					szBuffer[1] = (4 % 256); 
					szBuffer[2] = (g_struVariaveisGlobais.dwTotalFilasAbertas / 256);
					szBuffer[3] = (g_struVariaveisGlobais.dwTotalFilasAbertas % 256);
					ReleaseMutex(g_hMutex);

					nBufferCount = 4;
					nSocketAction = SOCKET_WRITE_STATE;
				}

				nRepetidor = 0;
				break;
			case COMANDO_NOME_FILA:
				{
					HRESULT ret;
					DWORD ExitCode = 0;
					TCHAR szMensagemErro[256];
					int nTamanho, nTamanhoErro;
					int nIndice;
					BOOL bThdStillActive;

					nIndice = ( szBuffer[4] * 256 ) + szBuffer[5];
					
					// Verifica se a abertura foi bem sucedida					
					WaitForSingleObject(g_hMutex, INFINITE);

					HANDLE hrThread = g_struVariaveisGlobais.hObjQueueThread[nIndice];

					if (hrThread != NULL)
					{
						ret = GetExitCodeThread(g_struVariaveisGlobais.hObjQueueThread[nIndice], &ExitCode);

						if(SUCCEEDED(ret))
						{
							if(ExitCode == STILL_ACTIVE)
								bThdStillActive = TRUE;
							else
								bThdStillActive = FALSE;
						}
						else
							bThdStillActive = FALSE;
					}
					else
						bThdStillActive = FALSE;
					

					if (!bThdStillActive)
					{
						_tcscpy(szMensagemErro, g_struVariaveisGlobais.szQueueError[nIndice]);
					}
					else
					{
						nTamanhoErro = 0;
						memset(&szMensagemErro, NULL, 256);
					}

					ReleaseMutex(g_hMutex);

					//Copia os dados para o Buffer
					memset(szBuffer, 0, MAX_BUFFER);

					szBuffer[0] = (nTamanho + 6 + nTamanhoErro) / 256;
					szBuffer[1] = (nTamanho + 6 + nTamanhoErro) % 256;

					szBuffer[2] = (bThdStillActive) / 256;
					szBuffer[3] = (bThdStillActive) % 256;

					szBuffer[4] = (nTamanho) / 256;
					szBuffer[5] = (nTamanho) % 256;

					memcpy(&szBuffer[6], szMQPathName, nTamanho);

					szBuffer[nTamanho + 6] = (nTamanhoErro) / 256;
					szBuffer[nTamanho + 7] = (nTamanhoErro) % 256;

					memcpy(&szBuffer[nTamanho + 8], szMensagemErro, nTamanhoErro);

					nSocketAction = SOCKET_WRITE_STATE;

					nBufferCount = nTamanho + 6 + nTamanhoErro;
				}

				nRepetidor = 0;
				break;
			case COMANDO_NOME_FILA_ERRO:
				memset(szBuffer, 0, MAX_BUFFER);

				nRepetidor = 0;

				nSocketAction = SOCKET_WRITE_STATE;

				break;

			case COMANDO_PING:
				memset(szBuffer, 0, MAX_BUFFER);

				nRepetidor = 0;

				nSocketAction = SOCKET_READ_STATE;

				break;
			case COMANDO_FINALIZAR:
				bLoop = FALSE;

				nSocketAction = SOCKET_WRITE_STATE;

				nRepetidor = 0;
				break;
			case COMANDO_IDLE:
				nSocketAction = SOCKET_READ_STATE;
				break;
		}

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	FINAL DA CONEXÃO		///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	RomaSocket.Close();

	WaitForSingleObject(g_hMutex, INFINITE);
	delete pParams;
	ReleaseMutex(g_hMutex);

	ResetConnection(nConnectioNumber);

	ExitThread(0);

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//	ERRO DA CONEXÃO	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////

	SocketError:
		RomaSocket.Close();

		WaitForSingleObject(g_hMutex, INFINITE);
		delete pParams;
		ReleaseMutex(g_hMutex);

		ResetConnection(nConnectioNumber);
		
		WaitForSingleObject(g_hMutex, INFINITE);

		ReleaseMutex(g_hMutex);

		ExitThread(2);

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
}


void ResetConnection(short nConnectionNumber)
{
	WaitForSingleObject(g_hMutex, INFINITE);

	g_struVariaveisGlobais.g_Connections[nConnectionNumber].bUsed			= false;
	g_struVariaveisGlobais.g_Connections[nConnectionNumber].tmLastResponse = 0;
	g_struVariaveisGlobais.g_Connections[nConnectionNumber].hThread		= NULL;
	g_struVariaveisGlobais.g_Connections[nConnectionNumber].dwThreadID		= -1;
	g_struVariaveisGlobais.g_Connections[nConnectionNumber].lBytesReceived = 0;
	g_struVariaveisGlobais.g_Connections[nConnectionNumber].lBytesSent		= 0;

	g_struVariaveisGlobais.g_nConnectionCount--;

	ReleaseMutex(g_hMutex);
}
