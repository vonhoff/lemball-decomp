#include "ENGINE/CORE/VSINIT.H"

struct DoorChunkObjectManagerView {
	unsigned short GetObjectSlotId(int nIndex);
};

struct LevelModeChunkManagerView {
	unsigned char m_abReserved00[0x190];
	DoorChunkObjectManagerView* m_pDoorManager190;

	unsigned short GetDoorSlotId(int nIndex);
};

// FUNCTION: LEMBALL 0x00413000
unsigned short LevelModeChunkManagerView::GetDoorSlotId(int nIndex)
{
	return m_pDoorManager190->GetObjectSlotId(nIndex);
}
