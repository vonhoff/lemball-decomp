#include "ENGINE/CORE/COMMON.H"

struct LevelListEntryArrayView {
	unsigned char m_abReserved00[0x18];
	int m_cEntries18;
};

struct LevelDrawTextListOwnerView {
	unsigned char m_abReserved00[0x7c];
	LevelListEntryArrayView* m_pPrimaryEntries7C;
	LevelListEntryArrayView* m_pSecondaryEntries80;

	int CountEntryArraysWithData(void);
};

// FUNCTION: LEMBALL 0x0045d970
int LevelDrawTextListOwnerView::CountEntryArraysWithData(void)
{
	int count;

	count = 0;
	if (m_pPrimaryEntries7C->m_cEntries18 != 0) {
		count = 1;
	}
	if (m_pSecondaryEntries80->m_cEntries18 != 0) {
		++count;
	}
	return count;
}
