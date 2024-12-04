/*****************************************************************************
 * Arquivo: Librarys.cpp                                                     *
 * Define funções usadas na versão Pascal.                                   *
 * Data:24/09/97                                                             *
 *****************************************************************************/
//#include "StdAfx.h"
#include "Library.h"

 /****************************************************************************
 *                                                                           *
 * ArquivoExiste                                                             *
 * Retorna TRUE se o arquivo pesquisado existe, e FALSE se não.              *
 *                                                                           *
 *****************************************************************************/
BOOL ArquivoExiste(CString txtArquivo)
{
	WIN32_FIND_DATA FindFileData;
	BOOL bReturn;

	HANDLE hFind;
	hFind = FindFirstFile(txtArquivo, &FindFileData); 
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		bReturn = TRUE; 
	}
	else
	{
		bReturn = FALSE;
	}
	FindClose(hFind);

	return (bReturn);
}

 /*****************************************************************************
 *                                                                           *
 * Alltrim                                                                   *
 * Retorna uma CString sem os espaços em branco da direita e da esquerda.    *
 *                                                                           *
 *****************************************************************************/
CString Alltrim(CString cadeia)
{
	cadeia.TrimLeft();
	cadeia.TrimRight();

	return cadeia;
}

#ifndef _WIN32_WCE
/*****************************************************************************
 *                                                                           *
 * Cls                                                                       *
 * Limpa a tela.                                                             *
 *                                                                           *
 *****************************************************************************/
void Cls(HANDLE hConsole)
{
	COORD coordScreen = { 0, 0 };       /* here's where we'll home the cursor */ 
	BOOL bSuccess;
	DWORD cCharsWritten; 
	CONSOLE_SCREEN_BUFFER_INFO csbi;    /* to get buffer info */ 
	DWORD dwConSize;                    /* number of character cells in the current buffer */ 

	/* get the number of character cells in the current buffer */ 
	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi); 
	Perr(bSuccess, "GetConsoleScreenBufferInfo"); 
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y; 

	/* fill the entire screen with blanks */ 
	bSuccess = FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten); 
	Perr(bSuccess, "FillConsoleOutputCharacter"); 

	/* get the current text attribute */ 
	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi); 
	Perr(bSuccess, "ConsoleScreenBufferInfo"); 

	/* now set the buffer's attributes accordingly */ 
	bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	Perr(bSuccess, "FillConsoleOutputAttribute"); 

	/* put the cursor at (0, 0) */ 
	bSuccess = SetConsoleCursorPosition(hConsole, coordScreen); 
	Perr(bSuccess, "SetConsoleCursorPosition"); 

	return; 
}

/*****************************************************************************
 *                                                                           *
 * Clear                                                                     *
 * Copia o conteúdo de uma matriz para outra.                                *
 *                                                                           *
 *****************************************************************************/
void Clear(HANDLE hConsole,int xa, int ya,int xb, int yb)
{
	GotoXY(hConsole,xa,ya);
	for (int nCont = ya;nCont <= yb;nCont++)
	{
		GotoXY(hConsole,xa,nCont);
		printf("%s",Space(xb-xa));
	}
}

/*****************************************************************************
 *                                                                           *
 * Gaussian                                                                  *
 * Calcula a curva randômica gaussiana.                                      *
 *                                                                           *
 *****************************************************************************/
double Gaussian(double nDesvio, double nMedia, double tTempo)
{
	double nResultado;

	double  nDesvioPotencia;
	double  nPotencia;

	nDesvioPotencia  = 2 * nDesvio * nDesvio;

	nResultado =  1 / (nDesvio * Raiz( 2, (2 * PI) ));
	nPotencia  =  (tTempo - nMedia) * (tTempo - nMedia);
	nPotencia  = -nPotencia / nDesvioPotencia;
	nResultado *= exp( nPotencia );

	return nResultado;
}

/*****************************************************************************
 *                                                                           *
 * GaussianRnd                                                               *
 * Retorna a área da curva randômica gaussina.                               *
 *                                                                           *
 *****************************************************************************/
double  GaussianRnd(double nDesvio, double nMedia, clock_t tInstanteInicial, clock_t tInstante1, clock_t tInstante2, double *AreaAmostrada)
{
	double nTempoConversa1, nTempoConversa2, nDiferenca;
	double  pf1, pf2, nCoeficiente, nArea;

	nTempoConversa1 = ((double)tInstante1 - (double)tInstanteInicial) / (double) CLOCKS_PER_SEC;
	nTempoConversa2 = ((double)tInstante2 - (double)tInstanteInicial) / (double) CLOCKS_PER_SEC;

	nDiferenca      = nTempoConversa2 - nTempoConversa1;

	pf1 = Gaussian((double)nDesvio, (double)nMedia, nTempoConversa1);
	pf2 = Gaussian((double)nDesvio, (double)nMedia, nTempoConversa2);

	if (*AreaAmostrada >= 1)
		*AreaAmostrada = 0.9999999999999;

	nCoeficiente = 1 / (1 - *AreaAmostrada);

	nArea = nDiferenca * ((pf1 + pf2)/2);

	*AreaAmostrada += nArea;

	nArea *= nCoeficiente;

	return (nArea);
}

double  GaussianRnd(double nDesvio, double nMedia, double tInstanteInicial, double  tInstante1, double  tInstante2, double *AreaAmostrada)
{
	double nTempoConversa1, nTempoConversa2, nDiferenca;
	double  pf1, pf2, nCoeficiente, nArea;
	time_t tInstanteInicialAux, tInstante1Aux, tInstante2Aux;

	tInstanteInicialAux = (time_t) tInstanteInicial;
	tInstante1Aux       = (time_t) tInstante1;
	tInstante2Aux       = (time_t) tInstante2;

	nTempoConversa1 = difftime(tInstante1Aux,tInstanteInicialAux);   
	nTempoConversa2 = difftime(tInstante2Aux,tInstanteInicialAux);  

	nDiferenca      = nTempoConversa2 - nTempoConversa1;

	pf1 = Gaussian((double)nDesvio, (double)nMedia, nTempoConversa1);
	pf2 = Gaussian((double)nDesvio, (double)nMedia, nTempoConversa2);

	if (*AreaAmostrada >= 1)
		*AreaAmostrada = 0.9999999999999;

	nCoeficiente = 1 / (1 - *AreaAmostrada);

	nArea = nDiferenca * ((pf1 + pf2)/2);

	*AreaAmostrada += nArea;

	return (nArea);
}

/*****************************************************************************
 *                                                                           *
 * GotoXY                                                                    *
 * Posiciona o cursor na linha e coluna indicada.                            *
 *                                                                           *
 *****************************************************************************/
void GotoXY(HANDLE hConsole, int x, int y)
{
	COORD nPos;

	nPos.X = x;
	nPos.Y = y;
	
	SetConsoleCursorPosition(hConsole, nPos);
}

/*****************************************************************************
 *                                                                           *
 * IniciaAreaGaussian                                                        *
 * Inicia a área para cálculo da curva randômica gaussiana.                  *
 *                                                                           *
 *****************************************************************************/
double IniciaAreaGaussian(double nMedia, double nDesvio, double nVezesDesvio)
{
	double nResultado=0;
	double pf1, pf2;
	double nTempoConversa1, nTempoConversa2;
	double t0, t1;
	double nDiferenca;

	t0 = (nMedia - ((double)Potencia(nDesvio, nVezesDesvio)));
	t1 = t0;

	for ( double nCont = t1; nCont < 0.0; nCont+=nDesvio)
	{

		nTempoConversa1 = (double)(t1 - t0);
		nTempoConversa2 = (double)(nCont - t0);

		nDiferenca = nTempoConversa2 - nTempoConversa1;

		pf1 = Gaussian(nDesvio, nMedia, nTempoConversa1);
		pf2 = Gaussian(nDesvio, nMedia, nTempoConversa2);

		nResultado += (nDiferenca * ((pf1 + pf2)/2));

		t1 = nCont;
	}

	return (nResultado);
}

/*****************************************************************************
 *                                                                           *
 * Perr                                                                      *
 * Imprime uma mensagem de erro caso o sistema não consiga executar a API    *
 *                                                                           *
 *****************************************************************************/
void Perr( BOOL bSucess , char *api  ) 
{
	if ( bSucess == FALSE)
		printf("\nAPI: %s - Erro: %d", api, GetLastError()); 
}

/*****************************************************************************
 *                                                                           *
 * SetConsoleSizeScreen                                                      *
 * Redimensiona o tamanho da tela de console.                                *
 *                                                                           *
 *****************************************************************************/
BOOL SetConsoleSizeScreen(HANDLE hConsole, int xSize, int ySize)
{

	CONSOLE_SCREEN_BUFFER_INFO csbi; /* hold current console buffer info */ 
	BOOL bSuccess; 
	SMALL_RECT srWindowRect; /* hold the new console size */ 
	COORD coordScreen; 

	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi); 
	Perr(bSuccess, "GetConsoleScreenBufferInfo"); 

	/* get the largest size we can size the console window to */ 
	coordScreen = GetLargestConsoleWindowSize(hConsole); 
	Perr(coordScreen.X | coordScreen.Y, "GetLargestConsoleWindowSize"); 

	/* define the new console window size and scroll position */ 
	srWindowRect.Right = (SHORT) (min(xSize, coordScreen.X) - 1); 
	srWindowRect.Bottom = (SHORT) (min(ySize, coordScreen.Y) - 1); 
	srWindowRect.Left = srWindowRect.Top = (SHORT) 0; 

	/* define the new console buffer size */ 
	coordScreen.X = xSize; 
	coordScreen.Y = ySize; 

	/* if the current buffer is larger than what we want, resize the */ 
	/* console window first, then the buffer */ 
	if ((DWORD) csbi.dwSize.X * csbi.dwSize.Y > (DWORD) xSize * ySize) 
	{ 
		bSuccess = SetConsoleWindowInfo(hConsole, TRUE, &srWindowRect); 
		Perr(bSuccess, "SetConsoleWindowInfo"); 

		bSuccess = SetConsoleScreenBufferSize(hConsole, coordScreen); 
		Perr(bSuccess, "SetConsoleScreenBufferSize"); 
	} 

	/* if the current buffer is smaller than what we want, resize the */ 
	/* buffer first, then the console window */ 
	if ((DWORD) csbi.dwSize.X * csbi.dwSize.Y < (DWORD) xSize * ySize) 
	{ 
		bSuccess = SetConsoleScreenBufferSize(hConsole, coordScreen); 
		Perr(bSuccess, "SetConsoleScreenBufferSize"); 

		bSuccess = SetConsoleWindowInfo(hConsole, TRUE, &srWindowRect); 
		Perr(bSuccess, "SetConsoleWindowInfo"); 
	} 
	/* if the current buffer *is* the size we want, don't do anything! */ 

	return(bSuccess); 
}

#endif


