#include "PasswordDrawer.h"

#include "../../Control/Game/GameStatus.h"
#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/PvButton.h"

#include <string.h>

// GLOBAL: LEMBALL 0x004a0028
int g_passwordKeyMap[12] = { 7, 8, 9, 4, 5, 6, 1, 2, 3, 10, 0, 11 };

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
// STUB: LEMBALL 0x00451320
void PasswordDrawer::Load()
{
}

// 68K 0x1080c7fc UnLoad__15CPasswordDrawerFv
// STUB: LEMBALL 0x00451550
void PasswordDrawer::UnLoad()
{
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
// STUB: LEMBALL 0x004516f0
bool PasswordDrawer::ProcessMessages(Message* p_message)
{
	return 0;
}

// 68K 0x1080cdce Processing__15CPasswordDrawerFv
// FUNCTION: LEMBALL 0x00451a70
void PasswordDrawer::Processing()
{
	unsigned long now;

	if (m_passwordSubmitted == 0) {
		return;
	}
	now = CurrentMilliTimer();
	if (m_returnDeadline < now) {
		m_quitYet = 1;
		m_returnState = 2;
	}
}

// 68K 0x1080ce1e DrawText__15CPasswordDrawerFv
// STUB: LEMBALL 0x00451aa0
void PasswordDrawer::DrawText()
{
}

// 68K 0x1080d034 DrawPassword__15CPasswordDrawerFv
// STUB: LEMBALL 0x00451c90
void PasswordDrawer::DrawPassword()
{
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
		} else {
			g_pSoundView->PlayEffect((eSoundEffect) 0x19);
		}
		break;
	case 10:
		if (m_passwordLength > 0) {
			m_passwordLength--;
		} else {
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
		m_submitTime = CurrentMilliTimer();
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
// STUB: LEMBALL 0x00451e60
void PasswordDrawer::DrawHilite()
{
}

// 68K 0x1080d3c8 SetHiliteWindow__15CPasswordDrawerFv
// STUB: LEMBALL 0x00451f10
void PasswordDrawer::SetHiliteWindow()
{
}

// 68K 0x1080c8ce __dt__15CPasswordDrawerFv
PasswordDrawer::~PasswordDrawer()
{
}

