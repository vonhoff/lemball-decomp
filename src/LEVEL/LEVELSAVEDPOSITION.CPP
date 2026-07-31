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
	char m_abReservedBE[0x6a];
	int m_nGmobState128;
	char m_abReserved12C[0x0c];
	int m_nSavedPositionX138;
	int m_nSavedPositionY13C;
	int m_nSavedPositionZ140;
	short m_nLinkedVariant144;

	void ResetToSavedPosition(void);
	void ResetVariant(void);
	void ResetLinkedVariant(void);
	void ResetGmobState(void);
};

struct GmobType14SavedPositionView {
	char m_abReserved00[0xbc];
	short m_nVariantBC;
	char m_abReservedBE[0x86];
	int m_nLinkedVariant144;
	char m_abReserved148[0x14];
	short m_nType14Variant15C;

	void ResetType14State(void);
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

// FUNCTION: LEMBALL 0x0041cca0
void SavedPositionChunkObjectView::ResetGmobState(void)
{
	ResetToSavedPosition();
	m_nGmobState128 = 0x18;
}

// FUNCTION: LEMBALL 0x0041d100
void GmobType14SavedPositionView::ResetType14State(void)
{
	((SavedPositionChunkObjectView*) this)->ResetToSavedPosition();
	m_nType14Variant15C = 0;
	m_nLinkedVariant144 = 0;
	m_nVariantBC = 0;
}

struct LevelChunkObjectRuntimeStateView {
	void ResetRuntimeStateThunk(void);
};

struct ManagedEntityGroupChildrenView {
	char m_abReserved00[0x124];
	int m_nChildCount124;
	void* m_apChildren128[10];

	void ClearChildren(void);
};

// FUNCTION: LEMBALL 0x0041de80
void ManagedEntityGroupChildrenView::ClearChildren(void)
{
	int i;

	((LevelChunkObjectRuntimeStateView*) this)->ResetRuntimeStateThunk();
	for (i = 0; i < 10; ++i) {
		m_apChildren128[i] = 0;
	}
	m_nChildCount124 = 0;
}
