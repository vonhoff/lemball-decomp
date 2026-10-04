#include "CSurface.h"

#include "Visos/Graphics/Primitives/CPoint.h"
#include "Visos/Graphics/Primitives/CSolidRect.h"
#include "Visos/Graphics/Primitives/CLine.h"
#include "Visos/Graphics/Primitives/CCircle.h"
#include "Visos/Graphics/Primitives/CFilledCircle.h"
#include "Visos/Graphics/Primitives/CClipRect.h"
#include "Visos/Math/CVSPoint.h"
#include "CGDIDevice.h"
#include <stdlib.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

enum eCircleClipResult {
	CIRCLE_OUTSIDE_CLIP = 1,
	CIRCLE_FULLY_INSIDE_CLIP = 2,
	CIRCLE_PARTIALLY_CLIPPED = 3
};

#include "CSurface.h"

#include "Visos/Math/CVSRect.h"

enum eLineClipRegionFlag {
	LINE_CLIP_REGION_INSIDE = 0,
	LINE_CLIP_REGION_LEFT = 0x01,
	LINE_CLIP_REGION_RIGHT = 0x02,
	LINE_CLIP_REGION_TOP = 0x04,
	LINE_CLIP_REGION_BOTTOM = 0x08
};

inline unsigned int CSurface::ClipCode(int p_x, int p_y)
{
	unsigned int code = 0;
	if (p_x < m_clipRect.m_x) {
		code |= 1;
	}
	else if (p_x > m_clipRect.m_x + m_clipRect.m_width - 1) {
		code |= 2;
	}
	if (p_y < m_clipRect.m_y) {
		code |= 4;
	}
	else if (p_y > m_clipRect.m_y + m_clipRect.m_height - 1) {
		code |= 8;
	}
	return code;
}

// FUNCTION: LEMBALL 0x0046cbe0
void CSurface::Blit(class CClipRect* p_clipRect)
{
	CVSRect* clip = &m_clipRect;
	short clipRight;

	if ((p_clipRect->m_flags & CClipRect::CLIP_EXPAND_BOUNDS) == 0) {
		const short* coords;

		clip->m_width = p_clipRect->m_bounds.m_width;
		clip->m_height = p_clipRect->m_bounds.m_height;
		if (&p_clipRect->m_bounds.m_width != NULL) {
			coords = &p_clipRect->m_bounds.m_x;
		}
		else {
			coords = NULL;
		}
		clip->m_x = *coords;
		clip->m_y = coords[1];
	}
	else if ((int) p_clipRect->m_bounds.m_width * (int) p_clipRect->m_bounds.m_height != 0) {
		clipRight = clip->m_x;
		if (p_clipRect->m_bounds.m_x < clipRight) {
			clip->m_width = (short) (clip->m_width + (clipRight - p_clipRect->m_bounds.m_x));
			clip->m_x = p_clipRect->m_bounds.m_x;
		}
		short clipX;
		short primitiveWidth = p_clipRect->m_bounds.m_width;
		clipX = clip->m_x;
		short right = clip->m_width;
		right += clipX;
		short primitiveRight = p_clipRect->m_bounds.m_x;
		primitiveRight += primitiveWidth;
		if (right < primitiveRight) {
			primitiveWidth -= clipX;
			primitiveWidth += p_clipRect->m_bounds.m_x;
			clip->m_width = primitiveWidth;
		}
		if (p_clipRect->m_bounds.m_y < clip->m_y) {
			clip->m_height = (short) (clip->m_height + (clip->m_y - p_clipRect->m_bounds.m_y));
			clip->m_y = p_clipRect->m_bounds.m_y;
		}
		short primitiveY;
		short primitiveHeight = p_clipRect->m_bounds.m_height;
		primitiveY = p_clipRect->m_bounds.m_y;
		short clipBottom = (short) (clip->m_height + clip->m_y);
		short primitiveBottom = (short) (primitiveY + primitiveHeight);
		if (clipBottom < primitiveBottom) {
			clip->m_height = (short) ((primitiveHeight - clip->m_y) + primitiveY);
		}
	}
	CSurface* parent = m_parentSurface;
	if ((CSurface*) g_pGdiHelperTarget != parent && (p_clipRect->m_flags & CClipRect::CLIP_IGNORE_PARENT) == 0) {
		CVSRect* parentClip = &parent->m_clipRect;
		short parentX = parentClip->m_x;
		CVSRect* childClip = &m_clipRect;
		clipRight = childClip->m_x;
		if (clipRight < parentX) {
			childClip->m_width = (short) (childClip->m_width + (clipRight - parentX));
			childClip->m_x = parentClip->m_x;
		}
		short parentWidth;
		short childX = childClip->m_x;
		parentX = parentClip->m_x;
		parentWidth = parentClip->m_width;
		if ((short) (parentWidth + parentX) < (short) (childClip->m_width + childX)) {
			parentX -= childX;
			parentX += parentWidth;
			childClip->m_width = parentX;
		}
		if (childClip->m_y < parentClip->m_y) {
			childClip->m_height = (short) (childClip->m_height + (childClip->m_y - parentClip->m_y));
			childClip->m_y = parentClip->m_y;
		}
		if ((short) (parentClip->m_y + parentClip->m_height) < (short) (childClip->m_height + childClip->m_y)) {
			childClip->m_height = (short) ((parentClip->m_height - childClip->m_y) + parentClip->m_y);
		}
		if (childClip->m_width <= 0 || childClip->m_height <= 0) {
			childClip->m_height = 0;
			childClip->m_width = 0;
			childClip->m_y = 0;
			childClip->m_x = 0;
		}
	}
}

