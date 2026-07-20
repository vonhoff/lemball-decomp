#ifndef VSNET_FORWARD_H
#define VSNET_FORWARD_H

struct VsNetEffTransportRecordBuffer;

static void NetworkRuntimeTransportUnavailable(void* pObject);
static void NetworkRuntimeTransportNoop(void* pObject);
static int NetworkRuntimeCreateNoChannelOwner(void* pObject);
static void* NetworkRuntimeWindowCreateNoChannelOwner(void* pObject);

void LEMBALL_FASTCALL WriteEffTransportRequestStreamName(void* pObject);
int CompareEffTransportRequestStreamName(void* pObject);
void* ConstructEffTransportRequestConnectControlStream(void* pObject, char* pszStreamName);
void __fastcall LoadEffTransportRequestConnectStream(void* pObject);
void __fastcall WriteEffTransportRequestPortFields(void* pObject);
void* ConstructEffTransportRequestNewPortControlStream(void* pObject, char* pszStreamName);
void LEMBALL_FASTCALL LoadEffTransportRequestNewPortStream(void* pObject);
void LEMBALL_FASTCALL WriteEffTransportRequestNewPortStream(void* pObject);
void* ConstructEffTransportAuthoriseConnectControlStream(void* pObject, char* pszStreamName);
void __fastcall LoadEffTransportAuthoriseConnectStream(void* pObject);
void LEMBALL_FASTCALL WriteEffTransportAuthoriseConnectStream(void* pObject);
void* ConstructEffTransportGoAheadConnectControlStream(void* pObject, char* pszStreamName);
void __fastcall LoadEffTransportGoAheadConnectStream(void* pObject);
void LEMBALL_FASTCALL WriteEffTransportGoAheadConnectStream(void* pObject);
void* ConstructEffTransportFailedConnectControlStream(void* pObject, char* pszStreamName);
void LEMBALL_FASTCALL LoadEffTransportFailedConnectStream(void* pObject);
void LEMBALL_FASTCALL WriteEffTransportFailedConnectStream(void* pObject);
void CheckEffTransportIdleTimeout(void* pObject);
int RetryPendingEffTransportRecord(void* pObject, int nRecord);
void WINAPI LoadEffStreamFromGlobalRange(void* pStream);
void QueueEffTransportErrorEvent(int nError);
void ConfigureActiveEffTransportConnectHost(void* pRuntime, char* pszHostName);
void* ConstructShiftedLockedRecordSlotTable(void* pObject, int cPacketIds, unsigned int cbRecord);
void* ConstructRangeEffTransportRecordBufferTable(void* pObject,
												  int nFirstPacket,
												  int nLastPacket,
												  int cbPayload,
												  unsigned int cbRecord);
void* ConstructRingLockedRecordSlotTable(void* pObject, int cRecords, unsigned int cbRecord);
void* ConstructRingEffTransportRecordBufferTable(void* pObject, int cRecords, int cbPayload, unsigned int cbRecord);
int LEMBALL_FASTCALL AllocateEffTransportPeerEntry(void* pRuntimeState);
void* LEMBALL_FASTCALL DeleteEffTransportRecordBufferBaseWrapper(VsNetEffTransportRecordBuffer* pObject,
																 int,
																 BYTE fFreeMemory);
void* DeleteLockedEffTransportRecordSlotWrapper(void* pObject, BYTE fFreeMemory);
void* DeleteSimpleEffTransportRecordSlotWrapper(void* pObject, BYTE fFreeMemory);
void* DeleteEffTransportRecordBufferWrapper(void* pObject, BYTE fFreeMemory);
void* LEMBALL_FASTCALL DeleteRangeEffTransportRecordBufferTableWrapper(VsNetEffTransportRecordBuffer* pObject,
																	   int,
																	   BYTE fFreeMemory);
void* LEMBALL_FASTCALL DeleteRingEffTransportRecordBufferTableWrapper(VsNetEffTransportRecordBuffer* pObject,
																	  int,
																	  BYTE fFreeMemory);
void* DeleteEffTransportRuntimeGlobalsWrapper(void* pObject, BYTE fFreeMemory);
DWORD FileBasedNetworkMessageThreadMain(void);
int StartFileBasedNetworkMessageThread(void);
int StopFileBasedNetworkMessageThread(void);
void ConfigureFileBasedNetworkPaths(char* pszLocalHost, char* pszBasePath);
void WINAPI ConfigureFileBasedNetworkPathsWrapper(char* pszLocalHost, char* pszBasePath);
static void* AllocateFileNetworkAddress(void);
DWORD TcpipNetworkMessageThreadMain(void);
int StartTcpipNetworkMessageThread(void);
int StopTcpipNetworkMessageThread(void);

#endif
