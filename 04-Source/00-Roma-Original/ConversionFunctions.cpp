// Functions.cpp: implementation of the Functions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConversionFunctions.h"
#include <afxdisp.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

long	s2l(const CString& str)
{
#ifdef UNICODE
	return _wtol(str);
#else
	return atol(str);
#endif
}

CString	l2s(const long lVal)
{
	WCHAR c[11];
	return _ltow(lVal, c, 10);
}

int		s2i(const CString& str)
{
#ifdef UNICODE
	return _wtoi(str);
#else
	return atoi(str);
#endif
}

CString i2s(const int nVal)
{
	TCHAR c[10];
#ifdef UNICODE
	return _itow(nVal, c, 10);
#else
	return _itoa(nVal, c, 10);
#endif	
}

double	s2d(const CString& str)
{
	TCHAR* c;
	CString strOut = str;

	strOut.Replace(_T(','), _T('.'));
	
#ifdef UNICODE
	return wcstod(strOut, &c);
#else
	return strtod(strOut, &c);
#endif
}

CString d2s(const double dVal, const int nDecs, const int nInts)
{
	CString str1, str2;
	str2.Format(_T("%%%d.%df"), nInts, nDecs);
	str1.Format(str2, dVal );

	return str1;
}

__int64	s2i64(const CString& str)
{
	#ifdef UNICODE
		return _wtoi64(str);
	#else
		return _atoi64(str);
	#endif
}

CString i642s(const __int64 n64Val)
{
	CString str;
	__int64 n64 = n64Val;
	bool bNegativo = false;

	if (n64 < 0)
	{
		n64 *= -1;
		bNegativo = true;
	}
	else if (n64 == 0)
		return _T('0');
	
	while(n64 > 0)
	{
		str += (char) ((n64 % 10) + 48);
		n64 /= 10;
	}

	if (bNegativo)
		str += _T('-');
	
	str.MakeReverse();

	return str;
}

CTime s2tm(const CString& str, int nOption) // Formato:dd/mm/aaaa hh:mm:ss
{
	if(s2i( str.Mid(6, 4)) > 2038)
		return CTime(2039, 1, 1, 0, 0, 0);

	switch(nOption)
	{
	case DT_DATETIME:
	default:
		{
			ASSERT(str.GetLength() == 19);

			CTime tm(	s2i( str.Mid(6, 4) ), 
						s2i( str.Mid(3, 2) ), 
						s2i( str.Mid(0, 2) ), 
						s2i( str.Mid(11, 2) ), 
						s2i( str.Mid(14, 2) ), 
						s2i( str.Mid(17, 2) ));
			return tm;
		}
	case DT_DATE:
		{
			ASSERT(str.GetLength() == 10);

			CTime tm(	s2i( str.Mid(6, 4) ), 
						s2i( str.Mid(3, 2) ), 
						s2i( str.Mid(0, 2) ), 
						0, 
						0, 
						0);
			return tm;
		}
	case DT_TIME:
		{
			ASSERT(str.GetLength() == 8);

			CTime tm(	1970, 
						1, 
						1, 
						s2i( str.Mid(0, 2) ), 
						s2i( str.Mid(3, 2) ), 
						s2i( str.Mid(6, 2) ));
			return tm;
		}
	}
}

CString tm2s(const CTime& tm, int nOption)
{
	if(tm < 0)
		return CString(_T("01/01/1970 00:00:00"));

	CString str;
	
	str.Format(	_T("%02d/%02d/%04d %02d:%02d:%02d"), 
				tm.GetDay(),
				tm.GetMonth(),
				tm.GetYear(),
				tm.GetHour(),
				tm.GetMinute(),
				tm.GetSecond()
				);

	switch(nOption)
	{
	case DT_DATETIME:
		return str;
	case DT_DATE:
		return str.Left(10);
	case DT_TIME:
		return str.Right(8);
	default:
		return str;
	}
}