/*****************************************************************************
 *                                                                           *
 * Copychar                                                                  *
 * Copia o conteúdo de uma matriz para outra.                                *
 *                                                                           *
 *****************************************************************************/
void Copychar(TCHAR *origem, TCHAR *destino, int qtd)
{
	for (int cont = 0 ; cont < qtd; cont++)
	{
		destino[cont] = origem[cont];
	}
}
	
/*****************************************************************************
 *                                                                           *
 * Expoente                                                                  *
 * Retorna o expoente de uma potenciação.                                    *
 *                                                                           *
 *****************************************************************************/
double Expoente( double base, double resultado)
{
	double nResultado;

	nResultado = log(resultado) / log(base);

	return nResultado;
}

/****************************************************************************************************************
	CountWord
	Retorna a quantidade de elementos separador por "cSeparator". O parâmetro "cDelimiter" informa se o caracter
	cSeparator pode ou não fazer parte do elemento.
	Autor:		Edgard Kuboo
	Data:		28.12.2001
	Última Atualizaçao:	
	Parâmetros:	
		CString strText					- Cadeia de caracteres que contém a expressão analisada
		char	cSeparator				- Separador dos elementos
		char	cDelimiter				- Delimitador de uma string (por exemplo)
****************************************************************************************************************/
int CountWord(CString strText, char cSeparator, char cDelimiter)
{
	int		nIndex;
	int		nCount;
	int		nWord;
	char	cByte;
	char    cOldByte;

	cOldByte    = (char) 0;
	nCount		=  0;
	nWord		=  1;

	strText.TrimRight();

	for (nIndex = 0; nIndex < strText.GetLength(); nIndex++)
	{
		cByte = (char) strText[nIndex];

		if ((cOldByte == cDelimiter) && (nCount > 1) && (cByte != cSeparator))
			nCount--;
		
		if (cByte == cDelimiter)
			nCount++;

		if ((cByte == cSeparator) && (nCount % 2 == 0))
		{
			nCount = 0;
			nWord++;
		}

		cOldByte = cByte;
	}

	if (strText.GetLength() == 0)
		nWord = 0;

	return nWord;
}

/*****************************************************************************
 *                                                                           *
 * GetMQErrDescription                                                       *
 * Verifica se o que foi digitado existe no combo box                        *
 *                                                                           *
 *****************************************************************************/
CString GetMQErrDescription(ULONG hError)
{
	CString strErMsg;

	switch (hError)
	{
		case  MQ_ERROR:
			strErMsg = _T("GenericError ");
			break;
		case  MQ_ERROR_PROPERTY:
			strErMsg = _T("One or more of the passed properties are invalid. ");
			break;
		case  MQ_ERROR_QUEUE_NOT_FOUND:
			strErMsg = _T("The queue is not registered in the DS ");
			break;
		case  MQ_ERROR_QUEUE_EXISTS:
			strErMsg = _T("A queue with the same pathname is already registered ");
			break;
		case  MQ_ERROR_INVALID_PARAMETER:
			strErMsg = _T("An invalid parameter passed to a function. ");
			break;
		case  MQ_ERROR_INVALID_HANDLE:
			strErMsg = _T("An invalid handle passed to a function. ");
			break;
		case  MQ_ERROR_OPERATION_CANCELLED:
			strErMsg = _T("The operation was cancelled before it could be completed. ");
			break;
		case  MQ_ERROR_SHARING_VIOLATION:
			strErMsg = _T("Sharing violation. The queue is already opened for exclusive receive. ");
			break;
		case  MQ_ERROR_SERVICE_NOT_AVAILABLE:
			strErMsg = _T("The Message Queues service is not available ");
			break;
		case  MQ_ERROR_MACHINE_NOT_FOUND:
			strErMsg = _T("The specified machine could not be found. ");
			break;
		case  MQ_ERROR_ILLEGAL_SORT:
			strErMsg = _T("Illegal sort specified in MQLocateBegin (e.g., duplicate columns). ");
			break;
		case  MQ_ERROR_ILLEGAL_USER:
			strErMsg = _T("The user is an illegal user. ");
			break;
		case  MQ_ERROR_NO_DS:
			strErMsg = _T("No connection with this site's controller(s). ");
			break;
		case  MQ_ERROR_ILLEGAL_QUEUE_PATHNAME:
			strErMsg = _T("Illegal queue path name. ");
			break;
		case  MQ_ERROR_ILLEGAL_PROPERTY_VALUE:
			strErMsg = _T("Illegal property value. ");
			break;
		case  MQ_ERROR_ILLEGAL_PROPERTY_VT:
			strErMsg = _T("Invalid VARTYPE value. ");
			break;
		case  MQ_ERROR_BUFFER_OVERFLOW:
			strErMsg = _T("The buffer supplied to MQReceiveMessage for message body retrieval  was too small. The message is not removed from the queue and part  of the message body that fits in the buffer was copied. ");
			break;
		case  MQ_ERROR_IO_TIMEOUT:
			strErMsg = _T("The MQReceiveMessage timeout has expired ");
			break;
		case  MQ_ERROR_ILLEGAL_CURSOR_ACTION:
			strErMsg = _T("MQ_ACTION_PEEK_NEXT specified to MQReceiveMessage can not be used with  the current cursor position. ");
			break;
		case  MQ_ERROR_MESSAGE_ALREADY_RECEIVED:
			strErMsg = _T("A message that is currently pointed at by the cursor has been removed from  the queue by another process or by another call to MQReceiveMessage  without the use of this cursor. ");
			break;
		case  MQ_ERROR_ILLEGAL_FORMATNAME:
			strErMsg = _T("The given format name is invalid. ");
			break;
		case  MQ_ERROR_FORMATNAME_BUFFER_TOO_SMALL:
			strErMsg = _T("The format name buffer supplied to the API was too small  to fit the format name ");
			break;
		case  MQ_ERROR_UNSUPPORTED_FORMATNAME_OPERATION:
			strErMsg = _T("The requested operation for the specified format name is not  supported (e.g., delete a direct queue format name). ");
			break;
		case  MQ_ERROR_ILLEGAL_SECURITY_DESCRIPTOR:
			strErMsg = _T("The specified security descriptor is not a valid security descriptor. ");
			break;
		case  MQ_ERROR_SENDERID_BUFFER_TOO_SMALL:
			strErMsg = _T("The passed buffer for the user ID property is too small. ");
			break;
		case  MQ_ERROR_SECURITY_DESCRIPTOR_TOO_SMALL:
			strErMsg = _T("The size of the buffer passed to MQGetQueueSecurity is too small. ");
			break;
		case  MQ_ERROR_CANNOT_IMPERSONATE_CLIENT:
			strErMsg = _T("The RPC server can not impersonate the client application, hence security  credentials could not be verified. ");
			break;
		case  MQ_ERROR_ACCESS_DENIED:
			strErMsg = _T("Access is denied. ");
			break;
		case  MQ_ERROR_PRIVILEGE_NOT_HELD:
			strErMsg = _T("Client does not have the required privileges to perform the operation. ");
			break;
		case  MQ_ERROR_INSUFFICIENT_RESOURCES:
			strErMsg = _T("Insufficient resources to perform operation. ");
			break;
		case  MQ_ERROR_USER_BUFFER_TOO_SMALL:
			strErMsg = _T("Request failed because user buffer is too small to hold the returned information ");
			break;
		case  MQ_ERROR_MESSAGE_STORAGE_FAILED:
			strErMsg = _T("Could not store a recoverable or journal message. Message was not sent ");
			break;
		case  MQ_ERROR_SENDER_CERT_BUFFER_TOO_SMALL:
			strErMsg = _T("The passed buffer for the user certificate property is too small. ");
			break;
		case  MQ_ERROR_INVALID_CERTIFICATE:
			strErMsg = _T("The user certificate is not valid. ");
			break;
		case  MQ_ERROR_CORRUPTED_INTERNAL_CERTIFICATE:
			strErMsg = _T("The internal MSMQ certificate is corrupted. ");
			break;
		case  MQ_ERROR_NO_INTERNAL_USER_CERT:
			strErMsg = _T("The internal MSMQ certificate for the user does not exist. ");
			break;
		case  MQ_ERROR_CORRUPTED_SECURITY_DATA:
			strErMsg = _T("A cryptogrphic function has failed. ");
			break;
		case  MQ_ERROR_CORRUPTED_PERSONAL_CERT_STORE:
			strErMsg = _T("The personal certificate store is corrupted. ");
			break;
		case  MQ_ERROR_COMPUTER_DOES_NOT_SUPPORT_ENCRYPTION:
			strErMsg = _T("The computer does not support encryption operations. ");
			break;
		case  MQ_ERROR_BAD_SECURITY_CONTEXT:
			strErMsg = _T("Bad security context. ");
			break;
		case  MQ_ERROR_COULD_NOT_GET_USER_SID:
			strErMsg = _T("Could not get the SID information out of the thread token. ");
			break;
		case  MQ_ERROR_COULD_NOT_GET_ACCOUNT_INFO:
			strErMsg = _T("Could not get the account information for the user. ");
			break;
		case  MQ_ERROR_ILLEGAL_MQCOLUMNS:
			strErMsg = _T("Invalid MQCOLUMNS parameter ");
			break;
		case  MQ_ERROR_ILLEGAL_PROPID:
			strErMsg = _T("Invalid propid value ");
			break;
		case  MQ_ERROR_ILLEGAL_RELATION:
			strErMsg = _T("Invalid relation value in restriction ");
			break;
		case  MQ_ERROR_ILLEGAL_PROPERTY_SIZE:
			strErMsg = _T("Illegal property buffer size ");
			break;
		case  MQ_ERROR_ILLEGAL_RESTRICTION_PROPID:
			strErMsg = _T("Invalid propid value in MQRESTRICTION parameter ");
			break;
		case  MQ_ERROR_ILLEGAL_MQQUEUEPROPS:
			strErMsg = _T("Illegal MQQUEUEPROPS parameter, either null or with zero properties ");
			break;
		case  MQ_ERROR_PROPERTY_NOTALLOWED:
			strErMsg = _T("Invalid propid for the requested operation (e.g. PROPID_Q_INSTANCE  in MQSetQueueProperties) ");
			break;
		case  MQ_ERROR_INSUFFICIENT_PROPERTIES:
			strErMsg = _T("Not all the required properties for the operation were specified  in the input parameters ");
			break;
		case  MQ_ERROR_MACHINE_EXISTS:
			strErMsg = _T("Computer with the same name already exists in the site. ");
			break;
		case  MQ_ERROR_ILLEGAL_MQQMPROPS:
			strErMsg = _T("Illegal MQQMPROPS parameter, either null or with zero properties ");
			break;
		case  MQ_ERROR_DS_IS_FULL:
			strErMsg = _T("DS is full ");
			break;
		case  MQ_ERROR_DS_ERROR:
			strErMsg = _T("Internal DS error. ");
			break;
		case  MQ_ERROR_INVALID_OWNER:
			strErMsg = _T("Invalid object owner. For example MQCreateQueue failed because the QM  object is invalid ");
			break;
		case  MQ_ERROR_UNSUPPORTED_ACCESS_MODE:
			strErMsg = _T("The specified access mode is not supported. ");
			break;
		case  MQ_ERROR_RESULT_BUFFER_TOO_SMALL:
			strErMsg = _T("The supplied result buffer is too small ");
			break;
		case  MQ_ERROR_DELETE_CN_IN_USE:
			strErMsg = _T("The Connected Network can not be deleted, it is in use. ");
			break;
		case  MQ_ERROR_NO_RESPONSE_FROM_OBJECT_SERVER:
			strErMsg = _T("No response from object owner. ");
			break;
		case  MQ_ERROR_OBJECT_SERVER_NOT_AVAILABLE:
			strErMsg = _T("Object owner is not reachable. ");
			break;
		case  MQ_ERROR_QUEUE_NOT_AVAILABLE:
			strErMsg = _T("Error while reading from a queue residing on a remote computer ");
			break;
		case  MQ_ERROR_DTC_CONNECT:
			strErMsg = _T("Cannot connect to MS DTC ");
			break;
		case  MQ_ERROR_TRANSACTION_IMPORT:
			strErMsg = _T("Cannot import the transaction ");
			break;
		case  MQ_ERROR_TRANSACTION_USAGE:
			strErMsg = _T("Wrong transaction usage ");
			break;
		case  MQ_ERROR_TRANSACTION_SEQUENCE:
			strErMsg = _T("Wrong transaction operations sequence ");
			break;
		case  MQ_ERROR_MISSING_CONNECTOR_TYPE:
			strErMsg = _T("Connector Type is mandatory when sending Acknowledgment or secure message ");
			break;
		case  MQ_ERROR_STALE_HANDLE:
			strErMsg = _T("The Queue manager service has been restarted. The queue handle  is stale, and should be closed. ");
			break;
		case  MQ_ERROR_TRANSACTION_ENLIST:
			strErMsg = _T("Cannot enlist the transaction ");
			break;
		case  MQ_ERROR_QUEUE_DELETED:
			strErMsg = _T("The queue was deleted. Messages can not be received anymore using this  queue handle. The handle should be closed ");
			break;
		case  MQ_ERROR_ILLEGAL_CONTEXT:
			strErMsg = _T("Invalid context parameter (MQLocateBegin). ");
			break;
		case  MQ_ERROR_ILLEGAL_SORT_PROPID:
			strErMsg = _T("Invalid propid value in MQSORTSET ");
			break;
		case  MQ_ERROR_LABEL_TOO_LONG:
			strErMsg = _T("The passed label is too long. It should be less or equal to MQ_MAX_MSG_LABEL_LEN ");
			break;
		case  MQ_ERROR_LABEL_BUFFER_TOO_SMALL:
			strErMsg = _T("The label buffer supplied to the API was too small ");
			break;
		case  MQ_ERROR_MQIS_SERVER_EMPTY:
			strErMsg = _T("The list of MQIS servers (in registry) is empty. ");
			break;
		case  MQ_ERROR_MQIS_READONLY_MODE:
			strErMsg = _T("MQIS database is in read-only mode. ");
			break;
		case  MQ_ERROR_SYMM_KEY_BUFFER_TOO_SMALL:
			strErMsg = _T("The passed buffer for the Symmetric key property is too small. ");
			break;
		case  MQ_ERROR_SIGNATURE_BUFFER_TOO_SMALL:
			strErMsg = _T("The passed buffer for the Signature property is too small. ");
			break;
		case  MQ_ERROR_PROV_NAME_BUFFER_TOO_SMALL:
			strErMsg = _T("The passed buffer for the Provider name property is too small. ");
			break;
		case  MQ_ERROR_ILLEGAL_OPERATION:
			strErMsg = _T("The operation is illegal on foreign message queuing system ");
			break;
		case  MQ_ERROR_WRITE_NOT_ALLOWED:
			strErMsg = _T("Another MQIS server is being installed, write operations to the  database are not allowed at this stage. ");
			break;
		case  MQ_ERROR_WKS_CANT_SERVE_CLIENT:
			strErMsg = _T("MSMQ independent clients cannot serve MSMQ dependent clients. ");
			break;
		case  MQ_ERROR_DEPEND_WKS_LICENSE_OVERFLOW:
			strErMsg = _T("The number of dependent clients served by this MSMQ server reached  its upper limit. ");
			break;
		case  MQ_CORRUPTED_QUEUE_WAS_DELETED:
			strErMsg = _T("Ini file for queue %1 in LQS was deleted because it was corrupted. ");
			break;
		case  MQ_ERROR_REMOTE_MACHINE_NOT_AVAILABLE:
			strErMsg = _T("The remote machine is not available. ");
			break;
		case  MQ_INFORMATION_PROPERTY:
			strErMsg = _T("One or more of the passed properties resulted in warning but the  function completed. ");
			break;
		case  MQ_INFORMATION_ILLEGAL_PROPERTY:
			strErMsg = _T("Invalid property id. ");
			break;
		case  MQ_INFORMATION_PROPERTY_IGNORED:
			strErMsg = _T("The specified property is ignored in this operation  (e.g., PROPID_M_SENDERID in SendMessage(). ");
			break;
		case  MQ_INFORMATION_UNSUPPORTED_PROPERTY:
			strErMsg = _T("The specified property is not supported and is ignored in this operation ");
			break;
		case  MQ_INFORMATION_DUPLICATE_PROPERTY:
			strErMsg = _T("The specified property already appeared in the propid array, and is  ignored in this operation ");
			break;
		case  MQ_INFORMATION_OPERATION_PENDING:
			strErMsg = _T("Asynchronous operation is currently pending. ");
			break;
		case  MQ_INFORMATION_FORMATNAME_BUFFER_TOO_SMALL:
			strErMsg = _T("The format name buffer supplied to MQCreateQueue was too small  to fit the format name. Queue was created successfully ");
			break;
	default:
			strErMsg = _T("Don't seems to be a MQ error.");
	}
	return strErMsg;
}

