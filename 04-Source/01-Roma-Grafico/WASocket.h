#ifndef __MSSocket_H__
#define __MSSocket_H__

#include <winsock.h>

#define		WM_SENT					(WM_USER + 1001)
#define		WM_RECEIVED				(WM_USER + 1002)


#define SOCKET_IDLE			0
#define SOCKET_WRITE_STATE	1
#define SOCKET_READ_STATE	2


class CMSSocket 
{
// Attributes
protected:
	SOCKET m_Socket;
// Constructor & Destructor
public:
	CMSSocket(SOCKET s, int nID = 0)
	{
		m_Socket  = s;
		m_nID = nID;
	}

	virtual ~CMSSocket(void) 
	{
		if (m_Socket != NULL)
			closesocket(m_Socket);
	}

// Operations
public:
	int Send(void * pBuffer, int nLen) 
	{
		int nBytes = send(m_Socket, (const char*) pBuffer, nLen, 0);

		return nBytes;
	}

	int Recv(void * pBuffer, int nLen) 
	{
		int nBytes = recv(m_Socket, (char*) pBuffer, nLen, 0);

		return nBytes;
	}

	bool Read(void * pBuffer, int nLen);
	bool Write(void * pBuffer, int nLen);

	int	ReadEx(void *pBuffer, int nSize);
	void Close();

	void SetSocket(SOCKET s);

private:

	int		m_nID;
};

#endif
