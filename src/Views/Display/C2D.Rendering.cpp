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
#include "Views/Sound/SoundEffects.h"
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

enum eLevelTimeDisplay {
	LEVEL_TIME_DISPLAY_LIMIT_SECONDS = 10 * 60,
	LEVEL_TIME_DISPLAY_MAX_SECONDS = LEVEL_TIME_DISPLAY_LIMIT_SECONDS - 1,
	SECONDS_PER_DISPLAY_MINUTE = 60,
	SECONDS_PER_DISPLAY_TEN = 10
};

enum eSpriteSortCode {
	SPRITE_SORT_CODE_SPECIAL_OBJECT = 0x7d00,
	SPRITE_SORT_CODE_TOPMOST = 0x7fff
};

enum eScrollOffsetDirection {
	SCROLL_OFFSET_NONE = 0,
	SCROLL_OFFSET_X = 1,
	SCROLL_OFFSET_Y = 2,
	SCROLL_OFFSET_XY = 3
};

extern "C" unsigned long __stdcall timeGetTime(void);

extern char* g_demoText;

// GLOBAL: LEMBALL 0x0049efcc
int g_lastDrawnTime = 0;

// GLOBAL: LEMBALL 0x004a78bc
char g_timeText[5];

// GLOBAL: LEMBALL 0x0049ee70
char* g_demoText = "Demo";

// GLOBAL: LEMBALL 0x0049705c
static const short g_treeGroundOffset[] = {0x20, 0x30};

// GLOBAL: LEMBALL 0x00497060
extern const short g_groundOffset[] = {0x10, 0x10};

// FUNCTION: LEMBALL 0x0043a880
void C2D::DrawGround(int p_x, int p_y, eObjectType p_groundType, unsigned short p_frame)
{
	switch (p_groundType) {
	case TERRAIN_TREE:
		m_lemmingAnims->DrawAnim(p_x - g_treeGroundOffset[0],
								 p_y - g_treeGroundOffset[1],
								 g_anGroundStyleResourceIds[3],
								 p_frame,
								 0,
								 NULL);
		return;
	case TERRAIN_BLOX_1:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox1ResourceId, p_frame, 0, NULL);
		return;
	case TERRAIN_BLOX_2:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox2ResourceId, p_frame, 0, NULL);
		return;
	case TERRAIN_BLOX_3_SLOPE_SW_STEEP:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox3ResourceId, p_frame, 0, NULL);
		return;
	case TERRAIN_BLOX_4:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox4ResourceId, p_frame, 0, NULL);
		return;
	case TERRAIN_BLOX_5:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox5ResourceId, p_frame, 0, NULL);
		return;
	case TERRAIN_BLOX_6:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox6ResourceId, p_frame, 0, NULL);
		return;
	case TERRAIN_BLOX_7:
		m_lemmingAnims
			->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], g_groundBlox7ResourceId, p_frame, 0, NULL);
		return;
	case TERRAIN_BLOX_8_SLOPE_SE_STEEP:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0],
								 p_y - g_groundOffset[1],
								 g_anGroundStyleResourceIds[0],
								 p_frame,
								 0,
								 NULL);
		return;
	case TERRAIN_BLOX_14_SLOPE_SW_SHALLOW:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0],
								 p_y - g_groundOffset[1],
								 g_anGroundStyleResourceIds[4],
								 p_frame,
								 0,
								 NULL);
		return;
	case TERRAIN_BLOX_15_SLOPE_SE_SHALLOW:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0],
								 p_y - g_groundOffset[1],
								 g_anGroundStyleResourceIds[5],
								 p_frame,
								 0,
								 NULL);
		return;
	case TERRAIN_ANIM:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], RES_GAME_ANIM, p_frame, 0, NULL);
		return;
	case TERRAIN_FLAME:
		m_lemmingAnims->DrawAnim(
			p_x - 0x10,
			p_y - 0x20,
			RES_GAME_FLAME,
			(((unsigned short) p_x >> GROUND_BLOCK_PIXEL_SHIFT) + (unsigned short) m_groundAnimationFrame) % 9,
			0,
			NULL);
		return;
	case TERRAIN_ELECTRIC:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0],
								 p_y - g_groundOffset[1],
								 RES_GAME_ELECTRIC,
								 (unsigned short) m_groundAnimationFrame & GROUND_ANIMATION_VARIANT_MASK,
								 0,
								 NULL);
		return;
	case TERRAIN_EMBERS:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], RES_GAME_EMBERS, p_frame, 0, NULL);
		return;
	case TERRAIN_CONVEYOR_VARIANT_A:
	case TERRAIN_CONVEYOR_VARIANT_B:
		m_lemmingAnims->DrawAnim(p_x - g_groundOffset[0], p_y - g_groundOffset[1], RES_GAME_CONVEYOR, p_frame, 0, NULL);
		return;
	default:
		return;
	}
}