/****************************************************************************************************************
	GetWord
	Retorna o elemento numa cadeia de caracter tendo um separador. O delimitador informa, se o caracter separador
	faz parte ou não do elemento. Por exemplo: Numa string
	Autor:		Edgard Kuboo
	Data:		28.12.2001
	Última Atualizaçao:	
	Parâmetros:	
		CString strText					- Cadeia de caracteres que contém a expressão analisada
		char	cSeparator				- Separador dos elementos
		char	cDelimiter				- Delimitador de uma string (por exemplo)
		int		nPosicao				- Posição em relação ao separador. Ou seja "Novo, Velho" - 
										  O indice 1 representa "Novo". O indice 2 representa "Velho"
****************************************************************************************************************/
CString GetWord(CString strText, char cSeparator, char cDelimiter, int nPosition)
{
	int		nIndex;
	int		nCount;
	int		nStartPos;
	int		nEndPos;
	int		nWord;
	char	cByte;
	char    cOldByte;
	CString	strResult;

	cOldByte    = (char) 0;
	nCount		=  0;
	nWord		=  1;
	nStartPos	= -1;
	nEndPos		= -1;

	strResult.Empty();

	for (nIndex = 0; nIndex < strText.GetLength(); nIndex++)
	{
		cByte = (char) strText[nIndex];

		if ((cOldByte == cDelimiter) && (nCount > 1) && (cByte != cSeparator))
			nCount--;
		
		if (cByte == cDelimiter)
			nCount++;

		if ((nWord == nPosition) && (nStartPos == -1))
			nStartPos = nIndex;

		if ((cByte == cSeparator) && (nCount % 2 == 0))
		{
			nCount = 0;
			if (nStartPos != -1)
			{
				nEndPos = nIndex;
				break;
			}
			nWord++;
		}

		cOldByte = cByte;
	}

	if ((nStartPos != -1) && (nEndPos == -1))
		nEndPos = nIndex;

	nCount = nEndPos - nStartPos;

	if (nCount > 0)
		strResult = strText.Mid(nStartPos, nCount);

	return strResult.AllocSysString();
}

/****************************************************************************************************************
	GetWordSeparatedBy
	Retorna o elemento numa cadeia de caracter tendo algum separador.
	Autor:		José Sahle Netto
	Data:		8.10.2001
	Última Atualizaçao:	
	Parâmetros:	
		CString strPalavra             - Cadeia de caracter que contém a expressão analisada
		int nPosicao                   - Posição em relação ao separador. Ou seja "Novo, Velho" - 
		                                 O indice 1 representa "Novo". O indice 2 representa "Velho"
		CString strSeparador = _T(",") - Separador. Este parâmetro é opcional. Caso não seja especificado a
		                                 vírgula será utilizada.
****************************************************************************************************************/
CString GetWordSeparatedBy(CString strPalavra, int nPosicao, CString strSeparador, CString strDelimitador)
{
    CString strAux;
    long nCont, nPos;
	BOOL bAspasAberto;

	if (Alltrim(strSeparador) == _T(""))
        strSeparador = _T(",");

	strPalavra = strSeparador + strPalavra;

	bAspasAberto = FALSE;
    nPos = 0;
    strAux = _T("");
    for(nCont = 0; nCont < strPalavra.GetLength(); nCont++)
	{
		CString strChar = strPalavra.Mid(nCont, 1);

        if(strChar == strDelimitador)
		{
           bAspasAberto = !bAspasAberto;
		   continue;
		}

        if(nPos == nPosicao)
		{
            if(strChar != strSeparador || bAspasAberto)
               strAux += strPalavra.Mid(nCont, 1);
		}   

        if(strChar == strSeparador && !bAspasAberto)
            nPos++;
        
        if(nPos > nPosicao)
			break;
    }

	return strAux;
}

/****************************************************************************************************************
	GetWordSeparatedByCount
	Retorna a quantidade de elementos dentro de uma string.
	Autor:		José Sahle Netto
	Data:		31.7.2003
	Última Atualizaçao:	
	Parâmetros:	
		CString strPalavra             - Cadeia de caracter que contém a expressão analisada
		CString strSeparador = _T(",") - Separador. Este parâmetro é opcional. Caso não seja especificado a
		                                 vírgula será utilizada.
****************************************************************************************************************/
int GetWordSeparatedByCount(CString strPalavra, CString strSeparador, CString strDelimitador)
{
    CString strAux;
    int nCont, nPos;
	BOOL bAspasAberto;

	if (Alltrim(strSeparador) == _T(""))
        strSeparador = _T(",");

	if (strPalavra.GetLength() == 0)
		return 0;

	strPalavra = strSeparador + strPalavra;

	bAspasAberto = FALSE;
    nPos = 0;
    strAux = _T("");
    for(nCont = 0; nCont < strPalavra.GetLength(); nCont++)
	{
        if(strPalavra.Mid(nCont, 1) == strSeparador && !bAspasAberto)
            nPos++;
        
        if(strPalavra.Mid(nCont, 1) == strDelimitador)
           bAspasAberto = !bAspasAberto;
    }

	return nPos;
}

