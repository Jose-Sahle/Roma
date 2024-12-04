#if !defined(AFX_CRIARFILA_H__B893FE37_FDC5_4770_B6A0_432D4C18FB84__INCLUDED_)
#define AFX_CRIARFILA_H__B893FE37_FDC5_4770_B6A0_432D4C18FB84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CriarFila.h : header file
//

#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CCriarFila dialog

class CCriarFila : public CDialog
{
public:
	void SetNomeFila(CString strNomeFila);

// Construction
public:
	CCriarFila(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCriarFila)
	enum { IDD = IDD_DIALOG_CRIAR_FILA };
	CStatic	m_ctrlStaticFila;
	CEdit	m_ctrlEditFila;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCriarFila)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCriarFila)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditFila();
	afx_msg void OnButtonCriar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_strNomeFila;

private:
	BOOL CriarFila();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRIARFILA_H__B893FE37_FDC5_4770_B6A0_432D4C18FB84__INCLUDED_)