// FUNCTION: LEMBALL 0x00474fd0
void CSurface::Blit(CPoint* p_point)
{
	const short& x = p_point->m_x;
	int colour = p_point->m_colour;
	if (m_clipRect.m_x <= x && x < (short) (m_clipRect.m_width + m_clipRect.m_x)) {
		if (m_clipRect.m_y <= p_point->m_y && p_point->m_y < (short) (m_clipRect.m_height + m_clipRect.m_y)) {
			*((unsigned char*) m_lines[p_point->m_y] + x) = (unsigned char) colour;
			CVSRect rect(p_point->m_x, p_point->m_y, 1, 1);
			AddToChangeList(rect);
		}
	}
}

// FUNCTION: LEMBALL 0x00475080
void CSurface::Blit(CSolidRect* p_rect)
{
	BlitRect(*p_rect->GetBounds(), p_rect->m_colour);
}

// FUNCTION: LEMBALL 0x004750c0
void CSurface::Blit(CLine* p_line)
{
	int y2 = p_line->m_end.m_y;
	int x2 = p_line->m_end.m_x;
	int colourValue = p_line->m_colour;
	const int& colour = colourValue;
	int y1 = p_line->m_start.m_y;
	int x1 = p_line->m_start.m_x;
	if (x2 < x1) {
		int x = x1;
		x1 = x2;
		x2 = x;
		int y = y1;
		y1 = y2;
		y2 = y;
	}
	CSurface* surface = this;
	if (surface->LineClip(x1, y1, x2, y2) != 0) {
		return;
	}
	int endY = y2;
	int y = y1;
	int x = x1;
	int dx = x2 - x;
	int remaining = endY - y;
	int stepY = 1;
	int absDy;
	if (remaining < 0) {
		stepY = SURFACE_STEP_BACKWARD;
		absDy = -remaining;
	}
	else {
		absDy = remaining;
	}
	if (absDy < dx) {
		int doubleDx = dx * 2;
		int doubleDy = absDy * 2;
		remaining = dx;
		int err = 0;
		if (0 < dx) {
			do {
				remaining = remaining - 1;
				x = x + 1;
				err = err + doubleDy;
				*((unsigned char*) m_lines[y] + (x - 1)) = (unsigned char) colour;
				if (dx < err) {
					y = y + stepY;
					err = err - doubleDx;
				}
			} while (remaining != 0);
		}
	}
	else {
		int doubleDy = absDy * 2;
		int doubleDx = dx * 2;
		int err = 0;
		if (stepY != 1) {
			remaining = y - endY;
		}
		if (0 < remaining) {
			do {
				remaining = remaining - 1;
				err = err + doubleDx;
				*((unsigned char*) m_lines[y] + x) = (unsigned char) colour;
				y = y + stepY;
				if (absDy < err) {
					x = x + 1;
					err = err - doubleDy;
				}
			} while (remaining != 0);
		}
	}
	surface->AddToChangeList(
		CVSRect((short) x1, (short) min(y1, y2), (short) (x2 - x1 + 1), (short) (abs(y2 - y1) + 1)));
}

