// ListaDeUsuarios.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "ListaDeUsuarios.h"
#include "Library.h"
#include "MensagemDlg.h"

#include <Lm.h>
#include <mq.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListaDeUsuarios dialog


CListaDeUsuarios::CListaDeUsuarios(CWnd* pParent /*=NULL*/)
	: CDialog(CListaDeUsuarios::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListaDeUsuarios)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nTipo = 1;
}


void CListaDeUsuarios::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListaDeUsuarios)
	DDX_Control(pDX, IDC_LIST_USUARIOS_SELECIONADOS, m_ctrlUsuarioSelecionados);
	DDX_Control(pDX, IDC_LIST_LISTA_USUARIOS, m_ctrlListListaUsuario);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListaDeUsuarios, CDialog)
	//{{AFX_MSG_MAP(CListaDeUsuarios)
	ON_BN_CLICKED(ID_BUTTON_SELECIONAR, OnButtonSelecionar)
	ON_BN_CLICKED(ID_BUTTON_REMOVER, OnButtonRemover)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LISTA_USUARIOS, OnDblclkListListaUsuarios)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_LISTA_USUARIOS, OnColumnclickListListaUsuarios)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListaDeUsuarios message handlers

BOOL CListaDeUsuarios::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CMensagemDlg *dlgMensagem;
	
	BeginWaitCursor();
	dlgMensagem = new CMensagemDlg;
	dlgMensagem->Create(IDD_DIALOGBAR_MENSAGEM, NULL);

	SetWindowText(m_strTitulo);

	if (m_nTipo == 1)
	{
		dlgMensagem->SetMessage(_T("Aguarde... carregando lista de usuários."));
		dlgMensagem->ShowWindow(SW_SHOW);
		DoEvents();

		m_ctrlListListaUsuario.InsertColumn(0, _T("Nome"), LVCFMT_LEFT, 100);
		m_ctrlListListaUsuario.InsertColumn(1, _T("Nome completo"), LVCFMT_LEFT, 250);
		m_ctrlListListaUsuario.InsertColumn(2, _T("Comentários"), LVCFMT_LEFT, 300);

		PreencherListaUsuarios();
	}
	else
	{
		dlgMensagem->SetMessage(_T("Aguarde... carregando filas do MSMQ."));
		dlgMensagem->ShowWindow(SW_SHOW);
		DoEvents();

		m_ctrlListListaUsuario.InsertColumn(0, _T("Nome"), LVCFMT_LEFT, 630);
		m_ctrlListListaUsuario.InsertColumn(1, _T(""), LVCFMT_LEFT, 0);
		m_ctrlListListaUsuario.InsertColumn(2, _T(""), LVCFMT_LEFT, 0);

		PreencherListaFilas();
	}

	if (m_nTipo == 3)
		m_ctrlListListaUsuario.ModifyStyle(NULL, LVS_SINGLESEL);

	BOOL b = m_ctrlListListaUsuario.ModifyStyleEx(NULL, LVS_EX_FULLROWSELECT);

	dlgMensagem->DestroyWindow();

	EndWaitCursor();

	m_bOK = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CListaDeUsuarios::PreencherListaUsuarios()
{
	PNET_DISPLAY_USER pBuffer, pElement;
	DWORD res, dwRec, dwPonteiro = 0;
	WCHAR swzServer[255] = L""; 
	TCHAR  szServer[255];
	CString strServidor;
	int nTotal, nContador;

	m_strServidores.MakeUpper();

	nTotal = GetWordSeparatedByCount(m_strServidores, _T(";"));

	for (nContador = 1; nContador <= nTotal; nContador++)
	{
		strServidor = GetWordSeparatedBy(m_strServidores, nContador, _T(";"));

#ifndef _UNICODE
		strcpy(szServer, strServidor);

		MultiByteToWideChar(CP_ACP, 0, szServer, -1, swzServer, 255); 
#else
		wcscpy(swzServer, strServidor);
#endif

		do 
		{ 
			res = NetQueryDisplayInformation(swzServer, 1, dwPonteiro, 1000, MAX_PREFERRED_LENGTH, &dwRec, (VOID**) &pBuffer);

			if((res == ERROR_SUCCESS) || (res == ERROR_MORE_DATA))
			{
				pElement = pBuffer;
				for(; dwRec > 0 ; dwRec--)
				{
					TCHAR szName[256], szFullName[256], szComment[256];
					int nTamanho;

					int nTotalElementos = m_ctrlListListaUsuario.GetItemCount();

					szName[0]     = _T('\0');
					szFullName[0] = _T('\0');
					szComment[0]  = _T('\0');

					// Convertendo Nome
					_tcscpy(szName, pElement->usri1_name);

					// Convertendo Nome Completo
					_tcscpy(szFullName, pElement->usri1_full_name);

					// Convertendo Comentários
					_tcscpy(szComment, pElement->usri1_comment);

					if (!(szName[0] >= 48 && szName[0] <= 57))
					{
						CString strName;
						strName.Format(_T("%s\\%s"), szServer, szName);

						nTotalElementos = m_ctrlListListaUsuario.InsertItem(nTotalElementos, strName);
						m_ctrlListListaUsuario.SetItemText(nTotalElementos, 1, szFullName);
						m_ctrlListListaUsuario.SetItemText(nTotalElementos, 2, szComment);
					}

					dwPonteiro = pElement->usri1_next_index;
					pElement++;

					DoEvents();
				}

				NetApiBufferFree(pBuffer);
				DoEvents();
			}
			else
			{
				CString strPrompt;

				strPrompt.Format(_T("Erro:%u\n%s"), res, ErrorMessage(res));
				AfxMessageBox(strPrompt);
			}
			DoEvents();

		} while (res==ERROR_MORE_DATA); // end do
	}
	
}

