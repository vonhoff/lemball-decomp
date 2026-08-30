#include "BaseFrontendDrawer.h"

#include "../Controls/GunButtons.h"

#include "../../Control/Game/GameMain.h"
#include "../../Control/Game/GameStatus.h"
#include "../../Views/Display/Main2DDisplay.h"
#include "../../Visos/Animation/PlayThruAnim.h"
#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/ChangeList.h"
#include "../../Visos/Foundation/TextManager.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/Bitmap.h"
#include "../../Visos/Graphics/Cursor.h"
#include "../../Visos/Network/BaseNetwork.h"
#include "../../Visos/Network/Connect.h"
#include "../../Visos/Resources/ResBitmap.h"
#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Resources/MogRes.h"
#include "../Controls/GunButtons.h"
#include "../Controls/GunController.h"
#include "../Controls/HiliteController.h"
#include "../Base/BaseFrontendProcess.h"
#include "../../Network/Game/NetworkManager.h"
#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Foundation/VsOStream.h"

extern int* g_pSentinel;
extern char g_szUnknownUserActionSpecified[];
extern char g_szUnknownUserActionReceived[];

#include <new.h>
#include <string.h>

// 68K 0x10800108 __ct__19CBaseFrontendDrawerFP14CMain2DDisplayP4CGDIRC7CVSRect14eFlowProcessesiiiii
// FUNCTION: LEMBALL 0x00445420
BaseFrontendDrawer::BaseFrontendDrawer(Main2DDisplay* p_arg0,
									   Gdi* p_arg1,
									   const VsRect& p_arg2,
									   eFlowProcesses p_arg3,
									   int p_arg4,
									   int p_arg5,
									   int p_arg6,
									   int p_arg7,
									   int p_arg8)
	: m_anims(p_arg1, 0x2b6, p_arg4 + 3, p_arg5 + 200, p_arg6, 0)
{
	m_height = 0;
	m_staticAnim.m_frames = 1;
	m_width = 0;
	m_staticAnim.m_frameState = 0;
	m_animPosition.m_y = 0;
	m_animPosition.m_x = 0;
	m_flowProcess = p_arg3;
	m_display = p_arg0;
	m_gdi = p_arg1;
	m_width = p_arg2.m_width;
	m_height = p_arg2.m_height;
	m_textCapacity = p_arg7;
	m_textStyle = p_arg8;
	m_framePrimitiveCount = 0;
	m_drawBackground = 1;
	m_drawFrame = 1;
	m_drawSolid = 0;
	m_activePalette = 0;
	m_loaded = 0;
	m_desiredPalette = 0x10a;
	if (g_pMasterInputQueue != 0) {
		g_pMasterInputQueue->Attach(this, 0);
	}
	m_returnState = 0;
	m_quitYet = 0;
	m_backBufferReady = 0;
	m_drawingBackBuffer = 0;
	m_ready = 1;
	if (g_pGameStatus != 0 && g_pGameStatus->m_skill == 4 && g_pActiveConnection != 0) {
		m_networkMode = 1;
	}
	else {
		m_networkMode = 0;
	}
	m_startupPending = 1;
	m_actionPending = 0;
	m_gunController = 0;
	m_hiliteController = 0;
	m_ambientAnimId = 0;
	m_ambientAnim = 0;
	m_textManager = 0;
	m_createdAt = CurrentQueueTimer();
}

