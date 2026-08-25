#ifndef LEMBALL_VIEWS_DISPLAY_MAIN2DDISPLAY_H
#define LEMBALL_VIEWS_DISPLAY_MAIN2DDISPLAY_H

#include "../../Common.h"
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/VsRect.h"           // complete type

// SIZE 0xe4
// VTABLE: LEMBALL 0x00496bb8
class Main2DDisplay {
public:
	Main2DDisplay(Game* p_arg0);
	VsRect GetUseRect(int p_x, int p_y);
	bool GetMenu(int& p_count, MenuList*** p_menu);
	bool IsWindowValid();
	bool ProcessMsg(Message* p_message);
	unsigned char GetReturnState();
	unsigned char QuitYet();
	unsigned int GetStyle();
	virtual void OnDriverChange(); // vtable+0x5c
	virtual void OnMove();         // vtable+0x58
	virtual void OnRestore();      // vtable+0x54
	void KillDrawer(eFlowProcesses p_flow);
	void OnCreate();
	void OnDestroy();
	void OnPaint(const VsRect& p_rect);
	void OnSize();
	void OnZoom(int p_zoom);
	void Process();
	void RefreshView();
	void StatusUpdate(eFlowProcesses p_flow);
	void ToggleResolution();
	~Main2DDisplay();

	friend class NetworkOptionsDrawer;

private:
	undefined m_unknown04[0x48];      // 0x04
	void* m_displayDevice;            // 0x4c
	int m_refreshInterval;            // 0x50
	unsigned int m_paletteResourceId; // 0x54
	undefined m_unknown58[0x38];      // 0x58
	BaseQueueHandler m_queueHandler;  // 0x90
	unsigned int m_windowReady;      // 0xa0
	void* m_drawPrimitive;           // 0xa4
	void* m_background;              // 0xa8
	void* m_primaryPalette;          // 0xac
	void* m_secondaryPalette;        // 0xb0
	void* m_activePalette;           // 0xb4
	unsigned int m_frameCount;       // 0xb8
	unsigned int m_drawerClosing;    // 0xbc
	Game* m_game;                    // 0xc0
	void* m_activeProcess;           // 0xc4
	void* m_pauseWindow;             // 0xc8
	eFlowProcesses m_currentFlow;    // 0xcc
	void* m_processVtable;           // 0xd0
	void* m_drawer;                  // 0xd4
	unsigned int m_resolutionMode;   // 0xd8
	short m_lowWidth;                // 0xdc
	short m_lowHeight;               // 0xde
	short m_highWidth;               // 0xe0
	short m_highHeight;              // 0xe2
};

#endif
