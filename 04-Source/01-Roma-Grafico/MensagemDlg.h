#if !defined(AFX_MENSAGEMDLG_H__77E09A95_20E8_44FA_BFB9_953D57D6A433__INCLUDED_)
#define AFX_MENSAGEMDLG_H__77E09A95_20E8_44FA_BFB9_953D57D6A433__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MensagemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMensagemDlg dialog

class CMensagemDlg : public CDialog
{
// Construction
public:
	void SetMessage(CString strMensagem);
	CMensagemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMensagemDlg)
	enum { IDD = IDD_DIALOGBAR_MENSAGEM };
	CStatic	m_ctrlMensagem;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMensagemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMensagemDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENSAGEMDLG_H__77E09A95_20E8_44FA_BFB9_953D57D6A433__INCLUDED_)
