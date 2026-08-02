#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

struct LevelManagedEntityTargetView {
	unsigned char m_abReserved00[0x8c];
	int m_fActiveTarget8C;
};

struct LevelManagedEntityGroupTargetView {
	unsigned char m_abReserved00[0x170];
	LevelManagedEntityTargetView* m_pActiveTarget170;

	void ClearActiveTarget(void);
};

// FUNCTION: LEMBALL 0x00414130
void LevelManagedEntityGroupTargetView::ClearActiveTarget(void)
{
	if (m_pActiveTarget170 != 0) {
		m_pActiveTarget170->m_fActiveTarget8C = 0;
	}
	m_pActiveTarget170 = 0;
}
