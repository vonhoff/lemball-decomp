#ifndef LEMBALL_VIEWS_PANEL_CPANEL_H
#define LEMBALL_VIEWS_PANEL_CPANEL_H

#include "../../Visos/Foundation/CBaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/CVsPoint.h"          // complete type

class CAI;
class C2D;
class CPanelLemming;
class CPanelPauseButton;
class CPVGWnd;
class CResANIM;
// SIZE 0x58
// VTABLE: LEMBALL 0x00497740
class CPanel : public CBaseQueueHandler {
public:
	CPanel(C2D* p_gameView);
	CVsPoint* GetPausePos(CVsPoint* p_result);
	bool MouseInPanel(const CVsPoint& p_point);
	unsigned long TranslateKey(unsigned long p_key);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~CPanel();                          // vtable+0x04
	void OnSize();
	void Process();
	void SetPause(unsigned int p_paused);
	void RefreshLemmings();
	friend class CPanelLemming;
	friend class CPanelPauseButton;
	friend class CPanelButton;

private:
	C2D* m_game;                      // 0x10
	CPVGWnd* m_window;                // 0x14
	CAI* m_ai;                        // 0x18
	CPanelLemming* m_lemmings[4];     // 0x1c
	CPanelPauseButton* m_pauseButton; // 0x2c
	CResANIM* m_resources[4];         // 0x30
	unsigned int m_reserved;          // 0x40
	CVsPoint m_buttonSize;            // 0x44
	CVsPoint m_balloonSize;           // 0x48
	CVsPoint m_pauseSize;             // 0x4c
	CVsPoint m_panelSize;             // 0x50
	CVsPoint m_panelPosition;         // 0x54
};

// SYNTHETIC: LEMBALL 0x004439f0
// CPanel::`scalar deleting destructor'

#endif