/*****************************************************************************
 *                                                                           *
 * KillFocusComboBox                                                         *
 * Verifica se o que foi digitado existe no combo box                        *
 *                                                                           *
 *****************************************************************************/
BOOL KillFocusComboBox(UINT iID, CWnd* pParentWnd)
{
	CString strEditBox, strItem;
	int nCont;

	CComboBox * pComboBox = (CComboBox*) pParentWnd->GetDlgItem(iID);

	pComboBox->GetWindowText(strEditBox);

	strEditBox.MakeUpper();
	strEditBox.TrimLeft();
	strEditBox.TrimRight();

	// Verifica se o que foi digitado é igual a um existente no item
	for (nCont = 0; nCont < pComboBox->GetCount(); nCont++)
	{
		pComboBox->GetLBText(nCont, strItem);
		
		strItem.MakeUpper();
		strItem.TrimLeft();
		strItem.TrimRight();
		if (strItem == strEditBox)
		{
			pComboBox->SetCurSel(nCont);
			pComboBox->GetLBText(nCont, strItem);
			return TRUE;
		}
	}

	// Verifica se o que foi digitado é parecido com algum arquivo
	for (nCont = 0; nCont < pComboBox->GetCount(); nCont++)
	{
		pComboBox->GetLBText(nCont, strItem);
		
		strItem.MakeUpper();
		strItem.TrimLeft();
		strItem.TrimRight();
		if (strItem.Find(strEditBox, 0) == 0)
		{
			pComboBox->SetCurSel(nCont);
			pComboBox->SetWindowText(strItem);
			return TRUE;
		}
	}

	pComboBox->SetCurSel(-1);	

	return FALSE;
}

/*****************************************************************************
 *                                                                           *
 * ErrorMessage                                                              *
 * Retorna a descrição de uma erro                                           *
 *                                                                           *
 *****************************************************************************/
CString ErrorMessage(DWORD err)
{
	LPTSTR lpMsgBuf;
	CString strMensagemDeErro;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL ); // Display the string.

	strMensagemDeErro = lpMsgBuf;

	LocalFree( lpMsgBuf );

	return strMensagemDeErro;
}

/*****************************************************************************
 *                                                                           *
 * DispErrDesc                                                               *
 * Descrição da mensagem de erro                                             *
 *                                                                           *
 *****************************************************************************/
void DispErrDesc(DWORD err, CString strAplicacao)
{
	CString strMensagem;

	if(!err)
		err = GetLastError();

	if((err & 0xC0000000L) == 0xC0000000L)
		strMensagem =  GetMQErrDescription(err);
	else
		strMensagem = ErrorMessage(err);

	LogEvent(strMensagem, EVENTLOG_ERROR_TYPE, strAplicacao);
}

/*****************************************************************************
 *                                                                           *
 * FindOneOf                                                                 *
 * Encontra um caracter                                                      *
 *                                                                           *
 *****************************************************************************/
LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
    while (p1 != NULL && *p1 != NULL)
    {
        LPCTSTR p = p2;
        while (p != NULL && *p != NULL)
        {
            if (*p1 == *p)
                return CharNext(p1);
            p = CharNext(p);
        }
        p1 = CharNext(p1);
    }
    return NULL;
}

/*****************************************************************************
 *                                                                           *
 * LogEvent                                                                  *
 * Grava mensagem o EventViewer                                              *
 *                                                                           *
 *****************************************************************************/
#define MSG_ERR_EXIST                         ((DWORD)0xC0000004L)
void LogEvent(CString strMensagem, WORD wType, CString strAplicacao)
{
    TCHAR    chMsg[256];
    HANDLE  hEventSource;
    LPTSTR  lpszStrings[1];
	TCHAR MsgBuf[1024];
	LPTSTR lpMsgBuf = MsgBuf;
	//DWORD dwSize = strMensagem.GetLength() + 1;
	
	strMensagem = _T("\n") + strMensagem;

	strcpy(chMsg, strMensagem);
    lpszStrings[0] = chMsg;

    hEventSource = RegisterEventSource(NULL, strAplicacao);
    if (hEventSource != NULL)
    {
		//EVENTLOG_INFORMATION_TYPE
        if (!ReportEvent(hEventSource, wType, 1, 0, 0, 1, 0 /*dwSize*/, (LPCTSTR*) lpszStrings, NULL/*(LPCTSTR*) lpszStrings*/))
		{
			DWORD err = GetLastError();
			
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL ); // Display the string.
			
			strMensagem = MsgBuf;
		}
        DeregisterEventSource(hEventSource);
    }
}


/*****************************************************************************
 *                                                                           *
 * Nomearq                                                                   *
 * Retorna o nome do arquivo sem a extensao.                                 *
 *                                                                           *
 *****************************************************************************/
CString Nomearq(CString strArquivo)
{
	CString strNome;

	strNome = _T("");
	for (int nCont = 0; nCont < strArquivo.GetLength(); nCont++)
	{
		if (strArquivo[nCont] ==  _T('.')) 
			break;
		else strNome = strNome + strArquivo[nCont];
	}
	
	return(strNome);
}

/*****************************************************************************
 *                                                                           *
 * Padl                                                                      *
 * Retorna uma CString com o tamanho especificado. Se o tamanho for maior que*
 * cadeia, retorna com espaços à esquerda.                                   *
 *                                                                           *
 *****************************************************************************/
CString Padl(const CString& strCadeia,int nTamanho)
{
	CString strCadeia_de_Retorno;
	
	if(strCadeia.GetLength() > nTamanho) 
		strCadeia_de_Retorno = strCadeia;
	else
	{
		strCadeia_de_Retorno = Space(nTamanho - strCadeia.GetLength());
		strCadeia_de_Retorno = strCadeia_de_Retorno + strCadeia;
	}
	
    return (strCadeia_de_Retorno);
}

/*****************************************************************************
 *                                                                           *
 * Padr                                                                      *
 * Retorna uma CString com o tamanho especificado. Se o tamanho for maior que*
 * cadeia, retorna com espaços à direita.                                    *
 *                                                                           *
 *****************************************************************************/
CString Padr(const CString& strCadeia,int nTamanho)
{
	CString strCadeia_de_Retorno;
	
	if(strCadeia.GetLength() >= nTamanho) 
		strCadeia_de_Retorno = strCadeia;
	else
	{
		strCadeia_de_Retorno = Space(nTamanho - strCadeia.GetLength());
		strCadeia_de_Retorno = strCadeia + strCadeia_de_Retorno;
	}
	
    return (strCadeia_de_Retorno);
}

/*****************************************************************************
 *                                                                           *
 * Potencia                                                                  *
 * Retorna o valor de um número elevado a outro.                             *
 *                                                                           *
 *****************************************************************************/
double Potencia( double x, double y )
{
	double nResultado;

	nResultado = exp( (y * log(x)) );

	return ( nResultado );
}

/*****************************************************************************
 *                                                                           *
 * Raiz                                                                      *
 * Retorna a raiz de um número.                                              *
 *                                                                           *
 *****************************************************************************/
double Raiz(double expoente, double nValor)
{
	double nResultado;

	nResultado = exp( log(nValor) / expoente );

	return nResultado;
}

/*****************************************************************************
 *                                                                           *
 * Random                                                                    *
 * Retorna um pseudorandomico.                                               *
 *                                                                           *
 *****************************************************************************/
double Random(int nMax, int nLimite)
{
	int nRandom = 0;
	double nResultado= 0.0;
	
	nRandom = rand();

	nResultado = nMax * nRandom;
	nResultado = nResultado / (RAND_MAX + nLimite);

	return ( nResultado );
}

/*****************************************************************************
 *                                                                           *
 * GetPartOf                                                                 *
 * Retorna uma CString a partir de um caminho. Se for especificado, no tipo, *
 * ARCHIVE retorna a parte correspondente ao arquivo. Se for especificado    *
 * DIRECTORY retorna a parte correspondente ao diretório.                    *
 *                                                                           *
 *****************************************************************************/
CString GetPartOf(const CString cadeia,int tipo, int DOBRO)
{
	CString dir;
	Boolean e_dir;
	int cont;
	
	dir = _T("");
	e_dir = False;
	
	for (cont = 0; cont < cadeia.GetLength();cont++)
	{
		// Retorna o diretório.
		if (tipo == _A_DIR)
		{
			if ((cadeia[cadeia.GetLength()-cont-1] == _T('\\')) || (cadeia[cadeia.GetLength()-cont-1] == _T(':'))) 
				e_dir = True;
			if (e_dir==True)
			{
				if (cadeia[cadeia.GetLength()-cont-1] == _T('\\') && (DOBRO == dOUBLE))
					dir = dir + _T('\\') + _T('\\');
				else
					dir = dir + cadeia[cadeia.GetLength()-cont-1];
			}
		}

		// Retorna o arquivo.
		if (tipo == _A_ARQ)
		{
			if ((cadeia[cadeia.GetLength()-cont-1] == _T('\\')) || (cadeia[cadeia.GetLength()-cont-1] == _T(':'))) 
				e_dir = True;
			if (!e_dir==True)
			{
				if (cadeia[cadeia.GetLength()-cont-1] == _T('\\') && (DOBRO == dOUBLE))
					dir = dir + _T('\\') + _T('\\');
				else
					dir = dir + cadeia[cadeia.GetLength()-cont-1];
			}
		}

		// Retorna o diretorio pai.
		if ( tipo == _A_PAI)
		{
			if ((cadeia[cadeia.GetLength()-cont-1] == _T('\\')) || (cadeia[cadeia.GetLength()-cont-1] == _T(':'))) 
			{
				if (e_dir)
					break;
					
				e_dir = True;
			}
			if (e_dir==True)
			{
				if ((cadeia[cadeia.GetLength()-cont-1] == _T('\\')) || (cadeia[cadeia.GetLength()-cont-1] == _T(':'))) 
				{
				}
				else
					dir = dir + cadeia[cadeia.GetLength()-cont-1];
			}
		}
	}
	
	dir.MakeReverse();

	return (dir);
}

/*****************************************************************************
 *                                                                           *
 * Round                                                                     *
 * Arredonda um valor. P.e., se a frase for ate 0.5 arredonda para baixo e   *
 * se maior arredonta para cima                                              *
 *                                                                           *
 *****************************************************************************/
int Round(double nValor)
{
	int nFix;
	double nDecimal;

	nFix = (int) nValor;
	nDecimal = (nValor - nFix);

	if ( nDecimal <= 0.5 )
		return (nFix);
	else
		return (nFix+1);
}

/*****************************************************************************
 *                                                                           *
 * Space                                                                     *
 * Retorna uma CString com o tamanho especificado de espaços em branco.      *
 *                                                                           *
 *****************************************************************************/
