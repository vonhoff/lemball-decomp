#include "PreviewDrawer.h"

#include "../../Control/Game/GameStatus.h"
#include "../../Visos/Animation/RepeatAnim.h"
#include "../Base/BaseFrontendProcess.h"
#include "../Controls/HiliteController.h"

#include <new.h>

// 68K 0x1080adda __ct__14CPreviewDrawerFP14CMain2DDisplayP4CGDIRC7CVSRect
// FUNCTION: LEMBALL 0x004491b0
PreviewDrawer::PreviewDrawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2)
	: BaseFrontendDrawer(p_arg0, p_arg1, p_arg2, (eFlowProcesses) 4, 0x32, 200, 0, 0x28, 0x30)
{
	void* storage;

	m_drawBackground = 1;
	m_drawFrame = 1;
	m_drawSolid = 1;
	Setup();
	storage = operator new(0x1c);
	if (storage == 0) {
		m_lemmingAnim = 0;
	}
	else {
		m_lemmingAnim = new (storage) RepeatAnim(m_anims.GetnAnims(m_lemmingAnimId), 1);
	}
	m_lemmingAnim->StartAnim(500);
	m_lemmingAnim->m_fixedTime = 0xffffffff;
	storage = operator new(0x1c);
	if (storage == 0) {
		m_teamAnim = 0;
	}
	else {
		m_teamAnim = new (storage) RepeatAnim(m_anims.GetnAnims(m_teamAnimId), 1);
	}
	m_teamAnim->StartAnim(500);
	m_teamAnim->m_fixedTime = 0xffffffff;
	storage = operator new(0x1c);
	if (storage == 0) {
		m_opponentAnim = 0;
	}
	else {
		m_opponentAnim = new (storage) RepeatAnim(m_anims.GetnAnims(m_opponentAnimId), 1);
	}
	m_opponentAnim->StartAnim(500);
	m_opponentAnim->m_fixedTime = 0xffffffff;
	RegisterRemaps();
}

// 68K 0x1080b000 Load__14CPreviewDrawerFv
// STUB: LEMBALL 0x00449370
void PreviewDrawer::Load()
{
}

// 68K 0x1080b344 UnLoad__14CPreviewDrawerFv
// STUB: LEMBALL 0x00449670
void PreviewDrawer::UnLoad()
{
}

// 68K 0x1080b49c DrawBackGround__14CPreviewDrawerFv
// STUB: LEMBALL 0x00449750
void PreviewDrawer::DrawBackGround()
{
}

// 68K 0x1080b52c DrawText__14CPreviewDrawerFv
// STUB: LEMBALL 0x004497b0
void PreviewDrawer::DrawText()
{
}

// 68K 0x1080b82e DrawAnims__14CPreviewDrawerFv
// STUB: LEMBALL 0x00449a40
void PreviewDrawer::DrawAnims()
{
}

// 68K 0x1080bb42 ProcessMessages__14CPreviewDrawerFP10tagMESSAGE
// STUB: LEMBALL 0x00449d30
bool PreviewDrawer::ProcessMessages(Message* p_message)
{
	return 0;
}

// 68K 0x1080bc92 NextLevel__14CPreviewDrawerFv
// FUNCTION: LEMBALL 0x00449e60
void PreviewDrawer::NextLevel()
{
	g_pGameStatus->IncLevel();
	LoadLevelInformation();
}

// 68K 0x1080bcd0 PreviousLevel__14CPreviewDrawerFv
// FUNCTION: LEMBALL 0x00449e80
void PreviewDrawer::PreviousLevel()
{
	g_pGameStatus->DecLevel();
	LoadLevelInformation();
}

// 68K 0x1080bd14 Go__14CPreviewDrawerFv
// FUNCTION: LEMBALL 0x00449ea0
void PreviewDrawer::Go()
{
	m_quitYet = 1;
	m_returnState = 5;
}

// 68K 0x1080bd46 Return__14CPreviewDrawerFv
// FUNCTION: LEMBALL 0x00449ec0
void PreviewDrawer::Return()
{
	m_quitYet = 1;
	m_returnState = 2;
}

// 68K 0x1080bd7c ConfirmedAction__14CPreviewDrawerF12eUserActions
// FUNCTION: LEMBALL 0x00449ee0
bool PreviewDrawer::ConfirmedAction(int p_action)
{
	switch (p_action) {
	case 0:
		NextLevel();
		return 1;
	case 1:
		PreviousLevel();
		return 1;
	case 2:
		Go();
		return 1;
	case 3:
		Return();
		return 1;
	default:
		return 0;
	}
}

// 68K 0x1080be04 Processing__14CPreviewDrawerFv
// FUNCTION: LEMBALL 0x00449f40
void PreviewDrawer::Processing()
{
	if (g_nTestAllLevels) {
		m_quitYet = 1;
		m_returnState = 5;
	}
}

// 68K 0x1080be46 LoadLevelInformation__14CPreviewDrawerFv
// STUB: LEMBALL 0x00449f60
void PreviewDrawer::LoadLevelInformation()
{
}

// 68K 0x1080c14e SubWord__14CPreviewDrawerFPcPcRiRi
// STUB: LEMBALL 0x0044a250
void PreviewDrawer::SubWord(char* p_source, char* p_line, int& p_sourcePos, int& p_linePos)
{
}

// 68K 0x1080c1d4 AddWord__14CPreviewDrawerFPcPcRiRi
// STUB: LEMBALL 0x0044a2d0
bool PreviewDrawer::AddWord(char* p_source, char* p_line, int& p_sourcePos, int& p_linePos)
{
	return 0;
}

// 68K 0x1080c256 RegisterRemaps__14CPreviewDrawerFv
// STUB: LEMBALL 0x0044a330
void PreviewDrawer::RegisterRemaps()
{
}

// 68K 0x1080c318 UnRegisterRemaps__14CPreviewDrawerFv
// STUB: LEMBALL 0x0044a3c0
void PreviewDrawer::UnRegisterRemaps()
{
}

// 68K 0x1080c35c DisableNextLastButtons__14CPreviewDrawerFv
// FUNCTION: LEMBALL 0x0044a3e0
void PreviewDrawer::DisableNextLastButtons()
{
	bool next = g_pGameStatus->NextLevelAvailable();
	bool last = g_pGameStatus->LastLevelAvailable();

	m_nextDisabled = 0;
	if (next != 1) {
		m_nextDisabled = 1;
	}

	m_previousDisabled = 0;
	if (last != 1) {
		m_previousDisabled = 1;
	}

	m_hiliteController->UpdateAnimIDs(0xacef000e);
	m_hiliteController->UpdateAnimIDs(0xacef000f);
}

// 68K 0x1080b3d6 __dt__14CPreviewDrawerFv
PreviewDrawer::~PreviewDrawer()
{
}

