#ifndef LEMBALL_GAME_LEVEL_RUNTIME_H
#define LEMBALL_GAME_LEVEL_RUNTIME_H

#include "ENGINE/CORE/COMMON.H"

struct LevelLevelTileGridStaticView;
struct LevelLevelTileCell;

struct LevelCachedChunkChildView {
	virtual void Reserved00(void) = 0;
	virtual void Reserved01(void) = 0;
	virtual void Reserved02(void) = 0;
	virtual void Reserved03(void) = 0;
	virtual void Reserved04(void) = 0;
	virtual void ServiceCachedChunkObject(void) = 0;
};

struct LevelCachedChunkManagerAView {
	virtual void Reserved00(void) = 0;
	virtual LevelCachedChunkChildView* GetFirstCachedChunkObject(void) = 0;
	virtual LevelCachedChunkChildView* GetNextCachedChunkObject(void) = 0;

	int ServiceCachedChunkObjects(void);
	int ServiceCachedChunkObjectsThunk(void);
};

struct LevelCachedChunkManagerBView {
	virtual void Reserved00(void) = 0;
	virtual LevelCachedChunkChildView* GetFirstCachedChunkObject(void) = 0;
	virtual LevelCachedChunkChildView* GetNextCachedChunkObject(void) = 0;

	int ServiceCachedChunkObjects(void);
	int ServiceCachedChunkObjectsThunk(void);
};

struct LevelTileGridRuntimeView {
	void ReleaseLevelTileGridBuffers(void);
	void ResizeLevelTileGrid(int cColumns, int cRows);
	void ResizeLevelTileGridThunk(int cColumns, int cRows);
	void LoadLevelTileGridChunk(const unsigned short* pWords, unsigned int cbPayload, int fReuseState);
	void SetLevelTileGridDefaultVariantIds(const unsigned short* pWords, unsigned int cbPayload);
	void SetLevelTileGridName(const char* pszName, unsigned int cbPayload);
	void CopyLevelTileGridName(const char* pszName);
	void CopyLevelTileGridNameThunk(const char* pszName);
	void ComputeLevelTileGridRepeatedColumnCounts(void);
	void ComputeLevelTileGridRepeatedColumnCountsThunk(void);
};

struct LevelTileGridStaticRenderEntry {
	short m_nTileX00;
	short m_nTileY02;
	unsigned short m_nSortKey04;
	unsigned short m_nReserved06;
	void* m_pTileCell08;
};

struct LevelSpecialTileGridStaticRenderList {
	LevelTileGridStaticRenderEntry* m_pEntries00;
	int m_cEntries04;

	void BuildSpecialLevelTileGridStaticRenderEntries(LevelLevelTileGridStaticView* pTileGrid);
	void BuildSpecialLevelTileGridStaticRenderEntriesThunk(LevelLevelTileGridStaticView* pTileGrid);
};

struct LevelProjectileGeometryPair {
	int m_nX;
	int m_nY;

	LevelProjectileGeometryPair* InitializeSentinel(void);
	LevelProjectileGeometryPair* InitializeSentinelThunk(void);
};

struct LevelProjectileGeometryPairTable {
	LevelProjectileGeometryPair m_aPrimaryPairs00[24];
	LevelProjectileGeometryPair m_aSecondaryPairsC0[8];
	int m_nReserved100;

	LevelProjectileGeometryPairTable* InitializeProjectileGeometryPairTable(void);
	LevelProjectileGeometryPairTable* InitializeProjectileGeometryPairTableThunk(void);
	void ClearProjectileGeometryField100(void);
	void ClearProjectileGeometryField100Thunk(void);
};

struct LevelCachedChunkObjectManagerBase {
	void** m_pVtable00;
	void* m_apObjects04[40];
	int m_cObjectsA4;
	int m_nReservedA8;
	int m_fEnabledAC;

	void DestroyCachedChunkObjects(void);
};

struct LevelShpgChunkManager : public LevelCachedChunkObjectManagerBase {
	void ActivateNestedChildrenFromOwnerTableA4(void);
	void ActivateNestedChildrenFromOwnerTableA4Thunk(void);
};

struct LevelEnmyChunkManager : public LevelCachedChunkObjectManagerBase {
	void ActivateNestedChildrenFromOwnerTableVariant(void);
	void ActivateNestedChildrenFromOwnerTableVariantThunk(void);
};

struct LevelChunkLoaderContext {
	void* m_pLevelMode00;
	int m_nFallbackLevel04;

	LevelChunkLoaderContext* ConstructLevelChunkLoaderContext(void* pLevelMode);
	LevelChunkLoaderContext* ConstructLevelChunkLoaderContextThunk(void* pLevelMode);
};

