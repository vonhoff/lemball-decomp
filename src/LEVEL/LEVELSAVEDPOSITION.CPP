#include "ENGINE/CORE/VSINIT.H"

void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);

struct SavedPositionChunkObjectView {
	char m_abReserved00[0x9c];
	int m_nPositionX9C;
	int m_nPositionYA0;
	int m_nPositionZA4;
	char m_abReservedA8[0x10];
	int m_nStateB8;
	short m_nVariantBC;
	char m_abReservedBE[0x7a];
	int m_nSavedPositionX138;
	int m_nSavedPositionY13C;
	int m_nSavedPositionZ140;
	short m_nLinkedVariant144;

	void ResetToSavedPosition(void);
	void ResetVariant(void);
	void ResetLinkedVariant(void);
};

// FUNCTION: LEMBALL 0x0041c6c0
void SavedPositionChunkObjectView::ResetToSavedPosition(void)
{
	ResetManagedEntityRuntimeStateThunk(this);
	m_nPositionX9C = m_nSavedPositionX138;
	m_nPositionYA0 = m_nSavedPositionY13C;
	m_nPositionZA4 = m_nSavedPositionZ140;
	m_nStateB8 = 0x18;
}

// FUNCTION: LEMBALL 0x0041c700
void SavedPositionChunkObjectView::ResetVariant(void)
{
	ResetToSavedPosition();
	m_nVariantBC = 0;
}

// FUNCTION: LEMBALL 0x0041ca90
void SavedPositionChunkObjectView::ResetLinkedVariant(void)
{
	ResetToSavedPosition();
	m_nLinkedVariant144 = 0;
}
