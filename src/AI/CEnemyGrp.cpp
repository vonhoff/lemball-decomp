#include "ENGINE/CORE/WIN32.H"

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nVariant, unsigned short nStateId);
};

struct EnmyManagedEntityGroupMemberView {};
struct CompositeLevelChunkChildMemberView {};

typedef void (EnmyManagedEntityGroupMemberView::*RequestStateProc)(int nStateId);
typedef void (CompositeLevelChunkChildMemberView::*ServiceChildProc)(void);

struct EnmyManagedEntityGroupVtableView {
	void* m_apReserved00[78];
	RequestStateProc m_pRequestState138;
};

struct CompositeLevelChunkChildVtableView {
	void* m_apReserved00[5];
	ServiceChildProc m_pService14;
};

struct CompositeLevelChunkChildView {
	CompositeLevelChunkChildVtableView* m_pVtable00;
};

struct EnmyManagedEntityGroupView : LevelChunkObjectBaseView {
	EnmyManagedEntityGroupVtableView* m_pVtable00;
	char m_abReserved04[0x120];
	int m_cChildren124;
	CompositeLevelChunkChildView* m_apChildren128[10];

	EnmyManagedEntityGroupView* ConstructEnmyManagedEntityGroup(int nType,
																unsigned short nVariant,
																unsigned short nStateId);
};

extern void* g_LEVELVT_EnmyManagedEntityGroupVtable[16];

// FUNCTION: LEMBALL 0x0041deb0
int LEMBALL_FASTCALL ServiceEnmyManagedEntityGroup(EnmyManagedEntityGroupView* pGroup)
{
	CompositeLevelChunkChildView** ppChild;
	CompositeLevelChunkChildView* pChild;
	int iChild;

	iChild = 0;
	(((EnmyManagedEntityGroupMemberView*) pGroup)->*pGroup->m_pVtable00->m_pRequestState138)(0x18);
	if (pGroup->m_cChildren124 > 0) {
		ppChild = pGroup->m_apChildren128;
		do {
			pChild = *ppChild++;
			++iChild;
			(((CompositeLevelChunkChildMemberView*) pChild)->*pChild->m_pVtable00->m_pService14)();
		} while (iChild < pGroup->m_cChildren124);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00420ae0
EnmyManagedEntityGroupView* EnmyManagedEntityGroupView::ConstructEnmyManagedEntityGroup(int nType,
																						unsigned short nVariant,
																						unsigned short nStateId)
{
	InitializeLevelChunkObjectBase(nType, nVariant, nStateId);
	*(void**) this = g_LEVELVT_EnmyManagedEntityGroupVtable;
	return this;
}
