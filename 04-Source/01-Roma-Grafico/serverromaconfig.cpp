// serverromaconfig.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Roma.h"

#include "ServerRomaConfig.h"

#include "ListaDeUsuarios.h"

#include "Library.h"

#include "Estruturas.h"

#include "CriarFila.h"

#include "mq.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern STRU_VARIAVEIS_GLOBAIS g_struVariaveisGlobais;
/////////////////////////////////////////////////////////////////////////////
// CServerRomaConfig dialog


CServerRomaConfig::CServerRomaConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CServerRomaConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerRomaConfig)
	//}}AFX_DATA_INIT
	m_uIDSelecionado = 0;
}


void CServerRomaConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerRomaConfig)
	DDX_Control(pDX, IDC_EDIT_SVR_TIMEOUT, m_ctrlEditSrvTimeOut);
	DDX_Control(pDX, IDC_EDIT_MSMQ_ERROR, m_ctrlEditFilaErro);
	DDX_Control(pDX, IDC_BUTTON_USUARIO_REMOVER, m_btnRemoverUsuarios);
	DDX_Control(pDX, IDC_BUTTON_BUSCAR_USUARIOS, m_btnBuscarUsuarios);
	DDX_Control(pDX, IDC_CHECK_INICIAR_SERVICO, m_ctrlChkIniciarServico);
	DDX_Control(pDX, IDC_LIST_FILAS, m_ctrlListFilas);
	DDX_Control(pDX, IDC_LIST_USUARIOS, m_ctrlListUsuarios);
	DDX_Control(pDX, IDC_EDIT_SOCKET_DOMINIOS, m_ctrlServidores);
	DDX_Control(pDX, IDC_EDIT_SOCKET_TIMEOUT, m_ctrlEditSocketTimeOut);
	DDX_Control(pDX, IDC_EDIT_SOCKET_MAX_CONNECTIONS, m_ctrlEditSocketMaxConnections);
	DDX_Control(pDX, IDC_EDIT_SOCKET_PORT, m_ctrlEditSocketPort);
	DDX_Control(pDX, IDC_EDIT_MSMQ_TIMEOUT, m_ctrlEditMSMQTimeOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerRomaConfig, CDialog)
	//{{AFX_MSG_MAP(CServerRomaConfig)
	ON_BN_CLICKED(IDC_BUTTON_FILA_ADICIONAR, OnButtonFilaAdicionar)
	ON_BN_CLICKED(IDC_BUTTON_FILA_CRIAR, OnButtonFilaCriar)
	ON_BN_CLICKED(IDC_BUTTON_USUARIO_REMOVER, OnButtonUsuarioRemover)
	ON_BN_CLICKED(IDC_BUTTON_FILA_REMOVER, OnButtonFilaRemover)
	ON_BN_CLICKED(IDC_BUTTON_REGISTRAR, OnButtonRegistrar)
	ON_BN_CLICKED(IDC_BUTTON_DESREGISTRAR, OnButtonDesregistrar)
	ON_BN_CLICKED(IDC_BUTTON_BUSCAR_USUARIOS, OnButtonBuscarUsuarios)
	ON_EN_KILLFOCUS(IDC_EDIT_SOCKET_MAX_CONNECTIONS, OnKillfocusEditSocketMaxConnections)
	ON_EN_KILLFOCUS(IDC_EDIT_SOCKET_PORT, OnKillfocusEditSocketPort)
	ON_EN_KILLFOCUS(IDC_EDIT_SOCKET_TIMEOUT, OnKillfocusEditSocketTimeout)
	ON_BN_CLICKED(IDC_CHECK_INICIAR_SERVICO, OnCheckIniciarServico)
	ON_BN_CLICKED(IDC_BUTTON_FILA_EXCLUIR_MSMQ, OnButtonFilaExcluirMsmq)
	ON_BN_CLICKED(IDC_BUTTON_BUSCA_FILA_ERRO, OnButtonBuscaFilaErro)
	ON_EN_SETFOCUS(IDC_EDIT_MSMQ_ERROR, OnSetfocusEditMsmqError)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_FILAS, OnSetfocusListFilas)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerRomaConfig message handlers