// FUNCTION: LEMBALL 0x0043ace0
void C2D::DrawCliff(int p_x, int p_y, int p_height, int p_count)
{
	int x = p_x - g_groundOffset[0];
	int y = p_y + (p_count * 0x10 - p_height);

	if (p_count > 0) {
		do {
			m_lemmingAnims->DrawAnim(x, y - g_groundOffset[1], g_groundBlox1ResourceId, 0, 0, NULL);
			y -= 0x10;
		} while (--p_count != 0);
	}
}

// FUNCTION: LEMBALL 0x0043ef90
void C2D::TransformAndSortViewData()
{
	int viewIndex = 0;
	if ((int) m_viewDataCount > 0) {
		do {
			CViewData* viewData = m_viewData + viewIndex;
			viewData->m_gameX = (short) viewData->m_positionX;
			viewData->m_gameY = (short) viewData->m_positionY;

			C3DVector position;
			memcpy(&position, &m_viewData[viewIndex].m_positionX, sizeof(position));
			m_map->GameToScreen(position.m_xFixed, position.m_yFixed);
			position.m_yFixed -= position.m_zFixed;
			position.m_xFixed -= m_viewOriginX;
			position.m_yFixed -= m_viewOriginY;
			memcpy(&m_viewData[viewIndex].m_positionX, &position, sizeof(position));
			viewIndex++;
		} while ((int) m_viewDataCount > viewIndex);
	}
	SortViewData();
}

