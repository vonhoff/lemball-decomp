#ifndef LEMBALL_VIEWS_PANEL_CPANELLEMMING_H
#define LEMBALL_VIEWS_PANEL_CPANELLEMMING_H

#include "../../AI/Base/ObjectTypes.h"

class CPanel;
class CPanelButton;
class CPlayerLemming;
class CPvGWnd;
struct CVsPoint;
// SIZE 0x2c
class CPanelLemming {
public:
	CPanelLemming(CPlayerLemming* p_arg0, const CVsPoint& p_arg1, CPanel* p_arg2);
	void Move(const CVsPoint& p_position);
	void UpdateStatus();
	~CPanelLemming();

	friend class CPanelButton;
	friend class CPanel;

private:
	CPvGWnd* m_window;               // 0x00
	CPanelButton* m_button;          // 0x04
	CPlayerLemming* m_lemming;       // 0x08
	CPanel* m_panel;                 // 0x0c
	unsigned int m_reserved;         // 0x10
	unsigned int m_playerIndex;      // 0x14
	eObjectType m_balloonType;       // 0x18
	unsigned int m_inventoryCount;   // 0x1c
	eObjectType m_inventoryTypes[3]; // 0x20
};

#endif