BOOL CServerRomaConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ctrlListUsuarios.InsertColumn(0, _T("Nome"), LVCFMT_LEFT, 100);
	m_ctrlListUsuarios.InsertColumn(1, _T("Nome completo"), LVCFMT_LEFT, 150);
	m_ctrlListUsuarios.InsertColumn(2, _T("Comentários"), LVCFMT_LEFT, 200);

	m_ctrlListFilas.InsertColumn(0, _T("Nome"), LVCFMT_LEFT, 190);
	m_ctrlListFilas.InsertColumn(1, _T("Estado"), LVCFMT_LEFT, 93);

	m_ctrlEditMSMQTimeOut.SetLimitText(5);
	m_ctrlEditFilaErro.SetLimitText(255);
	m_ctrlEditSrvTimeOut.SetLimitText(2);

	PreencherDados();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CServerRomaConfig::OnButtonFilaAdicionar() 
{
	int nTotalSelecionados, nContador;

	CString strServidores;
	CListaDeUsuarios dlgListaUsuarios;

	dlgListaUsuarios.SetTipo(2, _T("MSMQ"));

	dlgListaUsuarios.DoModal();

	if (dlgListaUsuarios.GetOK() )
	{
		nTotalSelecionados = dlgListaUsuarios.GetTotalSelecionados();

		for (nContador = 0; nContador < nTotalSelecionados; nContador++)
		{
			CString strName, strFullName, strComment;
			int nTotalElementos;

			LVFINDINFO info;

			dlgListaUsuarios.GetSelecionador(nContador, strName, strFullName, strComment);

			info.flags = LVFI_STRING;
			info.psz = strName;

			if (m_ctrlListFilas.FindItem(&info) == -1)
			{
				nTotalElementos = m_ctrlListFilas.GetItemCount();

				m_ctrlListFilas.InsertItem(nTotalElementos, strName);
				m_ctrlListFilas.SetItemText(nTotalElementos, 1, _T("OK"));
			}
		}
	}	
}

void CServerRomaConfig::OnButtonFilaCriar() 
{
	CCriarFila dlgFila;
	CString strNomeFila;

	POSITION pos;
	int nIndex;

	switch(m_uIDSelecionado)
	{
		case IDC_LIST_FILAS:
			pos = m_ctrlListFilas.GetFirstSelectedItemPosition();
			nIndex = m_ctrlListFilas.GetNextSelectedItem(pos);

			if (nIndex != -1)
				strNomeFila = m_ctrlListFilas.GetItemText(nIndex, 0);
			break;

		case IDC_EDIT_MSMQ_ERROR:
			m_ctrlEditFilaErro.GetWindowText(strNomeFila);
			break;
	}

	dlgFila.SetNomeFila(strNomeFila);

	dlgFila.DoModal();

	VerificarFilas();
}

void CServerRomaConfig::OnButtonUsuarioRemover() 
{
	CStringArray arrayName;

	CString strUsuario;
	int nContador, nIndex;

	POSITION pos = m_ctrlListUsuarios.GetFirstSelectedItemPosition();

	while (pos)
	{
		nIndex = m_ctrlListUsuarios.GetNextSelectedItem(pos);

		strUsuario = m_ctrlListUsuarios.GetItemText(nIndex, 0);

		arrayName.Add(strUsuario);
	}

	for (nContador = 0; nContador < arrayName.GetSize(); nContador++)
	{
		LVFINDINFO info;

		info.flags = LVFI_STRING;
		info.psz = arrayName.GetAt(nContador);

		if ( (nIndex = m_ctrlListUsuarios.FindItem(&info)) > -1)
		{
			m_ctrlListUsuarios.DeleteItem(nIndex);
		}
	}
}

void CServerRomaConfig::OnButtonFilaRemover() 
{
	CStringArray arrayName;

	CString strFila;
	int nIndex;

	POSITION pos = m_ctrlListFilas.GetFirstSelectedItemPosition();

	while (pos)
	{
		nIndex = m_ctrlListFilas.GetNextSelectedItem(pos);

		strFila = m_ctrlListFilas.GetItemText(nIndex, 0);

		arrayName.Add(strFila);
	}

	RemoverFila(arrayName);
}

