#include "PasswordDrawer.h"

#include "../../Control/Game/GameStatus.h"
#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/TextManager.h"
#include "../../Visos/Foundation/VsString.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/GraphicButton.h"
#include "../../Visos/Graphics/PvButton.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Resources/ResFont.h"
#include "../Windows/PasswordHiliteWindow.h"

#include <new.h>
#include <string.h>

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
char g_szPasswordOk[] = "Password OK!";

// GLOBAL: LEMBALL 0x004a006c
char g_szPasswordInvalid[] = "Invalid Password!";

// GLOBAL: LEMBALL 0x004a00b0
char g_szPasswordLevelFormat[] = ": ";

// GLOBAL: LEMBALL 0x004a0058
char* g_apPasswordSkillLabels[4] = {
	g_szPasswordSkillFun,
	g_szPasswordSkillTricky,
	g_szPasswordSkillTaxing,
	g_szPasswordSkillMayhem,
};

// 68K 0x1080c452 __ct__15CPasswordDrawerFP14CMain2DDisplayP4CGDIRC7CVSRect
// FUNCTION: LEMBALL 0x00451210
PasswordDrawer::PasswordDrawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2)
	: BaseFrontendDrawer(p_arg0, p_arg1, p_arg2, (eFlowProcesses) 0x10, 10, 10, 0, 0x28, 0x30)
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

