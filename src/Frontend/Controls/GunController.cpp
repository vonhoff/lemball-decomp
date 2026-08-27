#include "GunController.h"

#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/VsPoint.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/GraphicButton.h"
#include "../../Visos/Graphics/HotAreaHandler.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../Windows/SpriteWindow.h"
#include "GunButtons.h"

#include <new.h>

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

// 68K 0x10803146 __ct__14CGunControllerFP5CGWndP4CGDIiUc
// STUB: LEMBALL 0x0044c870
GunController::GunController(GWnd* p_arg0, Gdi* p_arg1, int p_arg2, unsigned char p_arg3)
	: m_anims(p_arg1, 0x2b6, 10, 5, 0, 0)
{
	int i;

	m_staticAnim.m_frames = 1;
	m_staticAnim.m_frameState = 0;
	m_buttonsActive = 1;
	m_mode = p_arg3;
	m_window = p_arg0;
	m_gdi = p_arg1;
	m_nextMessageId = 0xabcd0000;
	m_spriteWindow = 0;
	m_spriteSurface = 0;
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
	if (g_pMasterInputQueue != 0) {
		g_pMasterInputQueue->Attach(this, 0);
	}
	if (m_mode == 1) {
		m_alternateAssets = 1;
		g_dwGunAnimCursor = 0x173;
		g_dwGunAnim147 = 0x174;
		g_dwGunAnim148 = 0x175;
		g_dwGunAnim149 = 0x176;
		g_dwGunAnimLeftShot = 0x177;
		g_dwGunAnim14b = 0x178;
		g_dwGunAnim14c = 0x179;
		g_dwGunAnim14d = 0x17a;
		g_dwGunAnim14e = 0x17b;
	}
	else {
		m_alternateAssets = 0;
		g_dwGunAnimCursor = 0x146;
		g_dwGunAnim147 = 0x147;
		g_dwGunAnim148 = 0x148;
		g_dwGunAnim149 = 0x149;
		g_dwGunAnimLeftShot = 0x14a;
		g_dwGunAnim14b = 0x14b;
		g_dwGunAnim14c = 0x14c;
		g_dwGunAnim14d = 0x14d;
		g_dwGunAnim14e = 0x14e;
	}
	m_anims.LoadAnims(g_dwGunAnimCursor);
	m_anims.LoadAnims(g_dwGunAnim147);
	m_anims.LoadAnims(g_dwGunAnim148);
	m_anims.LoadAnims(g_dwGunAnim149);
	m_anims.LoadAnims(g_dwGunAnimLeftShot);
	m_anims.LoadAnims(g_dwGunAnim14b);
	m_anims.LoadAnims(g_dwGunAnim14c);
	m_anims.LoadAnims(g_dwGunAnim14d);
	m_anims.LoadAnims(g_dwGunAnim14e);
	m_sideAnim = 0;
	m_leftShotAnim = 0;
	m_cursorAnim = 0;
	m_rightShotAnim = 0;
	m_hitAnim = 0;
	m_inputReadyTime = CurrentQueueTimer();
}

// 68K 0x108035a6 ActivateButtons__14CGunControllerFUc
// STUB: LEMBALL 0x0044cc90
void GunController::ActivateButtons(unsigned char p_active)
{
	int i;

	i = 0;
	m_buttonsActive = p_active;
	while (i < m_buttonCount) {
		if (m_buttons[i] != 0) {
			m_buttons[i]->m_active = p_active;
			if (m_buttons[i]->m_graphicButton != 0) {
				static_cast<HotAreaHandler*>(m_buttons[i]->m_graphicButton)->SetActive(p_active);
			}
		}
		i = i + 1;
	}
}

// 68K 0x10803618 SetSpriteWindow__14CGunControllerFv
// STUB: LEMBALL 0x0044cce0
void GunController::SetSpriteWindow()
{
	void* storage;
	VsRect createRect;

	storage = operator new(0x90);
	if (storage == 0) {
		m_spriteWindow = 0;
	}
	else {
		m_spriteWindow = new (storage) SpriteWindow();
	}
	createRect.m_width = m_window->m_rect.m_width;
	createRect.m_height = m_window->m_rect.m_height;
	createRect.m_x = 0;
	createRect.m_y = 0;
	m_spriteWindow->Create(createRect, m_window, 0);
	m_spriteSurface = m_spriteWindow->m_gdi;
}

