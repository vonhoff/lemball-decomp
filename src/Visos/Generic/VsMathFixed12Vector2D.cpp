#include "Visos/Generic/VsMathFixed12Vector2D.h"

// LINKERILT: LEMBALL 0x004035ad
VsMathFixed12Vector2D* VsMathFixed12Vector2D::InitializeFromPixelsThunk(int nX, int nY)
{
	return InitializeFromPixels(nX, nY);
}

// FUNCTION: LEMBALL 0x0044b640
VsMathFixed12Vector2D* VsMathFixed12Vector2D::InitializeFromPixels(int nX, int nY)
{
	m_nX = nX << 12;
	m_nY = nY << 12;
	return this;
}
