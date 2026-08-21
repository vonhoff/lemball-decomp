#ifndef LEMBALL_VSMATHFIXED12VECTOR2D_H
#define LEMBALL_VSMATHFIXED12VECTOR2D_H

class VsMathFixed12Vector2D {
public:
	int m_nX;
	int m_nY;

	VsMathFixed12Vector2D* InitializeFromPixels(int nX, int nY);
	VsMathFixed12Vector2D* InitializeFromPixelsThunk(int nX, int nY);
};

#endif