// FUNCTION: LEMBALL 0x0043f620
void C2D::DrawObjects()
{
	enum {
		ZRLE_DRAW_FLAG_Z_BUFFER = 0x40000
	};
	C2D& owner = *this;
	SetOrigin();
	owner.m_backBufferCopyCount = 0;
	owner.m_lemmingAnims->m_drawFlags = ZRLE_DRAW_FLAG_Z_BUFFER;
	int scrollX = abs((int) owner.m_scrollDeltaX);
	int scrollY = abs((int) owner.m_scrollDeltaY);
	CVSRect borders[4];

	if (owner.m_scrollPending != 0 && owner.m_redrawPending == 0 && scrollX < owner.m_clipSize.m_x &&
		scrollY < owner.m_clipSize.m_y) {
		unsigned char direction = (unsigned char) ((owner.m_scrollDeltaX < 0 ? SCROLL_OFFSET_X : 0) |
												   (owner.m_scrollDeltaY < 0 ? SCROLL_OFFSET_Y : 0));
		CVSRect exposed[2];
		short destinationX = 0;
		short destinationY = 0;
		CVSRect retained;
		switch ((unsigned int) direction) {
		case SCROLL_OFFSET_NONE:
			retained = CVSRect((short) scrollX,
							   (short) scrollY,
							   owner.m_clipSize.m_x - (short) scrollX,
							   owner.m_clipSize.m_y - (short) scrollY);
			if (scrollY != 0) {
				exposed[0] = CVSRect(0, 0, owner.m_clipSize.m_x, (short) scrollY);
			}
			if (scrollX != 0) {
				exposed[1] = CVSRect(0, (short) scrollY, (short) scrollX, owner.m_clipSize.m_y - (short) scrollY);
			}
			break;
		case SCROLL_OFFSET_X:
			destinationX = (short) scrollX;
			destinationY = 0;
			retained = CVSRect(0,
							   (short) scrollY,
							   owner.m_clipSize.m_x - (short) scrollX,
							   owner.m_clipSize.m_y - (short) scrollY);
			if (scrollY != 0) {
				exposed[0] = CVSRect(0, 0, owner.m_clipSize.m_x, (short) scrollY);
			}
			if (scrollX != 0) {
				exposed[1] = CVSRect(owner.m_clipSize.m_x - (short) scrollX,
									 (short) scrollY,
									 (short) scrollX,
									 owner.m_clipSize.m_y - (short) scrollY);
			}
			if (owner.m_clipOffsetX > 0 || owner.m_clipOffsetY > 0) {
				borders[0] = CVSRect((short) owner.m_clipOffsetX - (short) scrollX,
									 (short) owner.m_clipOffsetY,
									 (short) scrollX,
									 owner.m_clipSize.m_y + (short) scrollY);
				borders[1] = CVSRect((short) owner.m_clipOffsetX,
									 owner.m_clipSize.m_y + (short) owner.m_clipOffsetY,
									 owner.m_clipSize.m_x,
									 (short) scrollY);
			}
			break;
		case SCROLL_OFFSET_Y:
			destinationY = (short) scrollY;
			destinationX = 0;
			retained = CVSRect((short) scrollX,
							   0,
							   owner.m_clipSize.m_x - (short) scrollX,
							   owner.m_clipSize.m_y - (short) scrollY);
			if (scrollY != 0) {
				exposed[0] = CVSRect(0, owner.m_clipSize.m_y - (short) scrollY, owner.m_clipSize.m_x, (short) scrollY);
			}
			if (scrollX != 0) {
				exposed[1] = CVSRect(0, 0, (short) scrollX, owner.m_clipSize.m_y - (short) scrollY);
			}
			break;
		case SCROLL_OFFSET_XY:
			destinationX = (short) scrollX;
			destinationY = (short) scrollY;
			retained = CVSRect(0, 0, owner.m_clipSize.m_x - (short) scrollX, owner.m_clipSize.m_y - (short) scrollY);
			if (scrollY != 0) {
				exposed[0] = CVSRect(0, owner.m_clipSize.m_y - (short) scrollY, owner.m_clipSize.m_x, (short) scrollY);
			}
			if (scrollX != 0) {
				exposed[1] = CVSRect(owner.m_clipSize.m_x - (short) scrollX,
									 0,
									 (short) scrollX,
									 owner.m_clipSize.m_y - (short) scrollY);
			}
			break;
		}
		owner.m_screenScroll.m_destination.m_x = destinationX;
		owner.m_screenScroll.m_destination.m_y = destinationY;
		owner.m_screenScroll.m_rect = retained;
		owner.m_screenScroll.Draw(owner.m_gdi);
		CVSRect* strip = exposed;
		do {
			if ((int) strip->m_width * (int) strip->m_height != 0) {
				CVSRect copyRect = *strip;
				owner.m_scrollDepthClears[0].m_bounds = copyRect;
				owner.m_scrollDepthClears[0].m_depth = 0;
				owner.m_scrollDepthClears[0].Draw(owner.m_gdi);
				DrawClippedRectangle(*strip);
			}
			strip++;
		} while (strip < exposed + 2);
	}
	else if (owner.m_scrollPending != 0 || owner.m_redrawPending != 0) {
		owner.m_gdi->m_renderTarget->ResetScroll();
		CVSRect fullRect(0, 0, (CVSSize*) &owner.m_clipSize);
		if (owner.m_redrawPending != 0) {
			fullRect = CVSRect(0, 0, owner.m_clipSize.m_x, owner.m_clipSize.m_y);
		}
		owner.m_depthClear.m_bounds = fullRect;
		owner.m_depthClear.m_depth = 0;
		owner.m_depthClear.Draw(owner.m_gdi);
		DrawClippedRectangle(CVSRect(0, 0, owner.m_clipSize.m_x, owner.m_clipSize.m_y));
		owner.m_redrawPending = 0;
	}
	owner.m_scrollPending = 0;
	DrawObjectsZBuff();
	owner.m_lemmingAnims->m_drawFlags = 0;
}

// FUNCTION: LEMBALL 0x0043fce0
void C2D::DrawDemo()
{
	// GLOBAL: LEMBALL 0x004a78c4
	static unsigned long g_lastBlink = CurrentMilliTimer();
	// GLOBAL: LEMBALL 0x004a78c8
	// ?$S2@?1??DrawDemo@C2D@@QAEXXZ@4EA
	// GLOBAL: LEMBALL 0x0049efc8
	static int g_visible = 0;
	if (CurrentMilliTimer() - g_lastBlink > DEMO_TEXT_BLINK_INTERVAL_MS) {
		g_visible = !g_visible;
		g_lastBlink = CurrentMilliTimer();
	}
	if (g_visible) {
		CVSPoint& position = m_demoTextRect;
		CVSSize advance;
		advance.m_height = 0;
		advance.m_width = 0;
		m_textManager->DrawString(m_gdi,
								  position,
								  advance,
								  RES_BORDERS_LORES_CUTFONT,
								  g_demoText,
								  TEXT_ADVANCE_X_POSITIVE,
								  (CRemap*) m_remaps[4]);
	}
}

