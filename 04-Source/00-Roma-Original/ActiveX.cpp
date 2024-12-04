// ActiveX.cpp: implementation of the CActiveX class.
//
//////////////////////////////////////////////////////////////////////

#include "ActiveX.h"
#include "Library.h"
#include <stdarg.h>
#include <comdef.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static TCHAR* BASED_CODE g_rgszVT[] =
{
	_T("void"),             //VT_EMPTY           = 0,   /* [V]   [P]  nothing                     */
	_T("null"),             //VT_NULL            = 1,   /* [V]        SQL style Null              */
	_T("short"),            //VT_I2              = 2,   /* [V][T][P]  2 byte signed int           */
	_T("long"),             //VT_I4              = 3,   /* [V][T][P]  4 byte signed int           */
	_T("single"),           //VT_R4              = 4,   /* [V][T][P]  4 byte real                 */
	_T("double"),           //VT_R8              = 5,   /* [V][T][P]  8 byte real                 */
	_T("CURRENCY"),         //VT_CY              = 6,   /* [V][T][P]  currency                    */
	_T("DATE"),             //VT_DATE            = 7,   /* [V][T][P]  date                        */
	_T("BSTR"),             //VT_BSTR            = 8,   /* [V][T][P]  binary string               */
	_T("IDispatch*"),       //VT_DISPATCH        = 9,   /* [V][T]     IDispatch FAR*              */
	_T("SCODE"),            //VT_ERROR           = 10,  /* [V][T]     SCODE                       */
	_T("boolean"),          //VT_BOOL            = 11,  /* [V][T][P]  True=-1, False=0            */
	_T("VARIANT"),          //VT_VARIANT         = 12,  /* [V][T][P]  VARIANT FAR*                */
	_T("IUnknown*"),        //VT_UNKNOWN         = 13,  /* [V][T]     IUnknown FAR*               */
	_T("wchar_t"),          //VT_WBSTR           = 14,  /* [V][T]     wide binary string          */
	_T(""),                 //                   = 15,
	_T("char"),             //VT_I1              = 16,  /*    [T]     signed char                 */
	_T("unsigned char"),             //VT_UI1             = 17,  /*    [T]     unsigned char               */
	_T("unsigned short"),           //VT_UI2             = 18,  /*    [T]     unsigned short              */
	_T("unsigned long"),            //VT_UI4             = 19,  /*    [T]     unsigned short              */
	_T("int64"),            //VT_I8              = 20,  /*    [T][P]  signed 64-bit int           */
	_T("uint64"),           //VT_UI8             = 21,  /*    [T]     unsigned 64-bit int         */
	_T("int"),              //VT_INT             = 22,  /*    [T]     signed machine int          */
	_T("unsigned int"),             //VT_UINT            = 23,  /*    [T]     unsigned machine int        */
	_T("void"),             //VT_VOID            = 24,  /*    [T]     C style void                */
	_T("HRESULT"),          //VT_HRESULT         = 25,  /*    [T]                                 */
	_T("PTR"),              //VT_PTR             = 26,  /*    [T]     pointer type                */
	_T("SAFEARRAY"),        //VT_SAFEARRAY       = 27,  /*    [T]     (use VT_ARRAY in VARIANT)   */
	_T("CARRAY"),           //VT_CARRAY          = 28,  /*    [T]     C style array               */
	_T("USERDEFINED"),      //VT_USERDEFINED     = 29,  /*    [T]     user defined type         */
	_T("LPSTR"),            //VT_LPSTR           = 30,  /*    [T][P]  null terminated string      */
	_T("LPWSTR"),           //VT_LPWSTR          = 31,  /*    [T][P]  wide null terminated string */
	_T(""),                 //                   = 32,
	_T(""),                 //                   = 33,
	_T(""),                 //                   = 34,
	_T(""),                 //                   = 35,
	_T(""),                 //                   = 36,
	_T(""),                 //                   = 37,
	_T(""),                 //                   = 38,
	_T(""),                 //                   = 39,
	_T(""),                 //                   = 40,
	_T(""),                 //                   = 41,
	_T(""),                 //                   = 42,
	_T(""),                 //                   = 43,
	_T(""),                 //                   = 44,
	_T(""),                 //                   = 45,
	_T(""),                 //                   = 46,
	_T(""),                 //                   = 47,
	_T(""),                 //                   = 48,
	_T(""),                 //                   = 49,
	_T(""),                 //                   = 50,
	_T(""),                 //                   = 51,
	_T(""),                 //                   = 52,
	_T(""),                 //                   = 53,
	_T(""),                 //                   = 54,
	_T(""),                 //                   = 55,
	_T(""),                 //                   = 56,
	_T(""),                 //                   = 57,
	_T(""),                 //                   = 58,
	_T(""),                 //                   = 59,
	_T(""),                 //                   = 60,
	_T(""),                 //                   = 61,
	_T(""),                 //                   = 62,
	_T(""),                 //                   = 63,
	_T("FILETIME"),         //VT_FILETIME        = 64,  /*       [P]  FILETIME                    */
	_T("BLOB"),             //VT_BLOB            = 65,  /*       [P]  Length prefixed bytes       */
	_T("STREAM"),           //VT_STREAM          = 66,  /*       [P]  Name of the stream follows  */
	_T("STORAGE"),          //VT_STORAGE         = 67,  /*       [P]  Name of the storage follows */
	_T("STREAMED_OBJECT"),  //VT_STREAMED_OBJECT = 68,  /*       [P]  Stream contains an object   */
	_T("STORED_OBJECT"),    //VT_STORED_OBJECT   = 69,  /*       [P]  Storage contains an object  */
	_T("BLOB_OBJECT"),      //VT_BLOB_OBJECT     = 70,  /*       [P]  Blob contains an object     */
	_T("CF"),               //VT_CF              = 71,  /*       [P]  Clipboard format            */
	_T("CLSID"),            //VT_CLSID           = 72   /*       [P]  A Class ID                  */
};

CString VTtoString( VARTYPE vt )
{
	CString str ;
	vt &= ~0xF000 ;
	if (vt <= VT_CLSID)
	   str = g_rgszVT[vt]  ;
	else
	   str = _T("Tipo inválido");

	return str ;
}

/*****************************************************************************
 * Classe CConnection                                                        *
 *                                                                           *
 *****************************************************************************/

/*****************************************************************************
 * Construtor                                                                *
 *                                                                           *
 *****************************************************************************/
CActiveX::CActiveX()
{
	m_bCreated = FALSE;
	m_pdisp = NULL;  
	m_punk = NULL; 
	m_hr = NOERROR;
	m_bByRef = FALSE;
}

/*****************************************************************************
 * Destrutor                                                                 *
 *                                                                           *
 *****************************************************************************/
CActiveX::~CActiveX()
{
	if (m_bByRef)
		return;

    if(m_punk) 
	{
		m_punk->Release();
		m_punk = NULL;
	}
    if(m_pdisp) 
	{
		m_pdisp->Release();
		m_pdisp = NULL;
	}
}

