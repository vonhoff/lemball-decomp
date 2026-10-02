#include "CPreviewDrawer.h"

#include "../../Control/Game/CGameStatus.h"
#include "../../Control/Level/CLevelLoader.h"
#include "../../Control/Support/tPreviewData.h"
#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Visos/Animation/CAnimsManager.h"
#include "../../Visos/Animation/CRepeatAnim.h"
#include "../../Visos/Foundation/CTextManager.h"
#include "../../Visos/Graphics/CBasePalManager.h"
#include "../../Visos/Resources/CResBITMAP.h"
#include "../../Visos/Resources/CResFONT.h"
#include "../../Visos/Resources/CResPALETTE.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Base/CBaseFrontendProcess.h"
#include "../Controls/CHiliteController.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Base/FlowProcesses.h"
#include "Frontend/Support/CoordPair.h"
#include "Visos/Foundation/CVSPoint.h"
#include "Visos/Foundation/CVSSize.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Foundation/tagPRIMS.h"
#include "Visos/Graphics/CBaseRemap.h"
#include "Visos/Graphics/CBigBitmap.h"

#include <new.h>
#include <string.h>

class CGWnd;
class CRemap;

#pragma intrinsic(strcpy, strlen)

#define PREVIEW_BUTTON_MESSAGE_GO 0xacef000c
#define PREVIEW_BUTTON_MESSAGE_RETURN 0xacef000d
#define PREVIEW_BUTTON_MESSAGE_NEXT_LEVEL 0xacef000e
#define PREVIEW_BUTTON_MESSAGE_PREVIOUS_LEVEL 0xacef000f

// GLOBAL: LEMBALL 0x0049f678
unsigned char g_abPreviewLayoutFull[0xd8] = {
	0x19, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0xb2, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0x4b, 0x01,
	0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0xe4, 0x01, 0x00, 0x00, 0x77, 0x01, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x24, 0x01,
	0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x8a, 0x00, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00,
	0x60, 0x01, 0x00, 0x00, 0x0a, 0x01, 0x00, 0x00, 0x94, 0x01, 0x00, 0x00, 0x0b, 0x01, 0x00, 0x00, 0xb8, 0x01,
	0x00, 0x00, 0x0b, 0x01, 0x00, 0x00, 0x60, 0x01, 0x00, 0x00, 0xcb, 0x00, 0x00, 0x00, 0x94, 0x01, 0x00, 0x00,
	0xdb, 0x00, 0x00, 0x00, 0xb8, 0x01, 0x00, 0x00, 0xcb, 0x00, 0x00, 0x00, 0x60, 0x01, 0x00, 0x00, 0x99, 0x00,
	0x00, 0x00, 0x94, 0x01, 0x00, 0x00, 0x9e, 0x00, 0x00, 0x00, 0xb8, 0x01, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00,
	0x60, 0x01, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0xe8, 0x01,
	0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x60, 0x01, 0x00, 0x00, 0x31, 0x01, 0x00, 0x00, 0x94, 0x01, 0x00, 0x00,
	0x36, 0x01, 0x00, 0x00, 0xb8, 0x01, 0x00, 0x00, 0x31, 0x01, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x60, 0x01,
	0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdd, 0x00, 0x00, 0x00,
};

// GLOBAL: LEMBALL 0x0049f750
unsigned char g_abPreviewLayoutCompact[0xd8] = {
	0x0c, 0x00, 0x00, 0x00, 0xba, 0x00, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00, 0xba, 0x00, 0x00, 0x00, 0xa5, 0x00,
	0x00, 0x00, 0xba, 0x00, 0x00, 0x00, 0xf2, 0x00, 0x00, 0x00, 0xba, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0xa0, 0x00,
	0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x45, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00,
	0xb0, 0x00, 0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0xca, 0x00, 0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0xdc, 0x00,
	0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0xca, 0x00, 0x00, 0x00,
	0x70, 0x00, 0x00, 0x00, 0xdc, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x4d, 0x00,
	0x00, 0x00, 0xca, 0x00, 0x00, 0x00, 0x51, 0x00, 0x00, 0x00, 0xdc, 0x00, 0x00, 0x00, 0x4d, 0x00, 0x00, 0x00,
	0xb0, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0xf4, 0x00,
	0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00, 0xca, 0x00, 0x00, 0x00,
	0x9d, 0x00, 0x00, 0x00, 0xdc, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0xb0, 0x00,
	0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71, 0x00, 0x00, 0x00,
};