// 68K 0x10800308 Setup__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x004455f0
void BaseFrontendDrawer::Setup()
{
	int cursorType;
	void* storage;

	if (m_drawBackground == 0) {
		cursorType = 0;
	}
	else {
		cursorType = 2;
	}
	CursorChangeType(cursorType, 0);

	if (m_textCapacity > 0) {
		storage = operator new(sizeof(TextManager));
		if (storage == 0) {
			m_textManager = 0;
		}
		else {
			m_textManager = new (storage) TextManager(0x2b6, 1, m_textCapacity, m_textStyle);
		}
	}

	Restart();

	if (m_ambientAnimId != 0) {
		storage = operator new(0x1c);
		if (storage == 0) {
			m_ambientAnim = 0;
		}
		else {
			m_ambientAnim = new (storage) PlayThruAnim(m_anims.GetnAnims(m_ambientAnimId), 1);
		}
		m_ambientAnim->m_fixedTime = 0xffffffff;
		m_ambientAnim->StartAnim(500);
		m_ambientDelay = 0;
		m_ambientUpdatedAt = CurrentMilliTimer();
	}

	g_pBaseFrontendDrawer = this;

	if (m_networkMode != 0) {
		int desiredState;
		m_startupPending = 0;
		m_actionPending = 1;
		if (m_hiliteController != 0) {
			m_hiliteController->ActivateButtons(0);
			m_hiliteController->m_active = 0;
		}
		switch (m_flowProcess) {
		case 4:
			desiredState = 1;
			break;
		case 5:
			desiredState = 3;
			break;
		case 0xe:
		case 0xf:
			desiredState = 2;
			break;
		}
		if (desiredState != 0) {
			g_pNetworkManager->m_desiredGameState = desiredState;
			g_pNetworkManager->m_observedGameState = 0;
		}
	}
}

// 68K 0x1080048c __dt__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x004457e0
BaseFrontendDrawer::~BaseFrontendDrawer()
{
	g_pBaseFrontendDrawer = 0;
	if (m_networkMode != 0 && m_returnState == 2) {
		if (g_pNetworkManager != 0) {
			g_pNetworkManager->Stop();
		}
		if (g_pBaseNetwork != 0) {
			unsigned long start = CurrentMilliTimer();
			while (g_pBaseNetwork->m_queueTransitionPending != 0) {
				if (1999 < CurrentMilliTimer() - start) {
					break;
				}
			}
		}
		if (g_pNetworkManager != 0) {
			delete g_pNetworkManager;
			g_pNetworkManager = 0;
		}
	}
	if (m_ambientAnim != 0) {
		delete m_ambientAnim;
		m_ambientAnim = 0;
	}
	if (g_pMasterInputQueue != 0) {
		g_pMasterInputQueue->Detach(this != 0 ? static_cast<BaseQueueHandler*>(this) : 0, 0);
	}
	if (m_loaded != 0) {
		_UnLoad();
	}
	if (m_textManager != 0) {
		delete m_textManager;
		m_textManager = 0;
	}
	if (g_pMogRes != 0) {
		g_pMogRes->CleanUpResources();
	}
}

// 68K 0x1080070e InitialiseBackBuffer__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x004458e0
void BaseFrontendDrawer::InitialiseBackBuffer()
{
	unsigned int hiliteActive;
	unsigned int gunActive;

	hiliteActive = 0;
	gunActive = 0;
	m_backBufferNeeded = 0;
	m_drawingBackBuffer = 1;
	if (m_hiliteController != 0 && m_hiliteController->m_buttonsActive != 0) {
		hiliteActive = m_hiliteController->m_buttonsActive;
		m_hiliteController->ActivateButtons(0);
	}
	if (m_gunController != 0 && m_gunController->m_buttonsActive != 0) {
		gunActive = m_gunController->m_buttonsActive;
		m_gunController->ActivateButtons(0);
	}
	m_backBufferReady = 1;
	g_pCursor->SetActive(0);
	m_display->Render();
	m_primitiveBundle.m_bitmap.m_x = 0;
	m_primitiveBundle.m_bitmap.m_y = 0;
	m_primitiveBundle.m_bitmap.m_width = m_width;
	m_primitiveBundle.m_bitmap.m_height = m_height;
	m_primitiveBundle.m_bitmap.m_sourceX = 0;
	m_primitiveBundle.m_bitmap.m_sourceY = 0;
	m_primitiveBundle.m_bitmap.Draw(m_gdi);
	m_drawingBackBuffer = 0;
	if (m_hiliteController != 0 && hiliteActive != 0) {
		m_hiliteController->ActivateButtons(1);
	}
	if (m_gunController != 0 && gunActive != 0) {
		m_gunController->ActivateButtons(1);
	}
	g_pCursor->SetActive(1);
}

