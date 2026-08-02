#include "Visos/Generic/VSMath.h"

class VsWingRenderBlitRect {
public:
	short m_nX;
	short m_nY;
	short m_nWidth;
	short m_nHeight;

	VsWingRenderBlitRect* Initialize(short nWidth, short nHeight, const VsMathPoint2D* pPoint);
};

// FUNCTION: LEMBALL 0x00478b80
VsWingRenderBlitRect* VsWingRenderBlitRect::Initialize(short nWidth, short nHeight, const VsMathPoint2D* pPoint)
{
	m_nX = pPoint->m_nX;
	m_nY = pPoint->m_nY;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	return this;
}
