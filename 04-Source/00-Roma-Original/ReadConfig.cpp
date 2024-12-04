// ReadConfig.cpp : Gets the MQ names from srvROMA.cfg file.
//

#include <stdio.h>
#include <windows.h>

#define KEY_SEARCH		"Queue"
#define KEYERROR_SEARCH		"QueueError"

static void StrZToWCSZ( char *in, WCHAR* out)
{
	while(*out++ =(WCHAR) *in++);
}
/////////////////////////////////////////////
// Parse QUEUE NAMES from a config file  "srvROMA.cfg"
//////////
int ReadConfigFile(CHAR *szCfgFile, WCHAR **szNames, int cNames)
{
	HANDLE hHeap = GetProcessHeap();
	WCHAR MsgBuf[256];
	WCHAR* lpMsgBuf = MsgBuf;

	static const char szSection[]="srvROMA";
	static const int nBuffer=128;
	char szKey[64] = KEY_SEARCH;
	char sBuffer[64];

	int i=0;

	for(i=0;i<cNames;i++) 
	{
		szNames[i] = NULL;
		sprintf( szKey, "%s%i", KEY_SEARCH, i+1);	
		if(!GetPrivateProfileString(szSection, szKey, "", sBuffer, nBuffer, szCfgFile))
			break;
		szNames[i] = (WCHAR*)HeapAlloc( hHeap, 0, 128);
		StrZToWCSZ( sBuffer, szNames[i]);
		wprintf(L"%s\n", szNames[i]);

	}

	//HeapFree(hHeap, 0, szNames[i]);

	return i;  //Returns the number of Queue names found
}
//////////////////////////////////////////////////////
// Gets the error MSMQ QUEUE NAME from cfg file
//////////////
WCHAR* ReadMQErrNamefromCfgFile(CHAR *szCfgFile)
{
	HANDLE hHeap = GetProcessHeap();
	static const char szSection[]="srvROMA";
	static const int nBuffer=128;
	char szKey[64] = KEYERROR_SEARCH;
	char sBuffer[64];
	WCHAR *szName = NULL;

	if(GetPrivateProfileString(szSection, szKey, "", sBuffer, nBuffer, szCfgFile))
	{
		szName = (WCHAR*)HeapAlloc( hHeap, 0, 128);
		StrZToWCSZ( sBuffer, szName);
		wprintf(L"QUEUEERROR=%s\n", szName);
		//HeapFree(hHeap, 0, sBuffer);
	}
	return szName;
}
////////////////////////////////////////////////
//////////