// FUNCTION: LEMBALL 0x0043fd80
void C2D::DrawTime()
{
	unsigned short baseTime = (unsigned short) m_ai->m_levelTimeRemaining;
	short time = (short) m_ai->m_gameTime;
	time = (short) (time + baseTime);
	if (time < 0) {
		time = 0;
	}
	if (time >= LEVEL_TIME_DISPLAY_LIMIT_SECONDS) {
		if (baseTime >= LEVEL_TIME_DISPLAY_LIMIT_SECONDS) {
			return;
		}
		if (time >= LEVEL_TIME_DISPLAY_LIMIT_SECONDS) {
			time = LEVEL_TIME_DISPLAY_MAX_SECONDS;
		}
	}

	if (time != g_lastDrawnTime) {
		g_lastDrawnTime = time;
		int seconds = time % SECONDS_PER_DISPLAY_MINUTE;
		g_timeText[0] = (char) (time / SECONDS_PER_DISPLAY_MINUTE) + '0';
		g_timeText[1] = ':';
		g_timeText[2] = (char) (seconds / SECONDS_PER_DISPLAY_TEN) + '0';
		g_timeText[3] = (char) (seconds % SECONDS_PER_DISPLAY_TEN) + '0';
		g_timeText[4] = 0;
	}

	CVSPoint& position = m_spriteGroundLookupRectA;
	CVSSize advance;
	advance.m_width = -4;
	advance.m_height = 0;
	m_textManager->DrawString(m_gdi,
							  position,
							  advance,
							  RES_NEWFRONT_FONTS_GAME_SCORETIME,
							  g_timeText,
							  TEXT_ADVANCE_X_POSITIVE,
							  NULL);
}

// FUNCTION: LEMBALL 0x0043fe70
void C2D::DrawPaused()
{
}

// FUNCTION: LEMBALL 0x0043fe80
void C2D::DrawScore()
{
	int targetScore = m_ai->m_score;
	int score = m_score;
	if (m_scoreTimestamp <= g_dwGameTick) {
		if (score != targetScore) {
			if (m_ai->m_gameStatus == GAME_STATUS_RUNNING) {
				score += 10;
				if (score >= targetScore) {
					score = targetScore;
				}
			}
			else {
				score += 100;
				if (score >= targetScore) {
					score = targetScore;
				}
			}
			m_score = score;
		}
		m_scoreTimestamp = g_dwGameTick + 1;
	}
	if (score >= GAME_SCORE_DISPLAY_VALUE_LIMIT) {
		score = GAME_SCORE_MAX_DISPLAY_VALUE;
	}

	CVSSize advance;
	char scoreText[8];
	scoreText[7] = 0;
	int i = 1;
	do {
		scoreText[7 - i] = (char) (score % 10) + '0';
		i++;
		score /= 10;
	} while (i <= 7);

	CVSPoint* position = &m_spriteGroundLookupRectB;
	advance.m_width = -4;
	advance.m_height = 0;
	m_textManager->DrawString(m_gdi,
							  *position,
							  advance,
							  RES_NEWFRONT_FONTS_GAME_SCORETIME,
							  scoreText,
							  TEXT_ADVANCE_X_POSITIVE,
							  NULL);
}

// FUNCTION: LEMBALL 0x0043ff70
void C2D::SortViewData()
{
	int index = 0;
	if (m_viewDataCount > 0) {
		do {
			if (m_redrawPending == 0 && m_viewData[index].m_transientFlags == 0) {
				m_redrawPending = 0;
			}
			else {
				m_redrawPending = 1;
			}

			m_viewData[index].m_sortZKey = CalcZValue_Sprite(index);
			index++;
		} while (index < (int) m_viewDataCount);
	}

	VSQSort(m_viewData, m_viewDataCount, sizeof(CViewData), ViewDataCmp);
}

