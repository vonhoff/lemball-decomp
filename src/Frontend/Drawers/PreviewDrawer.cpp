#include "PreviewDrawer.h"

#include "../../Control/Game/GameStatus.h"
#include "../../Control/Level/LevelLoader.h"
#include "../../Control/Support/PreviewData.h"
#include "../../Views/Display/Main2DDisplay.h"
#include "../../Visos/Animation/AnimsManager.h"
#include "../../Visos/Animation/RepeatAnim.h"
#include "../../Visos/Foundation/TextManager.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/BasePalManager.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Resources/ResBitmap.h"
#include "../../Visos/Resources/ResFont.h"
#include "../../Visos/Resources/ResPalette.h"
#include "../Base/BaseFrontendProcess.h"
#include "../Controls/HiliteController.h"

#include <new.h>
#include <string.h>

#pragma intrinsic(strcpy, strlen)

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
unsigned long g_dwPreviewGoAnimIdsFull = RES_NEWFRONT_ANIMS_HIRES_HILITE;

// GLOBAL: LEMBALL 0x0049f82c
unsigned long g_dwPreviewReturnAnimIdsFull = RES_NEWFRONT_ANIMS_HIRES_FAIL_EYES;

// GLOBAL: LEMBALL 0x0049f830
unsigned long g_dwPreviewPreviousAnimIdsFull[2] = {RES_NEWFRONT_ANIMS_HIRES_SUCCESS_EYES,
												   RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_1};

// GLOBAL: LEMBALL 0x0049f838
unsigned long g_dwPreviewNextAnimIdsFull[2] = {RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_0,
											   RES_NEWFRONT_ANIMS_HIRES_PASSWORD_BUTTON_2};

// GLOBAL: LEMBALL 0x0049f840
unsigned long g_dwPreviewGoAnimIdsCompact = RES_NEWFRONT_ANIMS_LORES_HILITE;

// GLOBAL: LEMBALL 0x0049f844
unsigned long g_dwPreviewReturnAnimIdsCompact = RES_NEWFRONT_ANIMS_LORES_FAIL_EYES;

// GLOBAL: LEMBALL 0x0049f848
unsigned long g_dwPreviewPreviousAnimIdsCompact[2] = {RES_NEWFRONT_ANIMS_LORES_SUCCESS_EYES,
													  RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_1};

// GLOBAL: LEMBALL 0x0049f850
unsigned long g_dwPreviewNextAnimIdsCompact[2] = {RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_0,
												  RES_NEWFRONT_ANIMS_LORES_PASSWORD_BUTTON_2};

// GLOBAL: LEMBALL 0x0049f858
int g_previewRemapSourceIndices[10] = {0xa4, 0xa2, 0x80, 0x64, 0x4b, 0x5a, 0x34, 0x37, 0x3a, 0x96};

// GLOBAL: LEMBALL 0x0049f880
int g_previewRemapTargetIndices[10] = {0xb4, 0xb7, 0xb9, 0xbc, 0xbe, 0xbc, 0xbf, 0xbe, 0xbf, 0xb8};

// GLOBAL: LEMBALL 0x0049f8a8
int g_anPreviewTextIndices[4] = {0x0a, 0x0d, 0x10, 0x16};

// GLOBAL: LEMBALL 0x0049f8b8
char g_szPreviewInfinite[] = "Infinite";

// GLOBAL: LEMBALL 0x0049f8bc
char g_szPreviewX[] = "x";

// GLOBAL: LEMBALL 0x0049f8c0
char* g_szPreviewSkillNames[6] = {"Fun", " Tricky", "Taxing", "Mayhem", "Network", "None"};

// GLOBAL: LEMBALL 0x0049f908
char g_szPreviewUnnamedLevel[] = "UN-NAMED LEVEL";

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
		m_lemmingAnim = new (storage) RepeatAnim(AnimsManager::GetnAnims(m_lemmingAnimId), 1);
	}
	m_lemmingAnim->StartAnim(500);
	m_lemmingAnim->m_fixedTime = 0xffffffff;
	storage = operator new(0x1c);
	if (storage == 0) {
		m_teamAnim = 0;
	}
	else {
		m_teamAnim = new (storage) RepeatAnim(AnimsManager::GetnAnims(m_teamAnimId), 1);
	}
	m_teamAnim->StartAnim(500);
	m_teamAnim->m_fixedTime = 0xffffffff;
	storage = operator new(0x1c);
	if (storage == 0) {
		m_opponentAnim = 0;
	}
	else {
		m_opponentAnim = new (storage) RepeatAnim(AnimsManager::GetnAnims(m_opponentAnimId), 1);
	}
	m_opponentAnim->StartAnim(500);
	m_opponentAnim->m_fixedTime = 0xffffffff;
	RegisterRemaps();
}

