#include "ENGINE/CORE/VSINIT.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

struct DoorChunkObjectRecordView {
	unsigned char m_abData[0x14c];
};

struct DoorChunkObjectManagerView {
	unsigned char m_abReserved00[0x34];
	int m_nObjectCount34;
	unsigned char m_abReserved38[4];
	DoorChunkObjectRecordView* m_pObjects3C;

	unsigned short GetObjectSlotId(int nIndex);
};

// FUNCTION: LEMBALL 0x0040e600
unsigned short DoorChunkObjectManagerView::GetObjectSlotId(int nIndex)
{
	if (nIndex < m_nObjectCount34) {
		return GetManagedEntitySlotIdThunk((int) (unsigned long) &m_pObjects3C[nIndex]);
	}
	return 0xffff;
}