/*****************************************************************************
 * CreateObject                                                              *
 * Função que intancia um objeto dinamicamente                               *
 * 
 *****************************************************************************/
UINT CActiveX::CreateObject(TCHAR * pszObjeto)
{
	LPOLESTR pszProgID;
	UINT nError;
    CLSID clsid;                  // CLSID of ActiveX object.    

#ifndef UNICODE
	AnsiToUnicode(pszObjeto, &pszProgID);
	strcpy(m_szCOMName, pszObjeto);
#else
	pszProgID = pszObjeto; 
	_tcscpy(m_szCOMName, pszObjeto);
#endif
    
    // Retrieve CLSID from the ProgID that the user specified.
	TRY
	{
		m_hr = CLSIDFromProgID(pszProgID, &clsid);
		if(FAILED(m_hr))
		{
			nError = E_INVALID_PROGID;
			goto error;
		}

		m_hr = CoCreateInstance(clsid, NULL, CLSCTX_SERVER, IID_IUnknown, (void **)&m_punk);
		if(FAILED(m_hr))
		{
			nError = E_INSTANCE_NOT_CREATED;
			goto error;
		}

		// Ask the ActiveX object for the IDispatch interface.
		m_hr = m_punk->QueryInterface(IID_IDispatch, (void **)&m_pdisp);
		if(FAILED(m_hr))
		{
			nError = E_OBJECT_NOT_CREATED;
			goto error;
		}
	}
	CATCH_ALL( e )
	{
		nError = E_OBJECT_NOT_CREATED;
		goto error;
	}
    END_CATCH_ALL

    m_punk->Release();
	m_punk = NULL;
	m_bCreated = TRUE;
	m_bByRef   = FALSE;

    return S_OK;
    
error:
	m_bCreated = FALSE;

    if(m_punk) 
	{
		m_punk->Release();
		m_punk = NULL;
	}
    if(m_pdisp) 
	{
		m_pdisp->Release();
		m_pdisp = NULL;
	}
	
    return nError;
}

void CActiveX::DestroyObject()
{
	m_szCOMName[0] = _T('\0');
	if (m_bByRef)
	{
		m_punk = NULL;
		m_pdisp = NULL;
		return;
	}

	m_bCreated = FALSE;

    if(m_punk) 
	{
		m_punk->Release();
		m_punk = NULL;
	}

    if(m_pdisp) 
	{
		m_pdisp->Release();
		m_pdisp = NULL;
	}
}

/*****************************************************************************
 * ExecuteMethod                                                             *
 * Função que executa um método                                              *
 * Esta função recebe um formatador de parâmetros, futuramente será          *
 * substituida por ExecuteMethod, que não precisará do formatador            *
 *****************************************************************************/
UINT CActiveX::ExecuteMethod(TCHAR * pszMetodo, TCHAR * pszTipo, TCHAR * pszParametro, VARIANT &ret)
{
	DISPPARAMS dispparams;
	EXCEPINFO FAR pExcepInfo;
	DISPID dispid;
	LPOLESTR pOleStr;
	unsigned int uArgErr = 0;
	int nTotalParametros;
	int nRetorno;
	int nPos;

	memset(&dispparams, NULL, sizeof(dispparams));

#ifndef UNICODE
	AnsiToUnicode(pszMetodo, &pOleStr);
#else
	pOleStr = pszMetodo; 
#endif

	nTotalParametros = GetWordSeparatedByCount(CString(pszParametro), _T(","), _T("|"));

	dispparams.cArgs = nTotalParametros;
	dispparams.rgvarg = new VARIANTARG[nTotalParametros];
	dispparams.cNamedArgs = 0;

	nPos = -1;
	for (int nCont = nTotalParametros; nCont > 0; nCont--)
	{
		CString strParametro = GetWordSeparatedBy(CString(pszParametro), nCont, _T(","), _T("|"));
		CString strTipo = GetWordSeparatedBy(CString(pszTipo), nCont, _T(","), _T("|"));

		strTipo.MakeUpper();
		strTipo = Alltrim(strTipo);
		
		nPos++;
		if (strTipo == _T("TCURRENCY"))
		{
			dispparams.rgvarg[nPos].vt = VT_R8;
			Val(strParametro, (double)dispparams.rgvarg[nPos].dblVal);
		}
		else if (strTipo == _T("TDATE"))
		{
			COleDateTime oleDate;
			int nDia, nMes, nAno;

			Val(GetWordSeparatedBy(strParametro, 1, _T("/"), _T("|")), nDia);
			Val(GetWordSeparatedBy(strParametro, 2, _T("/"), _T("|")), nMes);
			Val(GetWordSeparatedBy(strParametro, 3, _T("/"), _T("|")), nAno);
			
			oleDate.SetDate(nAno, nMes, nDia);

			dispparams.rgvarg[nPos].vt = VT_DATE;
			dispparams.rgvarg[nPos].date = oleDate.m_dt;
		}
		else if (strTipo == _T("TDOUBLE"))
		{
			dispparams.rgvarg[nPos].vt = VT_R8;
			Val(strParametro, dispparams.rgvarg[nPos].dblVal);
		}
		else if (strTipo == _T("TINTEGER"))
		{
			dispparams.rgvarg[nPos].vt = VT_I2;
			Val(strParametro, dispparams.rgvarg[nPos].iVal);
		}
		else if (strTipo == _T("TLONG"))
		{
			dispparams.rgvarg[nPos].vt = VT_I4;
			Val(strParametro, dispparams.rgvarg[nPos].lVal);
		}
		else if (strTipo == _T("TSINGLE"))
		{
			dispparams.rgvarg[nPos].vt = VT_R4;
			Val(strParametro, dispparams.rgvarg[nPos].fltVal);
		}
		else if (strTipo == _T("TSTRING"))
		{

			dispparams.rgvarg[nPos].vt = VT_BSTR;
			dispparams.rgvarg[nPos].bstrVal = strParametro.AllocSysString();
		}
	}

	nRetorno = S_OK;

	TRY
	{
		m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
		if (FAILED(m_hr))
		{
			nRetorno = E_GETIDOFNAMES_ERROR;
			goto Finalizar;
		}

		memset(&pExcepInfo, 0, sizeof(pExcepInfo));
		m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, &ret, &pExcepInfo, &uArgErr);

		m_strErroDescription = _T("");
		if (FAILED(m_hr))
		{
			nRetorno = E_INVOKE_METHOD_ERROR;
			goto Finalizar;
		}
	}
	CATCH_ALL(e)
	{
		goto Finalizar;
	}
	END_CATCH_ALL

Finalizar:

	for (nCont = 0; nCont < nTotalParametros; nCont++)
	{
		if (dispparams.rgvarg[nCont].vt == VT_BSTR)
			::SysFreeString(dispparams.rgvarg[nCont].bstrVal);
	}
	delete dispparams.rgvarg;

	return S_OK;
}