CString Space(int tamanho)
{
	CString cadeia_de_retorno;
	
	for (int cont = 0; cont < tamanho; cont++)
		cadeia_de_retorno = cadeia_de_retorno + _T(" ");
	
    return (cadeia_de_retorno);
}

/*****************************************************************************
 *                                                                           *
 * Str                                                                       *
 * Converte um valor numérico em string.                                     *
 *                                                                           *
 *****************************************************************************/
CString Str(int valor)
{
	CString retorno;

	retorno.Format(_T("%i"), valor);

	return (retorno);
}

CString Str(unsigned int valor)
{
	CString retorno;
	
	retorno.Format(_T("%i"), valor);

	return (retorno);
}

CString Str(long valor)
{
	CString retorno;

	retorno.Format(_T("%i"), valor);

	return (retorno);
}

CString Str(unsigned long valor)
{
	CString retorno;

	retorno.Format(_T("%i"), valor);

	return (retorno);
}

CString Str(WORD valor)
{
	CString retorno;

	retorno.Format(_T("%d"), valor);

	return (retorno);
}


CString Str(float valor)
{
	CString retorno;

	retorno.Format(_T("%f"), valor);

	return (retorno);
}

CString Str(double valor)
{
	CString retorno;

	retorno.Format(_T("%f"), valor);

	return (retorno);
}

/*****************************************************************************
 *                                                                           *
 * Str                                                                       *
 * Converte um valor numérico em string.                                     *
 *                                                                           *
 *****************************************************************************/
CString Str(int valor, int tamanho)
{
	CString retorno;
	CString auxiliar;
	TCHAR variavel[200];

#ifndef _UNICODE
	sprintf(variavel, _T("%d"), valor);
#else
	swprintf(variavel, _T("%d"), valor);
#endif

	retorno = variavel;

	auxiliar = _T("");

	for (int cont = 1; cont <= tamanho - retorno.GetLength(); cont++)
		auxiliar = auxiliar + _T(" ");

	retorno = auxiliar + retorno;

	return (retorno);
}

CString Str(unsigned int valor, int tamanho)
{
	CString retorno;
	CString auxiliar;
	TCHAR variavel[200];

#ifndef _UNICODE
	sprintf(variavel, _T("%d"), valor);
#else
	swprintf(variavel, _T("%d"), valor);
#endif

	retorno = variavel;

	auxiliar = _T("");

	for (int cont = 1; cont <= tamanho - retorno.GetLength(); cont++)
		auxiliar = auxiliar + _T(" ");

	retorno = auxiliar + retorno;

	return (retorno);
}

CString Str(long valor, int tamanho)
{
	CString retorno;
	CString auxiliar;
	TCHAR variavel[200];

#ifndef _UNICODE
	sprintf(variavel, _T("%d"), valor);
#else
	swprintf(variavel, _T("%d"), valor);
#endif

	retorno = variavel;

	auxiliar = _T("");

	for (int cont = 1; cont <= tamanho - retorno.GetLength(); cont++)
		auxiliar = auxiliar + _T(" ");

	retorno = auxiliar + retorno;

	return (retorno);
}

CString Str(float valor, int tamanho)
{
	CString retorno;
	CString auxiliar;
	TCHAR variavel[200];

#ifndef _UNICODE
	sprintf(variavel, _T("%f"), valor);
#else
	swprintf(variavel, _T("%f"), valor);
#endif

	retorno = variavel;

	auxiliar = _T("");

	for (int cont = 1; cont <= tamanho - retorno.GetLength(); cont++)
		auxiliar = auxiliar + _T(" ");

	retorno = auxiliar + retorno;

	return (retorno);
}

CString Str(double valor, int tamanho)
{
	CString retorno;
	CString auxiliar;
	TCHAR variavel[200];

#ifndef _UNICODE
	sprintf(variavel, _T("%f"), valor);
#else
	swprintf(variavel, _T("%f"), valor);
#endif

	retorno = variavel;

	auxiliar = _T("");

	for (int cont = 1; cont <= tamanho - retorno.GetLength(); cont++)
		auxiliar = auxiliar + _T(" ");

	retorno = auxiliar + retorno;

	return (retorno);
}

/*****************************************************************************
 *                                                                           *
 * StrVal                                                                    *
 * Converte um valor numérico em string.                                     *
 *                                                                           *
 *****************************************************************************/

CString StrVal(long valor)
{
	CString retorno;
	TCHAR variavel[200];

#ifndef _UNICODE
	sprintf(variavel, _T("%d"), valor);
#else
	swprintf(variavel, _T("%d"), valor);
#endif

	retorno = variavel;

	return (retorno);
}	


CString StrVal(double valor)
{
	CString strVariavel;

	TCHAR szVariavel[200];

#ifndef _UNICODE
	sprintf(szVariavel, _T("%f"), valor);
#else
	swprintf(szVariavel, _T("%f"), valor);
#endif

	strVariavel = szVariavel;

	return strVariavel;
}

CString StrVal(float valor)
{
	CString strVariavel;

	TCHAR szVariavel[200];

#ifndef _UNICODE
	sprintf(szVariavel, _T("%f"), valor);
#else
	swprintf(szVariavel, _T("%f"), valor);
#endif

	strVariavel = szVariavel;

	return strVariavel;
}

CString StrVal(double valor, int niDecimal)
{
	CString strVariavel, strMascara;

	TCHAR szVariavel[200];

	strMascara = _T("%.") + Str(niDecimal) + _T("f");

#ifndef _UNICODE
	sprintf(szVariavel, strMascara, valor);
#else
	swprintf(szVariavel, strMascara, valor);
#endif

	strVariavel = szVariavel;

	return strVariavel;
}

CString StrVal(float valor, int niDecimal)
{
	CString strVariavel, strMascara;

	TCHAR szVariavel[200];

	strMascara = _T("%.") + Str(niDecimal) + _T("f");

#ifndef _UNICODE
	sprintf(szVariavel, strMascara, valor);
#else
	swprintf(szVariavel, strMascara, valor);
#endif

	strVariavel = szVariavel;

	return strVariavel;
}


/*****************************************************************************
 *                                                                           *
 * Strzero                                                                   *
 * Converte um valor numérico em string.                                     *
 *                                                                           *
 *****************************************************************************/
CString Strzero(int valor, int tamanho)
{
	CString retorno;
	CString auxiliar;
	TCHAR variavel[200];

#ifndef _UNICODE
	sprintf(variavel, _T("%d"), valor);
#else
	swprintf(variavel, _T("%d"), valor);
#endif

	retorno = variavel;

	auxiliar = _T("");

	for (int cont = 1; cont <= tamanho - retorno.GetLength(); cont++)
		auxiliar = auxiliar + _T("0");

	retorno = auxiliar + retorno;

	return (retorno);
}	

CString Strzero(unsigned int valor, int tamanho)
{
	CString retorno;
	CString auxiliar;
	TCHAR variavel[200];

#ifndef _UNICODE
	sprintf(variavel, _T("%d"), valor);
#else
	swprintf(variavel, _T("%d"), valor);
#endif

	retorno = variavel;

	auxiliar = _T("");

	for (int cont = 1; cont <= tamanho - retorno.GetLength(); cont++)
		auxiliar = auxiliar + _T("0");

	retorno = auxiliar + retorno;

	return (retorno);
}	

CString Strzero(long valor, int tamanho)
{
	CString retorno;
	CString auxiliar;
	TCHAR variavel[200];

#ifndef _UNICODE
	sprintf(variavel, _T("%d"), valor);
#else
	swprintf(variavel, _T("%d"), valor);
#endif

	retorno = variavel;

	auxiliar = _T("");

	for (int cont = 1; cont <= tamanho - retorno.GetLength(); cont++)
		auxiliar = auxiliar + _T("0");

	retorno = auxiliar + retorno;

	return (retorno);
}	

CString Strzero(unsigned long valor, int tamanho)
{
	CString retorno;
	CString auxiliar;
	TCHAR variavel[200];

#ifndef _UNICODE
	sprintf(variavel, _T("%d"), valor);
#else
	swprintf(variavel, _T("%d"), valor);
#endif

	retorno = variavel;

	auxiliar = _T("");

	for (int cont = 1; cont <= tamanho - retorno.GetLength(); cont++)
		auxiliar = auxiliar + _T("0");

	retorno = auxiliar + retorno;

	return (retorno);
}	

CString Strzero(double valor, int tamanho)
{
	CString retorno;
	CString auxiliar;
	TCHAR variavel[200];

#ifndef _UNICODE
	sprintf(variavel, _T("%f"), valor);
#else
	swprintf(variavel, _T("%f"), valor);
#endif

	retorno = variavel;

	auxiliar = _T("");

	for (int cont = 1; cont <= tamanho - retorno.GetLength(); cont++)
		auxiliar = auxiliar + _T("0");

	retorno = auxiliar + retorno;

	return (retorno);
}	

CString Strzero(float valor, int tamanho)
{
	CString retorno;
	CString auxiliar;
	TCHAR variavel[200];

#ifndef _UNICODE
	sprintf(variavel, _T("%f"), valor);
#else
	swprintf(variavel, _T("%f"), valor);
#endif


	retorno = variavel;

	auxiliar = _T("");

	for (int cont = 1; cont <= tamanho - retorno.GetLength(); cont++)
		auxiliar = auxiliar + "0";

	retorno = auxiliar + retorno;

	return (retorno);
}	

/*****************************************************************************
 *                                                                           *
 * Repeat                                                                    *
 * Repete um caracter n vezes                                                *
 * cadeia, retorna com espaços à direita.                                    *
 *                                                                           *
 *****************************************************************************/
CString Repeat(CString strCaracter, int niRepeticao)
{
	CString strResult;

	strResult.Empty();
	for (int niCont = 0; niCont < niRepeticao; niCont++)
		strResult += strCaracter;

	return strResult;
}

/*****************************************************************************
 *                                                                           *
 * Upper                                                                     *
 * Retorna uma CString com as letras em maiúsculas.                          *
 *                                                                           *
 *****************************************************************************/
CString Upper(CString& cadeia)
{
	CString auxiliar;

	auxiliar = cadeia;
	auxiliar.MakeUpper();

	return (auxiliar);
}		

/*****************************************************************************
 *                                                                           *
 * TimeConv                                                                  *
 * Transforma segundos em formato hora.                                      *
 *                                                                           *
 *****************************************************************************/
CString TimeConv( double dwSec, BOOL bSecond )
{
	int dwTempo;
	int nHora, nMin, nSeg;
	CString szRetorno;

	dwTempo = (int) dwSec;

	if (dwSec > 0 )
	{
		nHora = dwTempo / 3600;
		dwTempo = dwTempo % 3600;
		nMin = dwTempo / 60;
		nSeg = dwTempo % 60;

		if(bSecond)
			szRetorno = Strzero(nHora,2) + _T(":") + Strzero(nMin,2) + _T(":") + Strzero(nSeg,2);
		else
			szRetorno = Strzero(nHora,2) + _T(":") + Strzero(nMin,2);

		return (szRetorno);
	}
	else
	{
		if(bSecond)
			return ( _T("00:00:00") );
		else
			return ( _T("00:00") );
	}
}


