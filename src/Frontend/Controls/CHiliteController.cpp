#include "CHiliteController.h"

#include "../../Engine/Resources/Manifest.h"
#include "Application/SoundEffects.h"
#include "CHiliteButtons.h"
#include "CHiliteWindow.h"
#include "Engine/Animation/CAnimsManager.h"
#include "Engine/Animation/CStaticAnim.h"
#include "Engine/Graphics/Primitives/CClipRect.h"
#include "Engine/Graphics/Primitives/CGDI.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"
#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Queues/tagMESSAGE.h"
#include "Engine/Resources/ResourceLimits.h"
#include "Engine/Time/VsTime.h"
#include "Frontend/Controls/CHiliteController.h"
#include "Frontend/Controls/ControlMessages.h"
#include "Frontend/FrontendLayoutMode.h"
#include "GameView/Sound/CSoundView.h"
#include "Platform/Windows/Graphics/CSurface.h"
#include "Platform/Windows/Windowing/CGWnd.h"
#include "Platform/Windows/Windowing/CGraphicButton.h"
#include "Platform/Windows/Windowing/CPVGWnd.h"

#include <new.h>
#include <stddef.h>

class CAnimFrameBASE;

enum eHiliteNavigationMode {
	HILITE_NAVIGATION_HORIZONTAL = 0,
	HILITE_NAVIGATION_VERTICAL = 1
};

