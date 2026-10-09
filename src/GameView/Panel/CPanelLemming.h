#ifndef LEMBALL_VIEWS_PANEL_CPANELLEMMING_H
#define LEMBALL_VIEWS_PANEL_CPANELLEMMING_H

enum ePanelRemapIndex {
	PANEL_REMAP_INDEX_NONE = -1,
	PANEL_REMAP_INDEX_0 = 0,
	PANEL_REMAP_INDEX_1 = 1,
	PANEL_REMAP_INDEX_3 = 3,
	PANEL_REMAP_INDEX_4 = 4
};

class CPanel;
class CPanelButton;
class CPlayerLemming;
class CPVGWnd;
struct CVSPoint;
// SIZE 0x2c
class CPanelLemming {
public:
	CPanelLemming(CPlayerLemming* p_lemming, const CVSPoint& p_position, CPanel* p_panel);
	void Move(const CVSPoint& p_position);
	void UpdateStatus();
	~CPanelLemming();

	friend class CPanelButton;
	friend class CPanel;

private:
	CPVGWnd* m_window;                     // 0x00
	CPanelButton* m_button;                // 0x04
	CPlayerLemming* m_lemming;             // 0x08
	CPanel* m_panel;                       // 0x0c
	unsigned int m_reserved;               // 0x10
	unsigned int m_playerIndex;            // 0x14
	ePanelRemapIndex m_balloonRemap;       // 0x18
	unsigned int m_inventoryCount;         // 0x1c
	ePanelRemapIndex m_inventoryRemaps[3]; // 0x20
};

#endif
