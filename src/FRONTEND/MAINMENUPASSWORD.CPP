#include "FRONTEND/MENUSEL.H"

class GameCompactZrleOverlay {
public:
	void RebuildSourceRect(int fForce);
};

class GamePasswordKeyOverlayTableView {
public:
	void RebuildSourceRects(void);

private:
	unsigned char m_abReserved00[0x424];
	GameCompactZrleOverlay* m_apKeyOverlays424[12];
};

// FUNCTION: LEMBALL 0x00451e40
void GamePasswordKeyOverlayTableView::RebuildSourceRects(void)
{
	int i;
	GameCompactZrleOverlay** ppOverlay;
	GameCompactZrleOverlay* pOverlay;

	ppOverlay = m_apKeyOverlays424;
	i = 12;
	do {
		pOverlay = *ppOverlay;
		++ppOverlay;
		pOverlay->RebuildSourceRect(1);
	} while (--i != 0);
}