// 68K 0x108008ba Draw__19CBaseFrontendDrawerFRC7CVSRect
// FUNCTION: LEMBALL 0x00445a40
void BaseFrontendDrawer::Draw(const VsRect& p_rect)
{
	if (m_gdi != 0) {
		m_gdi->m_renderTarget->GetCurrDb();
		m_primitiveBank = 0;
		if (m_gunController != 0) {
			if (((GunButtons*) this)->DrawBackBuffer() == 0 && m_backBufferNeeded == 0) {
				m_backBufferNeeded = 0;
			}
			else {
				m_backBufferNeeded = 1;
			}
		}
		if (m_backBufferNeeded != 0) {
			InitialiseBackBuffer();
		}
		ReplaceBackground();
		m_ready = 0;
	}
}

// 68K 0x10800958 ReplaceBackground__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x00445ac0
void BaseFrontendDrawer::ReplaceBackground()
{
	if (m_gdi != 0 && m_gdi->m_renderTarget != 0) {
		m_gdi->m_renderTarget->GetChangeList()->Reset();
		m_primitiveBundle.m_drawingMark.Draw(m_gdi);
		if (m_drawingBackBuffer != 0) {
			if (m_drawFrame == 0) {
				m_primitiveBundle.m_lines[m_framePrimitiveCount].m_x1 = m_width;
				m_primitiveBundle.m_lines[m_framePrimitiveCount].m_y1 = m_height;
				m_primitiveBundle.m_lines[m_framePrimitiveCount].m_x2 = 0;
				m_primitiveBundle.m_lines[m_framePrimitiveCount].m_y2 = 0;
				m_primitiveBundle.m_lines[m_framePrimitiveCount].m_color = 0;
				m_primitiveBundle.m_lines[m_framePrimitiveCount].Draw(m_gdi);
				m_framePrimitiveCount = m_framePrimitiveCount + 1;
			}
			_DrawBackGround();
			DrawBackGround();
		}
		if (m_drawingBackBuffer == 0) {
			_DrawAnims();
			DrawAnims();
		}
		DrawText();
		if (m_drawingBackBuffer == 0 && m_gunController != 0) {
			m_gunController->DrawSpriteWindow();
		}
		if (m_drawingBackBuffer == 0 && m_hiliteController != 0) {
			m_hiliteController->DrawHiliteWindow();
		}
	}
}

// 68K 0x10800ae8 _DrawBackGround__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x00445c10
void BaseFrontendDrawer::_DrawBackGround()
{
	if (m_drawFrame != 0) {
		short tileWidth = m_tileBitmap->m_x;
		short tileHeight = m_tileBitmap->m_y;
		short startRow = 0 / tileHeight;
		short rowCount = (short) (m_height + tileHeight - 1) / tileHeight - startRow;
		short startCol = 0 / tileWidth;
		short colCount = (short) (m_width + tileWidth - 1) / tileWidth - startCol;
		unsigned int oddRow = 0;
		int recordIndex = 0;
		for (short row = startRow; row < (short) (rowCount + startRow); row++) {
			oddRow ^= 1;
			for (short col = startCol; col < (short) (colCount + startCol + oddRow); col++) {
				BitmapRes& rec = m_primitiveBundle.m_records[recordIndex];
				rec.m_x = col * tileWidth - (tileWidth / 2) * oddRow;
				rec.m_y = tileHeight * row;
				rec.m_resource = m_tileBitmap;
				rec.m_flags = 0;
				rec.m_remap = 0;
				rec.Draw(m_gdi);
				recordIndex++;
			}
		}
	}
	if (m_drawSolid != 0) {
		m_primitiveBundle.m_primitive.Draw(m_gdi);
	}
	if (m_gunController != 0) {
		m_gunController->DrawButtons(1, 0);
	}
	if (m_hiliteController != 0) {
		m_hiliteController->DrawButtons(1);
	}
	if (m_activePalette != m_desiredPalette) {
		m_display->AttachPalette(m_desiredPalette);
		m_activePalette = m_desiredPalette;
	}
}

