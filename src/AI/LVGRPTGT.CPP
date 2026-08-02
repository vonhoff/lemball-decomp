#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

struct LevelManagedEntityTargetView {
	unsigned char m_abReserved00[0x8c];
	int m_fActiveTarget8C;
};

struct CPlayerLemmingGroup {
	unsigned char m_abReserved00[0x170];
	LevelManagedEntityTargetView* m_pActiveTarget170;

	void Delete(void);
};

// MACINTOSH: CPlayerLemmingGroup::Delete()
// FUNCTION: LEMBALL 0x00414130
void CPlayerLemmingGroup::Delete(void)
{
	if (m_pActiveTarget170 != 0) {
		m_pActiveTarget170->m_fActiveTarget8C = 0;
	}
	m_pActiveTarget170 = 0;
}
