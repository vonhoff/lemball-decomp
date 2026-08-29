#include "SuccFailDrawer.h"

#include "../../Control/Game/GameStatus.h"
#include "../Base/BaseFrontendProcess.h"
#include "../../Visos/Foundation/VsTime.h"

extern char g_szMoviePrefix[];

// 68K 0x10809058 __ct__15CSuccFailDrawerFP14CMain2DDisplayP4CGDIRC7CVSRectUc
// FUNCTION: LEMBALL 0x00450020
SuccFailDrawer::SuccFailDrawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2, unsigned char p_arg3)
	: BaseFrontendDrawer(p_arg0, p_arg1, p_arg2, (eFlowProcesses) 0xe, 0x32, 200, 0, 0x28, 0x30)
{
	m_firstLinePos.m_y = 0;
	m_firstLinePos.m_x = 0;
	m_secondLinePos.m_y = 0;
	m_secondLinePos.m_x = 0;
	m_passwordLabelPos.m_y = 0;
	m_passwordLabelPos.m_x = 0;
	m_passwordPos.m_y = 0;
	m_passwordPos.m_x = 0;
	m_variant = p_arg3;
	m_animationsEnabled = (unsigned int) (g_nAnimationsDisabled == 0);
	m_animWindow.Initialise(this, m_display, p_arg3);
	m_animWindow.m_resolveMoviePath = 1;
	m_animWindow.m_moviePrefix = g_szMoviePrefix;
	m_animWindow.m_useMoviePrefix = 1;
	m_animStarted = 0;
	m_soundStarted = 0;
	m_soundStopped = 0;
	m_drawBackground = 1;
	m_animStartDeadline = CurrentMilliTimer() + 0x28;
	m_drawFrame = 1;
	m_drawSolid = 1;
	m_password = g_pGameStatus->EncodePassword();
	Setup();
}

// 68K 0x1080919a CalculateText__15CSuccFailDrawerFv
// STUB: LEMBALL 0x00450160
void SuccFailDrawer::CalculateText()
{
}

// 68K 0x10809508 Load__15CSuccFailDrawerFv
// STUB: LEMBALL 0x00450460
void SuccFailDrawer::Load()
{
}

// 68K 0x10809888 UnLoad__15CSuccFailDrawerFv
// STUB: LEMBALL 0x00450770
void SuccFailDrawer::UnLoad()
{
}

// 68K 0x1080999a DestroyDrawer__15CSuccFailDrawerFv
// STUB: LEMBALL 0x00450820
void SuccFailDrawer::DestroyDrawer()
{
}

// 68K 0x108099c8 DrawText__15CSuccFailDrawerFv
// STUB: LEMBALL 0x00450860
void SuccFailDrawer::DrawText()
{
}

// 68K 0x10809ac4 ProcessMessages__15CSuccFailDrawerFP10tagMESSAGE
// STUB: LEMBALL 0x00450970
bool SuccFailDrawer::ProcessMessages(Message* p_message)
{
	return 0;
}

// 68K 0x10809b8c Return__15CSuccFailDrawerFv
// FUNCTION: LEMBALL 0x004509f0
void SuccFailDrawer::Return()
{
	m_quitYet = 1;
	m_returnState = 2;
}

// 68K 0x10809bc2 Go__15CSuccFailDrawerFv
// STUB: LEMBALL 0x00450a10
void SuccFailDrawer::Go()
{
}

// 68K 0x10809c48 ConfirmedAction__15CSuccFailDrawerF12eUserActions
// FUNCTION: LEMBALL 0x00450a60
bool SuccFailDrawer::ConfirmedAction(int p_action)
{
	switch (p_action) {
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

// 68K 0x10809cb4 Processing__15CSuccFailDrawerFv
// STUB: LEMBALL 0x00450a90
void SuccFailDrawer::Processing()
{
}

// 68K 0x10809d96 DrawBackGround__15CSuccFailDrawerFv
// STUB: LEMBALL 0x00450ba0
void SuccFailDrawer::DrawBackGround()
{
}

// 68K 0x108098f2 __dt__15CSuccFailDrawerFv
SuccFailDrawer::~SuccFailDrawer()
{
}

