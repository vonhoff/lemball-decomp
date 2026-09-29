#include "CGunController.h"

#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Animation/CPlayThruAnim.h"
#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/CVsPoint.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/CGDI.h"
#include "../../Visos/Graphics/CGraphicButton.h"
#include "../../Visos/Graphics/CSurface.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Windows/CSpriteWindow.h"
#include "../Windows/CTrackWindow.h"
#include "CGunButtons.h"
#include "CTrackerButton.h"
#include "Frontend/Controls/GunControllerJunction.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Animation/CAnimsManager.h"
#include "Visos/Animation/CStaticAnim.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CGWnd.h"
#include "Visos/Graphics/CSolidRect.h"

#include <stdlib.h>

class CAnimFrameBASE;

int sgn(int p_value);

// GLOBAL: LEMBALL 0x004a7b38
unsigned long g_gunEffectLeftResourceId = 0;
// GLOBAL: LEMBALL 0x004a7b3c
unsigned long g_gunBulletRightResourceId = 0;
// GLOBAL: LEMBALL 0x004a7b40
unsigned long g_gunFireLeftResourceId = 0;
// GLOBAL: LEMBALL 0x004a7b44
unsigned long g_gunBulletLeftResourceId = 0;
// GLOBAL: LEMBALL 0x004a7b48
unsigned long g_gunEffectRightResourceId = 0;
// GLOBAL: LEMBALL 0x004a7b4c
unsigned long g_gunSplatRightResourceId = 0;
// GLOBAL: LEMBALL 0x004a7b50
unsigned long g_gunFireRightResourceId = 0;
// GLOBAL: LEMBALL 0x004a7b54
unsigned long g_gunTurnResourceId = 0;
// GLOBAL: LEMBALL 0x004a7b58
unsigned long g_gunSplatLeftResourceId = 0;

// GLOBAL: LEMBALL 0x0049fa70
int g_anGunSpriteOffset[18] = {0, 12, -8, -2, 52, -2, -13, 28, 116, 0, 0, -13, 19, 28, -16, 0, -24, -2};
// GLOBAL: LEMBALL 0x0049fab8
int g_anGunSpriteOffsetCompact[20] = {0, 6, -4, -2, 26, -2, -7, 14, 58, 0, 0, 0, 10, 14, -8, 0, -12, -2, 0, 0};