UINT CActiveX::ExecuteMethod(TCHAR * pszMetodo, TCHAR * pszArgumentList, ...)
{
	DISPPARAMS dispparams;
	VARTYPE * pVarType;
	EXCEPINFO FAR pExcepInfo;
	DISPID dispid;
	LPOLESTR pOleStr;
	unsigned int uArgErr = 0;

	memset(&dispparams, 0, sizeof(dispparams));

#ifndef UNICODE
	AnsiToUnicode(pszMetodo, &pOleStr);
#else
	pOleStr = pszMetodo; 
#endif

	va_list vl;  
	va_start(vl, pszArgumentList);  
	int niCont = 1;
	
	UINT nLen = (UINT)_tcslen(pszArgumentList);

	for(register UINT i = 0; i < nLen; i++, pszArgumentList++)
	{
		if(*pszArgumentList == ',')
			niCont++;		
	}

	pszArgumentList -= nLen;

	pVarType = new VARTYPE[niCont];

	CString strAux;
	strAux.Empty();
	int nElement = 0;
	for(i = 0; i < nLen; i++, pszArgumentList++)
	{

		if(*pszArgumentList != _T(','))
		{
			strAux += *pszArgumentList;
		}

		if  (*pszArgumentList == _T(',') || i + 1 == nLen)
		{
			strAux.TrimLeft();
			strAux.TrimRight();
			strAux.MakeLower();

			if (strAux == _T("int") || strAux == _T("short"))
				pVarType[nElement] = VT_I2;
			else if (strAux == _T("long"))
				pVarType[nElement] = VT_I4;

			else if (strAux == _T("float"))
				pVarType[nElement] = VT_R4;
			else if (strAux == _T("double"))
				pVarType[nElement] = VT_R8;

			else if (strAux == _T("bool"))
				pVarType[nElement] = VT_BOOL;
			else if (strAux == _T("bstr"))
				pVarType[nElement] = VT_BSTR;

			else if (strAux == _T("idispatch"))
				pVarType[nElement] = VT_DISPATCH;

			else if(strAux == _T("pint"))
				pVarType[nElement] = VT_BYREF | VT_I2;
			else if (strAux == _T("plong"))
				pVarType[nElement] = VT_BYREF | VT_I4;
			else if (strAux == _T("pbool"))
				pVarType[nElement] = VT_BYREF | VT_BOOL;
			else if (strAux == _T("pbstr"))
				pVarType[nElement] = VT_BYREF | VT_BSTR;

			else if(strAux == _T("uint"))
				pVarType[nElement] = VT_UI2;
			else if (strAux == _T("ulong"))
				pVarType[nElement] = VT_UI4;

			else if(strAux == _T("puint"))
				pVarType[nElement] = VT_BYREF | VT_UI2;
			else if (strAux == _T("pulong"))
				pVarType[nElement] = VT_BYREF | VT_UI4;

			else if (strAux == _T("pfloat"))
				pVarType[nElement] =  VT_BYREF | VT_R4;
			else if (strAux == _T("pdouble"))
				pVarType[nElement] =  VT_BYREF | VT_R8;

			else if(strAux == _T("variant"))
				pVarType[nElement] = VT_BYREF | VT_VARIANT;
			else if (strAux == _T("pvariant"))
				pVarType[nElement] = VT_BYREF | VT_VARIANT;

			else if(strAux == _T("safearray"))
				pVarType[nElement] = VT_SAFEARRAY; //VT_ARRAY | VT_VARIANT;
			else if (strAux == _T("psafearray"))
				pVarType[nElement] = VT_BYREF | VT_SAFEARRAY;

			nElement++;
			strAux.Empty();
		}
	}

	dispparams.cArgs = niCont;
	dispparams.rgvarg = new VARIANTARG[niCont];
	dispparams.cNamedArgs = 0;

	int nElementVarType = 0;
	for (nElement = niCont-1; nElement >= 0; nElement--)
	{
		dispparams.rgvarg[nElement].vt = pVarType[nElementVarType];

		switch(pVarType[nElementVarType])
		{
			case VT_I2:
				dispparams.rgvarg[nElement].iVal = va_arg(vl, short);
				break;
			case VT_I4:
				dispparams.rgvarg[nElement].lVal = va_arg(vl, long);
				break;
			case VT_R4:
				dispparams.rgvarg[nElement].fltVal = va_arg(vl, float);
				break;
			case VT_R8:
				dispparams.rgvarg[nElement].dblVal = va_arg(vl, double);
				break;
			case VT_BOOL:
				dispparams.rgvarg[nElement].boolVal = va_arg(vl, short);
				break;
			case VT_BSTR:
				#ifdef UNICODE
					dispparams.rgvarg[nElement].bstrVal = va_arg(vl, TCHAR*);
					#ifdef DEBUG
						afxDump << _T("\n") << dispparams.rgvarg[nElement].bstrVal << _T("\n");
					#endif
				#else
					dispparams.rgvarg[nElement].bstrVal = _bstr_t(va_arg(vl, TCHAR*));
				#endif
				break;

			case VT_DISPATCH:
				dispparams.rgvarg[nElement].pdispVal = va_arg(vl, LPDISPATCH);
				break;

			case VT_BYREF | VT_I2:
				dispparams.rgvarg[nElement].piVal = va_arg(vl, short*);
				break;
			case VT_BYREF | VT_I4:
				dispparams.rgvarg[nElement].plVal = va_arg(vl, long*);
				break;
			case VT_BYREF | VT_R4:
				dispparams.rgvarg[nElement].pfltVal = va_arg(vl, float*);
				break;
			case VT_BYREF | VT_R8:
				dispparams.rgvarg[nElement].pdblVal = va_arg(vl, double*);
				break;
			case VT_BYREF | VT_BOOL:
				dispparams.rgvarg[nElement].pboolVal = va_arg(vl, short*);
				break;
			case VT_BYREF | VT_BSTR:
				#ifdef UNICODE
					dispparams.rgvarg[nElement].pbstrVal = va_arg(vl, TCHAR**);
				#else
					//dispparams.rgvarg[nElement].pbstrVal = _bstr_t(va_arg(vl, _bstr_t(TCHAR**)));
				#endif
				break;

			case VT_UI2:
				dispparams.rgvarg[nElement].uiVal = va_arg(vl, unsigned short);
				break;
			case VT_UI4:
				dispparams.rgvarg[nElement].ulVal = va_arg(vl, unsigned long);
				break;

			case VT_BYREF | VT_UI2:
				dispparams.rgvarg[nElement].puiVal = va_arg(vl, unsigned short*);
				break;
			case VT_BYREF | VT_UI4:
				dispparams.rgvarg[nElement].pulVal = va_arg(vl, unsigned long*);
				break;

			case VT_BYREF | VT_VARIANT:
				dispparams.rgvarg[nElement].pvarVal = va_arg(vl, VARIANT*);
				break;

			case VT_SAFEARRAY:
			//case VT_ARRAY | VT_VARIANT:
				dispparams.rgvarg[nElement].parray = va_arg(vl, SAFEARRAY*);
				break;

			case VT_BYREF | VT_SAFEARRAY:
				dispparams.rgvarg[nElement].pparray = va_arg(vl, SAFEARRAY**);
				break;
		}

		nElementVarType++;
	}

	TRY
	{
		m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
		if (FAILED(m_hr))
		{
			va_end(vl);
			delete dispparams.rgvarg;
 			delete pVarType;
			return E_GETIDOFNAMES_ERROR;
		}

		memset(&pExcepInfo, 0, sizeof(pExcepInfo));
		m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, NULL, &pExcepInfo, &uArgErr);

		m_strErroDescription = _T("");
		if (FAILED(m_hr))
		{
			va_end(vl);
			delete dispparams.rgvarg;
 			delete pVarType;
			m_strErroDescription = pExcepInfo.bstrDescription;
			return E_INVOKE_METHOD_ERROR;
		}

		va_end(vl);

		delete dispparams.rgvarg;
 		delete pVarType;
	}
	CATCH_ALL(e)
	{
		va_end(vl);

		delete dispparams.rgvarg;
 		delete pVarType;
	}
	END_CATCH_ALL

	return 0;
}

