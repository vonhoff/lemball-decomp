#include "../../C2D.h"

#include "../../../Target/SpriteGroundLookup.h"
#include "AI/Base/Coord3d.h"
#include "AI/Navigation/CAI.h"
#include "AI/Objects/LiftEndpointRecord.h"
#include "Map/Base/CMap.h"

#include <string.h>

// FUNCTION: LEMBALL 0x0043f060
void C2D::MarkGroundAnimAndLiftBounds()
{
	char* scratch = m_groundClipScratch;
	int count = m_ai->ExportGroundAnimRecords((Coord3d*) scratch);
	if (count > 0) {
		int remaining = count;
		Coord3d* coordinate = (Coord3d*) scratch;
		do {
			int screenX = coordinate->m_x;
			int screenY = coordinate->m_y;
			int groundHeight = coordinate->m_z;
			C2D* view = this;
			view->m_map->GameToScreen(screenX, screenY);
			screenX -= view->m_viewOriginX;
			screenY -= view->m_viewOriginY;
			screenY -= groundHeight;
			short pixelX = (short) screenX - 16;
			short pixelY = (short) screenY - 24;
			short rectHeight = (short) groundHeight + 48;
			SpriteGroundLookup* lookup = m_spriteGroundLookup;
			int cellX = (short) (pixelX / 16);
			int cellY = (short) (pixelY / 16);
			int columns = (pixelX + 32 - 1) / 16 - cellX + 1;
			int rows = (pixelY + rectHeight - 1) / 16 - cellY + 1;
			int width = lookup->m_width;
			int height;
			if (width > cellX && ((height = lookup->m_height), height > cellY)) {
				if (cellX < 0) {
					columns += cellX;
					cellX = 0;
				}
				if (cellY < 0) {
					rows += cellY;
					cellY = 0;
				}
				if (cellX + columns >= width) {
					columns = width - cellX;
				}
				if (cellY + rows >= height) {
					rows = height - cellY;
				}
				if (columns > 0 && rows > 0) {
					int offset = cellX + width * cellY;
					unsigned char* maskA = lookup->m_maskA + offset;
					unsigned char* maskB = lookup->m_maskB + offset;
					for (; rows != 0; rows--) {
						memset(maskA, 1, columns);
						memset(maskB, 1, columns);
						maskA += lookup->m_width;
						maskB += lookup->m_width;
					}
				}
			}
			coordinate++;
		} while (--remaining != 0);
	}

	count = m_ai->ExportLiftEndpointRecords((LiftEndpointRecord*) scratch);
	if (count > 0) {
		int remaining = count;
		LiftEndpointRecord* endpoints = (LiftEndpointRecord*) scratch;
		do {
			int groundHeight = endpoints->m_start.m_z;
			int startX = endpoints->m_start.m_x;
			int startY = endpoints->m_start.m_y;
			m_map->GameToScreen(startX, startY);
			startX -= m_viewOriginX;
			startY -= m_viewOriginY;
			startY -= groundHeight;

			int rightX = endpoints->m_end.m_x;
			int rightY = endpoints->m_start.m_y;
			m_map->GameToScreen(rightX, rightY);
			rightX -= m_viewOriginX;
			rightY -= m_viewOriginY;
			rightY -= groundHeight;

			int endX = endpoints->m_end.m_x;
			int endY = endpoints->m_end.m_y;
			m_map->GameToScreen(endX, endY);
			endX -= m_viewOriginX;
			endY -= m_viewOriginY;
			endY -= groundHeight;

			int leftX = endpoints->m_start.m_x;
			int leftY = endpoints->m_end.m_y;
			m_map->GameToScreen(leftX, leftY);
			leftX -= m_viewOriginX;
			leftY -= m_viewOriginY;
			leftY -= groundHeight;

			short pixelX = (short) leftX - 16;
			short pixelY = (short) startY - 24;
			short rectWidth = (short) rightX - (short) leftX + 32;
			short rectHeight = (short) groundHeight - (short) startY + (short) endY + 32;
			SpriteGroundLookup* lookup = m_spriteGroundLookup;
			int cellX = (short) (pixelX / 16);
			int cellY = (short) (pixelY / 16);
			int columns = (pixelX + rectWidth - 1) / 16 - cellX + 1;
			int rows = (pixelY + rectHeight - 1) / 16 - cellY + 1;
			int width = lookup->m_width;
			int height;
			if (width > cellX && ((height = lookup->m_height), height > cellY)) {
				if (cellX < 0) {
					columns += cellX;
					cellX = 0;
				}
				if (cellY < 0) {
					rows += cellY;
					cellY = 0;
				}
				if (cellX + columns >= width) {
					columns = width - cellX;
				}
				if (cellY + rows >= height) {
					rows = height - cellY;
				}
				if (columns > 0 && rows > 0) {
					int offset = cellX + width * cellY;
					unsigned char* maskA = lookup->m_maskA + offset;
					unsigned char* maskB = lookup->m_maskB + offset;
					for (; rows != 0; rows--) {
						memset(maskA, 1, columns);
						memset(maskB, 1, columns);
						maskA += lookup->m_width;
						maskB += lookup->m_width;
					}
				}
			}
			endpoints++;
		} while (--remaining != 0);
	}
}
