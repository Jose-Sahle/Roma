// Functions.h: interface for the Functions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNCTIONS_H__5159A116_1602_11D4_88D3_0050DAD660D2__INCLUDED_)
#define AFX_FUNCTIONS_H__5159A116_1602_11D4_88D3_0050DAD660D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DT_STRING	0x00
#define DT_NUMBER	0x01
#define DT_DATETIME	0x02
#define DT_DATE		0x03
#define DT_TIME		0x04
#define DT_IMAGE	0x05

#define ST_ASC			1
#define ST_DESC		   -1

typedef struct _SortInfo
{
	CListCtrl*	pctrlList;
	short		nColumn;
	short		nColumnDataType;
	short		nSortType;
}SortInfo;

CString CriptografaSenha(CString strSenha);
CString DescriptografaSenha(CString strSenha);
long			s2l(const CString&);
CString			l2s(const long);
int				s2i(const CString&);
CString			i2s(const int);
double			s2d(const CString&);
CString			d2s(const double, const int nDecs = 2, const int nInts = 0);
__int64			s2i64(const CString&);
CString			i642s(const __int64);
CTime			s2tm(const CString&, int nOption = DT_DATETIME);
CString			tm2s(const CTime&, int nOption = DT_DATETIME);
WCHAR*			A2U(const char* pszStr, size_t Size, WCHAR* pszRet);
char*			U2A(const WCHAR* pszStr, size_t Size, char* pszRet);
char*			STRU2A(const WCHAR* pszStr, char* pszRet);
int	CALLBACK	CompareFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
double			trunc(const double dVal, const int nDecs);
CString			InsertLeftZeros(int nFullSize, const CString& str);
CString			InsertLeftChars(int nFullSize, const CString& str, const TCHAR c = _T(' '));
CString			Trim(const CString& str, TCHAR cCharToRemove = _T(' '));
void			HideKeyboard();
void			ShowKeyboard(CWnd* pWnd = NULL);
bool			WaCreateDirectory(TCHAR *szPath);
CString			tmOle2s(const COleDateTime& tm, int nOption = DT_DATETIME);
COleDateTime    s2tmOle(const CString&, int nOption = DT_DATETIME);



#endif // !defined(AFX_FUNCTIONS_H__5159A116_1602_11D4_88D3_0050DAD660D2__INCLUDED_)
