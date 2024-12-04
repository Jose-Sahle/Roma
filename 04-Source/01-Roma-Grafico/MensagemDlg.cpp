// MensagemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "srvroma.h"
#include "MensagemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMensagemDlg dialog


CMensagemDlg::CMensagemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMensagemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMensagemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMensagemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMensagemDlg)
	DDX_Control(pDX, IDC_STATIC_MENSAGEM, m_ctrlMensagem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMensagemDlg, CDialog)
	//{{AFX_MSG_MAP(CMensagemDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMensagemDlg message handlers

void CMensagemDlg::SetMessage(CString strMensagem)
{
	m_ctrlMensagem.SetWindowText(strMensagem);
}

void CMensagemDlg::PostNcDestroy() 
{
	delete this;
	//CDialog::PostNcDestroy();
}

BOOL CMensagemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ModifyStyleEx(NULL, WS_EX_TOPMOST);

	CenterWindow();

	//CFont * font = m_ctrlMensagem.GetFont();
	//font->

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
