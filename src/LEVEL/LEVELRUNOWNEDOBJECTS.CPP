#include "ENGINE/CORE/VSINIT.H"

struct LevelOwnedObjectView {
	virtual void Destroy(unsigned int nFlags) = 0;
};

struct LevelOwnedObjectManagerView {
	unsigned char m_abReserved00[0x30];
	LevelOwnedObjectView* m_apOwnedObjects30[8];
	int m_nOwnedObjectCount50;

	void ReleaseOwnedObjects(void);
};

struct LevelModeOwnedObjectManagerView {
	unsigned char m_abReserved00[0x1c4];
	LevelOwnedObjectManagerView* m_pOwnedObjectManager1C4;

	void ClearManagerOwnedObjects(void);
};

// FUNCTION: LEMBALL 0x0040cbc0
void LevelOwnedObjectManagerView::ReleaseOwnedObjects(void)
{
	int i;

	m_nOwnedObjectCount50 = 0;
	for (i = 0; i < 8; ++i) {
		if (m_apOwnedObjects30[i] != 0) {
			m_apOwnedObjects30[i]->Destroy(1);
			m_apOwnedObjects30[i] = 0;
		}
	}
}

// FUNCTION: LEMBALL 0x004133c0
void LevelModeOwnedObjectManagerView::ClearManagerOwnedObjects(void)
{
	m_pOwnedObjectManager1C4->ReleaseOwnedObjects();
}
