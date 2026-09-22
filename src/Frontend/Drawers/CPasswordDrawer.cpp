#include "CPasswordDrawer.h"

#include "../../Control/Game/CGameStatus.h"
#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/CTextManager.h"
#include "../../Visos/Foundation/VsString.h"
#include "../../Visos/Graphics/CGdi.h"
#include "../../Visos/Graphics/CGraphicButton.h"
#include "../../Visos/Graphics/CPvButton.h"
#include "../../Visos/Graphics/CSurface.h"
#include "../../Visos/Resources/CResFont.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Windows/CPasswordHiliteWindow.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Base/FlowProcesses.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Animation/CAnimsManager.h"
#include "Visos/Animation/CStaticAnim.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Foundation/Prims.h"
#include "Visos/Graphics/CBitmapRes.h"
#include "Visos/Graphics/CPvGWnd.h"
#include "Visos/Graphics/CSolidRect.h"

#include <string.h>

class CFrames;
class CResBitmap;

extern "C" unsigned long __stdcall timeGetTime(void);
extern char g_abPasswordLevelText[24];

// GLOBAL: LEMBALL 0x0049ff48
unsigned char g_abPasswordLayoutFull[0x80] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
	0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xd0, 0x00,
	0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0xc0,
	0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00,
	0xf0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x70, 0x01, 0x00,
	0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x70, 0x01, 0x00, 0x00, 0x40, 0x01,
	0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x90, 0x01, 0x00, 0x00,
};

// GLOBAL: LEMBALL 0x0049fec8
unsigned char g_abPasswordLayoutCompact[0x80] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
	0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x68, 0x00,
	0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x60,
	0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
	0x78, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0xb8, 0x00, 0x00,
	0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0xb8, 0x00, 0x00, 0x00, 0xa0, 0x00,
	0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00,
};

// GLOBAL: LEMBALL 0x0049ffc8
unsigned long g_dwPasswordButtonAnimIdsFull[12] = {
	RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_0,
	RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_1,
	RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_2,
	RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_3,
	RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_4,
	RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_5,
	RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_6,
	RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_7,
	RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_8,
	RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_9,
	RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_CLR,
	RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_END,
};

// GLOBAL: LEMBALL 0x0049fff8
unsigned long g_dwPasswordButtonAnimIdsCompact[12] = {
	RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_0,
	RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_1,
	RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_2,
	RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_3,
	RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_4,
	RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_5,
	RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_6,
	RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_7,
	RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_8,
	RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_9,
	RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_CLR,
	RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_END,
};

// GLOBAL: LEMBALL 0x004a0028
int g_passwordKeyMap[12] = {7, 8, 9, 4, 5, 6, 1, 2, 3, 10, 0, 11};

// GLOBAL: LEMBALL 0x004a0070
char g_szPasswordSkillFun[] = "Fun";

// GLOBAL: LEMBALL 0x004a0074
char g_szPasswordSkillTricky[] = "Tricky";

// GLOBAL: LEMBALL 0x004a007c
char g_szPasswordSkillTaxing[] = "Taxing";

// GLOBAL: LEMBALL 0x004a0084
char g_szPasswordSkillMayhem[] = "Mayhem";

// GLOBAL: LEMBALL 0x004a0068
char* g_szPasswordOk = "Password OK!";

// GLOBAL: LEMBALL 0x004a006c
char* g_szPasswordInvalid = "Invalid Password!";

// GLOBAL: LEMBALL 0x004a00b0
char g_szPasswordLevelFormat[] = ": ";

// GLOBAL: LEMBALL 0x004a0058
char* g_apPasswordSkillLabels[4] = {
	g_szPasswordSkillFun,
	g_szPasswordSkillTricky,
	g_szPasswordSkillTaxing,
	g_szPasswordSkillMayhem,
};

// FUNCTION: LEMBALL 0x00451210
CPasswordDrawer::CPasswordDrawer(CMain2DDisplay* p_arg0, CGdi* p_arg1, const CVsRect& p_arg2)
	: CBaseFrontendDrawer(p_arg0, p_arg1, p_arg2, FLOW_PASSWORD, 10, 10, 0, 0x28, 0x30)
{
	char* encoded;

	encoded = g_pGameStatus->EncodePassword();
	strcpy(m_password, encoded);
	m_passwordValid = 0;
	m_passwordSubmitted = 0;
	m_passwordLength = 10;
	m_selectedButton = 4;
	m_drawBackground = 1;
	m_drawFrame = 1;
	m_drawSolid = 1;
	Setup();
}

