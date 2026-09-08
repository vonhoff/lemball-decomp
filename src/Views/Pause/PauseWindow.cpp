#include "PauseWindow.h"

#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Graphics/BasePalManager.h"
#include "../../Visos/Graphics/Cursor.h"
#include "../../Visos/Graphics/HotAreaList.h"
#include "../../Visos/Graphics/ReceiveWindowState.h"
#include "../../Visos/Resources/ResAnim.h"
#include "../../Visos/Resources/ResFont.h"
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
// FUNCTION: LEMBALL 0x00443d40
void PauseWindow::UnLoad()
{
	if (m_loaded != 0) {
		m_textManager.UnLoadFont(m_fontId);
		m_verticalBorderAnim->UnLoad();
		m_horizontalBorderAnim->UnLoad();
		m_loaded = 0;
	}
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
// FUNCTION: LEMBALL 0x00443db0
void PauseWindow::CreateTheWindow(const VsRect& p_rect)
{
	short verticalOffset = (short) m_verticalTextOffset;
	VsPoint point;
	VsRect borderRect;

	if (m_pauseMessage == 3) {
		short x = (short) ((p_rect.m_width - m_textSizes[0].m_x) / 2);
		m_textSizes[1].m_x = x;
		m_textSizes[1].m_y = verticalOffset;
		verticalOffset = (short) (verticalOffset + m_textSizes[0].m_y + m_textSpacing.m_y);
		m_textSizes[3].m_x = x;
		m_textSizes[3].m_y = verticalOffset;
		m_textSizes[5].m_x = (short) (x + m_textSizes[0].m_x - m_textSizes[4].m_x);
		m_textSizes[5].m_y = verticalOffset;
	}
	else if (m_menuItemCount > 0) {
		int item = 0;
		VsPoint* textSize = m_textSizes;
		VsPoint* textPosition = m_textSizes + 1;
		do {
			item++;
			textPosition->m_x = (short) ((p_rect.m_width - textSize->m_x) / 2);
			textPosition->m_y = verticalOffset;
			verticalOffset = (short) (verticalOffset + textSize->m_y + m_textSpacing.m_y);
			textSize += 2;
			textPosition += 2;
		} while (item < m_menuItemCount);
	}

	m_width = p_rect.m_width;
	m_height = p_rect.m_height;
	m_x = p_rect.m_x;
	m_y = p_rect.m_y;
	HotAreaHandler::SetActive(1);
	m_externalEnabled = 1;

	if (m_lifecycleRefs == 1) {
		PvWnd::SetRect(p_rect);
	}
	else {
		m_gdiFlags = m_borderAnimCount * 2 + 0x3ed;
		Create(p_rect, m_parentWindow, "Pause mode");
		m_parentWindow->m_hotAreaList->AddToList(this);
	}

	point = *(const VsPoint*) &p_rect;
	borderRect.m_width = point.m_x;
	borderRect.m_height = point.m_y;
	borderRect.m_x = 0;
	borderRect.m_y = 0;
	m_borderPadding.m_x = 4;
	m_borderPadding.m_y = 4;
	if (m_lowResolution == 0) {
		m_borderPadding.m_x = 8;
		m_borderPadding.m_y = 8;
	}
	point.m_x = m_borderPadding.m_x;
	point.m_y = m_borderPadding.m_y;
	((VsPoint*) &borderRect.m_x)->AddInPlace(&point);
	point.m_x = (short) (m_borderPadding.m_x * 2);
	point.m_y = (short) (m_borderPadding.m_y * 2);
	((VsPoint*) &borderRect.m_width)->SubtractInPlace(&point);
	m_borderLine.m_x1 = borderRect.m_width;
	m_borderLine.m_y1 = borderRect.m_height;
	m_borderLine.m_x2 = borderRect.m_x;
	m_borderLine.m_y2 = borderRect.m_y;
	m_borderLine.m_color = 0xc;
}

// 68K 0x10b0e6ce CalculateWindow__12CPauseWindowFv
// FUNCTION: LEMBALL 0x00444050
VsRect PauseWindow::CalculateWindow()
{
	VsRect result;
	VsPoint position;
	VsSize textSize;
	VsPoint maxTextSize;
	VsSize* measuredTextSize;
	VsSize windowSize;
	short parentWidth;
	short parentHeight;
	short horizontalWidth;
	short verticalHeight;
	int itemCount;
	int i;

	if (m_lowResolution != 0) {
		m_textSpacing.m_x = 0;
		m_textSpacing.m_y = 2;
		m_windowPadding.m_x = 0x14;
		m_windowPadding.m_y = 0x0a;
	}
	else {
		m_textSpacing.m_x = 0;
		m_textSpacing.m_y = 4;
		m_windowPadding.m_x = 0x28;
		m_windowPadding.m_y = 0x14;
	}

	maxTextSize.m_x = 0;
	maxTextSize.m_y = 0;
	itemCount = m_menuItemCount;
	if (m_pauseMessage == 3) {
		itemCount--;
	}
	for (i = 0; i < m_menuItemCount; i++) {
		measuredTextSize = m_font->GetSize(&textSize, m_menuLabels[i], 0x20);
		m_textSizes[i * 2].m_x = measuredTextSize->m_width;
		m_textSizes[i * 2].m_y = measuredTextSize->m_height;
		if (i < itemCount) {
			maxTextSize.m_y += measuredTextSize->m_height + m_textSpacing.m_y;
		}
		if (maxTextSize.m_x < measuredTextSize->m_width) {
			maxTextSize.m_x = measuredTextSize->m_width;
		}
	}
	maxTextSize.m_y -= m_textSpacing.m_y;
	maxTextSize.m_x += m_windowPadding.m_x;

	if ((int) m_parentWindow->m_innerRect.m_width * (int) m_parentWindow->m_innerRect.m_height != 0) {
		parentWidth = (short) ((int) m_parentWindow->m_innerRect.m_width / (int) m_parentWindow->m_zoom);
		parentHeight = (short) ((int) m_parentWindow->m_innerRect.m_height / (int) m_parentWindow->m_zoom);
	}
	else {
		parentWidth = (short) ((int) m_parentWindow->m_rect.m_width / (int) m_parentWindow->m_zoom);
		parentHeight = (short) ((int) m_parentWindow->m_rect.m_height / (int) m_parentWindow->m_zoom);
	}

	windowSize.m_width = maxTextSize.m_x;
	windowSize.m_height = maxTextSize.m_y;
	position.m_x = (short) (((int) parentWidth - (int) windowSize.m_width) / 2);
	position.m_y = (short) (((int) parentHeight - (int) windowSize.m_height) / 2);

	horizontalWidth = m_verticalBorderAnim->m_animationEntries[0].m_width;
	verticalHeight = m_verticalBorderAnim->m_animationEntries[2].m_height;
	windowSize.m_width = (short) (((int) windowSize.m_width + horizontalWidth - 1) / horizontalWidth);
	windowSize.m_height = (short) (((int) windowSize.m_height + verticalHeight - 1) / verticalHeight);
	windowSize.m_width = (short) (windowSize.m_width * horizontalWidth);
	windowSize.m_height = (short) (windowSize.m_height * verticalHeight);
	m_verticalTextOffset = (short) (((int) windowSize.m_height - (int) maxTextSize.m_y) / 2);
	m_horizontalTiles = (short) ((int) windowSize.m_width / horizontalWidth - 2);
	m_verticalTiles = (short) ((int) windowSize.m_height / verticalHeight - 2);

	if (m_borderAnimCount != m_horizontalTiles + m_verticalTiles) {
		if (m_borderAnims != 0) {
			delete[] m_borderAnims;
		}
		m_borderAnimCount = m_horizontalTiles + m_verticalTiles;
		m_borderAnims = new Anim[m_borderAnimCount * 2];
	}

	m_cornerAnims[0].m_x = windowSize.m_width;
	m_cornerAnims[0].m_y = 0;
	m_cornerAnims[0].m_flags = 0;
	m_cornerAnims[0].m_remap = 0;
	m_cornerAnims[0].m_state = 0;
	m_cornerAnims[0].m_animIndex = 0;
	m_cornerAnims[0].m_animResource = m_horizontalBorderAnim;
	m_cornerAnims[1].m_x = windowSize.m_width;
	m_cornerAnims[1].m_y = 0;
	m_cornerAnims[1].m_flags = 1;
	m_cornerAnims[1].m_remap = 0;
	m_cornerAnims[1].m_state = 0;
	m_cornerAnims[1].m_animIndex = 1;
	m_cornerAnims[1].m_animResource = m_horizontalBorderAnim;
	m_cornerAnims[2].m_x = 0;
	m_cornerAnims[2].m_y = windowSize.m_height;
	m_cornerAnims[2].m_flags = 2;
	m_cornerAnims[2].m_remap = 0;
	m_cornerAnims[2].m_state = 0;
	m_cornerAnims[2].m_animIndex = 2;
	m_cornerAnims[2].m_animResource = m_horizontalBorderAnim;
	m_cornerAnims[3].m_x = windowSize.m_width;
	m_cornerAnims[3].m_y = windowSize.m_height;
	m_cornerAnims[3].m_flags = 3;
	m_cornerAnims[3].m_remap = 0;
	m_cornerAnims[3].m_state = 0;
	m_cornerAnims[3].m_animIndex = 3;
	m_cornerAnims[3].m_animResource = m_horizontalBorderAnim;

	{
		short topX = horizontalWidth;
		short bottomX = horizontalWidth;
		int index;

		for (index = 0; index < m_horizontalTiles; index++) {
			Anim& top = m_borderAnims[index];
			Anim& bottom = m_borderAnims[m_borderAnimCount + index];
			top.m_x = topX;
			top.m_y = 0;
			top.m_flags = 0;
			top.m_remap = 0;
			top.m_state = 0;
			top.m_animIndex = 0;
			top.m_animResource = m_verticalBorderAnim;
			bottom.m_x = bottomX;
			bottom.m_y = (short) (windowSize.m_height - verticalHeight);
			bottom.m_flags = 1;
			bottom.m_remap = 0;
			bottom.m_state = 0;
			bottom.m_animIndex = 1;
			bottom.m_animResource = m_verticalBorderAnim;
			topX = (short) (topX + horizontalWidth);
			bottomX = (short) (bottomX + horizontalWidth);
		}
	}

	{
		short leftX = position.m_x;
		short leftY = (short) (position.m_y + m_horizontalBorderAnim->m_animationEntries[0].m_height);
		short rightX =
			(short) (position.m_x + windowSize.m_width - m_verticalBorderAnim->m_animationEntries[2].m_width);
		short rightY = leftY;
		int index;

		for (index = 0; index < m_verticalTiles; index++) {
			Anim& left = m_borderAnims[m_horizontalTiles + index];
			Anim& right = m_borderAnims[m_borderAnimCount + m_horizontalTiles + index];
			left.m_x = leftX;
			left.m_y = leftY;
			left.m_flags = 2;
			left.m_remap = 0;
			left.m_state = 0;
			left.m_animIndex = 2;
			left.m_animResource = m_verticalBorderAnim;
			right.m_x = rightX;
			right.m_y = rightY;
			right.m_flags = 3;
			right.m_remap = 0;
			right.m_state = 0;
			right.m_animIndex = 3;
			right.m_animResource = m_verticalBorderAnim;
			leftY = (short) (leftY + verticalHeight);
			rightY = (short) (rightY + verticalHeight);
		}
	}

	result.m_width = windowSize.m_width;
	result.m_height = windowSize.m_height;
	result.m_x = position.m_x;
	result.m_y = position.m_y;
	return result;
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
		short relX = p_point.m_x - m_relativeTopLeft.m_x;
		short relY = p_point.m_y - m_relativeTopLeft.m_y;
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
	int pauseMessage = m_pauseMessage;

	if (pauseMessage == 0 || pauseMessage == 3) {
		switch (p_message->type) {
		case 4:
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
				if (pauseMessage != 0 || m_receiverState->GetPauser()) {
					m_receiverState->SetOptionSelection(m_initialSelection + 1);
					g_pSoundView->PlayEffect((eSoundEffect) 3);
				}
				return 1;
			}
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