// FUNCTION: LEMBALL 0x00440000
void C2D::Draw(const CVSRect& p_rect)
{
	if (m_gdi == NULL || m_clipSize.m_x <= 0 || m_clipSize.m_y <= 0) {
		return;
	}
	if (m_lemmingAnims->m_loaded == 0) {
		m_lemmingAnims->Draw();
		return;
	}

	unsigned long groundAnimationFrame = g_dwSimulationTimestamp / 100;
	m_frameCount++;
	m_groundAnimationFrame = (short) groundAnimationFrame;
	unsigned long startTime = timeGetTime();
	m_clipSearchHeight = 0x40;

	CVSRect* displayRect = &m_display->m_rect;
	CVSPoint* displayPosition = displayRect;
	int zoom = m_display->m_zoom;
	CVSPoint cursorPosition;
	cursorPosition.m_y = (short) ((short) (g_pCursor->m_position.m_y - displayPosition->m_y) / zoom);
	cursorPosition.m_x = (short) ((short) (g_pCursor->m_position.m_x - displayPosition->m_x) / zoom);
	m_spriteGroundTranslationPoint.m_x = cursorPosition.m_x;
	m_spriteGroundTranslationPoint.m_y = cursorPosition.m_y;
	ReplaceBackground();

	if (m_clipOffsetX != 0 || m_clipOffsetY != 0) {
		int left = m_clipOffsetX + m_spriteGroundTranslatedPointRect.m_x;
		int height = m_spriteGroundTranslatedPointRect.m_height;
		int top = m_clipOffsetY + m_spriteGroundTranslatedPointRect.m_y;
		int right = m_spriteGroundTranslatedPointRect.m_width + left;
		int bottom = height + top;
		int clipBottom = m_clipOffsetY + m_clipSize.m_y;
		int clipRight = m_clipOffsetX + m_clipSize.m_x;
		if (left < m_clipOffsetX || top < m_clipOffsetY || clipRight <= right || bottom >= clipBottom) {
			CVSRect translatedBounds;
			CVSSize& translatedSize = translatedBounds;
			CVSPoint& translatedPosition = translatedBounds;
			translatedPosition.m_x = (short) left;
			translatedPosition.m_y = (short) top;
			translatedSize.m_width = m_spriteGroundTranslatedPointRect.m_width;
			translatedSize.m_height = m_spriteGroundTranslatedPointRect.m_height;
			m_lineAt9a8.m_bounds.m_width = translatedSize.m_width;
			m_lineAt9a8.m_bounds.m_height = translatedSize.m_height;
			m_lineAt9a8.m_bounds.CVSPoint::operator=(translatedPosition);
			m_lineAt9a8.m_colour = 0;
			m_lineAt9a8.Draw(m_gdi);
		}
	}

	m_viewDataCount = (unsigned short) m_ai->GetData(m_viewData);
	m_pushActive.Draw(m_gdi);
	CVSRect backgroundBounds;

	{
		int primitiveIndex = m_primitiveCount++;
		CClipRect& background = m_clipRects[primitiveIndex];
		if (m_clipConfigured == 0 && m_redrawPending == 0) {
			backgroundBounds.m_width = m_clipSize.m_x;
			backgroundBounds.m_height = m_clipSize.m_y;
			backgroundBounds.m_x = 0;
			backgroundBounds.m_y = 0;
		}
		else {
			backgroundBounds.m_width = m_clipSize.m_x;
			backgroundBounds.m_height = m_clipSize.m_y;
			backgroundBounds.m_x = 0;
			backgroundBounds.m_y = 0;
		}
		static_cast<CVSSize&>(background.m_bounds) = backgroundBounds;
		background.m_bounds.CVSPoint::operator=(backgroundBounds);
		background.m_flags = 0;
		background.Draw(m_gdi);
	}

	if (m_clipConfigured != 0 || m_redrawPending != 0) {
		CVSRect translatedBounds;
		m_clipConfigured = 0;
		translatedBounds.m_width = m_clipSize.m_x;
		translatedBounds.m_height = m_clipSize.m_y;
		translatedBounds.m_x = 0;
		translatedBounds.m_y = 0;
		m_lineAt998.m_bounds = translatedBounds;
		m_lineAt998.m_colour = 0;
		m_lineAt998.Draw(m_gdi);
	}

	DrawObjects();
	if (g_pDemo != NULL && g_pDemo->m_demoMode != 0) {
		DrawDemo();
	}
	else {
		DrawTime();
		DrawScore();
	}
	if (m_paused != 0) {
		DrawPaused();
	}
	m_popActive.Draw(m_gdi);
	g_pSoundView->SoundEffect(m_viewData, m_viewDataCount, m_originPosition);

	{
		int surfacePrimitiveIndex = m_primitiveCount++;
		CClipRect& surfaceBackground = m_clipRects[surfacePrimitiveIndex];
		CVSRect& windowRect = m_gdi->m_renderTarget->m_windowRect;
		CVSRect translatedBounds(windowRect);
		translatedBounds.m_x = 0;
		translatedBounds.m_y = 0;
		memcpy(&surfaceBackground.m_bounds.m_width, &translatedBounds.m_width, sizeof(short));
		memcpy(&surfaceBackground.m_bounds.m_height, &translatedBounds.m_height, sizeof(short));
		memcpy(&surfaceBackground.m_bounds.m_x, &translatedBounds.m_x, sizeof(short));
		memcpy(&surfaceBackground.m_bounds.m_y, &translatedBounds.m_y, sizeof(short));
		surfaceBackground.m_flags = 0;
		surfaceBackground.Draw(m_gdi);
	}

	m_frameTime += timeGetTime() - startTime;
}

