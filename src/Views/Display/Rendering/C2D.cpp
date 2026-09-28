#include "../C2D.h"

#include "../../../Frontend/Resources/CFrontendResourceLoader.h"
#include "../../../Visos/Graphics/CGDI.h"
#include "../../../Visos/Graphics/CSurface.h"
#include "../../../Visos/Resources/Manifest.h"
#include "../../Animation/CLemmingAnimsManager.h"
#include "AI/Base/C3DVector.h"
#include "Map/Base/CMap.h"

#include <stdlib.h>
#include <string.h>

// GLOBAL: LEMBALL 0x00497044
static const short g_baseOffset[] = {40, 60};

// GLOBAL: LEMBALL 0x00497048
static const short g_animOffset[] = {40, 60};

// FUNCTION: LEMBALL 0x0043d130
void C2D::DrawCatapult(CViewData& p_viewData, int p_objectNo)
{
	int x;
	int y;
	CBaseRemap* remap;
	eAction action;
	unsigned int stateTimer;
	C2D& owner = *this;

	action = p_viewData.m_action;
	stateTimer = p_viewData.m_stateTimer;
	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;
	remap = 0;

	if (p_viewData.m_actionArgument != 0) {
		remap = m_paletteRemap;
	}

	switch (action) {
	case ACTION_READY:
		owner.m_lemmingAnims
			->DrawAnim(x - g_baseOffset[0], y - g_baseOffset[1], g_anGroundStyleResourceIds[8], 1, 0, 0);
		owner.m_lemmingAnims
			->DrawAnim(x - g_baseOffset[0], y - g_baseOffset[1], g_anGroundStyleResourceIds[8], 0, 0, 0);
		break;

	case ACTION_ACTIVATING:
		owner.m_lemmingAnims
			->DrawAnim(x - g_baseOffset[0], y - g_baseOffset[1], g_anGroundStyleResourceIds[8], 1, 0, 0);
		owner.m_lemmingAnims
			->DrawAnim(x - g_baseOffset[0], y - g_baseOffset[1], g_anGroundStyleResourceIds[8], 0, 0, 0);
		owner.m_lemmingAnims->DrawAnim(x - g_animOffset[0] - 8,
									   y - g_animOffset[1],
									   RES_GAME_CATMOUNT_SE,
									   stateTimer,
									   p_viewData.m_animationTime,
									   (CRemap*) remap);
		break;

	case ACTION_ACTIVATED:
		owner.m_lemmingAnims
			->DrawAnim(x - g_baseOffset[0], y - g_baseOffset[1], g_anGroundStyleResourceIds[8], 1, 0, 0);
		owner.m_lemmingAnims->DrawAnim(x - g_animOffset[0],
									   y - g_animOffset[1],
									   g_anGroundStyleResourceIds[9],
									   stateTimer + 0x640,
									   p_viewData.m_animationTime,
									   0);
		owner.m_lemmingAnims->DrawAnim(x - g_animOffset[0] - 8,
									   y - g_animOffset[1],
									   RES_GAME_CATMOUNT_SE,
									   stateTimer,
									   p_viewData.m_animationTime,
									   (CRemap*) remap);
		break;

	case ACTION_RUNNING:
		owner.m_lemmingAnims
			->DrawAnim(x - g_baseOffset[0], y - g_baseOffset[1], g_anGroundStyleResourceIds[8], 1, 0, 0);
		owner.m_lemmingAnims->DrawAnim(x - g_animOffset[0],
									   y - g_animOffset[1],
									   g_anGroundStyleResourceIds[9],
									   stateTimer + 0x640,
									   p_viewData.m_animationTime,
									   0);
		break;
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
	C2D& owner = *this;
	SetOrigin();
	owner.m_bitmapCount = 0;
	owner.m_lemmingAnims->m_drawFlags = 0x40000;
	int scrollX = abs((int) owner.m_scrollDeltaX);
	int scrollY = abs((int) owner.m_scrollDeltaY);
	CVsRect borders[4];

	if (owner.m_scrollPending != 0 && owner.m_redrawPending == 0 && scrollX < owner.m_clipSize.m_x &&
		scrollY < owner.m_clipSize.m_y) {
		unsigned char direction = (unsigned char) ((owner.m_scrollDeltaX < 0) | (owner.m_scrollDeltaY < 0 ? 2 : 0));
		CVsRect exposed[2];
		short destinationX = 0;
		short destinationY = 0;
		CVsRect retained;
		switch ((unsigned int) direction) {
		case 0:
			retained = CVsRect((short) scrollX,
							   (short) scrollY,
							   owner.m_clipSize.m_x - (short) scrollX,
							   owner.m_clipSize.m_y - (short) scrollY);
			if (scrollY != 0) {
				exposed[0] = CVsRect(0, 0, owner.m_clipSize.m_x, (short) scrollY);
			}
			if (scrollX != 0) {
				exposed[1] = CVsRect(0, (short) scrollY, (short) scrollX, owner.m_clipSize.m_y - (short) scrollY);
			}
			break;
		case 1:
			destinationX = (short) scrollX;
			destinationY = 0;
			retained = CVsRect(0,
							   (short) scrollY,
							   owner.m_clipSize.m_x - (short) scrollX,
							   owner.m_clipSize.m_y - (short) scrollY);
			if (scrollY != 0) {
				exposed[0] = CVsRect(0, 0, owner.m_clipSize.m_x, (short) scrollY);
			}
			if (scrollX != 0) {
				exposed[1] = CVsRect(owner.m_clipSize.m_x - (short) scrollX,
									 (short) scrollY,
									 (short) scrollX,
									 owner.m_clipSize.m_y - (short) scrollY);
			}
			if (owner.m_clipOffsetX > 0 || owner.m_clipOffsetY > 0) {
				borders[0] = CVsRect((short) owner.m_clipOffsetX - (short) scrollX,
									 (short) owner.m_clipOffsetY,
									 (short) scrollX,
									 owner.m_clipSize.m_y + (short) scrollY);
				borders[1] = CVsRect((short) owner.m_clipOffsetX,
									 owner.m_clipSize.m_y + (short) owner.m_clipOffsetY,
									 owner.m_clipSize.m_x,
									 (short) scrollY);
			}
			break;
		case 2:
			destinationY = (short) scrollY;
			destinationX = 0;
			retained = CVsRect((short) scrollX,
							   0,
							   owner.m_clipSize.m_x - (short) scrollX,
							   owner.m_clipSize.m_y - (short) scrollY);
			if (scrollY != 0) {
				exposed[0] = CVsRect(0, owner.m_clipSize.m_y - (short) scrollY, owner.m_clipSize.m_x, (short) scrollY);
			}
			if (scrollX != 0) {
				exposed[1] = CVsRect(0, 0, (short) scrollX, owner.m_clipSize.m_y - (short) scrollY);
			}
			break;
		case 3:
			destinationX = (short) scrollX;
			destinationY = (short) scrollY;
			retained = CVsRect(0, 0, owner.m_clipSize.m_x - (short) scrollX, owner.m_clipSize.m_y - (short) scrollY);
			if (scrollY != 0) {
				exposed[0] = CVsRect(0, owner.m_clipSize.m_y - (short) scrollY, owner.m_clipSize.m_x, (short) scrollY);
			}
			if (scrollX != 0) {
				exposed[1] = CVsRect(owner.m_clipSize.m_x - (short) scrollX,
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
		CVsRect* strip = exposed;
		do {
			if ((int) strip->m_width * (int) strip->m_height != 0) {
				CVsRect copyRect = *strip;
				owner.m_scrollCopyToBackBuffs[0].m_destination = copyRect;
				memset(&owner.m_scrollCopyToBackBuffs[0].m_sourceX, 0, sizeof(CVsPoint));
				owner.m_scrollCopyToBackBuffs[0].Draw(owner.m_gdi);
				DrawClippedRectangle(*strip);
			}
			strip++;
		} while (strip < exposed + 2);
	}
	else if (owner.m_scrollPending != 0 || owner.m_redrawPending != 0) {
		owner.m_gdi->m_renderTarget->ResetScroll();
		CVsRect fullRect(0, 0, (CVsSize*) &owner.m_clipSize);
		if (owner.m_redrawPending != 0) {
			fullRect = CVsRect(0, 0, owner.m_clipSize.m_x, owner.m_clipSize.m_y);
		}
		owner.m_copyToBackBuff.m_destination = fullRect;
		memset(&owner.m_copyToBackBuff.m_sourceX, 0, sizeof(CVsPoint));
		owner.m_copyToBackBuff.Draw(owner.m_gdi);
		DrawClippedRectangle(CVsRect(0, 0, owner.m_clipSize.m_x, owner.m_clipSize.m_y));
		owner.m_redrawPending = 0;
	}
	owner.m_scrollPending = 0;
	DrawObjectsZBuff();
	owner.m_lemmingAnims->m_drawFlags = 0;
}