UINT CActiveX::ExecuteMethodRet(TCHAR * pszMetodo, VARIANT &ret, TCHAR * pszArgumentList, ...)
{
	DISPPARAMS dispparams;
	VARTYPE * pVarType;
	DISPID dispid;
	LPOLESTR pOleStr;
	EXCEPINFO FAR pExcepInfo;

	memset(&ret, 0, sizeof(VARIANT));
#ifndef UNICODE
	AnsiToUnicode(pszMetodo, &pOleStr);
#else
	pOleStr = pszMetodo; 
#endif

	va_list vl;  
	va_start(vl, pszArgumentList);  
	int niCont = 1;
	
	UINT nLen = (UINT)_tcslen(pszArgumentList);

	for(register UINT i = 0; i < nLen; i++, pszArgumentList++)
	{
		if(*pszArgumentList == ',')
			niCont++;		
	}

	pszArgumentList -= nLen;

	pVarType = new VARTYPE[niCont];

	CString strAux;
	strAux.Empty();
	int nElement = 0;
	for(i = 0; i < nLen; i++, pszArgumentList++)
	{

		if(*pszArgumentList != ',')
		{
			strAux += *pszArgumentList;
		}

		if  (*pszArgumentList == ',' || i + 1 == nLen)
		{
			strAux.TrimLeft();
			strAux.TrimRight();
			strAux.MakeLower();

			if (strAux == _T("int") || strAux == _T("short"))
				pVarType[nElement] = VT_I2;
			else if (strAux == _T("long"))
				pVarType[nElement] = VT_I4;

			else if (strAux == _T("float"))
				pVarType[nElement] = VT_R4;
			else if (strAux == _T("double"))
				pVarType[nElement] = VT_R8;

			else if (strAux == _T("bool"))
				pVarType[nElement] = VT_BOOL;
			else if (strAux == _T("bstr"))
				pVarType[nElement] = VT_BSTR;

			else if (strAux == _T("idispatch"))
				pVarType[nElement] = VT_DISPATCH;

			else if(strAux == _T("pint"))
				pVarType[nElement] = VT_BYREF | VT_I2;
			else if (strAux == _T("plong"))
				pVarType[nElement] = VT_BYREF | VT_I4;
			else if (strAux == _T("pbool"))
				pVarType[nElement] = VT_BYREF | VT_BOOL;
			else if (strAux == _T("pbstr"))
				pVarType[nElement] = VT_BYREF | VT_BSTR;

			else if(strAux == _T("uint"))
				pVarType[nElement] = VT_UI2;
			else if (strAux == _T("ulong"))
				pVarType[nElement] = VT_UI4;

			else if (strAux == _T("pfloat"))
				pVarType[nElement] =  VT_BYREF | VT_R4;
			else if (strAux == _T("pdouble"))
				pVarType[nElement] =  VT_BYREF | VT_R8;

			else if(strAux == _T("puint"))
				pVarType[nElement] = VT_BYREF | VT_UI2;
			else if (strAux == _T("pulong"))
				pVarType[nElement] = VT_BYREF | VT_UI4;

			else if(strAux == _T("variant"))
				pVarType[nElement] = VT_BYREF | VT_VARIANT;
			else if (strAux == _T("pvariant"))
				pVarType[nElement] = VT_BYREF | VT_VARIANT;

			else if(strAux == _T("safearray"))
				pVarType[nElement] = VT_ARRAY|VT_VARIANT;
			else if (strAux == _T("psafearray"))
				pVarType[nElement] = VT_BYREF | VT_SAFEARRAY;

			nElement++;
			strAux.Empty();
		}
	}

	dispparams.cArgs = niCont;
	dispparams.rgvarg = new VARIANTARG[niCont];
	dispparams.cNamedArgs = 0;

	int nElementVarType = 0;
	for (nElement = niCont-1; nElement >= 0; nElement--)
	{
		dispparams.rgvarg[nElement].vt = pVarType[nElementVarType];

		switch(pVarType[nElementVarType])
		{
			case VT_I2:
				dispparams.rgvarg[nElement].iVal = va_arg(vl, short);
				break;
			case VT_I4:
				dispparams.rgvarg[nElement].lVal = va_arg(vl, long);
				break;
			case VT_R4:
				dispparams.rgvarg[nElement].fltVal = va_arg(vl, float);
				break;
			case VT_R8:
				dispparams.rgvarg[nElement].dblVal = va_arg(vl, double);
				break;
			case VT_BOOL:
				dispparams.rgvarg[nElement].boolVal = va_arg(vl, short);
				break;
			case VT_BSTR:
				#ifdef UNICODE
					dispparams.rgvarg[nElement].bstrVal = va_arg(vl, TCHAR*);
				#else
					dispparams.rgvarg[nElement].bstrVal = _bstr_t(va_arg(vl, TCHAR*));
				#endif
				break;

			case VT_DISPATCH:
				dispparams.rgvarg[nElement].pdispVal = va_arg(vl, LPDISPATCH);
				break;

			case VT_BYREF | VT_I2:
				dispparams.rgvarg[nElement].piVal = va_arg(vl, short*);
				break;
			case VT_BYREF | VT_I4:
				dispparams.rgvarg[nElement].plVal = va_arg(vl, long*);
				break;
			case VT_BYREF | VT_R4:
				dispparams.rgvarg[nElement].pfltVal = va_arg(vl, float*);
				break;
			case VT_BYREF | VT_R8:
				dispparams.rgvarg[nElement].pdblVal = va_arg(vl, double*);
				break;
			case VT_BYREF | VT_BOOL:
				dispparams.rgvarg[nElement].pboolVal = va_arg(vl, short*);
				break;
			case VT_BYREF | VT_BSTR:
				#ifdef UNICODE
					dispparams.rgvarg[nElement].pbstrVal = va_arg(vl, TCHAR**);
				#else
					//dispparams.rgvarg[nElement].pbstrVal = _bstr_t(va_arg(vl, _bstr_t(TCHAR**)));
				#endif
				break;

			case VT_UI2:
				dispparams.rgvarg[nElement].uiVal = va_arg(vl, unsigned short);
				break;
			case VT_UI4:
				dispparams.rgvarg[nElement].ulVal = va_arg(vl, unsigned long);
				break;

			case VT_BYREF | VT_UI2:
				dispparams.rgvarg[nElement].puiVal = va_arg(vl, unsigned short*);
				break;
			case VT_BYREF | VT_UI4:
				dispparams.rgvarg[nElement].pulVal = va_arg(vl, unsigned long*);
				break;

			case VT_BYREF | VT_VARIANT:
				dispparams.rgvarg[nElement].pvarVal = va_arg(vl, VARIANT*);
				break;

			case VT_ARRAY|VT_VARIANT:
				dispparams.rgvarg[nElement].parray = va_arg(vl, SAFEARRAY*);
				break;

			case VT_BYREF | VT_SAFEARRAY:
				dispparams.rgvarg[nElement].pparray = va_arg(vl, SAFEARRAY**);
				break;
		}

		nElementVarType++;
	}

	TRY
	{
		m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
		if (FAILED(m_hr))
		{
			va_end(vl);
			delete dispparams.rgvarg;
 			delete pVarType;
			return E_GETIDOFNAMES_ERROR;
		}

		m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, &ret, &pExcepInfo, NULL);
		m_strErroDescription = _T("");
		if (FAILED(m_hr))
		{
			va_end(vl);
			delete dispparams.rgvarg;
 			delete pVarType;
			m_strErroDescription = pExcepInfo.bstrDescription;
			return E_INVOKE_METHOD_ERROR;
		}

		va_end(vl);

		delete dispparams.rgvarg;
 		delete pVarType;
	}
	CATCH_ALL(e)
	{
		va_end(vl);

		delete dispparams.rgvarg;
 		delete pVarType;
	}
	END_CATCH_ALL

	return 0;
}

