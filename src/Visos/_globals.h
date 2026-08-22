#ifndef LEMBALL_SCAFFOLD_VISOS__GLOBALS_H
#define LEMBALL_SCAFFOLD_VISOS__GLOBALS_H

#include "../Common.h"
#include "Messaging/BasePacketHeader.h"         // complete type
#include "Resources/ResourceTypeList.h"         // complete type
#include "Target/TargetGraphicsSystemState.h"   // complete type
#include "Target/TargetInputTranslationEntry.h" // complete type

// Confirmed module globals.
extern void* g_apCResRasterConstructionVtable[15];
extern void* g_pDebugAcceleratorTable;
extern TargetGraphicsSystemState* g_pTargetGraphicsSystem;
extern int g_nDebugInitialized;
extern int g_nDebugFileOutputEnabled;
extern int g_nAsyncDebugEnabled;
extern unsigned int g_dwFileNetworkThreadId;
extern void* g_hFileNetworkThread;
extern unsigned int g_dwTCPIPNetworkThreadId;
extern void* g_hTCPIPNetworkThread;
extern unsigned int g_dwInputQuitRequested;
extern TargetInputTranslationEntry g_dwInputTranslationPairs[61];
extern char g_szSoundDriverInfo[1024];
extern void* g_pApplicationInstance;
extern unsigned int g_dwWindowQuitRequested;
extern void* g_pDebugThread;
extern void* g_pDebugSyncEvent;
extern int g_nDebugThreadId;
extern FILE* g_pDebugOutputFile;
extern char* g_pDebugOutputPath;
extern int g_nTargetTextWindowClassRegistered;
extern int g_nTargetTextWindowActive;
extern int g_nTargetTextWindowCreated;
extern char g_szCurrentDirectory[256];
extern void* g_apTargetTextWindowLockVtable[2];
extern unsigned int g_dwWinGDrawColourTable[256];
extern char g_szSourceDiskPath[256];
extern char g_szCDRootPath[52];
extern unsigned int g_networkPacketSize;
extern BasePacketHeader* g_pNetworkPacketScratch;
extern unsigned int g_receivedPacketSize;
extern void* g_pFileBroadcastData;
extern unsigned char* g_pBroadcastPacketTemplate;
extern char* g_szBroadcastPeerName;
extern unsigned int g_broadcastPayloadLength;
extern unsigned short g_broadcastPort;
extern int g_lastNetworkError;
extern ResourceTypeList* g_pCompressedResourceTypes;
extern ResourceTypeList* g_pResourceTypes;
extern ResourceTypeList* g_pPreloadedResourceTypes;
extern int g_nSmallMemoryEnabled;
extern void* g_pMasterArenaMemory;
extern int g_nVSTrigSource[512];
extern unsigned int g_dwVSTrigInitialised;
#endif
