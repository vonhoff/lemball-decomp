#include "CPasswordDrawer.h"

#include "../../Control/Game/CGameStatus.h"
#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/CTextManager.h"
#include "../../Visos/Foundation/VsString.h"
#include "../../Visos/Graphics/CGDI.h"
#include "../../Visos/Graphics/CGraphicButton.h"
#include "../../Visos/Graphics/CPVButton.h"
#include "../../Visos/Graphics/CSurface.h"
#include "../../Visos/Resources/CResFONT.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Windows/CPasswordHiliteWindow.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Base/FlowProcesses.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Animation/CAnimsManager.h"
#include "Visos/Animation/CStaticAnim.h"
#include "Visos/Foundation/CVSPoint.h"
#include "Visos/Foundation/CVSRect.h"
#include "Visos/Foundation/CVSSize.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Foundation/tagPRIMS.h"
#include "Visos/Graphics/CBigBitmap.h"
#include "Visos/Graphics/CClipRect.h"
#include "Visos/Graphics/CPVGWnd.h"

#include <string.h>

class CAnimFrameBASE;
class CResBITMAP;

extern "C" unsigned long __stdcall timeGetTime(void);
extern char g_abPasswordLevelText[24];

#define PASSWORD_BUTTON_MESSAGE_FIRST 0xabcd00b0
#define PASSWORD_BUTTON_MESSAGE_LAST 0xabcd00bb
#define PASSWORD_BUTTON_MESSAGE_TO_INDEX_OFFSET 0x5432ff50
#define PASSWORD_BUTTON_COUNT 12
#define PASSWORD_CODE_LENGTH 10
#define PASSWORD_CLEAR_BUTTON_INDEX 10
#define PASSWORD_RETURN_DELAY_MS 1000

// GLOBAL: LEMBALL 0x0049ff48
PasswordTextLayout g_passwordLayoutFull = {0,
										   0,
										   384,
										   32,
										   16,
										   16,
										   {{16, 176}, {16, 208}, {16, 240}, {16, 272}},
										   {{192, 176}, {192, 208}, {192, 240}, {192, 272}},
										   64,
										   368,
										   64,
										   64,
										   {320, 368, 320, 40},
										   {32, 400}};

// GLOBAL: LEMBALL 0x0049fec8
PasswordTextLayout g_passwordLayoutCompact = {0,
											  0,
											  192,
											  16,
											  8,
											  8,
											  {{8, 88}, {8, 104}, {8, 120}, {8, 136}},
											  {{96, 88}, {96, 104}, {96, 120}, {96, 136}},
											  32,
											  184,
											  32,
											  32,
											  {160, 184, 160, 24},
											  {16, 200}};