// FUNCTION: LEMBALL 0x0044f440
CHiliteController::CHiliteController(CGWnd* p_window,
									 CGDI* p_gdi,
									 int p_arg2,
									 unsigned int p_layoutMode,
									 unsigned int p_horizontalMode)
	: CAnimsManager(p_gdi, RESOURCE_ID_COUNT, 1, 1, 0, 0)
{
	int index;

	m_layoutMode = p_layoutMode;
	m_horizontalMode = p_horizontalMode;
	m_window = p_window;
	m_gdi = p_gdi;
	m_active = 1;
	m_nextControlMessage = FRONTEND_CONTROL_MESSAGE_ID_BASE;
	index = 0;
	while (index < 4) {
		m_buttons[index] = NULL;
		m_junctions[index].m_present = 0;
		index = index + 1;
	}
	m_buttonCount = 0;
	m_currentButton = 0;
	g_pMasterInputQueue->Attach(this, 0);
	if (m_layoutMode == FRONTEND_LAYOUT_COMPACT) {
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
	if (storage == NULL) {
		m_hiliteWindow = NULL;
	}
	else {
		m_hiliteWindow = new (storage) CHiliteWindow();
	}
	CVSRect rect(m_window->m_rect);
	rect.m_x = 0;
	rect.m_y = 0;
	m_hiliteWindow->Create(rect, (CPVGWnd*) m_window, NULL);
	m_hiliteSurface = (void*) m_hiliteWindow->m_gdi;
}

// FUNCTION: LEMBALL 0x0044f610
CHiliteController::~CHiliteController()
{
	g_pMasterInputQueue->Detach(this, 0);
	for (int i = 0; i < 4; i++) {
		if (m_buttons[i] != NULL) {
			delete m_buttons[i];
		}
	}
	CAnimsManager::UnLoadAnims(g_dwHiliteAnimationId);
	if (m_hiliteWindow->m_lifecycleRefs == 1) {
		m_hiliteWindow->Destroy();
	}
	if (m_hiliteWindow != NULL) {
		delete m_hiliteWindow;
	}
}

// FUNCTION: LEMBALL 0x0044f6c0
int CHiliteController::ProcessMsg(tagMESSAGE* p_message)
{
	if (m_active == 0) {
		return 0;
	}
	switch ((unsigned int) p_message->m_type) {
	default:
		m_processedCount++;
		return 0;
	case MESSAGE_KEY_UP:
		if (p_message->m_code == INPUT_KEY_SPACE || p_message->m_code == INPUT_KEY_ACTIVATE ||
			p_message->m_code == INPUT_KEY_RETURN) {
			CGraphicButton* button = m_buttons[m_currentButton]->m_button;
			button->OnButtonUp(CVSPoint(0, 0), MOUSE_BUTTON_INDEX_LEFT);
			return 0;
		}
		break;
	case MESSAGE_KEY_DOWN:
		switch (p_message->m_code) {
		case INPUT_KEY_UP:
			if (m_horizontalMode == HILITE_NAVIGATION_HORIZONTAL) {
				return 0;
			}
			MoveLeft();
			g_pSoundView->PlayEffect(SFX_CHANGEOP);
			return 1;
		case INPUT_KEY_DOWN:
			if (m_horizontalMode == HILITE_NAVIGATION_HORIZONTAL) {
				return 0;
			}
			MoveRight();
			g_pSoundView->PlayEffect(SFX_CHANGEOP);
			return 1;
		case INPUT_KEY_LEFT:
			if (m_horizontalMode == HILITE_NAVIGATION_VERTICAL) {
				return 0;
			}
			MoveLeft();
			g_pSoundView->PlayEffect(SFX_CHANGEOP);
			return 1;
		case INPUT_KEY_RIGHT:
			if (m_horizontalMode == HILITE_NAVIGATION_VERTICAL) {
				return 0;
			}
			MoveRight();
			g_pSoundView->PlayEffect(SFX_CHANGEOP);
			return 1;
		case INPUT_KEY_SPACE:
		case INPUT_KEY_ACTIVATE:
		case INPUT_KEY_RETURN: {
			CGraphicButton* button = m_buttons[m_currentButton]->m_button;
			button->OnButtonDown(CVSPoint(0, 0), MOUSE_BUTTON_INDEX_LEFT);
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
	void* storage = operator new(sizeof(CHiliteButtons));
	m_buttons[m_buttonCount] = storage != NULL ? new (storage) CHiliteButtons(m_window,
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
																			  p_actionMessage)
											   : NULL;
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
		if (*buttonPtr != NULL) {
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
		int offset = m_layoutMode == FRONTEND_LAYOUT_COMPACT ? -1 : -2;
		CGDI* hiliteGdi = (CGDI*) m_hiliteSurface;
		CSurface* surface = hiliteGdi->m_renderTarget;
		CVSSize dimensions(surface->m_windowRect);
		m_hiliteRect.m_flags = CClipRect::CLIP_IGNORE_PARENT;
		m_hiliteRect.m_bounds.m_width = dimensions.m_width;
		m_hiliteRect.m_bounds.m_height = dimensions.m_height;
		m_hiliteRect.m_bounds.m_x = 0;
		m_hiliteRect.m_bounds.m_y = 0;
		m_hiliteRect.Draw(hiliteGdi);
		m_hiliteAnim.m_frameState = 0;
		CVSPoint position;
		position.m_x = (short) m_currentX + (short) offset;
		position.m_y = (short) m_currentY + (short) offset;
		unsigned long animationId = g_dwHiliteAnimationId;
		CGDI* savedGdi = CAnimsManager::m_gdi;
		CAnimsManager::m_gdi = (CGDI*) m_hiliteSurface;
		CAnimsManager::DrawAnim(position, animationId, 0, (CAnimFrameBASE*) &m_hiliteAnim, NULL);
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
	tagMESSAGE& posted = m_navigationState;
	posted.m_type = MESSAGE_BUTTON_RELEASED;
	m_navigationState.m_time = CurrentQueueTimer();
	m_navigationState.m_code = m_junctions[m_currentButton].m_controlMessage;
	g_pMasterInputQueue->Post(posted);
	g_pSoundView->PlayEffect(SFX_GUNHIT);
}

// FUNCTION: LEMBALL 0x0044fbc0
void CHiliteController::Process()
{
}

// FUNCTION: LEMBALL 0x0044fbd0
void CHiliteController::ActivateButtons(int p_active)
{
	int active = p_active;
	int initialCount = m_buttonCount;
	m_buttonsActive = active;
	int i = 0;
	if (initialCount > i) {
		do {
			CHiliteButtons* btn = m_buttons[i];
			if (btn != NULL) {
				btn->m_active = active;
				btn->m_button->SetActive(active);
			}
			i++;
			initialCount = m_buttonCount;
		} while (initialCount > i);
	}
}

// FUNCTION: LEMBALL 0x0044fc20
void CHiliteController::UpdateAllAnimIDs()
{
	int i = 0;
	if (m_buttonCount > i) {
		CHiliteButtons** button = m_buttons;
		do {
			if (*button != NULL) {
				(*button)->UpdateAnimID();
			}
			button++;
			i++;
		} while (m_buttonCount > i);
	}
}

// FUNCTION: LEMBALL 0x0044fc50
void CHiliteController::UpdateAnimIDs(unsigned long p_actionMessage)
{
	CHiliteButtons** buttonCursor;
	int buttonIndex = 0;
	if (m_buttonCount > buttonIndex) {
		buttonCursor = m_buttons;
		do {
			if (*buttonCursor != NULL && (*buttonCursor)->m_actionMessage == p_actionMessage) {
				m_buttons[buttonIndex]->UpdateAnimID();
				break;
			}
			buttonCursor++;
			buttonIndex++;
		} while (buttonIndex < m_buttonCount);
	}
}