// 68K 0x108038ee ProcessMsg__14CGunControllerFP10tagMESSAGE
// STUB: LEMBALL 0x0044cec0
int GunController::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x108039d4 AddButton__14CGunControllerFiiPUlUciiiPvUl
// STUB: LEMBALL 0x0044cfb0
void GunController::AddButton(int p_x,
							  int p_y,
							  unsigned long* p_animIds,
							  unsigned char p_postAction,
							  int p_minimum,
							  int p_maximum,
							  int p_value,
							  void* p_binding,
							  unsigned long p_actionMessage)
{
	void* storage;
	unsigned long controlMessage;

	controlMessage = m_nextMessageId + 1;
	m_nextMessageId = controlMessage;
	storage = operator new(0x60);
	if (storage == 0) {
		m_buttons[m_buttonCount] = 0;
	}
	else {
		m_buttons[m_buttonCount] = new (storage)
			GunButtons(m_window, m_gdi, p_x, p_y, p_animIds, p_postAction, p_minimum, p_maximum, p_value, controlMessage, p_binding, p_actionMessage);
	}
	if (m_buttons[m_buttonCount] != 0) {
		AddJunction(p_x, p_y, 0 < p_maximum, m_buttons[m_buttonCount]->m_controlMessage);
	}
	m_buttonCount = m_buttonCount + 1;
}