// 68K 0x1080b000 Load__14CPreviewDrawerFv
// FUNCTION: LEMBALL 0x00449370
void PreviewDrawer::Load()
{
	unsigned long* goAnim;
	unsigned long* returnAnim;
	int* layout;

	if (m_mode == 1) {
		m_backgroundBitmap = ResBitmap::Load(RES_NEWFRONT_BITMAPS_LORES_GUNLEMM);
		m_layout = g_abPreviewLayoutCompact;
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
		m_backgroundBitmap = ResBitmap::Load(RES_NEWFRONT_BITMAPS_HIRES_GUNLEMM);
		m_layout = g_abPreviewLayoutFull;
		goAnim = &g_dwPreviewGoAnimIdsFull;
		returnAnim = &g_dwPreviewReturnAnimIdsFull;
		m_lemmingAnimId = RES_NEWFRONT_ANIMS_HIRES_DANCE;
		m_teamAnimId = RES_NEWFRONT_ANIMS_HIRES_FLAG;
		m_ambientAnimId = RES_NEWFRONT_ANIMS_HIRES_GUNLEMM_EYES;
		m_opponentAnimId = RES_NEWFRONT_ANIMS_HIRES_CLOCK;
		m_nextButtonAnimIds = g_dwPreviewNextAnimIdsFull;
		m_previousButtonAnimIds = g_dwPreviewPreviousAnimIdsFull;
	}
	layout = (int*) m_layout;
	m_animPosition.m_x = (short) layout[0x38 / 4] + (short) layout[0x40 / 4];
	m_animPosition.m_y = (short) layout[0x3c / 4] + (short) layout[0x44 / 4];
	m_primitiveBundle.m_primitive.m_x = (short) layout[0x20 / 4];
	m_primitiveBundle.m_primitive.m_y = (short) layout[0x24 / 4];
	m_primitiveBundle.m_primitive.m_resource = BaseFrontendDrawer::m_backgroundBitmap;
	m_primitiveBundle.m_primitive.m_flags = 0x800;
	m_primitiveBundle.m_primitive.m_remap = 0;
	m_primitive.m_bitmap.m_x = (short) layout[0x38 / 4];
	m_primitive.m_bitmap.m_y = (short) layout[0x3c / 4];
	m_primitive.m_bitmap.m_resource = m_backgroundBitmap;
	m_primitive.m_bitmap.m_flags = 0x800;
	AnimsManager::LoadAnims(m_lemmingAnimId);
	AnimsManager::LoadAnims(m_teamAnimId);
	AnimsManager::LoadAnims(m_ambientAnimId);
	AnimsManager::LoadAnims(m_opponentAnimId);
	m_buttonBinding = 0;
	m_nextDisabled = 0;
	m_previousDisabled = 0;
	m_hiliteController = new HiliteController((GWnd*) m_display, m_gdi, 4, (unsigned char) m_mode, 0);
	m_hiliteController->AddButton(layout[0], layout[1], returnAnim, 1, 0, 0, 0, &m_buttonBinding, 0xacef000d);
	m_hiliteController->AddButton(layout[2], layout[3], goAnim, 1, 0, 0, 0, &m_buttonBinding, 0xacef000c);
	m_hiliteController
		->AddButton(layout[4], layout[5], m_previousButtonAnimIds, 1, 0, 1, 0, &m_previousDisabled, 0xacef000f);
	m_hiliteController->AddButton(layout[6], layout[7], m_nextButtonAnimIds, 1, 0, 1, 0, &m_nextDisabled, 0xacef000e);
	m_hiliteController->SetHilite(0);
	m_hiliteController->SetHiliteWindow();
	LoadLevelInformation();
}

