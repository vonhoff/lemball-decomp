#include "HiliteController.h"

#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/VsPoint.h"
#include "../../Visos/Foundation/VsRect.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/GraphicButton.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../Windows/HiliteWindow.h"
#include "HiliteButtons.h"

#include <new.h>

// 68K 0x1080503a __ct__17CHiliteControllerFP5CGWndP4CGDIiUcUc
// FUNCTION: LEMBALL 0x0044f440
HiliteController::HiliteController(GWnd* p_arg0, Gdi* p_arg1, int p_arg2, unsigned char p_arg3, unsigned char p_arg4)
	: m_anims(p_arg1, 0x2b6, 1, 1, 0, 0)
{
	int index;

	m_layoutMode = p_arg3;
	m_horizontalMode = p_arg4;
	m_window = p_arg0;
	m_gdi = p_arg1;
	m_active = 1;
	m_nextControlMessage = 0xabcd0000;
	index = 0;
	while (index < 4) {
		m_buttons[index] = 0;
		m_junctions[index].m_present = 0;
		index = index + 1;
	}
	m_buttonCount = 0;
	m_currentButton = 0;
	g_pMasterInputQueue->Attach(this, 0);
	if (m_layoutMode == 1) {
		m_animationSet = 1;
		g_dwHiliteAnimationId = 0x183;
	}
	else {
		m_animationSet = 0;
		g_dwHiliteAnimationId = 0x156;
	}
	m_anims.LoadAnims(g_dwHiliteAnimationId);
}

// 68K 0x108051b4 SetHiliteWindow__17CHiliteControllerFv
// FUNCTION: LEMBALL 0x0044f590
void HiliteController::SetHiliteWindow()
{
	void* storage;
	VsRect rect;

	storage = operator new(0x90);
	if (storage == 0) {
		m_hiliteWindow = 0;
	}
	else {
		m_hiliteWindow = new (storage) HiliteWindow();
	}
	if (m_hiliteWindow != 0 && m_window != 0) {
		rect.m_width = m_window->m_rect.m_width;
		rect.m_height = m_window->m_rect.m_height;
		rect.m_x = 0;
		rect.m_y = 0;
		m_hiliteWindow->Create(rect, (PvGWnd*) m_window, 0);
		m_hiliteSurface = (void*) m_hiliteWindow->m_gdi;
	}
}

// 68K 0x10805292 __dt__17CHiliteControllerFv
// FUNCTION: LEMBALL 0x0044f610
HiliteController::~HiliteController()
{
	g_pMasterInputQueue->Detach(this, 0);
	for (int i = 0; i < 4; i++) {
		if (m_buttons[i] != 0) {
			delete m_buttons[i];
		}
	}
	m_anims.UnLoadAnims(g_dwHiliteAnimationId);
	if (m_hiliteWindow != 0) {
		if (m_hiliteWindow->m_lifecycleRefs == 1) {
			m_hiliteWindow->Destroy();
		}
		delete m_hiliteWindow;
	}
}

// 68K 0x108053e4 ProcessMsg__17CHiliteControllerFP10tagMESSAGE
// FUNCTION: LEMBALL 0x0044f6c0
int HiliteController::ProcessMsg(Message* p_message)
{
	HiliteButtons* currentBtn;
	VsPoint point;

	if (m_active == 0) {
		return 0;
	}
	if (p_message->type == 3) {
		if (p_message->code == 0x1f || p_message->code == 0x22 || p_message->code == 0x4c) {
			if (m_currentButton < m_buttonCount) {
				currentBtn = m_buttons[m_currentButton];
				if (currentBtn != 0 && currentBtn->m_button != 0) {
					point.m_x = 0;
					point.m_y = 0;
					currentBtn->m_button->OnButtonDown(point, 0);
				}
			}
		}
		return 0;
	}
	if (p_message->type != 4) {
		return 0;
	}
	switch (p_message->code) {
	case 1:
		if (m_horizontalMode == 0) {
			return 0;
		}
		MoveLeft();
		g_pSoundView->PlayEffect((eSoundEffect) 0x1b);
		return 1;
	case 2:
		if (m_horizontalMode == 0) {
			return 0;
		}
		MoveRight();
		g_pSoundView->PlayEffect((eSoundEffect) 0x1b);
		return 1;
	case 3:
		if (m_horizontalMode != 0) {
			return 0;
		}
		MoveLeft();
		g_pSoundView->PlayEffect((eSoundEffect) 0x1b);
		return 1;
	case 4:
		if (m_horizontalMode != 0) {
			return 0;
		}
		MoveRight();
		g_pSoundView->PlayEffect((eSoundEffect) 0x1b);
		return 1;
	case 0x1f:
	case 0x22:
	case 0x4c:
		if (m_currentButton < m_buttonCount) {
			currentBtn = m_buttons[m_currentButton];
			if (currentBtn != 0 && currentBtn->m_button != 0) {
				point.m_x = 0;
				point.m_y = 0;
				currentBtn->m_button->OnButtonUp(point, 0);
			}
		}
		break;
	}
	return 0;
}

