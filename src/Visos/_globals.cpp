#include "_globals.h"

// Confirmed module globals.
// GLOBAL: LEMBALL 0x00498ab0
void* g_apCResRasterConstructionVtable[15];

// GLOBAL: LEMBALL 0x004a071c
void* g_pDebugAcceleratorTable;

// GLOBAL: LEMBALL 0x004a0768
TargetGraphicsSystemState* g_pTargetGraphicsSystem;

// GLOBAL: LEMBALL 0x004a0e68
int g_nDebugInitialized;

// GLOBAL: LEMBALL 0x004a0e80
int g_nDebugFileOutputEnabled;

// GLOBAL: LEMBALL 0x004a0e84
int g_nAsyncDebugEnabled;

// GLOBAL: LEMBALL 0x004a2260
unsigned int g_dwFileNetworkThreadId;

// GLOBAL: LEMBALL 0x004a2264
void* g_hFileNetworkThread;

// GLOBAL: LEMBALL 0x004a23b0
unsigned int g_dwTCPIPNetworkThreadId;

// GLOBAL: LEMBALL 0x004a23b4
void* g_hTCPIPNetworkThread;

// GLOBAL: LEMBALL 0x004a27a0
unsigned int g_dwInputQuitRequested;

// GLOBAL: LEMBALL 0x004a2808
TargetInputTranslationEntry g_dwInputTranslationPairs[61];

// GLOBAL: LEMBALL 0x004a97c8
char g_szSoundDriverInfo[1024];

// GLOBAL: LEMBALL 0x004a8188
void* g_pApplicationInstance;

// GLOBAL: LEMBALL 0x004a818c
unsigned int g_dwWindowQuitRequested;

// GLOBAL: LEMBALL 0x004a29f4
void* g_pDebugThread;

// GLOBAL: LEMBALL 0x004a2a04
void* g_pDebugSyncEvent;

// GLOBAL: LEMBALL 0x004a2a08
int g_nDebugThreadId;

// GLOBAL: LEMBALL 0x004a2a0c
FILE* g_pDebugOutputFile;

// GLOBAL: LEMBALL 0x004a2a10
char* g_pDebugOutputPath;

// GLOBAL: LEMBALL 0x004a2b68
int g_nTargetTextWindowClassRegistered;

// GLOBAL: LEMBALL 0x004a2b7c
int g_nTargetTextWindowActive;

// GLOBAL: LEMBALL 0x004a2b8c
int g_nTargetTextWindowCreated;

// GLOBAL: LEMBALL 0x004a8088
char g_szCurrentDirectory[256];

// GLOBAL: LEMBALL 0x0049a4a0
void* g_apTargetTextWindowLockVtable[2];

// GLOBAL: LEMBALL 0x004a9bf8
unsigned int g_dwWinGDrawColourTable[256];

// GLOBAL: LEMBALL 0x004a9ff8
char g_szSourceDiskPath[256];

// GLOBAL: LEMBALL 0x004a1dd0
char g_szCDRootPath[52];

// GLOBAL: LEMBALL 0x004a1e24
unsigned int g_networkPacketSize;

// GLOBAL: LEMBALL 0x004a1e28
BasePacketHeader* g_pNetworkPacketScratch;

// GLOBAL: LEMBALL 0x004a1e2c
unsigned int g_receivedPacketSize;

// GLOBAL: LEMBALL 0x004a2de4
void* g_pFileBroadcastData;

// GLOBAL: LEMBALL 0x004a1e34
unsigned char* g_pBroadcastPacketTemplate;

// GLOBAL: LEMBALL 0x004a1e38
char* g_szBroadcastPeerName;

// GLOBAL: LEMBALL 0x004a1e3c
unsigned int g_broadcastPayloadLength;

// GLOBAL: LEMBALL 0x004a1e48
unsigned short g_broadcastPort;

// GLOBAL: LEMBALL 0x004a1e4c
int g_lastNetworkError;

// GLOBAL: LEMBALL 0x004a1d64
ResourceTypeList* g_pCompressedResourceTypes;

// GLOBAL: LEMBALL 0x004a1d68
ResourceTypeList* g_pResourceTypes;

// GLOBAL: LEMBALL 0x004a1d6c
ResourceTypeList* g_pPreloadedResourceTypes;

// GLOBAL: LEMBALL 0x004a217c
int g_nSmallMemoryEnabled;

// GLOBAL: LEMBALL 0x004a2180
void* g_pMasterArenaMemory;

// GLOBAL: LEMBALL 0x004a13c8
int g_nVSTrigSource[512];

// GLOBAL: LEMBALL 0x004a1bc8
unsigned int g_dwVSTrigInitialised;
