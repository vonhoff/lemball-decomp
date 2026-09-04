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
	int* layout;
	unsigned long* animIds;
	int gridX;
	int gridY;
	int row;
	int col;
	int keyIndex;
	void* storage;
	VsPoint position;
	int* offsetPtr;
	int buttonIndex;

	if (m_mode == 0) {
		m_layout = g_abPasswordLayoutFull;
		m_buttonAnimIds = g_dwPasswordButtonAnimIdsFull;
		m_animationId = RES_NEWFRONT_ANIMS_HIRES_PASSWORD_HILITE;
	}
	else {
		m_layout = g_abPasswordLayoutCompact;
		m_buttonAnimIds = g_dwPasswordButtonAnimIdsCompact;
		m_animationId = RES_NEWFRONT_ANIMS_LORES_PASSWORD_HILITE;
	}
	layout = (int*) m_layout;
	animIds = (unsigned long*) m_buttonAnimIds;
	m_primitiveBundle.m_primitive.m_x = (short) layout[0];
	m_primitiveBundle.m_primitive.m_y = (short) layout[1];
	m_primitiveBundle.m_primitive.m_resource = m_backgroundBitmap;
	m_primitiveBundle.m_primitive.m_flags = 0x800;
	m_primitiveBundle.m_primitive.m_remap = 0;
	AnimsManager::LoadAnims(m_animationId);
	keyIndex = 0;
	offsetPtr = m_buttonOffsets;
	gridX = layout[2];
	gridY = layout[3];
	for (row = 0; row < 4; row++) {
		for (col = 0; col < 3; col++) {
			buttonIndex = g_passwordKeyMap[keyIndex];
			storage = operator new(0x130);
			if (storage == 0) {
				m_buttons[buttonIndex] = 0;
			}
			else {
				position.m_x = (short) gridX;
				position.m_y = (short) gridY;
				m_buttons[buttonIndex] =
					new (storage) GraphicButton(position, (PvGWnd*) m_display, animIds[buttonIndex], 3);
			}
			if (m_buttons[buttonIndex] != 0) {
				Gdi* buttonGdi;
				Surface* target;

				m_buttons[buttonIndex]->m_controlMessage = 0xabcd00b0 + buttonIndex;
				m_buttons[buttonIndex]->m_messageHandler = g_pMasterInputQueue;
				m_buttons[buttonIndex]->SetAutoDraw(0);
				buttonGdi = m_buttons[buttonIndex]->m_gdi;
				target = 0;
				if (buttonGdi != 0) {
					target = buttonGdi->m_renderTarget;
				}
				if (target != 0) {
					target->m_flag70 = 0;
				}
			}
			offsetPtr[0] = gridX - layout[2];
			offsetPtr[1] = gridY - layout[3];
			gridX = gridX + layout[0x60 / 4] + layout[4];
			keyIndex++;
			offsetPtr = offsetPtr + 2;
		}
		gridX = layout[2];
		gridY = gridY + layout[0x64 / 4] + layout[5];
	}
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
	int* layout;
	char text[24];
	char* textPtr;
	int skillIndex;
	short* labelPos;
	short* countPos;
	VsPoint position;
	VsSize advance;

	layout = (int*) m_layout;
	labelPos = (short*) &layout[0x1c / 4];
	countPos = (short*) &layout[0x3c / 4];
	textPtr = text;
	skillIndex = 0;
	do {
		advance.m_width = 0;
		advance.m_height = 0;
		position.m_y = labelPos[0];
		position.m_x = (short) *(int*) (labelPos - 1);
		m_textManager->DrawString(m_gdi, position, advance, m_unknown384, g_apPasswordSkillLabels[skillIndex], 0x20, 0);
		strcpy(textPtr, g_szPasswordLevelFormat);
		VsLtoa(g_pGameStatus->m_maxLevels[skillIndex] + 1, textPtr + 2, 10);
		advance.m_width = 0;
		advance.m_height = 0;
		position.m_y = countPos[0];
		position.m_x = (short) *(int*) (countPos - 1);
		m_textManager->DrawString(m_gdi, position, advance, m_unknown384, text, 0x20, 0);
		labelPos = labelPos + 4;
		countPos = countPos + 4;
		skillIndex = skillIndex + 1;
		textPtr = textPtr + 6;
	} while (skillIndex < 4);
	if (m_passwordSubmitted == 1) {
		advance.m_width = 0;
		advance.m_height = 0;
		position.m_y = (short) layout[0x7c / 4];
		position.m_x = (short) layout[0x78 / 4];
		if (m_passwordValid == 1) {
			m_textManager->DrawString(m_gdi, position, advance, m_unknown384, g_szPasswordOk, 0x20, 0);
		}
		else {
			m_textManager->DrawString(m_gdi, position, advance, m_unknown384, g_szPasswordInvalid, 0x20, 0);
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
	font = m_textManager->GetFont(m_unknown384);
	textSize = font->GetSize(m_password, 0x20);
	position.m_y = (short) layout[0x5c / 4];
	position.m_x = (short) (m_width - layout[0x58 / 4] - textSize.m_x);
	textSize.m_x = 0;
	textSize.m_y = 0;
	m_textManager->DrawString(m_gdi, position, (const VsSize&) textSize, m_unknown384, m_password, 0x20, 0);
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