void CServerRomaConfig::OnCancel() 
{	
	CDialog::OnCancel();
}

void CServerRomaConfig::OnButtonRegistrar() 
{
	if (_Module->RegisterServer(TRUE, TRUE) == 0 )
		AfxMessageBox(_T("Roma foi registrado com sucesso!"));
	else
		AfxMessageBox(_T("Roma falhou ao tentar se registrar!"));
}

void CServerRomaConfig::OnButtonDesregistrar() 
{
	if (_Module->UnregisterServer() == 0)
	{
		CRegKey keyAppID;
		LONG lRes = keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_READ | KEY_WRITE);
		if (lRes == ERROR_SUCCESS)
		{
			keyAppID.DeleteSubKey(_T("{9049DC8A-D32B-426F-B4D1-A152DED669A4}"));
		}

		AfxMessageBox(_T("Roma foi desregistrado com sucesso!"));
	}
	else
		AfxMessageBox(_T("Roma falhou ao tentar se desregistrar!"));
}

void CServerRomaConfig::OnButtonBuscarUsuarios() 
{
	int nTotalSelecionados, nContador;

	CString strServidores;
	CListaDeUsuarios dlgListaUsuarios;

	m_ctrlServidores.GetWindowText(strServidores);

	if (strServidores.GetLength() == 0)
	{
		AfxMessageBox(_T("Nenhum servidor foi informado!"));
		m_ctrlServidores.SetFocus();
		return;
	}

	dlgListaUsuarios.SetServidores(strServidores);

	dlgListaUsuarios.DoModal();

	if (dlgListaUsuarios.GetOK() )
	{
		nTotalSelecionados = dlgListaUsuarios.GetTotalSelecionados();

		for (nContador = 0; nContador < nTotalSelecionados; nContador++)
		{
			CString strName, strFullName, strComment;
			int nTotalElementos;

			LVFINDINFO info;

			dlgListaUsuarios.GetSelecionador(nContador, strName, strFullName, strComment);

			info.flags = LVFI_STRING;
			info.psz = strName;

			if (m_ctrlListUsuarios.FindItem(&info) == -1)
			{
				nTotalElementos = m_ctrlListUsuarios.GetItemCount();

				m_ctrlListUsuarios.InsertItem(nTotalElementos, strName);
				m_ctrlListUsuarios.SetItemText(nTotalElementos, 1, strFullName);
				m_ctrlListUsuarios.SetItemText(nTotalElementos, 2, strComment);
			}
		}
	}
}

void CServerRomaConfig::OnOK() 
{
	if (VerificarRegistro())
	{
		if (!SalvarRegistro())
			return;
	}
	else
		return;
	
	CDialog::OnOK();
}


void CServerRomaConfig::OnKillfocusEditSocketMaxConnections() 
{
	CString strMaxConnections;
	int nMaxConnections;

	m_ctrlEditSocketMaxConnections.GetWindowText(strMaxConnections);

	Val(strMaxConnections, nMaxConnections);

	if (nMaxConnections > 1000)
	{
		AfxMessageBox(_T("Número máximo de conexão simultâneas não pode ultrapassar 1000."));
		m_ctrlEditSocketMaxConnections.SetFocus();
		return;
	}
}

void CServerRomaConfig::OnKillfocusEditSocketPort() 
{
	CString strPort;
	int nPort;

	m_ctrlEditSocketPort.GetWindowText(strPort);

	Val(strPort, nPort);

	if (nPort > 65535 || nPort < 1)
	{
		AfxMessageBox(_T("Número da porta deve estar entre 1 e 65535."));
		m_ctrlEditSocketPort.SetFocus();
		return;
	}
}