// 68K 0x1080c588 Load__15CPasswordDrawerFv
// FUNCTION: LEMBALL 0x00451320
void PasswordDrawer::Load()
{
	Prims* primitiveBundle;
	int primitiveCount;
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
	primitiveCount = 1;
	primitiveBundle = &m_primitiveBundle;
	do {
		ResBitmap* background = m_backgroundBitmap;
		int* layout = (int*) m_layout;
		int layoutY = layout[1];
		primitiveBundle->m_primitive.m_x = (short) layout[0];
		primitiveBundle->m_primitive.m_y = (short) layoutY;
		primitiveBundle->m_primitive.m_resource = background;
		primitiveBundle->m_primitive.m_flags = 0x800;
		primitiveBundle->m_primitive.m_remap = 0;
		primitiveBundle++;
	} while (--primitiveCount != 0);
	AnimsManager::LoadAnims(m_animationId);
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
			m_buttons[*keyMap] = new GraphicButton(VsPoint((short) gridX, (short) buttonY),
												   (PvGWnd*) m_display,
												   m_buttonAnimIds[*keyMap],
												   3);
			m_buttons[*keyMap]->m_controlMessage = 0xabcd00b0 + *keyMap;
			m_buttons[*keyMap]->m_messageHandler = g_pMasterInputQueue;
			offsetPtr[0] = gridX - m_layout[2];
			offsetPtr[1] = buttonY - m_layout[3];
			Gdi* buttonGdi = m_buttons[*keyMap]->m_gdi;
			Surface* target = buttonGdi->m_renderTarget;
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

// 68K 0x1080c7fc UnLoad__15CPasswordDrawerFv
// FUNCTION: LEMBALL 0x00451550
void PasswordDrawer::UnLoad()
{
	int i;

	i = 0;
	do {
		if (m_buttons[i] != 0) {
			delete m_buttons[i];
		}
		i++;
	} while (i < 12);
	AnimsManager::UnLoadAnims(m_animationId);
	if (m_hiliteWindow->m_lifecycleRefs == 1) {
		m_hiliteWindow->Destroy();
	}
	delete m_hiliteWindow;
}

// 68K 0x1080c8ce __dt__15CPasswordDrawerFv
// FUNCTION: LEMBALL 0x004515c0
PasswordDrawer::~PasswordDrawer()
{
	if (m_loaded != 0) {
		UnLoad();
	}
}

// 68K 0x1080c972 DrawBackGround__15CPasswordDrawerFv
// FUNCTION: LEMBALL 0x00451600 FOLDED
void PasswordDrawer::DrawBackGround()
{
	DrawButtons();
}

// 68K 0x1080c9ac DrawAnims__15CPasswordDrawerFv
// FUNCTION: LEMBALL 0x00451610
void PasswordDrawer::DrawAnims()
{
	DrawHilite();
	DrawPassword();
}

// 68K 0x1080c9ea ShiftHilite__15CPasswordDrawerFi
// FUNCTION: LEMBALL 0x00451630
void PasswordDrawer::ShiftHilite(int p_delta)
{
	if (m_passwordSubmitted == 1) {
		return;
	}
	VsPoint pt;
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
	g_pSoundView->PlayEffect((eSoundEffect) 0x1b);
}

// 68K 0x1080cab8 ProcessMessages__15CPasswordDrawerFP10tagMESSAGE
// FUNCTION: LEMBALL 0x004516f0
bool PasswordDrawer::ProcessMessages(Message* p_message)
{
	VsPoint pt;
	unsigned int code;

	switch (p_message->type) {
	case 3:
		code = p_message->code;
		switch (code) {
		case 0x1f:
		case 0x22:
			pt.m_x = 0;
			pt.m_y = 0;
			m_buttons[g_passwordKeyMap[m_selectedButton]]->OnButtonDown(pt, 0);
			return 1;
		case 0x4c:
			pt.m_x = 0;
			pt.m_y = 0;
			m_buttons[11]->OnButtonDown(pt, 0);
			return 1;
		case 0x4d:
		case 0x4e:
			pt.m_x = 0;
			pt.m_y = 0;
			m_buttons[10]->OnButtonDown(pt, 0);
			return 1;
		}
		if (code >= 0x39 && code <= 0x42) {
			pt.m_x = 0;
			pt.m_y = 0;
			m_buttons[code - 0x39]->OnButtonDown(pt, 0);
			return 1;
		}
		break;
	case 4:
		code = p_message->code;
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
		case 0x22:
			pt.m_x = 0;
			pt.m_y = 0;
			m_buttons[g_passwordKeyMap[m_selectedButton]]->OnButtonUp(pt, 0);
			return 1;
		case 0x4c:
			pt.m_x = 0;
			pt.m_y = 0;
			m_buttons[11]->OnButtonUp(pt, 0);
			return 1;
		case 0x4d:
		case 0x4e:
			pt.m_x = 0;
			pt.m_y = 0;
			m_buttons[10]->OnButtonUp(pt, 0);
			return 1;
		}
		if (code >= 0x39 && code <= 0x42) {
			pt.m_x = 0;
			pt.m_y = 0;
			m_buttons[code - 0x39]->OnButtonUp(pt, 0);
			return 1;
		}
		break;
	case 0xb:
		g_pSoundView->PlayEffect((eSoundEffect) 0x25);
		break;
	case 0xc:
		code = p_message->code;
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

// 68K 0x1080cdce Processing__15CPasswordDrawerFv
// FUNCTION: LEMBALL 0x00451a70
void PasswordDrawer::Processing()
{
	if (m_passwordSubmitted != 0) {
		if (timeGetTime() > m_returnDeadline) {
			m_quitYet = 1;
			m_returnState = 2;
		}
	}
}

// 68K 0x1080ce1e DrawText__15CPasswordDrawerFv
// FUNCTION: LEMBALL 0x00451aa0
void PasswordDrawer::DrawText()
{
	short* countPos;
	short* labelPos;
	int skillOffset;
	char* textPtr;
	VsPoint position;
	VsSize advance;

	countPos = (short*) &((PasswordTextLayout*) m_layout)->m_countPositions[0].m_y;
	labelPos = (short*) &((PasswordTextLayout*) m_layout)->m_labelPositions[0].m_y;
	textPtr = g_abPasswordLevelText;
	skillOffset = 0;
	do {
		advance.m_width = 0;
		advance.m_height = 0;
		position.m_x = labelPos[-2];
		position.m_y = labelPos[0];
		m_textManager
			->DrawString(m_gdi, position, advance, m_chalkFontId, g_apPasswordSkillLabels[skillOffset / 4], 0x20, 0);
		strcpy(textPtr, g_szPasswordLevelFormat);
		VsLtoa(g_pGameStatus->m_maxLevels[skillOffset / 4] + 1, textPtr + 2, 10);
		advance.m_width = 0;
		advance.m_height = 0;
		position.m_x = countPos[-2];
		position.m_y = countPos[0];
		m_textManager->DrawString(m_gdi, position, advance, m_chalkFontId, textPtr, 0x20, 0);
		labelPos += 4;
		countPos += 4;
		skillOffset += 4;
		textPtr = textPtr + 6;
	} while (textPtr < g_abPasswordLevelText + 24);
	if (m_passwordSubmitted == 1) {
		advance.m_width = 0;
		advance.m_height = 0;
		position.m_y = (short) ((PasswordTextLayout*) m_layout)->m_resultPosition.m_y;
		position.m_x = (short) ((PasswordTextLayout*) m_layout)->m_resultPosition.m_x;
		if (m_passwordValid == 1) {
			m_textManager->DrawString(m_gdi, position, advance, m_chalkFontId, g_szPasswordOk, 0x20, 0);
		}
		else {
			m_textManager->DrawString(m_gdi, position, advance, m_chalkFontId, g_szPasswordInvalid, 0x20, 0);
		}
	}
}

// 68K 0x1080d034 DrawPassword__15CPasswordDrawerFv
// FUNCTION: LEMBALL 0x00451c90
void PasswordDrawer::DrawPassword()
{
	int* layout;
	VsPoint position;
	VsPoint textSize;
	ResFont* font;

	layout = (int*) m_layout;
	font = m_textManager->GetFont(m_chalkFontId);
	font->GetSize(&textSize, m_password, 0x20);
	position.m_y = (short) layout[0x5c / 4];
	position.m_x = (short) (m_width - layout[0x58 / 4] - textSize.m_x);
	textSize.m_x = 0;
	textSize.m_y = 0;
	m_textManager->DrawString(m_gdi, position, (const VsSize&) textSize, m_chalkFontId, m_password, 0x20, 0);
}

// 68K 0x1080d0ec ButtonNumeric__15CPasswordDrawerFi
// FUNCTION: LEMBALL 0x00451d20
void PasswordDrawer::ButtonNumeric(int p_button)
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
			g_pSoundView->PlayEffect((eSoundEffect) 0x19);
		}
		break;
	case 10:
		if (m_passwordLength > 0) {
			m_passwordLength--;
		}
		else {
			g_pSoundView->PlayEffect((eSoundEffect) 0x19);
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

// 68K 0x1080d266 DrawButtons__15CPasswordDrawerFv
// FUNCTION: LEMBALL 0x00451e40 FOLDED
void PasswordDrawer::DrawButtons()
{
	for (int i = 0; i < 12; i++) {
		m_buttons[i]->Draw(1);
	}
}

// 68K 0x1080d2c2 DrawHilite__15CPasswordDrawerFv
// FUNCTION: LEMBALL 0x00451e60
void PasswordDrawer::DrawHilite()
{
	Gdi* savedGdi;
	VsPoint position;
	Surface* surface;

	surface = ((Gdi*) m_hiliteSurface)->m_renderTarget;
	m_hiliteRect.m_color = 0x10000;
	m_hiliteRect.m_left = surface->m_windowRect.m_width;
	m_hiliteRect.m_top = surface->m_windowRect.m_height;
	m_hiliteRect.m_right = 0;
	m_hiliteRect.m_bottom = 0;
	m_hiliteRect.Draw((Gdi*) m_hiliteSurface);
	savedGdi = AnimsManager::m_gdi;
	m_hiliteAnim.m_frameState = 0;
	AnimsManager::m_gdi = (Gdi*) m_hiliteSurface;
	position.m_x = (short) m_hiliteX;
	position.m_y = (short) m_hiliteY;
	AnimsManager::DrawAnim(position, m_animationId, 0, (Frames*) &m_hiliteAnim, 0);
	AnimsManager::m_gdi = savedGdi;
}

// 68K 0x1080d3c8 SetHiliteWindow__15CPasswordDrawerFv
// FUNCTION: LEMBALL 0x00451f10
void PasswordDrawer::SetHiliteWindow()
{
	int* layout;
	short pitch;
	void* storage;

	layout = (int*) m_layout;
	pitch = (short) layout[0x60 / 4] + (short) layout[0x10 / 4];
	storage = operator new(0x90);
	if (storage == 0) {
		m_hiliteWindow = 0;
	}
	else {
		m_hiliteWindow = new (storage) PasswordHiliteWindow();
	}
	VsRect rect((short) (layout[2] - 1), (short) (layout[3] - 1), (short) (pitch * 3), (short) (pitch * 4));
	m_hiliteWindow->Create(rect, (PvGWnd*) m_display, 0);
	m_hiliteSurface = (void*) m_hiliteWindow->m_gdi;
}