// GLOBAL: LEMBALL 0x0049f828
unsigned long g_dwPreviewGoAnimIdsFull = RES_NEWFRONT_ICONS_HIRES_RETURN;

// GLOBAL: LEMBALL 0x0049f82c
unsigned long g_dwPreviewReturnAnimIdsFull = RES_NEWFRONT_ICONS_HIRES_OKAY;

// GLOBAL: LEMBALL 0x0049f830
unsigned long g_dwPreviewPreviousAnimIdsFull[2] = {RES_NEWFRONT_ICONS_HIRES_LEFT_ARROW,
												   RES_NEWFRONT_ICONS_HIRES_LEFT_ARROW_DISABLED};

// GLOBAL: LEMBALL 0x0049f838
unsigned long g_dwPreviewNextAnimIdsFull[2] = {RES_NEWFRONT_ICONS_HIRES_RIGHT_ARROW,
											   RES_NEWFRONT_ICONS_HIRES_RIGHT_ARROW_DISABLED};

// GLOBAL: LEMBALL 0x0049f840
unsigned long g_dwPreviewGoAnimIdsCompact = RES_NEWFRONT_ICONS_LORES_RETURN;

// GLOBAL: LEMBALL 0x0049f844
unsigned long g_dwPreviewReturnAnimIdsCompact = RES_NEWFRONT_ICONS_LORES_OKAY;

// GLOBAL: LEMBALL 0x0049f848
unsigned long g_dwPreviewPreviousAnimIdsCompact[2] = {RES_NEWFRONT_ICONS_LORES_LEFT_ARROW,
													  RES_NEWFRONT_ICONS_LORES_LEFT_ARROW_DISABLED};

// GLOBAL: LEMBALL 0x0049f850
unsigned long g_dwPreviewNextAnimIdsCompact[2] = {RES_NEWFRONT_ICONS_LORES_RIGHT_ARROW,
												  RES_NEWFRONT_ICONS_LORES_RIGHT_ARROW_DISABLED};

// GLOBAL: LEMBALL 0x0049f858
int g_previewRemapSourceIndices[10] = {0xa4, 0xa2, 0x80, 0x64, 0x4b, 0x5a, 0x34, 0x37, 0x3a, 0x96};

// GLOBAL: LEMBALL 0x0049f880
int g_previewRemapTargetIndices[10] = {0xb4, 0xb7, 0xb9, 0xbc, 0xbe, 0xbc, 0xbf, 0xbe, 0xbf, 0xb8};

// GLOBAL: LEMBALL 0x0049f8a8
int g_anPreviewTextIndices[4] = {0x0a, 0x0d, 0x10, 0x16};

// GLOBAL: LEMBALL 0x0049f8b8
char* g_szPreviewInfinite = "Infinite";

// GLOBAL: LEMBALL 0x0049f8bc
char* g_szPreviewX = "x";

// GLOBAL: LEMBALL 0x0049f8c0
char* g_szPreviewSkillNames[5] = {"Fun", " Tricky", "Taxing", "Mayhem", "Network"};

// GLOBAL: LEMBALL 0x0049f908
char g_szPreviewNone[] = "None";

// GLOBAL: LEMBALL 0x0049f910
char g_szPreviewUnnamedLevel[] = "UN-NAMED LEVEL";

