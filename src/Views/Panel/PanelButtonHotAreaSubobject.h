#ifndef LEMBALL_VIEWS_PANEL_PANELBUTTONHOTAREASUBOBJECT_H
#define LEMBALL_VIEWS_PANEL_PANELBUTTONHOTAREASUBOBJECT_H

#include "../../Common.h"
#include "../../Visos/Animation/Anim.h"    // complete type
#include "../../Visos/Foundation/VsRect.h" // complete type
#include "../../Visos/Graphics/Line.h"     // complete type

// SIZE 0x128
class PanelButtonHotAreaSubobject {
private:
	unsigned int m_depressed;          // 0x74
	unsigned int m_lastDrawnDepressed; // 0x78
	unsigned int m_pressedInside;      // 0x7c
	PanelLemming* m_lemming;           // 0x80
	Line m_statusLine;                 // 0x84
	Line[3] m_inventoryLines;          // 0x94
	VsRect m_statusRect;               // 0xc4
	VsRect m_inventoryRect;            // 0xcc
	unsigned int m_lastAmmo;           // 0xd4
	unsigned int m_unavailable;        // 0xd8
	unsigned int m_alternatePlayer;    // 0xdc
	unsigned int m_lastBalloon;        // 0xe0
	unsigned int m_inventoryCount;     // 0xe4
	Anim m_statusAnim;                 // 0xe8
	Anim m_inventoryAnim;              // 0x108
};

#endif