struct LevelNodeChunkRecord {
	int m_nX00;
	int m_nY04;
	int m_cExpectedLinks08;
	int* m_pLinks0C;
	int m_cLinks10;

	LevelNodeChunkRecord* InitializeNodeChunkRecord(void);
	LevelNodeChunkRecord* InitializeNodeChunkRecordThunk(void);
	void InitializeNodeChunkRecordFromLevelData(int nX, int nY, int cLinks);
	void InitializeNodeChunkRecordFromLevelDataThunk(int nX, int nY, int cLinks);
	void AppendNodeChunkRecordLink(int nFirst, int nSecond);
	void AppendNodeChunkRecordLinkThunk(int nFirst, int nSecond);
	void ResetNodeChunkRecordLinkCount(void);
	void ResetNodeChunkRecordLinkCountThunk(void);
};

struct LevelNodeChunkManager {
	LevelNodeChunkRecord* m_pRecords00;
	int m_nReserved04;
	int m_cCapacity08;

	LevelNodeChunkManager* ConstructNodeChunkManager(int cCapacity);
	LevelNodeChunkManager* ConstructNodeChunkManagerThunk(int cCapacity);
	void AllocateNodeChunkRecordArray(int cRecords);
	void AllocateNodeChunkRecordArrayThunk(int cRecords);
	void LoadNodeChunkRecords(const unsigned short* pWords, unsigned int cbPayload, int fReuseState);
	void LoadNodeChunkRecordsThunk(const unsigned short* pWords, unsigned int cbPayload, int fReuseState);
	void ResetNodeChunkRecords(void);
	void ResetNodeChunkRecordsThunk(void);
	void DestroyNodeChunkManagerRecords(void);
};

struct LevelSlnkChunkManager {
	void* m_pLevelMode00;
	void* m_pObjects04;
	int m_cCapacity08;

	LevelSlnkChunkManager* ConstructSlnkChunkManager(void* pLevelMode, int cCapacity);
	LevelSlnkChunkManager* ConstructSlnkChunkManagerThunk(void* pLevelMode, int cCapacity);
	void ResetType35ChunkObjects(void);
	void ResetType35ChunkObjectsThunk(void);
	void DeleteType35ChunkObjectArray(void);
};

struct LevelBallChunkManager {
	void* m_pLevelMode00;
	void* m_pObjects04;
	int m_nReserved08;
	int m_cCapacity0C;

	LevelBallChunkManager* ConstructBallChunkManager(void* pLevelMode, int cCapacity);
	LevelBallChunkManager* ConstructBallChunkManagerThunk(void* pLevelMode, int cCapacity);
	void DestroyBallChunkManagerEntries(void);
};

struct LevelField194TimerBlock {
	int m_nNextTick00;
	int m_nReserved04;
	char m_abReserved08[0x12c0];
	int m_fActive12C8;

	void ResetLevelField194TimerBlock(void);
	void ResetLevelField194TimerBlockThunk(void);
};

void* LEMBALL_FASTCALL GetLevelGameModeOwner(void* pLevelMode);
void* LEMBALL_FASTCALL GetLevelGameModeOwnerThunk(void* pLevelMode);
void AdvanceLevelFrameClock(void);
void LEMBALL_FASTCALL ServiceRegisteredLevelChunkStreams(void* pDispatcher);
void LoadLevelAiChunk(void* pLevelMode, const unsigned short* pWords, unsigned int cbPayload);
void LoadLevelFlagChunk(void* pLevelMode, const unsigned short* pWords);
void LoadAnimChunkRecords(void* pTimerBlock, const unsigned short* pWords);
void LEMBALL_FASTCALL AdvanceAnimChunkFrames(void* pTimerBlock);
void LEMBALL_FASTCALL DestroyLevelTileReachabilityHelper(void* pReachabilityHelper);

extern LevelBallChunkManager* g_pBallChunkManager;
extern char* g_pszActiveLevelFilePath;
extern void* g_pLoadedLevelFileBuffer;

struct LevelMetadata {
	unsigned short m_cSkill00;
	unsigned short m_cPlayers02;
	unsigned short m_cNetworkPlayers04;
	unsigned short m_nTimeSeconds06;
	char m_szTitle08[0x24];
};

int LoadOverrideLevelFileBuffer(void);
unsigned char* GetNextLevelChunk(unsigned char* pChunk);
void ReadLevelMetadataForSkillAndLevel(unsigned int nSkill, unsigned int nLevel, LevelMetadata* pMetadata);
int GetLevelBinResourceIdForSkillAndLevel(int nSkill, int nLevel);

#endif
