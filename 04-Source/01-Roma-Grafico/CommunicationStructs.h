#ifndef __WA_COMMUNICATION_STRUCTS__
#define __WA_COMMUNICATION_STRUCTS__

#include "stdafx.h"

#define MAX_SEND_BUFFER	2048
#define MAX_KEY_LENGTH  1053
#define LENGTH_OF_KEY   20

class CAccessKey
{
public:
	CAccessKey(LPCTSTR lpszFileName);

	void Load();
	void Save();
	unsigned int GetKeyLength() {return LENGTH_OF_KEY;}

	CString StrExpirationDate();

	void SetDate(WORD wYear, BYTE byMonth, BYTE byDay);
	void GetDate(WORD &wYear, BYTE &byMonth, BYTE &byDay);

	BOOL IsValidate();
	CString Encode();
	BOOL	Decode();

private:
	BYTE	m_byDay;
	BYTE	m_byMonth;
	WORD	m_wYear;
	BOOL	m_isValidate;

	CString m_strExpirationDate;

	char	DecodeChar(char cByte, int nLin);

public:
	CString m_strFileName;
	CString m_strControlKey;
	CString m_strAccessKey;
};

enum WaRequestOptions 
{
	rqoUserValidation = 0, 
	rqoReceiveFiles,
	rqoSendFiles,
	rqoSystemDateTime,
	rqoEndConnection,
	rqoOthers
};

typedef struct _WaSendFileBuffer
{
	DWORD	nBufferSize;
	char	cBuffer[MAX_SEND_BUFFER];
} WaSendFileBuffer;

typedef struct _WALoginInfo
{
	short	nUserCode;
	long	lValidationCode;
	TCHAR	szUserName[30];
	TCHAR	szCxPostal[30];
#ifdef COMUNICATION_USE_PREFIXO
	TCHAR   szPrefixo[10];
#endif
} WaLoginInfo;

typedef struct _WARequest
{
	short	nRequest; // 0 = Receive File, 1 = Send File, 2 = System Date/Time, 3 = Other...
	char	cRequestOption[128];
	long	lValidationCode;
} WARequest;

typedef struct _WAFileInfo
{
	DWORD	dwFileSize;
	TCHAR	szFileName[128];
} WAFileInfo;

bool GenerateValidationCode(WaLoginInfo& LoginInfo, bool bCheck = false);

#endif