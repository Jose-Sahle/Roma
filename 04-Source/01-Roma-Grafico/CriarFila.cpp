// CriarFila.cpp : implementation file
//

#include "stdafx.h"
#include "srvroma.h"
#include "CriarFila.h"

#include "mq.h"

#include "Library.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCriarFila dialog


CCriarFila::CCriarFila(CWnd* pParent /*=NULL*/)
	: CDialog(CCriarFila::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCriarFila)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strNomeFila = _T("");
}


void CCriarFila::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCriarFila)
	DDX_Control(pDX, IDC_STATIC_FILA, m_ctrlStaticFila);
	DDX_Control(pDX, IDC_EDIT_FILA, m_ctrlEditFila);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCriarFila, CDialog)
	//{{AFX_MSG_MAP(CCriarFila)
	ON_EN_CHANGE(IDC_EDIT_FILA, OnChangeEditFila)
	ON_BN_CLICKED(IDC_BUTTON_CRIAR, OnButtonCriar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCriarFila message handlers

void CCriarFila::OnOK() 
{
	CDialog::OnOK();
}

void CCriarFila::OnCancel() 
{
	
	CDialog::OnCancel();
}

BOOL CCriarFila::CriarFila()
{

	CString strNomeFila;
	TCHAR	szNomeFila[256];
	WCHAR	wszPathName[256];
    WCHAR	wszOutFormatName[256];
    DWORD	dwOutFormatNameLength = sizeof(wszOutFormatName);
	const int NUMBEROFPROPERTIES = 2;

	m_ctrlEditFila.GetWindowText(strNomeFila);

	if (strNomeFila.IsEmpty())
	{
		AfxMessageBox(_T("Nome inválido de fila!"));
		return FALSE;
	}

	m_ctrlStaticFila.GetWindowText(strNomeFila);

#ifndef _UNICODE
	strcpy(szNomeFila, strNomeFila);

	MultiByteToWideChar(CP_ACP, 0, szNomeFila, -1, wszPathName, 256);
#else
	wcscpy(wszPathName, strNomeFila);
#endif


	// Define a queue property structure and the structures needed to initialize it.
	MQQUEUEPROPS   QueueProps;
	MQPROPVARIANT  aQueuePropVar[NUMBEROFPROPERTIES];
	QUEUEPROPID    aQueuePropId[NUMBEROFPROPERTIES];
	HRESULT        aQueueStatus[NUMBEROFPROPERTIES];
	HRESULT        hr = MQ_OK;
  
	hr = MQPathNameToFormatName((WCHAR*)strNomeFila.GetBuffer(),
							  wszOutFormatName,
							  &dwOutFormatNameLength);
  
	// Validate the input parameters.
	if (wszPathName == NULL || wszOutFormatName == NULL || dwOutFormatNameLength == NULL)
	{
		CString strMensagem;
		strMensagem.Format(_T("%s"), ErrorMessage(MQ_ERROR_INVALID_PARAMETER));
		return FALSE;
	}
    
	// Set queue properties.
	DWORD cPropId = 0;
	aQueuePropId[cPropId] = PROPID_Q_PATHNAME;
	aQueuePropVar[cPropId].vt = VT_LPWSTR;
	aQueuePropVar[cPropId].pwszVal = wszPathName;
	cPropId++;

	WCHAR wszLabel[MQ_MAX_Q_LABEL_LEN] = L"Roma";
	aQueuePropId[cPropId] = PROPID_Q_LABEL;
	aQueuePropVar[cPropId].vt = VT_LPWSTR;
	aQueuePropVar[cPropId].pwszVal = wszLabel;
	cPropId++;


	// Initialize the MQQUEUEPROPS structure.
	QueueProps.cProp = cPropId;               // Number of properties
	QueueProps.aPropID = aQueuePropId;        // IDs of the queue properties
	QueueProps.aPropVar = aQueuePropVar;      // Values of the queue properties
	QueueProps.aStatus = aQueueStatus;        // Pointer to the return status


	// Call MQCreateQueue to create the queue.
	WCHAR wszFormatNameBuffer[256];
	DWORD dwFormatNameBufferLength = sizeof(wszFormatNameBuffer)/sizeof(wszFormatNameBuffer[0]);
	hr = MQCreateQueue(NULL,                        // Default security descriptor
					 &QueueProps,					// Address of queue property structure
					 wszFormatNameBuffer,			// Pointer to format name buffer
					 &dwOutFormatNameLength);		// Pointer to receive the queue's format name length


	// Return the format name if the queue is created successfully.
	if (hr == MQ_OK || hr == MQ_INFORMATION_PROPERTY)
	{
		CString strMensagem;

		strMensagem.Format(_T("%s"), ErrorMessage(hr));

		AfxMessageBox(strMensagem);
		return TRUE;
	}
	else
	{
		CString strMensagem;
		strMensagem.Format(_T("%s"), ErrorMessage(hr));
		if (strMensagem.IsEmpty())
			strMensagem.Format(_T("%s"), ErrorMessage(GetLastError()));

		if (strMensagem.IsEmpty())
			strMensagem = _T("Erro ao tentar criar a fila!");

		AfxMessageBox(strMensagem);

		return FALSE;
	}

	return TRUE;
}

void CCriarFila::SetNomeFila(CString strNomeFila)
{
	strNomeFila.Replace(_T(".\\private$\\"), _T(""));
	m_strNomeFila = strNomeFila;
}

BOOL CCriarFila::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strNomeFila;
	
	m_ctrlEditFila.SetLimitText(255);

	strNomeFila = _T(".\\private$\\") + m_strNomeFila;

	m_ctrlStaticFila.SetWindowText(strNomeFila);

	m_ctrlEditFila.SetWindowText(m_strNomeFila);

	return TRUE;
}

void CCriarFila::OnChangeEditFila() 
{
	CString strNomeFila;

	m_ctrlEditFila.GetWindowText(strNomeFila);

	strNomeFila = _T(".\\private$\\") + strNomeFila;

	m_ctrlStaticFila.SetWindowText(strNomeFila);
}

void CCriarFila::OnButtonCriar() 
{
	if (!CriarFila())
		return;	
}