// 68K 0x108055ce AddButton__17CHiliteControllerFiiPUlUciiiPvUl
// FUNCTION: LEMBALL 0x0044f8c0
void HiliteController::AddButton(int p_x,
								 int p_y,
								 unsigned long* p_animIds,
								 unsigned char p_mode,
								 int p_minimum,
								 int p_maximum,
								 int p_value,
								 void* p_binding,
								 unsigned long p_actionMessage)
{
	unsigned long controlMessage;
	void* storage;

	controlMessage = m_nextControlMessage + 1;
	m_nextControlMessage = controlMessage;
	storage = operator new(0x58);
	if (storage == 0) {
		m_buttons[m_buttonCount] = 0;
	}
	else {
		m_buttons[m_buttonCount] = new (storage) HiliteButtons(
			m_window, m_gdi, p_x, p_y, p_animIds, p_mode, p_minimum, p_maximum, p_value, controlMessage, p_binding, p_actionMessage);
	}
	AddHJunction(p_x, p_y, m_buttons[m_buttonCount]->m_controlMessage);
	m_buttonCount = m_buttonCount + 1;
}

// 68K 0x108056a0 AddHJunction__17CHiliteControllerFiiUl
// FUNCTION: LEMBALL 0x0044f970
void HiliteController::AddHJunction(int p_x, int p_y, unsigned long p_controlMessage)
{
	m_junctions[m_buttonCount].m_present = 1;
	m_junctions[m_buttonCount].m_x = p_x;
	m_junctions[m_buttonCount].m_y = p_y;
	m_junctions[m_buttonCount].m_controlMessage = p_controlMessage;
}

// 68K 0x1080571e DrawButtons__17CHiliteControllerFUc
// FUNCTION: LEMBALL 0x0044f9d0
void HiliteController::DrawButtons(int p_force)
{
	int count;
	HiliteButtons** buttonPtr;

	buttonPtr = m_buttons;
	count = 4;
	do {
		if (*buttonPtr != 0) {
			(*buttonPtr)->Draw(p_force);
		}
		buttonPtr++;
		count--;
	} while (count != 0);
}

// FUNCTION: LEMBALL 0x0044fa00
void HiliteController::DrawHiliteWindow()
{
	if (m_active != 0 && m_hiliteSurface != 0) {
		Gdi* hiliteGdi = (Gdi*) m_hiliteSurface;
		Surface* surface = hiliteGdi->m_renderTarget;
		m_hiliteRect.m_color = 0x10000;
		m_hiliteRect.m_left = surface->m_windowRect.m_width;
		m_hiliteRect.m_top = surface->m_windowRect.m_height;
		m_hiliteRect.m_right = 0;
		m_hiliteRect.m_bottom = 0;
		m_hiliteRect.Draw(hiliteGdi);
		m_hiliteAnim.m_frameState = 0;
		Gdi* savedGdi = m_anims.m_gdi;
		m_anims.m_gdi = hiliteGdi;
		VsPoint position;
		position.m_x = 0;
		position.m_y = 0;
		m_anims.DrawAnim(position, g_dwHiliteAnimationId, 0, (Frames*) &m_hiliteAnim, 0);
		m_anims.m_gdi = savedGdi;
		m_anims.ResetPrimitives();
	}
}

// 68K 0x108058bc MoveLeft__17CHiliteControllerFv
// FUNCTION: LEMBALL 0x0044fae0
void HiliteController::MoveLeft()
{
	int nextButton = m_currentButton - 1;
	if (nextButton >= 0) {
		m_currentButton = nextButton;
		SetHilite(nextButton);
	}
}

// 68K 0x10805958 MoveRight__17CHiliteControllerFv
// FUNCTION: LEMBALL 0x0044fb00
void HiliteController::MoveRight()
{
	int nextButton = m_currentButton + 1;
	if (nextButton < m_buttonCount) {
		m_currentButton = nextButton;
		SetHilite(nextButton);
	}
}

// 68K 0x108059f8 SetHilite__17CHiliteControllerFi
// FUNCTION: LEMBALL 0x0044fb20
void HiliteController::SetHilite(int p_buttonIndex)
{
	m_targetX = m_currentX = m_junctions[p_buttonIndex].m_x;
	m_targetY = m_currentY = m_junctions[p_buttonIndex].m_y;
	unsigned long now = CurrentMilliTimer();
	m_transitionEnd = now;
	m_transitionStart = now;
	m_currentButton = p_buttonIndex;
}

// 68K 0x10805a72 Process__17CHiliteControllerFv
// FUNCTION: LEMBALL 0x0044fbc0
void HiliteController::Process()
{
}

// 68K 0x10805aa4 ActivateButtons__17CHiliteControllerFUc
// FUNCTION: LEMBALL 0x0044fbd0
void HiliteController::ActivateButtons(int p_active)
{
	m_buttonsActive = p_active;
	int i = 0;
	if (m_buttonCount > i) {
		HiliteButtons** pBtn = m_buttons;
		do {
			HiliteButtons* btn = *pBtn;
			if (btn != 0) {
				btn->m_active = p_active;
				btn->m_button->SetActive(p_active);
			}
			pBtn++;
			i++;
		} while (m_buttonCount > i);
	}
}

// 68K 0x10805b28 UpdateAnimIDs__17CHiliteControllerFUl
// FUNCTION: LEMBALL 0x0044fc50
void HiliteController::UpdateAnimIDs(unsigned long p_actionMessage)
{
	int i = 0;
	if (m_buttonCount > i) {
		HiliteButtons** pBtn = m_buttons;
		do {
			if (*pBtn != 0 && (*pBtn)->m_actionMessage == p_actionMessage) {
				m_buttons[i]->UpdateAnimId();
				break;
			}
			pBtn++;
			i++;
		} while (i < m_buttonCount);
	}
}