void CListaDeUsuarios::PreencherListaFilas()
{
	DWORD dwTamanho = MAX_COMPUTERNAME_LENGTH + 1;

	TCHAR szComputerName[MAX_COMPUTERNAME_LENGTH + 1];

	WCHAR wszComputerName[MAX_COMPUTERNAME_LENGTH + 1];

	ZeroMemory(szComputerName, MAX_COMPUTERNAME_LENGTH + 1);

#ifndef _UNICODE
	GetComputerName(szComputerName, &dwTamanho);

	MultiByteToWideChar(CP_ACP, 0, szComputerName, -1, wszComputerName, MAX_COMPUTERNAME_LENGTH + 1);
#else
	GetComputerName(wszComputerName, &dwTamanho);
#endif

	// Define the required constants and variables.
	const int NUMBEROFPROPERTIES = 1;                  // Number of properties
	DWORD cPropId = 0;                                 // Property counter
	DWORD cQ = 0;                                      // Queue counter
	HRESULT hr = MQ_OK;                                // Return code

	// Define an MQMGMTROPS structure.
	MQMGMTPROPS mgmtprops;
	MGMTPROPID aMgmtPropId[NUMBEROFPROPERTIES];
	MQPROPVARIANT aMgmtPropVar[NUMBEROFPROPERTIES];


	// Specify PROPID_MGMT_MSMQ_PRIVATEQ as a property to be retrieved.
	aMgmtPropId[cPropId] = PROPID_MGMT_MSMQ_PRIVATEQ;  // Property ID
	aMgmtPropVar[cPropId].vt = VT_NULL;                // Type indicator
	cPropId++;

	// Initialize the MQMGMTPROPS structure.
	mgmtprops.cProp = cPropId;                         // Number of management properties
	mgmtprops.aPropID = aMgmtPropId;                   // IDs of the management properties
	mgmtprops.aPropVar = aMgmtPropVar;                 // Values of management properties
	mgmtprops.aStatus  = NULL;                         // No storage for error codes


	// Call MQMgmtGetInfo to retrieve the information.
	hr = MQMgmtGetInfo(wszComputerName,               // Name of the computer
					 MO_MACHINE_TOKEN,              // Object name
					 &mgmtprops);                   // Management properties structure

	if (FAILED(hr))
	{
		return;
	}

	// Display the path names of the private queues found.
	if(aMgmtPropVar[0].calpwstr.cElems > 0)
	{
		for (cQ = 0; cQ < aMgmtPropVar[0].calpwstr.cElems; cQ++)
		{
			int nValor;
			TCHAR szFila[255];
			CString strFila;
			CString strComputer;

			memset(szFila, NULL, 255 * sizeof(TCHAR));

			#ifndef _UNICODE
				nValor = wcslen(aMgmtPropVar[0].calpwstr.pElems[cQ]) + 1;
				WideCharToMultiByte(CP_ACP, 0, aMgmtPropVar[0].calpwstr.pElems[cQ], -1, szFila, nValor, NULL, NULL);
			#else
				wcscpy(szFila, aMgmtPropVar[0].calpwstr.pElems[cQ]);
			#endif

			nValor = m_ctrlListListaUsuario.GetItemCount();

			strFila = CString(szFila);

			strComputer = CString(szComputerName);

			strFila.Replace(strComputer, _T("."));

			strComputer.MakeLower();
			strFila.Replace(strComputer, _T("."));

			strComputer.MakeUpper();
			strFila.Replace(strComputer, _T("."));

			m_ctrlListListaUsuario.InsertItem(nValor, strFila);
		}
	}

	// Free the memory allocated to store the path names.
	for (cQ = 0; cQ < aMgmtPropVar[0].calpwstr.cElems; cQ++)
	{
		MQFreeMemory(aMgmtPropVar[0].calpwstr.pElems[cQ]);
	}

	MQFreeMemory(aMgmtPropVar[0].calpwstr.pElems);
}

