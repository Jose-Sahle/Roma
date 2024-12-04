/*****************************************************************************
 * Arquivo: WFread.cpp                                                       *
 * Define classe para entrada e saida em arquivo.                            *
 * Data:24/09/97                                                             *
 *****************************************************************************/

#include "wfread.h"

/*****************************************************************************
 * F r e a d                                                                 *
 *                                                                           *
 * Cria um objeto CFread.                                                    *
 *****************************************************************************/
CFread::CFread(const CString file_arquivo, TCHAR w_tipo = _T('t'), int w_maxlin = 0)
{   
	tipo = w_tipo;
	maxlin = w_maxlin;
	tamanho = 0;
	maximo = maxtam;
	linha = new TCHAR[maxtam];
	nomedoarquivo = file_arquivo;
	_open = FALSE;
	_eof  = TRUE;

	file_pointer = CreateFile(file_arquivo, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if ((file_pointer != INVALID_HANDLE_VALUE))
	{
		_open          = TRUE;
		_eof           = FALSE;
		_issuccessfull = TRUE;
	}
	else
		_issuccessfull = FALSE;

	erro  = GetLastError();	
}
	
/*****************************************************************************
 * F r e a d                                                                 *
 *                                                                           *
 * Cria um objeto CFread.                                                    *
 *****************************************************************************/
CFread::CFread(void) 
{   
	tipo          = _T('t');
	maxlin        = maxtam;
	tamanho       = 0;
	maximo        = maxtam;
	linha         = new TCHAR[maxtam];
	nomedoarquivo = _T(" ");
	_issuccessfull   = TRUE;
	_open = FALSE;
}

/*****************************************************************************
 * ~ F r e a d                                                               *
 *                                                                           *
 * Destroi um objeto CFread.                                                 *
 *****************************************************************************/
CFread::~CFread(void) 
{ 
	if (_open)
	{
		if (CloseHandle(file_pointer))
		{
			_open          = FALSE;
			_eof           = TRUE;
			_issuccessfull = TRUE;
		}
		else
			_issuccessfull = FALSE;

		erro  = GetLastError();
	}
	delete linha; 
}

/*****************************************************************************
 * F r e a d  de Cópia                                                       *
 *                                                                           *
 * Cria um objeto CFread de Cópia.                                           *
 *****************************************************************************/
CFread::CFread(const CFread &file_arquivo)
{   
	tipo             = file_arquivo.tipo;
	maxlin           = file_arquivo.maxlin;
	linha            = file_arquivo.linha;    
	_issuccessfull   = file_arquivo._issuccessfull;
	file_pointer     = file_arquivo.file_pointer;
	nomedoarquivo    = file_arquivo.nomedoarquivo;
	erro             = file_arquivo.erro;
	tamanho          = file_arquivo.tamanho;
	maximo           = file_arquivo.maximo;
	_open            = file_arquivo._open;
	_eof             = file_arquivo._eof;
	linha            = new TCHAR[maximo];

	for (int cont = 0 ; cont <= sizeof(linha); cont++)
	{
		linha[cont] = file_arquivo.linha[cont];
	}
}
	
/*****************************************************************************
 * O p e n                                                                   *
 *                                                                           *
 * Abre um arquivo, especificado em "arquivo". A variável "operacao" especi- *
 * fica como o arquivo será aberto.                                          *
 *****************************************************************************/
void CFread::Open(const CString file_arquivo, TCHAR w_tipo, int w_maxlin)
{   
	tipo = w_tipo;

	if (_open == TRUE)
	{
		erro = 200013;
		_issuccessfull = FALSE;
		return;
	}

	if (tipo == _T('t'))
		maxlin = 0;
	else
		maxlin = w_maxlin;

	nomedoarquivo = file_arquivo;

	file_pointer = CreateFile(file_arquivo, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if ((file_pointer != INVALID_HANDLE_VALUE))
	{
		_open          = TRUE;
		_eof           = FALSE;
		_issuccessfull = TRUE;
	}
	else
		_issuccessfull = FALSE;

	erro  = GetLastError();	
}		

/*****************************************************************************
 * C r e a t e                                                               *
 *                                                                           *
 * Cria um arquivo, especificado em "arquivo".                               *
 *****************************************************************************/
void CFread::Create(const CString file_arquivo, TCHAR w_tipo = _T('t'), int w_maxlin = 0)
{   
	tipo = w_tipo;

	if (tipo == _T('t'))
		maxlin = 0;
	else
		maxlin = w_maxlin;

	tamanho = 0;
	maximo = maxtam;
	delete linha;
	linha = new TCHAR[maxtam];
	nomedoarquivo = file_arquivo;

	file_pointer = CreateFile(file_arquivo, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if ((file_pointer != INVALID_HANDLE_VALUE))
	{
		_open          = TRUE;
		_eof           = FALSE;
		_issuccessfull = TRUE;
	}
	else
	{
		_issuccessfull = FALSE;
		_open          = FALSE;
		_eof           = FALSE;
	}

	erro  = GetLastError();	
}		

/*****************************************************************************
 * C l o s e                                                                 *
 *                                                                           *
 * Fecha um arquivo, previamente aberto.                                     *
 *****************************************************************************/
void CFread::Close(void)
{
	if (CloseHandle(file_pointer))
	{
		_open          = FALSE;
		_eof           = TRUE;
		_issuccessfull = TRUE;
	}
	else
		_issuccessfull = FALSE;

	erro  = GetLastError();
}

/*****************************************************************************
 * R e a d                                                                   *
 *                                                                           *
 * Lê uma linha do arquivo aberto.                                           *
 *****************************************************************************/
void CFread::Read(void)
{                      
	int _le_grava;
	unsigned long _byteslidos;
	int posicao;
    
	if (_open == FALSE)
		return;

	if (_eof == TRUE)
		return;

    posicao = 0;
	delete linha;
	if (tipo == _T('b'))             
	{
		maximo = maxlin;
		tamanho = 0;
	}
	else
	{
		tamanho = 0;             
		maximo = maxtam;
	}

	linha = new TCHAR[maximo];
	while(TRUE)
	{	
		char cBuffer;

		if ((maxlin != 0) && (tamanho >= maxlin) && (tipo == _T('b')))
			break;
		
		ReadFile(file_pointer, &cBuffer, sizeof(char), &_byteslidos, NULL);

		if (_byteslidos == 0)
		{
			_eof = TRUE;
			break;
		}
		_le_grava = cBuffer;
		
		//Se o caracter lido for 13 (Enter) e o tipo de arquivo for binário, o lê
		if (((TCHAR) _le_grava == _T('\n')) && tipo == _T('b'))
			goto Maisum;

		//Se o caracter lido for 10 (Carriage-return) e o tipo de arquivo for binário, o lê
		if (((TCHAR) _le_grava == _T('\r')) && tipo == _T('b'))
			goto Maisum;
		
		//Se o caracter lido for 13 (Enter) e o tipo de arquivo for texto, interrompe a leitura
		if (((TCHAR) _le_grava == _T('\n')) && tipo == _T('t'))
			break;

		//Se o caracter lido for 10 (Carriage-return) e o tipo de arquivo for texto, interromp a leitura
		if (((TCHAR) _le_grava == _T('\r')) && tipo == _T('t'))
		{
			ReadFile(file_pointer, &cBuffer, sizeof(char), &_byteslidos, NULL);
			if (_byteslidos == 0)
			{
				_eof = TRUE;
				break;
			}
			_le_grava = cBuffer;
			break;
		}
		
		Maisum:
		tamanho++;
		
		//Verifica se a matriz-string já não comporta o novo caracter
		//Se não comporta mais então aumenta o tamanho da matriz-string
		if (tamanho > maximo)
		{               
			int w_maximo;
			TCHAR *w_linha;
			w_maximo = maximo;
			w_linha = new TCHAR[maximo];
			Copychar(linha,w_linha,maximo);
			delete linha;
			maximo = maxtam*(tamanho / maxtam + 1);
			linha = new TCHAR[maximo];
			Copychar(w_linha,linha,w_maximo);
			delete w_linha;
		}

		//Inclui o caracter lido na Linha
		posicao = tamanho - 1;
		linha[posicao] = (TCHAR) _le_grava;
	}
}

/*****************************************************************************
 * W r i t e                                                                 *
 *                                                                           *
 * Função usada para escrever uma linha num arquivo.                         *
 *****************************************************************************/
void CFread::Write(void)
{
	if (_open == FALSE)
		return;

	for(int cont = 0; cont < tamanho; cont++)
	{ 
		TCHAR ch;
		DWORD nTotal;

		ch = linha[cont];
		WriteFile(file_pointer, &ch, sizeof(TCHAR), &nTotal, NULL);
	}
	
	if (tipo == _T('t'))
	{	
		TCHAR ch;
		DWORD nTotal;

		ch = _T('\r');
		WriteFile(file_pointer, &ch, sizeof(TCHAR), &nTotal, NULL);
		ch = _T('\n');
		WriteFile(file_pointer, &ch, sizeof(TCHAR), &nTotal, NULL);
	}
}

/*****************************************************************************
 * E o f                                                                     *
 *                                                                           *
 * Função que retorna TRUE se o inicio do arquivo foi encontrado.            *
 *****************************************************************************/
BOOL CFread::Eof(void)
{
	if (_open == FALSE)
		return FALSE;

    return (_eof);
}

/*****************************************************************************
 * M a x L i n e L e n g t h                                                 *
 *                                                                           *
 * Retorna o tamanho máximo da linha.                                        *
 *****************************************************************************/
long CFread::MaxLineLength(void)
{
	if (_open == FALSE)
		return 0;
	return (maxlin);
}

/*****************************************************************************
 * E r r o r                                                                 *
 *                                                                           *
 * Função que retorna o valor do erro da ultima operação.                    *
 *****************************************************************************/
int CFread::LastError(void)
{
    return (erro);
}

/*****************************************************************************
 * I s O p e n                                                               *
 *                                                                           *
 * Retorna se existem um arquivo aberto.                                     *
 *****************************************************************************/
BOOL CFread::IsOpen(void)
{
	return (_open);
}

/*****************************************************************************
 * I s S u c e f u l l                                                       *
 *                                                                           *
 * Função para verificar se a última operacao foi bem sucedida.              *
 *****************************************************************************/
BOOL CFread::IsSuccessfull(void)
{
	return (_issuccessfull);
}


/*****************************************************************************
 * C o n t e n t                                                             *
 *                                                                           *
 * Função que retorna o conteúdo da linha lida.                              *
 *****************************************************************************/
CString CFread::GetContent(void)
{
	CString w_linha;
	
	if (_open == FALSE)
		return _T("");

	w_linha = _T("");
	
	for (int cont = 0 ; cont < tamanho; cont++)
	{
		w_linha = w_linha + (TCHAR) linha[cont];
	}
	
    return (w_linha);
}

/*****************************************************************************
 * C o n t e n t                                                             *
 *                                                                           *
 * Função que retorna o conteúdo da linha lida.                              *
 *****************************************************************************/
TCHAR CFread::GetContent(int ponteiro)
{
	if (_open == FALSE)
		return (TCHAR) _T("");

    return (linha[ponteiro]);
}
/*****************************************************************************
 * P u t c o n t e n t                                                       *
 *                                                                           *
 * Função que preenche o buffer de linha com o valor do parâmetro.           *
 *****************************************************************************/
void CFread::SetContent(const CString& w_linha)
{
	if (_open == FALSE)
		return;

	delete linha;
	tamanho = w_linha.GetLength();
	maximo = maxtam * (tamanho / maxtam + 1);
	linha = new TCHAR[maximo];
	for (int cont = 0; cont < w_linha.GetLength(); cont++)
	{
		linha[cont] = w_linha[cont];
	}
}

/*****************************************************************************
 * S e t C o n t e n t                                                       *
 *                                                                           *
 * Função que preenche o buffer de linha com o valor do parâmetro.           *
 *****************************************************************************/
void CFread::SetContent(const TCHAR& w_linha)
{
	int posicao,w_maximo;
	
	if (_open == FALSE)
		return;

	posicao = 0;
	tamanho++;
	if (tamanho > maximo)
	{
		TCHAR *t_linha;
		w_maximo = maximo;
		t_linha = new TCHAR[maximo];
		Copychar(linha,t_linha,maximo);
		delete linha;
		maximo = maxtam*(tamanho / maxtam + 1);
		linha = new TCHAR[maximo];
		Copychar(t_linha,linha,w_maximo);
		delete t_linha;
	}
	
	posicao = tamanho - 1;
	linha[posicao] = w_linha;
}

/*****************************************************************************
 * T r u n c                                                                 *
 *                                                                           *
 * Zera o tamanho do arquivo.                                                *
 *****************************************************************************/
void CFread::Trunc(void)
{
	if (_open == FALSE)
		return;
	
	if (CloseHandle(file_pointer) == NULL)
		_issuccessfull = TRUE;
	else
		_issuccessfull = FALSE;
		
	erro  = GetLastError();
	
	if ( _issuccessfull == TRUE)
 	{
		file_pointer = CreateFile(nomedoarquivo, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
		if ((file_pointer != INVALID_HANDLE_VALUE))
		{
			_open          = TRUE;
			_eof           = FALSE;
			_issuccessfull = TRUE;
		}
		else
			_issuccessfull = FALSE;
	
		erro  = GetLastError();	
	}
	
}
	
/*****************************************************************************
 * G e t L e n g t h                                                         *
 *                                                                           *
 * Função que devolve o tamanho da linha.                                    *
 *****************************************************************************/
int CFread::GetLength(void)
{
	return (tamanho);
}

/*****************************************************************************
 * G e t L e n g t h                                                         *
 *                                                                           *
 * Função que devolve o tamanho da linha.                                    *
 *****************************************************************************/
double CFread::GetFileLength(void)
{

	if ( !_open )
	{
		return 0;
	}
	else
	{
		DWORD fileSize;

		fileSize = GetFileSize(file_pointer, NULL);

		if (fileSize != INVALID_FILE_SIZE)
			return (fileSize);
		else
			return 0;
	}
}

/*****************************************************************************
 * R e s e t                                                                 *
 *                                                                           *
 * Função que zera o conteúdo da linha.                                      *
 *****************************************************************************/
void CFread::ResetLine(void)
{
	tamanho = 0;
	maximo = maxtam;
	delete linha;
	linha = new TCHAR[maximo];
}

/*****************************************************************************
 * R e p l a c e                                                             *
 *                                                                           *
 * Função que substitui o primeiro elemento pelo segundo.                    *
 *****************************************************************************/
void CFread::Replace(const CString& exp1, const CString& exp2)
{
	if (_open == FALSE)
		return;

	TCHAR *w_linha;
	int w_tamanho = 0;
	int w_maximo  = maximo;
	int posicao;
	int w_tam = 0;
	BOOL igual = FALSE;

	w_linha = new TCHAR[maximo];
	if (exp1.GetLength() > tamanho) return;

	posicao = -1;
	
	while(posicao + exp1.GetLength() <= tamanho)
	{   
		posicao++;
		w_tam = 0;
		igual = FALSE;
		
		for (int cont = 0; cont < exp1.GetLength() && posicao + exp1.GetLength() <= tamanho; cont++)
		{
			
			if(linha[posicao+cont] == exp1[cont])
			{
				if(igual == FALSE) igual = TRUE;
				w_tam++;
			}
			else
			{
				if(igual == FALSE)
				{
					int w_pos;
					posicao++;
					cont = -1;
					w_tamanho++;
					if (w_tamanho > w_maximo)
					{         
						int tw_maximo;
						TCHAR *t_linha;
						tw_maximo = w_maximo;
						t_linha = new TCHAR[w_maximo];
						Copychar(w_linha,t_linha,w_maximo);
						delete w_linha;
						w_maximo = maxtam*(w_tamanho / maxtam + 1);
						w_linha = new TCHAR[w_maximo];
						Copychar(t_linha,w_linha,tw_maximo);
						delete t_linha;
					}
					
					w_pos = w_tamanho - 1;
					w_linha[w_pos] = linha[posicao-1];
					continue;
				}
				break;
			}
		}
		
		if (w_tam != exp1.GetLength()) 
		{
			int w_pos;
			w_tamanho++;
			if (w_tamanho > w_maximo)
			{        
				int tw_maximo;
				TCHAR *t_linha; 
				tw_maximo = w_maximo;
				t_linha = new TCHAR[w_maximo];
				Copychar(w_linha,t_linha,w_maximo);
				delete w_linha;
				w_maximo = maxtam*(w_tamanho / maxtam + 1);
				w_linha = new TCHAR[w_maximo];
				Copychar(t_linha,w_linha,tw_maximo);
				delete t_linha;
			}
						
			w_pos = w_tamanho - 1;
			w_linha[w_pos] = linha[posicao];
		}
		if(w_tam == exp1.GetLength())
		{
			int w_pos;
			posicao = posicao + exp1.GetLength() - 1;
			for (int cont = 0; cont < exp2.GetLength(); cont++)
			{
				w_tamanho++;
				if (w_tamanho > w_maximo)
				{
					int tw_maximo;
					TCHAR *t_linha;
					tw_maximo = w_maximo;
					t_linha = new TCHAR[w_maximo];
					Copychar(w_linha,t_linha,w_maximo);
					delete w_linha;
					w_maximo = maxtam*(w_tamanho / maxtam + 1);
					w_linha = new TCHAR[w_maximo];
					Copychar(t_linha,w_linha,tw_maximo);
					delete t_linha;
				}
							
				w_pos = w_tamanho - 1;
				w_linha[w_pos] = exp2[cont];
			}
		}
			
	}		
	
	for (int cont = posicao+1; cont < tamanho; cont++)
	{
		int w_pos;
		w_pos = 0;
		w_tamanho++;
		if (w_tamanho > w_maximo)
		{    
			int tw_maximo;
			TCHAR *t_linha;
			tw_maximo = w_maximo;
			t_linha = new TCHAR[w_maximo];
			Copychar(w_linha,t_linha,w_maximo);
			delete w_linha;
			w_maximo = maxtam*(w_tamanho / maxtam + 1);
			w_linha = new TCHAR[w_maximo];
			Copychar(t_linha,w_linha,tw_maximo);
			delete t_linha;
		}
							
		w_pos = w_tamanho - 1;
		w_linha[w_pos] = linha[cont];
	}			
			
    if(w_tamanho > maximo)
    {
    	delete linha;
    	maximo = w_maximo;
    	linha = new TCHAR[maximo];
    }
    Copychar(w_linha,linha,maximo);
    tamanho = w_tamanho;
    
    delete w_linha;
}