int CALLBACK CompareFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	SortInfo * si = (SortInfo*) lParamSort;

	int nIndex1 = (int) lParam1;
	int nIndex2 = (int) lParam2;

	CString str1, str2;
	double d1, d2;
	CTime tm1, tm2;
	int nReturn;
	LVITEM Item;
	
	switch(si->nColumnDataType)
	{
	case DT_NUMBER:
		d1 = s2d(si->pctrlList->GetItemText(nIndex1, si->nColumn));
		d2 = s2d(si->pctrlList->GetItemText(nIndex2, si->nColumn));

		if(d1 == d2)
			nReturn = 0;
		else if(d1 < d2)
			nReturn = -1;
		else
			nReturn = 1;
	break;

	case DT_TIME:
	case DT_DATE:
	case DT_DATETIME:
		// ATK 22.11.01 Verifica se os valores não estão em branco
		if(si->pctrlList->GetItemText(nIndex1, si->nColumn).IsEmpty())
			tm1 = 0;
		else
			tm1 = s2tm(si->pctrlList->GetItemText(nIndex1, si->nColumn), si->nColumnDataType);

		if(si->pctrlList->GetItemText(nIndex2, si->nColumn).IsEmpty())
			tm2 = 0;
		else
			tm2 = s2tm(si->pctrlList->GetItemText(nIndex2, si->nColumn), si->nColumnDataType);

		if(tm1 == tm2)
			nReturn = 0;
		else if(tm1 < tm2)
			nReturn = -1;
		else
			nReturn = 1;
	break;
	case DT_IMAGE:
		Item.mask = LVIF_IMAGE;
		Item.iItem = nIndex1;
		Item.iSubItem = si->nColumn;
		si->pctrlList->GetItem(&Item);
		nIndex1 = Item.iImage;

		Item.mask = LVIF_IMAGE;
		Item.iItem = nIndex2;
		Item.iSubItem = si->nColumn;
		si->pctrlList->GetItem(&Item);
		nIndex2 = Item.iImage;

		if(nIndex1 == nIndex2)
			nReturn = 0;
		else if(nIndex1 < nIndex2)
			nReturn = -1;
		else
			nReturn = 1;
	break;

	case DT_STRING:
	default:
		str1 = si->pctrlList->GetItemText(nIndex1, si->nColumn);
		str2 = si->pctrlList->GetItemText(nIndex2, si->nColumn);

		nReturn = str1.CompareNoCase(str2);
	break;
	}

	return (nReturn * si->nSortType);
}

WCHAR* A2U(const char* pszStr, size_t Size, WCHAR* pszRet)
{
	for(size_t n = 0; n < Size; n++)
		pszRet[n] = (WCHAR) pszStr[n];

	return pszRet;
}

char* U2A(const WCHAR* pszStr, size_t Size, char* pszRet)
{
    for(size_t n = 0; n < Size; n++)
		pszRet[n] = (char) pszStr[n];
	
	return pszRet;
}

char *STRU2A(const WCHAR* pszStr, char* pszRet)
{
	BOOL bContinua = TRUE;
	size_t nCont = 0;

	while (bContinua)
	{
		if (pszStr[nCont] == '\0')
		{
			pszRet[nCont] = (char) '\0';
			break;
		}

		pszRet[nCont] = (char) pszStr[nCont];

		nCont++;
	}

	return pszRet;
	
}

double trunc(const double dVal, const int nDecs)
{
	long lPow = 1;
	for (int nIndex = 0; nIndex < nDecs; nIndex++)
		lPow *=10;

	double dMultValue = (dVal * lPow) + 0.5;
	__int64 nVal = (__int64) dMultValue ;
	double dResult = (double) nVal / lPow;
	return dResult;
}

CString InsertLeftZeros(int nFullSize, const CString& str)
{
	if(str.GetLength() >= nFullSize)
		return str.Left(nFullSize);

	CString strRet = _T("000000000000000000000000000000000000000000000000000000000");
	strRet = strRet.Left(nFullSize - str.GetLength()) + str;
	return strRet;
}

CString	InsertLeftChars(int nFullSize, const CString& str, const TCHAR c)
{
	if(str.GetLength() >= nFullSize)
		return str.Left(nFullSize);
	
	CString strRet;
	strRet.Format(_T("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"), c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c);
	strRet = strRet.Left(nFullSize - str.GetLength()) + str;
	return strRet;
}

CString Trim(const CString& str, TCHAR cCharToRemove)
{
	CString sRetorno = str;
	sRetorno.TrimLeft(cCharToRemove);
	sRetorno.TrimRight(cCharToRemove);

	return sRetorno;
}

