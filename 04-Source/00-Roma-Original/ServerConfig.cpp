// ServerConfig.cpp : implementation file
//

#include "stdafx.h"
#include "srvroma.h"
#include "ServerConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerConfig dialog


CServerConfig::CServerConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CServerConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CServerConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerConfig)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerConfig, CDialog)
	//{{AFX_MSG_MAP(CServerConfig)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerConfig message handlers
