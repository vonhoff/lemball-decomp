#include "ENGINE/CORE/VSINIT.H"

#include <string.h>

extern void* g_pVariantResourceEntryManager;

struct LevelScreenInputEvent {
	short m_nType00;
	short m_nReserved02;
	int m_nReserved04;
	int m_nReserved08;
	int m_nReserved0C;
	int m_nReserved10;
};

struct LevelScreenInputEventSink {
	virtual void ReservedSlot0(void) = 0;
	virtual void ReservedSlot1(void) = 0;
	virtual void DispatchLevelScreenInputEvent(const LevelScreenInputEvent* pEvent) = 0;
};

struct LevelScreenManagedEntitySelectionView {
	unsigned char m_abReserved000[0x974];
	LevelScreenInputEventSink* m_pInputEventSink974;
	unsigned char m_abReserved978[0xd0];
	int m_nPendingSelectionA48;
	unsigned short m_nPendingSelectionVariantA4C;
};

struct VariantResourceEntryManagerView {
	unsigned char m_abReserved00[0x10];
	int m_nSelectionMode10;
};

// Macintosh: C2D::CancelMoves()
// FUNCTION: LEMBALL 0x00437250
void LEMBALL_FASTCALL ClearActiveEntityChildrenPending(LevelScreenManagedEntitySelectionView* pScreen)
{
	LevelScreenInputEvent Event;
	Event.m_nType00 = 3;
	memset(&Event.m_nReserved04, 0, 0x10);
	pScreen->m_pInputEventSink974->DispatchLevelScreenInputEvent(&Event);
	pScreen->m_nPendingSelectionVariantA4C = 0;
	pScreen->m_nPendingSelectionA48 = 0;
	((VariantResourceEntryManagerView*) g_pVariantResourceEntryManager)->m_nSelectionMode10 = 0x25;
}

// Macintosh: C2D::NextGroup()
// FUNCTION: LEMBALL 0x004372a0
void LEMBALL_FASTCALL SelectNextReadyManagedEntityFromLevelScreen(LevelScreenManagedEntitySelectionView* pScreen)
{
	LevelScreenInputEvent Event;
	Event.m_nType00 = 7;
	memset(&Event.m_nReserved04, 0, 0x10);
	pScreen->m_pInputEventSink974->DispatchLevelScreenInputEvent(&Event);
	pScreen->m_nPendingSelectionVariantA4C = 0;
	pScreen->m_nPendingSelectionA48 = 0;
	((VariantResourceEntryManagerView*) g_pVariantResourceEntryManager)->m_nSelectionMode10 = 0x1b;
}

// Macintosh: C2D::PrevGroup()
// FUNCTION: LEMBALL 0x004372f0
void LEMBALL_FASTCALL SelectPreviousReadyManagedEntityFromLevelScreen(LevelScreenManagedEntitySelectionView* pScreen)
{
	LevelScreenInputEvent Event;
	Event.m_nType00 = 6;
	memset(&Event.m_nReserved04, 0, 0x10);
	pScreen->m_pInputEventSink974->DispatchLevelScreenInputEvent(&Event);
	pScreen->m_nPendingSelectionVariantA4C = 0;
	pScreen->m_nPendingSelectionA48 = 0;
	((VariantResourceEntryManagerView*) g_pVariantResourceEntryManager)->m_nSelectionMode10 = 0x1b;
}
