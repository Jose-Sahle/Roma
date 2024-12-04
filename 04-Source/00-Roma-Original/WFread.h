/*****************************************************************************
 * Arquivo: WFread.h                                                         *
 * Define classe para entrada e saida em arquivo.                            *
 * Data:24/09/97                                                             *
 * Auto: Jos� Sahle Netto                                                    *
 *****************************************************************************/

#if !defined(__CFread_H)

#define __CFread_H

#include <stdio.h>
#include "Library.h"

const maxtam = 8;

class CFread
{
	public:
		// Construtores
		CFread(void);
		CFread(const CString file_arquivo, TCHAR w_tipo, int w_maxlin);
		CFread(const CFread &file_arquivo); //Construtor de C�pia

		// Destrutor
		~CFread();
		
		/* Defini��es das fun��es membros da classe */
		// AAA
		// Fun��o usada para abrir um arquivo.
		void Open(const CString file_arquivo, TCHAR w_tipo = 't', int w_maxlin = 0); 

		// Fun��o que cria e abre um arquivo.
		void Create(const CString file_arquivo, TCHAR w_tipo, int w_maxlin);

		// Fun��o usada para ler uma linha.
		void Read(void);
		
		// Fun��o usada para escrever em um arquivo.
		void Write(void);
		
		// Fun��o usada para fechar um arquivo.
		void Close(void);
		
		// Fun��o para verificar se a �ltima operacao foi bem sucedida.
		BOOL IsSuccessfull(void);

		// Fun��o que retorna verdadeiro se o arquivo foi aberto e false se n�o.
		BOOL IsOpen(void);
		
		// Fun��o para posicionar no inicio do arquivo.
		//void first(void);
		
		// Fun��o para posicionar no final do arquivo.
		//void last(void);
		
		// Fun��o para posicionar na pr�xima linha do arquivo.
		//void next(void);
		
		// Fun��o para posicionar na linha anterior do arquivo.
		//void previous(void);
		
		// Fun��o que retorna True se o final do arquivo foi encontrado.
		BOOL Eof(void);
		
		// Fun��o que retorna True se o inicio do arquivo foi encontrado.
		//BOOL bof(void);
		
		//Fun��o que retorna o valor do erro da ultima opera��o.
		int LastError(void);

		//Fun��o que retorna o n�mero m�ximo de caracteres para leitura.
		long MaxLineLength(void);
		
		//Fun��o que retorna o conte�do da linha lida.
		CString GetContent(void);
		TCHAR    GetContent(int ponteiro);
		
		//Fun��o que preenche o buffer de linha com o valor do par�metro.
		void SetContent(const CString& w_linha);
		void SetContent(const TCHAR& w_linha);
		
		//Fun��o usada para truncar o arquivo, ou seja, deix�-lo com tamanho zero.
		void Trunc(void);
		
		//Fun��o que devolve o tamanho da linha.
		int GetLength(void);

		//Fun��o que retorna o tamanho do arquivo em bytes.
		double GetFileLength(void);

		//Fun��o que zera o conte�do da linha.
		void ResetLine(void);
		
		//Fun��o que substitui o primeiro elemento pelo segundo.
		void Replace(const CString& exp1, const CString& exp2);
		
	protected:
		
		/* Vari�veis protegidas para configurar o objeto CFread. */
		TCHAR *linha;          // Conte�do da �ltima linha lida.
		long tamanho;          // Tamanho da linha;
		int maximo;            // Usado para n�o ser necess�rio criar muitas vezes o ponteiro.
		BOOL _issuccessfull;   // Vari�vel que retorna qual o status da �ltima operacao.
		HANDLE file_pointer;   // Ponteiro do arquivo.
		TCHAR tipo;            // Tipo da leitura. Pode ser "b" bin�ria ou "t"texto.
		long maxlin;           // Tamanho m�ximo de uma linha.
		CString nomedoarquivo; // Aguarda o nome do arquivo;
		long erro;			   // Retorna o erro da ultima operac�o;
		BOOL _open;            // Verifica se ha um arquivo aberto.
		BOOL _eof;             // Verifica se � fim de arquivo
};  

#endif
