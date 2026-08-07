#ifndef LEMBALL_ENGINE_CORE_LINKSCF_H
#define LEMBALL_ENGINE_CORE_LINKSCF_H


struct CPaintGunManager {
	int m_nCapacity30;         // 0x30
	int m_nObjectCount34;      // 0x34
	void* m_pObjectArray38;    // 0x38
	CPaintGunManager* ConstructPgunChunkManager(void* pLevelMode, int nCapacity);
	CPaintGunManager* ConstructPgunChunkManagerThunk(void* pLevelMode, int nCapacity);
	void Initialise(int nCapacity);
	void ResetObjectCount(void);
};

struct CTrampolineManager {
	int m_nCapacity30;         // 0x30
	int m_nObjectCount34;      // 0x34
	void* m_pObjectArray38;    // 0x38
	CTrampolineManager* ConstructTramChunkManager(void* pLevelMode, int nCapacity);
	CTrampolineManager* ConstructTramChunkManagerThunk(void* pLevelMode, int nCapacity);
	void Restart(void);
	void Initialise(int nCapacity);
	int Hit(void* pCoord, void* pGameObject);
	void ResetObjectCount(void);
};


struct CRocketManager {
	int m_nCapacity30;         // 0x30
	int m_nObjectCount34;      // 0x34
	void* m_pObjectArray38;    // 0x38
	CRocketManager* ConstructRockChunkManager(void* pLevelMode, int nCapacity);
	CRocketManager* ConstructRockChunkManagerThunk(void* pLevelMode, int nCapacity);
	void ResetObjectCount(void);
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
class CGenericGroup;
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
		void* pFormationManager);
	CGenericGroupManager* ConstructCachedChunkObjectManagerBaseThunk(void* pLevelMode,
		void* pEntityLookup,
		void* pFormationManager);
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
	void DeleteGroup(CGenericGroup* pGroup);
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





// MACINTOSH: CCollectableManager::Restart()
// MACINTOSH: CCollectableManager::Process()
// STUB: LEMBALL 0x00422550
// RefreshActiveCollChunkObjects

// RefreshActiveMineChunkObjects
// EmitMineChunkManagerRenderEntries


// MACINTOSH: CPaintGunManager::Process()
// MACINTOSH: CPaintGunManager::GetViewData(CViewData*)
// MACINTOSH: CMoverManager::GetViewData(CViewData*)

#endif
