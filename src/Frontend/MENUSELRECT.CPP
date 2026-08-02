#include "ENGINE/CORE/COMMON.H"

class GameCompactZrleOverlay {
public:
	void RebuildSourceRect(int fForce);
};

class GameSingleCompactZrleOverlayOwner {
	unsigned char m_abReserved00[0x4c];
	GameCompactZrleOverlay* m_pOverlay4C;

public:
	void RebuildSourceRect(int fForce);
};

// FUNCTION: LEMBALL 0x0044f270
void GameSingleCompactZrleOverlayOwner::RebuildSourceRect(int fForce)
{
	if (m_pOverlay4C != 0) {
		m_pOverlay4C->RebuildSourceRect(fForce);
	}
}