/*****************************************************************************
 * ExecuteMethodSafe                                                         *
 * Função que executa um método                                              *
 * Esta função recebe um formatador de parâmetros, futuramente será          *
 * substituida por ExecuteMethod, que não precisará do formatador            *
 *****************************************************************************/
UINT CActiveX::ExecuteMethodSafe(TCHAR *pszMetodo, SAFEARRAY *pSA)
{
	DISPPARAMS dispparams;
	DISPID dispid;
	LPOLESTR pOleStr;
	VARIANT vtValor;
	long    nIndices[1];
	int nElement = 0;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszMetodo, &pOleStr);
#else
	pOleStr = pszMetodo; 
#endif

	long nLBound, nUBound;
	int niCont;
	
	SafeArrayGetLBound(pSA, 1, &nLBound);
	SafeArrayGetUBound(pSA, 1, &nUBound);
	
	niCont = nUBound - nLBound +1;

	dispparams.cArgs = niCont;
	dispparams.rgvarg = new VARIANTARG[niCont];
	dispparams.cNamedArgs = 0;

	int nElementVarType = 0; 
	VariantInit(&vtValor);
	for (nElement = niCont-1; nElement >= 0; nElement--)
	{
		VariantClear(&vtValor);
		nIndices[0] = nElementVarType;

		SafeArrayGetElement(pSA, nIndices, &vtValor);
		dispparams.rgvarg[nElement] = vtValor;

		/*switch(vtValor.vt)
		{
			case VT_I2:
				dispparams.rgvarg[nElement].iVal = va_arg(vl, short);
				break;
			case VT_I4:
				dispparams.rgvarg[nElement].lVal = va_arg(vl, long);
				break;
			case VT_BOOL:
				dispparams.rgvarg[nElement].boolVal = va_arg(vl, short);
				break;
			case VT_BSTR:
				#ifdef UNICODE
					dispparams.rgvarg[nElement].bstrVal = va_arg(vl, TCHAR*);
				#else
					dispparams.rgvarg[nElement].bstrVal = _bstr_t(va_arg(vl, TCHAR*));
				#endif
				break;

			case VT_DISPATCH:
				dispparams.rgvarg[nElement].pdispVal = va_arg(vl, LPDISPATCH);
				break;

			case VT_BYREF | VT_I2:
				dispparams.rgvarg[nElement].piVal = va_arg(vl, short*);
				break;
			case VT_BYREF | VT_I4:
				dispparams.rgvarg[nElement].plVal = va_arg(vl, long*);
				break;
			case VT_BYREF | VT_BOOL:
				dispparams.rgvarg[nElement].pboolVal = va_arg(vl, short*);
				break;
			case VT_BYREF | VT_BSTR:
				#ifdef UNICODE
					dispparams.rgvarg[nElement].pbstrVal = va_arg(vl, TCHAR**);
				#else
					//dispparams.rgvarg[nElement].pbstrVal = _bstr_t(va_arg(vl, _bstr_t(TCHAR**)));
				#endif
				break;

			case VT_UI2:
				dispparams.rgvarg[nElement].uiVal = va_arg(vl, unsigned short);
				break;
			case VT_UI4:
				dispparams.rgvarg[nElement].ulVal = va_arg(vl, unsigned long);
				break;

			case VT_BYREF | VT_UI2:
				dispparams.rgvarg[nElement].puiVal = va_arg(vl, unsigned short*);
				break;
			case VT_BYREF | VT_UI4:
				dispparams.rgvarg[nElement].pulVal = va_arg(vl, unsigned long*);
				break;

			case VT_BYREF | VT_VARIANT:
				dispparams.rgvarg[nElement].pvarVal = va_arg(vl, VARIANT*);
				break;

			case VT_ARRAY|VT_VARIANT:
				dispparams.rgvarg[nElement].parray = va_arg(vl, SAFEARRAY*);
				break;

			case VT_BYREF | VT_SAFEARRAY:
				dispparams.rgvarg[nElement].pparray = va_arg(vl, SAFEARRAY**);
				break;
		}*/

		nElementVarType++;
	}

	TRY
	{
		m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
		if (FAILED(m_hr))
		{
			delete dispparams.rgvarg;
			return E_GETIDOFNAMES_ERROR;
		}

		m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, NULL, &pExcepInfo, NULL);
		m_strErroDescription = _T("");
		if (FAILED(m_hr))
		{
			m_strErroDescription = pExcepInfo.bstrDescription;
			delete dispparams.rgvarg;
			return E_INVOKE_METHOD_ERROR;
		}

		delete dispparams.rgvarg;
	}
	CATCH_ALL(e)
	{
		delete dispparams.rgvarg;
	}
	END_CATCH_ALL

	return 0;
}

