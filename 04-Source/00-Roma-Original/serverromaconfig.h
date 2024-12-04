#if !defined(AFX_SERVERROMACONFIG_H__2CD68F43_AC45_4642_AC6B_6F1778CBC3B7__INCLUDED_)
#define AFX_SERVERROMACONFIG_H__2CD68F43_AC45_4642_AC6B_6F1778CBC3B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// serverromaconfig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerRomaConfig dialog

class CServerRomaConfig : public CDialog
{
// Construction
public:
	CServerRomaConfig(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CServerRomaConfig)
	enum { IDD = IDD_DIALOG_SERVER_CONFIG };
	CEdit	m_ctrlEditSocketTimeOut;
	CEdit	m_ctrlEditMSMQTimeOut;
	CEdit	m_ctrlEditMSMQMaxFila;
	CEdit	m_ctrlEditSocketMaxConnections;
	CEdit	m_ctrlEditSocketPort;
	CListCtrl	m_ctrlListUsuarios;
	CListBox	m_ctrlListFilas;
	CEdit	m_ctrlEditSocketUsuario;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerRomaConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerRomaConfig)
	afx_msg void OnButtonUsuarioAdicionar();
	afx_msg void OnButtonFilaAdicionar();
	afx_msg void OnButtonFilaCriar();
	afx_msg void OnButtonUsuarioRemover();
	afx_msg void OnButtonFilaRemover();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERROMACONFIG_H__2CD68F43_AC45_4642_AC6B_6F1778CBC3B7__INCLUDED_)
