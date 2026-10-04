#include "C2D.h"

#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Animation/AnimSpecialEntry.h"
#include "Gameplay/Animation/CAnimSpecial.h"
#include "Gameplay/Characters/CPlayerLemming.h"
#include "Gameplay/Hazards/CSlinky.h"
#include "Gameplay/Mechanisms/CSwitch.h"
#include "Gameplay/Objects/CViewData.h"
#include "Game/CDemo.h"
#include "Game/CGame.h"
#include "Game/GameMain.h"
#include "Game/GameTime.h"
#include "Level/CLevelLoader.h"
#include "Frontend/CBaseFrontendProcess.h"
#include "Frontend/Loading/CFrontendResourceLoader.h"
#include "Map/CMap.h"
#include "Network/CNetworkManager.h"
#include "Visos/Queues/CBaseQueue.h"
#include "CObjSq.h"
#include "Visos/Text/CTextManager.h"
#include "Gameplay/Geometry/Facing.h"
#include "Visos/Sorting/VsSort.h"
#include "Visos/Time/VsTime.h"
#include "Visos/Graphics/Palettes/CBasePalManager.h"
#include "Platform/Windows/Graphics/CCursor.h"
#include "Visos/Graphics/Primitives/CGDI.h"
#include "Visos/Controls/CHotAreaList.h"
#include "Visos/Graphics/Surfaces/CSurface.h"
#include "Visos/Graphics/Primitives/CZRLE.h"
#include "../../Visos/Network/CBaseNetwork.h"
#include "../../Visos/Network/NetworkConstants.h"
#include "../../Visos/Network/NetworkMode.h"
#include "Visos/Resources/Types/CResFONT.h"
#include "Visos/Resources/Types/CResPALETTE.h"
#include "../../Visos/Resources/ResourceLimits.h"
#include "../Animation/CLemmingAnimsManager.h"
#include "../Input/CPadToButton.h"
#include "../Panel/CPanel.h"
#include "../Pause/CPauseWindow.h"
#include "../Sound/CSoundView.h"
#include "ObjectClipGrid.h"
#include "SpriteGroundLookup.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "CMain2DDisplay.h"
#include "CPBButton.h"
#include "Frontend/FlowProcesses.h"
#include "Visos/Math/FixedPoint.h"

#include <new.h>
#include <string.h>

#include "Game/CGameStatus.h"

#include "Visos/Graphics/Surfaces/CChangeList.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Geometry/C3DVector.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Game/SoundEffects.h"
#include "Visos/Math/CVSPoint.h"
#include "Visos/Math/CVSRect.h"
#include "Visos/Math/CVSSize.h"
#include "Visos/Queues/Message.h"
#include "Visos/Input/CBaseCursor.h"
#include "Visos/Graphics/Palettes/CBaseRemap.h"
#include "Visos/Graphics/Primitives/CClipRect.h"
#include "Visos/Graphics/Primitives/CCopyToBackBuff.h"
#include "Visos/Graphics/Primitives/CDrawingMark.h"
#include "Visos/Controls/CHotAreaHandler.h"
#include "Visos/Graphics/Primitives/CPopActive.h"
#include "Visos/Graphics/Primitives/CPushActive.h"
#include "Visos/Graphics/Primitives/CSolidRect.h"

class CBaseQueueHandler;
class CRemap;

#include "Visos/Math/CFixed.h"

#include "Gameplay/Geometry/tCoord3d.h"
#include "Gameplay/Mechanisms/LiftEndpointRecord.h"

#include "Visos/Streams/CVSOStream.h"
#include "Visos/Network/CConnect.h"
#include "Visos/Network/NetworkMode.h"
#include "Views/Animation/CLemmingAnimsManager.h"
#include "Views/Panel/CPanel.h"

#include <stddef.h>

#include "Views/Sound/CSoundView.h"

#include "Visos/Resources/Manifest.h"

#include <stdlib.h>

extern char* g_demoText;

extern const unsigned int* g_styleObjectClip;

extern const short g_slinkyOffsets[4][2];

extern const short g_sheepOffset[2];

// GLOBAL: LEMBALL 0x00497218
static const int g_clipMapStepXByOrientation[4] = {1, -1, -1, 1};

// GLOBAL: LEMBALL 0x00497228
static const int g_clipMapStepYByOrientation[4] = {-1, -1, 1, 1};

// GLOBAL: LEMBALL 0x00497238
static const int g_clipNeighborStepXByOrientation[4] = {0, 1, 0, -1};

// GLOBAL: LEMBALL 0x00497248
static const int g_clipNeighborStepYByOrientation[4] = {1, 0, -1, 0};

// GLOBAL: LEMBALL 0x00497258
static const int g_clipRowStepXByOrientation[4] = {1, 1, -1, -1};

// GLOBAL: LEMBALL 0x00497268
static const int g_clipRowStepYByOrientation[4] = {1, -1, -1, 1};

// FUNCTION: LEMBALL 0x004368f0
CVSRect* C2D::GetClipRectangle()
{
	static CVSRect g_clipRectangle;
	CVSRect rectangle((short) m_clipOffsetX, (short) m_clipOffsetY, m_clipSize.m_x, m_clipSize.m_y);
	CVSRect* source = &rectangle;
	g_clipRectangle.m_width = source->m_width;
	g_clipRectangle.m_height = source->m_height;
	g_clipRectangle.m_x = source->m_x;
	g_clipRectangle.m_y = source->m_y;
	return &g_clipRectangle;
}