// FUNCTION: LEMBALL 0x004491b0
CPreviewDrawer::CPreviewDrawer(CMain2DDisplay* p_display, CGDI* p_gdi, const CVSRect& p_rect)
	: CBaseFrontendDrawer(p_display, p_gdi, p_rect, FLOW_PREVIEW, 0x32, 200, 0, 0x28, 0x30)
{
	m_drawBackground = 1;
	m_drawFrame = 1;
	m_drawSolid = 1;
	Setup();
	m_lemmingAnim = new CRepeatAnim(CAnimsManager::GetnAnims(m_lemmingAnimId), 1);
	m_lemmingAnim->StartAnim(500);
	m_lemmingAnim->m_fixedTime = 0xffffffff;
	m_teamAnim = new CRepeatAnim(CAnimsManager::GetnAnims(m_teamAnimId), 1);
	m_teamAnim->StartAnim(500);
	m_teamAnim->m_fixedTime = 0xffffffff;
	m_opponentAnim = new CRepeatAnim(CAnimsManager::GetnAnims(m_opponentAnimId), 1);
	m_opponentAnim->StartAnim(500);
	m_opponentAnim->m_fixedTime = 0xffffffff;
	RegisterRemaps();
}

// FUNCTION: LEMBALL 0x00449370
void CPreviewDrawer::Load()
{
	int i;
	unsigned long* returnAnim;
	unsigned long* goAnim;
	PreviewLayout* layout;

	if (m_mode == 1) {
		m_backgroundBitmap = CResBITMAP::Load(RES_NEWFRONT_BITMAPS_LORES_GUNLEMM);
		m_layout = (PreviewLayout*) g_abPreviewLayoutCompact;
		goAnim = &g_dwPreviewGoAnimIdsCompact;
		returnAnim = &g_dwPreviewReturnAnimIdsCompact;
		m_lemmingAnimId = RES_NEWFRONT_ANIMS_LORES_DANCE;
		m_teamAnimId = RES_NEWFRONT_ANIMS_LORES_FLAG;
		m_ambientAnimId = RES_NEWFRONT_ANIMS_LORES_GUNLEMM_EYES;
		m_opponentAnimId = RES_NEWFRONT_ANIMS_LORES_CLOCK;
		m_nextButtonAnimIds = g_dwPreviewNextAnimIdsCompact;
		m_previousButtonAnimIds = g_dwPreviewPreviousAnimIdsCompact;
	}
	else {
		m_backgroundBitmap = CResBITMAP::Load(RES_NEWFRONT_BITMAPS_HIRES_GUNLEMM);
		m_layout = (PreviewLayout*) g_abPreviewLayoutFull;
		goAnim = &g_dwPreviewGoAnimIdsFull;
		returnAnim = &g_dwPreviewReturnAnimIdsFull;
		m_lemmingAnimId = RES_NEWFRONT_ANIMS_HIRES_DANCE;
		m_teamAnimId = RES_NEWFRONT_ANIMS_HIRES_FLAG;
		m_ambientAnimId = RES_NEWFRONT_ANIMS_HIRES_GUNLEMM_EYES;
		m_opponentAnimId = RES_NEWFRONT_ANIMS_HIRES_CLOCK;
		m_nextButtonAnimIds = g_dwPreviewNextAnimIdsFull;
		m_previousButtonAnimIds = g_dwPreviewPreviousAnimIdsFull;
	}
	layout = m_layout;
	short y =
		(short) layout->m_positions[PreviewGunLemming].m_y + (short) layout->m_positions[PreviewGunLemmingOffset].m_y;
	short x =
		(short) layout->m_positions[PreviewGunLemmingOffset].m_x + (short) layout->m_positions[PreviewGunLemming].m_x;
	m_animPosition.m_x = x;
	m_animPosition.m_y = y;
	for (i = 0; i < 1; i++) {
		m_primitiveBundle[i].m_primitive.m_x = (short) m_layout->m_positions[PreviewBackground].m_x;
		m_primitiveBundle[i].m_primitive.m_y = (short) m_layout->m_positions[PreviewBackground].m_y;
		m_primitiveBundle[i].m_primitive.m_resource = CBaseFrontendDrawer::m_backgroundBitmap;
		m_primitiveBundle[i].m_primitive.m_flags = CBitmap::BITMAP_TRANSPARENT_ZERO;
		m_primitiveBundle[i].m_primitive.m_remap = 0;
		m_primitive[i].m_bitmap.m_x = (short) m_layout->m_positions[PreviewGunLemming].m_x;
		m_primitive[i].m_bitmap.m_y = (short) m_layout->m_positions[PreviewGunLemming].m_y;
		m_primitive[i].m_bitmap.m_resource = m_backgroundBitmap;
		m_primitive[i].m_bitmap.m_flags = CBitmap::BITMAP_TRANSPARENT_ZERO;
		m_primitive[i].m_bitmap.m_remap = 0;
	}
	LoadAnims(m_lemmingAnimId);
	LoadAnims(m_teamAnimId);
	LoadAnims(m_ambientAnimId);
	unsigned int* buttonBinding = &m_buttonBinding;
	LoadAnims(m_opponentAnimId);
	unsigned int* nextDisabled = &m_nextDisabled;
	*buttonBinding = 0;
	*nextDisabled = 0;
	m_previousDisabled = 0;
	m_hiliteController = new CHiliteController((CGWnd*) m_display, m_gdi, 4, m_mode, 0);
	m_hiliteController->AddButton(m_layout->m_positions[PreviewReturnButton].m_x,
								  m_layout->m_positions[PreviewReturnButton].m_y,
								  returnAnim,
								  1,
								  0,
								  0,
								  0,
								  buttonBinding,
								  PREVIEW_BUTTON_MESSAGE_RETURN);
	m_hiliteController->AddButton(m_layout->m_positions[PreviewGoButton].m_x,
								  m_layout->m_positions[PreviewGoButton].m_y,
								  goAnim,
								  1,
								  0,
								  0,
								  0,
								  buttonBinding,
								  PREVIEW_BUTTON_MESSAGE_GO);
	m_hiliteController->AddButton(m_layout->m_positions[PreviewPreviousButton].m_x,
								  m_layout->m_positions[PreviewPreviousButton].m_y,
								  m_previousButtonAnimIds,
								  1,
								  0,
								  1,
								  0,
								  &m_previousDisabled,
								  PREVIEW_BUTTON_MESSAGE_PREVIOUS_LEVEL);
	m_hiliteController->AddButton(m_layout->m_positions[PreviewNextButton].m_x,
								  m_layout->m_positions[PreviewNextButton].m_y,
								  m_nextButtonAnimIds,
								  1,
								  0,
								  1,
								  0,
								  nextDisabled,
								  PREVIEW_BUTTON_MESSAGE_NEXT_LEVEL);
	m_hiliteController->SetHilite(0);
	m_hiliteController->SetHiliteWindow();
	LoadLevelInformation();
}