/*****************************************************************************
 *                                                                           *
 * IsLetter                                                                  *
 * Verifica se o caracter é letra                                            *
 *                                                                           *
 *****************************************************************************/
BOOL IsLetter(CString strLetter)
{
	int niValor;

	if (strLetter == _T(""))
		strLetter = _T(" ");

	niValor = 0;

	if(strLetter >= _T("A") && strLetter <= _T("Z"))
		return TRUE;
	else if (strLetter >= _T("a") && strLetter <= _T("z"))
		return TRUE;
	else if (strLetter == _T("Ä") || strLetter == _T("ä"))
		return TRUE;
	else if (strLetter == _T("Ë") || strLetter == _T("ë"))
		return TRUE;
	else if (strLetter == _T("Ï") || strLetter == _T("ï"))
		return TRUE;
	else if (strLetter == _T("Ö") || strLetter == _T("ö"))
		return TRUE;
	else if (strLetter == _T("Ü") || strLetter == _T("ü"))
		return TRUE;
	else if (strLetter == _T("Ã") || strLetter == _T("ã"))
		return TRUE;
	else if (strLetter == _T("Õ") || strLetter == _T("õ"))
		return TRUE;
	else if (strLetter == _T("Á") || strLetter == _T("á"))
		return TRUE;
	else if (strLetter == _T("É") || strLetter == _T("é"))
		return TRUE;
	else if (strLetter == _T("Í") || strLetter == _T("í"))
		return TRUE;
	else if (strLetter == _T("Ó") || strLetter == _T("ó"))
		return TRUE;
	else if (strLetter == _T("Ú") || strLetter == _T("ú"))
		return TRUE;
	else if (strLetter == _T("À") || strLetter == _T("à"))
		return TRUE;
	else if (strLetter == _T("È") || strLetter == _T("è"))
		return TRUE;
	else if (strLetter == _T("Ì") || strLetter == _T("ì"))
		return TRUE;
	else if (strLetter == _T("Ò") || strLetter == _T("ò"))
		return TRUE;
	else if (strLetter == _T("Ù") || strLetter == _T("ù"))
		return TRUE;
	else if (strLetter == _T("Â") || strLetter == _T("â"))
		return TRUE;
	else if (strLetter == _T("Ê") || strLetter == _T("ê"))
		return TRUE;
	else if (strLetter == _T("Î") || strLetter == _T("î"))
		return TRUE;
	else if (strLetter == _T("Ô") || strLetter == _T("ô"))
		return TRUE;
	else if (strLetter == _T("Û") || strLetter == _T("û"))
		return TRUE;
	else if (strLetter == _T("Ç") || strLetter == _T("ç"))
		return TRUE;
	else if (strLetter == _T("Ñ") || strLetter == _T("ñ"))
		return TRUE;
	else
		return FALSE;

}

/*****************************************************************************
 *                                                                           *
 * IsNumeric                                                                 *
 * Verifica se o caracter é número                                           *
 *                                                                           *
 *****************************************************************************/
BOOL IsNumeric(CString strLetter)
{
	if (strLetter == _T(""))
		strLetter = _T(" ");
	
	if(strLetter >= _T("0") && strLetter <= _T("9"))
		return TRUE;
	
	return FALSE;
}

/*****************************************************************************
 *                                                                           *
 * Val                                                                       *
 * Converte uma String para numérico                                         *
 *                                                                           *
 *****************************************************************************/
void Val(CString strCadeia, short &pValor)
{
	strCadeia.Replace(_T(","), _T("."));

#ifdef UNICODE
	pValor =  (short)_wtoi(strCadeia);
#else
	pValor =  atoi(strCadeia);
#endif
}

void Val(CString strCadeia, int &pValor)
{
	strCadeia.Replace(_T(","), _T("."));

#ifdef UNICODE
	pValor =  _wtoi(strCadeia);
#else
	pValor =  atoi(strCadeia);
#endif
}

void Val(CString strCadeia, WORD &pValor)
{
	strCadeia.Replace(_T(","), _T("."));

#ifdef UNICODE
	pValor =  (WORD)_wtoi(strCadeia);
#else
	pValor =  (WORD)atoi(strCadeia);
#endif
}

void Val(CString strCadeia, long &pValor)
{
	strCadeia.Replace(_T(","), _T("."));

#ifdef UNICODE
	pValor =  _wtol(strCadeia);
#else
	pValor =  atol(strCadeia);
#endif
}

void Val(CString strCadeia, unsigned long &pValor)
{
	strCadeia.Replace(_T(","), _T("."));

#ifdef UNICODE
	pValor =  _wtol(strCadeia);
#else
	pValor =  atol(strCadeia);
#endif
}

void Val(CString strCadeia, double &pValor)
{
	TCHAR* c;

	strCadeia.Replace(_T(","), _T("."));

#ifdef UNICODE
	pValor = wcstod(strCadeia, &c);
#else
	pValor = strtod(strCadeia, &c);
#endif
}

void Val(CString strCadeia, float &pValor)
{
	TCHAR* c;

	strCadeia.Replace(_T(","), _T("."));

#ifdef UNICODE
	pValor = (float)wcstod(strCadeia, &c);
#else
	pValor = (float)strtod(strCadeia, &c);
#endif
}

/*****************************************************************************
 *                                                                           *
 * CToD                                                                      *
 * Converte caracter para data                                               *
 *                                                                           *
 *****************************************************************************/
void CToD(const CString& strValor, CTime &tmRet, int nOption) // Formato:dd/mm/aaaa hh:mm:ss
{
	int nValor;
	int nAno, nMes, nDia, nHora, nMinuto, nSegundo;

	if(strValor == _T(" / / ") || strValor.IsEmpty())
		tmRet = CTime(1970, 1, 1, 0, 0, 0);
	else
	{

		Val( strValor.Mid(6, 4), nValor);

		if( nValor > 2038)
		{
			tmRet = CTime(2039, 1, 1, 0, 0, 0);
		}
		else
			switch(nOption)
			{
			case DT_DATETIME:
			default:
				{
					ASSERT(strValor.GetLength() == 19);
					
					Val(strValor.Mid(6, 4) , nAno); 
					Val(strValor.Mid(3, 2) , nMes);
					Val(strValor.Mid(0, 2) , nDia);
					Val(strValor.Mid(11, 2), nHora);
					Val(strValor.Mid(14, 2), nMinuto);
					Val(strValor.Mid(17, 2), nSegundo);

					CTime tm(	nAno, 
								nMes, 
								nDia, 
								nHora, 
								nMinuto, 
								nSegundo);

					tmRet = tm;
				}
				break;
			case DT_DATE:
				{
					ASSERT(strValor.GetLength() == 10);

					Val(strValor.Mid(6, 4) , nAno); 
					Val(strValor.Mid(3, 2) , nMes);
					Val(strValor.Mid(0, 2) , nDia);

					CTime tm(	nAno, 
								nMes, 
								nDia, 
								0, 
								0, 
								0);
					tmRet = tm;
				}
				break;
			case DT_TIME:
				{
					ASSERT(strValor.GetLength() == 8);

					Val(strValor.Mid(11, 2), nHora);
					Val(strValor.Mid(14, 2), nMinuto);
					Val(strValor.Mid(17, 2), nSegundo);

					CTime tm(	1970, 
								1, 
								1, 
								nHora, 
								nMinuto, 
								nSegundo);

					tmRet = tm;
				}
				break;
			}
	}
}

void CToD(const CString& strValor, DATE &dtRet, int nOption)
{
	int nValor;
	int nAno, nMes, nDia, nHora, nMinuto, nSegundo;

	if(strValor == _T(" / / ") || strValor.IsEmpty())
		dtRet = 0;
	else
	{

		Val( strValor.Mid(6, 4), nValor);

		if( nValor > 2038)
		{
			COleDateTime tm(2039, 1, 1, 0, 0, 0);
			dtRet = tm;
		}
		else
			switch(nOption)
			{
			case DT_DATETIME:
			default:
				{
					ASSERT(strValor.GetLength() == 19);
					
					Val(strValor.Mid(6, 4) , nAno); 
					Val(strValor.Mid(3, 2) , nMes);
					Val(strValor.Mid(0, 2) , nDia);
					Val(strValor.Mid(11, 2), nHora);
					Val(strValor.Mid(14, 2), nMinuto);
					Val(strValor.Mid(17, 2), nSegundo);

					COleDateTime tm(nAno, 
									nMes, 
									nDia, 
									nHora, 
									nMinuto, 
									nSegundo);

					dtRet = tm;
				}
				break;
			case DT_DATE:
				{
					ASSERT(strValor.GetLength() == 10);

					Val(strValor.Mid(6, 4) , nAno); 
					Val(strValor.Mid(3, 2) , nMes);
					Val(strValor.Mid(0, 2) , nDia);

					COleDateTime tm(nAno, 
									nMes, 
									nDia, 
									0, 
									0, 
									0);
					dtRet = tm;
				}
				break;
			case DT_TIME:
				{
					ASSERT(strValor.GetLength() == 8);

					Val(strValor.Mid(11, 2), nHora);
					Val(strValor.Mid(14, 2), nMinuto);
					Val(strValor.Mid(17, 2), nSegundo);

					COleDateTime tm(1899, 
									1, 
									1, 
									nHora, 
									nMinuto, 
									nSegundo);

					dtRet = tm;
				}
				break;
			}
	}
}


/*****************************************************************************
 *                                                                           *
 * DToC                                                                      *
 * Converte data para caracter                                               *
 *                                                                           *
 *****************************************************************************/
void DToC(const CTime& tm, CString &strData, int nOption)
{
	CString strValor;

	if(tm < 0)
	{
		strData = _T(" / / ");
		return;
	}
	
	strValor.Format(	_T("%02d/%02d/%04d %02d:%02d:%02d"), 
				tm.GetDay(),
				tm.GetMonth(),
				tm.GetYear(),
				tm.GetHour(),
				tm.GetMinute(),
				tm.GetSecond()
				);

	switch(nOption)
	{
	case DT_DATETIME:
		strData = strValor;
		break;
	case DT_DATE:
		strData = strValor.Left(10);
		break;
	case DT_TIME:
		strData = strValor.Right(8);
		break;
	default:
		strData = strValor;
		break;
	}
}

void DToC(const DATE& dtData, CString &strData, int nOption)
{
	CString strValor;
	COleDateTime tm(dtData);

	if(dtData == 0)
	{
		strData = _T(" / / ");
		return;
	}
	
	strValor.Format(	_T("%02d/%02d/%04d %02d:%02d:%02d"), 
				tm.GetDay(),
				tm.GetMonth(),
				tm.GetYear(),
				tm.GetHour(),
				tm.GetMinute(),
				tm.GetSecond()
				);

	switch(nOption)
	{
	case DT_DATETIME:
		strData = strValor;
		break;
	case DT_DATE:
		strData = strValor.Left(10);
		break;
	case DT_TIME:
		strData = strValor.Right(8);
		break;
	default:
		strData = strValor;
		break;
	}
}