// FUNCTION: LEMBALL 0x00438500
void C2D::SetClipSize()
{
	int width;
	int height;
	int count;
	SpriteGroundLookup* lookup;
	CResFONT* font;
	short clipSizeX;
	short translatedX;

	if (g_nZoomEnabled != 0 && g_nCompactPrimaryContextLayout == 0) {
		m_clipSize.m_x = 0x140;
		m_clipSize.m_y = 0xf0;
		m_clipOffsetX = 0xa0;
		m_clipOffsetY = 0x78;
	}
	else {
		m_clipSize.m_x = m_viewSize.m_x;
		m_clipSize.m_y = m_viewSize.m_y;
		m_clipOffsetX = 0;
		m_clipOffsetY = 0;
	}
	if (g_pDemo != NULL) {
		short demoOffsetY = (short) m_clipOffsetY;
		CDemo* demo = g_pDemo;
		demo->m_offsetX = (short) m_clipOffsetX;
		demo->m_offsetY = demoOffsetY;
	}
	lookup = m_spriteGroundLookup;
	if (lookup != NULL) {
		width = (m_clipSize.m_x + GROUND_BLOCK_PIXEL_MASK) / GROUND_BLOCK_PIXEL_SIZE;
		height = (m_clipSize.m_y + GROUND_BLOCK_PIXEL_MASK) / GROUND_BLOCK_PIXEL_SIZE;
		if (lookup->m_width != width || lookup->m_height != height) {
			if (lookup->m_maskA != NULL) {
				operator delete(lookup->m_maskA);
				lookup->m_maskA = NULL;
			}
			if (lookup->m_maskB != NULL) {
				operator delete(lookup->m_maskB);
				lookup->m_maskB = NULL;
			}
			lookup->m_width = (short) width;
			lookup->m_height = (short) height;
			lookup->m_maskA =
				(unsigned char*) operator new((unsigned int) lookup->m_width*(unsigned int) lookup->m_height);
			lookup->m_maskB =
				(unsigned char*) operator new((unsigned int) lookup->m_width*(unsigned int) lookup->m_height);
		}
		count = (int) lookup->m_width * (int) lookup->m_height;
		memset(lookup->m_maskA, 1, count);
		count = (int) lookup->m_width * (int) lookup->m_height;
		memset(lookup->m_maskB, 1, count);
	}
	clipSizeX = m_clipSize.m_x;
	m_spriteGroundLookupRectA.m_width = 0x33;
	translatedX = clipSizeX - 0x43;
	m_clipConfigured = 1;
	m_spriteGroundLookupRectA.m_height = 0x20;
	m_spriteGroundLookupRectB.m_width = 0x60;
	m_spriteGroundLookupRectB.m_x = 0x10;
	m_spriteGroundLookupRectA.m_x = translatedX;
	m_spriteGroundLookupRectA.m_y = 8;
	m_spriteGroundLookupRectB.m_height = 0x20;
	m_spriteGroundLookupRectB.m_y = 8;
	g_nLevelViewportHorizontalRemainder = m_viewSize.m_x - clipSizeX;
	g_nLevelViewportVerticalRemainder = m_viewSize.m_y - m_clipSize.m_y;
	m_redrawPending = 1;
	if (g_pDemo != NULL && g_pDemo->m_demoMode != 0) {
		font = m_textManager->GetFont(0xf8);
		short remainingWidth = m_clipSize.m_x;
		remainingWidth -= font->GetSize(g_demoText, TEXT_ADVANCE_X_POSITIVE).m_width;
		m_demoTextRect.m_y = 0;
		m_demoTextRect.m_x = remainingWidth / 2;
	}
}

// FUNCTION: LEMBALL 0x0043ad40
void C2D::DoClipWidth(int p_mapX, int p_mapY, int p_count)
{
	int mapX = p_mapX;
	int screenY;
	int screenX;
	eObjectType defaultGroundType;
	int defaultGroundData;
	int processed;
	short baseZ;
	int delayed;
	int drawGround;
	CGround* ground;
	int groundStep;
	unsigned short groundData;
	short height;
	unsigned short cliff;
	eObjectType groundType;
	int heightValue;
	int groundY;
	int zOffset;

	screenY = m_clipScreenY;
	screenX = m_clipScreenX;
	defaultGroundType = m_map->m_defaultBlox;
	defaultGroundData = m_map->m_defaultBloxData;
	processed = 0;
	baseZ = ((short) p_mapY + (short) mapX) * 0x40;
	if (baseZ < 0) {
		baseZ = 0;
	}
	m_lemmingAnims->m_primitiveSequence = baseZ;

	if (p_count > 0) {
		do {
			if (mapX >= 0 && p_mapY >= 0 && mapX < m_groundWidth && p_mapY < m_groundHeight) {
				break;
			}
			DrawGround(screenX, screenY, defaultGroundType, defaultGroundData);
			screenX += 0x20;
			mapX += m_clipMapStepX;
			processed++;
			p_mapY += m_clipMapStepY;
		} while (processed < p_count);
	}

	if (processed < p_count) {
		delayed = 0;
		drawGround = 1;
		ground = m_map->m_ground.m_ground + m_map->m_ground.m_width * p_mapY + mapX;
		groundStep = 1 - m_groundWidth;

		for (; p_count > processed && mapX >= 0 && p_mapY >= 0 && mapX < m_groundWidth && p_mapY < m_groundHeight;) {
			if ((ground->m_collision & GROUND_COLLISION_SPECIAL_RENDER) == 0) {
				groundData = ground->m_objectData;
				height = ground->m_height;
				cliff = ground->m_cliff;
				groundType = ground->m_objectType;

				if (height < 0) {
					groundType = defaultGroundType;
					groundData = defaultGroundData;
					height = 0;
				}

				heightValue = height;
				groundY = screenY - heightValue;
				switch (groundType) {
				case TERRAIN_TREE:
					zOffset = 0x20;
					break;
				case TERRAIN_BLOX_1:
					zOffset = 0x10;
					break;
				case TERRAIN_BLOX_2:
					zOffset = 8;
					break;
				case TERRAIN_BLOX_5:
					zOffset = 8;
					break;
				case TERRAIN_BLOX_6:
				case TERRAIN_BLOX_7:
					delayed = 1;
					zOffset = 0;
					break;
				case TERRAIN_ANIM:
				case TERRAIN_FLAME:
				case TERRAIN_ELECTRIC:
				case TERRAIN_CONVEYOR_VARIANT_A:
				case TERRAIN_CONVEYOR_VARIANT_B:
					drawGround = 0;
				case TERRAIN_BLOX_3_SLOPE_SW_STEEP:
					zOffset = 0;
					break;
				case TERRAIN_BLOX_4:
					zOffset = 0;
					break;
				case TERRAIN_BLOX_8_SLOPE_SE_STEEP:
					zOffset = 0;
					break;
				case TERRAIN_BLOX_14_SLOPE_SW_SHALLOW:
					zOffset = 0;
					break;
				case TERRAIN_BLOX_15_SLOPE_SE_SHALLOW:
					zOffset = 0;
					break;
				case TERRAIN_CLIFF_ONLY_GROUND:
				case TERRAIN_EMBERS:
					zOffset = 0;
					break;
				}

				m_lemmingAnims->m_primitiveSequence = (unsigned short) (zOffset + baseZ + height);
				if (delayed == 0) {
					if (height > 0) {
						DrawCliff(screenX, screenY, heightValue, (short) cliff);
					}
					if (drawGround != 0) {
						DrawGround(screenX, groundY, groundType, groundData);
					}
					drawGround = 1;
				}
				if (delayed != 0) {
					if (height > 0) {
						DrawCliff(screenX, screenY, heightValue, (short) cliff);
					}
					if (drawGround != 0) {
						DrawGround(screenX, groundY, groundType, groundData);
					}
					delayed = 0;
					drawGround = 1;
				}
			}

			mapX += m_clipMapStepX;
			p_mapY += m_clipMapStepY;
			screenX += 0x20;
			processed++;
			ground += groundStep;
		}
	}

	m_lemmingAnims->m_primitiveSequence = baseZ;
	if (processed < p_count) {
		processed = p_count - processed;
		do {
			DrawGround(screenX, screenY, defaultGroundType, defaultGroundData);
			screenX += 0x20;
		} while (--processed != 0);
	}
}