void CListaDeUsuarios::SetServidores(CString strServidores)
{
	m_strServidores = strServidores;
}

int CListaDeUsuarios::GetTotalSelecionados()
{
	return m_nTotalSelecionados;
}

void CListaDeUsuarios::GetSelecionador(int nIndex, CString &strName, CString &strFullName, CString &strComment)
{
	if (m_nTotalSelecionados == 0)
		return;

	strName     = arrayName.GetAt(nIndex);
	strFullName = arrayFullName.GetAt(nIndex);
	strComment  = arrayComment.GetAt(nIndex);
}


void CListaDeUsuarios::OnCancel() 
{
	m_bOK = FALSE;

	m_nTotalSelecionados = 0;

	CDialog::OnCancel();
}

void CListaDeUsuarios::OnButtonSelecionar() 
{
	CString strUsuario;
	int nIndex, nIndexUsuario;

	POSITION pos = m_ctrlListListaUsuario.GetFirstSelectedItemPosition();

	while (pos)
	{
		nIndex = m_ctrlListListaUsuario.GetNextSelectedItem(pos);

		strUsuario = m_ctrlListListaUsuario.GetItemText(nIndex, 0);

		if (m_ctrlUsuarioSelecionados.FindStringExact(0, strUsuario) == -1)
		{
			nIndexUsuario = m_ctrlUsuarioSelecionados.AddString(strUsuario);
			m_ctrlUsuarioSelecionados.SetItemData(nIndexUsuario, nIndex);
		}
	}
}

void CListaDeUsuarios::OnButtonRemover() 
{
	if (m_ctrlUsuarioSelecionados.GetCurSel() != - 1)
		m_ctrlUsuarioSelecionados.DeleteString(m_ctrlUsuarioSelecionados.GetCurSel());
}

void CListaDeUsuarios::OnDblclkListListaUsuarios(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString strUsuario;
	int nIndex, nIndexUsuario;

	POSITION pos = m_ctrlListListaUsuario.GetFirstSelectedItemPosition();

	nIndex = m_ctrlListListaUsuario.GetNextSelectedItem(pos);

	if (nIndex == -1)
		return;

	strUsuario = m_ctrlListListaUsuario.GetItemText(nIndex, 0);

	if (m_nTipo == 3)
	{
		m_ctrlUsuarioSelecionados.DeleteString(0);
		nIndexUsuario = m_ctrlUsuarioSelecionados.AddString(strUsuario);
		m_ctrlUsuarioSelecionados.SetItemData(nIndexUsuario, nIndex);
	}
	else
	{
		if (m_ctrlUsuarioSelecionados.FindStringExact(0, strUsuario) == -1)
		{
			nIndexUsuario = m_ctrlUsuarioSelecionados.AddString(strUsuario);
			m_ctrlUsuarioSelecionados.SetItemData(nIndexUsuario, nIndex);
		}
	}

	*pResult = 0;
}

BOOL CListaDeUsuarios::GetOK()
{
	return m_bOK;
}

void CListaDeUsuarios::OnOK() 
{
	int nContador;
	DWORD nIndex;

	m_bOK = TRUE;

	m_nTotalSelecionados = m_ctrlUsuarioSelecionados.GetCount();

	for (nContador = 0; nContador < m_nTotalSelecionados; nContador++)
	{
		CString strName, strFullName, strComment;

		nIndex = m_ctrlUsuarioSelecionados.GetItemData(nContador);

		strName     = m_ctrlListListaUsuario.GetItemText(nIndex, 0);
		strFullName = m_ctrlListListaUsuario.GetItemText(nIndex, 1);
		strComment  = m_ctrlListListaUsuario.GetItemText(nIndex, 2);
		
		arrayName.Add(strName);
		arrayFullName.Add(strFullName);
		arrayComment.Add(strComment);
	}

	CDialog::OnOK();
}

void CListaDeUsuarios::OnColumnclickListListaUsuarios(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	SortListInfo si;

	si.nColumn = pNMListView->iSubItem;
	si.nColumnDataType = DT_STRING;
	si.pctrlList = &m_ctrlListListaUsuario;
	si.nSortType = ST_ASC;

	m_ctrlListListaUsuario.SortItems(CompareFunctionList, (LPARAM) &si);
	m_ctrlListListaUsuario.Arrange(LVA_DEFAULT);

	*pResult = 0;
}

void CListaDeUsuarios::SetTipo(int nTipo, CString strTitulo)
{
	m_nTipo = nTipo;

	m_strTitulo = strTitulo;
}