/****************************************************************************************************************
	DToCR
	Converte a data informada para caracter de acordo com o Regional Settings
	Author:	Anderson Takemitsu Kubota
	Date:	28.11.2001
	LastUp:	
	Parameters:
		DATE dtDate		- Data a ser convertida
		CString strDate	- Retorno da string
****************************************************************************************************************/
void DToCR(const DATE& dtDate, CString& strDate)
{
	COleDateTime dtAux;
	dtAux.m_dt = dtDate;
	TCHAR strAux[50];

	SYSTEMTIME stDate;
	dtAux.GetAsSystemTime(stDate);

	GetDateFormat(NULL,NULL,&stDate,NULL,strAux,50);

	strDate = strAux;

	return;
}

/****************************************************************************************************************
	DToCR
	Converte a data informada para caracter de acordo com o Regional Settings
	Author:	Anderson Takemitsu Kubota
	Date:	28.11.2001
	LastUp:	
	Parameters:
		CTime dtDate		- Data a ser convertida
		CString strDate	- Retorno da string
****************************************************************************************************************/
void DToCR(const CTime& dtDate, CString& strDate)
{
	TCHAR strAux[50];

	SYSTEMTIME stDate;
	dtDate.GetAsSystemTime(stDate);

	GetDateFormat(NULL,NULL,&stDate,NULL,strAux,50);

	strDate = strAux;

	return;
}

