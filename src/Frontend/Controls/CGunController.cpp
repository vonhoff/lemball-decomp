#include "CGunController.h"

#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Animation/CPlayThruAnim.h"
#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/CVsPoint.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/CGdi.h"
#include "../../Visos/Graphics/CGraphicButton.h"
#include "../../Visos/Graphics/VsGdi.h"
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

class CFrames;

int Sgn(int p_value);

// GLOBAL: LEMBALL 0x004a7b38
unsigned long g_dwGunAnimLeftShot = 0;
// GLOBAL: LEMBALL 0x004a7b3c
unsigned long g_dwGunAnim147 = 0;
// GLOBAL: LEMBALL 0x004a7b40
unsigned long g_dwGunAnim148 = 0;
// GLOBAL: LEMBALL 0x004a7b44
unsigned long g_dwGunAnimCursor = 0;
// GLOBAL: LEMBALL 0x004a7b48
unsigned long g_dwGunAnim14b = 0;
// GLOBAL: LEMBALL 0x004a7b4c
unsigned long g_dwGunAnim14e = 0;
// GLOBAL: LEMBALL 0x004a7b50
unsigned long g_dwGunAnim149 = 0;
// GLOBAL: LEMBALL 0x004a7b54
unsigned long g_dwGunAnim14c = 0;
// GLOBAL: LEMBALL 0x004a7b58
unsigned long g_dwGunAnim14d = 0;

// GLOBAL: LEMBALL 0x0049fa70
int g_anGunSpriteOffset[18] = {0, 12, -8, -2, 52, -2, -13, 28, 116, 0, 0, -13, 19, 28, -16, 0, -24, -2};
// GLOBAL: LEMBALL 0x0049fab8
int g_anGunSpriteOffsetCompact[20] = {0, 6, -4, -2, 26, -2, -7, 14, 58, 0, 0, 0, 10, 14, -8, 0, -12, -2, 0, 0};

// FUNCTION: LEMBALL 0x0044c870
CGunController::CGunController(CGWnd* p_arg0, CGdi* p_arg1, int p_arg2, unsigned int p_arg3)
	: CAnimsManager(p_arg1, 0x2b6, 10, 5, 0, 0)
{
	int i;

	m_buttonsActive = 1;
	m_mode = p_arg3;
	m_window = p_arg0;
	m_gdi = p_arg1;
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
		g_dwGunAnimCursor = RES_NEWFRONT_ANIMS_LORES_BULLET_LEFT;
		g_dwGunAnim147 = RES_NEWFRONT_ANIMS_LORES_BULLET_RIGHT;
		g_dwGunAnim148 = RES_NEWFRONT_ANIMS_LORES_FIRE_LEFT;
		g_dwGunAnim149 = RES_NEWFRONT_ANIMS_LORES_FIRE_RIGHT;
		g_dwGunAnimLeftShot = RES_NEWFRONT_ANIMS_LORES_FX_LEFT;
		g_dwGunAnim14b = RES_NEWFRONT_ANIMS_LORES_FX_RIGHT;
		g_dwGunAnim14c = RES_NEWFRONT_ANIMS_LORES_GUNTURN;
		g_dwGunAnim14d = RES_NEWFRONT_ANIMS_LORES_SPLAT_LEFT;
		g_dwGunAnim14e = RES_NEWFRONT_ANIMS_LORES_SPLAT_RIGHT;
	}
	else {
		m_alternateAssets = 0;
		g_dwGunAnimCursor = RES_NEWFRONT_ANIMS_HIRES_BULLET_LEFT;
		g_dwGunAnim147 = RES_NEWFRONT_ANIMS_HIRES_BULLET_RIGHT;
		g_dwGunAnim148 = RES_NEWFRONT_ANIMS_HIRES_FIRE_LEFT;
		g_dwGunAnim149 = RES_NEWFRONT_ANIMS_HIRES_FIRE_RIGHT;
		g_dwGunAnimLeftShot = RES_NEWFRONT_ANIMS_HIRES_FX_LEFT;
		g_dwGunAnim14b = RES_NEWFRONT_ANIMS_HIRES_FX_RIGHT;
		g_dwGunAnim14c = RES_NEWFRONT_ANIMS_HIRES_GUNTURN;
		g_dwGunAnim14d = RES_NEWFRONT_ANIMS_HIRES_SPLAT_LEFT;
		g_dwGunAnim14e = RES_NEWFRONT_ANIMS_HIRES_SPLAT_RIGHT;
	}
	CAnimsManager::LoadAnims(g_dwGunAnimCursor);
	CAnimsManager::LoadAnims(g_dwGunAnim147);
	CAnimsManager::LoadAnims(g_dwGunAnim148);
	CAnimsManager::LoadAnims(g_dwGunAnim149);
	CAnimsManager::LoadAnims(g_dwGunAnimLeftShot);
	CAnimsManager::LoadAnims(g_dwGunAnim14b);
	CAnimsManager::LoadAnims(g_dwGunAnim14c);
	CAnimsManager::LoadAnims(g_dwGunAnim14d);
	CAnimsManager::LoadAnims(g_dwGunAnim14e);
	m_sideAnim = new CPlayThruAnim(CAnimsManager::GetnAnims(g_dwGunAnim14c), 1);
	m_sideAnim->m_fixedTime = 0xffffffff;
	m_leftShotAnim = new CPlayThruAnim(CAnimsManager::GetnAnims(g_dwGunAnimLeftShot), 1);
	m_leftShotAnim->m_fixedTime = 0xffffffff;
	m_cursorAnim = new CPlayThruAnim(CAnimsManager::GetnAnims(g_dwGunAnimCursor), 1);
	m_cursorAnim->m_fixedTime = 0xffffffff;
	m_rightShotAnim = new CPlayThruAnim(CAnimsManager::GetnAnims(g_dwGunAnim14d), 1);
	m_rightShotAnim->m_fixedTime = 0xffffffff;
	m_hitAnim = new CPlayThruAnim(CAnimsManager::GetnAnims(g_dwGunAnim148), 1);
	m_hitAnim->m_fixedTime = 0xffffffff;
	m_inputReadyTime = CurrentQueueTimer();
}