// 68K 0x10803ab0 AddJunction__14CGunControllerFiiUcUl
// STUB: LEMBALL 0x0044d150
void GunController::AddJunction(int p_x, int p_y, unsigned char p_side, unsigned long p_message)
{
	int mid;
	int side;
	int unused;
	int existing;
	int i;

	existing = -1;
	mid = (short) ((int) m_window->m_rect.m_width / 2);
	if (p_x < mid) {
		side = 0;
	}
	else {
		side = 1;
		if (p_x <= mid) {
			side = unused;
		}
	}
	i = 0;
	while (i < 8) {
		if (m_junctions[i].m_direction != 3 && m_junctions[i].m_y == p_y) {
			existing = i;
		}
		i = i + 1;
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
	i = 0;
	while (i < 8) {
		if (m_junctions[i].m_direction == 3) {
			m_junctions[i].m_y = p_y;
			m_junctions[i].m_direction = side;
			if (side == 0) {
				m_junctions[i].m_leftMessage = p_side;
				m_junctions[i].m_leftX = p_x;
				m_junctions[i].m_leftBinding = (void*) p_message;
				return;
			}
			m_junctions[i].m_rightMessage = p_side;
			m_junctions[i].m_rightBinding = (void*) p_message;
			m_junctions[i].m_rightX = p_x;
			return;
		}
		i = i + 1;
	}
}

// 68K 0x10803cc4 DrawButtons__14CGunControllerFUcUc
// STUB: LEMBALL 0x0044d290
void GunController::DrawButtons(unsigned char p_firstState, unsigned char p_secondState)
{
	int i;

	i = 0;
	while (i < m_buttonCount) {
		if (m_buttons[i] != 0) {
			m_buttons[i]->Draw(p_firstState, p_secondState);
		}
		i = i + 1;
	}
}

// 68K 0x10803d36 DrawSpriteWindow__14CGunControllerFv
// STUB: LEMBALL 0x0044d2d0
void GunController::DrawSpriteWindow()
{
	Gdi* previousGdi;
	short* offsets;
	VsPoint position;
	unsigned long frame;

	m_spriteSurface->m_renderTarget->GetCurrDb();
	m_cursorRect.m_left = m_spriteSurface->m_renderTarget->m_clipRect.m_x;
	m_cursorRect.m_top = m_spriteSurface->m_renderTarget->m_clipRect.m_y;
	m_cursorRect.m_right = 0;
	m_cursorRect.m_bottom = 0;
	m_cursorRect.m_color = 0x10000;
	m_cursorRect.Draw(m_spriteSurface);
	offsets = (short*) g_anGunSpriteOffsetCompact;
	if (m_alternateAssets != 1) {
		offsets = (short*) g_anGunSpriteOffset;
	}
	position.m_x = 0;
	position.m_y = 0;
	switch (m_selectionState) {
	case 0:
		frame = 0;
		if (m_currentSide != 0) {
			frame = m_anims.GetnAnims(g_dwGunAnim14c) - 1;
		}
		m_staticAnim.m_frameState = frame;
		position.m_x = (short) (m_gunX + *offsets);
		position.m_y = (short) (*(int*) (offsets + 2) + m_gunY);
		previousGdi = m_anims.m_gdi;
		m_anims.m_gdi = m_spriteSurface;
		m_anims.DrawAnim(position, g_dwGunAnim14c, 0, (AnimFrameBASE*) &m_staticAnim, 0);
		m_anims.m_gdi = previousGdi;
		break;
	case 1:
		position.m_x = (short) (m_gunX + *offsets);
		position.m_y = (short) (*(int*) (offsets + 2) + m_gunY);
		previousGdi = m_anims.m_gdi;
		m_anims.m_gdi = m_spriteSurface;
		m_anims.DrawAnim(position, g_dwGunAnim14c, 0, (AnimFrameBASE*) m_sideAnim, 0);
		m_anims.m_gdi = previousGdi;
		break;
	case 2:
		previousGdi = m_anims.m_gdi;
		m_anims.m_gdi = m_spriteSurface;
		if (m_targetSide == 0) {
			m_anims.DrawAnim(position, g_dwGunAnimCursor, 0, (AnimFrameBASE*) m_cursorAnim, 0);
			m_anims.m_gdi = previousGdi;
			previousGdi = m_anims.m_gdi;
			m_anims.m_gdi = m_spriteSurface;
			m_anims.DrawAnim(position, g_dwGunAnim148, 0, (AnimFrameBASE*) m_hitAnim, 0);
			m_anims.m_gdi = previousGdi;
			previousGdi = m_anims.m_gdi;
			m_anims.m_gdi = m_spriteSurface;
			m_anims.DrawAnim(position, g_dwGunAnimLeftShot, 0, (AnimFrameBASE*) m_leftShotAnim, 0);
			m_anims.m_gdi = previousGdi;
		}
		else {
			m_anims.DrawAnim(position, g_dwGunAnim147, 0, (AnimFrameBASE*) m_cursorAnim, 0);
			m_anims.m_gdi = previousGdi;
			previousGdi = m_anims.m_gdi;
			m_anims.m_gdi = m_spriteSurface;
			m_anims.DrawAnim(position, g_dwGunAnim149, 0, (AnimFrameBASE*) m_hitAnim, 0);
			m_anims.m_gdi = previousGdi;
			previousGdi = m_anims.m_gdi;
			m_anims.m_gdi = m_spriteSurface;
			m_anims.DrawAnim(position, g_dwGunAnim14b, 0, (AnimFrameBASE*) m_leftShotAnim, 0);
			m_anims.m_gdi = previousGdi;
		}
		break;
	case 3:
		previousGdi = m_anims.m_gdi;
		m_anims.m_gdi = m_spriteSurface;
		if (m_targetSide == 0) {
			m_anims.DrawAnim(position, g_dwGunAnim14d, 0, (AnimFrameBASE*) m_rightShotAnim, 0);
			m_anims.m_gdi = previousGdi;
			previousGdi = m_anims.m_gdi;
			m_anims.m_gdi = m_spriteSurface;
			m_anims.DrawAnim(position, g_dwGunAnim148, 0, (AnimFrameBASE*) m_hitAnim, 0);
			m_anims.m_gdi = previousGdi;
		}
		else {
			m_anims.DrawAnim(position, g_dwGunAnim14e, 0, (AnimFrameBASE*) m_rightShotAnim, 0);
			m_anims.m_gdi = previousGdi;
			position.m_y = (short) (m_gunY + offsets[2]);
			position.m_x = (short) (m_gunX + *offsets);
			previousGdi = m_anims.m_gdi;
			m_anims.m_gdi = m_spriteSurface;
			m_anims.DrawAnim(position, g_dwGunAnim149, 0, (AnimFrameBASE*) m_hitAnim, 0);
			m_anims.m_gdi = previousGdi;
		}
		break;
	}
	m_anims.ResetPrimitives();
}

// 68K 0x1080424e MoveUp__14CGunControllerFv
// STUB: LEMBALL 0x0044d830
void GunController::MoveUp()
{
}

// 68K 0x1080435e MoveDown__14CGunControllerFv
// STUB: LEMBALL 0x0044d8e0
void GunController::MoveDown()
{
}

// 68K 0x10804474 MoveLeft__14CGunControllerFv
// STUB: LEMBALL 0x0044d990
void GunController::MoveLeft()
{
}

// 68K 0x108044f0 MoveRight__14CGunControllerFv
// STUB: LEMBALL 0x0044d9e0
void GunController::MoveRight()
{
}

// 68K 0x10804570 SetGun__14CGunControllerFi
// STUB: LEMBALL 0x0044da70
void GunController::SetGun(int p_junction)
{
	VsSize animSize;
	int direction;

	animSize = m_anims.GetAnimSize(g_dwGunAnim14c, 0);
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

// 68K 0x1080465c SelectOption__14CGunControllerFv
// STUB: LEMBALL 0x0044db20
void GunController::SelectOption()
{
}

// 68K 0x10804874 Process__14CGunControllerFv
// STUB: LEMBALL 0x0044dce0
void GunController::Process()
{
}

// 68K 0x108036f4 __dt__14CGunControllerFv
GunController::~GunController()
{
}
