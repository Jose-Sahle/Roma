#include "stdafx.h"

#include <windows.h>

#include "WASocket.h"

int CMSSocket::ReadEx(void *pBuffer, int nSize)
{
	return Recv(pBuffer, nSize);
}

bool CMSSocket::Read(void * pBuffer, int nLen) 
{
	char * cBuffer = (char *)pBuffer;
	int    nRecv;

	while (nLen > 0) 
	{
		nRecv = Recv(cBuffer, nLen);
		if (nRecv > 0) 
		{
			nLen -= nRecv;
			cBuffer += nRecv;
		}
		else
			return false;
	}

	return true;
}

bool CMSSocket::Write(void * pBuffer, int nLen) 
{
	char * cBuffer = (char *)pBuffer;
	int    nSend;

	while (nLen > 0) 
	{
		nSend = Send(cBuffer, nLen);
		if (nSend > 0) 
		{
			nLen -= nSend;
			cBuffer += nSend;
		}
		else
			return false;
	}

	return true;
}

void CMSSocket::Close()
{
	if (m_Socket != NULL)
		closesocket(m_Socket);
}

void CMSSocket::SetSocket(SOCKET s)
{
	m_Socket  = s;
}