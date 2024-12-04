#include <stdio.h>
#include <windows.h>
#include <stdarg.h>
#include <objbase.h>
#include <comdef.h>
#include <mq.h>

char m_szServiceName[]="EventLog1";

///////////////////////////////////////////////////////////////////////////////////////
// Converte código MSMQ em um código interno de mensagens
int CodigoMSMQ2Interno(unsigned long nCod)
{
	int nNum;

	switch(nCod) {
		case MQ_ERROR:									nNum = 0x1;		break;
		case MQ_ERROR_PROPERTY:							nNum = 0x2;		break;
		case MQ_ERROR_QUEUE_NOT_FOUND:					nNum = 0x3;		break;
		case MQ_ERROR_QUEUE_EXISTS:						nNum = 0x4;		break;
		case MQ_ERROR_INVALID_PARAMETER:				nNum = 0x5;		break;
		case MQ_ERROR_INVALID_HANDLE:					nNum = 0x6;		break;
		case MQ_ERROR_OPERATION_CANCELLED:				nNum = 0x7;		break;
		case MQ_ERROR_SHARING_VIOLATION:				nNum = 0x8;		break;
		case MQ_ERROR_SERVICE_NOT_AVAILABLE:			nNum = 0x9;		break;
		case MQ_ERROR_MACHINE_NOT_FOUND:				nNum = 0xa;		break;
		case MQ_ERROR_ILLEGAL_SORT:						nNum = 0xb;		break;
		case MQ_ERROR_ILLEGAL_USER:						nNum = 0xc;		break;
		case MQ_ERROR_NO_DS:							nNum = 0xd;		break;
		case MQ_ERROR_ILLEGAL_QUEUE_PATHNAME:			nNum = 0xe;		break;
		case MQ_ERROR_ILLEGAL_PROPERTY_VALUE:			nNum = 0xf;		break;
		case MQ_ERROR_ILLEGAL_PROPERTY_VT:				nNum = 0x10;		break;
		case MQ_ERROR_BUFFER_OVERFLOW:					nNum = 0x11;		break;
		case MQ_ERROR_IO_TIMEOUT:						nNum = 0x12;		break;
		case MQ_ERROR_ILLEGAL_CURSOR_ACTION:			nNum = 0x13;		break;
		case MQ_ERROR_MESSAGE_ALREADY_RECEIVED:			nNum = 0x14;		break;
		case MQ_ERROR_ILLEGAL_FORMATNAME:				nNum = 0x15;		break;
		case MQ_ERROR_FORMATNAME_BUFFER_TOO_SMALL:		nNum = 0x16;		break;
		case MQ_ERROR_UNSUPPORTED_FORMATNAME_OPERATION:nNum = 0x17;		break;
		case MQ_ERROR_ILLEGAL_SECURITY_DESCRIPTOR:		nNum = 0x18;		break;
		case MQ_ERROR_SENDERID_BUFFER_TOO_SMALL:		nNum = 0x19;		break;
		case MQ_ERROR_SECURITY_DESCRIPTOR_TOO_SMALL:	nNum = 0x1a;		break;
		case MQ_ERROR_CANNOT_IMPERSONATE_CLIENT:		nNum = 0x1b;		break;
		case MQ_ERROR_ACCESS_DENIED:					nNum = 0x1c;		break;
		case MQ_ERROR_PRIVILEGE_NOT_HELD:				nNum = 0x1d;		break;			
		case MQ_ERROR_INSUFFICIENT_RESOURCES:			nNum = 0x1e;		break;		
		case MQ_ERROR_USER_BUFFER_TOO_SMALL:			nNum = 0x1f;		break;
		case MQ_ERROR_MESSAGE_STORAGE_FAILED:			nNum = 0x20;		break;
		case MQ_ERROR_SENDER_CERT_BUFFER_TOO_SMALL:		nNum = 0x21;		break;
		case MQ_ERROR_INVALID_CERTIFICATE:				nNum = 0x22;		break;
		case MQ_ERROR_CORRUPTED_INTERNAL_CERTIFICATE:	nNum = 0x23;		break;
		case MQ_ERROR_NO_INTERNAL_USER_CERT:			nNum = 0x24;		break;
		case MQ_ERROR_CORRUPTED_SECURITY_DATA:			nNum = 0x25;		break;
		case MQ_ERROR_CORRUPTED_PERSONAL_CERT_STORE:	nNum = 0x26;		break;
		case MQ_ERROR_COMPUTER_DOES_NOT_SUPPORT_ENCRYPTION:	nNum = 0x27;		break;
		case MQ_ERROR_BAD_SECURITY_CONTEXT:				nNum = 0x28;		break;
		case MQ_ERROR_COULD_NOT_GET_USER_SID:			nNum = 0x29;		break;
		case MQ_ERROR_COULD_NOT_GET_ACCOUNT_INFO:		nNum = 0x2a;		break;
		case MQ_ERROR_ILLEGAL_MQCOLUMNS:				nNum = 0x2b;		break;
		case MQ_ERROR_ILLEGAL_PROPID:					nNum = 0x2c;		break;
		case MQ_ERROR_ILLEGAL_RELATION:					nNum = 0x2d;		break;
		case MQ_ERROR_ILLEGAL_PROPERTY_SIZE:			nNum = 0x2e;		break;
		case MQ_ERROR_ILLEGAL_RESTRICTION_PROPID:		nNum = 0x2f;		break;
		case MQ_ERROR_ILLEGAL_MQQUEUEPROPS:				nNum = 0x30;		break;
		case MQ_ERROR_PROPERTY_NOTALLOWED:				nNum = 0x31;		break;
		case MQ_ERROR_INSUFFICIENT_PROPERTIES:			nNum = 0x32;		break;
		case MQ_ERROR_MACHINE_EXISTS:					nNum = 0x33;		break;
		case MQ_ERROR_ILLEGAL_MQQMPROPS:				nNum = 0x34;		break;
		case MQ_ERROR_DS_IS_FULL:						nNum = 0x35;		break;
		case MQ_ERROR_DS_ERROR:							nNum = 0x36;		break;
		case MQ_ERROR_INVALID_OWNER:					nNum = 0x37;		break;
		case MQ_ERROR_UNSUPPORTED_ACCESS_MODE:			nNum = 0x38;		break;
		case MQ_ERROR_RESULT_BUFFER_TOO_SMALL:			nNum = 0x39;		break;
		case MQ_ERROR_DELETE_CN_IN_USE:					nNum = 0x3a;		break;
		case MQ_ERROR_NO_RESPONSE_FROM_OBJECT_SERVER:	nNum = 0x3b;		break;
		case MQ_ERROR_OBJECT_SERVER_NOT_AVAILABLE:		nNum = 0x3c;		break;
		case MQ_ERROR_QUEUE_NOT_AVAILABLE:				nNum = 0x3d;		break;
		case MQ_ERROR_DTC_CONNECT:						nNum = 0x3e;		break;
		case MQ_ERROR_TRANSACTION_IMPORT:				nNum = 0x3f;		break;
		case MQ_ERROR_TRANSACTION_USAGE:				nNum = 0x40;		break;
		case MQ_ERROR_TRANSACTION_SEQUENCE:				nNum = 0x41;		break;
		case MQ_ERROR_MISSING_CONNECTOR_TYPE:			nNum = 0x42;		break;
		case MQ_ERROR_STALE_HANDLE:						nNum = 0x43;		break;
		case MQ_ERROR_TRANSACTION_ENLIST:				nNum = 0x44;		break;
		case MQ_ERROR_QUEUE_DELETED:					nNum = 0x45;		break;
		case MQ_ERROR_ILLEGAL_CONTEXT:					nNum = 0x46;		break;
		case MQ_ERROR_ILLEGAL_SORT_PROPID:				nNum = 0x47;		break;
		case MQ_ERROR_LABEL_TOO_LONG:					nNum = 0x48;		break;
		case MQ_ERROR_LABEL_BUFFER_TOO_SMALL:			nNum = 0x49;		break;
		case MQ_ERROR_MQIS_SERVER_EMPTY:				nNum = 0x4a;		break;
		case MQ_ERROR_MQIS_READONLY_MODE:				nNum = 0x4b;		break;
		case MQ_ERROR_SYMM_KEY_BUFFER_TOO_SMALL:		nNum = 0x4c;		break;
		case MQ_ERROR_SIGNATURE_BUFFER_TOO_SMALL:		nNum = 0x4d;		break;
		case MQ_ERROR_PROV_NAME_BUFFER_TOO_SMALL:		nNum = 0x4e;		break;
		case MQ_ERROR_ILLEGAL_OPERATION:				nNum = 0x4f;		break;
		case MQ_ERROR_WRITE_NOT_ALLOWED:				nNum = 0x50;		break;
		case MQ_ERROR_WKS_CANT_SERVE_CLIENT:			nNum = 0x51;		break;
		case MQ_ERROR_DEPEND_WKS_LICENSE_OVERFLOW:		nNum = 0x52;		break;
		case MQ_CORRUPTED_QUEUE_WAS_DELETED:			nNum = 0x53;		break;
		case MQ_ERROR_REMOTE_MACHINE_NOT_AVAILABLE:		nNum = 0x54;		break;
		case MQ_INFORMATION_PROPERTY:					nNum = 0x55;		break;
		case MQ_INFORMATION_ILLEGAL_PROPERTY:			nNum = 0x56;		break;
		case MQ_INFORMATION_PROPERTY_IGNORED:			nNum = 0x57;		break;
		case MQ_INFORMATION_UNSUPPORTED_PROPERTY:		nNum = 0x58;		break;
		case MQ_INFORMATION_DUPLICATE_PROPERTY:			nNum = 0x59;		break;
		case MQ_INFORMATION_OPERATION_PENDING:			nNum = 0x5a;		break;
		case MQ_INFORMATION_FORMATNAME_BUFFER_TOO_SMALL:	nNum = 0x5b;		break;

		default:	return 0;
		}

	return (nNum);
}

