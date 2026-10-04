#include "CBaseFrontendDrawer.h"

#include "Application/CGameStatus.h"
#include "Application/GameMain.h"
#include "Level/CLevelLoader.h"
#include "GameView/Display/CMain2DDisplay.h"
#include "Engine/Animation/CPlayThruAnim.h"
#include "Engine/Queues/CBaseQueue.h"
#include "Platform/Windows/Graphics/CChangeList.h"
#include "Engine/Text/CTextManager.h"
#include "Engine/Time/VsTime.h"
#include "Engine/Graphics/Primitives/CCopyToBackBuff.h"
#include "Platform/Windows/Input/CCursor.h"
#include "Engine/Graphics/Primitives/CGDI.h"
#include "Platform/Windows/Graphics/CSurface.h"
#include "Multiplayer/Transport/CBaseNetwork.h"
#include "Multiplayer/Transport/CConnect.h"
#include "Multiplayer/Transport/NetworkConstants.h"
#include "Multiplayer/Transport/NetworkMode.h"
#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/Types/CResBITMAP.h"
#include "Engine/Resources/Manifest.h"
#include "CBaseFrontendProcess.h"
#include "Frontend/Controls/CGunButtons.h"
#include "Frontend/Controls/CGunController.h"
#include "Frontend/Controls/CHiliteController.h"
#include "Engine/Animation/AnimationConstants.h"
#include "Engine/Math/RandomConstants.h"
extern "C" unsigned long __stdcall timeGetTime(void);
#include "Multiplayer/CNetworkManager.h"
#include "GameView/CSoundView.h"
#include "Engine/Streams/CVSOStream.h"

extern char g_szUnknownUserActionSpecified[];
extern char g_szUnknownUserActionReceived[];

#include "Application/FlowProcesses.h"
#include "CUserActionMessage.h"
#include "CoordPair.h"
#include "Application/SoundEffects.h"
#include "Engine/Animation/CAnimsManager.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"
#include "Engine/Queues/Message.h"
#include "tagPRIMS.h"
#include "Engine/Input/CBaseCursor.h"
#include "Engine/Graphics/Primitives/CBigBitmap.h"
#include "Engine/Graphics/Primitives/CPrimitive.h"
#include "Engine/Graphics/Primitives/CSolidRect.h"
#include "Engine/Resources/ResourceLimits.h"

#include <new.h>
#include <stddef.h>

enum {
	BASE_FRONTEND_AMBIENT_ANIMATION_DURATION_MS = 500
};

class CAnimFrameBASE;

// GLOBAL: LEMBALL 0x0049f144
CBaseFrontendDrawer* g_pBaseFrontendDrawer = NULL;

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

#include "Engine/Animation/CStaticAnim.h"

// FUNCTION: LEMBALL 0x00445420
CBaseFrontendDrawer::CBaseFrontendDrawer(CMain2DDisplay* p_display,
										 CGDI* p_gdi,
										 const CVSRect& p_rect,
										 eFlowProcesses p_flowProcess,
										 int p_resourceCapacity,
										 int p_animCapacity,
										 int p_zrleCapacity,
										 int p_textPrimitiveCapacity,
										 int p_maxStringLen)
	: CAnimsManager(p_gdi, RESOURCE_ID_COUNT, p_resourceCapacity + 3, p_animCapacity + 200, p_zrleCapacity, 0)
{
	m_flowProcess = p_flowProcess;
	m_display = p_display;
	m_gdi = p_gdi;
	m_size.m_width = p_rect.m_width;
	m_size.m_height = p_rect.m_height;
	m_textPrimitiveCapacity = p_textPrimitiveCapacity;
	m_maxStringLen = p_maxStringLen;
	m_framePrimitiveCount = 0;
	m_drawBackground = 1;
	m_drawFrame = 1;
	m_drawSolid = 0;
	m_activePalette = 0;
	m_loaded = 0;
	m_desiredPalette = RES_PALETTES_TITLEPALETTE;
	g_pMasterInputQueue->Attach(this, 0);
	m_returnState = FLOW_NONE;
	m_quitYet = 0;
	m_backBufferReady = 0;
	m_drawingBackBuffer = 0;
	m_ready = 1;
	if (g_pGameStatus->m_skill == SKILL_NETWORK && g_pActiveConnection != NULL) {
		m_networkMode = NETWORK_MODE_MULTIPLAYER;
	}
	else {
		m_networkMode = NETWORK_MODE_SINGLE_PLAYER;
	}
	m_startupPending = 1;
	m_actionPending = 0;
	m_gunController = NULL;
	m_hiliteController = NULL;
	m_ambientAnimId = 0;
	m_ambientAnim = NULL;
	m_textManager = NULL;
	m_createdAt = CurrentQueueTimer();
}

