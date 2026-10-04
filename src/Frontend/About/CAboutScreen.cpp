#include "CAboutScreen.h"

#include "GameView/Display/CMain2DDisplay.h"
#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Graphics/Surfaces/CChangeList.h"
#include "Engine/Text/CTextManager.h"
#include "Engine/Strings/VsString.h"
#include "Engine/VsTime.h"
#include "Platform/Windows/Graphics/CCursor.h"
#include "Engine/Graphics/Primitives/CGDI.h"
#include "Engine/Controls/CHotAreaHandler.h"
#include "Engine/Graphics/Surfaces/CSurface.h"
#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/Types/CResBITMAP.h"
#include "Engine/Resources/Types/CResFONT.h"
#include "Engine/Resources/Types/CResSTRING.h"
#include "Engine/Resources/Manifest.h"
#include "Application/FlowProcesses.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"
#include "Engine/Queues/Message.h"
#include "Engine/Graphics/Primitives/CBigBitmap.h"
#include "Engine/Graphics/Primitives/CClipRect.h"
#include "Engine/Controls/CPVButton.h"
#include "Engine/Graphics/Primitives/CSolidRect.h"
#include "Engine/Resources/ResourceLimits.h"

#include <new.h>
#include <string.h>

enum {
	ABOUT_SCREEN_DISPLAY_DURATION_MS = 5 * MILLISECONDS_PER_SECOND
};

class ChangeListItem;

// GLOBAL: LEMBALL 0x0049f9e4
char g_szRegisteredTo[] = "Registered to";

// GLOBAL: LEMBALL 0x0049f9f4
char g_szAboutDecodeBuffer[24] = "01234567890123456789";

// GLOBAL: LEMBALL 0x0049fa0c
char g_szAboutWeatherManKey[] = "John Ketley is a Weatherman, and so is Michael Fish";

// GLOBAL: LEMBALL 0x0049fa40
char g_szVisosBuild[] = "ViSOS Build ";

// GLOBAL: LEMBALL 0x004a78d0
char g_szVisosBuildBuffer[80];

// FUNCTION: LEMBALL 0x0044b750
CAboutScreen::CAboutScreen(CMain2DDisplay* p_display, CGDI* p_gdi, const CVSRect& p_rect)
{
	void* storage;

	m_returnState = FLOW_NONE;
	g_pCursor->SetActive(0);
	m_complete = 0;
	g_pMasterInputQueue->Attach(this, 0);
	m_display = p_display;
	m_gdi = p_gdi;
	m_size.m_width = p_rect.m_width;
	m_size.m_height = p_rect.m_height;
	p_display->AttachPalette(RES_REGISTRATION_VISOS_PALETTE);
	m_backgroundBitmap = CResBITMAP::Load(RES_REGISTRATION_VISOS_LOGO);
	m_textWindow = NULL;
	storage = operator new(sizeof(CTextManager));
	if (storage != NULL) {
		m_textManager = new (storage) CTextManager(RESOURCE_ID_COUNT, 1, 10, 0);
	}
	else {
		m_textManager = NULL;
	}
	m_textManager->LoadFont(RES_GAME_FONT3);
	m_aboutString = CResSTRING::Load(RES_REGISTRATION_FINGERPRINT);
	CResSTRING* aboutString = m_aboutString;
	if (aboutString->m_loaded != 0) {
		aboutString->m_age = 0;
	}
	else {
		aboutString->LoadData();
	}
	aboutString->m_directUseCount = aboutString->m_directUseCount + 1;
	m_aboutText = (char*) aboutString->m_data;
	m_startTime = CurrentMilliTimer();
	m_endTime = m_startTime + ABOUT_SCREEN_DISPLAY_DURATION_MS;
}