/****************************************************************************************************************
	DeleteFiles
	Deleta os arquivos com a máscara informada de um diretório especificado
	Author:	Anderson Takemitsu Kubota
	Date:	18.12.2001
	LastUp:	
	Parameters:
		CString strFileName - máscara do arquivo a ser excluido
		CString strDirectory - diretório dos arquivos a serem excluidos
****************************************************************************************************************/
int DeleteFiles(CString strFileName, CString strDirectory, BOOL bSubDirectories)
{
	if(strFileName.IsEmpty() || strDirectory.IsEmpty())
	{
		AfxMessageBox(_T("Erro na chamada da função DeleteFiles. É necessário informar a máscara do arquivo e o diretório!"), MB_ICONWARNING);
		return -1;
	}

	int nDeleted = 0;
	WIN32_FIND_DATA fd;
	HANDLE	hFindFile	= INVALID_HANDLE_VALUE;
	CString strFile;

	AddBS(strDirectory);
	strFile	= strDirectory;
	strFile	+= strFileName;

	hFindFile = FindFirstFile(strFile, &fd);
	while(INVALID_HANDLE_VALUE != hFindFile)
	{
		if((fd.cFileName[0] == '.') || (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if(bSubDirectories && fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				CString strAux;
				strAux = strDirectory;
				strAux += fd.cFileName;
				DeleteFiles(strFileName,strAux,bSubDirectories);
				RemoveDirectory(strAux);
			}

			if(FindNextFile(hFindFile, &fd))
				continue;
			else
				break;
		}

		strFile	= strDirectory;
		strFile	+= fd.cFileName;

		DeleteFile(strFile);
		nDeleted++;

		if(FindNextFile(hFindFile, &fd))
			continue;
		else
			break;
	}

	FindClose(hFindFile);

	return nDeleted;
}

/****************************************************************************************************************
	MoveFiles
	Move os arquivos com a máscara informada de um diretório especificado
	Author:	Anderson Takemitsu Kubota
	Date:	25.12.2001
	LastUp:	
	Parameters:
		CString strFileName - máscara dos arquivos a serem movidos
		CString strDirectoryFrom - diretório origem dos arquivos a serem movidos
		CString strDirectoryTo - diretório destino dos arquivos a serem movidos
****************************************************************************************************************/
int MoveFiles(CString strFileName, CString strDirectoryFrom, CString strDirectoryTo)
{
	if(strFileName.IsEmpty() || strDirectoryFrom.IsEmpty() || strDirectoryTo.IsEmpty())
	{
		AfxMessageBox(_T("Erro na chamada da função MoveFiles. É necessário informar a máscara do arquivo e o diretório origem e destino!"), MB_ICONWARNING);
		return -1;
	}

	int nMoved = 0;
	WIN32_FIND_DATA fd;
	HANDLE	hFindFile	= INVALID_HANDLE_VALUE;
	CString strFile,strFileDestino;

	AddBS(strDirectoryFrom);
	AddBS(strDirectoryTo);

	strFile	= strDirectoryFrom;
	strFile	+= strFileName;

	hFindFile = FindFirstFile(strFile, &fd);
	while(INVALID_HANDLE_VALUE != hFindFile)
	{
		if((fd.cFileName[0] == '.') || (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if(FindNextFile(hFindFile, &fd))
				continue;
			else
				break;
		}

		strFile	= strDirectoryFrom;
		strFile	+= fd.cFileName;

		strFileDestino = strDirectoryTo;
		strFileDestino += fd.cFileName;

		if(ExistFile(strFileDestino))
			DeleteFile(strFileDestino);

		MoveFile(strFile,strFileDestino);
		nMoved++;

		if(FindNextFile(hFindFile, &fd))
			continue;
		else
			break;
	}

	FindClose(hFindFile);

	return nMoved;
}


/****************************************************************************************************************
	CopyFiles
	Copia os arquivos com a máscara informada de um diretório especificado
	Author:	Anderson Takemitsu Kubota
	Date:	22.02.2002
	LastUp:	
	Parameters:
		CString strFileName - máscara dos arquivos a serem copiados
		CString strDirectoryFrom - diretório origem dos arquivos a serem copiados
		CString strDirectoryTo - diretório destino dos arquivos a serem copiados
****************************************************************************************************************/
int CopyFiles(CString strFileName, CString strDirectoryFrom, CString strDirectoryTo)
{
	if(strFileName.IsEmpty() || strDirectoryFrom.IsEmpty() || strDirectoryTo.IsEmpty())
	{
		AfxMessageBox(_T("Erro na chamada da função CopyFiles. É necessário informar a máscara do arquivo e o diretório origem e destino!"), MB_ICONWARNING);
		return -1;
	}

	int nCopied = 0;
	WIN32_FIND_DATA fd;
	HANDLE	hFindFile	= INVALID_HANDLE_VALUE;
	CString strFile,strFileDestino;

	AddBS(strDirectoryFrom);
	AddBS(strDirectoryTo);

	strFile	= strDirectoryFrom;
	strFile	+= strFileName;

	hFindFile = FindFirstFile(strFile, &fd);
	while(INVALID_HANDLE_VALUE != hFindFile)
	{
		if((fd.cFileName[0] == '.') || (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if(FindNextFile(hFindFile, &fd))
				continue;
			else
				break;
		}

		strFile	= strDirectoryFrom;
		strFile	+= fd.cFileName;

		strFileDestino = strDirectoryTo;
		strFileDestino += fd.cFileName;

		CopyFile(strFile,strFileDestino,FALSE);

		nCopied++;

		if(FindNextFile(hFindFile, &fd))
			continue;
		else
			break;
	}

	FindClose(hFindFile);

	return nCopied;
}


/****************************************************************************************************************
	ExistFile
	Verifica se o arquivo especificado existe
	Author:	Anderson Takemitsu Kubota
	Date:	25.12.2001
	LastUp:	
	Parameters:
		CString strFileName - caminho completo do arquivo
****************************************************************************************************************/
BOOL ExistFile(const CString& strFileName)
{
	HANDLE	hFindFile	= INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA	fd;

	hFindFile = FindFirstFile(strFileName, &fd);

	if(hFindFile != INVALID_HANDLE_VALUE)
	{
		// Arquivo encontrado
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	FindClose(hFindFile);
}



/****************************************************************************************************************
	WACreateDirectory
	Cria um diretório
	Author:	Anderson Takemitsu Kubota
	Date:	27.12.2001
	LastUp:	
	Parameters:
		CString strDirectory - caminho completo do diretório
****************************************************************************************************************/
BOOL WACreateDirectory(const CString& strDirectory)
{
	TCHAR szDir[MAX_PATH];
	TCHAR szPath[MAX_PATH];
	int nPos = 1;
	BOOL bEnd = FALSE;

	_tcscpy(szPath,strDirectory);

	szDir[0] = _T('\\');

	while(!bEnd)
	{
		if(szPath[nPos] == _T('\0'))
			bEnd = true;

		if(szPath[nPos] == _T('\\'))
		{
			szDir[nPos] = _T('\0');
			if(!CreateDirectory(szDir, NULL))
				if(::GetLastError() != ERROR_ALREADY_EXISTS)
					return FALSE;
		}

		szDir[nPos] = szPath[nPos];
		nPos++;
	}

	return TRUE;
}

/****************************************************************************************************************
	Rotina para arredondamento de valores
****************************************************************************************************************/
double round(const double dVal, const int nDecs)
{
	long lPow = 1;
	for (int nIndex = 0; nIndex < nDecs; nIndex++)
		lPow *=10;

	double dMultValue = (dVal * lPow) + 0.5;
	__int64 nVal = (__int64) dMultValue ;
	double dResult = (double) nVal / lPow;
	return dResult;
}


BOOL Yes(const CString& strMessage)
{
	if(AfxMessageBox(strMessage,MB_ICONQUESTION | MB_YESNO) == IDYES)
		return TRUE;
	else
		return FALSE;
}


#ifdef _WIN32_WCE
int LoadHtml(CString strFile)
{
	CFile file;
	PROCESS_INFORMATION pProcInfo;

	file.m_hFile = CFile::hFileNull;

	if ( file.Open(strFile, CFile::modeRead ,NULL) )
	{
		if ( file.m_hFile != CFile::hFileNull ) 
			file.Close();

		if ( !CreateProcess(TEXT("PegHelp.exe"),(LPCTSTR)strFile,NULL,NULL,FALSE,0,NULL,NULL,NULL,&pProcInfo) )
		{
			strFile.Format(_T("Erro ao visualizar o arquivo: "), strFile);
			Warn(strFile);
		}
		else
		{
			WaitForSingleObject(pProcInfo.hProcess, INFINITE);
		}
	}
	else
	{
		strFile.Format(_T("Erro ao visualizar o arquivo: %s"), strFile);
		Warn(strFile);
	}

	if ( file.m_hFile != CFile::hFileNull )
		file.Close();

	return 0;
}



BOOL InfraVermelhoSend(CString strFile)
{
	PROCESS_INFORMATION pi;
	DWORD   exitCode=(DWORD)0L;

	// Infrared
	if(!CreateProcess(_T("\\Windows\\irsquirt.exe"),
		strFile,NULL,NULL,
		FALSE,0,NULL,NULL,NULL,&pi))
	{
		Warn(_T("Não foi possível iniciar a transferência por infra-vermelho!"));
		return FALSE;
	}

	//Give time to the process.
	WaitForSingleObject(pi.hProcess,INFINITE);
	GetExitCodeProcess(pi.hProcess,&exitCode);

	CloseHandle(pi.hProcess);

	if(exitCode)
	{
		CString csMessage;
		csMessage.Format(_T("Infrared Error/Warning: %ld"),exitCode);
		Warn(csMessage);
		return FALSE;
	}
	else
		return TRUE;
}


BOOL InfraVermelhoReceive()
{
	PROCESS_INFORMATION pi;
	DWORD   exitCode=(DWORD)0L;

	// Infrared
	if(!CreateProcess(_T("\\Windows\\irsquirt.exe"),
		_T(""),NULL,NULL,
		FALSE,0,NULL,NULL,NULL,&pi))
	{
		Warn(_T("Não foi possível iniciar a recepção por infra-vermelho!"));
		return FALSE;
	}

	//Give time to the process.
	WaitForSingleObject(pi.hProcess,INFINITE);
	GetExitCodeProcess(pi.hProcess,&exitCode);

	CloseHandle(pi.hProcess);

	if(exitCode)
	{
		CString csMessage;
		csMessage.Format(_T("Infrared Error/Warning: %ld"),exitCode);
		Warn(csMessage);
		return FALSE;
	}
	else
		return TRUE;
}

#endif


BOOL SpecialYes(const CString& strMessage,int nCount)
{
	for(int i = 0; i < nCount; i++)
	{
		if(!Yes(strMessage))
			return FALSE;
	}
	return TRUE;
}


/****************************************************************************************************************
	AddBs
	Adds a backslash (if needed) to a path expression.
	Author:	Anderson Takemitsu Kubota
	Date:	26.02.2002
	LastUp:	
	Parameters:
		CString strPath - Specifies the path name to which to add the backslash.
****************************************************************************************************************/
BOOL AddBS(CString& strPath)
{
	if(strPath.IsEmpty() || strPath.Right(1) != _T("\\"))
	{
		strPath += _T("\\");
		return TRUE;
	}
	else
		return FALSE;
}

/****************************************************************************************************************
	Rotina Hora do Campo Data
****************************************************************************************************************/

void DataSemHora(COleDateTime dtAux1, COleDateTime &dtAux2)
{
	COleDateTime dtAux3( dtAux1.GetYear() , dtAux1.GetMonth(), dtAux1.GetDay(), 0, 0, 0 ); 

	dtAux2 = dtAux3;
}

/****************************************************************************************************************
	Rotina para ordenar um Grid
****************************************************************************************************************/
int CALLBACK CompareFunctionList(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	SortListInfo * si = (SortListInfo*) lParamSort;

	int nIndex1 = (int) lParam1;
	int nIndex2 = (int) lParam2;

	CString str1, str2;
	double d1, d2;
	CTime tm1, tm2;
	int nReturn;
	LVITEM Item;
	
	switch(si->nColumnDataType)
	{
	case DT_NUMBER:
		Val(si->pctrlList->GetItemText(nIndex1, si->nColumn), d1);
		Val(si->pctrlList->GetItemText(nIndex2, si->nColumn), d2);

		if(d1 == d2)
			nReturn = 0;
		else if(d1 < d2)
			nReturn = -1;
		else
			nReturn = 1;
	break;

	case DT_TIME:
	case DT_DATE:
	case DT_DATETIME:
		// ATK 22.11.01 Verifica se os valores não estão em branco
		if(si->pctrlList->GetItemText(nIndex1, si->nColumn).IsEmpty())
			tm1 = 0;
		else
			CToD(si->pctrlList->GetItemText(nIndex1, si->nColumn), tm1, si->nColumnDataType);

		if(si->pctrlList->GetItemText(nIndex2, si->nColumn).IsEmpty())
			tm2 = 0;
		else
			CToD(si->pctrlList->GetItemText(nIndex2, si->nColumn), tm2, si->nColumnDataType);

		if(tm1 == tm2)
			nReturn = 0;
		else if(tm1 < tm2)
			nReturn = -1;
		else
			nReturn = 1;
	break;
	case DT_IMAGE:
		Item.mask = LVIF_IMAGE;
		Item.iItem = nIndex1;
		Item.iSubItem = si->nColumn;
		si->pctrlList->GetItem(&Item);
		nIndex1 = Item.iImage;

		Item.mask = LVIF_IMAGE;
		Item.iItem = nIndex2;
		Item.iSubItem = si->nColumn;
		si->pctrlList->GetItem(&Item);
		nIndex2 = Item.iImage;

		if(nIndex1 == nIndex2)
			nReturn = 0;
		else if(nIndex1 < nIndex2)
			nReturn = -1;
		else
			nReturn = 1;
	break;

	case DT_STRING:
	default:
		str1 = si->pctrlList->GetItemText(nIndex1, si->nColumn);
		str2 = si->pctrlList->GetItemText(nIndex2, si->nColumn);

		nReturn = str1.CompareNoCase(str2);
	break;
	}

	return (nReturn * si->nSortType);
}

/****************************************************************************************************************
* SearchKey
****************************************************************************************************************/
CString SearchKey(CString strText, CString strKey, CString strDelimiter)
{
	CString strRet;
	int		nStart, nEnd;

	strRet.Empty();

	nStart = strText.Find(strKey);

	if (nStart < 0) 
		return strRet;

	nStart = strText.Find(strDelimiter, nStart);
	nEnd   = strText.Find(strDelimiter, nStart + 1);

	if (nStart < nEnd)
		strRet = strText.Mid(nStart+1, nEnd - nStart - 1);

	return strRet;
}

BOOL TestObject(TCHAR * pszObjeto)
{

#define E_INVALID_PROGID				0x00000FA2
#define E_INSTANCE_NOT_CREATED			0x00000FA3
#define E_OBJECT_NOT_CREATED			0x00000FA1

	LPOLESTR	pszProgID;
	UINT		nError;
    CLSID		clsid;					// CLSID of ActiveX object.    
	LPUNKNOWN	punk;					// Ponteiro para a variável IUnknown.    
	LPDISPATCH	pdisp;				// Ponteiro para a variável IDispatch.
	HRESULT		hr;
	BOOL		bCreate = FALSE;

#ifndef UNICODE
	AnsiToUnicode(pszObjeto, &pszProgID);
//	strcpy(m_szCOMName, pszObjeto);
#else
	pszProgID = pszObjeto; 
//	_tcscpy(m_szCOMName, pszObjeto);
#endif
    
    // Retrieve CLSID from the ProgID that the user specified.
	__try
	{
		hr = CLSIDFromProgID(pszProgID, &clsid);
		if(FAILED(hr))
		{
			nError = E_INVALID_PROGID;
			goto error;
		}

		hr = CoCreateInstance(clsid, NULL, CLSCTX_SERVER, IID_IUnknown, (void **)&punk);
		if(FAILED(hr))
		{
			nError = E_INSTANCE_NOT_CREATED;
			goto error;
		}

		// Ask the ActiveX object for the IDispatch interface.
		hr = punk->QueryInterface(IID_IDispatch, (void **)&pdisp);
		if(FAILED(hr))
		{
			nError = E_OBJECT_NOT_CREATED;
			goto error;
		}

		bCreate = TRUE;

	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		nError = E_OBJECT_NOT_CREATED;
		goto error;
	}

	punk->Release();
	punk = NULL;

	pdisp->Release();
	pdisp = NULL;

    return TRUE;
    
error:

	if( bCreate )
	{
		punk->Release();
		punk = NULL;

		pdisp->Release();
		pdisp = NULL;
	}

    return FALSE;
}


BOOL IsRegistered(LPCTSTR pszProgID)
{
	BOOL		bRet = FALSE;
	
	CLSID clsid;
	HRESULT hr = AfxGetClassIDFromString(pszProgID, &clsid);
	if(SUCCEEDED(hr))
	{
		TCHAR	szObjeto[256];
		bRet = TRUE;

		_tcscpy(szObjeto,pszProgID);
		if(! TestObject(szObjeto) )
			bRet = FALSE;

	}

	return bRet;
}

void Register(LPCTSTR pszFileName)
{

	HINSTANCE hOCX = NULL;
	TCHAR szPrompt[500];

	hOCX = LoadLibrary( pszFileName );

#ifdef _UNICODE
	#ifdef _WIN32_WCE
		if(FAILED((*(GetProcAddress(hOCX, _T("DllRegisterServer"))))()))
		{

			DWORD dwError = GetLastError();
			_tcscpy(szPrompt, _T("DLLRegisterServer() Falhou.\nErro:"));
			MessageBox(NULL, szPrompt, _T("ERRO"), MB_OK );
		}
	#else
		if(FAILED((*(GetProcAddress(hOCX, "DllRegisterServer")))()))
		{

			DWORD dwError = GetLastError();
			_tcscpy(szPrompt, _T("DLLRegisterServer() Falhou.\nErro:"));
			MessageBox(NULL, szPrompt, _T("ERRO"), MB_OK );
		}
	#endif
#else
	if(FAILED((*(GetProcAddress(hOCX, "DllRegisterServer")))()))
	{
		DWORD dwError = GetLastError();

		strcpy(szPrompt,"DLLRegisterServer() Falhou.\nErro:");
		MessageBox(NULL, szPrompt, _T("ERRO"), MB_OK );
	}
#endif

	FreeLibrary(hOCX);

}


#ifndef UNICODE
HRESULT __fastcall AnsiToUnicode(TCHAR *pszA, LPOLESTR* ppszW)
{

    ULONG cCharacters;
    DWORD dwError;

    // If input is null then just return the same.
    if (NULL == pszA)
    {
        *ppszW = NULL;
        return NOERROR;
    }

    // Determine number of wide characters to be allocated for the
    // Unicode string.
    cCharacters =  strlen(pszA)+1;

    // Use of the OLE allocator is required if the resultant Unicode
    // string will be passed to another COM component and if that
    // component will free it. Otherwise you can use your own allocator.
    *ppszW = (LPOLESTR) CoTaskMemAlloc(cCharacters*2);
    if (NULL == *ppszW)
        return E_OUTOFMEMORY;

    // Covert to Unicode.
    if (0 == MultiByteToWideChar(CP_ACP, 0, pszA, cCharacters,
                  *ppszW, cCharacters))
    {
        dwError = GetLastError();
        CoTaskMemFree(*ppszW);
        *ppszW = NULL;
        return HRESULT_FROM_WIN32(dwError);
    }

    return NOERROR;
/*
 * UnicodeToAnsi converts the Unicode string pszW to an ANSI string
 * and returns the ANSI string through ppszA. Space for the
 * the converted string is allocated by UnicodeToAnsi.
 */ 

}
#endif

#if _WINDOWS
 /****************************************************************************
 *                                                                           *
 * DoEvents                                                                  *
 * Libera mensagens pendentes                                                *
 *                                                                           *
 *****************************************************************************/
void DoEvents()
{
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

#endif