// FUNCTION: LEMBALL 0x004455f0
void CBaseFrontendDrawer::Setup()
{
	void* storage;

	if (m_drawBackground != 0) {
		CursorChangeType(CURSOR_DISPLAY_PAW, 0);
	}
	else {
		CursorChangeType(CURSOR_DISPLAY_NONE, 0);
	}

	if (m_textPrimitiveCapacity > 0) {
		storage = operator new(sizeof(CTextManager));
		if (storage == NULL) {
			m_textManager = NULL;
		}
		else {
			m_textManager = new (storage) CTextManager(RESOURCE_ID_COUNT, 1, m_textPrimitiveCapacity, m_maxStringLen);
		}
	}

	Restart();

	if (m_ambientAnimId != 0) {
		storage = operator new(0x1c);
		if (storage == NULL) {
			m_ambientAnim = NULL;
		}
		else {
			m_ambientAnim = new (storage) CPlayThruAnim(CAnimsManager::GetnAnims(m_ambientAnimId), 1);
		}
		m_ambientAnim->m_fixedTime = ANIMATION_TIME_REALTIME;
		m_ambientAnim->SetAnimTime(BASE_FRONTEND_AMBIENT_ANIMATION_DURATION_MS);
		unsigned long now = CurrentMilliTimer();
		m_ambientDelay = 0;
		m_ambientUpdatedAt = now;
	}

	g_pBaseFrontendDrawer = this;

	if (m_networkMode != NETWORK_MODE_SINGLE_PLAYER) {
		CNetworkManager* network;
		int desiredState;
		m_startupPending = 0;
		m_actionPending = 1;
		if (m_hiliteController != NULL) {
			m_hiliteController->ActivateButtons(0);
			m_hiliteController->m_active = 0;
		}
		switch (m_flowProcess) {
		case FLOW_PREVIEW:
			desiredState = 1;
			break;
		case FLOW_GAMEPLAY:
			desiredState = 3;
			break;
		case FLOW_SUCCESS:
		case FLOW_FAILURE:
			desiredState = 2;
			break;
		}
		if (desiredState != 0) {
			network = g_pNetworkManager;
			network->m_desiredGameState = desiredState;
			network->m_observedGameState = 0;
		}
	}
}

// FUNCTION: LEMBALL 0x00445790
CBaseFrontendDrawer::~CBaseFrontendDrawer()
{
	g_pBaseFrontendDrawer = NULL;
	if (m_networkMode != NETWORK_MODE_SINGLE_PLAYER && m_returnState == FLOW_MAIN_OPTIONS_1) {
		if (g_pNetworkManager != NULL) {
			g_pNetworkManager->Stop();
		}
		if (g_pBaseNetwork != NULL) {
			unsigned long start = CurrentMilliTimer();
			while (CurrentMilliTimer() - start < NETWORK_QUEUE_TRANSITION_TIMEOUT_MS &&
				   g_pBaseNetwork->m_queueTransitionPending != 0) {
			}
		}
		if (g_pNetworkManager != NULL) {
			delete g_pNetworkManager;
			g_pNetworkManager = NULL;
		}
	}
	if (m_ambientAnim != NULL) {
		delete m_ambientAnim;
	}
	g_pMasterInputQueue->Detach(this, 0);
	if (m_loaded != 0) {
		_UnLoad();
	}
	if (m_textManager != NULL) {
		delete m_textManager;
	}
	g_pMogRes->CleanUpResources();
}