// 68K 0x1080b344 UnLoad__14CPreviewDrawerFv
// FUNCTION: LEMBALL 0x00449670
void PreviewDrawer::UnLoad()
{
	if (m_hiliteController != 0) {
		delete m_hiliteController;
	}
	m_backgroundBitmap->UnLoad();
	AnimsManager::UnLoadAnims(m_lemmingAnimId);
	AnimsManager::UnLoadAnims(m_teamAnimId);
	AnimsManager::UnLoadAnims(m_ambientAnimId);
	AnimsManager::UnLoadAnims(m_opponentAnimId);
}

// 68K 0x1080b3d6 __dt__14CPreviewDrawerFv
// FUNCTION: LEMBALL 0x004496d0
PreviewDrawer::~PreviewDrawer()
{
	if (m_lemmingAnim != 0) {
		delete m_lemmingAnim;
	}
	if (m_opponentAnim != 0) {
		delete m_opponentAnim;
	}
	if (m_teamAnim != 0) {
		delete m_teamAnim;
	}
	UnRegisterRemaps();
	if (m_loaded != 0) {
		UnLoad();
	}
}

// 68K 0x1080b49c DrawBackGround__14CPreviewDrawerFv
// FUNCTION: LEMBALL 0x00449750
void PreviewDrawer::DrawBackGround()
{
	int* layout = (int*) m_layout;
	m_primitive.m_bitmap.Draw(m_gdi);
	DrawFrame(*(CoordPair*) &layout[0x28 / 4], *(CoordPair*) &layout[0x2c / 4]);
}

// 68K 0x1080b52c DrawText__14CPreviewDrawerFv
// FUNCTION: LEMBALL 0x004497b0
void PreviewDrawer::DrawText()
{
	VsSize advance;
	VsPoint pos;
	VsPoint size;
	char* line;
	int* positions;
	int count;
	int* layout;
	int skill;
	ResFont* font;

	if (m_drawingBackBuffer != 0) {
		line = (char*) m_levelNameLines;
		positions = m_textPositions;
		count = 3;
		do {
			if (*positions != -1) {
				pos.m_x = (short) *positions;
				pos.m_y = (short) positions[1];
				advance.m_height = 0;
				advance.m_width = 0;
				m_textManager->DrawString(m_gdi, pos, advance, m_chalkFontId, line, 0x20, 0);
			}
			line = line + 0x20;
			positions = positions + 2;
			count = count - 1;
		} while (count != 0);

		count = (m_networkMode != 0) ? 4 : 3;
		if (count != 0) {
			positions = g_anPreviewTextIndices;
			do {
				advance.m_height = 0;
				advance.m_width = 0;
				layout = (int*) ((char*) m_layout + *positions * 8);
				positions = positions + 1;
				pos.m_x = (short) *layout;
				pos.m_y = (short) layout[1];
				m_textManager->DrawString(m_gdi, pos, advance, m_chalkFontId, (char*) g_szPreviewX, 0x20, 0);
				count = count - 1;
			} while (count != 0);
		}

		layout = (int*) m_layout;
		if (m_timeText[0] < ':') {
			pos.m_x = (short) layout[0x58 / 4];
			pos.m_y = (short) layout[0x5c / 4];
			advance.m_height = 0;
			advance.m_width = 0;
			m_textManager->DrawString(m_gdi, pos, advance, m_chalkFontId, m_timeText, 0x20, 0);
		}
		else {
			pos.m_x = (short) layout[0x58 / 4];
			pos.m_y = (short) layout[0x5c / 4];
			advance.m_height = 0;
			advance.m_width = 0;
			m_textManager->DrawString(m_gdi, pos, advance, m_chalkFontId, g_szPreviewInfinite, 0x20, 0);
		}

		skill = g_pGameStatus->m_skill;
		font = m_textManager->GetFont(m_chalkFontId);
		size = font->GetSize(g_szPreviewSkillNames[skill], 0x20);
		advance.m_height = 0;
		advance.m_width = 0;
		pos.m_x = (short) layout[0xa0 / 4] - size.m_x / 2;
		pos.m_y = (short) layout[0xa4 / 4];
		m_textManager->DrawString(m_gdi, pos, advance, m_chalkFontId, g_szPreviewSkillNames[skill], 0x20, 0);

		if (m_teamCount > 4) {
			advance.m_height = 0;
			advance.m_width = 0;
			pos.m_x = (short) layout[0x58 / 4];
			pos.m_y = (short) layout[0xd4 / 4];
			m_textManager->DrawString(m_gdi, pos, advance, m_chalkFontId, g_szPreviewSkillNames[5], 0x20, 0);
		}
	}
}