// FUNCTION: LEMBALL 0x0044c870
CGunController::CGunController(CGWnd* p_window, CGDI* p_gdi, int p_arg2, unsigned int p_mode)
	: CAnimsManager(p_gdi, 0x2b6, 10, 5, 0, 0)
{
	int i;

	m_buttonsActive = 1;
	m_mode = p_mode;
	m_window = p_window;
	m_gdi = p_gdi;
	m_nextMessageId = 0xabcd0000;
	i = 0;
	while (i < 8) {
		m_junctions[i].m_y = -1;
		m_junctions[i].m_leftX = -1;
		m_junctions[i].m_direction = 3;
		m_buttons[i] = 0;
		i = i + 1;
	}
	m_controllerActive = 0;
	m_buttonCount = 0;
	m_verticalMoving = 0;
	m_selectionState = 0;
	m_messageSent = 0;
	g_pMasterInputQueue->Attach(this, 0);
	if (m_mode == 1) {
		m_alternateAssets = 1;
		g_gunBulletLeftResourceId = RES_NEWFRONT_ANIMS_LORES_BULLET_LEFT;
		g_gunBulletRightResourceId = RES_NEWFRONT_ANIMS_LORES_BULLET_RIGHT;
		g_gunFireLeftResourceId = RES_NEWFRONT_ANIMS_LORES_FIRE_LEFT;
		g_gunFireRightResourceId = RES_NEWFRONT_ANIMS_LORES_FIRE_RIGHT;
		g_gunEffectLeftResourceId = RES_NEWFRONT_ANIMS_LORES_FX_LEFT;
		g_gunEffectRightResourceId = RES_NEWFRONT_ANIMS_LORES_FX_RIGHT;
		g_gunTurnResourceId = RES_NEWFRONT_ANIMS_LORES_GUNTURN;
		g_gunSplatLeftResourceId = RES_NEWFRONT_ANIMS_LORES_SPLAT_LEFT;
		g_gunSplatRightResourceId = RES_NEWFRONT_ANIMS_LORES_SPLAT_RIGHT;
	}
	else {
		m_alternateAssets = 0;
		g_gunBulletLeftResourceId = RES_NEWFRONT_ANIMS_HIRES_BULLET_LEFT;
		g_gunBulletRightResourceId = RES_NEWFRONT_ANIMS_HIRES_BULLET_RIGHT;
		g_gunFireLeftResourceId = RES_NEWFRONT_ANIMS_HIRES_FIRE_LEFT;
		g_gunFireRightResourceId = RES_NEWFRONT_ANIMS_HIRES_FIRE_RIGHT;
		g_gunEffectLeftResourceId = RES_NEWFRONT_ANIMS_HIRES_FX_LEFT;
		g_gunEffectRightResourceId = RES_NEWFRONT_ANIMS_HIRES_FX_RIGHT;
		g_gunTurnResourceId = RES_NEWFRONT_ANIMS_HIRES_GUNTURN;
		g_gunSplatLeftResourceId = RES_NEWFRONT_ANIMS_HIRES_SPLAT_LEFT;
		g_gunSplatRightResourceId = RES_NEWFRONT_ANIMS_HIRES_SPLAT_RIGHT;
	}
	CAnimsManager::LoadAnims(g_gunBulletLeftResourceId);
	CAnimsManager::LoadAnims(g_gunBulletRightResourceId);
	CAnimsManager::LoadAnims(g_gunFireLeftResourceId);
	CAnimsManager::LoadAnims(g_gunFireRightResourceId);
	CAnimsManager::LoadAnims(g_gunEffectLeftResourceId);
	CAnimsManager::LoadAnims(g_gunEffectRightResourceId);
	CAnimsManager::LoadAnims(g_gunTurnResourceId);
	CAnimsManager::LoadAnims(g_gunSplatLeftResourceId);
	CAnimsManager::LoadAnims(g_gunSplatRightResourceId);
	m_sideAnim = new CPlayThruAnim(CAnimsManager::GetnAnims(g_gunTurnResourceId), 1);
	m_sideAnim->m_fixedTime = 0xffffffff;
	m_leftShotAnim = new CPlayThruAnim(CAnimsManager::GetnAnims(g_gunEffectLeftResourceId), 1);
	m_leftShotAnim->m_fixedTime = 0xffffffff;
	m_cursorAnim = new CPlayThruAnim(CAnimsManager::GetnAnims(g_gunBulletLeftResourceId), 1);
	m_cursorAnim->m_fixedTime = 0xffffffff;
	m_rightShotAnim = new CPlayThruAnim(CAnimsManager::GetnAnims(g_gunSplatLeftResourceId), 1);
	m_rightShotAnim->m_fixedTime = 0xffffffff;
	m_hitAnim = new CPlayThruAnim(CAnimsManager::GetnAnims(g_gunFireLeftResourceId), 1);
	m_hitAnim->m_fixedTime = 0xffffffff;
	m_inputReadyTime = CurrentQueueTimer();
}

// FUNCTION: LEMBALL 0x0044cc90
void CGunController::ActivateButtons(int p_active)
{
	int i;
	int buttonCount;
	buttonCount = m_buttonCount;

	m_buttonsActive = p_active;
	i = 0;
	if (buttonCount > i) {
		do {
			CGunButtons* button = m_buttons[i];
			if (button != 0) {
				button->m_active = p_active;
				button->m_graphicButton->SetActive(p_active);
			}
			i = i + 1;
			buttonCount = m_buttonCount;
		} while (buttonCount > i);
	}
}

