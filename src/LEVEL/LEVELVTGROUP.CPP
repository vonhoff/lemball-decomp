#include "ENGINE/CORE/VSINIT.H"

struct LevelPoint3 {
	int m_nX;
	int m_nY;
	int m_nZ;
};

struct LevelManagedEntityChildIterator {
	void* GetFirstChildThunk(void);
};

struct LevelManagedEntityChildView {
	char m_abReserved00[0x9c];
	LevelPoint3 m_position9C;
};

struct LevelManagedEntityActiveGroupView {
	LevelManagedEntityChildIterator* GetFirstActiveEntityThunk(void);
	int GetActiveEntityPosition(LevelPoint3* pPosition);
};

// Split from LEVELVT.CPP to preserve compiler state in the original translation unit.

// FUNCTION: LEMBALL 0x004185f0
int LevelManagedEntityActiveGroupView::GetActiveEntityPosition(LevelPoint3* pPosition)
{
	LevelManagedEntityChildIterator* pEntity;
	LevelManagedEntityChildView* pChild;

	pEntity = GetFirstActiveEntityThunk();
	if (pEntity == NULL) {
		return 0;
	}
	pChild = (LevelManagedEntityChildView*) pEntity->GetFirstChildThunk();
	if (pChild == NULL) {
		return 0;
	}
	pPosition->m_nX = pChild->m_position9C.m_nX;
	pPosition->m_nY = pChild->m_position9C.m_nY;
	pPosition->m_nZ = pChild->m_position9C.m_nZ;
	return 1;
}
