#include "BaseFrontendDrawer.h"

// 68K 0x10800108 __ct__19CBaseFrontendDrawerFP14CMain2DDisplayP4CGDIRC7CVSRect14eFlowProcessesiiiii
// STUB: LEMBALL 0x00445420
BaseFrontendDrawer::BaseFrontendDrawer(Main2DDisplay* p_arg0,
									   Gdi* p_arg1,
									   const VsRect& p_arg2,
									   eFlowProcesses p_arg3,
									   int p_arg4,
									   int p_arg5,
									   int p_arg6,
									   int p_arg7,
									   int p_arg8)
{
}

// 68K 0x10800308 Setup__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x004455f0
void BaseFrontendDrawer::Setup()
{
}

// 68K 0x1080070e InitialiseBackBuffer__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x004458e0
void BaseFrontendDrawer::InitialiseBackBuffer()
{
}

// 68K 0x108008ba Draw__19CBaseFrontendDrawerFRC7CVSRect
// STUB: LEMBALL 0x00445a40
void BaseFrontendDrawer::Draw(const VsRect& p_rect)
{
}

// 68K 0x10800958 ReplaceBackground__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x00445ac0
void BaseFrontendDrawer::ReplaceBackground()
{
}

// 68K 0x10800ae8 _DrawBackGround__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x00445c10
void BaseFrontendDrawer::DrawBackGround()
{
}

// 68K 0x10800db0 Restart__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x00445e70
void BaseFrontendDrawer::Restart()
{
}

// 68K 0x10800e3a _Load__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x00445ed0
void BaseFrontendDrawer::Load()
{
}

// 68K 0x10800f3a _UnLoad__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x00445fe0
void BaseFrontendDrawer::UnLoad()
{
}

// 68K 0x10800fca _DrawAnims__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x00446050
void BaseFrontendDrawer::DrawAnims()
{
}

// 68K 0x1080103c ResetPrimitives__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x004460a0
void BaseFrontendDrawer::ResetPrimitives()
{
}

// 68K 0x1080109a DrawFrame__19CBaseFrontendDrawerF12tagCoordPair12tagCoordPair
// STUB: LEMBALL 0x004460d0
void BaseFrontendDrawer::DrawFrame(CoordPair p_start, CoordPair p_end)
{
}

// 68K 0x1080115a DrawFrame__19CBaseFrontendDrawerF7CVSRect
// STUB: LEMBALL 0x00446110
void BaseFrontendDrawer::DrawFrame(VsRect p_rect)
{
}

// 68K 0x108014f4 ProcessMsg__19CBaseFrontendDrawerFP10tagMESSAGE
// STUB: LEMBALL 0x00446480
bool BaseFrontendDrawer::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x108015ca Process__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x004464d0
void BaseFrontendDrawer::Process()
{
}

// 68K 0x108016f2 LostConnection__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x004465c0
void BaseFrontendDrawer::LostConnection()
{
}

// 68K 0x10801736 Action__19CBaseFrontendDrawerF12eUserActions17eUserActionStages
// STUB: LEMBALL 0x004465e0
void BaseFrontendDrawer::Action(int p_action, int p_stage)
{
}

// 68K 0x108017a0 RemoteAction__19CBaseFrontendDrawerF12eUserActions17eUserActionStages
// STUB: LEMBALL 0x00446610
void BaseFrontendDrawer::RemoteAction(int p_action, int p_stage)
{
}

// 68K 0x108018b4 OnDriverChange__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x004466e0
void BaseFrontendDrawer::OnDriverChange()
{
}

// 68K 0x1011beea Processing__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x00446f50
void BaseFrontendDrawer::Processing()
{
}

// 68K 0x1011bea8 ProcessMessages__19CBaseFrontendDrawerFP10tagMESSAGE
// STUB: LEMBALL 0x00446f60
bool BaseFrontendDrawer::ProcessMessages(Message* p_message)
{
	return 0;
}

// 68K 0x1011bd5e DrawText__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x00446f80
void BaseFrontendDrawer::DrawText()
{
}

// 68K 0x1011bf1a ConfirmedAction__19CBaseFrontendDrawerF12eUserActions
// STUB: LEMBALL 0x00446fa0
bool BaseFrontendDrawer::ConfirmedAction(int p_action)
{
	return 0;
}

// 68K 0x1011bfb8 GetReturnState__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x00446fb0
int BaseFrontendDrawer::GetReturnState()
{
	return 0;
}

// 68K 0x1011bff4 QuitYet__19CBaseFrontendDrawerFv
// STUB: LEMBALL 0x00446fc0
bool BaseFrontendDrawer::QuitYet()
{
	return 0;
}

// 68K 0x1011c028 OnSize__19CBaseFrontendDrawerFRC7CVSRect
// STUB: LEMBALL 0x00446fd0
void BaseFrontendDrawer::OnSize(const VsRect& p_rect)
{
}

// 68K 0x1080048c __dt__19CBaseFrontendDrawerFv
// SYNTHETIC: LEMBALL 0x00447000
// BaseFrontendDrawer::`scalar deleting destructor'
BaseFrontendDrawer::~BaseFrontendDrawer()
{
}

// GLOBAL: LEMBALL 0x0049f628
int g_nPendingEffectsVolume = 100;

// GLOBAL: LEMBALL 0x0049f62c
int g_nPendingMusicVolume = 100;

// GLOBAL: LEMBALL 0x004a6278
int g_nMusicVolume = 0;

// GLOBAL: LEMBALL 0x004a627c
int g_nEffectsVolume = 0;