// FUNCTION: LEMBALL 0x004458e0
void CBaseFrontendDrawer::InitialiseBackBuffer()
{
	unsigned int hiliteActive;
	unsigned int gunActive;

	m_backBufferNeeded = 0;
	m_drawingBackBuffer = 1;
	if (m_hiliteController != NULL && (hiliteActive = m_hiliteController->m_buttonsActive) != 0) {
		m_hiliteController->ActivateButtons(0);
	}
	if (m_gunController != NULL && (gunActive = m_gunController->m_buttonsActive) != 0) {
		m_gunController->ActivateButtons(0);
	}
	if (m_backBufferReady == 0) {
		m_backBufferReady = 1;
	}
	g_pCursor->SetActive(0);
	m_display->Render();
	CVSRect source(0, 0, m_size.m_width, m_size.m_height);
	const CVSSize* size = &source;
	const CVSPoint* origin = &source;
	CCopyToBackBuff* bitmap = &m_primitiveBundle[m_primitiveBank].m_bitmap;
	bitmap->m_x = 0;
	bitmap->m_y = 0;
	bitmap->m_destination.m_width = size->m_width;
	bitmap->m_destination.m_height = size->m_height;
	bitmap->m_destination.m_x = origin->m_x;
	bitmap->m_destination.m_y = origin->m_y;
	m_primitiveBundle[m_primitiveBank].m_bitmap.Draw(m_gdi);
	m_drawingBackBuffer = 0;
	if (m_hiliteController != NULL && hiliteActive != 0) {
		m_hiliteController->ActivateButtons(1);
	}
	if (m_gunController != NULL && gunActive != 0) {
		m_gunController->ActivateButtons(1);
	}
	g_pCursor->SetActive(1);
}

