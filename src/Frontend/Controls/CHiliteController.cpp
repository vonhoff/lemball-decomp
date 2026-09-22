#include "CHiliteController.h"

#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/CVsPoint.h"
#include "../../Visos/Foundation/CVsRect.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/CGdi.h"
#include "../../Visos/Graphics/CGraphicButton.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Windows/CHiliteWindow.h"
#include "CHiliteButtons.h"
#include "Frontend/Controls/HiliteControllerJunction.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Animation/CAnimsManager.h"
#include "Visos/Animation/CStaticAnim.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CGWnd.h"
#include "Visos/Graphics/CPvGWnd.h"
#include "Visos/Graphics/CSolidRect.h"

#include <new.h>

class CFrames;

// FUNCTION: LEMBALL 0x0044f440
CHiliteController::CHiliteController(CGWnd* p_arg0, CGdi* p_arg1, int p_arg2, unsigned int p_arg3, unsigned int p_arg4)
	: CAnimsManager(p_arg1, 0x2b6, 1, 1, 0, 0)
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
		g_dwHiliteAnimationId = RES_NEWFRONT_ANIMS_LORES_HILITE;
	}
	else {
		m_animationSet = 0;
		g_dwHiliteAnimationId = RES_NEWFRONT_ANIMS_HIRES_HILITE;
	}
	CAnimsManager::LoadAnims(g_dwHiliteAnimationId);
}

// FUNCTION: LEMBALL 0x0044f590
void CHiliteController::SetHiliteWindow()
{
	void* storage = operator new(0x90);
	if (storage == 0) {
		m_hiliteWindow = 0;
	}
	else {
		m_hiliteWindow = new (storage) CHiliteWindow();
	}
	CVsRect rect(m_window->m_rect);
	rect.m_x = 0;
	rect.m_y = 0;
	m_hiliteWindow->Create(rect, (CPvGWnd*) m_window, 0);
	m_hiliteSurface = (void*) m_hiliteWindow->m_gdi;
}

// FUNCTION: LEMBALL 0x0044f610
CHiliteController::~CHiliteController()
{
	g_pMasterInputQueue->Detach(this, 0);
	for (int i = 0; i < 4; i++) {
		if (m_buttons[i] != 0) {
			delete m_buttons[i];
		}
	}
	CAnimsManager::UnLoadAnims(g_dwHiliteAnimationId);
	if (m_hiliteWindow->m_lifecycleRefs == 1) {
		m_hiliteWindow->Destroy();
	}
	if (m_hiliteWindow != 0) {
		delete m_hiliteWindow;
	}
}

