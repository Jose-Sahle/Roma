// WaCServer.h: interface for the CWaCServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WACSERVER_H__D7C04995_DF5D_49D0_846F_2A56B34F2D7C__INCLUDED_)
#define AFX_WACSERVER_H__D7C04995_DF5D_49D0_846F_2A56B34F2D7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef void (*GETCONNECTEDSOCKET)(void *);

class CWaCServer
{
public:
	CWaCServer();
	virtual ~CWaCServer();

	bool	StopListen();
	bool	StartListen(unsigned short nPort, GETCONNECTEDSOCKET pFunction);
	TCHAR *	WaCGetLastErrorText();
	
private:
	friend void			ThreadWaitConnection(LPVOID pParam);

	DWORD				m_dwThreadId;

	WSADATA				m_wsaData;
	SOCKET				m_ListenSocket;
	struct sockaddr_in	m_ServerAddress;
	struct hostent *	m_pHost;
	bool				m_bIsListening;
	TCHAR				m_szLastErrorText[512];
};

typedef struct _WaitConnectionParams
{
	GETCONNECTEDSOCKET		Function;
	unsigned short			nPort;
	CWaCServer *			pParent;
} WaitConnectionParams;

typedef struct _AcceptedConnectionParams
{
	SOCKET				Socket;
	struct sockaddr_in	Address;
	short				nConnectionNumber;
} AcceptedConnectionParams;

#endif // !defined(AFX_WACSERVER_H__D7C04995_DF5D_49D0_846F_2A56B34F2D7C__INCLUDED_)
