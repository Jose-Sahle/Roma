#if !defined(AFX_SERVERROMACONFIG_H__2CD68F43_AC45_4642_AC6B_6F1778CBC3B7__INCLUDED_)
#define AFX_SERVERROMACONFIG_H__2CD68F43_AC45_4642_AC6B_6F1778CBC3B7__INCLUDED_

#include "Resource.h"

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

	CServiceModule* _Module;

// Dialog Data
	//{{AFX_DATA(CServerRomaConfig)
	enum { IDD = IDD_DIALOG_SERVER_CONFIG };
	CEdit	m_ctrlEditSrvTimeOut;
	CEdit	m_ctrlEditFilaErro;
	//}}AFX_DATA
	CListCtrl	m_ctrlListFilas;
	CListCtrl	m_ctrlListUsuarios;

	CButton	m_btnRemoverUsuarios;
	CButton	m_btnBuscarUsuarios;
	CButton	m_ctrlChkIniciarServico;

	CEdit	m_ctrlServidores;

	CEdit	m_ctrlEditSocketTimeOut;
	CEdit	m_ctrlEditSocketMaxConnections;
	CEdit	m_ctrlEditSocketPort;

	CEdit	m_ctrlEditMSMQTimeOut;


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
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFilaAdicionar();
	afx_msg void OnButtonFilaCriar();
	afx_msg void OnButtonUsuarioRemover();
	afx_msg void OnButtonFilaRemover();
	virtual void OnCancel();
	afx_msg void OnButtonRegistrar();
	afx_msg void OnButtonDesregistrar();
	afx_msg void OnButtonBuscarUsuarios();
	virtual void OnOK();
	afx_msg void OnKillfocusEditSocketMaxConnections();
	afx_msg void OnKillfocusEditSocketPort();
	afx_msg void OnKillfocusEditSocketTimeout();
	afx_msg void OnCheckIniciarServico();
	afx_msg void OnButtonFilaExcluirMsmq();
	afx_msg void OnButtonBuscaFilaErro();
	afx_msg void OnSetfocusEditMsmqError();
	afx_msg void OnSetfocusListFilas(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	UINT m_uIDSelecionado;

private:
	void ExcluirFilaMSMQ(CString strFila);
	void RemoverFilaMSMQ(CStringArray &arrayName);
	void RemoverFila(CStringArray &arrayName);
	void AtivaDesativaControlesRemoto(BOOL bValor);
	void PreencherDados();

	BOOL VerificarFilas();
	BOOL VerificarRegistro();
	BOOL SalvarRegistro();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERROMACONFIG_H__2CD68F43_AC45_4642_AC6B_6F1778CBC3B7__INCLUDED_)