// FUNCTION: LEMBALL 0x0044cce0
void CGunController::SetSpriteWindow()
{
	m_spriteWindow = new CSpriteWindow();
	CVsRect createRect(m_window->m_rect);
	createRect.m_x = 0;
	createRect.m_y = 0;
	m_spriteWindow->Create(createRect, m_window, 0);
	m_spriteSurface = m_spriteWindow->m_gdi;
}

// FUNCTION: LEMBALL 0x0044cd60
CGunController::~CGunController()
{
	int i;

	g_pMasterInputQueue->Detach(this, 0);
	i = 0;
	while (i < 8) {
		if (m_buttons[i] != 0) {
			delete m_buttons[i];
		}
		i = i + 1;
	}
	CAnimsManager::UnLoadAnims(g_gunBulletLeftResourceId);
	CAnimsManager::UnLoadAnims(g_gunBulletRightResourceId);
	CAnimsManager::UnLoadAnims(g_gunFireLeftResourceId);
	CAnimsManager::UnLoadAnims(g_gunFireRightResourceId);
	CAnimsManager::UnLoadAnims(g_gunEffectLeftResourceId);
	CAnimsManager::UnLoadAnims(g_gunEffectRightResourceId);
	CAnimsManager::UnLoadAnims(g_gunTurnResourceId);
	CAnimsManager::UnLoadAnims(g_gunSplatLeftResourceId);
	CAnimsManager::UnLoadAnims(g_gunSplatRightResourceId);
	delete m_sideAnim;
	delete m_leftShotAnim;
	delete m_cursorAnim;
	delete m_rightShotAnim;
	delete m_hitAnim;
	if (m_spriteWindow->m_lifecycleRefs == 1) {
		m_spriteWindow->Destroy();
	}
	delete m_spriteWindow;
}

