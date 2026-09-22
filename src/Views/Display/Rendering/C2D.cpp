#include "../C2D.h"

#include "../../../Visos/Graphics/CGDI.h"
#include "../../../Visos/Graphics/CSurface.h"
#include "../../Animation/CLemmingAnimsManager.h"

#include <stdlib.h>
#include <string.h>

// FUNCTION: LEMBALL 0x0043f620
void C2D::DrawObjects()
{
	SetOrigin();
	m_bitmapCount = 0;
	m_lemmingAnims->m_drawFlags = 0x40000;
	int scrollX = abs((int) m_scrollDeltaX);
	int scrollY = abs((int) m_scrollDeltaY);
	CVsRect borders[4];

	if (m_scrollPending != 0 && m_redrawPending == 0 && scrollX < m_clipSize.m_x && scrollY < m_clipSize.m_y) {
		unsigned char direction = (unsigned char) ((m_scrollDeltaX < 0) | (m_scrollDeltaY < 0 ? 2 : 0));
		CVsRect exposed[2];
		short destinationX = 0;
		short destinationY = 0;
		CVsRect retained;
		switch ((unsigned int) direction) {
		case 0:
			retained = CVsRect((short) scrollX,
							   (short) scrollY,
							   m_clipSize.m_x - (short) scrollX,
							   m_clipSize.m_y - (short) scrollY);
			if (scrollY != 0) {
				exposed[0] = CVsRect(0, 0, m_clipSize.m_x, (short) scrollY);
			}
			if (scrollX != 0) {
				exposed[1] = CVsRect(0, (short) scrollY, (short) scrollX, m_clipSize.m_y - (short) scrollY);
			}
			break;
		case 1:
			destinationX = (short) scrollX;
			destinationY = 0;
			retained = CVsRect(0, (short) scrollY, m_clipSize.m_x - (short) scrollX, m_clipSize.m_y - (short) scrollY);
			if (scrollY != 0) {
				exposed[0] = CVsRect(0, 0, m_clipSize.m_x, (short) scrollY);
			}
			if (scrollX != 0) {
				exposed[1] = CVsRect(m_clipSize.m_x - (short) scrollX,
									 (short) scrollY,
									 (short) scrollX,
									 m_clipSize.m_y - (short) scrollY);
			}
			if (m_clipOffsetX > 0 || m_clipOffsetY > 0) {
				borders[0] = CVsRect((short) m_clipOffsetX - (short) scrollX,
									 (short) m_clipOffsetY,
									 (short) scrollX,
									 m_clipSize.m_y + (short) scrollY);
				borders[1] = CVsRect((short) m_clipOffsetX,
									 m_clipSize.m_y + (short) m_clipOffsetY,
									 m_clipSize.m_x,
									 (short) scrollY);
			}
			break;
		case 2:
			destinationY = (short) scrollY;
			destinationX = 0;
			retained = CVsRect((short) scrollX, 0, m_clipSize.m_x - (short) scrollX, m_clipSize.m_y - (short) scrollY);
			if (scrollY != 0) {
				exposed[0] = CVsRect(0, m_clipSize.m_y - (short) scrollY, m_clipSize.m_x, (short) scrollY);
			}
			if (scrollX != 0) {
				exposed[1] = CVsRect(0, 0, (short) scrollX, m_clipSize.m_y - (short) scrollY);
			}
			break;
		case 3:
			destinationX = (short) scrollX;
			destinationY = (short) scrollY;
			retained = CVsRect(0, 0, m_clipSize.m_x - (short) scrollX, m_clipSize.m_y - (short) scrollY);
			if (scrollY != 0) {
				exposed[0] = CVsRect(0, m_clipSize.m_y - (short) scrollY, m_clipSize.m_x, (short) scrollY);
			}
			if (scrollX != 0) {
				exposed[1] =
					CVsRect(m_clipSize.m_x - (short) scrollX, 0, (short) scrollX, m_clipSize.m_y - (short) scrollY);
			}
			break;
		}
		m_screenScroll.m_destination.m_x = destinationX;
		m_screenScroll.m_destination.m_y = destinationY;
		m_screenScroll.m_rect = retained;
		m_screenScroll.Draw(m_gdi);
		CVsRect* strip = exposed;
		do {
			if ((int) strip->m_width * (int) strip->m_height != 0) {
				CVsRect copyRect = *strip;
				m_scrollCopyToBackBuffs[0].m_destination = copyRect;
				memset(&m_scrollCopyToBackBuffs[0].m_sourceX, 0, sizeof(CVsPoint));
				m_scrollCopyToBackBuffs[0].Draw(m_gdi);
				DrawClippedRectangle(*strip);
			}
			strip++;
		} while (strip < exposed + 2);
	}
	else if (m_scrollPending != 0 || m_redrawPending != 0) {
		m_gdi->m_renderTarget->ResetScroll();
		CVsRect fullRect(0, 0, (CVsSize*) &m_clipSize);
		if (m_redrawPending != 0) {
			fullRect = CVsRect(0, 0, m_clipSize.m_x, m_clipSize.m_y);
		}
		m_copyToBackBuff.m_destination = fullRect;
		memset(&m_copyToBackBuff.m_sourceX, 0, sizeof(CVsPoint));
		m_copyToBackBuff.Draw(m_gdi);
		DrawClippedRectangle(CVsRect(0, 0, m_clipSize.m_x, m_clipSize.m_y));
		m_redrawPending = 0;
	}
	m_scrollPending = 0;
	DrawObjectsZBuff();
	m_lemmingAnims->m_drawFlags = 0;
}
