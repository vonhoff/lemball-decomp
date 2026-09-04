#include "PauseWindow.h"

#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Graphics/BasePalManager.h"
#include "../../Visos/Graphics/Cursor.h"
#include "../../Visos/Graphics/HotAreaList.h"
#include "../../Visos/Graphics/ReceiveWindowState.h"
#include "../Sound/SoundView.h"

// 68K 0x10b0e048 Initialise__12CPauseWindowFv
// STUB: LEMBALL 0x00443af0
void PauseWindow::Initialise()
{
}

// 68K 0x10b0e1da Load__12CPauseWindowFv
// STUB: LEMBALL 0x00443c70
void PauseWindow::Load()
{
}

// 68K 0x10b0e2a0 UnLoad__12CPauseWindowFv
// STUB: LEMBALL 0x00443d40
void PauseWindow::UnLoad()
{
}

// 68K 0x10b0e2fc Restart__12CPauseWindowFv
// FUNCTION: LEMBALL 0x00443d80
void PauseWindow::Restart()
{
	VsRect rect;
	UnLoad();
	Load();
	rect = CalculateWindow();
	CreateTheWindow(rect);
}

// 68K 0x10b0e390 CreateTheWindow__12CPauseWindowFRC7CVSRect
// STUB: LEMBALL 0x00443db0
void PauseWindow::CreateTheWindow(const VsRect& p_rect)
{
}

// 68K 0x10b0e6ce CalculateWindow__12CPauseWindowFv
// STUB: LEMBALL 0x00444050
VsRect PauseWindow::CalculateWindow()
{
	return *(VsRect*) 0;
}

// 68K 0x10b0eeba __ct__12CPauseWindowFP19CReceiveWindowStateP7CPVGWnd20ePauseWindowMessages
// FUNCTION: LEMBALL 0x00444680
PauseWindow::PauseWindow(ReceiveWindowState* p_arg0, PvGWnd* p_arg1, ePauseWindowMessages p_arg2)
	: m_textManager(0x2b6, 1, 15, 0)
{
	m_verticalTiles = 0;
	m_horizontalTiles = 0;
	m_windowPadding.m_x = 0;
	m_windowPadding.m_y = 0;
	m_textSpacing.m_x = 0;
	m_textSpacing.m_y = 0;
	m_borderPadding.m_x = 0;
	m_borderPadding.m_y = 0;
	m_receiverState = p_arg0;
	m_pauseMessage = p_arg2;
	m_parentWindow = p_arg1;
	m_cursorState = 0;
	m_borderAnimCount = 0;
	Initialise();
	Restart();
}

// 68K 0x10b0f02a __dt__12CPauseWindowFv
// FUNCTION: LEMBALL 0x00444790
PauseWindow::~PauseWindow()
{
	m_parentWindow->m_hotAreaList->RemoveFromList(this);
	if (m_lifecycleRefs == 1) {
		Destroy();
	}
	delete[] m_textSizes;
	UnRegisterRemaps();
	if (m_borderAnims != 0) {
		delete[] m_borderAnims;
	}
	UnLoad();
	g_pMasterInputQueue->Detach(this, 0);
}

// 68K 0x10b0f1aa RegisterRemaps__12CPauseWindowFv
// STUB: LEMBALL 0x004448c0
void PauseWindow::RegisterRemaps()
{
}

// 68K 0x10b0f222 UnRegisterRemaps__12CPauseWindowFv
// FUNCTION: LEMBALL 0x00444900
void PauseWindow::UnRegisterRemaps()
{
	int i;
	BaseRemap** remaps = m_remaps;

	i = 4;
	do {
		g_pBasePalManager->UnRegisterRemap(*remaps);
		remaps++;
		i--;
	} while (i != 0);
}

// 68K 0x10b0f282 Remap__12CPauseWindowFi
// FUNCTION: LEMBALL 0x00444930
BaseRemap* PauseWindow::Remap(int p_item)
{
	if (m_selection == p_item) {
		return m_remaps[2];
	}
	if (m_minimumSelection <= p_item) {
		return m_remaps[0];
	}
	if (p_item <= m_unavailableItems && p_item > 0) {
		return m_remaps[3];
	}
	return m_remaps[1];
}

