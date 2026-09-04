#include "TargetAboutScreen.h"

#include "../../Views/Display/Main2DDisplay.h"
#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/ChangeList.h"
#include "../../Visos/Foundation/TextManager.h"
#include "../../Visos/Foundation/VsString.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/Cursor.h"
#include "../../Visos/Graphics/GWnd.h"
#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/HotAreaHandler.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Resources/MogRes.h"
#include "../../Visos/Resources/ResBitmap.h"
#include "../../Visos/Resources/ResFont.h"
#include "../../Visos/Resources/ResString.h"

#include <new.h>
#include <string.h>

// GLOBAL: LEMBALL 0x0049f9e4
char g_szRegisteredTo[] = "Registered to";

// GLOBAL: LEMBALL 0x0049f9f4
char g_szAboutDecodeBuffer[24] = "01234567890123456789";

// GLOBAL: LEMBALL 0x0049fa0c
char g_szAboutWeatherManKey[] = "John Ketley is a Weatherman, and so is Michael Fish";

// GLOBAL: LEMBALL 0x0049fa40
char g_szVisosBuild[] = "ViSOS Build ";

struct AboutTextWindowBase : public GWnd, public HotAreaHandler {};

// GLOBAL: LEMBALL 0x004a78d0
char g_szVisosBuildBuffer[80];

// FUNCTION: LEMBALL 0x0044b750
TargetAboutScreen::TargetAboutScreen(Main2DDisplay* p_display, Gdi* p_gdi, const VsRect& p_rect)
{
	void* storage;

	m_returnState = 0;
	g_pCursor->SetActive(0);
	m_complete = 0;
	g_pMasterInputQueue->Attach(this, 0);
	m_display = p_display;
	m_gdi = p_gdi;
	m_width = p_rect.m_width;
	m_height = p_rect.m_height;
	p_display->AttachPalette(RES_REGISTRATION_VISOS_PALETTE);
	m_backgroundBitmap = ResBitmap::Load(RES_REGISTRATION_VISOS_LOGO);
	m_textWindow = 0;
	storage = operator new(0x24);
	if (storage == 0) {
		m_textManager = 0;
	}
	else {
		m_textManager = new (storage) TextManager(0x2b6, 1, 10, 0);
	}
	m_textManager->LoadFont(RES_GAME_FONT3);
	m_aboutString = ResString::Load(RES_REGISTRATION_FINGERPRINT);
	if (m_aboutString->m_loaded == 0) {
		m_aboutString->LoadData();
	}
	else {
		m_aboutString->m_age = 0;
	}
	m_aboutString->m_directUseCount = m_aboutString->m_directUseCount + 1;
	m_aboutText = (char*) m_aboutString->m_data;
	m_startTime = CurrentMilliTimer();
	m_endTime = m_startTime + 5000;
}

// FUNCTION: LEMBALL 0x0044b8f0
TargetAboutScreen::~TargetAboutScreen()
{
	g_pMasterInputQueue->Detach(this, 0);
	m_backgroundBitmap->UnLoad();
	m_textManager->UnLoadFont(RES_GAME_FONT3);
	if (m_textWindow != 0) {
		delete (AboutTextWindowBase*) m_textWindow;
	}
	if (m_textManager != 0) {
		delete m_textManager;
	}
	m_aboutString->m_directUseCount = m_aboutString->m_directUseCount - 1;
	m_aboutString->UnLoad();
	g_pCursor->SetActive(1);
	g_pMogRes->CleanUpResources();
}

// FUNCTION: LEMBALL 0x0044b9e0
void TargetAboutScreen::Draw(const VsRect& p_rect)
{
	if (m_gdi != 0) {
		DrawChangedRegion();
	}
}

// FUNCTION: LEMBALL 0x0044b9f0
void TargetAboutScreen::DrawRegistrationText()
{
	ResFont* font;
	unsigned char* key;
	VsPoint labelSize;
	VsPoint buildSize;
	VsPoint nameSize;
	VsPoint position;
	VsSize advance;
	int labelY;
	int index;
	char ch;

	key = (unsigned char*) g_szAboutWeatherManKey;
	font = m_textManager->GetFont(RES_GAME_FONT3);
	labelSize = font->GetSize(g_szRegisteredTo, 0x20);
	advance.m_height = 0;
	labelY = (int) (m_height / 2) - (int) (labelSize.m_y / 2);
	advance.m_width = 0;
	position.m_x = m_width / 2 - labelSize.m_x / 2;
	position.m_y = (short) labelY;
	m_textManager->DrawString(m_gdi, position, advance, RES_GAME_FONT3, g_szRegisteredTo, 0x20, 0);
	strcpy(g_szVisosBuildBuffer, g_szVisosBuild);
	VsLtoa(0xc9, g_szVisosBuildBuffer + strlen(g_szVisosBuildBuffer), 10);
	buildSize = font->GetSize(g_szVisosBuildBuffer, 0x20);
	advance.m_height = 0;
	advance.m_width = 0;
	position.m_x = (short) (m_width - buildSize.m_x) / 2;
	position.m_y = (short) (m_height - buildSize.m_y) / 2 + buildSize.m_y * 4;
	m_textManager->DrawString(m_gdi, position, advance, RES_GAME_FONT3, g_szVisosBuildBuffer, 0x20, 0);
	ch = *m_aboutText;
	index = 0;
	while (ch != '\0') {
		g_szAboutDecodeBuffer[index] = m_aboutText[index] - 1U ^ *key;
		ch = m_aboutText[index + 1];
		index = index + 1;
		key = key + 1;
	}
	g_szAboutDecodeBuffer[index] = '\0';
	nameSize = font->GetSize(g_szAboutDecodeBuffer, 0x20);
	advance.m_height = 0;
	advance.m_width = 0;
	position.m_x = m_width / 2 - nameSize.m_x / 2;
	position.m_y = (short) labelY + 0x23;
	m_textManager->DrawString(m_gdi, position, advance, RES_GAME_FONT3, g_szAboutDecodeBuffer, 0x20, 0);
	m_textManager->ResetPrimitives();
}

