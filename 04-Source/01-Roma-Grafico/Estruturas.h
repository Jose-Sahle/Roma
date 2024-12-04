/*****************************************************************************
 * Arquivo: Estruturas.h                                                     *
 *                                                                           *
 * Data:29/08/2003                                                           *
 * Auto: José Sahle Netto                                                    *
 *****************************************************************************/

#ifndef __VARIAVEIS_GLOBAIS__
#define __VARIAVEIS_GLOBAIS__

#include "WaCServer.h"

#define COINI_MUTEX_NAME  "MUTEX_ROMA_COINI"
#define MAX_COUNT_RX_QUEUE						15
#define MSG_BODY_LEN							1024 * 64
#define MSG_EXT_LEN								256
#define MULTI_QUEUE_COUNT						4
#define MAX_OBJ_THREAD_COUNTER					50      /// #50 max simultaneous calling objs
#define DEFAULT_ERROR_QUEUE_NAME				L".\\Private$\\ROMA_ERROR"

typedef struct _EnumConnections
{
	HANDLE	hThread;
	DWORD	dwThreadID;
	bool	bUsed;
	time_t	tmLastResponse;
	long	lBytesSent;
	long	lBytesReceived;
} EnumConnections;

typedef struct _VariaveisGlobais
{
	// Controla a quantidade de threads que invocam componentes estão ativas
	HANDLE	hwndObjInvokeThread[MAX_OBJ_THREAD_COUNTER];	// Recebe o HANDLE da thread criada

	DWORD	dwObjThreadID[MAX_OBJ_THREAD_COUNTER];			// Recebe o ID da thread criada
	BOOL	bObjInvokeThreadActive[MAX_OBJ_THREAD_COUNTER];	// Indica se a thread que invoca os componentes estão ativas ou não;
	BOOL	bProgramFinishRequest;							/* Variável utilizada para que as thread que verificam 
															   as filas continuem ativas até que esta variável seja configurada com FALSE. */
	DWORD	dwObjInstNum;									// Quantidade de threads ativas que invocam os componentes.

	HANDLE	hEvSynObj;

	// Tratamento das filas de mensagens assíncronas
	WCHAR	*wsaQueueNames[MAX_COUNT_RX_QUEUE];
	TCHAR	*szQueueError[MAX_COUNT_RX_QUEUE];
	HANDLE	hObjQueueThread[MAX_COUNT_RX_QUEUE];
	int		dwFilaTimeOut;
	int		dwTotalFilas;
	int		dwTotalFilasAbertas;
	int		dwTotalTentativasDeAbertura;

	// Nome da fila de erro
	WCHAR	wsErrorQueueName[255];

	// Serviço Remoto
	BOOL	bIniciarServico;
	short	dwPortaDeComunicacao;
	short	dwQuantidadeConexoesSimultaneas;
	int		dwQuantidadeDeUsuarios;
	int		dwTempoSemResposta;
	char	**szSecurityUsuarios;

	// Variáveis do Serviço
	int		dwServicoTimeOut;
	
	// Variáveis do Servidor
	CWaCServer g_Server;

	EnumConnections *g_Connections;
	short			g_nConnectionCount;


} STRU_VARIAVEIS_GLOBAIS;


// Comandos do Servidor
#define COMANDO_PING					0
#define COMANDO_TOTAL_THREADS_ATIVAS	1
#define COMANDO_TOTAL_FILAS_ABERTAS		2
#define COMANDO_NOME_FILA				3
#define COMANDO_NOME_FILA_ERRO			4
#define COMANDO_TOTAL_USUARIOS			5
#define COMANDO_NOME_USUARIO			6
#define COMANDO_FINALIZAR				7
#define COMANDO_TOTAL_FILAS				8

#define COMANDO_IDLE					-1

#define SOCKET_BUFFER_SIZE				2048

#endif