// FUNCTION: LEMBALL 0x00449670
void CPreviewDrawer::UnLoad()
{
	if (m_hiliteController != 0) {
		delete m_hiliteController;
	}
	m_backgroundBitmap->UnLoad();
	CAnimsManager::UnLoadAnims(m_lemmingAnimId);
	CAnimsManager::UnLoadAnims(m_teamAnimId);
	CAnimsManager::UnLoadAnims(m_ambientAnimId);
	CAnimsManager::UnLoadAnims(m_opponentAnimId);
}

// FUNCTION: LEMBALL 0x004496d0
CPreviewDrawer::~CPreviewDrawer()
{
	delete m_lemmingAnim;
	delete m_opponentAnim;
	delete m_teamAnim;
	UnRegisterRemaps();
	if (m_loaded != 0) {
		UnLoad();
	}
}

// FUNCTION: LEMBALL 0x00449750
void CPreviewDrawer::DrawBackGround()
{
	m_primitive[m_primitiveBank].m_bitmap.Draw(m_gdi);
	DrawFrame(m_layout->m_positions[PreviewFormationAnchor],
			  m_layout->m_positions[m_networkMode != 0 ? PreviewNetworkFrameEndpoint : PreviewFormationOffset]);
}

// FUNCTION: LEMBALL 0x004497b0
void CPreviewDrawer::DrawText()
{
	CVSSize advance;
	CVSSize pos;
	int* positions;
	char* line;
	int count;
	int skill;

	if (m_drawingBackBuffer != 0) {
		line = (char*) m_levelNameLines;
		positions = m_textPositions;
		count = 3;
		do {
			if (*positions != -1) {
				pos.m_width = (short) *positions;
				pos.m_height = (short) positions[1];
				advance.m_height = 0;
				advance.m_width = 0;
				m_textManager->DrawString(m_gdi, (CVSPoint&) pos, advance, m_chalkFontId, line, 0x20, 0);
			}
			line = line + 0x20;
			positions = positions + 2;
			count = count - 1;
		} while (count != 0);

		count = 3;
		if (m_networkMode != 0) {
			count = 4;
		}
		if (count > 0) {
			positions = g_anPreviewTextIndices;
			do {
				advance.m_height = 0;
				advance.m_width = 0;
				CoordPair* layoutPosition = &m_layout->m_positions[*positions];
				pos.m_width = (short) layoutPosition->m_x;
				pos.m_height = (short) layoutPosition->m_y;
				positions = positions + 1;
				m_textManager
					->DrawString(m_gdi, (CVSPoint&) pos, advance, m_chalkFontId, (char*) g_szPreviewX, 0x20, 0);
				count = count - 1;
			} while (count != 0);
		}

		{
			PreviewLayout* layout = m_layout;
			if (m_timeText[0] > '9') {
				advance.m_height = 0;
				advance.m_width = 0;
				pos.m_width = (short) layout->m_positions[PreviewTimeText].m_x;
				pos.m_height = (short) layout->m_positions[PreviewTimeText].m_y;
				m_textManager->DrawString(m_gdi, (CVSPoint&) pos, advance, m_chalkFontId, g_szPreviewInfinite, 0x20, 0);
			}
			else {
				advance.m_height = 0;
				advance.m_width = 0;
				pos.m_width = (short) layout->m_positions[PreviewTimeText].m_x;
				pos.m_height = (short) layout->m_positions[PreviewTimeText].m_y;
				m_textManager->DrawString(m_gdi, (CVSPoint&) pos, advance, m_chalkFontId, m_timeText, 0x20, 0);
			}
		}

		skill = g_pGameStatus->m_skill;
		PreviewLayout* skillLayout = m_layout;
		int skillX = skillLayout->m_positions[PreviewSkillText].m_x;
		int skillY = skillLayout->m_positions[PreviewSkillText].m_y;
		short halfWidth =
			(short) (m_textManager->GetFont(m_chalkFontId)->GetSize(g_szPreviewSkillNames[skill], 0x20).m_width / 2);
		pos.m_width = (short) (skillX - halfWidth);
		advance.m_height = 0;
		advance.m_width = 0;
		pos.m_height = (short) skillY;
		m_textManager
			->DrawString(m_gdi, (CVSPoint&) pos, advance, m_chalkFontId, g_szPreviewSkillNames[skill], 0x20, 0);

		if (m_teamCount > 4) {
			advance.m_height = 0;
			advance.m_width = 0;
			pos.m_width = (short) m_layout->m_positions[PreviewTimeText].m_x;
			pos.m_height = (short) m_layout->m_positions[PreviewNoneText].m_y;
			m_textManager->DrawString(m_gdi, (CVSPoint&) pos, advance, m_chalkFontId, g_szPreviewNone, 0x20, 0);
		}
	}
}

