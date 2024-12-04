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
#include "ReadConfig.h"

#include "Library.h"

#include "Estruturas.h"

#define NUMBEROFPROPERTIES 4

extern HANDLE	g_hMutex;

CRITICAL_SECTION  gpCriticalSection;

STRU_VARIAVEIS_GLOBAIS g_struVariaveisGlobais;
////////////////////////////////////////////
////////// Simple WSTR to STR convertion
void OletoStr(BSTR in, char *out)
{
	while(*out++=(char)*in++);
}
////////////////////////

void StrZToWCSZ( char *in, WCHAR* out)
{
	while(*out++ =(WCHAR) *in++);
}


//////////////////////////////////////
/// Dispara metodos no ActiveX via interface Idispatch
//////////////////////////////////////
#include <Resapi.h>

static int gsux=0;

///////////////////
HRESULT SendMQmsg(WCHAR* wszMQPathName, WCHAR *wszLabel, WCHAR *wszExtension, LONG ulAppSpecific, WCHAR *wszBody)
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
		char *szMQPathName;
		long nTamanho = wcslen((WCHAR*)wszMQPathName)+1;

		szMQPathName = new char[nTamanho];
		memset(szMQPathName, 0, nTamanho);

		WideCharToMultiByte(CP_ACP, 0, wszMQPathName, -1, szMQPathName, nTamanho, NULL, NULL);

		strMensagem.Format(_T("Falhou ao tentar mapear a fila \"%s\", erro = 0x%x\n"), szMQPathName, res);

		delete szMQPathName;

		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
		DispErrDesc(res, _T("Roma"));
	}

	res = MQOpenQueue(wszPathFormatName, MQ_SEND_ACCESS, MQ_DENY_NONE, &hQueue);

	if (FAILED(res))
	{
		CString strMensagem;

		strMensagem.Format(_T("Falha ao abrir a fila destino, erro = 0x%x\n"), res);
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
		DispErrDesc(res, _T("Roma"));
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
		strMensagem.Format(_T("Falhou ao fechar a fila destino, erro = 0x%x\n"), res);
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
		char *szMQPathName;
		long nTamanho = wcslen((WCHAR*)wszMQPathName)+1;

		szMQPathName = new char[nTamanho];
		memset(szMQPathName, 0, nTamanho);

		WideCharToMultiByte(CP_ACP, 0, wszMQPathName, -1, szMQPathName, nTamanho, NULL, NULL);

		strMensagem.Format(_T("Falhou ao mapear a fila \"%s\", erro = 0x%x\n"), szMQPathName, res);

		delete szMQPathName;

		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
		DispErrDesc(res, _T("Roma"));
	}

	res = MQOpenQueue(wszPathFormatName, MQ_SEND_ACCESS, MQ_DENY_NONE, &hQueue);

	if (FAILED(res))
	{
		CString strMensagem;

		strMensagem.Format(_T("Falha ao abrir a fila destino, erro = 0x%x\n"), res);
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
		DispErrDesc(res, _T("Roma"));
	}

	res = MQSendMessage( hQueue, tMsgProps, NULL);

	if (FAILED(res))
	{
		DispErrDesc(res, _T("Roma"));
	}

	res = MQCloseQueue(hQueue);

	if (FAILED(res))
	{
		CString strMensagem;
		strMensagem.Format(_T("Falhou ao fechar a fila destino, erro = 0x%x\n"), res);
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
		DispErrDesc(res, _T("Roma"));
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
	WaitForSingleObject(g_hMutex, INFINITE);
	g_struVariaveisGlobais.caObjThreadActive[obj->ActivityIx] = TRUE;
	ReleaseMutex(g_hMutex);

	DISPPARAMS dps;
	VARIANTARG  VariantResult;
	VariantResult.bstrVal = NULL;
	VARIANTARG  *lpVarResult = &VariantResult;

	//*** ATENTION this array displacement order is reversed
	//***  in relation the original order of methode arguments list

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

	//	hresult = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	//*** Instancing the OBJ
	hresult = CLSIDFromProgID( obj->LibDotClass , &gg);
	DISPID dispid;
	try
	{
		hresult = CoCreateInstance(gg, NULL, CLSCTX_SERVER, IID_IUnknown, (void FAR* FAR*)&punk);
		if(!SUCCEEDED(hresult))
		{
			CString strMensagem;
			char szClass[255];

			int nTamanho = wcslen(obj->LibDotClass) + 1;

			WideCharToMultiByte ( CP_ACP, 0, obj->LibDotClass, -1, szClass, nTamanho, NULL, NULL );

			strMensagem.Format(_T("Falhou CoCreateInstance - \"%s\", erro = 0x%x\n"), CString(szClass), hresult);
			LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
			DispErrDesc(hresult, _T("Roma"));

			throw hresult;
		}
		hresult = punk->QueryInterface(IID_IDispatch,
						(void FAR* FAR*)&pdisp);
		if(!SUCCEEDED(hresult))
		{
			CString strMensagem;

			char szClass[255];

			int nTamanho = wcslen(obj->LibDotClass) + 1;

			WideCharToMultiByte ( CP_ACP, 0, obj->LibDotClass, -1, szClass, nTamanho, NULL, NULL );

			strMensagem.Format(_T("Falhou QueryInterface - \"%s\", erro = 0x%x\n"), CString(szClass), hresult);
			LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
			DispErrDesc(hresult, _T("Roma"));

			throw hresult;
		}

		hresult = pdisp->GetIDsOfNames(IID_NULL, &szMember, 1, LOCALE_USER_DEFAULT, &dispid);
		if(!SUCCEEDED(hresult))
		{
			CString strMensagem;
			char szClass[255], szMethod[255];

			int nTamanho = wcslen(obj->LibDotClass) + 1;
			WideCharToMultiByte ( CP_ACP, 0, obj->LibDotClass, -1, szClass, nTamanho, NULL, NULL );

			nTamanho = wcslen(obj->Methode) + 1;
			WideCharToMultiByte ( CP_ACP, 0, obj->Methode, -1, szMethod, nTamanho, NULL, NULL );

			strMensagem.Format(_T("Falhou GetIDsOfNames - \"%s.%s\", erro = 0x%x\n"), CString(szClass), CString(szMethod), hresult);

			LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
			DispErrDesc(hresult, _T("Roma"));

			throw hresult;
		}
		EXCEPINFO ExcepInfo;

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
				char szClass[255], szMethod[255];

				int nTamanho = wcslen(obj->LibDotClass) + 1;
				WideCharToMultiByte ( CP_ACP, 0, obj->LibDotClass, -1, szClass, nTamanho, NULL, NULL );

				nTamanho = wcslen(obj->Methode) + 1;
				WideCharToMultiByte ( CP_ACP, 0, obj->Methode, -1, szMethod, nTamanho, NULL, NULL );

				strMensagem.Format(_T("Falhou Invoke - \"%s.%s\", erro = 0x%x\n"), CString(szClass), CString(szMethod), hresult);
				LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
				DispErrDesc(hresult, _T("Roma"));
			}
			DispErrDesc(GetLastError(), _T("Roma"));

 			error_num = GetLastError();
			throw error_num;
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
		strMensagem.Format(_T("%s(%d): OLE Execption caught: SCODE = %x\n"), __FILE__, __LINE__, COleException::Process(e));
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
		DispErrDesc(GetLastError(), _T("Roma"));

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

		char szErrorMessage[1024];
		int nTamanho;

		error_num = error_number; 

		swprintf( wcsExtension, L"%s.%s", obj->LibDotClass, obj->Methode);

		strErrorMessage = ErrorMessage(error_num);

		nTamanho = strErrorMessage.GetLength() + 1;
		strcpy(szErrorMessage, strErrorMessage);

		MultiByteToWideChar(CP_ACP, 0, szErrorMessage, -1, wcsErrorMessage, nTamanho);
		
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

	//	CoUninitialize();

	SysFreeString(txx);

	WaitForSingleObject(g_hMutex, INFINITE);
	g_struVariaveisGlobais.caObjThreadActive[obj->ActivityIx] = FALSE;
	ReleaseMutex(g_hMutex);
	
	HANDLE hHeap = GetProcessHeap();
	HeapFree(hHeap, NULL, obj->LibDotClass);
	HeapFree(hHeap, NULL, obj->Argument);

	if(lpVarResult->bstrVal != NULL)
		SysFreeString(lpVarResult->bstrVal); 

	HeapFree(hHeap, NULL, obj); 

	WaitForSingleObject(g_hMutex, INFINITE);
 	g_struVariaveisGlobais.cObjInstNum--;   
	ReleaseMutex(g_hMutex);

	return hresult;
}
///////////////////////////////////////
/////// Thread Pool Arbitrator
/////// Gets a free Thread from the ThreadPool and starts 
/////// a new instance of a object with it
HANDLE ThreadArbitrator(ROMAOBJCALL* lpObj)
{
	HANDLE hThrObj;
	DWORD hIx;
	DWORD thID;

	EnterCriticalSection( &gpCriticalSection);

	hIx=0;
	do
	{
		DWORD ExitCode=0;
		WaitForSingleObject(g_hMutex, INFINITE);
		if(g_struVariaveisGlobais.haObjThread[hIx] == NULL)
		{
			//PRINTF_DEBUG("Thread %i FREE\n", hIx);
			ReleaseMutex(g_hMutex);
			break;
		}
		ReleaseMutex(g_hMutex);

		Sleep(1);          //Wait 1 mSecs to avoid CPU lock up

		hIx++;

		if(hIx > (MAX_OBJ_THREAD_COUNTER-1))
			hIx=0;
	}while(TRUE);

	WaitForSingleObject(g_hMutex, INFINITE);
	if(g_struVariaveisGlobais.haObjThread[hIx]!=NULL)
	{
		CloseHandle(g_struVariaveisGlobais.haObjThread[hIx]);
	}
	ReleaseMutex(g_hMutex);

	///// Warning: Memory allocated in RouteObj should be deallocated in ObjInvoke
	hThrObj = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ObjInvoke, lpObj, 0 , &thID);

	WaitForSingleObject(g_hMutex, INFINITE);
	g_struVariaveisGlobais.waObjThreadID[hIx] = thID;
	ReleaseMutex(g_hMutex);

	if(hThrObj != NULL)
	{
		WaitForSingleObject(g_hMutex, INFINITE);
		g_struVariaveisGlobais.haObjThread[hIx] = hThrObj;
		lpObj->ActivityIx = hIx;
		g_struVariaveisGlobais.cObjInstNum++; // Obj instances counter
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
	ROMAOBJCALL* obj = (ROMAOBJCALL*)HeapAlloc( hHeap, 
						HEAP_ZERO_MEMORY, sizeof(ROMAOBJCALL));
	WCHAR*  extension = (WCHAR*)HeapAlloc( hHeap, 
						HEAP_ZERO_MEMORY, pmsgprops->aPropVar[3].caub.cElems +1);
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

	*FoundDot = '\0';
	obj->LibDotClass = extension;
	obj->Methode = ++FoundDot;					//The methode name follows the 2nd dot
	WCHAR* msgbody = (WCHAR*)HeapAlloc(GetProcessHeap(), 
						HEAP_ZERO_MEMORY, pmsgprops->aPropVar[0].caub.cElems +1);
	memcpy( msgbody, pmsgprops->aPropVar[0].caub.pElems, pmsgprops->aPropVar[0].caub.cElems);
	obj->Argument = msgbody;
	//WPRINTF_DEBUG( L"A extensao veio :%s, %s \n", obj->LibDotClass, obj->Methode);
	//WPRINTF_DEBUG(L"Chamada ao Obj ");

	/////  these pointers allocated in RouteObj have to be deallocated in ObjInvoke
	if(FAILED(ThreadArbitrator( obj)))
	{
		//PRINTF_DEBUG("Arbitrator failed\n");
		HeapFree( hHeap, 0, obj);
		HeapFree( hHeap, 0, extension);
		HeapFree( hHeap, 0, msgbody);
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
	DWORD dwDestFormatNameBufferLength = sizeof(szwFormatNameBuffer);
	hr = MQPathNameToFormatName((WCHAR*)MQPathName,
                              szwFormatNameBuffer,
                              &dwDestFormatNameBufferLength);


	hr = MQOpenQueue(
		 szwFormatNameBuffer,    // Format Name of the queue to be opened
		 MQ_RECEIVE_ACCESS,      // Access rights to the Queue
		 0,                      // No receive Exclusive
		 &hQueue                 // OUT: handle to the opened Queue
	);
 
	if (FAILED(hr))
	{
		CString strMensagem;
		char *szMQPathName;
		long nTamanho = wcslen((WCHAR*)MQPathName)+1;

		szMQPathName = new char[nTamanho];
		memset(szMQPathName, 0, nTamanho);

		WideCharToMultiByte(CP_ACP, 0, (WCHAR*)MQPathName, -1, szMQPathName, nTamanho, NULL, NULL);

		strMensagem.Format(_T("Erro ao abrir a fila \"%s\"\n"), szMQPathName);

		delete szMQPathName;

		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
		DispErrDesc(hr, _T("Roma"));

		return hr;
	}
 
 
	MQMSGPROPS * pmsgprops;
	MQPROPVARIANT *paVariant;
	MSGPROPID * paPropId;
	DWORD dwcPropId = 0;
	WCHAR *wszLabelBuffer; 
	WCHAR *wszBodyBuffer;
	UCHAR* wszExtentionBuffer;
	HANDLE hHeap;
	BOOL bProgramFinishRequest = FALSE;
	//
	//WPRINTF_DEBUG(L"Set %s to Rx mode.\n", MQPathName);
	do
	{
		//  The output parameters to an asynchronous call to MQReceiveMessage 
		//  should be kept intact until the operation completes, you should 
		//  not free or reuse them until the operation is complete.
		//

		hHeap = GetProcessHeap ();
		pmsgprops = (MQMSGPROPS *)HeapAlloc (hHeap, 
						   HEAP_ZERO_MEMORY, 
						   sizeof(MQMSGPROPS));

		paVariant = (MQPROPVARIANT*)HeapAlloc (hHeap, 
						   HEAP_ZERO_MEMORY,sizeof(MQPROPVARIANT)*10);
		paPropId = (MSGPROPID*)HeapAlloc (hHeap, 
						   HEAP_ZERO_MEMORY,sizeof(MSGPROPID)*10);
		wszLabelBuffer = (WCHAR*)HeapAlloc (hHeap, 
						   HEAP_ZERO_MEMORY,sizeof(WCHAR)*256);
		wszBodyBuffer = (WCHAR*)HeapAlloc (hHeap, 
						   HEAP_ZERO_MEMORY,sizeof(WCHAR)*MSG_BODY_LEN);
		wszExtentionBuffer = (UCHAR*)HeapAlloc (hHeap, 
						   HEAP_ZERO_MEMORY,sizeof(UCHAR)*MSG_EXT_LEN);
    
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
			2000,    					// Max time (msec) to wait
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
				paPropId[1] = PROPID_M_LABEL;           // Property ID
				paVariant[1].vt = VT_LPWSTR;            // Type indicator
				paVariant[1].pwszVal = L"Invalid Content";  // Value

				//WPRINTF_DEBUG(L"Msg in %s, (w/ invalid content).\n", MQPathName);
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

	
	//WPRINTF_DEBUG(L"MQ %s RX finished.\n", MQPathName);
	return MQ_OK;
}
////////////////////////////////////////////

/// Main ENTRY  ////////

HRESULT ObjMQRoute( HANDLE *hEvStop)
{
	int ix=0, keyCode=0;
	int ctQueues;

	HANDLE  haMQRxThread[5];
	BOOL ret;
	TCHAR *lpBackSlash;
	TCHAR szEXEPathname[_MAX_PATH];

	WaitForSingleObject(g_hMutex, INFINITE);
	g_struVariaveisGlobais.cObjInstNum = 0;
	g_struVariaveisGlobais.bProgramFinishRequest;
	ReleaseMutex(g_hMutex);

	GetModuleFileName(NULL, szEXEPathname, _MAX_PATH);

	lpBackSlash = strrchr( szEXEPathname, '\\');
	*lpBackSlash='\0';

	strcat( szEXEPathname, "\\srvROMA.cfg");

	HANDLE hHeap = GetProcessHeap();

	if (ArquivoExiste(szEXEPathname))
	{
		WaitForSingleObject(g_hMutex, INFINITE);
		ctQueues = ReadConfigFile( szEXEPathname, g_struVariaveisGlobais.wsaQueueNames, MAX_COUNT_RX_QUEUE);
		ReleaseMutex(g_hMutex);

		if( ctQueues < 1)
		{
			CString strMensagem;
			strMensagem = _T("Nome de filas inválidos\n");
			LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
		}

		WaitForSingleObject(g_hMutex, INFINITE);
		g_struVariaveisGlobais.wsErrorQueueName = ReadMQErrNamefromCfgFile( szEXEPathname);
		ReleaseMutex(g_hMutex);
	}
	else
	{
		CString strMensagem;
		strMensagem.Format(_T("Arquivo \"%s\" não foi encontrado!\n"), szEXEPathname);
		LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, _T("Roma"));
	}

	WaitForSingleObject(g_hMutex, INFINITE);
	if( g_struVariaveisGlobais.wsErrorQueueName == NULL) 
		g_struVariaveisGlobais.wsErrorQueueName = DEFAULT_ERROR_QUEUE_NAME;
	ReleaseMutex(g_hMutex);

	
	InitializeCriticalSection( &gpCriticalSection);
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	WaitForSingleObject(g_hMutex, INFINITE);
	g_struVariaveisGlobais.hEvSynObj = CreateEvent(NULL, TRUE, FALSE, NULL);
	ReleaseMutex(g_hMutex);

	DWORD thId;

	for(ix=0; ix < ctQueues; ix++)
	{
		WaitForSingleObject(g_hMutex, INFINITE);
		WCHAR * wsaQueueNames = g_struVariaveisGlobais.wsaQueueNames[ix];
		ReleaseMutex(g_hMutex);

		haMQRxThread[ix] = CreateThread(NULL,NULL, (LPTHREAD_START_ROUTINE)MQSyncRcv, (void*)wsaQueueNames, 0, &thId);
	}

	//Wait all the MQRx threads to finish
	DWORD  nLoops=0;
	BOOL bThdStillActive=FALSE;
	do
	{
		//***  if EvStop is Signed then procede Stopping
		//*** else after 2 secs procede cleaning Thread flags
		if(WaitForSingleObject( *hEvStop, 2000) == WAIT_OBJECT_0)
		{
			LogEvent(_T("\nWAIT_OBJECT_0\n"), EVENTLOG_INFORMATION_TYPE, _T("Roma"));
			break;
		}

		for(ix = 0; ix < MAX_OBJ_THREAD_COUNTER; ix++)
		{
			DWORD ExitCode=0;
			WaitForSingleObject(g_hMutex, INFINITE);
			if((g_struVariaveisGlobais.haObjThread[ix]!=NULL) && !g_struVariaveisGlobais.caObjThreadActive[ix])  // Print MSG if thread finished but yet active.
			{
				ret = GetExitCodeThread(g_struVariaveisGlobais.haObjThread[ix], &ExitCode);	
				if(ret)
				{
					if((ExitCode != STILL_ACTIVE))
					{
						g_struVariaveisGlobais.haObjThread[ix] = FALSE;
					}
				}
			}
			ReleaseMutex(g_hMutex);
		}

	}while(TRUE);

	WaitForSingleObject(g_hMutex, INFINITE);
	g_struVariaveisGlobais.bProgramFinishRequest = TRUE;
	ReleaseMutex(g_hMutex);

	do
	{
		DWORD ExitCode=0;
		for(ix=0; ix<5; ix++)
		{
			ret = GetExitCodeThread(haMQRxThread[ix],&ExitCode);	
			if(!ret)
			{
				if(ExitCode == STILL_ACTIVE)
					bThdStillActive=TRUE;
					break;
			}
		}
		Sleep(1000);
		//PRINTF_DEBUG(".");

	}while((++nLoops <20) && bThdStillActive);

	if(nLoops >=20)
	{
		//PRINTF_DEBUG("Timeout waiting threads to finish.\n");
	}

	//PRINTF_DEBUG("\n.");
	DeleteCriticalSection(&gpCriticalSection);
	CoUninitialize();

	return 0;
   
}
