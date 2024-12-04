/*****************************************************************************
 * Arquivo: WFread.h                                                         *
 * Define classe para entrada e saida em arquivo.                            *
 * Data:24/09/97                                                             *
 * Auto: José Sahle Netto                                                    *
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
		CFread(const CFread &file_arquivo); //Construtor de Cópia

		// Destrutor
		~CFread();
		
		/* Definições das funções membros da classe */
		// AAA
		// Função usada para abrir um arquivo.
		void Open(const CString file_arquivo, TCHAR w_tipo = 't', int w_maxlin = 0); 

		// Função que cria e abre um arquivo.
		void Create(const CString file_arquivo, TCHAR w_tipo, int w_maxlin);

		// Função usada para ler uma linha.
		void Read(void);
		
		// Função usada para escrever em um arquivo.
		void Write(void);
		
		// Função usada para fechar um arquivo.
		void Close(void);
		
		// Função para verificar se a última operacao foi bem sucedida.
		BOOL IsSuccessfull(void);

		// Função que retorna verdadeiro se o arquivo foi aberto e false se não.
		BOOL IsOpen(void);
		
		// Função para posicionar no inicio do arquivo.
		//void first(void);
		
		// Função para posicionar no final do arquivo.
		//void last(void);
		
		// Função para posicionar na próxima linha do arquivo.
		//void next(void);
		
		// Função para posicionar na linha anterior do arquivo.
		//void previous(void);
		
		// Função que retorna True se o final do arquivo foi encontrado.
		BOOL Eof(void);
		
		// Função que retorna True se o inicio do arquivo foi encontrado.
		//BOOL bof(void);
		
		//Função que retorna o valor do erro da ultima operação.
		int LastError(void);

		//Função que retorna o número máximo de caracteres para leitura.
		long MaxLineLength(void);
		
		//Função que retorna o conteúdo da linha lida.
		CString GetContent(void);
		TCHAR    GetContent(int ponteiro);
		
		//Função que preenche o buffer de linha com o valor do parâmetro.
		void SetContent(const CString& w_linha);
		void SetContent(const TCHAR& w_linha);
		
		//Função usada para truncar o arquivo, ou seja, deixá-lo com tamanho zero.
		void Trunc(void);
		
		//Função que devolve o tamanho da linha.
		int GetLength(void);

		//Função que retorna o tamanho do arquivo em bytes.
		double GetFileLength(void);

		//Função que zera o conteúdo da linha.
		void ResetLine(void);
		
		//Função que substitui o primeiro elemento pelo segundo.
		void Replace(const CString& exp1, const CString& exp2);
		
	protected:
		
		/* Variáveis protegidas para configurar o objeto CFread. */
		TCHAR *linha;          // Conteúdo da última linha lida.
		long tamanho;          // Tamanho da linha;
		int maximo;            // Usado para não ser necessário criar muitas vezes o ponteiro.
		BOOL _issuccessfull;   // Variável que retorna qual o status da última operacao.
		HANDLE file_pointer;   // Ponteiro do arquivo.
		TCHAR tipo;            // Tipo da leitura. Pode ser "b" binária ou "t"texto.
		long maxlin;           // Tamanho máximo de uma linha.
		CString nomedoarquivo; // Aguarda o nome do arquivo;
		long erro;			   // Retorna o erro da ultima operacão;
		BOOL _open;            // Verifica se ha um arquivo aberto.
		BOOL _eof;             // Verifica se é fim de arquivo
};  

#endif