// FUNCTION: LEMBALL 0x00449a40
void CPreviewDrawer::DrawAnims()
{
	short width;
	int i;
	int x;
	int y;

	CAnimsManager::DrawAnim(CVSPoint((short) m_layout->m_positions[PreviewLemmingAnim].m_x,
									 (short) m_layout->m_positions[PreviewLemmingAnim].m_y),
							m_lemmingAnimId,
							0,
							m_lemmingAnim,
							0);

	CAnimsManager::DrawAnim(CVSPoint((short) m_layout->m_positions[PreviewOpponentAnim].m_x,
									 (short) m_layout->m_positions[PreviewOpponentAnim].m_y),
							m_opponentAnimId,
							0,
							m_opponentAnim,
							0);

	DrawAnim(CVSPoint((short) m_layout->m_positions[PreviewTeamAnim].m_x,
					  (short) m_layout->m_positions[PreviewTeamAnim].m_y),
			 m_teamAnimId,
			 0,
			 m_teamAnim,
			 0);

	if (m_networkMode != 0) {
		CAnimsManager::DrawAnim(CVSPoint((short) m_layout->m_positions[PreviewNetworkLemmingAnim].m_x,
										 (short) m_layout->m_positions[PreviewNetworkLemmingAnim].m_y),
								m_lemmingAnimId,
								0,
								m_lemmingAnim,
								(CRemap*) m_remap);
		width = CAnimsManager::GetAnimSize(m_lemmingAnimId, 0).m_width;
		x = m_layout->m_positions[PreviewFormationAnchor].m_x - (short) (width / 4) +
			m_layout->m_positions[PreviewFormationOffset].m_x;
		y = m_layout->m_positions[PreviewNetworkLemmingRow].m_y;
		i = 0;
		if (m_lemmingCount > 0) {
			do {
				x = x - ((short) (width / 8) + width);
				CVSPoint point((short) x, (short) y);
				CAnimsManager::DrawAnim(point, m_lemmingAnimId, 0, m_lemmingAnim, (CRemap*) m_remap);
				i = i + 1;
			} while (i < m_lemmingCount);
		}
	}

	width = CAnimsManager::GetAnimSize(m_lemmingAnimId, 0).m_width;
	x = m_layout->m_positions[PreviewFormationAnchor].m_x - (short) (width / 4) +
		m_layout->m_positions[PreviewFormationOffset].m_x;
	y = m_layout->m_positions[PreviewOpponentRow].m_y;
	i = 0;
	if (m_opponentCount > 0) {
		do {
			x = x - ((short) (width / 8) + width);
			CVSPoint point((short) x, (short) y);
			CAnimsManager::DrawAnim(point, m_lemmingAnimId, 0, m_lemmingAnim, 0);
			i = i + 1;
		} while (i < m_opponentCount);
	}

	width = (short) CAnimsManager::GetAnimSize(m_teamAnimId, 0).m_width;
	x = m_layout->m_positions[PreviewFormationAnchor].m_x - m_layout->m_positions[PreviewTeamOffset].m_x +
		m_layout->m_positions[PreviewFormationOffset].m_x;
	y = m_layout->m_positions[PreviewTeamRow].m_y;
	if (m_teamCount <= 4) {
		i = 0;
		if (m_teamCount > 0) {
			do {
				x = x - width;
				CVSPoint point((short) x, (short) y);
				CAnimsManager::DrawAnim(point, m_teamAnimId, 0, m_teamAnim, 0);
				i = i + 1;
			} while (i < m_teamCount);
		}
	}
}