// FUNCTION: LEMBALL 0x0044cec0
int CGunController::ProcessMsg(Message* p_message)
{
	if ((int) (p_message->m_time - m_inputReadyTime) < 0) {
		return 0;
	}
	switch ((unsigned int) p_message->m_type) {
	case 4:
		break;
	default:
		m_processedCount = m_processedCount + 1;
		return 0;
	}
	switch (p_message->m_code) {
	case 1:
		MoveUp();
		return 1;
	case 2:
		MoveDown();
		return 1;
	case 3:
		MoveLeft();
		return 1;
	case 4:
		MoveRight();
		return 1;
	case 0x1f:
	case 0x22:
	case 0x4c:
		SelectOption();
		break;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0044cfb0
void CGunController::AddButton(int p_x,
							   int p_y,
							   unsigned long* p_animIds,
							   unsigned int p_postAction,
							   int p_minimum,
							   int p_maximum,
							   int p_value,
							   void* p_binding,
							   unsigned long p_actionMessage)
{

	unsigned long controlMessage;

	controlMessage = m_nextMessageId + 1;
	m_nextMessageId = controlMessage;
	m_buttons[m_buttonCount] = new CGunButtons(m_window,
											   m_gdi,
											   p_x,
											   p_y,
											   p_animIds,
											   p_postAction,
											   p_minimum,
											   p_maximum,
											   p_value,
											   controlMessage,
											   p_binding,
											   p_actionMessage);
	unsigned int side = 0 < p_maximum;
	AddJunction(p_x, p_y, side, m_buttons[m_buttonCount]->m_controlMessage);
	m_buttonCount = m_buttonCount + 1;
}

// FUNCTION: LEMBALL 0x0044d080
void CGunController::AddButtonWithRect(int p_x,
									   int p_y,
									   unsigned long* p_animIds,
									   unsigned int p_postAction,
									   unsigned int p_unusedFirst,
									   unsigned int p_unusedSecond,
									   int p_value,
									   int* p_binding,
									   const CVsRect& p_rect,
									   int p_actionMessage,
									   int p_context)
{
	unsigned int message = ++m_nextMessageId;
	m_buttons[m_buttonCount] = new CGunButtons(p_rect,
											   m_window,
											   m_gdi,
											   p_x,
											   p_y,
											   p_animIds,
											   p_postAction,
											   p_value,
											   message,
											   p_binding,
											   p_actionMessage);
	AddJunction(p_x, p_y, 1, m_buttons[m_buttonCount]->m_controlMessage);
	m_buttons[m_buttonCount]->m_trackerButton->m_trackWindow->m_contextId = p_context;
	m_buttonCount++;
}

// FUNCTION: LEMBALL 0x0044d150
void CGunController::AddJunction(int p_x, int p_y, unsigned int p_side, unsigned long p_message)
{
	int centreX = (short) ((int) m_window->m_rect.m_width / 2);
	int side;
	if (p_x < centreX) {
		side = 0;
	}
	else if (p_x > centreX) {
		side = 1;
	}
	else {
		side = p_side;
	}
	int junctionIndex = -1;
	for (int i = 0; i < 8; i++) {
		if (m_junctions[i].m_direction != 3 && m_junctions[i].m_y == p_y) {
			junctionIndex = i;
		}
	}
	if (junctionIndex != -1) {
		m_junctions[junctionIndex].m_direction = 2;
		if (side == 0) {
			m_junctions[junctionIndex].m_leftMessage = p_side;
			m_junctions[junctionIndex].m_leftBinding = (void*) p_message;
			m_junctions[junctionIndex].m_leftX = p_x;
			return;
		}
		m_junctions[junctionIndex].m_rightMessage = p_side;
		m_junctions[junctionIndex].m_rightBinding = (void*) p_message;
		m_junctions[junctionIndex].m_rightX = p_x;
		return;
	}
	for (int j = 0; j < 8; j++) {
		if (m_junctions[j].m_direction == 3) {
			m_junctions[j].m_y = p_y;
			m_junctions[j].m_direction = side;
			if (side == 0) {
				m_junctions[j].m_leftMessage = p_side;
				m_junctions[j].m_leftX = p_x;
				m_junctions[j].m_leftBinding = (void*) p_message;
				return;
			}
			m_junctions[j].m_rightMessage = p_side;
			m_junctions[j].m_rightBinding = (void*) p_message;
			m_junctions[j].m_rightX = p_x;
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x0044d290
void CGunController::DrawButtons(int p_firstState, int p_secondState)
{
	CGunButtons** button = m_buttons;
	int remaining = 8;
	do {
		if (*button != 0) {
			(*button)->Draw(p_firstState, p_secondState);
		}
		++button;
	} while (--remaining != 0);
}

// FUNCTION: LEMBALL 0x0044d2d0
void CGunController::DrawSpriteWindow()
{
	CGDI* previousGdi;
	int* offsets;
	CVsPoint position;
	unsigned long frame;

	m_spriteSurface->m_renderTarget->GetCurrDB();
	m_cursorRect[0].m_bounds.m_width = m_spriteSurface->m_renderTarget->m_windowRect.m_width;
	m_cursorRect[0].m_bounds.m_height = m_spriteSurface->m_renderTarget->m_windowRect.m_height;
	m_cursorRect[0].m_bounds.m_x = 0;
	m_cursorRect[0].m_bounds.m_y = 0;
	m_cursorRect[0].m_colour = 0x10000;
	m_cursorRect[0].Draw(m_spriteSurface);
	offsets = g_anGunSpriteOffsetCompact;
	if (m_alternateAssets != 1) {
		offsets = g_anGunSpriteOffset;
	}
	position.m_x = 0;
	position.m_y = 0;
	switch (m_selectionState) {
	case 0:
		frame = 0;
		if (m_currentSide != 0) {
			frame = CAnimsManager::GetnAnims(g_gunTurnResourceId) - 1;
		}
		m_staticAnim.m_frameState = frame;
		position.m_x = (short) (m_gunX + offsets[0]);
		position.m_y = (short) (offsets[1] + m_gunY);
		previousGdi = CAnimsManager::m_gdi;
		CAnimsManager::m_gdi = m_spriteSurface;
		CAnimsManager::DrawAnim(position, g_gunTurnResourceId, 0, (CAnimFrameBASE*) &m_staticAnim, 0);
		CAnimsManager::m_gdi = previousGdi;
		break;
	case 1:
		position.m_x = (short) (m_gunX + offsets[0]);
		position.m_y = (short) (offsets[1] + m_gunY);
		previousGdi = CAnimsManager::m_gdi;
		CAnimsManager::m_gdi = m_spriteSurface;
		CAnimsManager::DrawAnim(position, g_gunTurnResourceId, 0, (CAnimFrameBASE*) m_sideAnim, 0);
		CAnimsManager::m_gdi = previousGdi;
		break;
	case 2:
		previousGdi = CAnimsManager::m_gdi;
		CAnimsManager::m_gdi = m_spriteSurface;
		if (m_targetSide == 0) {
			position.m_x = (short) m_projectileX;
			position.m_y = (short) m_projectileY;
			CAnimsManager::DrawAnim(position, g_gunBulletLeftResourceId, 0, (CAnimFrameBASE*) m_cursorAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
			previousGdi = CAnimsManager::m_gdi;
			CAnimsManager::m_gdi = m_spriteSurface;
			position.m_x = (short) (m_gunX + offsets[0]);
			position.m_y = (short) (m_gunY + offsets[1]);
			CAnimsManager::DrawAnim(position, g_gunFireLeftResourceId, 0, (CAnimFrameBASE*) m_hitAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
			previousGdi = CAnimsManager::m_gdi;
			CAnimsManager::m_gdi = m_spriteSurface;
			position.m_x = (short) (m_gunX + offsets[2]);
			position.m_y = (short) (m_gunY + offsets[3]);
			CAnimsManager::DrawAnim(position, g_gunEffectLeftResourceId, 0, (CAnimFrameBASE*) m_leftShotAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
		}
		else {
			position.m_x = (short) m_projectileX;
			position.m_y = (short) m_projectileY;
			CAnimsManager::DrawAnim(position, g_gunBulletRightResourceId, 0, (CAnimFrameBASE*) m_cursorAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
			previousGdi = CAnimsManager::m_gdi;
			CAnimsManager::m_gdi = m_spriteSurface;
			position.m_x = (short) (m_gunX + offsets[0]);
			position.m_y = (short) (m_gunY + offsets[1]);
			CAnimsManager::DrawAnim(position, g_gunFireRightResourceId, 0, (CAnimFrameBASE*) m_hitAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
			previousGdi = CAnimsManager::m_gdi;
			CAnimsManager::m_gdi = m_spriteSurface;
			position.m_x = (short) (m_gunX + offsets[4]);
			position.m_y = (short) (m_gunY + offsets[5]);
			CAnimsManager::DrawAnim(position, g_gunEffectRightResourceId, 0, (CAnimFrameBASE*) m_leftShotAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
		}
		break;
	case 3:
		previousGdi = CAnimsManager::m_gdi;
		CAnimsManager::m_gdi = m_spriteSurface;
		if (m_targetSide == 0) {
			position.m_x = (short) m_projectileEndX;
			position.m_y = (short) m_projectileEndY;
			CAnimsManager::DrawAnim(position, g_gunSplatLeftResourceId, 0, (CAnimFrameBASE*) m_rightShotAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
			previousGdi = CAnimsManager::m_gdi;
			CAnimsManager::m_gdi = m_spriteSurface;
			position.m_x = (short) (m_gunX + offsets[0]);
			position.m_y = (short) (m_gunY + offsets[1]);
			CAnimsManager::DrawAnim(position, g_gunFireLeftResourceId, 0, (CAnimFrameBASE*) m_hitAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
		}
		else {
			position.m_x = (short) m_projectileEndX;
			position.m_y = (short) m_projectileEndY;
			CAnimsManager::DrawAnim(position, g_gunSplatRightResourceId, 0, (CAnimFrameBASE*) m_rightShotAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
			previousGdi = CAnimsManager::m_gdi;
			CAnimsManager::m_gdi = m_spriteSurface;
			position.m_x = (short) (m_gunX + offsets[0]);
			position.m_y = (short) (m_gunY + offsets[1]);
			CAnimsManager::DrawAnim(position, g_gunFireRightResourceId, 0, (CAnimFrameBASE*) m_hitAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
		}
		break;
	}
	CAnimsManager::ResetPrimitives();
}

// FUNCTION: LEMBALL 0x0044d8e0
void CGunController::MoveDown()
{
	int remaining;
	int foundY;
	int* directionField;
	int direction;
	int y;
	int bestY;

	bestY = 999999;
	foundY = -1;
	directionField = &m_junctions[0].m_direction;
	remaining = 8;
	do {
		direction = *directionField;
		if (direction != 3 && m_targetY < (y = directionField[-2]) && y < bestY) {
			if (direction != 2) {
				m_targetSide = direction;
			}
			foundY = directionField[-2];
			bestY = foundY;
			g_pSoundView->PlayEffect(SFX_RELOAD);
		}
		directionField += 8;
	} while (--remaining != 0);
	if (foundY != -1) {
		m_targetY = foundY;
	}
	m_moveStartTime = CurrentMilliTimer();
	m_moveEndTime = abs(m_targetY - m_gunY) * 3 + m_moveStartTime;
	m_moveStartY = m_gunY;
	m_verticalMoving = 1;
}

// FUNCTION: LEMBALL 0x0044d990
void CGunController::MoveLeft()
{
	int i;

	if (m_currentSide == m_targetSide) {
		i = 0;
		while (i < 8) {
			if (m_junctions[i].m_y == m_targetY &&
				(m_junctions[i].m_direction == 0 || m_junctions[i].m_direction == 2)) {
				m_targetSide = 0;
			}
			i = i + 1;
		}
	}
}

// FUNCTION: LEMBALL 0x0044d9e0
void CGunController::MoveRight()
{
	int i;

	if (m_currentSide == m_targetSide) {
		i = 0;
		while (i < 8) {
			if (m_junctions[i].m_y == m_targetY &&
				(m_junctions[i].m_direction == 1 || m_junctions[i].m_direction == 2)) {
				m_targetSide = 1;
			}
			i = i + 1;
		}
	}
}

// FUNCTION: LEMBALL 0x0044da30
void CGunController::SetGunPosition(int p_x, int p_y, int p_side)
{
	m_gunX = p_x;
	m_gunY = p_y;
	m_moveStartY = p_y;
	m_currentSide = p_side;
	m_selectionStartX = p_x;
	m_targetY = p_y;
	m_targetSide = p_side;
}

// FUNCTION: LEMBALL 0x0044da70
void CGunController::SetGun(int p_junction)
{
	int direction;

	const CVsSize& animSize = CAnimsManager::GetAnimSize(g_gunTurnResourceId, 0);
	m_gunX = (int) (m_window->m_rect.m_width / 2) - (int) (animSize.m_width / 2);
	m_gunY = m_junctions[p_junction].m_y;
	direction = m_junctions[p_junction].m_direction;
	if (direction == 0 || direction != 1) {
		m_currentSide = 0;
	}
	else {
		m_currentSide = 1;
	}
	m_selectionStartX = m_gunX;
	m_targetY = m_gunY;
	m_selectionState = 0;
	m_targetSide = m_currentSide;
}

// FUNCTION: LEMBALL 0x0044db20
void CGunController::SelectOption()
{
	int i;
	int* targetX;
	int* offsets;
	unsigned int delta;

	m_selectionMessage.m_type = 0xc;
	if (m_gunY == m_targetY && m_selectionState == 0) {
		i = 0;
		while (i < 8) {
			if (m_junctions[i].m_y == m_targetY) {
				if (m_targetSide == 0) {
					m_selectionMessage.m_code = (int) m_junctions[i].m_leftBinding;
					m_selectedMessage = m_junctions[i].m_leftMessage;
				}
				else {
					m_selectionMessage.m_code = (int) m_junctions[i].m_rightBinding;
					m_selectedMessage = m_junctions[i].m_rightMessage;
				}
				break;
			}
			i = i + 1;
		}
		offsets = g_anGunSpriteOffsetCompact;
		if (m_alternateAssets != 1) {
			offsets = g_anGunSpriteOffset;
		}
		m_cursorAnim->StartAnim(0xfa);
		m_hitAnim->StartAnim(500);
		m_leftShotAnim->StartAnim(500);
		targetX = &m_projectileTargetX;
		if (m_targetSide == 0) {
			m_projectileX = offsets[6] + m_selectionStartX;
			m_projectileY = offsets[7] + m_targetY;
			*targetX = offsets[8] + m_junctions[i].m_leftX;
			m_projectileEndX = offsets[10] + m_junctions[i].m_leftX;
			m_projectileEndY = offsets[11] + m_junctions[i].m_y;
		}
		else {
			m_projectileX = offsets[12] + m_selectionStartX;
			m_projectileY = offsets[13] + m_targetY;
			*targetX = offsets[14] + m_junctions[i].m_rightX;
			m_projectileEndX = offsets[16] + m_junctions[i].m_rightX;
			m_projectileEndY = offsets[17] + m_junctions[i].m_y;
		}
		m_selectStartTime = CurrentMilliTimer();
		delta = *targetX - m_projectileX;
		delta = abs((int) delta);
		m_selectEndTime = delta * 2 + m_selectStartTime;
		m_selectionState = 2;
		g_pSoundView->PlayEffect(SFX_BIGGUN);
	}
}

// FUNCTION: LEMBALL 0x0044dce0
void CGunController::Process()
{
	unsigned long now;
	unsigned long fireTime;
	int step;
	unsigned long elapsed;
	now = CurrentMilliTimer();
	if (m_currentSide != m_targetSide && m_selectionState != 1) {
		m_sideStartTime = now;
		m_sideEndTime = now + 0xfa;
		m_selectionState = 1;
		m_sideAnim->StartAnim(0xfa);
		if (m_targetSide == 0) {
			m_sideAnim->SetAnimDirection(0xffffffff);
		}
		else {
			m_sideAnim->SetAnimDirection(1);
		}
	}
	else {
		switch (m_selectionState) {
		case 1:
			if (m_sideEndTime <= now) {
				m_selectionState = 0;
				m_currentSide = m_targetSide;
			}
			break;
		case 2:
			if (m_selectEndTime <= now) {
				g_pSoundView->PlayEffect(SFX_GUNHIT);
				m_fireStartTime = now;
				m_fireEndTime = now + 500;
				m_selectionState = 3;
				m_rightShotAnim->StartAnim(500);
			}
			else {
				elapsed = (now - m_selectStartTime) >> 1;
				step = m_projectileX;
				int direction = sgn(m_projectileTargetX - step);
				m_selectStartTime = now;
				m_projectileX = direction * elapsed + step;
			}
			break;
		case 3:
			fireTime = m_fireEndTime;
			if (m_selectedMessage != 0) {
				fireTime -= 0x177;
			}
			if (fireTime <= now && m_messageSent != 1) {
				m_selectionMessage.m_time = CurrentQueueTimer();
				g_pMasterInputQueue->Post(m_selectionMessage);
				m_messageSent = 1;
			}
			if (m_fireEndTime <= now) {
				m_selectionState = 0;
				m_messageSent = 0;
			}
			break;
		}
	}
	if (m_verticalMoving != 0) {
		if (now < m_moveEndTime) {
			m_gunY = (int) ((m_targetY - m_moveStartY) * (int) (now - m_moveStartTime)) /
						 (int) (m_moveEndTime - m_moveStartTime) +
					 m_moveStartY;
			return;
		}
		m_verticalMoving = 0;
		m_gunY = m_targetY;
	}
}