// FUNCTION: LEMBALL 0x0044b8f0
CAboutScreen::~CAboutScreen()
{
	g_pMasterInputQueue->Detach(this, 0);
	m_backgroundBitmap->UnLoad();
	m_textManager->UnLoadFont(RES_GAME_FONT3);
	if (m_textWindow != NULL) {
		delete m_textWindow;
	}
	if (m_textManager != NULL) {
		delete m_textManager;
	}
	m_aboutString->m_directUseCount = m_aboutString->m_directUseCount - 1;
	m_aboutString->UnLoad();
	g_pCursor->SetActive(1);
	g_pMogRes->CleanUpResources();
}

// FUNCTION: LEMBALL 0x0044b9e0
void CAboutScreen::Draw(const CVSRect& p_rect)
{
	if (m_gdi != NULL) {
		DrawChangedRegion();
	}
}

// FUNCTION: LEMBALL 0x0044b9f0
void CAboutScreen::DrawRegistrationText()
{
	unsigned char* key = (unsigned char*) g_szAboutWeatherManKey;
	CResFONT* font;
	int labelY;
	int index;

	font = m_textManager->GetFont(RES_GAME_FONT3);
	CVSSize sizeValue = font->GetSize(g_szRegisteredTo, TEXT_ADVANCE_X_POSITIVE);
	CVSSize& size = sizeValue;
	labelY = (int) (m_size.m_height / 2) - (int) (size.m_height / 2);
	short labelPointStorage[2];
	CVSPoint& labelPosition = *(CVSPoint*) labelPointStorage;
	{
		CVSSize advance;
		advance.m_height = 0;
		advance.m_width = 0;
		labelPosition.m_x = (short) (m_size.m_width / 2 - size.m_width / 2);
		labelPosition.m_y = (short) labelY;
		m_textManager->DrawString(m_gdi,
								  labelPosition,
								  advance,
								  RES_GAME_FONT3,
								  g_szRegisteredTo,
								  TEXT_ADVANCE_X_POSITIVE,
								  NULL);
	}
	strcpy(g_szVisosBuildBuffer, g_szVisosBuild);
	vsLtoa(0xc9, g_szVisosBuildBuffer + strlen(g_szVisosBuildBuffer), 10);
	{
		const CVSSize& measuredSize = font->GetSize(g_szVisosBuildBuffer, TEXT_ADVANCE_X_POSITIVE);
		size.m_width = measuredSize.m_width;
		size.m_height = measuredSize.m_height;
	}
	{
		CVSSize advance;
		advance.m_height = 0;
		advance.m_width = 0;
		short pointStorage[2];
		CVSPoint& position = *(CVSPoint*) pointStorage;
		position.m_x = (short) (m_size.m_width - size.m_width) / 2;
		position.m_y = (short) (m_size.m_height - size.m_height) / 2;
		position.m_y += size.m_height * 4;
		m_textManager
			->DrawString(m_gdi, position, advance, RES_GAME_FONT3, g_szVisosBuildBuffer, TEXT_ADVANCE_X_POSITIVE, NULL);
	}
	index = 0;
	while (m_aboutText[index] != '\0') {
		g_szAboutDecodeBuffer[index] = m_aboutText[index] - 1U ^ *key;
		index = index + 1;
		key = key + 1;
	}
	g_szAboutDecodeBuffer[index] = '\0';
	{
		const CVSSize& measuredSize = font->GetSize(g_szAboutDecodeBuffer, TEXT_ADVANCE_X_POSITIVE);
		size.m_width = measuredSize.m_width;
		size.m_height = measuredSize.m_height;
	}
	{
		CVSSize advance;
		advance.m_height = 0;
		advance.m_width = 0;
		CVSPoint position((short) (m_size.m_width / 2 - size.m_width / 2), (short) labelY + 0x23);
		m_textManager->DrawString(m_gdi,
								  position,
								  advance,
								  RES_GAME_FONT3,
								  g_szAboutDecodeBuffer,
								  TEXT_ADVANCE_X_POSITIVE,
								  NULL);
	}
	m_textManager->ResetPrimitives();
}