// FUNCTION: LEMBALL 0x00445a40
void CBaseFrontendDrawer::Draw(const CVSRect& p_rect)
{
	if (m_gdi != NULL) {
		m_gdi->m_renderTarget->GetCurrDB();
		m_primitiveBank = 0;
		if (m_gunController != NULL) {
			if (CGunButtons::DrawBackBuffer() == 0 && m_backBufferNeeded == 0) {
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

// FUNCTION: LEMBALL 0x00445ac0
void CBaseFrontendDrawer::ReplaceBackground()
{
	m_gdi->m_renderTarget->GetChangeList()->Reset();
	m_gdi->AddToList(&m_primitiveBundle[m_primitiveBank].m_drawingMark);
	if (m_drawingBackBuffer != 0) {
		if (m_drawFrame == 0) {
			CVSRect frame(0, 0, m_size.m_width, m_size.m_height);
			const CVSSize* size = &frame;
			const CVSPoint* origin = &frame;
			CSolidRect& line = m_primitiveBundle[m_primitiveBank].m_lines[m_framePrimitiveCount];
			line.m_bounds.m_width = size->m_width;
			line.m_bounds.m_height = size->m_height;
			line.m_bounds.m_x = origin->m_x;
			line.m_bounds.m_y = origin->m_y;
			line.m_colour = 0;
			CPrimitive* primitive = &m_primitiveBundle[m_primitiveBank].m_lines[m_framePrimitiveCount];
			primitive->Draw(m_gdi);
			m_framePrimitiveCount++;
		}
		_DrawBackGround();
		DrawBackGround();
	}
	if (m_drawingBackBuffer == 0) {
		_DrawAnims();
		DrawAnims();
	}
	DrawText();
	if (m_drawingBackBuffer == 0) {
		if (m_gunController != NULL) {
			m_gunController->DrawSpriteWindow();
		}
		if (m_hiliteController != NULL) {
			m_hiliteController->DrawHiliteWindow();
		}
	}
}

// FUNCTION: LEMBALL 0x00445c10
void CBaseFrontendDrawer::_DrawBackGround()
{
	CBigBitmap* rec;
	if (m_drawFrame != 0) {
		const CVSSize& tileSize = *(const CVSSize*) &m_tileBitmap->m_x;
		CVSRect tiles;
		tiles.m_width = m_size.m_width;
		tiles.m_height = m_size.m_height;
		CVSSize& count = tiles;
		CVSPoint& start = tiles;
		enum eBackgroundRowParity {
			BACKGROUND_ROW_PARITY_EVEN = 0,
			BACKGROUND_ROW_PARITY_TOGGLE_MASK = 1
		};
		short height = (short) (tiles.m_height + tileSize.m_height - 1) / tileSize.m_height;
		tiles.m_width = (short) (tiles.m_width + tileSize.m_width - 1) / tileSize.m_width;
		tiles.m_height = height;
		tiles.m_y /= tileSize.m_height;
		tiles.m_x /= tileSize.m_width;
		tiles.m_height -= tiles.m_y;
		tiles.m_width -= tiles.m_x;
		unsigned int oddRow = BACKGROUND_ROW_PARITY_EVEN;
		int recordIndex = 0;
		for (int row = start.m_y; (short) (count.m_height + start.m_y) > row; row++) {
			oddRow ^= BACKGROUND_ROW_PARITY_TOGGLE_MASK;
			for (int col = start.m_x; (int) ((short) (start.m_x + count.m_width) + oddRow) > col; col++) {
				CResBITMAP* bitmap = m_tileBitmap;
				rec = &m_primitiveBundle[m_primitiveBank].m_records[recordIndex];
				int y = tileSize.m_height * row;
				rec->m_x = col * tileSize.m_width - (tileSize.m_width / 2) * oddRow;
				rec->m_y = y;
				rec->m_resource = bitmap;
				rec->m_flags = 0;
				rec->m_remap = NULL;
				m_primitiveBundle[m_primitiveBank].m_records[recordIndex].Draw(m_gdi);
				recordIndex++;
			}
		}
	}
	if (m_drawSolid != 0) {
		m_primitiveBundle[m_primitiveBank].m_primitive.Draw(m_gdi);
	}
	if (m_gunController != NULL) {
		m_gunController->DrawButtons(1, 0);
	}
	if (m_hiliteController != NULL) {
		m_hiliteController->DrawButtons(1);
	}
	if (m_activePalette != m_desiredPalette) {
		m_display->AttachPalette(m_desiredPalette);
		m_activePalette = m_desiredPalette;
	}
}

// FUNCTION: LEMBALL 0x00445e70
void CBaseFrontendDrawer::Restart()
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

// FUNCTION: LEMBALL 0x00445ed0
void CBaseFrontendDrawer::_Load()
{
	m_loaded = 1;
	if (m_mode != FRONTEND_LAYOUT_STANDARD) {
		m_tileBitmap = CResBITMAP::Load(RES_NEWFRONT_BITMAPS_LORES_PAINTBALL_TILE);
		m_backgroundBitmap = CResBITMAP::Load(RES_NEWFRONT_BITMAPS_LORES_TITLE_BMP);
		m_sideFrameAnimId = RES_NEWFRONT_ANIMS_LORES_FRAME_2;
		m_chalkFontId = RES_NEWFRONT_FONTS_LORES_CHALK_FONT;
		m_topFrameAnimId = RES_NEWFRONT_ANIMS_LORES_FRAME_1;
		m_bottomFrameAnimId = RES_NEWFRONT_ANIMS_LORES_FRAME_3;
	}
	else {
		m_tileBitmap = CResBITMAP::Load(RES_NEWFRONT_BITMAPS_HIRES_PAINTBALL_TILE);
		m_backgroundBitmap = CResBITMAP::Load(RES_NEWFRONT_BITMAPS_HIRES_TITLE_BMP);
		m_sideFrameAnimId = RES_NEWFRONT_ANIMS_HIRES_FRAME_2;
		m_chalkFontId = RES_NEWFRONT_FONTS_HIRES_CHALK_FONT;
		m_topFrameAnimId = RES_NEWFRONT_ANIMS_HIRES_FRAME_1;
		m_bottomFrameAnimId = RES_NEWFRONT_ANIMS_HIRES_FRAME_3;
	}
	CAnimsManager::LoadAnims(m_topFrameAnimId);
	CAnimsManager::LoadAnims(m_sideFrameAnimId);
	CAnimsManager::LoadAnims(m_bottomFrameAnimId);
	if (m_textManager != NULL) {
		m_textManager->LoadFont(m_chalkFontId);
	}
}

// FUNCTION: LEMBALL 0x00445fe0
void CBaseFrontendDrawer::_UnLoad()
{
	if (m_textManager != NULL) {
		m_textManager->UnLoadFont(m_chalkFontId);
	}
	m_backgroundBitmap->UnLoad();
	m_tileBitmap->UnLoad();
	CAnimsManager::UnLoadAnims(m_topFrameAnimId);
	CAnimsManager::UnLoadAnims(m_sideFrameAnimId);
	CAnimsManager::UnLoadAnims(m_bottomFrameAnimId);
	m_loaded = 0;
}

// FUNCTION: LEMBALL 0x00446050
void CBaseFrontendDrawer::_DrawAnims()
{
	if (m_ambientAnim != NULL) {
		m_ambientAnim->m_fixedTime = timeGetTime();
		CAnimsManager::DrawAnim(m_animPosition, m_ambientAnimId, 0, (CAnimFrameBASE*) m_ambientAnim, NULL);
	}
}

// FUNCTION: LEMBALL 0x004460a0
void CBaseFrontendDrawer::ResetPrimitives()
{
	CAnimsManager::ResetPrimitives();
	if (m_textManager != NULL) {
		m_textManager->ResetPrimitives();
	}
	m_framePrimitiveCount = 0;
}

// FUNCTION: LEMBALL 0x004460d0
void CBaseFrontendDrawer::DrawFrame(CoordPair p_start, CoordPair p_end)
{
	DrawFrame(CVSRect(p_start.m_x, p_start.m_y, p_end.m_x, p_end.m_y));
}

// FUNCTION: LEMBALL 0x00446110
void CBaseFrontendDrawer::DrawFrame(CVSRect p_rect)
{
	int startX = p_rect.m_x;
	int startY = p_rect.m_y;
	int width = p_rect.m_width;
	int height = p_rect.m_height;
	int tileWidth;
	int tileHeight;
	{
		const CVSSize& tileSize = CAnimsManager::GetAnimSize(m_topFrameAnimId, 0);
		tileWidth = tileSize.m_width;
		tileHeight = tileSize.m_height;
	}
	width += tileWidth - 1;
	width -= width % tileWidth;
	height += tileHeight - 1;
	height -= height % tileHeight;
	{
		CVSRect frame(p_rect.m_x, p_rect.m_y, (short) width, (short) height);
		const CVSRect& frameBounds = frame;
		CSolidRect& line = m_primitiveBundle[m_primitiveBank].m_lines[m_framePrimitiveCount];
		line.m_bounds.m_width = frameBounds.m_width;
		line.m_bounds.m_height = frameBounds.m_height;
		line.m_bounds.m_x = frameBounds.m_x;
		line.m_bounds.m_y = frameBounds.m_y;
		line.m_colour = 0x10;
		m_primitiveBundle[m_primitiveBank].m_lines[m_framePrimitiveCount].Draw(m_gdi);
	}
	m_framePrimitiveCount++;
	m_staticAnim.m_frameState = 0;
	CAnimsManager::DrawAnim(CVSPoint((short) startX, (short) startY),
							m_topFrameAnimId,
							0,
							(CAnimFrameBASE*) &m_staticAnim,
							NULL);
	int x = tileWidth;
	int right = width - tileWidth;
	for (; right > x; x += tileWidth) {
		m_staticAnim.m_frameState = 1;
		CAnimsManager::DrawAnim(CVSPoint((short) (startX + x), (short) startY),
								m_topFrameAnimId,
								0,
								(CAnimFrameBASE*) &m_staticAnim,
								NULL);
	}
	m_staticAnim.m_frameState = 2;
	CAnimsManager::DrawAnim(CVSPoint((short) (startX + x), (short) startY),
							m_topFrameAnimId,
							0,
							(CAnimFrameBASE*) &m_staticAnim,
							NULL);
	int y = tileHeight;
	height -= tileHeight;
	for (; y < height; y += tileHeight) {
		m_staticAnim.m_frameState = 0;
		short currentY = (short) (startY + y);
		CVSPoint left((short) startX, currentY);
		CAnimsManager::DrawAnim(left, m_sideFrameAnimId, 0, (CAnimFrameBASE*) &m_staticAnim, NULL);
		m_staticAnim.m_frameState = 2;
		CAnimsManager::DrawAnim(CVSPoint((short) (width - tileWidth + startX), currentY),
								m_sideFrameAnimId,
								0,
								(CAnimFrameBASE*) &m_staticAnim,
								NULL);
	}
	m_staticAnim.m_frameState = 0;
	CAnimsManager::DrawAnim(CVSPoint((short) startX, (short) (startY + y)),
							m_bottomFrameAnimId,
							0,
							(CAnimFrameBASE*) &m_staticAnim,
							NULL);
	for (x = tileWidth; right > x; x += tileWidth) {
		m_staticAnim.m_frameState = 1;
		CAnimsManager::DrawAnim(CVSPoint((short) (startX + x), (short) (startY + y)),
								m_bottomFrameAnimId,
								0,
								(CAnimFrameBASE*) &m_staticAnim,
								NULL);
	}
	m_staticAnim.m_frameState = 2;
	CAnimsManager::DrawAnim(CVSPoint((short) (startX + x), (short) (startY + y)),
							m_bottomFrameAnimId,
							0,
							(CAnimFrameBASE*) &m_staticAnim,
							NULL);
}

// FUNCTION: LEMBALL 0x00446480
int CBaseFrontendDrawer::ProcessMsg(Message* p_message)
{
	unsigned int sequence;

	if (m_actionPending != 0) {
		return 0;
	}
	sequence = p_message->m_time;
	if ((int) (sequence - m_createdAt) < 0) {
		return 0;
	}
	if (ProcessMessages(p_message) == 0) {
		return 0;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x004464d0
void CBaseFrontendDrawer::Process()
{
	unsigned long now;
	int seed;
	if (m_networkMode != NETWORK_MODE_SINGLE_PLAYER && m_startupPending == 0 &&
		g_pNetworkManager->m_observedGameState == g_pNetworkManager->m_desiredGameState) {
		m_actionPending = 0;
		m_startupPending = 1;
		if (m_hiliteController != NULL) {
			m_hiliteController->ActivateButtons(1);
			m_hiliteController->m_active = 1;
		}
	}
	if (m_ambientAnim != NULL) {
		now = CurrentMilliTimer();
		if (m_ambientDelay + 1000U < now - m_ambientUpdatedAt) {
			now = CurrentMilliTimer();
			m_ambientUpdatedAt = now;
			m_ambientAnim->SetStartTime(now);
			seed = *g_pRandomSeed * RANDOM_SEED_MULTIPLIER + RANDOM_SEED_INCREMENT & RANDOM_SEED_MASK;
			*g_pRandomSeed = seed;
			m_ambientDelay = seed % 6000;
		}
	}
	if (m_gunController != NULL) {
		m_gunController->Process();
	}
	if (m_hiliteController != NULL) {
		m_hiliteController->Process();
	}
	Processing();
}

// FUNCTION: LEMBALL 0x004465c0
void CBaseFrontendDrawer::LostConnection()
{
	m_quitYet = 1;
	m_returnState = FLOW_MAIN_OPTIONS_1;
}

// FUNCTION: LEMBALL 0x004465e0
void CBaseFrontendDrawer::Action(eUserActions p_action, eUserActionStages p_stage)
{
	m_actionPending = 1;
	g_pCurrentFrontendProcess->Action(p_action, p_stage);
}

// FUNCTION: LEMBALL 0x00446610
void CBaseFrontendDrawer::RemoteAction(eUserActions p_action, eUserActionStages p_stage)
{
	int confirmed;

	switch (p_stage) {
	case USER_ACTION_STAGE_REQUEST:
		if (m_actionPending != 0) {
			Action(p_action, USER_ACTION_STAGE_REJECT);
			return;
		}
		m_actionPending = 1;
		Action(p_action, USER_ACTION_STAGE_CONFIRM);
		g_pSoundView->PlayEffect(SFX_DRUM1);
		confirmed = ConfirmedAction(p_action);
		if (confirmed == 0) {
			*g_pErrorOutput << g_szUnknownUserActionSpecified;
		}
		m_actionPending = 0;
		return;
	case USER_ACTION_STAGE_CONFIRM:
		confirmed = ConfirmedAction(p_action);
		if (confirmed == 0) {
			*g_pErrorOutput << g_szUnknownUserActionReceived;
		}
		m_actionPending = 0;
		return;
	case USER_ACTION_STAGE_REJECT:
		m_actionPending = 0;
	}
}

// FUNCTION: LEMBALL 0x004466e0
void CBaseFrontendDrawer::OnDriverChange()
{
	if (m_display->GetSizeStatus() != 0) {
		CMain2DDisplay* display = m_display;
		display->SetRect(display->GetUseRect(DISPLAY_COORDINATE_AUTO_CENTER, DISPLAY_COORDINATE_AUTO_CENTER));
	}
}

// FUNCTION: LEMBALL 0x00446f50
void CBaseFrontendDrawer::Processing()
{
}

// FUNCTION: LEMBALL 0x00446f60
bool CBaseFrontendDrawer::ProcessMessages(Message* p_message)
{
	return false;
}

// FUNCTION: LEMBALL 0x00446f70
void CBaseFrontendDrawer::DrawAnims()
{
}

// FUNCTION: LEMBALL 0x00446f80
void CBaseFrontendDrawer::DrawText()
{
}

// FUNCTION: LEMBALL 0x00446f90
void CBaseFrontendDrawer::DrawBackGround()
{
}

// FUNCTION: LEMBALL 0x00446fa0
bool CBaseFrontendDrawer::ConfirmedAction(eUserActions p_action)
{
	return false;
}

// FUNCTION: LEMBALL 0x00446fb0
int CBaseFrontendDrawer::GetReturnState()
{
	return m_returnState;
}

// FUNCTION: LEMBALL 0x00446fc0
bool CBaseFrontendDrawer::QuitYet()
{
	return m_quitYet;
}

// FUNCTION: LEMBALL 0x00446fd0
void CBaseFrontendDrawer::OnSize(const CVSRect& p_rect)
{
	m_size.m_width = p_rect.m_width;
	m_size.m_height = p_rect.m_height;
	Restart();
}