// FUNCTION: LEMBALL 0x00440400
void C2D::ReplaceBackground()
{
	CChangeList* changeList = m_gdi->m_renderTarget->GetChangeList();
	m_drawingMark.Draw(m_gdi);
	changeList->Reset();
}

// FUNCTION: LEMBALL 0x00440430
void C2D::ResetPrimitives()
{
	m_lemmingAnims->ResetPrimitives();
	m_textManager->ResetPrimitives();
	m_unk0xc90 = 0;
	m_primitiveCount = 0;
}

// FUNCTION: LEMBALL 0x00440460
void C2D::DrawZBuff_Sprite(int p_index, unsigned short p_z)
{
	m_lemmingAnims->m_primitiveSequence = p_z;
	DrawObject(m_viewData[p_index]);
}

// FUNCTION: LEMBALL 0x00440490
void C2D::DrawZBuff_Anim(int p_index, unsigned short p_z)
{
	AnimSpecialEntry* animation = m_zBufferAnimations + p_index;
	int gameX = (unsigned short) animation->m_x << GROUND_BLOCK_PIXEL_SHIFT;
	int gameY = (unsigned short) animation->m_y << GROUND_BLOCK_PIXEL_SHIFT;
	CGround* ground = animation->m_groundEntry;
	int height = ground->m_height;
	unsigned short collision = ground->m_collision;
	unsigned short frame = ground->m_objectData;
	int cliff = (short) ground->m_cliff;
	eObjectType groundType = ground->m_objectType;
	int screenX;
	int screenY;

	m_map->GameToScreen(gameX, gameY, screenX, screenY);
	screenX -= m_viewOriginX;
	screenY -= m_viewOriginY;
	m_lemmingAnims->m_primitiveSequence = p_z;
	if ((collision & GROUND_COLLISION_SPECIAL_RENDER) != 0 && height > 0) {
		DrawCliff(screenX, screenY, height, cliff);
	}
	DrawGround(screenX, screenY - height, groundType, frame);
}

// FUNCTION: LEMBALL 0x00440560
void C2D::DrawObjectsZBuff()
{
	CVSRect backgroundBounds;
	backgroundBounds.m_width = m_clipSize.m_x;
	backgroundBounds.m_height = m_clipSize.m_y;
	backgroundBounds.m_y = 0;
	backgroundBounds.m_x = 0;
	int primitiveIndex = m_primitiveCount++;
	CClipRect& background = m_clipRects[primitiveIndex];
	memcpy(&background.m_bounds.m_width, &backgroundBounds.m_width, sizeof(short));
	memcpy(&background.m_bounds.m_height, &backgroundBounds.m_height, sizeof(short));
	memcpy(&background.m_bounds.m_x, &backgroundBounds.m_x, sizeof(short));
	memcpy(&background.m_bounds.m_y, &backgroundBounds.m_y, sizeof(short));
	background.m_flags = 0;
	background.Draw(m_gdi);

	{
		C3DVector position;
		int viewIndex = 0;
		for (;;) {
			if ((int) m_viewDataCount <= viewIndex) {
				break;
			}
			CViewData* viewData = m_viewData + viewIndex;
			viewData->m_gameX = (short) viewData->m_positionX;
			viewData->m_gameY = (short) viewData->m_positionY;
			memcpy(&position, &m_viewData[viewIndex].m_positionX, sizeof(position));
			m_map->GameToScreen(position.m_xFixed, position.m_yFixed);
			position.m_yFixed -= position.m_zFixed;
			position.m_xFixed -= m_viewOriginX;
			position.m_yFixed -= m_viewOriginY;
			memcpy(&m_viewData[viewIndex].m_positionX, &position, sizeof(position));
			viewIndex++;
		}
	}
	SortViewData();
	m_lemmingAnims->m_drawFlags = ZRLE_DRAW_FLAG_QUICK_Z_BUFFER;

	CAnimSpecial* animations = m_ai->m_animSpecial;
	m_zBufferAnimationCount = animations->m_entryCount;
	m_zBufferAnimations = animations->m_entries;
	int spriteIndex = 0;
	int animationIndex = 0;
	bool spriteZValid = false;
	bool animationZValid = false;
	unsigned short spriteZ;
	unsigned short animationZ;

	while (spriteIndex < (int) m_viewDataCount && animationIndex < m_zBufferAnimationCount) {
		if (!spriteZValid) {
			spriteZ = CalcZValue_Sprite(spriteIndex);
			spriteZValid = true;
		}
		if (!animationZValid) {
			AnimSpecialEntry* animation = m_zBufferAnimations + animationIndex;
			animationZ = animation->m_groundEntry->m_height + animation->m_sortKey;
			animationZValid = true;
		}
		if (animationZ < spriteZ) {
			DrawZBuff_Anim(animationIndex, animationZ);
			animationIndex++;
			animationZValid = false;
		}
		else {
			DrawZBuff_Sprite(spriteIndex, spriteZ);
			spriteIndex++;
			spriteZValid = false;
		}
	}

	while (spriteIndex < (int) m_viewDataCount) {
		unsigned short z = CalcZValue_Sprite(spriteIndex);
		DrawZBuff_Sprite(spriteIndex, z);
		spriteIndex++;
	}

	while (animationIndex < m_zBufferAnimationCount) {
		AnimSpecialEntry* animation = m_zBufferAnimations + animationIndex;
		unsigned short z = animation->m_groundEntry->m_height + animation->m_sortKey;
		DrawZBuff_Anim(animationIndex, z);
		animationIndex++;
	}
}