// FUNCTION: LEMBALL 0x0043b0e0
void C2D::DoClipWidthSearch(int p_mapX, int p_mapY, int p_count)
{
	int screenX;
	int screenY;
	eObjectType defaultGroundType;
	int defaultGroundData;
	short baseZ;
	int processed;
	int delayed;
	unsigned short groundWidth;
	CGround* ground;
	int groundStep;
	unsigned short groundData;
	short height;
	unsigned short cliff;
	eObjectType groundType;
	int zOffset;
	int heightValue;
	int groundY;
	int remaining;

	screenX = m_clipScreenX;
	screenY = m_clipScreenY;
	defaultGroundType = m_map->m_defaultBlox;
	defaultGroundData = m_map->m_defaultBloxData;
	baseZ = ((short) p_mapY + (short) p_mapX) * 0x40;
	if (baseZ < 0) {
		baseZ = 0;
	}
	processed = 0;
	m_lemmingAnims->m_primitiveSequence = baseZ;

	if (p_count > 0) {
		do {
			if (p_mapX >= 0 && p_mapY >= 0 && p_mapX < m_groundWidth && p_mapY < m_groundHeight) {
				break;
			}
			DrawGround(screenX, screenY, defaultGroundType, defaultGroundData);
			screenX += 0x20;
			processed++;
			p_mapX += m_clipMapStepX;
			p_mapY += m_clipMapStepY;
		} while (processed < p_count);
	}

	if (processed < p_count) {
		delayed = 0;
		groundWidth = m_groundWidth;
		ground = m_map->m_ground.m_ground + m_map->m_ground.m_width * p_mapY + p_mapX;
		groundStep = 1 - groundWidth;

		for (;
			 processed < p_count && p_mapX >= 0 && p_mapY >= 0 && p_mapX < m_groundWidth && p_mapY < m_groundHeight;) {
			if ((ground->m_collision & GROUND_COLLISION_SPECIAL_RENDER) == 0) {
				groundData = ground->m_objectData;
				memcpy(&height, &ground->m_height, sizeof(height));
				cliff = ground->m_cliff;
				groundType = ground->m_objectType;
				if (height < 0) {
					height = 0;
					groundType = defaultGroundType;
					groundData = (unsigned short) defaultGroundData;
				}

				heightValue = height;
				groundY = screenY - heightValue;
				zOffset = 0;
				switch (groundType) {
				case TERRAIN_TREE:
					zOffset = 0x20;
					break;
				case TERRAIN_BLOX_1:
					zOffset = 0x10;
					break;
				case TERRAIN_BLOX_2:
				case TERRAIN_BLOX_5:
					zOffset = 8;
					break;
				}
				m_lemmingAnims->m_primitiveSequence = (unsigned short) (height + baseZ + zOffset);

				switch (groundType) {
				case TERRAIN_TREE:
					if (height > 0) {
						DrawCliff(screenX, screenY, heightValue, (short) cliff);
					}
					DrawGround(screenX, groundY, groundType, groundData);
					break;
				case TERRAIN_BLOX_1:
				case TERRAIN_BLOX_2:
				case TERRAIN_BLOX_3_SLOPE_SW_STEEP:
				case TERRAIN_BLOX_4:
				case TERRAIN_BLOX_5:
				case TERRAIN_BLOX_8_SLOPE_SE_STEEP:
				case TERRAIN_BLOX_14_SLOPE_SW_SHALLOW:
				case TERRAIN_BLOX_15_SLOPE_SE_SHALLOW:
				case TERRAIN_CLIFF_ONLY_GROUND:
				case TERRAIN_EMBERS:
					if (height > 0) {
						DrawCliff(screenX, screenY, heightValue, (short) cliff);
					}
					DrawGround(screenX, groundY, groundType, groundData);
					break;
				case TERRAIN_BLOX_6:
				case TERRAIN_BLOX_7:
					delayed = 1;
					break;
				case TERRAIN_ANIM:
				case TERRAIN_FLAME:
				case TERRAIN_ELECTRIC:
				case TERRAIN_CONVEYOR_VARIANT_A:
				case TERRAIN_CONVEYOR_VARIANT_B:
					if (height > 0) {
						DrawCliff(screenX, screenY, heightValue, (short) cliff);
					}
					break;
				}

				if (delayed != 0) {
					if (height > 0x18) {
						DrawCliff(screenX, screenY, heightValue, (short) cliff);
					}
					DrawGround(screenX, groundY, groundType, groundData);
					delayed = 0;
				}
			}

			p_mapX += m_clipMapStepX;
			p_mapY += m_clipMapStepY;
			screenX += 0x20;
			processed++;
			ground += groundStep;
		}
	}

	m_lemmingAnims->m_primitiveSequence = baseZ;
	if (processed < p_count) {
		remaining = p_count - processed;
		do {
			DrawGround(screenX, screenY, defaultGroundType, defaultGroundData);
			screenX += 0x20;
		} while (--remaining != 0);
	}
}