// 68K 0x10800db0 Restart__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x00445e70
void BaseFrontendDrawer::Restart()
{
	bool windowValid;

	windowValid = m_display->IsWindowValid();
	if (m_loaded != 0) {
		UnLoad();
		_UnLoad();
	}
	m_mode = g_nCompactPrimaryContextLayout;
	if (windowValid != 0) {
		_Load();
		Load();
		m_backBufferNeeded = 1;
	}
}

// 68K 0x10800e3a _Load__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x00445ed0
void BaseFrontendDrawer::_Load()
{
	m_loaded = 1;
	if (m_mode == 0) {
		m_tileBitmap = ResBitmap::Load(0x131);
		m_backgroundBitmap = ResBitmap::Load(0x132);
		m_sideFrameAnimId = 0x151;
		m_unknown384 = 0x113;
		m_topFrameAnimId = 0x150;
		m_bottomFrameAnimId = 0x152;
	}
	else {
		m_tileBitmap = ResBitmap::Load(0x13c);
		m_backgroundBitmap = ResBitmap::Load(0x13d);
		m_sideFrameAnimId = 0x17e;
		m_unknown384 = 0x114;
		m_topFrameAnimId = 0x17d;
		m_bottomFrameAnimId = 0x17f;
	}
	m_anims.LoadAnims(m_topFrameAnimId);
	m_anims.LoadAnims(m_sideFrameAnimId);
	m_anims.LoadAnims(m_bottomFrameAnimId);
	if (m_textManager != 0) {
		m_textManager->LoadFont(m_unknown384);
	}
}

// 68K 0x10800f3a _UnLoad__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x00445fe0
void BaseFrontendDrawer::_UnLoad()
{
	if (m_textManager != 0) {
		m_textManager->UnLoadFont(m_unknown384);
	}
	m_backgroundBitmap->UnLoad();
	m_tileBitmap->UnLoad();
	m_anims.UnLoadAnims(m_topFrameAnimId);
	m_anims.UnLoadAnims(m_sideFrameAnimId);
	m_anims.UnLoadAnims(m_bottomFrameAnimId);
	m_loaded = 0;
}

// 68K 0x10800fca _DrawAnims__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x00446050
void BaseFrontendDrawer::_DrawAnims()
{
	if (m_ambientAnim != 0) {
		m_ambientAnim->m_fixedTime = CurrentMilliTimer();
		m_anims.DrawAnim(m_animPosition, m_ambientAnimId, 0, (Frames*) m_ambientAnim, 0);
	}
}

// 68K 0x1080103c ResetPrimitives__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x004460a0
void BaseFrontendDrawer::ResetPrimitives()
{
	m_anims.ResetPrimitives();
	if (m_textManager != 0) {
		m_textManager->ResetPrimitives();
	}
	m_framePrimitiveCount = 0;
}

// 68K 0x1080109a DrawFrame__19CBaseFrontendDrawerF12tagCoordPair12tagCoordPair
// FUNCTION: LEMBALL 0x004460d0
void BaseFrontendDrawer::DrawFrame(CoordPair p_start, CoordPair p_end)
{
	VsRect rect(p_start.m_x, p_start.m_y, p_end.m_x - p_start.m_x, p_end.m_y - p_start.m_y);
	DrawFrame(rect);
}

