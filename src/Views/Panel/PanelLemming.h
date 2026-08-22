#ifndef LEMBALL_VIEWS_PANEL_PANELLEMMING_H
#define LEMBALL_VIEWS_PANEL_PANELLEMMING_H

#include "../../Common.h"

// SIZE 0x2c
class PanelLemming {
public:
	PanelLemming(PlayerLemming* p_arg0, const VsPoint& p_arg1, Panel* p_arg2);
	void Move(const VsPoint& p_position);
	void UpdateStatus();
	~PanelLemming();

private:
	PvGWnd* m_window;                // 0x00
	PanelButton* m_button;           // 0x04
	PlayerLemming* m_lemming;        // 0x08
	Panel* m_panel;                  // 0x0c
	unsigned int m_reserved;         // 0x10
	unsigned int m_playerIndex;      // 0x14
	eObjectType m_balloonType;       // 0x18
	unsigned int m_inventoryCount;   // 0x1c
	eObjectType m_inventoryTypes[3]; // 0x20
};

#endif