// 68K 0x10b0f2d8 OnPaint__12CPauseWindowFRC7CVSRect
// STUB: LEMBALL 0x00444980
void PauseWindow::OnPaint(const VsRect& p_rect)
{
}

// 68K 0x10b0f404 OnInside__12CPauseWindowFRC8CVSPoint
// FUNCTION: LEMBALL 0x00444a90
void PauseWindow::OnInside(const VsPoint& p_point)
{
	int selection = m_minimumSelection;
	m_cursorState = 0;
	if (selection < m_menuItemCount) {
		short relX = p_point.m_x - m_rect.m_x;
		short relY = p_point.m_y - m_rect.m_y;
		VsPoint* textSizes = m_textSizes + selection * 2 + 1;
		do {
			short textX = textSizes->m_x;
			if (textX <= relX) {
				short boundX = textSizes[-1].m_x + textX;
				if (boundX > relX) {
					short textY = textSizes->m_y;
					if (textY <= relY) {
						short boundY = textSizes[-1].m_y + textY;
						if (boundY > relY) {
							m_cursorState = 4;
							m_selection = selection;
							break;
						}
					}
				}
			}
			textSizes += 2;
			selection++;
		} while (selection < m_menuItemCount);
	}
	CursorChangeType((eCursorDisplayType) 1, m_cursorState);
}

// 68K 0x10b0f506 OnButtonDown__12CPauseWindowFRC8CVSPoint12BUTTON_FLAGS
// STUB: LEMBALL 0x00444b20
void PauseWindow::OnButtonDown(const VsPoint& p_point, int p_flags)
{
}

// 68K 0x10b0f61a OnButtonUp__12CPauseWindowFRC8CVSPoint12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00444bd0
void PauseWindow::OnButtonUp(const VsPoint& p_point, int p_flags)
{
	m_cursorState = 0;
	CursorChangeType((eCursorDisplayType) 1, 0);
}

// 68K 0x10b0f67c OnExternalButtonUp__12CPauseWindowFRC8CVSPoint12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00444bf0
void PauseWindow::OnExternalButtonUp(const VsPoint& p_point, int p_flags)
{
	m_cursorState = 0;
	CursorChangeType((eCursorDisplayType) 1, 0);
}

// 68K 0x10b0f6e6 ProcessMsg__12CPauseWindowFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00444c10
int PauseWindow::ProcessMsg(Message* p_message)
{
	if ((m_pauseMessage == 0 || m_pauseMessage == 3) && p_message->type == 4) {
		switch (p_message->code) {
		case 1:
		case 3:
			if (m_selection > m_minimumSelection) {
				m_selection--;
				g_pSoundView->PlayEffect((eSoundEffect) 0x1b);
				return 1;
			}
			break;
		case 2:
		case 4:
			if (m_selection < m_menuItemCount - 1) {
				m_selection++;
				g_pSoundView->PlayEffect((eSoundEffect) 0x1b);
				return 1;
			}
			break;
		case 0x22:
		case 0x2a:
		case 0x2e:
		case 0x4c:
		case 0x57:
		case 0x58:
			m_receiverState->SetOptionSelection(m_selection + 1);
			g_pSoundView->PlayEffect((eSoundEffect) 3);
			return 1;
		case 0x23:
			if (m_pauseMessage != 0 || m_receiverState->GetPauser()) {
				m_receiverState->SetOptionSelection(m_initialSelection + 1);
				g_pSoundView->PlayEffect((eSoundEffect) 3);
			}
			return 1;
		}
	}
	return 0;
}

// 68K 0x10b0f8a0 FreeVram__12CPauseWindowFv
// STUB: LEMBALL 0x00444da0
void PauseWindow::FreeVram()
{
}

// 68K 0x10b0f8fc OnDriverChange__12CPauseWindowFv
// FUNCTION: LEMBALL 0x00444dd0
void PauseWindow::OnDriverChange()
{
	Restart();
}
