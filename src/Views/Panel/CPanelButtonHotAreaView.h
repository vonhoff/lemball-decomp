#ifndef LEMBALL_VIEWS_PANEL_CPANELBUTTONHOTAREAVIEW_H
#define LEMBALL_VIEWS_PANEL_CPANELBUTTONHOTAREAVIEW_H

#include "../../Visos/Animation/CAnim.h"    // complete type
#include "../../Visos/Foundation/CVsRect.h" // complete type
#include "../../Visos/Graphics/CLine.h"     // complete type

class CPanelLemming;
// SIZE 0x128
class CPanelButtonHotAreaView {
private:
	unsigned int m_depressed;          // 0x74
	unsigned int m_lastDrawnDepressed; // 0x78
	unsigned int m_pressedInside;      // 0x7c
	CPanelLemming* m_lemming;          // 0x80
	CLine m_statusLine;                // 0x84
	CLine[3] m_inventoryLines;         // 0x94
	CVsRect m_statusRect;              // 0xc4
	CVsRect m_inventoryRect;           // 0xcc
	unsigned int m_lastAmmo;           // 0xd4
	unsigned int m_unavailable;        // 0xd8
	unsigned int m_alternatePlayer;    // 0xdc
	unsigned int m_lastBalloon;        // 0xe0
	unsigned int m_inventoryCount;     // 0xe4
	CAnim m_statusAnim;                // 0xe8
	CAnim m_inventoryAnim;             // 0x108
};

#endif
