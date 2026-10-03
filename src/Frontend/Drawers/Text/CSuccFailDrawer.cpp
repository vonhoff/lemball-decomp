#include "../CSuccFailDrawer.h"

#include "../../../Control/Game/CGameStatus.h"
#include "../../../Visos/Foundation/CTextManager.h"

extern "C" unsigned long __stdcall timeGetTime(void);

#include "../../../Network/Game/CNetworkManager.h"
#include "../../../Network/Messages/CNetworkGameMessage.h"
#include "../../../Visos/Network/CConnect.h"
#include "../../../Visos/Resources/CResFONT.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Support/CoordPair.h"
#include "Frontend/Windows/CSuccFailAnimWnd.h"
#include "Visos/Foundation/CVSPoint.h"
#include "Visos/Foundation/CVSSize.h"

#include <string.h>

class CGWnd;

#pragma intrinsic(strcpy, strlen)

extern char* g_apSuccFailSingleWin[8];
extern char* g_apSuccFailNetWin[8];
extern char* g_apSuccFailSingleLose[8];
extern char* g_apSuccFailNetLose[8];
extern char g_szPasswordLabel[];

// FUNCTION: LEMBALL 0x00450160
void CSuccFailDrawer::CalculateText()
{
	CResFONT* font;
	char* format;
	char* hash;

	font = m_textManager->GetFont(m_chalkFontId);
	char** messages;
	if (m_networkMode != 0) {
		messages = g_apSuccFailNetWin;
		if (m_success == 0) {
			messages = g_apSuccFailNetLose;
		}
	}
	else {
		messages = g_apSuccFailSingleWin;
		if (m_success == 0) {
			messages = g_apSuccFailSingleLose;
		}
	}
	format = messages[g_pGameStatus->m_skillState];
	hash = strchr(format, '#');
	if (hash != NULL) {
		int prefixLen = hash - format;
		if (prefixLen != 0) {
			strncpy(m_message, format, prefixLen);
		}
		m_message[prefixLen] = 0;
		if (g_pActiveConnection != NULL) {
			CNetworkGameMessage* opponentMsg = g_pNetworkManager->GetGameMessage(g_pActiveConnection);
			strcat(m_message, opponentMsg->m_gameName);
		}
		strcat(m_message, hash + 1);
	}
	else {
		strcpy(m_message, format);
	}

	{
		bool done = false;
		short layoutMinX = (short) m_layout->m_messagePosition.m_x;
		short layoutY = (short) m_layout->m_messagePosition.m_y;
		m_firstLine = m_message;
		m_secondLine = NULL;
		short lineX;
		CVSSize measuredSize;
		do {
			const CVSSize& textSize = font->GetSize(m_firstLine, 0x20);
			measuredSize.m_height = textSize.m_height;
			measuredSize.m_width = textSize.m_width;
			lineX = (short) m_layout->m_frameStart.m_x +
					(short) ((m_layout->m_frameEnd.m_x - (int) measuredSize.m_width) / 2);
			char* prevBreak = (m_secondLine != NULL) ? (m_secondLine - 1) : NULL;
			if (lineX < layoutMinX) {
				char* space = strrchr(m_firstLine, ' ');
				m_secondLine = space;
				*space = 0;
				m_secondLine = m_secondLine + 1;
				if (prevBreak != NULL) {
					*prevBreak = ' ';
				}
			}
			else {
				done = true;
			}
		} while (!done);

		m_firstLinePos.m_x = lineX;
		m_firstLinePos.m_y = layoutY;
		if (m_secondLine == NULL) {
			m_firstLinePos.m_y = layoutY + measuredSize.m_height / 2;
		}
		else {
			layoutY = layoutY + measuredSize.m_height;
			const CVSSize& textSize = font->GetSize(m_secondLine, 0x20);
			m_secondLinePos.m_x =
				(short) m_layout->m_frameStart.m_x + (short) ((m_layout->m_frameEnd.m_x - (int) textSize.m_width) / 2);
			m_secondLinePos.m_y = layoutY;
		}
	}
	short passwordLabelY;
	{
		const CVSSize& textSize = font->GetSize(g_szPasswordLabel, 0x20);
		short labelHeight = textSize.m_height;
		int labelWidth = textSize.m_width;
		passwordLabelY = (short) m_layout->m_passwordLabelPosition.m_y;
		m_passwordLabelPos.m_x =
			(short) m_layout->m_frameStart.m_x + (short) ((m_layout->m_frameEnd.m_x - labelWidth) / 2);
		m_passwordLabelPos.m_y = passwordLabelY;
		passwordLabelY += labelHeight;
	}

	{
		const CVSSize& passwordSize = font->GetSize(m_password, 0x20);
		int labelWidth = passwordSize.m_width;
		m_passwordPos.m_x = (short) m_layout->m_frameStart.m_x + (short) ((m_layout->m_frameEnd.m_x - labelWidth) / 2);
		m_passwordPos.m_y = passwordLabelY;
	}
}
