#ifndef LEMBALL_VIEWS_PANEL_PANEL_H
#define LEMBALL_VIEWS_PANEL_PANEL_H

#include "../../Common.h"
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/VsPoint.h"          // complete type

// SIZE 0x58
// VTABLE: LEMBALL 0x00497740
class Panel : public BaseQueueHandler {
public:
	Panel(C2D* p_arg0);
	VsPoint GetPausePos();
	bool MouseInPanel(const VsPoint& p_point);
	unsigned long TranslateKey(unsigned long p_key);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~Panel();                           // vtable+0x04
	void OnSize();
	void Process();
	void SetPause(unsigned char p_paused);

private:
	C2D* m_game;                     // 0x10
	PvGWnd* m_window;                // 0x14
	void* m_lemmingManager;          // 0x18
	PanelLemming* m_lemmings[4];     // 0x1c
	PanelPauseButton* m_pauseButton; // 0x2c
	ResAnim* m_resources[4];         // 0x30
	unsigned int m_reserved;         // 0x40
	VsPoint m_buttonSize;            // 0x44
	VsPoint m_balloonSize;           // 0x48
	VsPoint m_pauseSize;             // 0x4c
	VsPoint m_panelSize;             // 0x50
	VsPoint m_panelPosition;         // 0x54
};

#endif