/*****************************************************************************
 * ExecuteMethodNoArg                                                        *
 * Função que executa um método                                              *
 *                                                                           *
 *****************************************************************************/
UINT CActiveX::ExecuteMethodNoArg(TCHAR * pszMetodo)
{
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	DISPID dispid;
	LPOLESTR pOleStr;
	EXCEPINFO pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszMetodo, &pOleStr);
#else
	pOleStr = pszMetodo; 
#endif

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
		return E_GETIDOFNAMES_ERROR;

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparamsNoArgs, NULL, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		return E_INVOKE_METHOD_ERROR;
	}
	
	return S_OK;
}

UINT CActiveX::ExecuteMethodNoArgRet(TCHAR * pszMetodo, VARIANT &ret)
{
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	DISPID dispid;
	LPOLESTR pOleStr;
	EXCEPINFO pExcepInfo;

	memset(&ret, 0, sizeof(VARIANT));
#ifndef UNICODE
	AnsiToUnicode(pszMetodo, &pOleStr);
#else
	pOleStr = pszMetodo; 
#endif

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
		return E_GETIDOFNAMES_ERROR;

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparamsNoArgs, &ret, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		return E_INVOKE_METHOD_ERROR;
	}
	
	return S_OK;
}

/*****************************************************************************
 * SetProperty                                                               *
 * Atribui um valor para uma propriedade                                     *
 *                                                                           *
 *****************************************************************************/
UINT CActiveX::SetProperty(TCHAR * pszProperty, short nsParametro )
{	
	DISPPARAMS dispparams;
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	//Configurando propriedade ConnectionString
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = 1;
	dispparams.cNamedArgs = 1;
	dispparams.rgvarg = new VARIANTARG[dispparams.cArgs];
	memset(dispparams.rgvarg, 0, sizeof(VARIANTARG) * dispparams.cArgs);
	dispparams.rgvarg[0].vt      = VT_I2;
	dispparams.rgvarg[0].iVal    = nsParametro;
	dispparams.rgdispidNamedArgs = &mydispid;

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		delete dispparams.rgvarg;
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &dispparams,NULL, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		delete dispparams.rgvarg;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	delete dispparams.rgvarg;

	return S_OK;
}

UINT CActiveX::SetProperty(TCHAR * pszProperty, float nsParametro)
{	
	DISPPARAMS dispparams;
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	//Configurando propriedade ConnectionString
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = 1;
	dispparams.cNamedArgs = 1;
	dispparams.rgvarg = new VARIANTARG[dispparams.cArgs];
	memset(dispparams.rgvarg, 0, sizeof(VARIANTARG) * dispparams.cArgs);
	dispparams.rgvarg[0].vt      = VT_R4;
	dispparams.rgvarg[0].fltVal  = nsParametro;
	dispparams.rgdispidNamedArgs = &mydispid;

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		delete dispparams.rgvarg;
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &dispparams,NULL, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		delete dispparams.rgvarg;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	delete dispparams.rgvarg;

	return S_OK;
}

UINT CActiveX::SetProperty(TCHAR * pszProperty, DWORD nParametro)
{	
	DISPPARAMS dispparams;
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	//Configurando propriedade ConnectionString
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = 1;
	dispparams.cNamedArgs = 1;
	dispparams.rgvarg = new VARIANTARG[dispparams.cArgs];
	memset(dispparams.rgvarg, 0, sizeof(VARIANTARG) * dispparams.cArgs);
	dispparams.rgvarg[0].vt      = VT_UI4;
	dispparams.rgvarg[0].ulVal   = nParametro;
	dispparams.rgdispidNamedArgs = &mydispid;

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		delete dispparams.rgvarg;
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &dispparams,NULL, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		delete dispparams.rgvarg;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	delete dispparams.rgvarg;

	return S_OK;
}

UINT CActiveX::SetProperty(TCHAR * pszProperty, long  nlParametro )
{	
	DISPPARAMS dispparams;
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	//Configurando propriedade ConnectionString
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = 1;
	dispparams.cNamedArgs = 1;
	dispparams.rgvarg = new VARIANTARG[dispparams.cArgs];
	memset(dispparams.rgvarg, 0, sizeof(VARIANTARG) * dispparams.cArgs);
	dispparams.rgvarg[0].vt      = VT_I4;
	dispparams.rgvarg[0].lVal    = nlParametro;
	dispparams.rgdispidNamedArgs = &mydispid;

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		delete dispparams.rgvarg;
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &dispparams,NULL, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		delete dispparams.rgvarg;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	delete dispparams.rgvarg;

	return S_OK;
}

UINT CActiveX::SetProperty(TCHAR * pszProperty, CURRENCY ncParametro )
{	
	DISPPARAMS dispparams;
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	//Configurando propriedade ConnectionString
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = 1;
	dispparams.cNamedArgs = 1;
	dispparams.rgvarg = new VARIANTARG[dispparams.cArgs];
	memset(dispparams.rgvarg, 0, sizeof(VARIANTARG) * dispparams.cArgs);
	dispparams.rgvarg[0].vt      = VT_CY;
	dispparams.rgvarg[0].cyVal.int64   = ncParametro.int64;
	dispparams.rgdispidNamedArgs = &mydispid;

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		delete dispparams.rgvarg;
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &dispparams,NULL, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		delete dispparams.rgvarg;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	delete dispparams.rgvarg;

	return S_OK;
}

UINT CActiveX::SetProperty(TCHAR * pszProperty, DATE ndParametro)
{	
	DISPPARAMS dispparams;
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	//Configurando propriedade ConnectionString
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = 1;
	dispparams.cNamedArgs = 1;
	dispparams.rgvarg = new VARIANTARG[dispparams.cArgs];
	memset(dispparams.rgvarg, 0, sizeof(VARIANTARG) * dispparams.cArgs);
	dispparams.rgvarg[0].vt      = VT_DATE;
	dispparams.rgvarg[0].date    = ndParametro;
	dispparams.rgdispidNamedArgs = &mydispid;

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		delete dispparams.rgvarg;
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &dispparams,NULL, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		delete dispparams.rgvarg;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	delete dispparams.rgvarg;

	return S_OK;
}

UINT CActiveX::SetProperty(TCHAR * pszProperty, TCHAR * pszParametro)
{	
	DISPPARAMS dispparams;
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	LPOLESTR pOleProperty;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
	AnsiToUnicode(pszParametro, &pOleProperty);
#else
	pOleStr      = pszProperty; 
	pOleProperty = pszParametro;
#endif

	//Configurando propriedade ConnectionString
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = 1;
	dispparams.cNamedArgs = 1;
	dispparams.rgvarg = new VARIANTARG[dispparams.cArgs];
	memset(dispparams.rgvarg, 0, sizeof(VARIANTARG) * dispparams.cArgs);
	dispparams.rgvarg[0].vt      = VT_BSTR;
	dispparams.rgvarg[0].bstrVal = pOleProperty;
	dispparams.rgdispidNamedArgs = &mydispid;

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		delete dispparams.rgvarg;
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &dispparams,NULL, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		delete dispparams.rgvarg;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	delete dispparams.rgvarg;

	return S_OK;
}

