#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

struct LevelManagedEntityActiveGroupView {
	char m_abReserved00[0x110];
	int m_anProfileValues110[5];
	int m_nField124;

	int GetProfileValueByIndex(int iValue);
};

struct LevelModeProfileOwnerView {
	char m_abReserved00[0xc0];
	int m_anProfileRangeValuesC0[39];
	LevelManagedEntityActiveGroupView* m_pActiveGroup15C;

	int GetPlasChunkManagerField124(void);
	int GetLevelModeProfileValueByIndex(int iValue);
	int GetProfileRangeValue(int iValue);
	void SetProfileRangeValue(int nValue, int iValue);
};

int LevelManagedEntityActiveGroupView::GetProfileValueByIndex(int iValue)
{
	return m_anProfileValues110[iValue];
}

// FUNCTION: LEMBALL 0x00413120
int LevelModeProfileOwnerView::GetPlasChunkManagerField124(void)
{
	return m_pActiveGroup15C->m_nField124;
}

// FUNCTION: LEMBALL 0x00413160
int LevelModeProfileOwnerView::GetLevelModeProfileValueByIndex(int iValue)
{
	return m_pActiveGroup15C->GetProfileValueByIndex(iValue);
}

// FUNCTION: LEMBALL 0x00413290
int LevelModeProfileOwnerView::GetProfileRangeValue(int iValue)
{
	return m_anProfileRangeValuesC0[iValue];
}

// FUNCTION: LEMBALL 0x004132a0
void LevelModeProfileOwnerView::SetProfileRangeValue(int nValue, int iValue)
{
	m_anProfileRangeValuesC0[iValue] = nValue;
}