// FUNCTION: LEMBALL 0x00451320
void CPasswordDrawer::Load()
{
	int primitiveCount;
	int primitiveIndex;
	int gridStartX;
	int gridX;
	int gridY;
	int row;
	int col;
	int buttonY;

	if (m_mode != 0) {
		m_layout = (int*) g_abPasswordLayoutCompact;
		m_buttonAnimIds = g_dwPasswordButtonAnimIdsCompact;
		m_animationId = RES_NEWFRONT_ANIMS_LORES_PASSWORD_HILITE;
	}
	else {
		m_layout = (int*) g_abPasswordLayoutFull;
		m_buttonAnimIds = g_dwPasswordButtonAnimIdsFull;
		m_animationId = RES_NEWFRONT_ANIMS_HIRES_PASSWORD_HILITE;
	}
	primitiveIndex = 0;
	primitiveCount = 1;
	do {
		CResBitmap* background = m_backgroundBitmap;
		int* layout = (int*) m_layout;
		int layoutY = layout[1];
		m_primitiveBundle[primitiveIndex].m_primitive.m_x = (short) layout[0];
		m_primitiveBundle[primitiveIndex].m_primitive.m_y = (short) layoutY;
		m_primitiveBundle[primitiveIndex].m_primitive.m_resource = background;
		m_primitiveBundle[primitiveIndex].m_primitive.m_flags = 0x800;
		m_primitiveBundle[primitiveIndex].m_primitive.m_remap = 0;
		primitiveIndex++;
	} while (--primitiveCount != 0);
	CAnimsManager::LoadAnims(m_animationId);
	int* keyMap = g_passwordKeyMap;
	int* offsetPtr = m_buttonOffsets;
	gridStartX = m_layout[2];
	gridY = m_layout[3];
	gridX = gridStartX;
	buttonY = gridY;
	row = 4;
	do {
		col = 3;
		do {
			m_buttons[*keyMap] = new CGraphicButton(CVsPoint((short) gridX, (short) buttonY),
													(CPvGWnd*) m_display,
													m_buttonAnimIds[*keyMap],
													3);
			m_buttons[*keyMap]->m_controlMessage = 0xabcd00b0 + *keyMap;
			m_buttons[*keyMap]->m_messageHandler = g_pMasterInputQueue;
			offsetPtr[0] = gridX - m_layout[2];
			offsetPtr[1] = buttonY - m_layout[3];
			CGdi* buttonGdi = m_buttons[*keyMap]->m_gdi;
			CSurface* target = buttonGdi->m_renderTarget;
			m_buttons[*keyMap]->SetAutoDraw(0);
			target->m_flag70 = 0;
			gridX = gridX + m_layout[0x60 / 4] + m_layout[4];
			keyMap++;
			offsetPtr = offsetPtr + 2;
			--col;
		} while (col != 0);
		gridX = gridStartX;
		gridY = gridY + m_layout[0x64 / 4] + m_layout[5];
		buttonY = gridY;
		--row;
	} while (row != 0);
	m_hiliteX = m_buttonOffsets[m_selectedButton * 2];
	m_hiliteY = m_buttonOffsets[m_selectedButton * 2 + 1];
	SetHiliteWindow();
}

// FUNCTION: LEMBALL 0x00451550
void CPasswordDrawer::UnLoad()
{
	int i;

	i = 0;
	do {
		if (m_buttons[i] != 0) {
			delete m_buttons[i];
		}
		i++;
	} while (i < 12);
	CAnimsManager::UnLoadAnims(m_animationId);
	if (m_hiliteWindow->m_lifecycleRefs == 1) {
		m_hiliteWindow->Destroy();
	}
	delete m_hiliteWindow;
}

// FUNCTION: LEMBALL 0x004515c0
CPasswordDrawer::~CPasswordDrawer()
{
	if (m_loaded != 0) {
		UnLoad();
	}
}

// FUNCTION: LEMBALL 0x00451600 FOLDED
void CPasswordDrawer::DrawBackGround()
{
	DrawButtons();
}

// FUNCTION: LEMBALL 0x00451610
void CPasswordDrawer::DrawAnims()
{
	DrawHilite();
	DrawPassword();
}

// FUNCTION: LEMBALL 0x00451630
void CPasswordDrawer::ShiftHilite(int p_delta)
{
	if (m_passwordSubmitted == 1) {
		return;
	}
	CVsPoint pt;
	pt.m_x = 0;
	pt.m_y = 0;
	m_buttons[g_passwordKeyMap[m_selectedButton]]->OnButtonUp(pt, 0);
	m_selectedButton += p_delta;
	if (m_selectedButton < 0) {
		m_selectedButton += 12;
	}
	if (m_selectedButton >= 12) {
		m_selectedButton -= 12;
	}
	m_hiliteX = m_buttonOffsets[m_selectedButton * 2];
	m_hiliteY = m_buttonOffsets[m_selectedButton * 2 + 1];
	g_pSoundView->PlayEffect(SFX_CHANGEOP);
}