void CServerRomaConfig::OnKillfocusEditSocketTimeout() 
{
	CString strTimeOut;
	int nTimeOut;

	m_ctrlEditSocketTimeOut.GetWindowText(strTimeOut);

	Val(strTimeOut, nTimeOut);

	if (nTimeOut < 1)
	{
		AfxMessageBox(_T("Tempo máximo sem resposta deve ser maior que 1 milisegundo"));
		m_ctrlEditSocketTimeOut.SetFocus();
		return;
	}
}

void CServerRomaConfig::OnCheckIniciarServico() 
{
	int nChecked = m_ctrlChkIniciarServico.GetCheck();	

	if (nChecked == 0)
		AtivaDesativaControlesRemoto(FALSE);
	else
		AtivaDesativaControlesRemoto(TRUE);
}

void CServerRomaConfig::OnButtonFilaExcluirMsmq() 
{
	CStringArray arrayName;

	CString strMensagem;
	CString strFila;
	int nIndex, nContador;
	POSITION pos;

	switch(m_uIDSelecionado)
	{
		case IDC_LIST_FILAS:
			pos = m_ctrlListFilas.GetFirstSelectedItemPosition();

			while (pos)
			{
				nIndex = m_ctrlListFilas.GetNextSelectedItem(pos);

				strFila = m_ctrlListFilas.GetItemText(nIndex, 0);

				arrayName.Add(strFila);
			}

			break;

		case IDC_EDIT_MSMQ_ERROR:
			m_ctrlEditFilaErro.GetWindowText(strFila);
			arrayName.Add(strFila);

			break;
	}

	if (arrayName.GetSize() == 0)
		return;

	if (arrayName.GetSize() > 1)
		strMensagem = _T("**** ATENÇÃO ****\nAs filas abaixo relacionadas serão excluídas do ROMA e do MSMQ!\n\n");
	else
		strMensagem = _T("**** ATENÇÃO ****\nA fila abaixo relacionada será excluída do ROMA e do MSMQ!\n\n");

	for (nContador = 0; nContador < arrayName.GetSize(); nContador++)
	{
		strMensagem += arrayName.GetAt(nContador);
		strMensagem += _T("\n");
	}
	strMensagem += _T("\nDeseja continuar ?");
	
	if (AfxMessageBox(strMensagem, MB_YESNO | MB_DEFBUTTON2) == IDNO)
		return;

	RemoverFila(arrayName);

	RemoverFilaMSMQ(arrayName);
}

BOOL CServerRomaConfig::VerificarRegistro()
{	
	CString strValor;
	int nValor;

	m_ctrlEditMSMQTimeOut.GetWindowText(strValor);

	if (strValor.IsEmpty())
		nValor = 0;
	else
		Val(strValor, nValor);

	if (nValor > 999999)
	{
		AfxMessageBox(_T("Valor inválido para \'Tempo máximo sem resposta\'!"));

		return FALSE;
	}

	if (!VerificarFilas())
	{
		AfxMessageBox(_T("Filas inconsistentes, por favor verifique!"));

		return FALSE;
	}

	return TRUE;
}