///////////////////////////////////////////////////////////////////////////////////////
// Logging functions
void LogEvent(WORD wType, DWORD dwEventID, LPCTSTR pFormat, ...)
{
    TCHAR    chMsg[256];
    HANDLE  hEventSource;
    LPTSTR  lpszStrings[1];
    va_list pArg;

    va_start(pArg, pFormat);
    vsprintf(chMsg, pFormat, pArg);
    va_end(pArg);

    lpszStrings[0] = chMsg;

    /* Get a handle to use with ReportEvent(). */
    hEventSource = RegisterEventSource(NULL, m_szServiceName );
    if (hEventSource != NULL)
    {
        /* Write to event log. */
        ReportEvent(hEventSource, wType, 0, CodigoMSMQ2Interno(dwEventID), NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
        DeregisterEventSource(hEventSource);
    }
}


int main()
{
	int nEvento;
	char chMsg[300];


	//printf("Digite o código do evento a ser registrado: "); scanf("%d",&nEvento);
	//printf("Digite uma palavra adicional: ");  scanf("%s",chMsg);
	//LogEvent (EVENTLOG_SUCCESS,nEvento,chMsg);
	
	LogEvent (EVENTLOG_SUCCESS,MQ_ERROR_ILLEGAL_USER,"Mentira!!");
	return 0;
}