UINT CActiveX::SetProperty(TCHAR * pszProperty, BOOL bParametro)
{	
	DISPPARAMS dispparams;
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	//Configurando propriedade ConnectionString
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = 1;
	dispparams.cNamedArgs = 1;
	dispparams.rgvarg = new VARIANTARG[dispparams.cArgs];
	memset(dispparams.rgvarg, 0, sizeof(VARIANTARG) * dispparams.cArgs);
	dispparams.rgvarg[0].vt      = VT_BOOL;
	dispparams.rgvarg[0].boolVal = bParametro;
	dispparams.rgdispidNamedArgs = &mydispid;

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		delete dispparams.rgvarg;
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &dispparams,NULL, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		delete dispparams.rgvarg;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	delete dispparams.rgvarg;

	return S_OK;
}

UINT CActiveX::SetProperty(TCHAR * pszProperty, LPDISPATCH pdisp)
{	
	DISPPARAMS dispparams;
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	//Configurando propriedade ConnectionString
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = 1;
	dispparams.cNamedArgs = 1;
	dispparams.rgvarg = new VARIANTARG[dispparams.cArgs];
	memset(dispparams.rgvarg, 0, sizeof(VARIANTARG) * dispparams.cArgs);
	dispparams.rgvarg[0].vt		  = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = pdisp;
	dispparams.rgdispidNamedArgs = &mydispid;

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		delete dispparams.rgvarg;
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUTREF, &dispparams,NULL, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		delete dispparams.rgvarg;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	delete dispparams.rgvarg;

	return S_OK;
}

UINT CActiveX::SetProperty(TCHAR * pszProperty, VARIANT vtParametro)
{	
	DISPPARAMS dispparams;
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	//Configurando propriedade ConnectionString
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = 1;
	dispparams.cNamedArgs = 1;
	dispparams.rgvarg = new VARIANTARG[dispparams.cArgs];
	memset(dispparams.rgvarg, 0, sizeof(VARIANTARG) * dispparams.cArgs);
	memcpy(dispparams.rgvarg, &vtParametro, sizeof(VARIANT));
	dispparams.rgdispidNamedArgs = &mydispid;

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		delete dispparams.rgvarg;
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUTREF, &dispparams,NULL, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		delete dispparams.rgvarg;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	delete dispparams.rgvarg;

	return S_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*****************************************************************************
 * GetProperty                                                               *
 * Recebe o valor de uma propriedade	                                     *
 *                                                                           *
 *****************************************************************************/
UINT CActiveX::GetProperty(TCHAR* pszProperty, short& pParametro )
{	
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	VARIANT pVarResult;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	memset(&pVarResult, 0, sizeof(VARIANT));

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	switch(pVarResult.vt)
	{
		case VT_I2:
			pParametro =  pVarResult.iVal;
			break;
		case VT_I4:
			pParametro =  (short)pVarResult.lVal;
			break;
		case VT_UI2:
			pParametro =  (short)pVarResult.uiVal;
			break;
		case VT_UI4:
			pParametro =  (short)pVarResult.ulVal;
			break;
		case VT_R4:
			pParametro =  (short)pVarResult.fltVal;
			break;
		case VT_R8:
			pParametro =  (short)pVarResult.dblVal;
			break;
		case VT_I1:
			pParametro =  (short)pVarResult.intVal;
			break;
		case VT_DECIMAL:
			pParametro = (short)pVarResult.lVal;
			break;
		case VT_NULL:
			pParametro = 0;
			break;
		case VT_EMPTY:
			pParametro = 0;
			break;
	}

	return S_OK;
}

UINT CActiveX::GetProperty(TCHAR* pszProperty, float& pParametro )
{	
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	VARIANT pVarResult;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	memset(&pVarResult, 0, sizeof(VARIANT));

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	switch(pVarResult.vt)
	{
		case VT_I2:
			pParametro =  pVarResult.iVal;
			break;
		case VT_I4:
			pParametro =  (float)pVarResult.lVal;
			break;
		case VT_UI2:
			pParametro =  pVarResult.uiVal;
			break;
		case VT_UI4:
			pParametro =  (float)pVarResult.ulVal;
			break;
		case VT_R4:
			pParametro =  pVarResult.fltVal;
			break;
		case VT_R8:
			pParametro =  (float)pVarResult.dblVal;
			break;
		case VT_I1:
			pParametro =  (float)pVarResult.intVal;
			break;
		case VT_DECIMAL:
			if (pVarResult.wReserved1 > 0)
			{
				double nValor, nDecimal;
				nValor    = pVarResult.lVal;
				nDecimal  = Potencia(10, pVarResult.wReserved1);
				nValor = (nValor / nDecimal);
				pParametro = (float)nValor;
			}
			else
				pParametro = (float)pVarResult.lVal;
			break;
		case VT_NULL:
		case VT_EMPTY:
			pParametro = 0;
			break;

	}

	return S_OK;
}

UINT CActiveX::GetProperty(TCHAR* pszProperty, DWORD& pParametro )
{	
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	VARIANT pVarResult;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	memset(&pVarResult, 0, sizeof(VARIANT));

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	switch(pVarResult.vt)
	{
		case VT_I2:
			pParametro =  pVarResult.iVal;
			break;
		case VT_I4:
			pParametro =  pVarResult.lVal;
			break;
		case VT_UI2:
			pParametro =  pVarResult.uiVal;
			break;
		case VT_UI4:
			pParametro =  pVarResult.ulVal;
			break;
		case VT_R4:
			pParametro =  (DWORD)pVarResult.fltVal;
			break;
		case VT_R8:
			pParametro =  (DWORD)pVarResult.dblVal;
			break;
		case VT_I1:
			pParametro =  pVarResult.intVal;
			break;
		case VT_DECIMAL:
			if (pVarResult.wReserved1 > 0)
				pParametro = (long)(pVarResult.lVal / Potencia(10, pVarResult.wReserved1));
			else
				pParametro = pVarResult.lVal;
			break;
		case VT_NULL:
		case VT_EMPTY:
			pParametro = 0;
	}

	return S_OK;
}

UINT CActiveX::GetProperty(TCHAR * pszProperty, long&  pParametro )
{	
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	VARIANT pVarResult;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	memset(&pVarResult, 0, sizeof(VARIANT));

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	switch(pVarResult.vt)
	{
		case VT_I2:
			pParametro =  pVarResult.iVal;
			break;
		case VT_I4:
			pParametro =  pVarResult.lVal;
			break;
		case VT_UI2:
			pParametro =  pVarResult.uiVal;
			break;
		case VT_UI4:
			pParametro =  pVarResult.ulVal;
			break;
		case VT_R4:
			pParametro =  (long)pVarResult.fltVal;
			break;
		case VT_R8:
			pParametro =  (long)pVarResult.dblVal;
			break;
		case VT_I1:
			pParametro =  pVarResult.intVal;
			break;
		case VT_DECIMAL:
			pParametro = pVarResult.lVal;
			break;
		case VT_NULL:
		case VT_EMPTY:
			pParametro = 0;
	}

	return S_OK;
}

UINT CActiveX::GetProperty(TCHAR * pszProperty, CURRENCY& pParametro )
{	
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	VARIANT pVarResult;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	memset(&pVarResult, 0, sizeof(VARIANT));

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	pParametro = pVarResult.cyVal;

	return S_OK;
}

UINT CActiveX::GetProperty(TCHAR * pszProperty, DATE& pParametro)
{	
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	VARIANT pVarResult;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	memset(&pVarResult, 0, sizeof(VARIANT));

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	switch(pVarResult.vt)
	{
		case VT_I2:
			pParametro =  pVarResult.iVal;
			break;
		case VT_I4:
			pParametro =  pVarResult.lVal;
			break;
		case VT_UI2:
			pParametro =  pVarResult.uiVal;
			break;
		case VT_UI4:
			pParametro =  pVarResult.ulVal;
			break;
		case VT_R4:
			pParametro =  pVarResult.fltVal;
			break;
		case VT_R8:
			pParametro =  pVarResult.dblVal;
			break;
		case VT_I1:
			pParametro =  pVarResult.intVal;
			break;
		case VT_DECIMAL:
			if (pVarResult.wReserved1 > 0)
				pParametro = pVarResult.lVal / Potencia(10, pVarResult.wReserved1);
			else
				pParametro = pVarResult.lVal;
			break;
		case VT_DATE:
			pParametro =  pVarResult.date;
			break;
		case VT_NULL:
		case VT_EMPTY:
			pParametro = 0;
	}

	
	return S_OK;
}

UINT CActiveX::GetProperty(TCHAR* pszProperty, CString& strParametro)
{	
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	VARIANT pVarResult;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	memset(&pVarResult, 0, sizeof(VARIANT));

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	if (pVarResult.vt == VT_BSTR)
		strParametro = pVarResult.bstrVal;
	else if (pVarResult.vt == VT_BOOL)
	{
		if (pVarResult.boolVal)
		{
			strParametro = _T("Verdadeiro");
		}
		else
		{
			strParametro = _T("Falso");
		}
	}
	else if (pVarResult.vt == VT_NULL)
		strParametro = _T("");

	return S_OK;
}

UINT CActiveX::GetPropertyBool(TCHAR * pszProperty, BOOL&  pParametro )
{	
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	VARIANT pVarResult;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	pParametro = pVarResult.boolVal;

	return S_OK;
}

UINT CActiveX::GetProperty(TCHAR * pszProperty, CActiveX& pActiveX)
{	
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	VARIANT pVarResult;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif
	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		m_strErroDescription = pExcepInfo.bstrDescription;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}


	pActiveX.m_bByRef = TRUE;
	pActiveX.m_pdisp =  pVarResult.pdispVal;

	return S_OK;
}