BOOL CServerRomaConfig::SalvarRegistro()
{
	BOOL bRetorno = TRUE;
	CString strValor;

	DWORD dwError;
	TCHAR *lpBackSlash;
	TCHAR szConfigFile[_MAX_PATH];

	GetModuleFileName(NULL, szConfigFile, _MAX_PATH);

	lpBackSlash = _tcsrchr(szConfigFile, _T('\\'));
	*lpBackSlash= _T('\0');

	_tcscat( szConfigFile, _T("\\ROMA.cfg"));

	CString strConfigFile = CString(szConfigFile);

	// Se o arquivo de configuração existe, apagá-o primeiro
	if (ArquivoExiste(strConfigFile))
	{
		if (DeleteFile(strConfigFile) != 0)
		{
			CString strMensagem;
			strMensagem.Format(_T("Não foi possível excluir o arquivo \"%s\"!\n"), strConfigFile);
			
			AfxMessageBox(strMensagem);

			return FALSE;
		}
	}

	/////////////////////////////////////////////////////
	///
	/// INICIO GRAVAÇÃO DO SERVIÇO
	///
	/////////////////////////////////////////////////////	
	// TimeOut
	m_ctrlEditSrvTimeOut.GetWindowText(strValor);
	if (!WritePrivateProfileString(_T("SERVICO"), _T("timeout"), strValor, strConfigFile))
	{
		CString strMensagem;

		dwError = GetLastError();

		strMensagem.Format(_T("Erro ao tentar gravar no arquivo \"%s\"!\n%s"), strConfigFile, ErrorMessage(dwError));
		AfxMessageBox(strMensagem);

		return FALSE;
	}

	/////////////////////////////////////////////////////
	///
	/// FINAL GRAVAÇÃO DO SERVIÇO
	///
	/////////////////////////////////////////////////////	

	/////////////////////////////////////////////////////
	///
	/// INICIO CONFIGURAÇÕES PARA ACESSO REMOTO
	///
	/////////////////////////////////////////////////////
	// Inicia Servico
	BOOL bInicia = m_ctrlChkIniciarServico.GetCheck();
	if (bInicia)
	{
		if (!WritePrivateProfileString(_T("REMOTO"), _T("IniciarServico"), _T("TRUE"), strConfigFile))
		{
			CString strMensagem;

			dwError = GetLastError();

			strMensagem.Format(_T("Erro ao tentar gravar no arquivo \"%s\"!\n%s"), strConfigFile, ErrorMessage(dwError));
			AfxMessageBox(strMensagem);

			return FALSE;
		}
	}
	else
	{
		if (!WritePrivateProfileString(_T("REMOTO"), _T("IniciarServico"), _T("FALSE"), strConfigFile))
		{
			CString strMensagem;

			dwError = GetLastError();

			strMensagem.Format(_T("Erro ao tentar gravar no arquivo \"%s\"!\n%s"), strConfigFile, ErrorMessage(dwError));
			AfxMessageBox(strMensagem);

			return FALSE;
		}
	}


	// Número da Porta
	m_ctrlEditSocketPort.GetWindowText(strValor);
	if (!WritePrivateProfileString(_T("REMOTO"), _T("Porta"), strValor, strConfigFile))
	{
		CString strMensagem;

		dwError = GetLastError();

		strMensagem.Format(_T("Erro ao tentar gravar no arquivo \"%s\"!\n%s"), strConfigFile, ErrorMessage(dwError));
		AfxMessageBox(strMensagem);

		return FALSE;
	}

	// Conexões simultaneas
	m_ctrlEditSocketMaxConnections.GetWindowText(strValor);
	if (!WritePrivateProfileString(_T("REMOTO"), _T("Simultaneas"), strValor, strConfigFile))
	{
		CString strMensagem;

		dwError = GetLastError();

		strMensagem.Format(_T("Erro ao tentar gravar no arquivo \"%s\"!\n%s"), strConfigFile, ErrorMessage(dwError));
		AfxMessageBox(strMensagem);

		return FALSE;
	}

	// Tempo sem resposta
	m_ctrlEditSocketTimeOut.GetWindowText(strValor);
	if (!WritePrivateProfileString(_T("REMOTO"), _T("TimeOut"), strValor, strConfigFile))
	{
		CString strMensagem;

		dwError = GetLastError();

		strMensagem.Format(_T("Erro ao tentar gravar no arquivo \"%s\"!\n%s"), strConfigFile, ErrorMessage(dwError));
		AfxMessageBox(strMensagem);

		return FALSE;
	}

	/////////////////////////////////////////////////////
	///
	/// FINAL CONFIGURAÇÕES PARA ACESSO REMOTO
	///
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	///
	/// INICIO GRAVAÇÃO DAS FILAS
	///
	/////////////////////////////////////////////////////	
	strValor.Format(_T("%d"), m_ctrlListFilas.GetItemCount());

	// Grava a quantidade de filas
	if (!WritePrivateProfileString(_T("FILAS"), _T("queuecount"), strValor, strConfigFile))
	{
		CString strMensagem;

		dwError = GetLastError();

		strMensagem.Format(_T("Erro ao tentar gravar no arquivo \"%s\"!\n%s"), strConfigFile, ErrorMessage(dwError));
		AfxMessageBox(strMensagem);

		return FALSE;
	}

	// Grava o nome das filas
	for (int nContador = 0; nContador < m_ctrlListFilas.GetItemCount(); nContador++)
	{
		CString strFila;

		strFila.Format(_T("queue%d"), nContador+1);

		strValor = m_ctrlListFilas.GetItemText(nContador, 0);

		if (!WritePrivateProfileString(_T("FILAS"), strFila, strValor, strConfigFile))
		{
			CString strMensagem;

			dwError = GetLastError();

			strMensagem.Format(_T("Erro ao tentar gravar no arquivo \"%s\"!\n%s"), strConfigFile, ErrorMessage(dwError));
			AfxMessageBox(strMensagem);

			return FALSE;
		}

	}
	
	// Grava o nome da Fila de Erro
	m_ctrlEditFilaErro.GetWindowText(strValor);
	if (!WritePrivateProfileString(_T("FILAS"), _T("queueerror"), strValor, strConfigFile))
	{
		CString strMensagem;

		dwError = GetLastError();

		strMensagem.Format(_T("Erro ao tentar gravar no arquivo \"%s\"!\n%s"), strConfigFile, ErrorMessage(dwError));
		AfxMessageBox(strMensagem);

		return FALSE;
	}

	// Salva o tempo de TimeOut das filas
	m_ctrlEditMSMQTimeOut.GetWindowText(strValor);
	if (!WritePrivateProfileString(_T("FILAS"), _T("timeoutmsmq"), strValor, strConfigFile))
	{
		CString strMensagem;

		dwError = GetLastError();

		strMensagem.Format(_T("Erro ao tentar gravar no arquivo \"%s\"!\n%s"), strConfigFile, ErrorMessage(dwError));
		AfxMessageBox(strMensagem);

		return FALSE;
	}

	/////////////////////////////////////////////////////
	///
	/// FINAL GRAVAÇÃO DAS FILAS
	///
	/////////////////////////////////////////////////////

	return TRUE;
}

