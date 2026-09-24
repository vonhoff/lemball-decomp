#include "CBaseFrontendDrawer.h"

#include "../../Control/Game/CGameStatus.h"
#include "../../Control/Game/GameMain.h"
#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Visos/Animation/CPlayThruAnim.h"
#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/CChangeList.h"
#include "../../Visos/Foundation/CTextManager.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/CBitmap.h"
#include "../../Visos/Graphics/CCursor.h"
#include "../../Visos/Graphics/CGDI.h"
#include "../../Visos/Graphics/CSurface.h"
#include "../../Visos/Network/CBaseNetwork.h"
#include "../../Visos/Network/CConnect.h"
#include "../../Visos/Resources/CMogRes.h"
#include "../../Visos/Resources/CResBITMAP.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Base/CBaseFrontendProcess.h"
#include "../Controls/CGunButtons.h"
#include "../Controls/CGunController.h"
#include "../Controls/CHiliteController.h"

extern "C" unsigned long __stdcall timeGetTime(void);
#include "../../Network/Game/CNetworkManager.h"
#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Foundation/CVSOStream.h"

extern int* g_pSentinel;
extern char g_szUnknownUserActionSpecified[];
extern char g_szUnknownUserActionReceived[];

#include "Frontend/Base/FlowProcesses.h"
#include "Frontend/Support/CUserActionMessage.h"
#include "Frontend/Support/CoordPair.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Animation/CAnimsManager.h"
#include "Visos/Animation/CStaticAnim.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Foundation/tagPRIMS.h"
#include "Visos/Graphics/CBaseCursor.h"
#include "Visos/Graphics/CBitmapRes.h"
#include "Visos/Graphics/CDrawingMark.h"
#include "Visos/Graphics/CLine.h"
#include "Visos/Graphics/CPrimitive.h"

#include <new.h>
#include <string.h>

class CFrames;

// FUNCTION: LEMBALL 0x00445420
CBaseFrontendDrawer::CBaseFrontendDrawer(CMain2DDisplay* p_display,
										 CGDI* p_gdi,
										 const CVsRect& p_rect,
										 eFlowProcesses p_flowProcess,
										 int p_resourceCapacity,
										 int p_animCapacity,
										 int p_zrleCapacity,
										 int p_textPrimitiveCapacity,
										 int p_maxStringLen)
	: CAnimsManager(p_gdi, 0x2b6, p_resourceCapacity + 3, p_animCapacity + 200, p_zrleCapacity, 0)
{
	m_height = 0;
	m_width = 0;
	m_staticAnim.m_frameState = 0;
	m_animPosition.m_y = 0;
	m_animPosition.m_x = 0;
	m_flowProcess = p_flowProcess;
	m_display = p_display;
	m_gdi = p_gdi;
	m_width = p_rect.m_width;
	m_height = p_rect.m_height;
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
	m_returnState = 0;
	m_quitYet = 0;
	m_backBufferReady = 0;
	m_drawingBackBuffer = 0;
	m_ready = 1;
	if (g_pGameStatus->m_skill == 4 && g_pActiveConnection != 0) {
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
		if (storage == 0) {
			m_textManager = 0;
		}
		else {
			m_textManager = new (storage) CTextManager(0x2b6, 1, m_textPrimitiveCapacity, m_maxStringLen);
		}
	}

	Restart();

	if (m_ambientAnimId != 0) {
		storage = operator new(0x1c);
		if (storage == 0) {
			m_ambientAnim = 0;
		}
		else {
			m_ambientAnim = new (storage) CPlayThruAnim(CAnimsManager::GetnAnims(m_ambientAnimId), 1);
		}
		m_ambientAnim->m_fixedTime = 0xffffffff;
		m_ambientAnim->SetAnimTime(500);
		unsigned long now = CurrentMilliTimer();
		m_ambientDelay = 0;
		m_ambientUpdatedAt = now;
	}

	g_pBaseFrontendDrawer = this;

	if (m_networkMode != 0) {
		CNetworkManager* network;
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
			network = g_pNetworkManager;
			network->m_desiredGameState = desiredState;
			network->m_observedGameState = 0;
		}
	}
}