// 68K 0x1080b82e DrawAnims__14CPreviewDrawerFv
// FUNCTION: LEMBALL 0x00449a40
void PreviewDrawer::DrawAnims()
{
	VsPoint pos;
	VsPoint point;
	short width;
	int* layout;
	int x;
	int y;
	int i;
	int step;

	layout = (int*) m_layout;
	pos.m_x = (short) layout[0x78 / 4];
	pos.m_y = (short) layout[0x7c / 4];
	AnimsManager::DrawAnim(pos, m_lemmingAnimId, 0, m_lemmingAnim, 0);

	pos.m_x = (short) layout[0x48 / 4];
	pos.m_y = (short) layout[0x4c / 4];
	AnimsManager::DrawAnim(pos, m_opponentAnimId, 0, m_opponentAnim, 0);

	pos.m_x = (short) layout[0x60 / 4];
	pos.m_y = (short) layout[0x64 / 4];
	AnimsManager::DrawAnim(pos, m_teamAnimId, 0, m_teamAnim, 0);

	if (m_networkMode != 0) {
		pos.m_x = (short) layout[0xa8 / 4];
		pos.m_y = (short) layout[0xac / 4];
		AnimsManager::DrawAnim(pos, m_lemmingAnimId, 0, m_lemmingAnim, (Remap*) m_remap);
		width = AnimsManager::GetAnimSize(m_lemmingAnimId, 0).m_width;
		y = layout[0xbc / 4];
		x = layout[0x28 / 4] - (short) ((width + ((width >> 15) & 3)) >> 2) + layout[0x30 / 4];
		i = 0;
		if (m_lemmingCount > 0) {
			step = (short) ((width + ((width >> 15) & 7)) >> 3);
			do {
				x = x - (step + width);
				i = i + 1;
				point.m_x = (short) x;
				point.m_y = (short) y;
				AnimsManager::DrawAnim(point, m_lemmingAnimId, 0, m_lemmingAnim, (Remap*) m_remap);
			} while (i < m_lemmingCount);
		}
	}

	width = AnimsManager::GetAnimSize(m_lemmingAnimId, 0).m_width;
	y = layout[0x8c / 4];
	x = layout[0x28 / 4] - (short) ((width + ((width >> 15) & 3)) >> 2) + layout[0x30 / 4];
	i = 0;
	if (m_opponentCount > 0) {
		step = (short) ((width + ((width >> 15) & 7)) >> 3);
		do {
			x = x - (step + width);
			point.m_x = (short) x;
			point.m_y = (short) y;
			i = i + 1;
			AnimsManager::DrawAnim(point, m_lemmingAnimId, 0, m_lemmingAnim, 0);
		} while (i < m_opponentCount);
	}

	width = (short) AnimsManager::GetAnimSize(m_teamAnimId, 0).m_width;
	y = layout[0x74 / 4];
	x = layout[0x28 / 4] - layout[0xc8 / 4] + layout[0x30 / 4];
	if (m_teamCount <= 4 && m_teamCount > 0) {
		i = 0;
		do {
			x = x - width;
			point.m_x = (short) x;
			point.m_y = (short) y;
			i = i + 1;
			AnimsManager::DrawAnim(point, m_teamAnimId, 0, m_teamAnim, 0);
		} while (i < m_teamCount);
	}
}

