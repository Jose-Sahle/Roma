#if !defined(AFX_LISTADEUSUARIOS_H__250BE62A_191E_4616_8F7B_A51FF61BEDD5__INCLUDED_)
#define AFX_LISTADEUSUARIOS_H__250BE62A_191E_4616_8F7B_A51FF61BEDD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListaDeUsuarios.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CListaDeUsuarios dialog

class CListaDeUsuarios : public CDialog
{
// Construction
public:
	void SetTipo(int nTipo, CString strTitulo);
	BOOL GetOK();
	void GetSelecionador(int nIndex, CString &strName, CString &strFullName, CString &strComment);
	int GetTotalSelecionados();
	CListaDeUsuarios(CWnd* pParent = NULL);   // standard constructor

	void SetServidores(CString strServidores);

// Dialog Data
	//{{AFX_DATA(CListaDeUsuarios)
	enum { IDD = IDD_DIALOG_USUARIOS };
	CListBox	m_ctrlUsuarioSelecionados;
	CListCtrl	m_ctrlListListaUsuario;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListaDeUsuarios)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CListaDeUsuarios)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelecionar();
	afx_msg void OnButtonRemover();
	virtual void OnCancel();
	afx_msg void OnDblclkListListaUsuarios(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnColumnclickListListaUsuarios(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void PreencherListaFilas();
	int m_nTotalSelecionados;
	BOOL m_bOK;
	void PreencherListaUsuarios();

	CString m_strServidores;
	CStringArray arrayName, arrayFullName, arrayComment;
	CString m_strTitulo;

	int	m_nTipo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTADEUSUARIOS_H__250BE62A_191E_4616_8F7B_A51FF61BEDD5__INCLUDED_)