// 68K 0x1080115a DrawFrame__19CBaseFrontendDrawerF7CVSRect
// FUNCTION: LEMBALL 0x00446110
void BaseFrontendDrawer::DrawFrame(VsRect p_rect)
{
	VsSize tileSize;
	VsPoint position;
	int tileWidth;
	int tileHeight;
	int x;
	int y;
	int right;
	int bottom;

	if (m_gdi == 0) {
		return;
	}
	tileSize = m_anims.GetAnimSize(m_topFrameAnimId, 0);
	tileWidth = tileSize.m_width;
	tileHeight = tileSize.m_height;
	if (tileWidth < 1 || tileHeight < 1) {
		return;
	}
	right = p_rect.m_x + p_rect.m_width;
	bottom = p_rect.m_y + p_rect.m_height;
	m_staticAnim.m_frameState = 0;
	position.m_x = p_rect.m_x;
	position.m_y = p_rect.m_y;
	m_anims.DrawAnim(position, m_topFrameAnimId, 0, (Frames*) &m_staticAnim, 0);
	x = p_rect.m_x + tileWidth;
	while (x + tileWidth < right) {
		m_staticAnim.m_frameState = 1;
		position.m_x = (short) x;
		position.m_y = p_rect.m_y;
		m_anims.DrawAnim(position, m_topFrameAnimId, 0, (Frames*) &m_staticAnim, 0);
		x = x + tileWidth;
	}
	m_staticAnim.m_frameState = 2;
	position.m_x = (short) (right - tileWidth);
	position.m_y = p_rect.m_y;
	m_anims.DrawAnim(position, m_topFrameAnimId, 0, (Frames*) &m_staticAnim, 0);
	y = p_rect.m_y + tileHeight;
	while (y + tileHeight < bottom) {
		m_staticAnim.m_frameState = 0;
		position.m_x = p_rect.m_x;
		position.m_y = (short) y;
		m_anims.DrawAnim(position, m_sideFrameAnimId, 0, (Frames*) &m_staticAnim, 0);
		m_staticAnim.m_frameState = 2;
		position.m_x = (short) (right - tileWidth);
		m_anims.DrawAnim(position, m_sideFrameAnimId, 0, (Frames*) &m_staticAnim, 0);
		y = y + tileHeight;
	}
	m_staticAnim.m_frameState = 0;
	position.m_x = p_rect.m_x;
	position.m_y = (short) (bottom - tileHeight);
	m_anims.DrawAnim(position, m_bottomFrameAnimId, 0, (Frames*) &m_staticAnim, 0);
	x = p_rect.m_x + tileWidth;
	while (x + tileWidth < right) {
		m_staticAnim.m_frameState = 1;
		position.m_x = (short) x;
		position.m_y = (short) (bottom - tileHeight);
		m_anims.DrawAnim(position, m_bottomFrameAnimId, 0, (Frames*) &m_staticAnim, 0);
		x = x + tileWidth;
	}
	m_staticAnim.m_frameState = 2;
	position.m_x = (short) (right - tileWidth);
	position.m_y = (short) (bottom - tileHeight);
	m_anims.DrawAnim(position, m_bottomFrameAnimId, 0, (Frames*) &m_staticAnim, 0);
}

// 68K 0x108014f4 ProcessMsg__19CBaseFrontendDrawerFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00446480
int BaseFrontendDrawer::ProcessMsg(Message* p_message)
{
	unsigned int sequence;

	if (m_actionPending != 0) {
		return 0;
	}
	sequence = p_message->time;
	if ((int) (sequence - m_createdAt) < 0) {
		return 0;
	}
	if (ProcessMessages(p_message) == 0) {
		return 0;
	}
	return 1;
}

// 68K 0x108015ca Process__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x004464d0
void BaseFrontendDrawer::Process()
{
	unsigned long now;
	unsigned int seed;

	if (m_backBufferNeeded != 0) {
		InitialiseBackBuffer();
	}
	if (m_networkMode != 0 && m_startupPending == 0) {
		m_actionPending = 0;
		m_startupPending = 1;
		if (m_hiliteController != 0) {
			m_hiliteController->ActivateButtons(1);
			m_hiliteController->m_active = 1;
		}
	}
	if (m_ambientAnim != 0) {
		now = CurrentMilliTimer();
		if (m_ambientDelay + 1000U < now - m_ambientUpdatedAt) {
			now = CurrentMilliTimer();
			m_ambientUpdatedAt = now;
			m_ambientAnim->SetStartTime(now);
			if (g_pSentinel != 0) {
				seed = *g_pSentinel * 0x29 + 0x1fU & 0x7fffff;
				*g_pSentinel = (int) seed;
				m_ambientDelay = seed % 6000;
			}
		}
	}
	if (m_gunController != 0) {
		m_gunController->Process();
	}
	if (m_hiliteController != 0) {
		m_hiliteController->Process();
	}
	Processing();
}

// 68K 0x108016f2 LostConnection__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x004465c0
void BaseFrontendDrawer::LostConnection()
{
	m_quitYet = 1;
	m_returnState = 2;
}

