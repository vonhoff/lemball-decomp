#include "ENGINE/CORE/VSINIT.H"

struct LevelManagedEntityActiveGroupView {
	char m_abReserved00[0x110];
	int m_anProfileValues110[1];

	int GetProfileValueByIndex(int iValue);
};

struct LevelModeProfileOwnerView {
	char m_abReserved00[0x15c];
	LevelManagedEntityActiveGroupView* m_pActiveGroup15C;

	int GetLevelModeProfileValueByIndex(int iValue);
};

int LevelManagedEntityActiveGroupView::GetProfileValueByIndex(int iValue)
{
	return m_anProfileValues110[iValue];
}

// FUNCTION: LEMBALL 0x00413160
int LevelModeProfileOwnerView::GetLevelModeProfileValueByIndex(int iValue)
{
	return m_pActiveGroup15C->GetProfileValueByIndex(iValue);
}