void CServerRomaConfig::PreencherDados()
{
	TCHAR szFila[255];
	int nTamanho;

	// Campos do acesso remoto
	m_ctrlChkIniciarServico.SetCheck(g_struVariaveisGlobais.bIniciarServico);
	
	m_ctrlEditSocketMaxConnections.SetWindowText(Str(g_struVariaveisGlobais.dwQuantidadeConexoesSimultaneas));
	m_ctrlEditSocketPort.SetWindowText(Str(g_struVariaveisGlobais.dwPortaDeComunicacao));
	m_ctrlEditSocketTimeOut.SetWindowText(Str(g_struVariaveisGlobais.dwTempoSemResposta));
	
	m_ctrlServidores.SetWindowText(_T(""));
	m_ctrlListUsuarios.DeleteAllItems();

	AtivaDesativaControlesRemoto(g_struVariaveisGlobais.bIniciarServico);

	// Configura as filas
	//m_ctrlEditMSMQTimeOut.SetWindowText(g_struVariaveisGlobais.);
	m_ctrlListFilas.DeleteAllItems();
	for (int nContador = 0; nContador <  (int)g_struVariaveisGlobais.dwTotalFilas; nContador++)
	{
		if (g_struVariaveisGlobais.wsaQueueNames[nContador] != NULL)
		{
			nTamanho = wcslen(g_struVariaveisGlobais.wsaQueueNames[nContador]) + 1;

			_tcscpy(szFila, g_struVariaveisGlobais.wsaQueueNames[nContador]);
		
			m_ctrlListFilas.InsertItem(nContador, szFila);
		}
	}

	// Fila de Erro
	if (g_struVariaveisGlobais.wsaQueueNames != NULL)
	{
		nTamanho = wcslen(g_struVariaveisGlobais.wsErrorQueueName) + 1;

		_tcscpy(szFila, g_struVariaveisGlobais.wsErrorQueueName);

		m_ctrlEditFilaErro.SetWindowText(CString(szFila));
	}

	//TimeOut para finalizar o serviço
	m_ctrlEditSrvTimeOut.SetWindowText(Str(g_struVariaveisGlobais.dwServicoTimeOut));

	//TimeOut das Filas
	m_ctrlEditMSMQTimeOut.SetWindowText(Str(g_struVariaveisGlobais.dwFilaTimeOut));

	// Verifica se existe as filas no MSMQ
	VerificarFilas();	
}

