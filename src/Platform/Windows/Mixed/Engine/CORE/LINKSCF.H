#ifndef LEMBALL_ENGINE_CORE_LINKSCF_H
#define LEMBALL_ENGINE_CORE_LINKSCF_H

struct LinkScfIceChunkManagerView {
	LinkScfIceChunkManagerView* ConstructIceChunkManager(void* pLevelMode, int nCapacity);
	LinkScfIceChunkManagerView* ConstructIceChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfPgunChunkManagerView {
	LinkScfPgunChunkManagerView* ConstructPgunChunkManager(void* pLevelMode, int nCapacity);
	LinkScfPgunChunkManagerView* ConstructPgunChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfTramChunkManagerView {
	LinkScfTramChunkManagerView* ConstructTramChunkManager(void* pLevelMode, int nCapacity);
	LinkScfTramChunkManagerView* ConstructTramChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfCaptureChunkManagerView {
	LinkScfCaptureChunkManagerView* ConstructCaptureTriggerChunkManager(void* pLevelMode, int nCapacity);
	LinkScfCaptureChunkManagerView* ConstructCaptureTriggerChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfRockChunkManagerView {
	LinkScfRockChunkManagerView* ConstructRockChunkManager(void* pLevelMode, int nCapacity);
	LinkScfRockChunkManagerView* ConstructRockChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfDoorChunkManagerView {
	LinkScfDoorChunkManagerView* ConstructDoorChunkManager(void* pLevelMode, int nCapacity);
	LinkScfDoorChunkManagerView* ConstructDoorChunkManagerThunk(void* pLevelMode, int nCapacity);
};


struct CCollectableManager {
	CCollectableManager* ConstructCollChunkManager(void* pLevelMode, int nCapacity);
	CCollectableManager* ConstructCollChunkManagerThunk(void* pLevelMode, int nCapacity);
};

class CAI;
class CGlobalGameObject;
class CGameObject;
class CNetworkMessage;
class CViewData;
struct CGenericGroup;
struct tRect;
struct CVSRect;
struct AICOORD;
struct CGenericGroupManager {
	void* m_pVtable00;
	CGenericGroup* m_apGroups04[40];
	int m_nGroupCountA4;
	int m_nCurrentGroupA8;
	int m_fActiveAC;

	CGenericGroupManager* ConstructCachedChunkObjectManagerBase(void* pLevelMode,
		void* pEntityLookup,
		void* pProjectileGeometryTable);
	CGenericGroupManager* ConstructCachedChunkObjectManagerBaseThunk(void* pLevelMode,
		void* pEntityLookup,
		void* pProjectileGeometryTable);
	int GetNumberOfGroups(void);
	CGenericGroup* GetFirstGroup(void);
	CGenericGroup* GetNextGroup(void);
	CGenericGroup* GetNthGroup(int nIndex);
	CGenericGroup* GetCurrentGroup(void);
	int GetNumberOfElements(void);
	CGameObject* GetFirstElement(void);
	CGameObject* GetNextElement(void);
	CGameObject* GetCurrentElement(void);
	CGameObject* GetNthElement(int nIndex);
	CGenericGroup* GetGroupElementIsMemberOf(CGameObject* pObject);
	void AddNewGroup(CGenericGroup* pGroup);
	void CreateNewGroup(void);
	void AddElementToGroup(CGameObject* pObject, CGenericGroup* pGroup);
	int RemoveElementFromGroup(CGameObject* pObject, CGenericGroup* pGroup);
	void FindElementInGroupAndRemoveIt(CGameObject* pObject);
	int GetAllBoundingBoxes(tRect* pRects);
	int GetViewData(CViewData* pViewData);
	int CheckGroupIntersection(CVSRect* pRect, AICOORD* pPosition);
};
class CObjectManager {
public:
	char m_abReserved00[0x34];
	unsigned short m_nObjectCount34;
	unsigned short m_nPendingCount36;
	char m_abReserved38[4];
	void** m_apObjects3C;

	CObjectManager(CAI* pAI, int nCapacity);
	~CObjectManager();
	void Restart(void);
	void Initialise(int nCount);
	CGlobalGameObject* AddObject(unsigned short nId, CGlobalGameObject* pObject, unsigned char nState);
	CGlobalGameObject* FindObject(int nObjectId);
	void Remove(CGlobalGameObject* pObject);
	int Receive(unsigned short nMessage, CGlobalGameObject* pObject, CNetworkMessage* pMessage);
	void ConvertVer0ToVer1(void);
	int GetViewData(CViewData* pViewData);
	void Process(void);
};

struct LinkScfMoveChunkManagerView {
	LinkScfMoveChunkManagerView* ConstructMoveChunkManager(void* pLevelMode, int nCapacity);
	LinkScfMoveChunkManagerView* ConstructMoveChunkManagerThunk(void* pLevelMode, int nCapacity);
};


struct LinkScfEnmyChunkManagerView {
	LinkScfEnmyChunkManagerView* ConstructEnmyChunkManager(void* pLevelMode,
														   void* pGmobManager,
														   void* pProjectileGeometryTable);
	LinkScfEnmyChunkManagerView* ConstructEnmyChunkManagerThunk(void* pLevelMode,
																void* pGmobManager,
																void* pProjectileGeometryTable);
};

struct LinkScfShpgChunkManagerView {
	LinkScfShpgChunkManagerView* ConstructShpgChunkManager(void* pLevelMode,
														   void* pGmobManager,
														   void* pProjectileGeometryTable);
	LinkScfShpgChunkManagerView* ConstructShpgChunkManagerThunk(void* pLevelMode,
																void* pGmobManager,
																void* pProjectileGeometryTable);
};

struct LinkScfPlasChunkManagerView {
	void LoadPlasChunkObjects(const unsigned short* pChunk, unsigned int cbChunk, int fReuseState);
	void LoadPlasChunkObjectsThunk(const unsigned short* pChunk, unsigned int cbChunk, int fReuseState);
};

struct LinkScfLevelChunkLoaderView {
	void LoadLevelChunksForSkillAndLevel(unsigned int nSkill, unsigned int nLevel, int fReuseState);
	void LoadLevelChunksForSkillAndLevelThunk(unsigned int nSkill, unsigned int nLevel, int fReuseState);
};

struct LinkScfBoonChunkManagerView {
	LinkScfBoonChunkManagerView* ConstructBoonChunkManager(void* pLevelMode, void* pTileGrid);
	LinkScfBoonChunkManagerView* ConstructBoonChunkManagerThunk(void* pLevelMode, void* pTileGrid);
	void DestroyBoonChunkManagerChildren(void);
};

// Referenced real destinations that remain minimally implemented.
// STUB: LEMBALL 0x0040df90
// DispatchDoorChildArrayVirtual104
// STUB: LEMBALL 0x0040e550
// ServiceActiveDoorChunkObjects
// STUB: LEMBALL 0x0040e080
// EmitDoorChunkRenderEntries


// STUB: LEMBALL 0x0041ecb0
// RemoveCachedChunkObjectAndDestroy

// STUB: LEMBALL 0x0041ebe0
// AppendManagedEntityGroupFromIds


// MACINTOSH: CCollectableManager::Restart()
// STUB: LEMBALL 0x00422420
// ActivateCollChunkObjects
// MACINTOSH: CCollectableManager::Process()
// STUB: LEMBALL 0x00422550
// RefreshActiveCollChunkObjects
// STUB: LEMBALL 0x00422ef0
// EmitCollChunkManagerRenderEntries

// RefreshActiveMineChunkObjects
// EmitMineChunkManagerRenderEntries


// STUB: LEMBALL 0x00427010
// RefreshActiveRockChunkObjects
// STUB: LEMBALL 0x00427050
// EmitRockChunkManagerRenderEntries
// MACINTOSH: CHandManager::Process()
// STUB: LEMBALL 0x004283c0
// RefreshActiveCaptureTriggerChunkObjects
// MACINTOSH: CHandManager::GetViewData(CViewData*)
// STUB: LEMBALL 0x00428410
// EmitCaptureTriggerChunkManagerRenderEntries
// STUB: LEMBALL 0x0042afc0
// InitializeTramChunkObjects
// STUB: LEMBALL 0x0042b4f0
// EmitTramChunkManagerRenderEntries
// MACINTOSH: CPaintGunManager::Process()
// STUB: LEMBALL 0x0042c4d0
// RefreshActivePgunChunkObjects
// MACINTOSH: CPaintGunManager::GetViewData(CViewData*)
// STUB: LEMBALL 0x0042c520
// EmitActivePgunChunkManagerRenderEntries
// MACINTOSH: CMoverManager::GetViewData(CViewData*)
// STUB: LEMBALL 0x0042f540
// EmitMoveChunkManagerRenderEntries

#endif
