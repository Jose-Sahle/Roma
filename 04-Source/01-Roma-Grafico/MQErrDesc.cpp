////////////////////////////////////////////////
//This function load into wszDesc the description of the MQ 
//error number past into hError param.
//cDescSiz = can cuts the msg body							Aut= Oswaldo Bernice -MCS 2001
//Return: ULONG portion of wszDesc
///////////////////////
#include "MQErrDesc.h"

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