void CServerRomaConfig::AtivaDesativaControlesRemoto(BOOL bValor)
{
	m_ctrlEditSocketMaxConnections.EnableWindow(bValor);
	m_ctrlEditSocketPort.EnableWindow(bValor);
	m_ctrlEditSocketTimeOut.EnableWindow(bValor);
	m_ctrlServidores.EnableWindow(bValor);
	m_ctrlListUsuarios.EnableWindow(bValor);
	m_btnBuscarUsuarios.EnableWindow(bValor);
	m_btnRemoverUsuarios.EnableWindow(bValor);
}

void CServerRomaConfig::OnButtonBuscaFilaErro() 
{
	CString strServidores;
	CListaDeUsuarios dlgListaUsuarios;

	dlgListaUsuarios.SetTipo(3, _T("MSMQ"));

	dlgListaUsuarios.DoModal();

	if (dlgListaUsuarios.GetOK() && dlgListaUsuarios.GetTotalSelecionados() > 0)
	{
		CString strName, strFullName, strComment;

		dlgListaUsuarios.GetSelecionador(0, strName, strFullName, strComment);

		m_ctrlEditFilaErro.SetWindowText(strName);
	}		
}

BOOL CServerRomaConfig::VerificarFilas()
{
	BOOL bRetorno = TRUE;

	QUEUEHANDLE hQueue = 0;
	WCHAR MQPathName[256];
	WCHAR szwFormatNameBuffer[256];
	DWORD dwDestFormatNameBufferLength = sizeof(szwFormatNameBuffer);

	CStringArray arrayFilas;

	DWORD dwTamanho = MAX_COMPUTERNAME_LENGTH + 1;

	TCHAR szComputerName[MAX_COMPUTERNAME_LENGTH + 1];

	WCHAR wszComputerName[MAX_COMPUTERNAME_LENGTH + 1];

	ZeroMemory(szComputerName, MAX_COMPUTERNAME_LENGTH + 1);

	GetComputerName(wszComputerName, &dwTamanho);

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
		CString strMensagem;

		strMensagem.Format(_T("%s"), ErrorMessage(hr));
		if (strMensagem.IsEmpty())
			strMensagem.Format(_T("%s"), ErrorMessage(GetLastError()));

		if (strMensagem.IsEmpty())
			strMensagem = _T("Erro ao tentar ao executar MQMgmtGetInfo");

		AfxMessageBox(strMensagem);

		return FALSE;
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

			nValor = wcslen(aMgmtPropVar[0].calpwstr.pElems[cQ]) + 1;

			_tcscpy(szFila, aMgmtPropVar[0].calpwstr.pElems[cQ]);

			strFila = CString(szFila);

			strComputer = CString(szComputerName);

			strFila.Replace(strComputer, _T("."));

			strComputer.MakeLower();
			strFila.Replace(strComputer, _T("."));

			strComputer.MakeUpper();
			strFila.Replace(strComputer, _T("."));

			arrayFilas.Add(strFila);
		}
	}

	// Free the memory allocated to store the path names.
	for (cQ = 0; cQ < aMgmtPropVar[0].calpwstr.cElems; cQ++)
	{
		MQFreeMemory(aMgmtPropVar[0].calpwstr.pElems[cQ]);
	}

	MQFreeMemory(aMgmtPropVar[0].calpwstr.pElems);

	// Verifica se as filas relacionada existem no MSMQ
	for (INT nContador = 0; nContador < m_ctrlListFilas.GetItemCount(); nContador++)
	{
		BOOL bEncontrou = FALSE;
		CString strItem = m_ctrlListFilas.GetItemText(nContador, 0);

		for (int nContFilas = 0; nContFilas < arrayFilas.GetSize(); nContFilas++)
		{
			CString strFila = arrayFilas.GetAt(nContFilas);

			if (strFila == strItem)
			{
				bEncontrou = TRUE;
				break;
			}
		}

		if (bEncontrou)
			m_ctrlListFilas.SetItemText(nContador, 1, _T("OK"));
		else
		{
			bRetorno = FALSE;
			m_ctrlListFilas.SetItemText(nContador, 1, _T("Inexistente"));
		}

	}

	// Verifica se o acesso às filas estão OK
	for (INT nContador = 0; nContador < m_ctrlListFilas.GetItemCount(); nContador++)
	{
		CString strItem = m_ctrlListFilas.GetItemText(nContador, 1);

		if (strItem == _T("OK"))
		{	
			strItem = m_ctrlListFilas.GetItemText(nContador, 0);

			_tcscpy(MQPathName, strItem);

			hr = MQPathNameToFormatName((WCHAR*)MQPathName,
									  szwFormatNameBuffer,
									  &dwDestFormatNameBufferLength);

			hr = MQOpenQueue(
				 szwFormatNameBuffer,						// Format Name of the queue to be opened
				 MQ_RECEIVE_ACCESS  | MQ_ADMIN_ACCESS,		// Access rights to the Queue
				 0,											// No receive Exclusive
				 &hQueue);									// OUT: handle to the opened Queue

			if (FAILED(hr))
				m_ctrlListFilas.SetItemText(nContador, 1, ErrorMessage(hr));
			else
				hr = MQCloseQueue(hQueue);
		}
	}

	return bRetorno;
}