// FUNCTION: LEMBALL 0x00445790
CBaseFrontendDrawer::~CBaseFrontendDrawer()
{
	g_pBaseFrontendDrawer = 0;
	if (m_networkMode != 0 && m_returnState == 2) {
		if (g_pNetworkManager != 0) {
			g_pNetworkManager->Stop();
		}
		if (g_pBaseNetwork != 0) {
			unsigned long start = CurrentMilliTimer();
			while (CurrentMilliTimer() - start < 2000 && g_pBaseNetwork->m_queueTransitionPending != 0) {
			}
		}
		if (g_pNetworkManager != 0) {
			delete g_pNetworkManager;
			g_pNetworkManager = 0;
		}
	}
	if (m_ambientAnim != 0) {
		delete m_ambientAnim;
	}
	g_pMasterInputQueue->Detach(this, 0);
	if (m_loaded != 0) {
		_UnLoad();
	}
	if (m_textManager != 0) {
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
	if (m_hiliteController != 0 && (hiliteActive = m_hiliteController->m_buttonsActive) != 0) {
		m_hiliteController->ActivateButtons(0);
	}
	if (m_gunController != 0 && (gunActive = m_gunController->m_buttonsActive) != 0) {
		m_gunController->ActivateButtons(0);
	}
	if (m_backBufferReady == 0) {
		m_backBufferReady = 1;
	}
	g_pCursor->SetActive(0);
	m_display->Render();
	CVsRect source(0, 0, m_width, m_height);
	const CVsSize* size = &source;
	const CVsPoint* origin = &source;
	CBitmap* bitmap = &m_primitiveBundle[m_primitiveBank].m_bitmap;
	bitmap->m_x = 0;
	bitmap->m_y = 0;
	bitmap->m_width = size->m_width;
	bitmap->m_height = size->m_height;
	bitmap->m_sourceX = origin->m_x;
	bitmap->m_sourceY = origin->m_y;
	m_primitiveBundle[m_primitiveBank].m_bitmap.Draw(m_gdi);
	m_drawingBackBuffer = 0;
	if (m_hiliteController != 0 && hiliteActive != 0) {
		m_hiliteController->ActivateButtons(1);
	}
	if (m_gunController != 0 && gunActive != 0) {
		m_gunController->ActivateButtons(1);
	}
	g_pCursor->SetActive(1);
}

// FUNCTION: LEMBALL 0x00445a40
void CBaseFrontendDrawer::Draw(const CVsRect& p_rect)
{
	if (m_gdi != 0) {
		m_gdi->m_renderTarget->GetCurrDB();
		m_primitiveBank = 0;
		if (m_gunController != 0) {
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
			CVsRect frame(0, 0, m_width, m_height);
			const CVsSize* size = &frame;
			const CVsPoint* origin = &frame;
			CLine& line = m_primitiveBundle[m_primitiveBank].m_lines[m_framePrimitiveCount];
			line.m_bounds.m_width = size->m_width;
			line.m_bounds.m_height = size->m_height;
			line.m_bounds.m_x = origin->m_x;
			line.m_bounds.m_y = origin->m_y;
			line.m_color = 0;
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
		if (m_gunController != 0) {
			m_gunController->DrawSpriteWindow();
		}
		if (m_hiliteController != 0) {
			m_hiliteController->DrawHiliteWindow();
		}
	}
}
// FUNCTION: LEMBALL 0x00445c10
void CBaseFrontendDrawer::_DrawBackGround()
{
	if (m_drawFrame != 0) {
		CVsRect tiles(0, 0, m_width, m_height);
		CVsSize& count = tiles;
		CVsPoint& start = tiles;
		const CVsSize& tileSize = *(const CVsSize*) &m_tileBitmap->m_x;
		tiles.m_height = (short) (tiles.m_height + tileSize.m_height - 1) / tileSize.m_height;
		tiles.m_width = (short) (tiles.m_width + tileSize.m_width - 1) / tileSize.m_width;
		tiles.m_y /= tileSize.m_height;
		tiles.m_x /= tileSize.m_width;
		tiles.m_height -= tiles.m_y;
		tiles.m_width -= tiles.m_x;
		unsigned int oddRow = 0;
		int recordIndex = 0;
		for (int row = start.m_y; (short) (count.m_height + start.m_y) > row; row++) {
			oddRow ^= 1;
			for (int col = start.m_x; (int) ((short) (start.m_x + count.m_width) + oddRow) > col; col++) {
				CResBITMAP* bitmap = m_tileBitmap;
				int y = tileSize.m_height * row;
				CBitmapRes& rec = m_primitiveBundle[m_primitiveBank].m_records[recordIndex];
				rec.m_x = col * tileSize.m_width - (tileSize.m_width / 2) * oddRow;
				rec.m_y = y;
				rec.m_resource = bitmap;
				rec.m_flags = 0;
				rec.m_remap = 0;
				m_primitiveBundle[m_primitiveBank].m_records[recordIndex].Draw(m_gdi);
				recordIndex++;
			}
		}
	}
	if (m_drawSolid != 0) {
		m_primitiveBundle[m_primitiveBank].m_primitive.Draw(m_gdi);
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
	if (m_mode != 0) {
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
	if (m_textManager != 0) {
		m_textManager->LoadFont(m_chalkFontId);
	}
}

// FUNCTION: LEMBALL 0x00445fe0
void CBaseFrontendDrawer::_UnLoad()
{
	if (m_textManager != 0) {
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
	if (m_ambientAnim != 0) {
		m_ambientAnim->m_fixedTime = timeGetTime();
		CAnimsManager::DrawAnim(m_animPosition, m_ambientAnimId, 0, (CFrames*) m_ambientAnim, 0);
	}
}

// FUNCTION: LEMBALL 0x004460a0
void CBaseFrontendDrawer::ResetPrimitives()
{
	CAnimsManager::ResetPrimitives();
	if (m_textManager != 0) {
		m_textManager->ResetPrimitives();
	}
	m_framePrimitiveCount = 0;
}

// FUNCTION: LEMBALL 0x004460d0
void CBaseFrontendDrawer::DrawFrame(CoordPair p_start, CoordPair p_end)
{
	DrawFrame(CVsRect(p_start.m_x, p_start.m_y, p_end.m_x, p_end.m_y));
}

// FUNCTION: LEMBALL 0x00446110
void CBaseFrontendDrawer::DrawFrame(CVsRect p_rect)
{
	int startX = p_rect.m_x;
	int startY = p_rect.m_y;
	int width = p_rect.m_width;
	int height = p_rect.m_height;
	int tileWidth;
	int tileHeight;
	{
		const CVsSize& tileSize = CAnimsManager::GetAnimSize(m_topFrameAnimId, 0);
		tileWidth = tileSize.m_width;
		tileHeight = tileSize.m_height;
	}
	width += tileWidth - 1;
	width -= width % tileWidth;
	height += tileHeight - 1;
	height -= height % tileHeight;
	CVsRect frame(p_rect.m_x, p_rect.m_y, (short) width, (short) height);
	CLine& line = m_primitiveBundle[m_primitiveBank].m_lines[m_framePrimitiveCount];
	line.m_bounds.m_width = frame.m_width;
	line.m_bounds.m_height = frame.m_height;
	line.m_bounds.m_x = frame.m_x;
	line.m_bounds.m_y = frame.m_y;
	line.m_color = 0x10;
	m_primitiveBundle[m_primitiveBank].m_lines[m_framePrimitiveCount].Draw(m_gdi);
	m_framePrimitiveCount++;
	m_staticAnim.m_frameState = 0;
	CAnimsManager::DrawAnim(CVsPoint((short) startX, (short) startY), m_topFrameAnimId, 0, (CFrames*) &m_staticAnim, 0);
	int x = tileWidth;
	int right = width - tileWidth;
	for (; right > x; x += tileWidth) {
		m_staticAnim.m_frameState = 1;
		CAnimsManager::DrawAnim(CVsPoint((short) (startX + x), (short) startY),
								m_topFrameAnimId,
								0,
								(CFrames*) &m_staticAnim,
								0);
	}
	m_staticAnim.m_frameState = 2;
	CAnimsManager::DrawAnim(CVsPoint((short) (startX + x), (short) startY),
							m_topFrameAnimId,
							0,
							(CFrames*) &m_staticAnim,
							0);
	int y = tileHeight;
	height -= tileHeight;
	for (; y < height; y += tileHeight) {
		m_staticAnim.m_frameState = 0;
		short currentY = (short) (startY + y);
		CVsPoint left((short) startX, currentY);
		CAnimsManager::DrawAnim(left, m_sideFrameAnimId, 0, (CFrames*) &m_staticAnim, 0);
		m_staticAnim.m_frameState = 2;
		CAnimsManager::DrawAnim(CVsPoint((short) (width - tileWidth + startX), currentY),
								m_sideFrameAnimId,
								0,
								(CFrames*) &m_staticAnim,
								0);
	}
	m_staticAnim.m_frameState = 0;
	CAnimsManager::DrawAnim(CVsPoint((short) startX, (short) (startY + y)),
							m_bottomFrameAnimId,
							0,
							(CFrames*) &m_staticAnim,
							0);
	for (x = tileWidth; right > x; x += tileWidth) {
		m_staticAnim.m_frameState = 1;
		CAnimsManager::DrawAnim(CVsPoint((short) (startX + x), (short) (startY + y)),
								m_bottomFrameAnimId,
								0,
								(CFrames*) &m_staticAnim,
								0);
	}
	m_staticAnim.m_frameState = 2;
	CAnimsManager::DrawAnim(CVsPoint((short) (startX + x), (short) (startY + y)),
							m_bottomFrameAnimId,
							0,
							(CFrames*) &m_staticAnim,
							0);
}

// FUNCTION: LEMBALL 0x00446480
int CBaseFrontendDrawer::ProcessMsg(Message* p_message)
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

// FUNCTION: LEMBALL 0x004464d0
void CBaseFrontendDrawer::Process()
{
	unsigned long now;
	int seed;
	if (m_networkMode != 0 && m_startupPending == 0 &&
		g_pNetworkManager->m_observedGameState == g_pNetworkManager->m_desiredGameState) {
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
			seed = *g_pSentinel * 0x29 + 0x1f & 0x7fffff;
			*g_pSentinel = seed;
			m_ambientDelay = seed % 6000;
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

// FUNCTION: LEMBALL 0x004465c0
void CBaseFrontendDrawer::LostConnection()
{
	m_quitYet = 1;
	m_returnState = 2;
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
		display->SetRect(display->GetUseRect(-1, -1));
	}
}

// FUNCTION: LEMBALL 0x00446f50
void CBaseFrontendDrawer::Processing()
{
}

// FUNCTION: LEMBALL 0x00446f60
bool CBaseFrontendDrawer::ProcessMessages(Message* p_message)
{
	return 0;
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
	return 0;
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
void CBaseFrontendDrawer::OnSize(const CVsRect& p_rect)
{
	m_width = p_rect.m_width;
	m_height = p_rect.m_height;
	Restart();
}

// GLOBAL: LEMBALL 0x0049f144
CBaseFrontendDrawer* g_pBaseFrontendDrawer = 0;

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