// FUNCTION: LEMBALL 0x00449d30
bool CPreviewDrawer::ProcessMessages(Message* p_message)
{
	switch ((int) p_message->m_type) {
	default:
		m_processedCount = m_processedCount + 1;
		return 0;
	case 0xc:
		break;
	}
	switch (p_message->m_code) {
	default:
		return 0;
	case PREVIEW_BUTTON_MESSAGE_GO:
		if (m_networkMode != 0) {
			Action(USER_ACTION_2, USER_ACTION_STAGE_REQUEST);
			return 1;
		}
		m_quitYet = 1;
		m_returnState = 2;
		return 1;
	case PREVIEW_BUTTON_MESSAGE_RETURN:
		if (m_networkMode != 0) {
			Action(USER_ACTION_3, USER_ACTION_STAGE_REQUEST);
			return 1;
		}
		m_quitYet = 1;
		m_returnState = 5;
		return 1;
	case PREVIEW_BUTTON_MESSAGE_NEXT_LEVEL:
		if (m_nextDisabled == 1) {
			m_ready = 1;
			return 1;
		}
		if (m_networkMode != 0) {
			Action(USER_ACTION_0, USER_ACTION_STAGE_REQUEST);
			return 1;
		}
		NextLevel();
		return 1;
	case PREVIEW_BUTTON_MESSAGE_PREVIOUS_LEVEL:
		if (m_previousDisabled == 1) {
			m_ready = 1;
			return 1;
		}
		if (m_networkMode != 0) {
			Action(USER_ACTION_1, USER_ACTION_STAGE_REQUEST);
			return 1;
		}
		PreviousLevel();
		return 1;
	}
}