void CServerRomaConfig::OnSetfocusEditMsmqError() 
{
	m_uIDSelecionado = IDC_EDIT_MSMQ_ERROR;	
}

void CServerRomaConfig::OnSetfocusListFilas(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_uIDSelecionado = IDC_LIST_FILAS;	
	
	*pResult = 0;
}

void CServerRomaConfig::RemoverFila(CStringArray &arrayName)
{
	int nContador, nIndex;

	for (nContador = 0; nContador < arrayName.GetSize(); nContador++)
	{
		LVFINDINFO info;

		info.flags = LVFI_STRING;
		info.psz = arrayName.GetAt(nContador);

		if ( (nIndex = m_ctrlListFilas.FindItem(&info)) > -1)
		{
			m_ctrlListFilas.DeleteItem(nIndex);
		}
	}	
}

void CServerRomaConfig::RemoverFilaMSMQ(CStringArray &arrayName)
{
	int nContador;
	CString strFila;

	for (nContador = 0; nContador < arrayName.GetSize(); nContador++)
	{
		strFila = arrayName.GetAt(nContador);

		ExcluirFilaMSMQ(strFila);
	}	

}

void CServerRomaConfig::ExcluirFilaMSMQ(CString strFila)
{
	WCHAR	wszPathName[256];
    WCHAR	wszOutFormatName[256];
    DWORD	dwOutFormatNameLength = sizeof(wszOutFormatName);
	HRESULT hr;

#ifndef _UNICODE
	char	szNomeFila[256];
	strcpy(szNomeFila, strFila);

	MultiByteToWideChar(CP_ACP, 0, szNomeFila, -1, wszPathName, 256);
#else
	wcscpy(wszPathName, strFila);
#endif

	hr = MQPathNameToFormatName((WCHAR*)wszPathName,
							  wszOutFormatName,
							  &dwOutFormatNameLength);

	hr = MQDeleteQueue((WCHAR*) wszOutFormatName);

	if (hr != MQ_OK)
	{
		CString strMensagem;

		strMensagem.Format(_T("%s"), ErrorMessage(hr));
		if (strMensagem.IsEmpty())
			strMensagem.Format(_T("%s"), ErrorMessage(GetLastError()));

		if (strMensagem.IsEmpty())
			strMensagem.Format(_T("Erro ao tentar excluír a fila %s"), strFila);

		AfxMessageBox(strMensagem);

		return;
	}
}
