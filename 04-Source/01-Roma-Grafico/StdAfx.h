// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__E7AE5707_0233_4483_B883_4BAF2AB8882F__INCLUDED_)
#define AFX_STDAFX_H__E7AE5707_0233_4483_B883_4BAF2AB8882F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
/* VISUAL C++ 6.0
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
*/

#include <SDKDDKVer.h>

#define _ATL_APARTMENT_THREADED

#include <afx.h>
#include <Afxsock.h>
#include <afxcmn.h>
#include <afxdisp.h>
#include <stdio.h>
#include <Afxdlgs.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module

class CServiceModule : public CComModule
{
public:
	HRESULT RegisterServer(BOOL bRegTypeLib, BOOL bService);
	HRESULT UnregisterServer();
	void Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, UINT nServiceNameID, const GUID* plibid = NULL);
	void Start();
	void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
	void Handler(DWORD dwOpcode);
	void Run();
	BOOL IsInstalled();
	BOOL Install();
	BOOL Uninstall();
	LONG Unlock();
	void SetServiceStatus(DWORD dwState);
	void SetupAsLocalServer();

	//Implementation
private:
	static void WINAPI _ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
	static void WINAPI _Handler(DWORD dwOpcode);

	// data members
public:
	TCHAR m_szServiceName[256];
	SERVICE_STATUS_HANDLE m_hServiceStatus;
	SERVICE_STATUS m_status;
	DWORD dwThreadID;
	BOOL m_bService;
};

extern CServiceModule _Module;
#include <atlcom.h>

#ifndef _AFXDLL
	#ifdef _AFX_NO_MFC_CONTROLS_IN_DIALOGS
		#ifdef _DEBUG
			#pragma comment(lib, "afxnmcdd.lib")
		#else
			#pragma comment(lib, "afxnmcd.lib")
		#endif
		#pragma comment(linker, "/include:__afxNoMFCControlSupportInDialogs")
		#pragma comment(linker, "/include:__afxNoMFCControlContainerInDialogs")
	#endif
	#ifndef _UNICODE
		#ifdef _DEBUG
			#pragma comment(lib, "nafxcwd.lib")
		#else
			#pragma comment(lib, "nafxcw.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "uafxcwd.lib")
		#else
			#pragma comment(lib, "uafxcw.lib")
		#endif
	#endif
#else
	#ifndef _UNICODE
		#ifdef _DEBUG
			#pragma comment(lib, "mfc" _MFC_FILENAME_VER "d.lib")
			#pragma comment(lib, "mfcs" _MFC_FILENAME_VER "d.lib")
		#else
			#pragma comment(lib, "mfc" _MFC_FILENAME_VER ".lib")
			#pragma comment(lib, "mfcs" _MFC_FILENAME_VER ".lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "mfc" _MFC_FILENAME_VER "ud.lib")
			#pragma comment(lib, "mfcs" _MFC_FILENAME_VER "ud.lib")
		#else
			#pragma comment(lib, "mfc" _MFC_FILENAME_VER "u.lib")
			#pragma comment(lib, "mfcs" _MFC_FILENAME_VER "u.lib")
		#endif
	#endif
#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E7AE5707_0233_4483_B883_4BAF2AB8882F__INCLUDED)