// FUNCTION: LEMBALL 0x00475290
void CSurface::Blit(CCircle* p_circle)
{
	int colour = p_circle->m_colour;
	int centreY = p_circle->m_y;
	int centreX = p_circle->m_x;
	int radius = abs((int) p_circle->m_radius);
	int clipResult = ClipCircle(centreX, centreY, radius);
	if (clipResult != CIRCLE_OUTSIDE_CLIP) {
		switch (clipResult) {
		case CIRCLE_FULLY_INSIDE_CLIP: {
			int curX = 0;
			int curY = radius;
			int err = 0;
			int step = 1;
			int errLimit = radius * 2 - 1;
			*((unsigned char*) m_lines[centreY + radius] + centreX) = (unsigned char) colour;
			*((unsigned char*) m_lines[centreY - radius] + centreX) = (unsigned char) colour;
			unsigned char* centrePixel = (unsigned char*) m_lines[centreY] + centreX;
			centrePixel[radius] = (unsigned char) colour;
			*((unsigned char*) m_lines[centreY] - radius + centreX) = (unsigned char) colour;
			while (curX < curY) {
				curX++;
				err += step;
				step += 2;
				if (err * 2 > errLimit) {
					curY--;
					err -= errLimit;
					errLimit -= 2;
				}
				if (curX <= curY) {
					DrawCircleSymmetricPoints(centreX, centreY, curX, curY, colour);
					if (curX < curY) {
						DrawCircleSymmetricPoints(centreX, centreY, curY, curX, colour);
					}
				}
			}
			break;
		}
		case CIRCLE_PARTIALLY_CLIPPED:
			DrawClippedCircleOutline(centreX, centreY, radius, colour);
			break;
		}
		centreX -= radius;
		int boundY = centreY - radius;
		int boundW = radius * 2 + 1;
		int boundH = boundW;
		if (centreX < (int) m_clipRect.m_x) {
			boundW += centreX - m_clipRect.m_x;
			centreX = m_clipRect.m_x;
		}
		if ((int) m_clipRect.m_x + (int) m_clipRect.m_width - 1 < centreX + boundW) {
			boundW = (m_clipRect.m_x + m_clipRect.m_width) - centreX;
		}
		if (boundY < (int) m_clipRect.m_y) {
			boundH += boundY - m_clipRect.m_y;
			boundY = m_clipRect.m_y;
		}
		if ((int) m_clipRect.m_y + (int) m_clipRect.m_height - 1 < boundY + boundH) {
			boundH = (m_clipRect.m_y + m_clipRect.m_height) - boundY;
		}
		AddToChangeList(CVSRect((short) centreX, (short) boundY, (short) boundW, (short) boundH));
	}
}

// FUNCTION: LEMBALL 0x00475490
void CSurface::Blit(CFilledCircle* p_circle)
{
	int colour = p_circle->m_colour;
	int y = p_circle->m_y;
	int x = p_circle->m_x;
	int radius = abs((int) p_circle->m_radius);
	int clipResult = ClipCircle(x, y, radius);
	if (clipResult != CIRCLE_OUTSIDE_CLIP) {
		switch (clipResult) {
		case CIRCLE_FULLY_INSIDE_CLIP: {
			int curRadius;
			int curX = 0;
			curRadius = radius;
			int err = 0;
			int step = 1;
			int errLimit = radius * 2 - 1;
			*((unsigned char*) m_lines[y + radius] + x) = (unsigned char) colour;
			*((unsigned char*) m_lines[y - radius] + x) = (unsigned char) colour;
			memset((unsigned char*) m_lines[y] - radius + x, colour, radius * 2 + 1);
			if (radius > 1) {
				while (curX < curRadius) {
					int changed = 0;
					curX++;
					err += step;
					step += 2;
					if (err * 2 > errLimit) {
						changed = 1;
						curRadius--;
						err -= errLimit;
						errLimit -= 2;
					}
					if (curX <= curRadius) {
						if (changed) {
							DrawCircleSpans(x, y, curX, curRadius, colour);
						}
						if (curX < curRadius) {
							DrawCircleSpans(x, y, curRadius, curX, colour);
						}
					}
				}
			}
			break;
		}
		case CIRCLE_PARTIALLY_CLIPPED:
			DrawClippedFilledCircle(x, y, radius, colour);
			break;
		}
		int minX = x - radius;
		int minY = y - radius;
		int width = radius * 2 + 1;
		int height = width;
		if (minX < (int) m_clipRect.m_x) {
			width += minX - m_clipRect.m_x;
			minX = m_clipRect.m_x;
		}
		if (m_clipRect.m_x + m_clipRect.m_width - 1 < minX + width) {
			width = m_clipRect.m_x + m_clipRect.m_width - minX;
		}
		if (minY < (int) m_clipRect.m_y) {
			height += minY - m_clipRect.m_y;
			minY = m_clipRect.m_y;
		}
		if (m_clipRect.m_y + m_clipRect.m_height - 1 < minY + height) {
			height = m_clipRect.m_y + m_clipRect.m_height - minY;
		}
		AddToChangeList(CVSRect((short) minX, (short) minY, (short) width, (short) height));
	}
}

