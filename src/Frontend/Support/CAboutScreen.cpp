#include "CAboutScreen.h"

#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/CChangeList.h"
#include "../../Visos/Foundation/CTextManager.h"
#include "../../Visos/Foundation/VsString.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/CCursor.h"
#include "../../Visos/Graphics/CGWnd.h"
#include "../../Visos/Graphics/CGdi.h"
#include "../../Visos/Graphics/CHotAreaHandler.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Resources/CMogRes.h"
#include "../../Visos/Resources/CResBitmap.h"
#include "../../Visos/Resources/CResFont.h"
#include "../../Visos/Resources/CResString.h"
#include "../../Visos/Resources/Manifest.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CBitmapRes.h"
#include "Visos/Graphics/CDrawingMark.h"
#include "Visos/Graphics/CLine.h"
#include "Visos/Graphics/CPvGWnd.h"
#include "Visos/Graphics/CSolidRect.h"

#include <new.h>
#include <string.h>

class ChangeListItem;

// GLOBAL: LEMBALL 0x0049f9e4
char g_szRegisteredTo[] = "Registered to";

// GLOBAL: LEMBALL 0x0049f9f4
char g_szAboutDecodeBuffer[24] = "01234567890123456789";

// GLOBAL: LEMBALL 0x0049fa0c
char g_szAboutWeatherManKey[] = "John Ketley is a Weatherman, and so is Michael Fish";

// GLOBAL: LEMBALL 0x0049fa40
char g_szVisosBuild[] = "ViSOS Build ";

struct AboutTextWindowBase : public CGWnd, public CHotAreaHandler {};

// GLOBAL: LEMBALL 0x004a78d0
char g_szVisosBuildBuffer[80];

// FUNCTION: LEMBALL 0x0044b750
CAboutScreen::CAboutScreen(CMain2DDisplay* p_display, CGdi* p_gdi, const CVsRect& p_rect)
{
	void* storage;

	m_returnState = 0;
	g_pCursor->SetActive(0);
	m_complete = 0;
	g_pMasterInputQueue->Attach(this, 0);
	m_display = p_display;
	m_gdi = p_gdi;
	m_size.m_width = p_rect.m_width;
	m_size.m_height = p_rect.m_height;
	p_display->AttachPalette(RES_REGISTRATION_VISOS_PALETTE);
	m_backgroundBitmap = CResBitmap::Load(RES_REGISTRATION_VISOS_LOGO);
	m_textWindow = 0;
	storage = operator new(0x24);
	if (storage != 0) {
		m_textManager = new (storage) CTextManager(0x2b6, 1, 10, 0);
	}
	else {
		m_textManager = 0;
	}
	m_textManager->LoadFont(RES_GAME_FONT3);
	m_aboutString = CResString::Load(RES_REGISTRATION_FINGERPRINT);
	CResString* aboutString = m_aboutString;
	if (aboutString->m_loaded != 0) {
		aboutString->m_age = 0;
	}
	else {
		aboutString->LoadData();
	}
	aboutString->m_directUseCount = aboutString->m_directUseCount + 1;
	m_aboutText = (char*) aboutString->m_data;
	m_startTime = CurrentMilliTimer();
	m_endTime = m_startTime + 5000;
}

// FUNCTION: LEMBALL 0x0044b8f0
CAboutScreen::~CAboutScreen()
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
void CAboutScreen::Draw(const CVsRect& p_rect)
{
	if (m_gdi != 0) {
		DrawChangedRegion();
	}
}