UINT CActiveX::GetProperty(TCHAR * pszProperty, long nIndex, CActiveX& pActiveX)
{	
	DISPPARAMS dispparams, dispparamsNoArgs = {NULL, NULL, 0, 0};;
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	VARIANT pVarResult;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif

	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = 1;
	dispparams.rgvarg = new VARIANTARG[dispparams.cArgs];
	memset(dispparams.rgvarg, 0, sizeof(VARIANTARG) * dispparams.cArgs);
	dispparams.cNamedArgs = 0;
	dispparams.rgvarg[0].vt = VT_I4;
	dispparams.rgvarg[0].lVal = nIndex;
	dispparams.rgdispidNamedArgs = &mydispid;
	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		delete dispparams.rgvarg;
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET | DISPATCH_METHOD , &dispparams, &pVarResult, &pExcepInfo, NULL);
	m_strErroDescription = _T("");
	if (FAILED(m_hr))
	{
		delete dispparams.rgvarg;
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	pActiveX.m_bByRef = TRUE;
	pActiveX.m_pdisp =  pVarResult.pdispVal;

	delete dispparams.rgvarg;

	return S_OK;
}

UINT CActiveX::GetProperty(TCHAR * pszProperty, TCHAR * pszField, CActiveX& pActiveX)
{	
	DISPPARAMS dispparams, dispparamsNoArgs = {NULL, NULL, 0, 0};;
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr, pOleField;
	VARIANT pVarResult;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
	AnsiToUnicode(pszField, &pOleField);
#else
	pOleStr   = pszProperty; 
	pOleField = pszField;
#endif

	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = 1;
	dispparams.rgvarg = new VARIANTARG[dispparams.cArgs];
	memset(dispparams.rgvarg, 0, sizeof(VARIANTARG) * dispparams.cArgs);
	dispparams.cNamedArgs = 0;
	dispparams.rgvarg[0].vt       = VT_BSTR;
	dispparams.rgvarg[0].bstrVal = pOleField;
	dispparams.rgdispidNamedArgs = &mydispid;
	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		delete dispparams.rgvarg;
		return E_GETIDOFNAMES_ERROR;
	}

	TRY
	{
		m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET | DISPATCH_METHOD, &dispparams, &pVarResult, &pExcepInfo, NULL);
		if (FAILED(m_hr))
		{
			delete dispparams.rgvarg;
			return E_INVOKE_PROPERTYPUT_ERROR;
		}
	}
	CATCH_ALL(e)
	{
		int a = 0;
	}
	END_CATCH_ALL

	pActiveX.m_bByRef = TRUE;
	pActiveX.m_pdisp =  pVarResult.pdispVal;

	delete dispparams.rgvarg;

	return S_OK;
}

UINT CActiveX::GetProperty(TCHAR * pszProperty, VARIANT& vtParametro)
{
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	DISPID dispid;
	DISPID mydispid = DISPID_PROPERTYPUT;
	LPOLESTR pOleStr;
	VARIANT pVarResult;
	EXCEPINFO FAR pExcepInfo;

#ifndef UNICODE
	AnsiToUnicode(pszProperty, &pOleStr);
#else
	pOleStr = pszProperty; 
#endif
	m_hr = m_pdisp->GetIDsOfNames(IID_NULL, &pOleStr, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(m_hr))
	{
		return E_GETIDOFNAMES_ERROR;
	}

	m_hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &pExcepInfo, NULL);
	if (FAILED(m_hr))
	{
		return E_INVOKE_PROPERTYPUT_ERROR;
	}

	vtParametro = pVarResult;

	return S_OK;
}

/*****************************************************************************
 * SetDispatch                                                               *
 * Para que o objeto possa ser configurado por referência                    *
 *                                                                           *
 *****************************************************************************/
void CActiveX::SetDispatch(LPDISPATCH pDispatch)
{
	m_pdisp = pDispatch;
}

CString CActiveX::GetErrorDescription()
{
	return m_strErroDescription;
}