// GLOBAL: LEMBALL 0x0049ffc8
unsigned long g_dwPasswordButtonAnimIdsFull[PASSWORD_BUTTON_COUNT] = {
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
unsigned long g_dwPasswordButtonAnimIdsCompact[PASSWORD_BUTTON_COUNT] = {
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
int g_passwordKeyMap[PASSWORD_BUTTON_COUNT] = {7, 8, 9, 4, 5, 6, 1, 2, 3, 10, 0, 11};

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
CPasswordDrawer::CPasswordDrawer(CMain2DDisplay* p_arg0, CGDI* p_arg1, const CVSRect& p_arg2)
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
		m_layout = &g_passwordLayoutCompact;
		m_buttonAnimIds = g_dwPasswordButtonAnimIdsCompact;
		m_animationId = RES_NEWFRONT_ANIMS_LORES_PASSWORD_HILITE;
	}
	else {
		m_layout = &g_passwordLayoutFull;
		m_buttonAnimIds = g_dwPasswordButtonAnimIdsFull;
		m_animationId = RES_NEWFRONT_ANIMS_HIRES_PASSWORD_HILITE;
	}
	primitiveIndex = 0;
	primitiveCount = 1;
	do {
		CResBITMAP* background = m_backgroundBitmap;
		PasswordTextLayout* layout = m_layout;
		int layoutY = layout->m_backgroundY;
		m_primitiveBundle[primitiveIndex].m_primitive.m_x = (short) layout->m_backgroundX;
		m_primitiveBundle[primitiveIndex].m_primitive.m_y = (short) layoutY;
		m_primitiveBundle[primitiveIndex].m_primitive.m_resource = background;
		m_primitiveBundle[primitiveIndex].m_primitive.m_flags = CBitmap::BITMAP_TRANSPARENT_ZERO;
		m_primitiveBundle[primitiveIndex].m_primitive.m_remap = 0;
		primitiveIndex++;
	} while (--primitiveCount != 0);
	CAnimsManager::LoadAnims(m_animationId);
	int* keyMap = g_passwordKeyMap;
	int* offsetPtr = m_buttonOffsets;
	gridStartX = m_layout->m_keypadX;
	gridY = m_layout->m_keypadY;
	gridX = gridStartX;
	buttonY = gridY;
	row = 4;
	do {
		col = 3;
		do {
			m_buttons[*keyMap] = new CGraphicButton(CVSPoint((short) gridX, (short) buttonY),
													(CPVGWnd*) m_display,
													m_buttonAnimIds[*keyMap],
													3);
			m_buttons[*keyMap]->m_controlMessage = PASSWORD_BUTTON_MESSAGE_FIRST + *keyMap;
			m_buttons[*keyMap]->m_messageQueue = g_pMasterInputQueue;
			offsetPtr[0] = gridX - m_layout->m_keypadX;
			offsetPtr[1] = buttonY - m_layout->m_keypadY;
			CGDI* buttonGdi = m_buttons[*keyMap]->m_gdi;
			CSurface* target = buttonGdi->m_renderTarget;
			m_buttons[*keyMap]->SetAutoDraw(0);
			target->m_flag70 = 0;
			gridX = gridX + m_layout->m_buttonWidth + m_layout->m_buttonGapX;
			offsetPtr = offsetPtr + 2;
			keyMap++;
			--col;
		} while (col != 0);
		gridX = gridStartX;
		gridY = gridY + m_layout->m_buttonHeight + m_layout->m_buttonGapY;
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
	} while (i < PASSWORD_BUTTON_COUNT);
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
	CVSPoint pt;
	pt.m_x = 0;
	pt.m_y = 0;
	m_buttons[g_passwordKeyMap[m_selectedButton]]->OnButtonUp(pt, 0);
	m_selectedButton += p_delta;
	if (m_selectedButton < 0) {
		m_selectedButton += PASSWORD_BUTTON_COUNT;
	}
	if (m_selectedButton >= PASSWORD_BUTTON_COUNT) {
		m_selectedButton -= PASSWORD_BUTTON_COUNT;
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

	switch (message->m_type) {
	case 3:
		code = message->m_code;
		switch (code) {
		case INPUT_KEY_SPACE:
		case 0x22: {
			CPVButton* button = m_buttons[g_passwordKeyMap[m_selectedButton]];
			CVSPoint pt(0, 0);
			button->OnButtonUp(pt, 0);
			return true;
		}
		case INPUT_KEY_RETURN: {
			CPVButton* button = m_buttons[11];
			CVSPoint pt(0, 0);
			button->OnButtonUp(pt, 0);
			return true;
		}
		case INPUT_KEY_DELETE:
		case INPUT_KEY_BACKSPACE: {
			CPVButton* button = m_buttons[PASSWORD_CLEAR_BUTTON_INDEX];
			CVSPoint pt(0, 0);
			button->OnButtonUp(pt, 0);
			return true;
		}
		}
		if (code >= INPUT_KEY_0 && code <= INPUT_KEY_9) {
			{
				CPVButton* button = m_buttons[code - INPUT_KEY_0];
				CVSPoint pt(0, 0);
				button->OnButtonUp(pt, 0);
				return true;
			}
		}
		break;
	case 4:
		code = message->m_code;
		switch (code) {
		case INPUT_KEY_UP:
			ShiftHilite(-3);
			return true;
		case INPUT_KEY_DOWN:
			ShiftHilite(3);
			return true;
		case INPUT_KEY_LEFT:
			ShiftHilite(-1);
			return true;
		case INPUT_KEY_RIGHT:
			ShiftHilite(1);
			return true;
		case INPUT_KEY_SPACE:
		case 0x22: {
			CPVButton* button = m_buttons[g_passwordKeyMap[m_selectedButton]];
			CVSPoint pt(0, 0);
			button->OnButtonDown(pt, 0);
			return true;
		}
		case INPUT_KEY_RETURN: {
			CPVButton* button = m_buttons[11];
			CVSPoint pt(0, 0);
			button->OnButtonDown(pt, 0);
			return true;
		}
		case INPUT_KEY_DELETE:
		case INPUT_KEY_BACKSPACE: {
			CPVButton* button = m_buttons[PASSWORD_CLEAR_BUTTON_INDEX];
			CVSPoint pt(0, 0);
			button->OnButtonDown(pt, 0);
			return true;
		}
		}
		if (code >= INPUT_KEY_0 && code <= INPUT_KEY_9) {
			{
				CPVButton* button = m_buttons[code - INPUT_KEY_0];
				CVSPoint pt(0, 0);
				button->OnButtonDown(pt, 0);
				return true;
			}
		}
		break;
	case MESSAGE_BUTTON_PRESSED:
		g_pSoundView->PlayEffect(SFX_DRUM1);
		break;
	case MESSAGE_BUTTON_RELEASED:
		code = message->m_code;
		if (code >= PASSWORD_BUTTON_MESSAGE_FIRST && code <= PASSWORD_BUTTON_MESSAGE_LAST) {
			ButtonNumeric(code + PASSWORD_BUTTON_MESSAGE_TO_INDEX_OFFSET);
			return true;
		}
		break;
	default:
		m_processedCount = m_processedCount + 1;
		return false;
	}
	return false;
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
	PasswordTextPosition* countPos;
	int skillIndex;
	PasswordTextPosition* labelPos;
	char* textPtr;
	countPos = m_layout->m_countPositions;
	labelPos = m_layout->m_labelPositions;
	textPtr = g_abPasswordLevelText;
	skillIndex = 0;
	do {
		CVSSize advance;
		CVSPoint position((short) labelPos->m_x, (short) labelPos->m_y);
		m_textManager
			->DrawString(m_gdi, position, advance, m_chalkFontId, g_apPasswordSkillLabels[skillIndex], 0x20, 0);
		strcpy(textPtr, g_szPasswordLevelFormat);
		vsLtoa(g_pGameStatus->m_maxLevels[skillIndex] + 1, textPtr + 2, 10);
		CVSSize countAdvance;
		CVSPoint countPosition((short) countPos->m_x, (short) countPos->m_y);
		m_textManager->DrawString(m_gdi, countPosition, countAdvance, m_chalkFontId, textPtr, 0x20, 0);
		countPos++;
		skillIndex++;
		labelPos++;
		textPtr = textPtr + 6;
	} while (textPtr < g_abPasswordLevelText + 24);
	if (m_passwordSubmitted == 1) {
		if (m_passwordValid == 1) {
			CVSSize advance;
			CVSPoint position((short) m_layout->m_resultPosition.m_x, (short) m_layout->m_resultPosition.m_y);
			m_textManager->DrawString(m_gdi, position, advance, m_chalkFontId, g_szPasswordOk, 0x20, 0);
		}
		else {
			CVSSize advance;
			CVSPoint position((short) m_layout->m_resultPosition.m_x, (short) m_layout->m_resultPosition.m_y);
			m_textManager->DrawString(m_gdi, position, advance, m_chalkFontId, g_szPasswordInvalid, 0x20, 0);
		}
	}
}

// FUNCTION: LEMBALL 0x00451c90
void CPasswordDrawer::DrawPassword()
{
	PasswordTextLayout* layout = m_layout;
	int y = layout->m_passwordY;
	int x = m_size.m_width - layout->m_passwordRightMargin;
	CVSSize textSize = m_textManager->GetFont(m_chalkFontId)->GetSize(m_password, 0x20);
	x -= textSize.m_width;
	CVSPoint position((short) x, (short) y);
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
		if (m_passwordLength < PASSWORD_CODE_LENGTH) {
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
		if (m_passwordLength >= 0 && m_passwordLength < PASSWORD_CODE_LENGTH) {
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
		m_returnDeadline = m_submitTime + PASSWORD_RETURN_DELAY_MS;
		break;
	default:
		break;
	}
}

// FUNCTION: LEMBALL 0x00451e40 FOLDED
void CPasswordDrawer::DrawButtons()
{
	for (int i = 0; i < PASSWORD_BUTTON_COUNT; i++) {
		m_buttons[i]->Draw(1);
	}
}

// FUNCTION: LEMBALL 0x00451e60
void CPasswordDrawer::DrawHilite()
{
	CGDI* savedGdi;
	CSurface* surface;

	surface = ((CGDI*) m_hiliteSurface)->m_renderTarget;
	short width = surface->m_windowRect.m_width;
	short height = surface->m_windowRect.m_height;
	m_hiliteRect.m_flags = CClipRect::CLIP_IGNORE_PARENT;
	m_hiliteRect.m_bounds.m_width = width;
	m_hiliteRect.m_bounds.m_height = height;
	m_hiliteRect.m_bounds.m_x = 0;
	m_hiliteRect.m_bounds.m_y = 0;
	m_hiliteRect.Draw((CGDI*) m_hiliteSurface);
	CVSPoint position((short) m_hiliteX, (short) m_hiliteY);
	savedGdi = CAnimsManager::m_gdi;
	m_hiliteAnim.m_frameState = 0;
	CAnimsManager::m_gdi = (CGDI*) m_hiliteSurface;
	CAnimsManager::DrawAnim(position, m_animationId, 0, (CAnimFrameBASE*) &m_hiliteAnim, 0);
	CAnimsManager::m_gdi = savedGdi;
}

// FUNCTION: LEMBALL 0x00451f10
void CPasswordDrawer::SetHiliteWindow()
{
	PasswordTextLayout* layout;
	int pitch;

	layout = m_layout;
	pitch = layout->m_buttonWidth + layout->m_buttonGapX;
	m_hiliteWindow = new CPasswordHiliteWindow();
	layout = m_layout;
	CVSRect rect((short) (layout->m_keypadX - 1),
				 (short) (layout->m_keypadY - 1),
				 (short) (pitch * 3),
				 (short) (pitch * 4));
	m_hiliteWindow->Create(rect, (CPVGWnd*) m_display, 0);
	m_hiliteSurface = (void*) m_hiliteWindow->m_gdi;
}
