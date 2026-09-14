#include "TargetFramedButton.h"

#include "../Graphics/ClipRect.h"
#include "../Graphics/Line.h"

// FUNCTION: LEMBALL 0x00468b80
void TargetFramedButton::InitializeFramePrimitives()
{
	m_frameLine = new Line[1];
	m_gdiFlags++;
	m_frameRects = new ClipRect[4];
	m_gdiFlags += 4;
}