// FUNCTION: LEMBALL 0x004407e0
unsigned short C2D::CalcZValue_Sprite(int p_index)
{
	eObjectType objectType = m_viewData[p_index].m_objectType;
	if (objectType == OBJECT_TRAP_DOOR) {
		return SPRITE_SORT_CODE_TOPMOST;
	}

	CViewData* viewData = m_viewData + p_index;
	unsigned short z = (unsigned short) viewData->m_positionZ;
	return CalcGroundCode(objectType, (unsigned short) viewData->m_gameX, (unsigned short) viewData->m_gameY, z) + z +
		   1;
}

// FUNCTION: LEMBALL 0x00440840
unsigned short C2D::CalcGroundCode(eObjectType p_objectType, int p_x, int p_y, unsigned short p_z)
{
	bool baseCodeOnly = false;
	switch (p_objectType) {
	case OBJECT_TRAP_DOOR:
		return SPRITE_SORT_CODE_SPECIAL_OBJECT;
	case OBJECT_DOOR_1:
	case OBJECT_DOOR_2:
		p_x += GROUND_BLOCK_PIXEL_SIZE;
		p_y += GROUND_BLOCK_PIXEL_SIZE;
		baseCodeOnly = true;
		break;
	case OBJECT_HAND:
		return SPRITE_SORT_CODE_SPECIAL_OBJECT;
	case OBJECT_TRAMPOLINE:
		baseCodeOnly = true;
		break;
	case OBJECT_MOVER:
		p_x = (p_x - GROUND_BLOCK_PIXEL_HALF_SIZE) & ~GROUND_BLOCK_PIXEL_MASK;
		p_y = (p_y - GROUND_BLOCK_PIXEL_HALF_SIZE) & ~GROUND_BLOCK_PIXEL_MASK;
		break;
	}

	unsigned short tileX = (unsigned short) (p_x / GROUND_BLOCK_PIXEL_SIZE);
	unsigned short tileY = (unsigned short) (p_y / GROUND_BLOCK_PIXEL_SIZE);
	unsigned short code = (unsigned short) ((tileY + tileX) * 0x40 + 4);
	if (baseCodeOnly) {
		return code;
	}

	p_z += 2;

	unsigned short southZValue;
	unsigned short& southZ = southZValue;
	int southTileX = p_x >> GROUND_BLOCK_PIXEL_SHIFT;
	int southTileY = (p_y + GROUND_BLOCK_PIXEL_SIZE) >> GROUND_BLOCK_PIXEL_SHIFT;
	{
		CMap* map = m_map;
		int width;
		if (p_x < 0 || p_y + GROUND_BLOCK_PIXEL_SIZE < 0 || (width = map->m_ground.m_width, width <= southTileX) ||
			map->m_ground.m_height <= southTileY) {
			southZ = 0;
		}
		else {
			int sampleY;
			int& cellY = sampleY;
			int cellX = p_x;
			cellY = p_y;
			cellY &= 0xf;
			width *= southTileY;
			cellX &= 0xf;
			southZ = (map->m_ground.m_ground + width + southTileX)->GetZ(cellX, cellY);
		}
	}

	unsigned short eastZ;
	int eastTileX = (p_x + GROUND_BLOCK_PIXEL_SIZE) >> GROUND_BLOCK_PIXEL_SHIFT;
	int eastTileY = p_y >> GROUND_BLOCK_PIXEL_SHIFT;
	{
		CMap* map = m_map;
		if (p_x + GROUND_BLOCK_PIXEL_SIZE < 0 || p_y < 0 || map->m_ground.m_width <= eastTileX ||
			map->m_ground.m_height <= eastTileY) {
			eastZ = 0;
		}
		else {
			int sampleY;
			int& cellY = sampleY;
			int cellX = p_x;
			cellY = p_y;
			cellY &= 0xf;
			eastTileY *= map->m_ground.m_width;
			cellX &= 0xf;
			eastZ = (map->m_ground.m_ground + eastTileY + eastTileX)->GetZ(cellX, cellY);
		}
	}

	unsigned short southeastZ;
	{
		CMap* map = m_map;
		int width;
		if (p_x + GROUND_BLOCK_PIXEL_SIZE < 0 || p_y + GROUND_BLOCK_PIXEL_SIZE < 0 ||
			(width = map->m_ground.m_width, width <= eastTileX) || map->m_ground.m_height <= southTileY) {
			southeastZ = 0;
		}
		else {
			p_x &= 0xf;
			p_y &= 0xf;
			width *= southTileY;
			southeastZ = (map->m_ground.m_ground + width + eastTileX)->GetZ(p_x, p_y);
		}
	}

	{
		const int& southWithinZ = p_z >= southZ;
		bool eastWithinZ = p_z >= eastZ;
		bool southeastWithinZ = p_z >= southeastZ;
		int threshold = (int) p_z - 0x18;
		bool southSolid;
		if ((int) southZ < threshold) {
		southClear:
			southSolid = false;
		}
		else {
			int collisionY = tileY + 1;
			unsigned short collision;
			if (collisionY < 0) {
				collision = GROUND_COLLISION_OUT_OF_BOUNDS;
			}
			else {
				CMap* map = m_map;
				int collisionX = tileX;
				int width = map->m_ground.m_width;
				if (width <= collisionX || map->m_ground.m_height <= collisionY) {
					collision = GROUND_COLLISION_OUT_OF_BOUNDS;
				}
				else {
					collision = map->m_ground.m_ground[width * collisionY + collisionX].m_collision;
				}
			}
			if ((collision & GROUND_COLLISION_BLOCKS_WALKING) == 0) {
				goto southClear;
			}
			southSolid = true;
		}

		bool eastSolid;
		if ((int) eastZ < threshold) {
		eastClear:
			eastSolid = false;
		}
		else {
			int collisionX = tileX;
			unsigned short collision;
			if (collisionX + 1 < 0) {
				collision = GROUND_COLLISION_OUT_OF_BOUNDS;
			}
			else {
				CMap* map = m_map;
				int width = map->m_ground.m_width;
				if (width <= collisionX + 1 || map->m_ground.m_height <= tileY) {
					collision = GROUND_COLLISION_OUT_OF_BOUNDS;
				}
				else {
					collision = map->m_ground.m_ground[width * tileY + collisionX + 1].m_collision;
				}
			}
			if ((collision & GROUND_COLLISION_BLOCKS_WALKING) == 0) {
				goto eastClear;
			}
			eastSolid = true;
		}

		bool southeastSolid;
		if ((int) southeastZ < threshold) {
		southeastClear:
			southeastSolid = false;
		}
		else {
			int collisionX = tileX;
			int collisionY = tileY + 1;
			unsigned short collision;
			if (collisionX + 1 < 0 || collisionY < 0) {
				collision = GROUND_COLLISION_OUT_OF_BOUNDS;
			}
			else {
				CMap* map = m_map;
				int width = map->m_ground.m_width;
				if (width <= collisionX + 1 || map->m_ground.m_height <= collisionY) {
					collision = GROUND_COLLISION_OUT_OF_BOUNDS;
				}
				else {
					collision = map->m_ground.m_ground[width * collisionY + collisionX + 1].m_collision;
				}
			}
			if ((collision & GROUND_COLLISION_BLOCKS_WALKING) == 0) {
				goto southeastClear;
			}
			southeastSolid = true;
		}
		if (southWithinZ && eastWithinZ && southeastWithinZ && !southeastSolid && !eastSolid && !southSolid) {
			code += 0x80;
		}
		else if (southWithinZ || eastWithinZ) {
			code += 0x40;
		}
		return code;
	}
}

// FUNCTION: LEMBALL 0x00440c00
void C2D::InitSpriteGroundLU()
{
}