// 68K 0x10801736 Action__19CBaseFrontendDrawerF12eUserActions17eUserActionStages
// FUNCTION: LEMBALL 0x004465e0
void BaseFrontendDrawer::Action(int p_action, int p_stage)
{
	m_actionPending = 1;
	g_pCurrentFrontendProcess->Action(p_action, p_stage);
}

// 68K 0x108017a0 RemoteAction__19CBaseFrontendDrawerF12eUserActions17eUserActionStages
// FUNCTION: LEMBALL 0x00446610
void BaseFrontendDrawer::RemoteAction(int p_action, int p_stage)
{
	int confirmed;

	if (p_stage == 0) {
		if (m_actionPending != 0) {
			Action(p_action, 2);
			return;
		}
		m_actionPending = 1;
		Action(p_action, 1);
		g_pSoundView->PlayEffect((eSoundEffect) 0x25);
		confirmed = ConfirmedAction(p_action);
		if (confirmed == 0) {
			*g_pErrorOutput << g_szUnknownUserActionSpecified;
		}
		m_actionPending = 0;
		return;
	}
	if (p_stage != 1) {
		if (p_stage != 2) {
			return;
		}
		m_actionPending = 0;
		return;
	}
	confirmed = ConfirmedAction(p_action);
	if (confirmed == 0) {
		*g_pErrorOutput << g_szUnknownUserActionReceived;
	}
	m_actionPending = 0;
}

// 68K 0x108018b4 OnDriverChange__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x004466e0
void BaseFrontendDrawer::OnDriverChange()
{
	VsRect useRect;

	if (m_display->GetSizeStatus() != 0) {
		useRect = m_display->GetUseRect(-1, -1);
		m_display->SetRect(useRect);
	}
}

// 68K 0x1011beea Processing__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x00446f50
void BaseFrontendDrawer::Processing()
{
}

// 68K 0x1011bea8 ProcessMessages__19CBaseFrontendDrawerFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00446f60
bool BaseFrontendDrawer::ProcessMessages(Message* p_message)
{
	return 0;
}

// FUNCTION: LEMBALL 0x00446f70
void BaseFrontendDrawer::DrawAnims()
{
}

// 68K 0x1011bd5e DrawText__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x00446f80
void BaseFrontendDrawer::DrawText()
{
}

// FUNCTION: LEMBALL 0x00446f90
void BaseFrontendDrawer::DrawBackGround()
{
}

// 68K 0x1011bf1a ConfirmedAction__19CBaseFrontendDrawerF12eUserActions
// FUNCTION: LEMBALL 0x00446fa0
bool BaseFrontendDrawer::ConfirmedAction(int p_action)
{
	return 0;
}

// 68K 0x1011bfb8 GetReturnState__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x00446fb0
int BaseFrontendDrawer::GetReturnState()
{
	return m_returnState;
}

// 68K 0x1011bff4 QuitYet__19CBaseFrontendDrawerFv
// FUNCTION: LEMBALL 0x00446fc0
bool BaseFrontendDrawer::QuitYet()
{
	return m_quitYet;
}

// 68K 0x1011c028 OnSize__19CBaseFrontendDrawerFRC7CVSRect
// FUNCTION: LEMBALL 0x00446fd0
void BaseFrontendDrawer::OnSize(const VsRect& p_rect)
{
	m_width = p_rect.m_width;
	m_height = p_rect.m_height;
	Restart();
}

// GLOBAL: LEMBALL 0x0049f144
BaseFrontendDrawer* g_pBaseFrontendDrawer = 0;

// GLOBAL: LEMBALL 0x0049f148
char g_szUnknownUserActionSpecified[] = "Unknown user action specified\n";

// GLOBAL: LEMBALL 0x0049f168
char g_szUnknownUserActionReceived[] = "Unknown user action received\n";

// GLOBAL: LEMBALL 0x0049f628
int g_nPendingEffectsVolume = 100;

// GLOBAL: LEMBALL 0x0049f62c
int g_nPendingMusicVolume = 100;

// GLOBAL: LEMBALL 0x004a6278
int g_nMusicVolume = 0;

// GLOBAL: LEMBALL 0x004a627c
int g_nEffectsVolume = 0;

