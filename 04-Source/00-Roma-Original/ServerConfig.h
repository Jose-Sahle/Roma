#if !defined(AFX_SERVERCONFIG_H__14E4EBB4_C2CE_4447_8776_0181C1ED59F5__INCLUDED_)
#define AFX_SERVERCONFIG_H__14E4EBB4_C2CE_4447_8776_0181C1ED59F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerConfig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerConfig dialog

class CServerConfig : public CDialog
{
// Construction
public:
	CServerConfig(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CServerConfig)
	enum { IDD = IDD_SERVERCONFIG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerConfig)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERCONFIG_H__14E4EBB4_C2CE_4447_8776_0181C1ED59F5__INCLUDED_)