// FUNCTION: LEMBALL 0x0044bc50
void TargetAboutScreen::OnSize(const VsRect& p_rect)
{
	VsPoint position;
	int textY;
	int textX;

	m_width = p_rect.m_width;
	m_height = p_rect.m_height;
	textY = (int) p_rect.m_height - 0x20;
	textX = ((int) p_rect.m_width - 0x60) / 2;
	if (m_textWindow != 0) {
		position.m_x = (short) textX;
		position.m_y = (short) textY;
		m_textWindow->Move(position);
	}
}

// FUNCTION: LEMBALL 0x0044bca0
void TargetAboutScreen::DrawChangedRegion()
{
	ChangeList* changes;
	ChangeListItem* item;
	int itemCount;
	int index;
	VsRect area;
	ResBitmap* bitmap;

	changes = m_gdi->m_renderTarget->GetChangeList();
	itemCount = changes->GetNumItems();
	index = changes->GetDrawMark();
	if (index < itemCount) {
		item = changes->GetNItem(index);
		area.m_width = item->width;
		area.m_height = item->height;
		if (item == 0) {
			area.m_x = *(short*) 0;
			area.m_y = ((short*) 0)[1];
		}
		else {
			area.m_x = item->x;
			area.m_y = item->y;
		}
		index = index + 1;
		while (index < itemCount) {
			item = changes->GetNItem(index);
			area.ExpandToInclude(*(VsRect*) item);
			index = index + 1;
		}
		if (0 < (int) area.m_height * (int) area.m_width) {
			if (m_width < area.m_width) {
				area.m_width = m_width;
			}
			if (m_height < area.m_height) {
				area.m_height = m_height;
			}
			m_rects[0].m_left = area.m_width;
			m_rects[0].m_top = area.m_height;
			m_rects[0].m_right = area.m_x;
			m_rects[0].m_bottom = area.m_y;
			m_rects[0].m_color = 0;
			m_rects[0].Draw(m_gdi);
			bitmap = m_backgroundBitmap;
			m_line.m_x1 = m_width;
			m_line.m_color = 0;
			m_line.m_y1 = m_height;
			m_line.m_x2 = 0;
			m_line.m_y2 = 0;
			m_line.Draw(m_gdi);
			m_bitmap.m_y = (short) (((int) m_height - (int) (short) bitmap->m_y) / 2);
			m_bitmap.m_x = (short) (((int) m_width - (int) (short) bitmap->m_x) / 2);
			m_bitmap.m_resource = m_backgroundBitmap;
			m_bitmap.m_remap = 0;
			m_bitmap.m_flags = 8;
			m_bitmap.Draw(m_gdi);
			DrawRegistrationText();
			m_rects[1].m_color = 0;
			m_rects[1].Draw(m_gdi);
		}
	}
	changes->Reset();
	m_gdi->AddToList(&m_drawingMark);
}

// FUNCTION: LEMBALL 0x0044be80
int TargetAboutScreen::ProcessMsg(Message* p_message)
{
	switch (p_message->type) {
	case 4:
		return 1;
	default:
		m_processedCount = m_processedCount + 1;
		return 0;
	}
}

// FUNCTION: LEMBALL 0x0044bea0
bool TargetAboutScreen::QuitYet()
{
	if (m_endTime < CurrentMilliTimer()) {
		m_complete = 1;
		m_returnState = 1;
	}
	return m_complete;
}

// FUNCTION: LEMBALL 0x0044c0b0
int TargetAboutScreen::GetReturnState()
{
	return m_returnState;
}

// FUNCTION: LEMBALL 0x0044c0c0
void TargetAboutScreen::ResetPrimitives()
{
}

// FUNCTION: LEMBALL 0x0044c100
void VsRect::ExpandToInclude(const VsRect& p_rect)
{
	if ((int) p_rect.m_width * (int) p_rect.m_height != 0) {
		if (p_rect.m_x < m_x) {
			m_width = m_width + (m_x - p_rect.m_x);
			m_x = p_rect.m_x;
		}
		if ((short) (m_width + m_x) < (short) (p_rect.m_width + p_rect.m_x)) {
			m_width = (p_rect.m_x - m_x) + p_rect.m_width;
		}
		if (p_rect.m_y < m_y) {
			m_height = m_height + (m_y - p_rect.m_y);
			m_y = p_rect.m_y;
		}
		if ((short) (m_height + m_y) < (short) (p_rect.m_y + p_rect.m_height)) {
			m_height = (p_rect.m_height - m_y) + p_rect.m_y;
		}
	}
}
