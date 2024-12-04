/*****************************************************************************
 * Arquivo: Estruturas.h                                                     *
 *                                                                           *
 * Data:29/08/2003                                                           *
 * Auto: José Sahle Netto                                                    *
 *****************************************************************************/

#ifndef __VARIAVEIS_GLOBAIS__
#define __VARIAVEIS_GLOBAIS__


#define COINI_MUTEX_NAME  "MUTEX_ROMA_COINI"
#define MAX_COUNT_RX_QUEUE	15
#define MSG_BODY_LEN   1024 * 16
#define MSG_EXT_LEN    256
#define MULTI_QUEUE_COUNT  4
#define MAX_OBJ_THREAD_COUNTER  50      /// #50 max simultaneous calling objs
#define DEFAULT_ERROR_QUEUE_NAME   L".\\Private$\\ROMA_ERROR"


typedef struct _VariaveisGlobais
{
	HANDLE haObjThread[MAX_OBJ_THREAD_COUNTER+5];
	DWORD  waObjThreadID[MAX_OBJ_THREAD_COUNTER+5];
	DWORD  caObjThreadActive[MAX_OBJ_THREAD_COUNTER+5];
	HANDLE hEvSynObj;
	DWORD  cObjInstNum;
	BOOL   bProgramFinishRequest;
	WCHAR* wsaQueueNames[MAX_COUNT_RX_QUEUE];
	WCHAR* wsErrorQueueName;
} STRU_VARIAVEIS_GLOBAIS;


#endif