void HideKeyboard()
{
#ifdef DEBUG
	HWND InputWnd = ::FindWindow(NULL, _T("Calculator"));
#else
	HWND InputWnd = ::FindWindow(_T("SipWndClass"), _T("Input Panel"));	
#endif

	::SetWindowPos(InputWnd, NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
}

void ShowKeyboard(CWnd* pWnd)
{
#ifdef DEBUG
	HWND InputWnd = ::FindWindow(NULL, _T("Calculator"));
#else
	HWND InputWnd = ::FindWindow(_T("SipWndClass"), _T("Input Panel"));
#endif

	int y = 189;

	if(NULL != pWnd)
	{
		CRect rectWnd;
		pWnd->GetWindowRect(&rectWnd);
		
		if( (240 - rectWnd.bottom + 2) > 105)
			y = rectWnd.bottom + 2;
		else
			y = rectWnd.top - 12 - 105;
	}

	::SetWindowPos(InputWnd, HWND_TOPMOST, 0, y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_SHOWWINDOW );
}

bool WaCreateDirectory(TCHAR *szPath)
{
	TCHAR szDir[MAX_PATH];
	int nPos = 1;
	bool bEnd = false;

	szDir[0] = _T('\\');

	while(!bEnd)
	{
		if(szPath[nPos] == _T('\0'))
			bEnd = true;

		if(szPath[nPos] == _T('\\'))
		{
			szDir[nPos] = _T('\0');
			if(!CreateDirectory(szDir, NULL))
				if(::GetLastError() != ERROR_ALREADY_EXISTS)
					return false;
		}

		szDir[nPos] = szPath[nPos];
		nPos++;
	}

	return true;
}

CString tmOle2s(const COleDateTime& tm, int nOption)
{
	CString str;
	
	str.Format(	_T("%02d/%02d/%04d %02d:%02d:%02d"), 
				tm.GetDay(),
				tm.GetMonth(),
				tm.GetYear(),
				tm.GetHour(),
				tm.GetMinute(),
				tm.GetSecond()
				);

	switch(nOption)
	{
	case DT_DATETIME:
		return str;
	case DT_DATE:
		return str.Left(10);
	case DT_TIME:
		return str.Right(8);
	default:
		return str;
	}
}

COleDateTime s2tmOle(const CString& str, int nOption) // Formato:dd/mm/aaaa hh:mm:ss
{
	switch(nOption)
	{
	case DT_DATETIME:
	default:
		{
			ASSERT(str.GetLength() == 19);

			COleDateTime  tm(	s2i( str.Mid(6, 4) ), 
						s2i( str.Mid(3, 2) ), 
						s2i( str.Mid(0, 2) ), 
						s2i( str.Mid(11, 2) ), 
						s2i( str.Mid(14, 2) ), 
						s2i( str.Mid(17, 2) ));
			return tm;
		}
	case DT_DATE:
		{
			ASSERT(str.GetLength() == 10);

			COleDateTime tm(	s2i( str.Mid(6, 4) ), 
						s2i( str.Mid(3, 2) ), 
						s2i( str.Mid(0, 2) ), 
						0, 
						0, 
						0);
			return tm;
		}
	case DT_TIME:
		{
			ASSERT(str.GetLength() == 8);

			COleDateTime tm(	100, 
						1, 
						1, 
						s2i( str.Mid(0, 2) ), 
						s2i( str.Mid(3, 2) ), 
						s2i( str.Mid(6, 2) ));
			return tm;
		}
	}
}

CString CriptografaSenha(CString strSenha)
{
	CString strRetorno;

	int niCont;

	strRetorno.Empty();

	if (strSenha != _T(""))
	{
		for (niCont = 0; niCont < strSenha.GetLength(); niCont++)
		{
			TCHAR szCaracter[2];

			szCaracter[0] = _T('\0');
			_tcscpy(szCaracter, strSenha.Mid(niCont,1));
			szCaracter[0] += 128;

			strRetorno += szCaracter;
		}
	}

	return (strRetorno);
}

CString DescriptografaSenha(CString strSenha)
{
	CString strRetorno;

	int niCont;

	strRetorno.Empty();

	if (strSenha != _T(""))
	{
		for (niCont = 0; niCont < strSenha.GetLength(); niCont++)
		{
			TCHAR szCaracter[2];

			szCaracter[0] = _T('\0');
			_tcscpy(szCaracter, strSenha.Mid(niCont,1));
			szCaracter[0] -= 128;

			strRetorno += szCaracter;
		}
	}

	return (strRetorno);
}