// FUNCTION: LEMBALL 0x0044f6c0
int CHiliteController::ProcessMsg(Message* p_message)
{
	if (m_active == 0) {
		return 0;
	}
	switch ((unsigned int) p_message->type) {
	default:
		m_processedCount++;
		return 0;
	case 3:
		if (p_message->code == 0x1f || p_message->code == 0x22 || p_message->code == 0x4c) {
			CHiliteButtons* currentBtn = m_buttons[m_currentButton];
			CVsPoint point;
			currentBtn->m_button->OnButtonUp(point, 0);
			return 0;
		}
		break;
	case 4:
		switch (p_message->code) {
		case 1:
			if (m_horizontalMode == 0) {
				return 0;
			}
			MoveLeft();
			g_pSoundView->PlayEffect(SFX_CHANGEOP);
			return 1;
		case 2:
			if (m_horizontalMode == 0) {
				return 0;
			}
			MoveRight();
			g_pSoundView->PlayEffect(SFX_CHANGEOP);
			return 1;
		case 3:
			if (m_horizontalMode == 1) {
				return 0;
			}
			MoveLeft();
			g_pSoundView->PlayEffect(SFX_CHANGEOP);
			return 1;
		case 4:
			if (m_horizontalMode == 1) {
				return 0;
			}
			MoveRight();
			g_pSoundView->PlayEffect(SFX_CHANGEOP);
			return 1;
		case 0x1f:
		case 0x22:
		case 0x4c: {
			CHiliteButtons* currentBtn = m_buttons[m_currentButton];
			CVsPoint point;
			currentBtn->m_button->OnButtonDown(point, 0);
			break;
		}
		}
		break;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0044f8c0
void CHiliteController::AddButton(int p_x,
								  int p_y,
								  unsigned long* p_animIds,
								  unsigned int p_mode,
								  int p_minimum,
								  int p_maximum,
								  int p_value,
								  void* p_binding,
								  unsigned long p_actionMessage)
{
	unsigned long controlMessage = ++m_nextControlMessage;
	m_buttons[m_buttonCount] = new CHiliteButtons(m_window,
												  m_gdi,
												  p_x,
												  p_y,
												  p_animIds,
												  p_mode,
												  p_minimum,
												  p_maximum,
												  p_value,
												  controlMessage,
												  p_binding,
												  p_actionMessage);
	AddHJunction(p_x, p_y, m_buttons[m_buttonCount]->m_controlMessage);
	m_buttonCount++;
}

// FUNCTION: LEMBALL 0x0044f970
void CHiliteController::AddHJunction(int p_x, int p_y, unsigned long p_controlMessage)
{
	m_junctions[m_buttonCount].m_present = 1;
	m_junctions[m_buttonCount].m_x = p_x;
	m_junctions[m_buttonCount].m_y = p_y;
	m_junctions[m_buttonCount].m_controlMessage = p_controlMessage;
}

// FUNCTION: LEMBALL 0x0044f9d0
void CHiliteController::DrawButtons(int p_force)
{
	int count;
	CHiliteButtons** buttonPtr;

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
void CHiliteController::DrawHiliteWindow()
{
	if (m_active != 0) {
		int offset = m_layoutMode == 1 ? -1 : -2;
		CGdi* hiliteGdi = (CGdi*) m_hiliteSurface;
		CSurface* surface = hiliteGdi->m_renderTarget;
		CVsSize dimensions(surface->m_windowRect);
		m_hiliteRect.m_color = 0x10000;
		m_hiliteRect.m_bounds.m_width = dimensions.m_width;
		m_hiliteRect.m_bounds.m_height = dimensions.m_height;
		m_hiliteRect.m_bounds.m_x = 0;
		m_hiliteRect.m_bounds.m_y = 0;
		m_hiliteRect.Draw(hiliteGdi);
		m_hiliteAnim.m_frameState = 0;
		CVsPoint position;
		position.m_x = (short) m_currentX + (short) offset;
		position.m_y = (short) m_currentY + (short) offset;
		unsigned long animationId = g_dwHiliteAnimationId;
		CGdi* savedGdi = CAnimsManager::m_gdi;
		CAnimsManager::m_gdi = (CGdi*) m_hiliteSurface;
		CAnimsManager::DrawAnim(position, animationId, 0, (CFrames*) &m_hiliteAnim, 0);
		CAnimsManager::m_gdi = savedGdi;
		CAnimsManager::ResetPrimitives();
	}
}

// FUNCTION: LEMBALL 0x0044fae0
void CHiliteController::MoveLeft()
{
	int nextButton = m_currentButton - 1;
	if (nextButton >= 0) {
		m_currentButton = nextButton;
		SetHilite(nextButton);
	}
}

// FUNCTION: LEMBALL 0x0044fb00
void CHiliteController::MoveRight()
{
	int nextButton = m_currentButton + 1;
	if (nextButton < m_buttonCount) {
		m_currentButton = nextButton;
		SetHilite(nextButton);
	}
}

// FUNCTION: LEMBALL 0x0044fb20
void CHiliteController::SetHilite(int p_buttonIndex)
{
	m_currentX = m_junctions[p_buttonIndex].m_x;
	m_currentY = m_junctions[p_buttonIndex].m_y;
	m_targetX = m_currentX;
	m_targetY = m_currentY;
	m_transitionStart = m_transitionEnd = CurrentMilliTimer();
	m_currentButton = p_buttonIndex;
}

// FUNCTION: LEMBALL 0x0044fb70
void CHiliteController::PostSelectionMessage()
{
	m_navigationState.type = 0xc;
	m_navigationState.time = CurrentQueueTimer();
	m_navigationState.code = m_junctions[m_currentButton].m_controlMessage;
	g_pMasterInputQueue->Post(m_navigationState);
	g_pSoundView->PlayEffect(SFX_GUNHIT);
}

// FUNCTION: LEMBALL 0x0044fbc0
void CHiliteController::Process()
{
}

// FUNCTION: LEMBALL 0x0044fbd0
void CHiliteController::ActivateButtons(int p_active)
{
	m_buttonsActive = p_active;
	int i = 0;
	if (m_buttonCount > i) {
		CHiliteButtons** pBtn = m_buttons;
		do {
			CHiliteButtons* btn = *pBtn;
			if (btn != 0) {
				btn->m_active = p_active;
				btn->m_button->SetActive(p_active);
			}
			pBtn++;
			i++;
		} while (m_buttonCount > i);
	}
}

// FUNCTION: LEMBALL 0x0044fc20
void CHiliteController::UpdateAllAnimIDs()
{
	int i = 0;
	if (m_buttonCount > i) {
		CHiliteButtons** button = m_buttons;
		do {
			if (*button != 0) {
				(*button)->UpdateAnimId();
			}
			button++;
			i++;
		} while (m_buttonCount > i);
	}
}

// FUNCTION: LEMBALL 0x0044fc50
void CHiliteController::UpdateAnimIDs(unsigned long p_actionMessage)
{
	int i = 0;
	if (m_buttonCount > i) {
		CHiliteButtons** pBtn = m_buttons;
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