// FUNCTION: LEMBALL 0x004756e0
void CSurface::BlitRect(CVSRect p_rect, int p_colour)
{
	short storage[4];
	CVSRect& clipped = *(CVSRect*) storage;
	clipped.m_width = clipped.m_height = 0;
	clipped.m_x = clipped.m_y = 0;
	if (ClipRect(p_rect, &clipped)) {
		if (clipped.m_width <= 0 || clipped.m_height <= 0) {
			return;
		}
		p_rect.m_width = clipped.m_width;
		p_rect.m_height = clipped.m_height;
	}
	for (int y = 0; y < p_rect.m_height; y++) {
		memset((unsigned char*) m_lines[p_rect.m_y + y] + p_rect.m_x, p_colour, p_rect.m_width);
	}
	AddToChangeList(p_rect);
}

// FUNCTION: LEMBALL 0x004757a0
int CSurface::LineClip(int& p_x1, int& p_y1, int& p_x2, int& p_y2)
{
	unsigned int code1;
	unsigned int code2;
	int coordinate;
	int dx;
	int dy;
	int x1;
	int y1;
	int x2;
	int y2;

	if (m_clipRect.m_height <= 0 || m_clipRect.m_width <= 0) {
		return 1;
	}
	code1 = LINE_CLIP_REGION_INSIDE;
	coordinate = p_x1;
	if (coordinate < m_clipRect.m_x) {
		code1 = LINE_CLIP_REGION_LEFT;
	}
	else if (m_clipRect.m_width + m_clipRect.m_x - 1 < coordinate) {
		code1 = LINE_CLIP_REGION_RIGHT;
	}
	coordinate = p_y1;
	if (coordinate < m_clipRect.m_y) {
		code1 |= LINE_CLIP_REGION_TOP;
	}
	else if (m_clipRect.m_height + m_clipRect.m_y - 1 < coordinate) {
		code1 |= LINE_CLIP_REGION_BOTTOM;
	}
	code2 = LINE_CLIP_REGION_INSIDE;
	coordinate = p_x2;
	if (coordinate < m_clipRect.m_x) {
		code2 = LINE_CLIP_REGION_LEFT;
	}
	else if (m_clipRect.m_width + m_clipRect.m_x - 1 < coordinate) {
		code2 = LINE_CLIP_REGION_RIGHT;
	}
	coordinate = p_y2;
	if (coordinate < m_clipRect.m_y) {
		code2 |= LINE_CLIP_REGION_TOP;
	}
	else if (m_clipRect.m_height + m_clipRect.m_y - 1 < coordinate) {
		code2 |= LINE_CLIP_REGION_BOTTOM;
	}
	if ((code1 | code2) != LINE_CLIP_REGION_INSIDE) {
		do {
			if ((code1 & code2) != 0) {
				return 1;
			}
			x2 = p_x2;
			x1 = p_x1;
			dx = x2 - x1;
			y2 = p_y2;
			y1 = p_y1;
			dy = y2 - y1;
			if (code1 != LINE_CLIP_REGION_INSIDE) {
				if ((code1 & LINE_CLIP_REGION_LEFT) == 0) {
					if ((code1 & LINE_CLIP_REGION_RIGHT) != 0) {
						p_y1 = y1 + ((m_clipRect.m_x + m_clipRect.m_width - 1 - x1) * dy) / dx;
						p_x1 = m_clipRect.m_x + m_clipRect.m_width - 1;
					}
					else {
						if ((code1 & LINE_CLIP_REGION_TOP) == 0) {
							if ((code1 & LINE_CLIP_REGION_BOTTOM) != 0) {
								p_x1 = x1 + ((m_clipRect.m_y + m_clipRect.m_height - 1 - y1) * dx) / dy;
								p_y1 = m_clipRect.m_y + m_clipRect.m_height - 1;
							}
						}
						else {
							p_x1 = x1 + ((m_clipRect.m_y - y1) * dx) / dy;
							p_y1 = m_clipRect.m_y;
						}
					}
				}
				else {
					p_y1 = y1 + ((m_clipRect.m_x - x1) * dy) / dx;
					p_x1 = m_clipRect.m_x;
				}
				code1 = LINE_CLIP_REGION_INSIDE;
				if (p_x1 < m_clipRect.m_x) {
					code1 = LINE_CLIP_REGION_LEFT;
				}
				else if (m_clipRect.m_width + m_clipRect.m_x - 1 < p_x1) {
					code1 = LINE_CLIP_REGION_RIGHT;
				}
				if (p_y1 < m_clipRect.m_y) {
					code1 |= LINE_CLIP_REGION_TOP;
				}
				else if (m_clipRect.m_height + m_clipRect.m_y - 1 < p_y1) {
					code1 |= LINE_CLIP_REGION_BOTTOM;
				}
			}
			else {
				if ((code2 & LINE_CLIP_REGION_LEFT) == 0) {
					if ((code2 & LINE_CLIP_REGION_RIGHT) != 0) {
						p_y2 = y2 + ((m_clipRect.m_x + m_clipRect.m_width - 1 - x2) * dy) / dx;
						p_x2 = m_clipRect.m_x + m_clipRect.m_width - 1;
					}
					else {
						if ((code2 & LINE_CLIP_REGION_TOP) == 0) {
							if ((code2 & LINE_CLIP_REGION_BOTTOM) != 0) {
								p_x2 = x2 + ((m_clipRect.m_y + m_clipRect.m_height - 1 - y2) * dx) / dy;
								p_y2 = m_clipRect.m_y + m_clipRect.m_height - 1;
							}
						}
						else {
							p_x2 = x2 + ((m_clipRect.m_y - y2) * dx) / dy;
							p_y2 = m_clipRect.m_y;
						}
					}
				}
				else {
					p_y2 = y2 + ((m_clipRect.m_x - x2) * dy) / dx;
					p_x2 = m_clipRect.m_x;
				}
				code2 = LINE_CLIP_REGION_INSIDE;
				if (p_x2 < m_clipRect.m_x) {
					code2 = LINE_CLIP_REGION_LEFT;
				}
				else if (m_clipRect.m_width + m_clipRect.m_x - 1 < p_x2) {
					code2 = LINE_CLIP_REGION_RIGHT;
				}
				if (p_y2 < m_clipRect.m_y) {
					code2 |= LINE_CLIP_REGION_TOP;
				}
				else if (m_clipRect.m_height + m_clipRect.m_y - 1 < p_y2) {
					code2 |= LINE_CLIP_REGION_BOTTOM;
				}
			}
		} while ((code1 | code2) != LINE_CLIP_REGION_INSIDE);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00475bc0
int CSurface::ClipCircle(int p_centreX, int p_centreY, int p_radius)
{
	int left = p_centreX - p_radius;
	int top = p_centreY - p_radius;
	int right = p_centreX + p_radius;
	int bottom = p_centreY + p_radius;

	if (m_clipRect.m_height <= 0 || m_clipRect.m_width <= 0) {
		return CIRCLE_OUTSIDE_CLIP;
	}
	if (right >= m_clipRect.m_x && left <= ((int) m_clipRect.m_x + (int) m_clipRect.m_width - 1) &&
		bottom >= m_clipRect.m_y && top <= ((int) m_clipRect.m_y + (int) m_clipRect.m_height - 1)) {
		if (left >= m_clipRect.m_x && right <= ((int) m_clipRect.m_x + (int) m_clipRect.m_width - 1) &&
			top >= m_clipRect.m_y && bottom <= ((int) m_clipRect.m_y + (int) m_clipRect.m_height - 1)) {
			return CIRCLE_FULLY_INSIDE_CLIP;
		}
		return CIRCLE_PARTIALLY_CLIPPED;
	}
	return CIRCLE_OUTSIDE_CLIP;
}

// FUNCTION: LEMBALL 0x00475ce0
void CSurface::DrawClippedCircleOutline(int p_centreX, int p_centreY, int p_radius, unsigned char p_colour)
{
	int x = 0;
	int y = p_radius;
	int err = 0;
	int step = 1;
	int errLimit = y * 2 - 1;

	if (m_clipRect.m_x <= p_centreX && m_clipRect.m_x + m_clipRect.m_width - 1 >= p_centreX) {
		if (m_clipRect.m_y <= p_centreY + p_radius &&
			m_clipRect.m_y + m_clipRect.m_height - 1 >= p_centreY + p_radius) {
			*((unsigned char*) m_lines[p_centreY + p_radius] + p_centreX) = p_colour;
		}
	}
	if (m_clipRect.m_x <= p_centreX && m_clipRect.m_x + m_clipRect.m_width - 1 >= p_centreX) {
		if (m_clipRect.m_y <= p_centreY - p_radius &&
			m_clipRect.m_y + m_clipRect.m_height - 1 >= p_centreY - p_radius) {
			*((unsigned char*) m_lines[p_centreY - p_radius] + p_centreX) = p_colour;
		}
	}
	if (m_clipRect.m_x <= p_centreX + p_radius && m_clipRect.m_x + m_clipRect.m_width - 1 >= p_centreX + p_radius) {
		int clipY = m_clipRect.m_y;
		if (clipY <= p_centreY && clipY + m_clipRect.m_height - 1 >= p_centreY) {
			unsigned char* destination = (unsigned char*) m_lines[p_centreY] + p_centreX;
			destination[p_radius] = p_colour;
		}
	}
	if (m_clipRect.m_x <= p_centreX - p_radius && m_clipRect.m_x + m_clipRect.m_width - 1 >= p_centreX - p_radius) {
		int clipY = m_clipRect.m_y;
		if (clipY <= p_centreY && clipY + m_clipRect.m_height - 1 >= p_centreY) {
			*((unsigned char*) m_lines[p_centreY] + p_centreX - p_radius) = p_colour;
		}
	}
	if (p_radius <= 0) {
		return;
	}
	do {
		x = x + 1;
		err = err + step;
		step = step + 2;
		if (errLimit < err * 2) {
			y = y - 1;
			err = err - errLimit;
			errLimit = errLimit - 2;
		}
		if (y < x) {
			continue;
		}
		if (ClipCirclePoint(p_centreX + x, p_centreY + y) != 0) {
			unsigned char* destination = (unsigned char*) m_lines[p_centreY + y] + p_centreX;
			destination[x] = p_colour;
		}
		if (ClipCirclePoint(p_centreX - x, p_centreY + y) != 0) {
			*((unsigned char*) m_lines[p_centreY + y] + p_centreX - x) = p_colour;
		}
		if (ClipCirclePoint(p_centreX + x, p_centreY - y) != 0) {
			unsigned char* destination = (unsigned char*) m_lines[p_centreY - y] + p_centreX;
			destination[x] = p_colour;
		}
		if (ClipCirclePoint(p_centreX - x, p_centreY - y) != 0) {
			*((unsigned char*) m_lines[p_centreY - y] + p_centreX - x) = p_colour;
		}
		if (y > x) {
			DrawClippedCirclePoint(p_centreX, p_centreY, y, x, p_colour);
		}
	} while (y > x);
}

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

// FUNCTION: LEMBALL 0x00475fb0
void CSurface::DrawClippedCirclePoint(int p_centreX,
									  int p_centreY,
									  int p_xOffset,
									  int p_yOffset,
									  unsigned char p_colour)
{
	if (m_clipRect.m_x <= (p_centreX + p_xOffset) &&
		(p_centreX + p_xOffset) <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		if (m_clipRect.m_y <= (p_centreY + p_yOffset) &&
			(p_centreY + p_yOffset) <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[(p_centreY + p_yOffset)] + (p_centreX + p_xOffset)) = p_colour;
		}
	}
	if (m_clipRect.m_x <= (p_centreX - p_xOffset) &&
		(p_centreX - p_xOffset) <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		if (m_clipRect.m_y <= (p_centreY + p_yOffset) &&
			(p_centreY + p_yOffset) <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[(p_centreY + p_yOffset)] + (p_centreX - p_xOffset)) = p_colour;
		}
	}
	if (m_clipRect.m_x <= (p_centreX + p_xOffset) &&
		(p_centreX + p_xOffset) <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		if (m_clipRect.m_y <= (p_centreY - p_yOffset) &&
			(p_centreY - p_yOffset) <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[(p_centreY - p_yOffset)] + (p_centreX + p_xOffset)) = p_colour;
		}
	}
	if (m_clipRect.m_x <= (p_centreX - p_xOffset) &&
		(p_centreX - p_xOffset) <= m_clipRect.m_x + m_clipRect.m_width - 1) {
		if (m_clipRect.m_y <= (p_centreY - p_yOffset) &&
			(p_centreY - p_yOffset) <= m_clipRect.m_y + m_clipRect.m_height - 1) {
			*((unsigned char*) m_lines[(p_centreY - p_yOffset)] + (p_centreX - p_xOffset)) = p_colour;
		}
	}
}