// FUNCTION: LEMBALL 0x00449e60
void CPreviewDrawer::NextLevel()
{
	g_pGameStatus->IncLevel();
	LoadLevelInformation();
}

// FUNCTION: LEMBALL 0x00449e80
void CPreviewDrawer::PreviousLevel()
{
	g_pGameStatus->DecLevel();
	LoadLevelInformation();
}

// FUNCTION: LEMBALL 0x00449ea0
void CPreviewDrawer::Go()
{
	m_quitYet = 1;
	m_returnState = 5;
}

// FUNCTION: LEMBALL 0x00449ec0
void CPreviewDrawer::Return()
{
	m_quitYet = 1;
	m_returnState = 2;
}

// FUNCTION: LEMBALL 0x00449ee0
bool CPreviewDrawer::ConfirmedAction(eUserActions p_action)
{
	switch (p_action) {
	case USER_ACTION_0:
		NextLevel();
		return 1;
	case USER_ACTION_1:
		PreviousLevel();
		return 1;
	case USER_ACTION_2:
		Return();
		return 1;
	case USER_ACTION_3:
		Go();
		return 1;
	default:
		return 0;
	}
}

// FUNCTION: LEMBALL 0x00449f40
void CPreviewDrawer::Processing()
{
	if (g_nTestAllLevels) {
		m_quitYet = 1;
		m_returnState = 5;
	}
}

// FUNCTION: LEMBALL 0x00449f60
void CPreviewDrawer::LoadLevelInformation()
{
	tPreviewData preview;
	char* source;
	CResFONT* font;
	short measuredWidth;
	char candidateLine[32];
	int sourcePos;
	int linePos;
	int lineIndex;
	int layoutX;
	int layoutY;
	int layoutWidth;
	char* targetLine;
	int* targetPos;
	int endOfSource;

	endOfSource = 0;
	sourcePos = 0;
	m_backBufferNeeded = 1;
	m_ready = 1;

	CLevelLoader::RetrievePreviewData((eSkill) g_pGameStatus->m_skill,
									  g_pGameStatus->m_lastLevels[g_pGameStatus->m_skill],
									  &preview);

	if (m_networkMode != 0) {
		m_lemmingCount = preview.m_opponentLemmingCount;
	}
	m_teamCount = preview.m_playerCount;
	source = preview.m_name;
	m_opponentCount = preview.m_lemmingCount;
	m_timeSeconds = preview.m_timeLimit;
	if (preview.m_name[0] == 0) {
		source = g_szPreviewUnnamedLevel;
	}

	font = m_textManager->GetFont(m_chalkFontId);

	int i;
	memset(candidateLine, 0, sizeof(candidateLine));
	for (i = 0; i < 0x20; i++) {
		m_levelNameLines[0][i] = 0;
		m_levelNameLines[1][i] = 0;
		m_levelNameLines[2][i] = 0;
	}

	targetPos = m_textPositions;
	for (i = 0; i < 3; i++) {
		targetPos[0] = -1;
		targetPos[1] = -1;
		targetPos += 2;
	}

	lineIndex = 0;
	layoutX = m_layout->m_positions[PreviewLevelNameOrigin].m_x;
	layoutY = m_layout->m_positions[PreviewLevelNameOrigin].m_y;
	layoutWidth = m_layout->m_positions[PreviewLevelNameBounds].m_x;
	targetLine = (char*) m_levelNameLines;
	targetPos = m_textPositions;

	do {
		if (endOfSource == 1) {
			break;
		}
		linePos = 0;
		memset(candidateLine, '0', sizeof(candidateLine));
		while (1) {
			endOfSource = (int) AddWord(source, candidateLine, sourcePos, linePos);
			measuredWidth = font->GetSize(candidateLine, 0x20).m_width;
			if (measuredWidth > layoutWidth || endOfSource == 1) {
				break;
			}
			char* candidateEnd = candidateLine + strlen(candidateLine);
			candidateEnd[0] = ' ';
			candidateEnd[1] = 0;
			linePos = linePos + 1;
		}
		if (measuredWidth > layoutWidth) {
			endOfSource = 0;
			SubWord(source, candidateLine, sourcePos, linePos);
		}
		const CVSSize& size = font->GetSize(candidateLine, 0x20);
		short measuredHeight = size.m_height;
		targetPos[0] = (layoutWidth / 2 - (int) (size.m_width / 2)) + layoutX;
		targetPos[1] = layoutY;
		strcpy(targetLine, candidateLine);
		layoutY = layoutY + measuredHeight;
		lineIndex = lineIndex + 1;
		targetLine = targetLine + 0x20;
		targetPos = targetPos + 2;
	} while (lineIndex < 3);

	m_timeText[0] = (char) (m_timeSeconds / 60) + '0';
	m_timeText[1] = ':';
	m_timeText[2] = (char) ((m_timeSeconds % 60) / 10) + '0';
	m_timeText[3] = (char) (m_timeSeconds % 10) + '0';
	m_timeText[4] = 0;

	DisableNextLastButtons();
}