// FUNCTION: LEMBALL 0x004516f0
bool CPasswordDrawer::ProcessMessages(Message* p_message)
{
	Message* message = p_message;
	unsigned int code;

	switch (message->type) {
	case 3:
		code = message->code;
		switch (code) {
		case 0x1f:
		case 0x22: {
			CPvButton* button = m_buttons[g_passwordKeyMap[m_selectedButton]];
			CVsPoint pt(0, 0);
			button->OnButtonUp(pt, 0);
			return 1;
		}
		case 0x4c: {
			CPvButton* button = m_buttons[11];
			CVsPoint pt(0, 0);
			button->OnButtonUp(pt, 0);
			return 1;
		}
		case 0x4d:
		case 0x4e: {
			CPvButton* button = m_buttons[10];
			CVsPoint pt(0, 0);
			button->OnButtonUp(pt, 0);
			return 1;
		}
		}
		if (code >= 0x39 && code <= 0x42) {
			{
				CPvButton* button = m_buttons[code - 0x39];
				CVsPoint pt(0, 0);
				button->OnButtonUp(pt, 0);
				return 1;
			}
		}
		break;
	case 4:
		code = message->code;
		switch (code) {
		case 1:
			ShiftHilite(-3);
			return 1;
		case 2:
			ShiftHilite(3);
			return 1;
		case 3:
			ShiftHilite(-1);
			return 1;
		case 4:
			ShiftHilite(1);
			return 1;
		case 0x1f:
		case 0x22: {
			CPvButton* button = m_buttons[g_passwordKeyMap[m_selectedButton]];
			CVsPoint pt(0, 0);
			button->OnButtonDown(pt, 0);
			return 1;
		}
		case 0x4c: {
			CPvButton* button = m_buttons[11];
			CVsPoint pt(0, 0);
			button->OnButtonDown(pt, 0);
			return 1;
		}
		case 0x4d:
		case 0x4e: {
			CPvButton* button = m_buttons[10];
			CVsPoint pt(0, 0);
			button->OnButtonDown(pt, 0);
			return 1;
		}
		}
		if (code >= 0x39 && code <= 0x42) {
			{
				CPvButton* button = m_buttons[code - 0x39];
				CVsPoint pt(0, 0);
				button->OnButtonDown(pt, 0);
				return 1;
			}
		}
		break;
	case 0xb:
		g_pSoundView->PlayEffect(SFX_DRUM1);
		break;
	case 0xc:
		code = message->code;
		if (code >= 0xabcd00b0 && code <= 0xabcd00bb) {
			ButtonNumeric(code + 0x5432ff50);
			return 1;
		}
		break;
	default:
		m_processedCount = m_processedCount + 1;
		return 0;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00451a70
void CPasswordDrawer::Processing()
{
	if (m_passwordSubmitted != 0) {
		if (timeGetTime() > m_returnDeadline) {
			m_quitYet = 1;
			m_returnState = 2;
		}
	}
}

// FUNCTION: LEMBALL 0x00451aa0
void CPasswordDrawer::DrawText()
{
	int* countPos;
	int skillIndex;
	int* labelPos;
	char* textPtr;
	countPos = &((PasswordTextLayout*) m_layout)->m_countPositions[0].m_y;
	labelPos = &((PasswordTextLayout*) m_layout)->m_labelPositions[0].m_y;
	textPtr = g_abPasswordLevelText;
	skillIndex = 0;
	do {
		CVsSize advance;
		CVsPoint position((short) labelPos[-1], (short) labelPos[0]);
		m_textManager
			->DrawString(m_gdi, position, advance, m_chalkFontId, g_apPasswordSkillLabels[skillIndex], 0x20, 0);
		strcpy(textPtr, g_szPasswordLevelFormat);
		VsLtoa(g_pGameStatus->m_maxLevels[skillIndex] + 1, textPtr + 2, 10);
		CVsSize countAdvance;
		CVsPoint countPosition((short) countPos[-1], (short) countPos[0]);
		m_textManager->DrawString(m_gdi, countPosition, countAdvance, m_chalkFontId, textPtr, 0x20, 0);
		labelPos += 2;
		countPos += 2;
		skillIndex++;
		textPtr = textPtr + 6;
	} while (textPtr < g_abPasswordLevelText + 24);
	if (m_passwordSubmitted == 1) {
		if (m_passwordValid == 1) {
			CVsSize advance;
			CVsPoint position((short) ((PasswordTextLayout*) m_layout)->m_resultPosition.m_x,
							  (short) ((PasswordTextLayout*) m_layout)->m_resultPosition.m_y);
			m_textManager->DrawString(m_gdi, position, advance, m_chalkFontId, g_szPasswordOk, 0x20, 0);
		}
		else {
			CVsSize advance;
			CVsPoint position((short) ((PasswordTextLayout*) m_layout)->m_resultPosition.m_x,
							  (short) ((PasswordTextLayout*) m_layout)->m_resultPosition.m_y);
			m_textManager->DrawString(m_gdi, position, advance, m_chalkFontId, g_szPasswordInvalid, 0x20, 0);
		}
	}
}

// FUNCTION: LEMBALL 0x00451c90
void CPasswordDrawer::DrawPassword()
{
	int* layout = (int*) m_layout;
	int y = layout[0x5c / 4];
	int x = m_width - layout[0x58 / 4];
	CVsSize textSize;
	CVsPoint position(
		(short) (x - m_textManager->GetFont(m_chalkFontId)->GetSize(&textSize, m_password, 0x20)->m_width),
		(short) y);
	textSize.m_height = 0;
	textSize.m_width = 0;
	m_textManager->DrawString(m_gdi, position, textSize, m_chalkFontId, m_password, 0x20, 0);
}

// FUNCTION: LEMBALL 0x00451d20
void CPasswordDrawer::ButtonNumeric(int p_button)
{
	if (m_passwordSubmitted == 1) {
		return;
	}

	switch (p_button) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		if (m_passwordLength < 10) {
			m_password[m_passwordLength] = '0' + p_button;
			m_passwordLength++;
		}
		else {
			g_pSoundView->PlayEffect(SFX_CHINK);
		}
		break;
	case 10:
		if (m_passwordLength > 0) {
			m_passwordLength--;
		}
		else {
			g_pSoundView->PlayEffect(SFX_CHINK);
		}
		if (m_passwordLength >= 0 && m_passwordLength < 10) {
			m_password[m_passwordLength] = '-';
		}
		break;
	case 11:
		m_passwordValid = g_pGameStatus->DecodePassword(m_password);
		DrawText();
		g_pGameStatus->GotoLastLevels();
		g_pSoundView->PlayEffect((eSoundEffect) (0x13 + (m_passwordValid ? 0 : 0x0f)));
		m_submitTime = timeGetTime();
		m_passwordSubmitted = 1;
		m_returnDeadline = m_submitTime + 1000;
		break;
	default:
		break;
	}
}

// FUNCTION: LEMBALL 0x00451e40 FOLDED
void CPasswordDrawer::DrawButtons()
{
	for (int i = 0; i < 12; i++) {
		m_buttons[i]->Draw(1);
	}
}

// FUNCTION: LEMBALL 0x00451e60
void CPasswordDrawer::DrawHilite()
{
	CGdi* savedGdi;
	CSurface* surface;

	surface = ((CGdi*) m_hiliteSurface)->m_renderTarget;
	short width = surface->m_windowRect.m_width;
	short height = surface->m_windowRect.m_height;
	m_hiliteRect.m_color = 0x10000;
	m_hiliteRect.m_bounds.m_width = width;
	m_hiliteRect.m_bounds.m_height = height;
	m_hiliteRect.m_bounds.m_x = 0;
	m_hiliteRect.m_bounds.m_y = 0;
	m_hiliteRect.Draw((CGdi*) m_hiliteSurface);
	CVsPoint position((short) m_hiliteX, (short) m_hiliteY);
	savedGdi = CAnimsManager::m_gdi;
	m_hiliteAnim.m_frameState = 0;
	CAnimsManager::m_gdi = (CGdi*) m_hiliteSurface;
	CAnimsManager::DrawAnim(position, m_animationId, 0, (CFrames*) &m_hiliteAnim, 0);
	CAnimsManager::m_gdi = savedGdi;
}

// FUNCTION: LEMBALL 0x00451f10
void CPasswordDrawer::SetHiliteWindow()
{
	int* layout;
	int pitch;

	layout = (int*) m_layout;
	pitch = layout[0x60 / 4] + layout[0x10 / 4];
	m_hiliteWindow = new CPasswordHiliteWindow();
	layout = (int*) m_layout;
	CVsRect rect((short) (layout[2] - 1), (short) (layout[3] - 1), (short) (pitch * 3), (short) (pitch * 4));
	m_hiliteWindow->Create(rect, (CPvGWnd*) m_display, 0);
	m_hiliteSurface = (void*) m_hiliteWindow->m_gdi;
}