// FUNCTION: LEMBALL 0x00476100
void CSurface::DrawCircleSpans(int p_centreX, int p_centreY, int p_halfWidth, int p_yOffset, int p_colour)
{
	int spanWidth = p_halfWidth * 2 + 1;
	unsigned char* negativeSpan = (unsigned char*) m_lines[p_centreY - p_yOffset] + p_centreX - p_halfWidth;
	memset((unsigned char*) m_lines[p_centreY + p_yOffset] + p_centreX - p_halfWidth, p_colour, spanWidth);
	memset(negativeSpan, p_colour, spanWidth);
}

// FUNCTION: LEMBALL 0x00476190
void CSurface::DrawClippedFilledCircle(int p_centreX, int p_centreY, int p_radius, int p_colour)
{
	int x;
	int err;
	int step;
	int errLimit;
	int poleY;
	int x1;
	int x2;
	int changed;
	int doubleErr;
	int yTop;
	int yBottom;
	int clipY;
	int xLeft;
	int xRight;
	int clipX;
	x = 0;
	err = 0;
	step = 1;
	errLimit = p_radius * 2 - 1;

	if (p_centreX >= m_clipRect.m_x && p_centreX <= (m_clipRect.m_width + m_clipRect.m_x - 1)) {
		clipY = m_clipRect.m_y;
		poleY = p_centreY + p_radius;
		if (poleY >= clipY && poleY <= (m_clipRect.m_height + clipY - 1)) {
			*((unsigned char*) m_lines[poleY] + p_centreX) = (unsigned char) p_colour;
		}
	}
	if (p_centreX >= m_clipRect.m_x && p_centreX <= (m_clipRect.m_width + m_clipRect.m_x - 1)) {
		if ((p_centreY - p_radius) >= m_clipRect.m_y &&
			(p_centreY - p_radius) <= (m_clipRect.m_height + m_clipRect.m_y - 1)) {
			*((unsigned char*) m_lines[(p_centreY - p_radius)] + p_centreX) = (unsigned char) p_colour;
		}
	}
	if (p_centreY >= m_clipRect.m_y && p_centreY <= (m_clipRect.m_height + m_clipRect.m_y - 1)) {
		x1 = p_centreX - p_radius;
		x2 = p_centreX + p_radius;
		if (x1 < m_clipRect.m_x) {
			x1 = m_clipRect.m_x;
		}
		if ((m_clipRect.m_width + m_clipRect.m_x - 1) < x2) {
			x2 = m_clipRect.m_width + m_clipRect.m_x - 1;
		}
		memset((unsigned char*) m_lines[p_centreY] + x1, p_colour, x2 - x1 + 1);
	}

	if (p_radius > 1) {
		while (x < p_radius) {
			changed = 0;
			x++;
			err += step;
			step += 2;
			doubleErr = err * 2;
			if (errLimit < doubleErr) {
				p_radius--;
				changed = 1;
				err -= errLimit;
				errLimit -= 2;
			}
			if (x <= p_radius) {
				if (changed != 0) {
					yTop = p_centreY - p_radius;
					yBottom = p_centreY + p_radius;
					clipY = m_clipRect.m_y;
					if (yTop <= (m_clipRect.m_height + clipY - 1) && yBottom >= clipY) {
						xLeft = p_centreX - x;
						xRight = p_centreX + x;
						clipX = m_clipRect.m_x;
						if (clipX <= xRight && (m_clipRect.m_width + clipX - 1) >= xLeft) {
							if (xRight > (m_clipRect.m_width + clipX - 1)) {
								xRight = m_clipRect.m_width + clipX - 1;
							}
							if (xLeft < clipX) {
								xLeft = clipX;
							}
							if (yTop >= clipY) {
								memset((unsigned char*) m_lines[yTop] + xLeft, p_colour, xRight - xLeft + 1);
							}
							if (yBottom <= (m_clipRect.m_height + m_clipRect.m_y - 1)) {
								memset((unsigned char*) m_lines[yBottom] + xLeft, p_colour, xRight - xLeft + 1);
							}
						}
					}
				}
				if (x < p_radius) {
					FilledCircleClipPoints(p_centreX, p_centreY, p_radius, x, p_colour);
				}
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00476470
void CSurface::FilledCircleClipPoints(int p_centreX, int p_centreY, int p_xOffset, int p_yOffset, int p_colour)
{
	int y1 = p_centreY - p_yOffset;
	int y2 = p_centreY + p_yOffset;
	if (y1 <= (m_clipRect.m_height + m_clipRect.m_y - 1) && m_clipRect.m_y <= y2) {
		int x1 = p_centreX - p_xOffset;
		int x2 = p_centreX + p_xOffset;
		int clipX = m_clipRect.m_x;
		if (clipX <= x2 && x1 <= (m_clipRect.m_width + clipX - 1)) {
			if ((m_clipRect.m_width + clipX - 1) < x2) {
				x2 = m_clipRect.m_width + clipX - 1;
			}
			if (x1 < clipX) {
				x1 = clipX;
			}
			if (m_clipRect.m_y <= y1) {
				memset((unsigned char*) m_lines[y1] + x1, p_colour, x2 - x1 + 1);
			}
			if (y2 <= (m_clipRect.m_height + m_clipRect.m_y - 1)) {
				memset((unsigned char*) m_lines[y2] + x1, p_colour, x2 - x1 + 1);
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00476580
bool CSurface::ClipRect(CVSRect& p_rect, CVSRect* p_clipped)
{
	bool clipped = false;
	short clipX = m_clipRect.m_x;
	short rectX = p_rect.m_x;

	if ((short) (m_clipRect.m_width + clipX) < rectX || (short) (m_clipRect.m_height + m_clipRect.m_y) < p_rect.m_y ||
		(short) (p_rect.m_width + rectX) < clipX || (short) (p_rect.m_height + p_rect.m_y) < m_clipRect.m_y) {
		return true;
	}

	if (m_clipRect.m_x > p_rect.m_x) {
		p_clipped->m_x = m_clipRect.m_x - p_rect.m_x;
		short clippedX = m_clipRect.m_x;
		p_rect.m_x = clippedX;
		clipped = true;
		p_rect.m_width -= p_clipped->m_x;
	}

	if (m_clipRect.m_y > p_rect.m_y) {
		p_clipped->m_y = m_clipRect.m_y - p_rect.m_y;
		short clippedY = m_clipRect.m_y;
		p_rect.m_y = clippedY;
		clipped = true;
		p_rect.m_height -= p_clipped->m_y;
	}

	if ((short) (p_rect.m_x + p_rect.m_width) > (short) (m_clipRect.m_x + m_clipRect.m_width)) {
		p_clipped->m_width = (m_clipRect.m_x + m_clipRect.m_width) - p_rect.m_x;
		p_rect.m_width = (m_clipRect.m_x - p_rect.m_x) + m_clipRect.m_width;
		clipped = true;
	}
	else {
		p_clipped->m_width = p_rect.m_width;
	}

	if ((short) (p_rect.m_y + p_rect.m_height) > (short) (m_clipRect.m_y + m_clipRect.m_height)) {
		p_clipped->m_height = (m_clipRect.m_y + m_clipRect.m_height) - p_rect.m_y;
		p_rect.m_height = (m_clipRect.m_y - p_rect.m_y) + m_clipRect.m_height;
		return true;
	}

	p_clipped->m_height = p_rect.m_height;
	return clipped;
}