// FUNCTION: LEMBALL 0x0044a250
void CPreviewDrawer::SubWord(char* p_source, char* p_line, int& p_sourcePos, int& p_linePos)
{
	int count = 0;

	p_sourcePos -= 2;
	if ((int) strlen(p_source) > 0) {
		while (p_source[p_sourcePos] != ' ') {
			count++;
			p_sourcePos--;
			p_line[p_linePos--] = 0;
			if ((int) strlen(p_source) <= count) {
				return;
			}
		}
		p_line[p_linePos--] = 0;
		p_line[p_linePos] = 0;
		p_sourcePos++;
	}
}

// FUNCTION: LEMBALL 0x0044a2d0
bool CPreviewDrawer::AddWord(char* p_source, char* p_line, int& p_sourcePos, int& p_linePos)
{
	char c;

	c = p_source[p_sourcePos];
	while (c != 0 && p_source[p_sourcePos] != ' ') {
		p_line[p_linePos] = p_source[p_sourcePos];
		p_sourcePos = p_sourcePos + 1;
		p_linePos = p_linePos + 1;
		c = p_source[p_sourcePos];
	}
	p_line[p_linePos] = 0;
	if (p_source[p_sourcePos] == 0) {
		return 1;
	}
	p_sourcePos = p_sourcePos + 1;
	return 0;
}

// FUNCTION: LEMBALL 0x0044a3c0
void CPreviewDrawer::UnRegisterRemaps()
{
	g_pBasePalManager->UnRegisterRemap(m_remap);
}

// FUNCTION: LEMBALL 0x0044a3e0
void CPreviewDrawer::DisableNextLastButtons()
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

	m_hiliteController->UpdateAnimIDs(PREVIEW_BUTTON_MESSAGE_NEXT_LEVEL);
	m_hiliteController->UpdateAnimIDs(PREVIEW_BUTTON_MESSAGE_PREVIOUS_LEVEL);
}

// FUNCTION: LEMBALL 0x0044a970
CPreviewDrawer::tagPRIMS::tagPRIMS()
{
}

// FUNCTION: LEMBALL 0x0044a9c0
CPreviewDrawer::tagPRIMS::~tagPRIMS()
{
}
