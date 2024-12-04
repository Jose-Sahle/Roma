// serverromaconfig.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "srvroma.h"
#include "serverromaconfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerRomaConfig dialog


CServerRomaConfig::CServerRomaConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CServerRomaConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerRomaConfig)
	//}}AFX_DATA_INIT
}


void CServerRomaConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerRomaConfig)
	DDX_Control(pDX, IDC_EDIT_SOCKET_TIMEOUT, m_ctrlEditSocketTimeOut);
	DDX_Control(pDX, IDC_EDIT_MSMQ_TIMEOUT, m_ctrlEditMSMQTimeOut);
	DDX_Control(pDX, IDC_EDIT_MSQM_MAX_FILA, m_ctrlEditMSMQMaxFila);
	DDX_Control(pDX, IDC_EDIT_SOCKET_MAX_CONNECTIONS, m_ctrlEditSocketMaxConnections);
	DDX_Control(pDX, IDC_EDIT_SOCKET_PORT, m_ctrlEditSocketPort);
	DDX_Control(pDX, IDC_LIST_USUARIOS, m_ctrlListUsuarios);
	DDX_Control(pDX, IDC_LIST_FILAS, m_ctrlListFilas);
	DDX_Control(pDX, IDC_EDIT_SOCKET_USUARIO, m_ctrlEditSocketUsuario);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerRomaConfig, CDialog)
	//{{AFX_MSG_MAP(CServerRomaConfig)
	ON_BN_CLICKED(IDC_BUTTON_USUARIO_ADICIONAR, OnButtonUsuarioAdicionar)
	ON_BN_CLICKED(IDC_BUTTON_FILA_ADICIONAR, OnButtonFilaAdicionar)
	ON_BN_CLICKED(IDC_BUTTON_FILA_CRIAR, OnButtonFilaCriar)
	ON_BN_CLICKED(IDC_BUTTON_USUARIO_REMOVER, OnButtonUsuarioRemover)
	ON_BN_CLICKED(IDC_BUTTON_FILA_REMOVER, OnButtonFilaRemover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerRomaConfig message handlers

void CServerRomaConfig::OnButtonUsuarioAdicionar() 
{
	
}

void CServerRomaConfig::OnButtonFilaAdicionar() 
{
	
}

void CServerRomaConfig::OnButtonFilaCriar() 
{
	
}

void CServerRomaConfig::OnButtonUsuarioRemover() 
{
	
}

void CServerRomaConfig::OnButtonFilaRemover() 
{
	
}

void CServerRomaConfig::OnOK() 
{
	
	CDialog::OnOK();
}

void CServerRomaConfig::OnCancel() 
{	
	CDialog::OnCancel();
}