// FUNCTION: LEMBALL 0x0044cc90
void CGunController::ActivateButtons(int p_active)
{
	int i;

	m_buttonsActive = p_active;
	i = 0;
	while (i < m_buttonCount) {
		CGunButtons* button = m_buttons[i];
		if (button != 0) {
			button->m_active = p_active;
			button->m_graphicButton->SetActive(p_active);
		}
		i = i + 1;
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
	CAnimsManager::UnLoadAnims(g_dwGunAnimCursor);
	CAnimsManager::UnLoadAnims(g_dwGunAnim147);
	CAnimsManager::UnLoadAnims(g_dwGunAnim148);
	CAnimsManager::UnLoadAnims(g_dwGunAnim149);
	CAnimsManager::UnLoadAnims(g_dwGunAnimLeftShot);
	CAnimsManager::UnLoadAnims(g_dwGunAnim14b);
	CAnimsManager::UnLoadAnims(g_dwGunAnim14c);
	CAnimsManager::UnLoadAnims(g_dwGunAnim14d);
	CAnimsManager::UnLoadAnims(g_dwGunAnim14e);
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
	if ((int) (p_message->time - m_inputReadyTime) < 0) {
		return 0;
	}
	switch ((unsigned int) p_message->type) {
	case 4:
		break;
	default:
		m_processedCount = m_processedCount + 1;
		return 0;
	}
	switch (p_message->code) {
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
	int mid = (short) ((int) m_window->m_rect.m_width / 2);
	int side;
	if (p_x < mid) {
		side = 0;
	}
	else if (p_x > mid) {
		side = 1;
	}
	else {
		side = p_side;
	}
	int existing = -1;
	for (int i = 0; i < 8; i++) {
		if (m_junctions[i].m_direction != 3 && m_junctions[i].m_y == p_y) {
			existing = i;
		}
	}
	if (existing != -1) {
		m_junctions[existing].m_direction = 2;
		if (side == 0) {
			m_junctions[existing].m_leftMessage = p_side;
			m_junctions[existing].m_leftBinding = (void*) p_message;
			m_junctions[existing].m_leftX = p_x;
			return;
		}
		m_junctions[existing].m_rightMessage = p_side;
		m_junctions[existing].m_rightBinding = (void*) p_message;
		m_junctions[existing].m_rightX = p_x;
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
	int i;

	i = 0;
	while (i < 8) {
		if (m_buttons[i] != 0) {
			m_buttons[i]->Draw(p_firstState, p_secondState);
		}
		i = i + 1;
	}
}

// FUNCTION: LEMBALL 0x0044d2d0
void CGunController::DrawSpriteWindow()
{
	CGdi* previousGdi;
	int* offsets;
	CVsPoint position;
	unsigned long frame;

	m_spriteSurface->m_renderTarget->GetCurrDb();
	m_cursorRect[0].m_bounds.m_width = m_spriteSurface->m_renderTarget->m_windowRect.m_width;
	m_cursorRect[0].m_bounds.m_height = m_spriteSurface->m_renderTarget->m_windowRect.m_height;
	m_cursorRect[0].m_bounds.m_x = 0;
	m_cursorRect[0].m_bounds.m_y = 0;
	m_cursorRect[0].m_color = 0x10000;
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
			frame = CAnimsManager::GetnAnims(g_dwGunAnim14c) - 1;
		}
		m_staticAnim.m_frameState = frame;
		position.m_x = (short) (m_gunX + offsets[0]);
		position.m_y = (short) (offsets[1] + m_gunY);
		previousGdi = CAnimsManager::m_gdi;
		CAnimsManager::m_gdi = m_spriteSurface;
		CAnimsManager::DrawAnim(position, g_dwGunAnim14c, 0, (CFrames*) &m_staticAnim, 0);
		CAnimsManager::m_gdi = previousGdi;
		break;
	case 1:
		position.m_x = (short) (m_gunX + offsets[0]);
		position.m_y = (short) (offsets[1] + m_gunY);
		previousGdi = CAnimsManager::m_gdi;
		CAnimsManager::m_gdi = m_spriteSurface;
		CAnimsManager::DrawAnim(position, g_dwGunAnim14c, 0, (CFrames*) m_sideAnim, 0);
		CAnimsManager::m_gdi = previousGdi;
		break;
	case 2:
		previousGdi = CAnimsManager::m_gdi;
		CAnimsManager::m_gdi = m_spriteSurface;
		if (m_targetSide == 0) {
			position.m_x = (short) m_projectileX;
			position.m_y = (short) m_projectileY;
			CAnimsManager::DrawAnim(position, g_dwGunAnimCursor, 0, (CFrames*) m_cursorAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
			previousGdi = CAnimsManager::m_gdi;
			CAnimsManager::m_gdi = m_spriteSurface;
			position.m_x = (short) (m_gunX + offsets[0]);
			position.m_y = (short) (m_gunY + offsets[1]);
			CAnimsManager::DrawAnim(position, g_dwGunAnim148, 0, (CFrames*) m_hitAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
			previousGdi = CAnimsManager::m_gdi;
			CAnimsManager::m_gdi = m_spriteSurface;
			position.m_x = (short) (m_gunX + offsets[2]);
			position.m_y = (short) (m_gunY + offsets[3]);
			CAnimsManager::DrawAnim(position, g_dwGunAnimLeftShot, 0, (CFrames*) m_leftShotAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
		}
		else {
			position.m_x = (short) m_projectileX;
			position.m_y = (short) m_projectileY;
			CAnimsManager::DrawAnim(position, g_dwGunAnim147, 0, (CFrames*) m_cursorAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
			previousGdi = CAnimsManager::m_gdi;
			CAnimsManager::m_gdi = m_spriteSurface;
			position.m_x = (short) (m_gunX + offsets[0]);
			position.m_y = (short) (m_gunY + offsets[1]);
			CAnimsManager::DrawAnim(position, g_dwGunAnim149, 0, (CFrames*) m_hitAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
			previousGdi = CAnimsManager::m_gdi;
			CAnimsManager::m_gdi = m_spriteSurface;
			position.m_x = (short) (m_gunX + offsets[4]);
			position.m_y = (short) (m_gunY + offsets[5]);
			CAnimsManager::DrawAnim(position, g_dwGunAnim14b, 0, (CFrames*) m_leftShotAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
		}
		break;
	case 3:
		previousGdi = CAnimsManager::m_gdi;
		CAnimsManager::m_gdi = m_spriteSurface;
		if (m_targetSide == 0) {
			position.m_x = (short) m_projectileEndX;
			position.m_y = (short) m_projectileEndY;
			CAnimsManager::DrawAnim(position, g_dwGunAnim14d, 0, (CFrames*) m_rightShotAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
			previousGdi = CAnimsManager::m_gdi;
			CAnimsManager::m_gdi = m_spriteSurface;
			position.m_x = (short) (m_gunX + offsets[0]);
			position.m_y = (short) (m_gunY + offsets[1]);
			CAnimsManager::DrawAnim(position, g_dwGunAnim148, 0, (CFrames*) m_hitAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
		}
		else {
			position.m_x = (short) m_projectileEndX;
			position.m_y = (short) m_projectileEndY;
			CAnimsManager::DrawAnim(position, g_dwGunAnim14e, 0, (CFrames*) m_rightShotAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
			previousGdi = CAnimsManager::m_gdi;
			CAnimsManager::m_gdi = m_spriteSurface;
			position.m_x = (short) (m_gunX + offsets[0]);
			position.m_y = (short) (m_gunY + offsets[1]);
			CAnimsManager::DrawAnim(position, g_dwGunAnim149, 0, (CFrames*) m_hitAnim, 0);
			CAnimsManager::m_gdi = previousGdi;
		}
		break;
	}
	CAnimsManager::ResetPrimitives();
}

// FUNCTION: LEMBALL 0x0044d830
void CGunController::MoveUp()
{
	int i;
	int bestY;
	int foundY;
	int direction;

	bestY = -1;
	foundY = -1;
	i = 0;
	while (i < 8) {
		direction = m_junctions[i].m_direction;
		if (direction != 3 && m_junctions[i].m_y < m_targetY && bestY < m_junctions[i].m_y) {
			if (direction != 2) {
				m_targetSide = direction;
			}
			foundY = m_junctions[i].m_y;
			bestY = foundY;
			g_pSoundView->PlayEffect(SFX_RELOAD);
		}
		i = i + 1;
	}
	if (foundY != -1) {
		m_targetY = foundY;
	}
	m_moveStartTime = CurrentMilliTimer();
	m_moveStartY = m_gunY;
	m_verticalMoving = 1;
	m_moveEndTime = abs(m_targetY - m_gunY) * 3 + m_moveStartTime;
}

// FUNCTION: LEMBALL 0x0044d8e0
void CGunController::MoveDown()
{
	int i;
	int bestY;
	int foundY;
	int direction;

	bestY = 999999;
	foundY = -1;
	i = 0;
	while (i < 8) {
		direction = m_junctions[i].m_direction;
		if (direction != 3 && m_targetY < m_junctions[i].m_y && m_junctions[i].m_y < bestY) {
			if (direction != 2) {
				m_targetSide = direction;
			}
			foundY = m_junctions[i].m_y;
			bestY = foundY;
			g_pSoundView->PlayEffect(SFX_RELOAD);
		}
		i = i + 1;
	}
	if (foundY != -1) {
		m_targetY = foundY;
	}
	m_moveStartTime = CurrentMilliTimer();
	m_moveStartY = m_gunY;
	m_verticalMoving = 1;
	m_moveEndTime = abs(m_targetY - m_gunY) * 3 + m_moveStartTime;
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

// Descriptive name; the original Windows helper has no recovered symbol.
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

	const CVsSize& animSize = CAnimsManager::GetAnimSize(g_dwGunAnim14c, 0);
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
	int* offsets;
	unsigned int delta;

	m_selectionMessage.type = 0xc;
	if (m_gunY == m_targetY && m_selectionState == 0) {
		i = 0;
		while (i < 8) {
			if (m_junctions[i].m_y == m_targetY) {
				if (m_targetSide == 0) {
					m_selectionMessage.code = (int) m_junctions[i].m_leftBinding;
					m_selectedMessage = m_junctions[i].m_leftMessage;
				}
				else {
					m_selectionMessage.code = (int) m_junctions[i].m_rightBinding;
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
		if (m_targetSide == 0) {
			m_projectileX = offsets[6] + m_selectionStartX;
			m_projectileY = offsets[7] + m_targetY;
			m_projectileTargetX = offsets[8] + m_junctions[i].m_leftX;
			m_projectileEndX = offsets[10] + m_junctions[i].m_leftX;
			m_projectileEndY = offsets[11] + m_junctions[i].m_y;
		}
		else {
			m_projectileX = offsets[12] + m_selectionStartX;
			m_projectileY = offsets[13] + m_targetY;
			m_projectileTargetX = offsets[14] + m_junctions[i].m_rightX;
			m_projectileEndX = offsets[16] + m_junctions[i].m_rightX;
			m_projectileEndY = offsets[17] + m_junctions[i].m_y;
		}
		m_selectStartTime = CurrentMilliTimer();
		delta = m_projectileTargetX - m_projectileX;
		delta = abs((int) delta);
		m_selectionState = 2;
		m_selectEndTime = delta * 2 + m_selectStartTime;
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
				int direction = Sgn(m_projectileTargetX - step);
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
				m_selectionMessage.time = CurrentQueueTimer();
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