// FUNCTION: LEMBALL 0x0044b9f0
void CAboutScreen::DrawRegistrationText()
{
	unsigned char* key = (unsigned char*) g_szAboutWeatherManKey;
	CResFont* font;
	CVsSize size;
	int labelY;
	int index;

	font = m_textManager->GetFont(RES_GAME_FONT3);
	font->GetSize(&size, g_szRegisteredTo, 0x20);
	labelY = (int) (m_size.m_height / 2) - (int) (size.m_height / 2);
	{
		CVsSize advance;
		advance.m_height = 0;
		advance.m_width = 0;
		CVsPoint position((short) (m_size.m_width / 2 - size.m_width / 2), (short) labelY);
		m_textManager->DrawString(m_gdi, position, advance, RES_GAME_FONT3, g_szRegisteredTo, 0x20, 0);
	}
	strcpy(g_szVisosBuildBuffer, g_szVisosBuild);
	VsLtoa(0xc9, g_szVisosBuildBuffer + strlen(g_szVisosBuildBuffer), 10);
	{
		CVsSize* measuredSize = font->GetSize(&size, g_szVisosBuildBuffer, 0x20);
		size.m_width = measuredSize->m_width;
		size.m_height = measuredSize->m_height;
	}
	{
		CVsSize advance;
		advance.m_height = 0;
		advance.m_width = 0;
		CVsPoint position((short) (m_size.m_width - size.m_width) / 2, (short) (m_size.m_height - size.m_height) / 2);
		position.m_y += size.m_height * 4;
		m_textManager->DrawString(m_gdi, position, advance, RES_GAME_FONT3, g_szVisosBuildBuffer, 0x20, 0);
	}
	index = 0;
	while (m_aboutText[index] != '\0') {
		g_szAboutDecodeBuffer[index] = m_aboutText[index] - 1U ^ *key;
		index = index + 1;
		key = key + 1;
	}
	g_szAboutDecodeBuffer[index] = '\0';
	{
		CVsSize* measuredSize = font->GetSize(&size, g_szAboutDecodeBuffer, 0x20);
		size.m_width = measuredSize->m_width;
		size.m_height = measuredSize->m_height;
	}
	{
		CVsSize advance;
		advance.m_height = 0;
		advance.m_width = 0;
		CVsPoint position((short) (m_size.m_width / 2 - size.m_width / 2), (short) labelY + 0x23);
		m_textManager->DrawString(m_gdi, position, advance, RES_GAME_FONT3, g_szAboutDecodeBuffer, 0x20, 0);
	}
	m_textManager->ResetPrimitives();
}

// FUNCTION: LEMBALL 0x0044bc50
void CAboutScreen::OnSize(const CVsRect& p_rect)
{
	CVsPoint position;
	int textY;
	int textX;

	m_size.m_width = p_rect.m_width;
	m_size.m_height = p_rect.m_height;
	textY = (int) p_rect.m_height - 0x20;
	textX = ((int) p_rect.m_width - 0x60) / 2;
	if (m_textWindow != 0) {
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
	CVsRect area;
	CResBitmap* bitmap;

	changes = m_gdi->m_renderTarget->GetChangeList();
	itemCount = changes->GetNumItems();
	index = changes->GetDrawMark();
	if (index < itemCount) {
		item = changes->GetNItem(index);
		area = *(CVsRect*) item;
		index = index + 1;
		while (index < itemCount) {
			item = changes->GetNItem(index);
			area.ExpandToInclude(*(CVsRect*) item);
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
			m_rects[0].m_color = 0;
			m_rects[0].Draw(m_gdi);
			bitmap = m_backgroundBitmap;
			m_line.m_x1 = m_size.m_width;
			m_line.m_color = 0;
			m_line.m_y1 = m_size.m_height;
			m_line.m_x2 = 0;
			m_line.m_y2 = 0;
			m_line.Draw(m_gdi);
			m_bitmap.m_y = (short) (((int) m_size.m_height - (int) (short) bitmap->m_y) / 2);
			m_bitmap.m_x = (short) (((int) m_size.m_width - (int) (short) bitmap->m_x) / 2);
			m_bitmap.m_resource = m_backgroundBitmap;
			m_bitmap.m_remap = 0;
			m_bitmap.m_flags = 0x800;
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
int CAboutScreen::ProcessMsg(Message* p_message)
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
bool CAboutScreen::QuitYet()
{
	if (m_endTime < CurrentMilliTimer()) {
		m_complete = 1;
		m_returnState = 1;
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