// FUNCTION: LEMBALL 0x0044bc50
void CAboutScreen::OnSize(const CVSRect& p_rect)
{
	CVSPoint position;
	int textY;
	int textX;

	m_size.m_width = p_rect.m_width;
	m_size.m_height = p_rect.m_height;
	textY = (int) p_rect.m_height - 0x20;
	textX = ((int) p_rect.m_width - 0x60) / 2;
	if (m_textWindow != NULL) {
		position.m_x = (short) textX;
		position.m_y = (short) textY;
		m_textWindow->Move(position);
	}
}

// FUNCTION: LEMBALL 0x0044bca0
void CAboutScreen::DrawChangedRegion()
{
	CChangeList* changes;
	ChangeListItem* item;
	int itemCount;
	int index;
	CResBITMAP* bitmap;

	changes = m_gdi->m_renderTarget->GetChangeList();
	itemCount = changes->GetNumItems();
	index = changes->GetDrawMark();
	if (index < itemCount) {
		item = changes->GetNItem(index);
		CVSRect area = *(CVSRect*) item;
		index = index + 1;
		while (index < itemCount) {
			item = changes->GetNItem(index);
			area.ExpandToInclude(*(CVSRect*) item);
			index = index + 1;
		}
		if (0 < (int) area.m_height * (int) area.m_width) {
			if (m_size.m_width < area.m_width) {
				area.m_width = m_size.m_width;
			}
			if (m_size.m_height < area.m_height) {
				area.m_height = m_size.m_height;
			}
			m_rects[0].m_bounds.m_width = area.m_width;
			m_rects[0].m_bounds.m_height = area.m_height;
			m_rects[0].m_bounds.m_x = area.m_x;
			m_rects[0].m_bounds.m_y = area.m_y;
			m_rects[0].m_flags = 0;
			m_rects[0].Draw(m_gdi);
			bitmap = m_backgroundBitmap;
			m_line.m_bounds.m_width = m_size.m_width;
			m_line.m_colour = 0;
			m_line.m_bounds.m_height = m_size.m_height;
			m_line.m_bounds.m_x = 0;
			m_line.m_bounds.m_y = 0;
			m_line.Draw(m_gdi);
			int centreedY = ((int) m_size.m_height - (int) bitmap->m_y) / 2;
			m_bitmap.m_x = (short) (((int) m_size.m_width - (int) bitmap->m_x) / 2);
			m_bitmap.m_y = (short) centreedY;
			m_bitmap.m_resource = m_backgroundBitmap;
			m_bitmap.m_remap = NULL;
			m_bitmap.m_flags = CBitmap::BITMAP_TRANSPARENT_ZERO;
			m_bitmap.Draw(m_gdi);
			DrawRegistrationText();
			CVSSize surfaceSize;
			surfaceSize = static_cast<CVSSize&>(m_gdi->m_renderTarget->m_windowRect);
			CVSPoint origin;
			m_rects[1].m_bounds.CVSSize::operator=(surfaceSize);
			m_rects[1].m_bounds.CVSPoint::operator=(origin);
			m_rects[1].m_flags = 0;
			m_rects[1].Draw(m_gdi);
		}
	}
	changes->Reset();
	m_gdi->AddToList(&m_drawingMark);
}

// FUNCTION: LEMBALL 0x0044be80
int CAboutScreen::ProcessMsg(Message* p_message)
{
	switch (p_message->m_type) {
	case MESSAGE_KEY_DOWN:
		return 1;
	default:
		m_processedCount = m_processedCount + 1;
		return 0;
	}
}

// FUNCTION: LEMBALL 0x0044bea0
bool CAboutScreen::QuitYet()
{
	if (m_endTime < CurrentMilliTimer()) {
		m_complete = 1;
		m_returnState = FLOW_INTRO_ANIM;
	}
	return m_complete;
}

// FUNCTION: LEMBALL 0x0044bec0
void CAboutScreen::OnDriverChange()
{
}

// FUNCTION: LEMBALL 0x0044c0b0
int CAboutScreen::GetReturnState()
{
	return m_returnState;
}

// FUNCTION: LEMBALL 0x0044c0c0
void CAboutScreen::ResetPrimitives()
{
}