// 68K 0x1080bb42 ProcessMessages__14CPreviewDrawerFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00449d30
bool PreviewDrawer::ProcessMessages(Message* p_message)
{
	if (p_message->type != 0xc) {
		m_processedCount = m_processedCount + 1;
		return 0;
	}
	switch (p_message->code) {
	case 0xacef000c:
		if (m_networkMode == 0) {
			m_quitYet = 1;
			m_returnState = 2;
			return 1;
		}
		Action(2, 0);
		return 1;
	case 0xacef000d:
		if (m_networkMode == 0) {
			m_quitYet = 1;
			m_returnState = 5;
			return 1;
		}
		Action(3, 0);
		return 1;
	case 0xacef000e:
		break;
	case 0xacef000f:
		if (m_previousDisabled == 1) {
			m_ready = 1;
			return 1;
		}
		if (m_networkMode == 0) {
			PreviousLevel();
			return 1;
		}
		Action(1, 0);
		return 1;
	default:
		return 0;
	}
	if (m_nextDisabled == 1) {
		m_ready = 1;
		return 1;
	}
	if (m_networkMode == 0) {
		NextLevel();
		return 1;
	}
	Action(0, 0);
	return 1;
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
// FUNCTION: LEMBALL 0x00449f60
void PreviewDrawer::LoadLevelInformation()
{
	int* layout;
	VsPoint size;
	VsPoint lineSize;
	PreviewData preview;
	char* source;
	ResFont* font;
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

	m_backBufferNeeded = 1;
	endOfSource = 0;
	sourcePos = 0;
	m_ready = 1;

	LevelLoader::RetrievePreviewData((eSkill) g_pGameStatus->m_skill,
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

	memset(candidateLine, 0, sizeof(candidateLine));
	memset(m_levelNameLines, 0, sizeof(m_levelNameLines));

	targetPos = m_textPositions;
	for (int i = 0; i < 3; i++) {
		targetPos[0] = -1;
		targetPos[1] = -1;
		targetPos += 2;
	}

	layout = (int*) m_layout;
	lineIndex = 0;
	layoutX = layout[0x90 / 4];
	layoutY = layout[0x94 / 4];
	layoutWidth = layout[0x98 / 4];
	targetLine = (char*) m_levelNameLines;
	targetPos = m_textPositions;

	do {
		if (endOfSource == 1) {
			break;
		}
		linePos = 0;
		memset(candidateLine, 0, sizeof(candidateLine));
		while (1) {
			endOfSource = (int) AddWord(source, candidateLine, sourcePos, linePos);
			lineSize = font->GetSize(candidateLine, 0x20);
			if (lineSize.m_x > layoutWidth || endOfSource == 1) {
				break;
			}
			candidateLine[linePos] = ' ';
			linePos = linePos + 1;
			candidateLine[linePos] = 0;
		}
		if (lineSize.m_x > layoutWidth) {
			endOfSource = 0;
			SubWord(source, candidateLine, sourcePos, linePos);
		}
		size = font->GetSize(candidateLine, 0x20);
		targetPos[0] = (layoutWidth / 2 - (int) (size.m_x / 2)) + layoutX;
		targetPos[1] = layoutY;
		strcpy(targetLine, candidateLine);
		layoutY = layoutY + size.m_y;
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

// 68K 0x1080c14e SubWord__14CPreviewDrawerFPcPcRiRi
// FUNCTION: LEMBALL 0x0044a250
void PreviewDrawer::SubWord(char* p_source, char* p_line, int& p_sourcePos, int& p_linePos)
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

// 68K 0x1080c1d4 AddWord__14CPreviewDrawerFPcPcRiRi
// FUNCTION: LEMBALL 0x0044a2d0
bool PreviewDrawer::AddWord(char* p_source, char* p_line, int& p_sourcePos, int& p_linePos)
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

// 68K 0x1080c256 RegisterRemaps__14CPreviewDrawerFv
// FUNCTION: LEMBALL 0x0044a330
void PreviewDrawer::RegisterRemaps()
{
	ResPalette* palette;
	int i;

	palette = ResPalette::Load(RES_PALETTES_TITLEPALETTE);
	m_remapTable = (unsigned char*) operator new(0x100);
	i = 0;
	do {
		m_remapTable[i] = (unsigned char) i;
		i = i + 1;
	} while (i < 0x100);
	i = 0;
	do {
		int target = g_previewRemapTargetIndices[i];
		int source = g_previewRemapSourceIndices[i];
		if (target != 0) {
			m_remapTable[source] = (unsigned char) target;
		}
		i = i + 1;
	} while (i < 10);
	m_remap = g_pBasePalManager->RegisterRemap(RES_PALETTES_TITLEPALETTE, m_remapTable, (ePaletteTypes) 0);
	palette->UnLoad();
}

// 68K 0x1080c318 UnRegisterRemaps__14CPreviewDrawerFv
// FUNCTION: LEMBALL 0x0044a3c0
void PreviewDrawer::UnRegisterRemaps()
{
	g_pBasePalManager->UnRegisterRemap(m_remap);
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