// FUNCTION: LEMBALL 0x0043b4b0
int C2D::DrawClipData()
{
	// GLOBAL: LEMBALL 0x0049ee28
	static int g_clipDebug = 1;

	if (g_clipDebug != 0) {
		g_clipDebug = 0;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0043b4d0
int C2D::DrawClippedRectangle(const CVSRect& p_rect)
{
	int orientationOffset;
	int x;
	int y;
	int width;
	int height;
	CVSRect clippedRect;
	int left;
	int right;
	int bottom;
	int gameX;
	int gameY;
	unsigned int rowCount;
	int result;
	int neighborStepX;
	int neighborStepY;
	int rowStepX;
	int rowStepY;

	orientationOffset = m_viewOrientation;
	m_clipMapStepX = g_clipMapStepXByOrientation[orientationOffset];
	m_clipMapStepY = g_clipMapStepYByOrientation[orientationOffset];
	neighborStepX = g_clipNeighborStepXByOrientation[orientationOffset];
	neighborStepY = g_clipNeighborStepYByOrientation[orientationOffset];
	rowStepX = g_clipRowStepXByOrientation[orientationOffset];
	rowStepY = g_clipRowStepYByOrientation[orientationOffset];
	m_unk0x1464 = 0;

	width = p_rect.m_width;
	height = p_rect.m_height;
	x = p_rect.m_x;
	y = p_rect.m_y;
	if (x + width > m_clipSize.m_x) {
		width = m_clipSize.m_x - x;
	}
	if (y + height > m_clipSize.m_y) {
		height = m_clipSize.m_y - y;
	}

	clippedRect.m_width = (short) width;
	clippedRect.m_height = (short) height;
	clippedRect.m_x = (short) x;
	clippedRect.m_y = (short) y;
	CClipRect& clipRect = m_clipRects[m_primitiveCount++];
	clipRect.m_bounds.m_width = clippedRect.m_width;
	clipRect.m_bounds.m_height = clippedRect.m_height;
	memcpy(&clipRect.m_bounds.m_x, &clippedRect.m_x, sizeof(short));
	memcpy(&clipRect.m_bounds.m_y, &clippedRect.m_y, sizeof(short));
	clipRect.m_flags = 0;
	clipRect.Draw(m_gdi);

	left = x - 0x10;
	if (left < -0x10) {
		left = -0x10;
	}
	y -= 0x18;
	if (y < -0x18) {
		y = -0x18;
	}
	right = left + width + 0x20;
	if (right > m_clipSize.m_x) {
		right = m_clipSize.m_x;
	}
	bottom = y + height + 0x30;
	if (bottom > m_clipSize.m_y) {
		bottom = m_clipSize.m_y;
	}
	width = right / 0x20 - left / 0x20 + 3;

	m_map->ScreenToGame(m_viewOriginX + left, m_viewOriginY + y, gameX, gameY);
	gameX /= 0x10;
	gameY /= 0x10;
	m_map->GameToScreen(gameX << GROUND_BLOCK_PIXEL_SHIFT,
						gameY << GROUND_BLOCK_PIXEL_SHIFT,
						m_clipScreenX,
						m_clipScreenY);
	m_clipScreenX -= m_viewOriginX;
	m_clipScreenY -= m_viewOriginY;

	if (y < bottom) {
		rowCount = ((unsigned int) (bottom - y) + GROUND_BLOCK_PIXEL_MASK) >> GROUND_BLOCK_PIXEL_SHIFT;
		y += rowCount << GROUND_BLOCK_PIXEL_SHIFT;
		do {
			DoClipWidth(gameX, gameY, width);
			m_clipScreenX -= 0x10;
			m_clipScreenY += 8;
			DoClipWidth(gameX + neighborStepX, gameY + neighborStepY, width + 1);
			gameX += rowStepX;
			gameY += rowStepY;
			m_clipScreenX += 0x10;
			m_clipScreenY += 8;
		} while (--rowCount != 0);
	}

	bottom = y + m_clipSearchHeight;
	m_map->ScreenToGame(m_viewOriginX + left, m_viewOriginY + y, gameX, gameY);
	gameX /= 0x10;
	gameY /= 0x10;
	m_map->GameToScreen(gameX << GROUND_BLOCK_PIXEL_SHIFT,
						gameY << GROUND_BLOCK_PIXEL_SHIFT,
						m_clipScreenX,
						m_clipScreenY);
	m_clipScreenX -= m_viewOriginX;
	m_clipScreenY -= m_viewOriginY;

	if (y < bottom) {
		rowCount = ((unsigned int) (bottom - y) + GROUND_BLOCK_PIXEL_MASK) >> GROUND_BLOCK_PIXEL_SHIFT;
		do {
			DoClipWidthSearch(gameX, gameY, width);
			m_clipScreenX -= 0x10;
			m_clipScreenY += 8;
			DoClipWidthSearch(gameX + neighborStepX, gameY + neighborStepY, width + 1);
			gameX += rowStepX;
			gameY += rowStepY;
			m_clipScreenX += 0x10;
			m_clipScreenY += 8;
		} while (--rowCount != 0);
	}

	result = DrawClipData();
	CCopyToBackBuff& bitmap = m_backBufferCopies[m_backBufferCopyCount];
	bitmap.m_x = clippedRect.m_x;
	bitmap.m_y = clippedRect.m_y;
	bitmap.m_destination.m_width = clippedRect.m_width;
	bitmap.m_destination.m_height = clippedRect.m_height;
	bitmap.m_destination.m_x = clippedRect.m_x;
	bitmap.m_destination.m_y = clippedRect.m_y;
	m_backBufferCopies[m_backBufferCopyCount].Draw(m_gdi);
	m_backBufferCopyCount++;
	return result;
}

// FUNCTION: LEMBALL 0x0043df30
void C2D::AddViewIndexToObjectClipGrid(int p_x, int p_y, int p_viewIndex, int p_groundHeight, int p_adjustForGround)
{
	if (p_adjustForGround != 0) {
		CMap* map = m_map;
		p_groundHeight += 4;
		int blockX = p_x >> GROUND_BLOCK_PIXEL_SHIFT;
		int nextY = p_y + GROUND_BLOCK_PIXEL_SIZE;
		int nextBlockY = nextY >> GROUND_BLOCK_PIXEL_SHIFT;
		int yHeight;
		int xHeight;
		int diagonalHeight;
		unsigned short sampledHeight;
		if (p_x < 0 || nextY < 0 || map->m_ground.m_width <= blockX || map->m_ground.m_height <= nextBlockY) {
			sampledHeight = 0;
		}
		else {
			int localX = p_x & GROUND_BLOCK_PIXEL_MASK;
			int localY = p_y & GROUND_BLOCK_PIXEL_MASK;
			sampledHeight = map->m_ground.GetGroundCell(blockX, nextBlockY)->GetZ(localX, localY);
		}
		yHeight = sampledHeight;

		map = m_map;
		int blockY = p_y >> GROUND_BLOCK_PIXEL_SHIFT;
		int nextBlockX = (p_x + GROUND_BLOCK_PIXEL_SIZE) >> GROUND_BLOCK_PIXEL_SHIFT;
		if (p_x + GROUND_BLOCK_PIXEL_SIZE < 0 || p_y < 0 || map->m_ground.m_width <= nextBlockX ||
			map->m_ground.m_height <= blockY) {
			sampledHeight = 0;
		}
		else {
			int localY = p_y & GROUND_BLOCK_PIXEL_MASK;
			int localX = p_x & GROUND_BLOCK_PIXEL_MASK;
			sampledHeight = map->m_ground.GetGroundCell(nextBlockX, blockY)->GetZ(localX, localY);
		}
		xHeight = sampledHeight;

		map = m_map;
		if (p_x + GROUND_BLOCK_PIXEL_SIZE < 0 || nextY < 0 || map->m_ground.m_width <= nextBlockX ||
			map->m_ground.m_height <= nextBlockY) {
			sampledHeight = 0;
		}
		else {
			sampledHeight = map->m_ground.m_ground[map->m_ground.m_width * nextBlockY + nextBlockX].GetZ(
				p_x & GROUND_BLOCK_PIXEL_MASK,
				p_y & GROUND_BLOCK_PIXEL_MASK);
		}
		diagonalHeight = sampledHeight;

		blockX = p_x / GROUND_BLOCK_PIXEL_SIZE;
		blockY = p_y / GROUND_BLOCK_PIXEL_SIZE;
		unsigned short yCollision;
		unsigned short xCollision;
		unsigned short diagonalCollision;
		if (blockX < 0 || blockY + 1 < 0 || m_map->m_ground.m_width <= blockX ||
			m_map->m_ground.m_height <= blockY + 1) {
			yCollision = 3;
		}
		else {
			yCollision = m_map->m_ground.m_ground[m_map->m_ground.m_width * (blockY + 1) + blockX].m_collision;
		}
		yCollision &= 1;
		if (blockX + 1 < 0 || blockY < 0 || m_map->m_ground.m_width <= blockX + 1 ||
			m_map->m_ground.m_height <= blockY) {
			xCollision = 3;
		}
		else {
			xCollision = m_map->m_ground.m_ground[m_map->m_ground.m_width * blockY + blockX + 1].m_collision;
		}
		xCollision &= 1;
		if (blockX + 1 < 0 || blockY + 1 < 0 || m_map->m_ground.m_width <= blockX + 1 ||
			m_map->m_ground.m_height <= blockY + 1) {
			diagonalCollision = 3;
		}
		else {
			diagonalCollision =
				m_map->m_ground.m_ground[m_map->m_ground.m_width * (blockY + 1) + blockX + 1].m_collision;
		}
		diagonalCollision &= 1;

		int shiftY = yHeight <= p_groundHeight && yCollision == 0;
		int shiftX = xHeight <= p_groundHeight && xCollision == 0;
		int shiftDiagonal = diagonalHeight <= p_groundHeight && diagonalCollision == 0;
		if (shiftDiagonal && shiftX) {
			if (shiftY) {
				p_x += GROUND_BLOCK_PIXEL_SIZE;
				p_y = nextY;
			}
		}
		else if (shiftY) {
			p_y = nextY;
		}
	}

	p_x >>= 4;
	p_y >>= 4;
	ObjectClipGrid* grid = m_objectClipGrid;
	if (p_x >= 0 && p_y >= 0 && p_x < grid->m_width && p_y < grid->m_height) {
		CObjSq* cell = &grid->m_cells[grid->m_width * p_y + p_x];
		if (cell->m_objectCount < 4) {
			cell->m_viewIndices[cell->m_objectCount] = (short) p_viewIndex;
			cell->m_objectCount++;
		}
		if ((int) grid->m_touchedCount < 100) {
			grid->m_touchedCells[grid->m_touchedCount] = cell;
			grid->m_touchedCount++;
		}
	}
}

// FUNCTION: LEMBALL 0x0043e220
void C2D::BuildObjectClipData(CViewData& p_viewData, int p_viewIndex)
{
	CVSRect boundsValue;
	CVSRect& bounds = boundsValue;
	eObjectType objectType = p_viewData.m_objectType;
	int screenX = p_viewData.m_positionX;
	int screenY = p_viewData.m_positionY;
	if (screenX > -40 && screenY > -40 && screenX < m_clipSize.m_x + 40 && screenY < m_clipSize.m_y + 40) {
		{
			const int& gameX = (int) (unsigned short) p_viewData.m_gameX;
			const int& gameY = (int) (unsigned short) p_viewData.m_gameY;
			switch (objectType) {
			default:
				AddViewIndexToObjectClipGrid(gameX, gameY, p_viewIndex, p_viewData.m_positionZ, 1);
				break;
			case OBJECT_PLAYER_1:
			case OBJECT_PLAYER_2:
				switch (p_viewData.m_action) {
				default:
					AddViewIndexToObjectClipGrid(gameX, gameY, p_viewIndex, p_viewData.m_positionZ, 1);
					break;
				case ACTION_JUMPING:
				case ACTION_FALLING:
					AddViewIndexToObjectClipGrid(gameX - 1, gameY - 1, p_viewIndex, p_viewData.m_positionZ, 0);
					break;
				}
				break;
			case OBJECT_CRATE:
				switch (p_viewData.m_action) {
				case ACTION_OBJECT_READY:
					AddViewIndexToObjectClipGrid(gameX, gameY, p_viewIndex, p_viewData.m_positionZ, 1);
					break;
				case ACTION_OBJECT_ACTIVATING:
				case ACTION_OBJECT_ACTIVATED:
					AddViewIndexToObjectClipGrid(gameX + 16, gameY + 16, p_viewIndex, p_viewData.m_positionZ, 1);
					break;
				}
				break;
			case OBJECT_TRAP_DOOR:
				AddViewIndexToObjectClipGrid(gameX + 32, gameY + 32, p_viewIndex, p_viewData.m_positionZ, 1);
				break;
			case OBJECT_DOOR_1:
			case OBJECT_DOOR_2:
				AddViewIndexToObjectClipGrid(gameX + 16, gameY + 16, p_viewIndex, p_viewData.m_positionZ, 1);
				break;
			case OBJECT_PAINT_GUN:
			case OBJECT_TRAMPOLINE:
				AddViewIndexToObjectClipGrid(gameX, gameY, p_viewIndex, p_viewData.m_positionZ, 0);
				break;
			case OBJECT_MOVER:
				AddViewIndexToObjectClipGrid(gameX - 8, gameY - 8, p_viewIndex, p_viewData.m_positionZ + 8, 1);
				break;
			}
		}
		if (g_nStartupGraphicsDialogRequested != 0) {
			return;
		}

		switch (objectType) {
		case OBJECT_PLAYER_1:
		case OBJECT_PLAYER_2: {
			bounds.m_x = (short) screenX - 24;
			bounds.m_y = (short) screenY - 24;
			bounds.m_width = 48;
			bounds.m_height = 40;
			if (InGroupByObjectNo(p_viewData.m_objectId)) {
				bounds.m_y -= 20;
				bounds.m_height += 20;
			}
			if (p_viewData.m_action != ACTION_FLYING) {
				if (p_viewData.m_action != ACTION_ON_BALLOON) {
					break;
				}
				bounds.m_y -= 40;
				bounds.m_height += 40;
				m_clipSearchHeight = 160;
			}
			{
				SpriteGroundLookup* lookup = m_spriteGroundLookup;
				const CVSRect& markedRect = bounds;
				int rectangleWidth = markedRect.m_width;
				short pixelX = markedRect.m_x;
				short pixelY = markedRect.m_y;
				int cellX = (short) (pixelX / GROUND_BLOCK_PIXEL_SIZE);
				int cellY = (short) (pixelY / GROUND_BLOCK_PIXEL_SIZE);
				int columns = (rectangleWidth + pixelX - 1) / GROUND_BLOCK_PIXEL_SIZE - cellX + 1;
				int rows = (pixelY + markedRect.m_height - 1) / GROUND_BLOCK_PIXEL_SIZE - cellY + 1;
				short width = lookup->m_width;
				short height;
				if (cellX < width && ((height = lookup->m_height), cellY < height)) {
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
						int offset = cellX + cellY * width;
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
			}
			{
				int gameX = (unsigned short) p_viewData.m_gameX;
				int gameY = (unsigned short) p_viewData.m_gameY;
				CMap* map = m_map;
				int blockX = gameX >> GROUND_BLOCK_PIXEL_SHIFT;
				int blockY = gameY >> GROUND_BLOCK_PIXEL_SHIFT;
				unsigned short groundHeight;
				if (gameX < 0 || gameY < 0 || blockX >= map->m_ground.m_width || blockY >= map->m_ground.m_height) {
					groundHeight = 0;
				}
				else {
					int localX = gameX & GROUND_BLOCK_PIXEL_MASK;
					int localY = gameY & GROUND_BLOCK_PIXEL_MASK;
					groundHeight = map->m_ground.m_ground[map->m_ground.m_width * blockY + blockX].GetZ(localX, localY);
				}
				gameX = (gameX << FIXED_POINT_FRACTION_BITS) >> FIXED_POINT_FRACTION_BITS;
				gameY = (gameY << FIXED_POINT_FRACTION_BITS) >> FIXED_POINT_FRACTION_BITS;
				m_map->GameToScreen(gameX, gameY);
				CFixed heightFixed((unsigned int) groundHeight << FIXED_POINT_FRACTION_BITS);
				const int& projectedY = gameY - (heightFixed.m_value >> FIXED_POINT_FRACTION_BITS);
				CFixed topFixed((projectedY - m_viewOriginY) << FIXED_POINT_FRACTION_BITS);
				CFixed leftFixed((gameX - m_viewOriginX) << FIXED_POINT_FRACTION_BITS);
				int left = leftFixed.m_value >> FIXED_POINT_FRACTION_BITS;
				int top = topFixed.m_value >> FIXED_POINT_FRACTION_BITS;
				bounds.m_x = (short) left - 10;
				bounds.m_y = (short) top - 5;
				bounds.m_width = 20;
				bounds.m_height = 10;
				m_clipSearchHeight = 160;
			}
			break;
		}
		case OBJECT_BULLET:
			bounds.m_x = (short) screenX - 8;
			bounds.m_y = (short) screenY - 8;
			bounds.m_width = 16;
			bounds.m_height = 16;
			break;
		case OBJECT_CATAPULT:
			bounds.m_x = (short) screenX - 48;
			bounds.m_y = (short) screenY - 60;
			bounds.m_width = 64;
			bounds.m_height = 56;
			break;
		case OBJECT_AMMO:
			bounds.m_x = (short) screenX - 8;
			bounds.m_y = (short) screenY - 16;
			bounds.m_width = 16;
			bounds.m_height = 24;
			break;
		case OBJECT_SHEEP:
			bounds.m_x = (short) screenX - g_sheepOffset[0];
			bounds.m_y = (short) screenY - g_sheepOffset[1];
			bounds.m_width = g_sheepOffset[0] * 2;
			bounds.m_height = g_sheepOffset[1] * 2;
			break;
		case OBJECT_BALL:
			bounds.m_x = (short) screenX - 10;
			bounds.m_y = (short) screenY - 15;
			bounds.m_width = 24;
			bounds.m_height = 24;
			break;
		case OBJECT_FLAG_1:
		case OBJECT_FLAG_2:
			bounds.m_x = (short) screenX - 16;
			bounds.m_y = (short) screenY - 28;
			bounds.m_width = 32;
			bounds.m_height = 32;
			break;
		case OBJECT_TOWER:
			bounds.m_x = (short) screenX - (short) g_styleObjectClip[0];
			bounds.m_y = (short) screenY - (short) g_styleObjectClip[1];
			bounds.m_width = (short) g_styleObjectClip[2];
			bounds.m_height = (short) g_styleObjectClip[3];
			break;
		case OBJECT_CRATE:
			if (p_viewData.m_action == ACTION_OBJECT_READY) {
				bounds.m_x = (short) screenX - 12;
				bounds.m_y = (short) screenY - 12;
				bounds.m_width = 24;
				bounds.m_height = 24;
			}
			else {
				bounds.m_x = (short) screenX - 32;
				bounds.m_y = (short) screenY - 64;
				bounds.m_width = 64;
				bounds.m_height = 64;
			}
			break;
		case OBJECT_BONUS:
			bounds.m_x = (short) screenX - 16;
			bounds.m_y = (short) screenY - 16;
			bounds.m_width = 32;
			bounds.m_height = 20;
			break;
		case OBJECT_MINE:
			bounds.m_x = (short) screenX - 28;
			bounds.m_y = (short) screenY - 35;
			bounds.m_width = 56;
			bounds.m_height = 44;
			break;
		case OBJECT_SWITCH:
			bounds.m_x = (short) screenX - 16;
			bounds.m_y = (short) screenY - 16;
			bounds.m_width = 32;
			bounds.m_height = 32;
			break;
		case OBJECT_KEY_1:
		case OBJECT_KEY_2:
		case OBJECT_KEY_3:
			bounds.m_x = (short) screenX - 12;
			bounds.m_y = (short) screenY - 32;
			bounds.m_width = 24;
			bounds.m_height = 32;
			break;
		case OBJECT_TRAP_DOOR:
			bounds.m_x = (short) screenX - 48;
			bounds.m_y = (short) screenY - 40;
			bounds.m_width = 96;
			bounds.m_height = 168;
			m_clipSearchHeight = 160;
			break;
		case OBJECT_DOOR_1:
		case OBJECT_DOOR_2:
			bounds.m_x = (short) screenX - 26;
			bounds.m_y = (short) screenY - 24;
			bounds.m_width = 48;
			bounds.m_height = 40;
			if (p_viewData.m_action == ACTION_DOOR_LOCKED_FEEDBACK) {
				bounds.m_y -= 12;
				bounds.m_height += 12;
			}
			break;
		case OBJECT_TIME_BONUS:
			bounds.m_x = (short) screenX - 16;
			bounds.m_y = (short) screenY - 12;
			bounds.m_width = 32;
			bounds.m_height = 24;
			break;
		case OBJECT_DUPLICATOR:
			bounds.m_x = (short) screenX - 13;
			bounds.m_y = (short) screenY - 54;
			bounds.m_width = 100;
			bounds.m_height = 60;
			break;
		case OBJECT_LASER_HORIZONTAL:
		case OBJECT_LASER_VERTICAL:
		case OBJECT_LASER_EMITTER_H:
		case OBJECT_LASER_EMITTER_V:
			bounds.m_x = (short) screenX - 20;
			bounds.m_y = (short) screenY - 10;
			bounds.m_width = 40;
			bounds.m_height = 30;
			break;
		case OBJECT_HAND:
			bounds.m_x = (short) screenX - 48;
			bounds.m_y = (short) screenY - 20;
			bounds.m_width = 48;
			bounds.m_height = 48;
			break;
		case OBJECT_ROCKET:
			bounds.m_x = (short) screenX - 13;
			bounds.m_y = (short) screenY - 80;
			bounds.m_width = 26;
			bounds.m_height = 80;
			break;
		case OBJECT_PAINT_GUN:
			bounds.m_x = (short) screenX - 22;
			bounds.m_y = (short) screenY - 35;
			bounds.m_width = 48;
			bounds.m_height = 48;
			break;
		case OBJECT_TRAMPOLINE:
			bounds.m_x = (short) screenX - 22;
			bounds.m_y = (short) screenY - 22;
			bounds.m_width = 48;
			bounds.m_height = 32;
			break;
		case OBJECT_LASER_HORIZONTAL_BEAM:
			bounds.m_x = (short) screenX - 21;
			bounds.m_y = (short) screenY - 14;
			bounds.m_width = 22;
			bounds.m_height = 15;
			break;
		case OBJECT_BALLOON_0:
		case OBJECT_BALLOON_2:
		case OBJECT_BALLOON_4:
		case OBJECT_BALLOON_6:
			bounds.m_x = (short) screenX - 20;
			bounds.m_y = (short) screenY - 68;
			bounds.m_width = 40;
			bounds.m_height = 72;
			break;
		case OBJECT_BALLOON_1:
		case OBJECT_BALLOON_3:
		case OBJECT_BALLOON_5:
		case OBJECT_BALLOON_7:
			bounds.m_x = (short) screenX - 16;
			bounds.m_y = (short) screenY - 48;
			bounds.m_width = 32;
			bounds.m_height = 48;
			break;
		case OBJECT_LASER_VERTICAL_BEAM:
			bounds.m_x = (short) screenX - 21;
			bounds.m_y = (short) screenY - 14;
			bounds.m_width = 22;
			bounds.m_height = 15;
			break;
		case OBJECT_MOVER:
			bounds.m_x = (short) screenX - 24;
			bounds.m_y = (short) screenY - 24;
			bounds.m_width = 48;
			bounds.m_height = 56;
			break;
		case OBJECT_SLINKY: {
			unsigned int direction = (unsigned short) p_viewData.m_actionArgument;
			bounds.m_x = (short) screenX - g_slinkyOffsets[direction][0];
			bounds.m_y = (short) screenY - g_slinkyOffsets[direction][1];
			bounds.m_width = 40;
			bounds.m_height = 42;
			break;
		}
		default:
			return;
		}
	}
	{
		SpriteGroundLookup* lookup = m_spriteGroundLookup;
		const CVSRect& markedRect = bounds;
		int rectangleWidth = markedRect.m_width;
		short pixelX = markedRect.m_x;
		short pixelY = markedRect.m_y;
		int cellX = (short) (pixelX / GROUND_BLOCK_PIXEL_SIZE);
		int cellY = (short) (pixelY / GROUND_BLOCK_PIXEL_SIZE);
		int columns = (rectangleWidth + pixelX - 1) / GROUND_BLOCK_PIXEL_SIZE - cellX + 1;
		int rows = (pixelY + markedRect.m_height - 1) / GROUND_BLOCK_PIXEL_SIZE - cellY + 1;
		short width = lookup->m_width;
		short height;
		if (cellX < width && ((height = lookup->m_height), cellY < height)) {
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
				int offset = cellX + cellY * width;
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
	}
}

// FUNCTION: LEMBALL 0x0043f060
void C2D::MarkGroundAnimAndLiftBounds()
{
	char* scratch = m_groundClipScratch;
	int count = m_ai->ExportGroundAnimRecords((tCoord3d*) scratch);
	if (count > 0) {
		int remaining = count;
		tCoord3d* coordinate = (tCoord3d*) scratch;
		do {
			int screenX = coordinate->m_x;
			int screenY = coordinate->m_y;
			const int& groundHeight = (int) coordinate->m_z;
			C2D* view = this;
			view->m_map->GameToScreen(screenX, screenY);
			screenX -= view->m_viewOriginX;
			screenY -= view->m_viewOriginY;
			screenY -= groundHeight;
			short pixelX = (short) screenX - 16;
			short pixelY = (short) screenY - 24;
			short rectHeight = (short) groundHeight + 48;
			SpriteGroundLookup* lookup = m_spriteGroundLookup;
			int cellX = (short) (pixelX / GROUND_BLOCK_PIXEL_SIZE);
			int cellY = (short) (pixelY / GROUND_BLOCK_PIXEL_SIZE);
			int columns = (pixelX + 32 - 1) / GROUND_BLOCK_PIXEL_SIZE - cellX + 1;
			int rows = (pixelY + rectHeight - 1) / GROUND_BLOCK_PIXEL_SIZE - cellY + 1;
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
			int cellX = (short) (pixelX / GROUND_BLOCK_PIXEL_SIZE);
			int cellY = (short) (pixelY / GROUND_BLOCK_PIXEL_SIZE);
			int columns = (pixelX + rectWidth - 1) / GROUND_BLOCK_PIXEL_SIZE - cellX + 1;
			int rows = (pixelY + rectHeight - 1) / GROUND_BLOCK_PIXEL_SIZE - cellY + 1;
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

// FUNCTION: LEMBALL 0x0043f480
void C2D::UpdateSpriteGroundLookupRegions()
{
	const short& translatedX = (short) (m_spriteGroundTranslationPoint.m_x - (short) m_clipOffsetX - 3);
	const short& translatedY = (short) (m_spriteGroundTranslationPoint.m_y - (short) m_clipOffsetY - 3);
	SpriteGroundLookup* lookup = m_spriteGroundLookup;
	m_spriteGroundTranslatedPointRect.m_width = 16;
	m_spriteGroundTranslatedPointRect.m_height = 16;
	m_spriteGroundTranslatedPointRect.m_x = translatedX;
	m_spriteGroundTranslatedPointRect.m_y = translatedY;

	short pixelX = m_spriteGroundLookupRectA.m_x;
	int cellX = (short) (pixelX / GROUND_BLOCK_PIXEL_SIZE);
	int cellY = (short) (m_spriteGroundLookupRectA.m_y / GROUND_BLOCK_PIXEL_SIZE);
	int columns = (pixelX + m_spriteGroundLookupRectA.m_width - 1) / GROUND_BLOCK_PIXEL_SIZE - cellX + 1;
	int rows =
		(m_spriteGroundLookupRectA.m_y + m_spriteGroundLookupRectA.m_height - 1) / GROUND_BLOCK_PIXEL_SIZE - cellY + 1;
	short width = lookup->m_width;
	short height;
	if (cellX < width && ((height = lookup->m_height), cellY < height)) {
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
			int offset = cellX + cellY * width;
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
	m_spriteGroundLookup->MarkRect(m_spriteGroundLookupRectB);
}
