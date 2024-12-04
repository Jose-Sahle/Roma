#include "CommunicationStructs.h"

int g_nKeyTable[30][10] = {
	{4, 3, 2, 1, 0, 9, 8, 7, 6, 5},
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
	{6, 5, 4, 3, 8, 7, 2, 1, 0, 9},
	{8, 4, 9, 3, 0, 6, 2, 7, 5, 1},
	{6, 2, 0, 5, 8, 7, 9, 3, 1, 4},
	{9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
	{7, 5, 1, 4, 6, 2, 8, 3, 0, 9},
	{2, 4, 6, 8, 0, 1, 3, 5, 7, 9},
	{9, 7, 5, 3, 1, 0, 2, 4, 6, 8},
	{5, 3, 2, 8, 6, 1, 9, 4, 0, 7},
	{7, 2, 9, 3, 8, 4, 5, 6, 0, 1},
	{3, 8, 4, 7, 2, 9, 6, 0, 1, 5},
	{7, 4, 1, 0, 9, 6, 2, 3, 8, 5},
	{4, 3, 0, 5, 8, 7, 6, 1, 2, 9},
	{3, 9, 5, 8, 6, 4, 7, 1, 2, 0},
	{8, 3, 5, 7, 1, 0, 4, 2, 6, 9},
	{3, 2, 1, 7, 6, 5, 4, 9, 0, 8},
	{3, 0, 8, 5, 2, 6, 4, 1, 9, 7},
	{8, 7, 4, 5, 1, 3, 2, 6, 0, 9},
	{9, 8, 6, 4, 7, 2, 5, 1, 3, 0},
	{0, 5, 4, 2, 1, 3, 9, 7, 6, 8},
	{8, 5, 7, 6, 9, 4, 3, 1, 0, 2},
	{4, 3, 6, 5, 9, 2, 1, 0, 7, 8},
	{0, 1, 2, 6, 5, 4, 8, 3, 7, 9},
	{6, 4, 3, 2, 1, 7, 5, 8, 9, 0},
	{2, 1, 6, 9, 4, 8, 3, 5, 0, 7},
	{5, 0, 3, 2, 4, 6, 8, 1, 7, 9},
	{5, 3, 2, 1, 6, 4, 7, 0, 9, 8},
	{0, 3, 8, 5, 1, 2, 4, 7, 6, 9},
	{0, 8, 6, 5, 3, 4, 9, 7, 1, 2}
};

bool GenerateValidationCode(WaLoginInfo& LoginInfo, bool bCheck)
{
	int n;
	long lCode = 243;

	lCode  += LoginInfo.nUserCode;

	for(n = 0; n < (int) _tcslen(LoginInfo.szCxPostal); n++)
	{
		if(n%2)
			lCode -= LoginInfo.szCxPostal[n];
		else
			lCode += LoginInfo.szCxPostal[n];
	}

	for(n = 0; n < (int) _tcslen(LoginInfo.szUserName); n++)
	{
		if(n%3)
			lCode -= LoginInfo.szUserName[n];
		else
			lCode += LoginInfo.szUserName[n];
	}

	if(lCode == 0)
		lCode = 45;

	if(bCheck)
		return (lCode == LoginInfo.lValidationCode);
	else
		LoginInfo.lValidationCode = lCode;

	return true;
}

CAccessKey::CAccessKey(LPCTSTR lpszFileName)
{
	m_strFileName = lpszFileName;
	m_isValidate  = FALSE;
	Load();
}

void CAccessKey::Load()
{
	CFile	file;
	char	szControlKey[MAX_KEY_LENGTH];
	char	szAccessKey[MAX_KEY_LENGTH];
	int		nIndex, nCount;

	if (m_strFileName.GetLength() == 0)
		return;

	memset(szControlKey, 0, MAX_KEY_LENGTH * sizeof(char));
	memset(szAccessKey,  0, MAX_KEY_LENGTH * sizeof(char));

	if (!file.Open(m_strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::typeBinary))
		return;

	file.Read(szControlKey, MAX_KEY_LENGTH * sizeof(char));
	file.Read(szAccessKey,  MAX_KEY_LENGTH * sizeof(char));

	file.Close();

	nCount = 0;
	for (nIndex = 0; nIndex < LENGTH_OF_KEY; nIndex++)
		nCount += (int) szControlKey[nIndex];

	if (nCount == 0) 
	{
		for (nIndex = 0; nIndex < LENGTH_OF_KEY; nIndex++)
			szControlKey[nIndex] = (char) ((rand() + GetTickCount()) % 10);
	}

	m_strControlKey.Empty();
	m_strAccessKey.Empty();
	for (nIndex = 0; nIndex < LENGTH_OF_KEY; nIndex++)
	{
		m_strControlKey += (char) (szControlKey[nIndex] + 0x30);
		m_strAccessKey  += (char) (szAccessKey[nIndex]  + 0x30);
	}
}

void CAccessKey::Save()
{
	CFile	file;
	char	szControlKey[MAX_KEY_LENGTH];
	char	szAccessKey[MAX_KEY_LENGTH];
	int		nIndex;
	int		nByte;

	if (m_strFileName.GetLength() == 0)
		return;

	memset(szControlKey, 0, MAX_KEY_LENGTH * sizeof(char));
	memset(szAccessKey,  0, MAX_KEY_LENGTH * sizeof(char));

	for (nIndex = 0; nIndex < m_strControlKey.GetLength(); nIndex++)
	{
		nByte = (int) (m_strControlKey[nIndex] - 0x30);
		szControlKey[nIndex] = (char) ((nByte >= 0) ? nByte : 0);
	}

	for (nIndex = 0; nIndex < m_strAccessKey.GetLength(); nIndex++)
	{
		nByte = (int) (m_strAccessKey[nIndex] - 0x30);
		szAccessKey[nIndex] = (char) ((nByte >= 0) ? nByte : 0);
	}

	if (file.Open(m_strFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
	{
		file.Write(szControlKey, MAX_KEY_LENGTH * sizeof(char));
		file.Write(szAccessKey,  MAX_KEY_LENGTH * sizeof(char));
		file.Close();
	}
}

BOOL CAccessKey::IsValidate()
{
	return Decode();
}

CString CAccessKey::Encode()
{
	CFile	file;
	char	szControlKey[MAX_KEY_LENGTH];
	char	cByte;
	int		nIndex, nPos;
	int		nByte;
	DWORD	dwDate;
	CString strRet;

	strRet.Empty();
	if (m_strFileName.GetLength() == 0)
		return strRet;

	memset(szControlKey, 0, MAX_KEY_LENGTH * sizeof(char));

	for (nIndex = 0; nIndex < m_strControlKey.GetLength(); nIndex++)
	{
		nByte = (int) (m_strControlKey[nIndex] - 0x30);
		szControlKey[nIndex] = (char) ((nByte > 0) ? nByte : 0);
	}

	dwDate  = (DWORD) m_wYear;
	dwDate |= ((DWORD) m_byMonth) << 14;
	dwDate |= ((DWORD) m_byDay) << 18;

	for (nIndex = 0; nIndex < LENGTH_OF_KEY; nIndex++) 
	{
		nPos = nIndex % 30;

		switch (nIndex)
		{
		case 1:
			nByte = (int) ((dwDate >> 12) & 0x07);
			break;
		case 3:
			nByte = (int) ((dwDate >> 18) & 0x07);
			break;

		case 7:
			nByte = (int) ((dwDate >> 3) & 0x07);
			break;

		case 8:
			nByte = (int) ((dwDate >> 9) & 0x07);
			break;

		case 11:
			nByte = (int) ((dwDate >> 21) & 0x07);
			break;

		case 13:
			nByte = (int) (dwDate & 0x07);
			break;

		case 17:
			nByte = (int) ((dwDate >> 6) & 0x07);
			break;

		case 19:
			nByte = (int) ((dwDate >> 15) & 0x07);
			break;

		default:
			nByte = (int) szControlKey[nIndex];
		}
		cByte = (char) g_nKeyTable[nPos][nByte];
		strRet += (char) (cByte + 0x30);
	}

	return strRet;
}

BOOL CAccessKey::Decode()
{
	CFile	file;
	char	szControlKey[MAX_KEY_LENGTH];
	char	szAccessKey[MAX_KEY_LENGTH];
	int		nIndex, nPos;
	int		nByte;
	DWORD	dwDate;

	m_strExpirationDate = _T("99999999");

	m_isValidate = !((m_strFileName.GetLength() == 0) || (m_strAccessKey.GetLength() < LENGTH_OF_KEY));
	if (!m_isValidate)
		return m_isValidate;

	memset(szControlKey, 0, MAX_KEY_LENGTH * sizeof(char));
	memset(szAccessKey,  0, MAX_KEY_LENGTH * sizeof(char));

	for (nIndex = 0; nIndex < m_strControlKey.GetLength(); nIndex++)
	{
		nByte = (int) (m_strControlKey[nIndex] - 0x30);
		szControlKey[nIndex] = (char) ((nByte > 0) ? nByte : 0);
	}

	for (nIndex = 0; nIndex < m_strAccessKey.GetLength(); nIndex++)
	{
		nByte = (int) (m_strAccessKey[nIndex] - 0x30);
		m_isValidate = !((nByte < 0) || (nByte > 9));
		if (!m_isValidate)
			break;
		szAccessKey[nIndex] = (char) nByte;
	}

	if (!m_isValidate)
		return m_isValidate;

	m_wYear   = 0;
	m_byMonth = 0;
	m_byDay   = 0;
	dwDate    = 0;

	for (nIndex = 0; nIndex < LENGTH_OF_KEY; nIndex++) 
	{
		nPos = nIndex % 30;

		nByte = (int) DecodeChar(szAccessKey[nIndex], nPos);
		switch (nIndex)
		{
		case 1:
			dwDate |= (((DWORD) nByte) << 12);
			break;
		case 3:
			dwDate |= (((DWORD) nByte) << 18);
			break;

		case 7:
			dwDate |= (((DWORD) nByte) << 3);
			break;

		case 8:
			dwDate |= (((DWORD) nByte) << 9);
			break;

		case 11:
			dwDate |= (((DWORD) nByte) << 21);
			break;

		case 13:
			dwDate |= (DWORD) nByte;
			break;

		case 17:
			dwDate |= (((DWORD) nByte) << 6);
			break;

		case 19:
			dwDate |= (((DWORD) nByte) << 15);
			break;

		default:
			m_isValidate = (((char) nByte) == szControlKey[nIndex]);
		}

		if (!m_isValidate)
			break;
	}

	if (m_isValidate)
		SetDate ((WORD) (dwDate & 0x3FFF), (BYTE) ((dwDate >> 14) & 0x0F), (BYTE) ((dwDate >> 18) & 0x1F));

	return m_isValidate;
}

char CAccessKey::DecodeChar(char cByte, int nLin)
{
	int nIndex;

	for (nIndex = 0; (nIndex < 10) && (cByte != g_nKeyTable[nLin][nIndex]); nIndex++);

	return (char) ((nIndex >= 10) ? 0: nIndex);
}

void CAccessKey::SetDate(WORD wYear, BYTE byMonth, BYTE byDay)
{
	CString strToday;
	CTime	tmToday;
	WORD	wYearNow;
	BYTE	byMonthNow;
	BYTE	byDayNow;

	m_wYear		= wYear;
	m_byMonth	= byMonth;
	m_byDay		= byDay;

	tmToday  = CTime::GetCurrentTime();
//	strToday = tmToday.Format(_T("%Y%m%d"));

	wYearNow	= (WORD) tmToday.GetYear();
	byMonthNow	= (BYTE) tmToday.GetMonth();
	byDayNow	= (BYTE) tmToday.GetDay();

	strToday.Format(_T("%.4d%.2d%.2d"), wYearNow, byMonthNow, byDayNow);
	m_strExpirationDate.Format(_T("%.4d%.2d%.2d"), m_wYear, m_byMonth, m_byDay);

	m_isValidate = (m_strExpirationDate > strToday);
}

void CAccessKey::GetDate(WORD &wYear, BYTE &byMonth, BYTE &byDay)
{
	wYear	= m_wYear;
	byMonth = m_byMonth;
	byDay	= m_byDay;
}

CString CAccessKey::StrExpirationDate()
{
	return m_strExpirationDate;
}
