#include "../../CSurface.h"

#include "Visos/Graphics/CPVScrollableSurface.h"
// FUNCTION: LEMBALL 0x00475f60
int CSurface::ClipCirclePoint(int p_x, int p_y)
{
	int clipX;
	int clipRight;
	int clipY;
	int clipBottom;

	clipX = CPVScrollableSurface::m_clipRect.m_x;
	if (clipX <= p_x) {
		clipRight = CPVScrollableSurface::m_clipRect.m_width + clipX - 1;
		if (p_x <= clipRight) {
			clipY = CPVScrollableSurface::m_clipRect.m_y;
			if (clipY <= p_y) {
				clipBottom = CPVScrollableSurface::m_clipRect.m_height + clipY - 1;
				if (p_y <= clipBottom) {
					return 1;
				}
			}
		}
	}
	return 0;
}
