#ifndef LEMBALL_VIEWS_DISPLAY_CMAIN2DDISPLAY_H
#define LEMBALL_VIEWS_DISPLAY_CMAIN2DDISPLAY_H

#include "../../Frontend/Base/FlowProcesses.h"
#include "../../Visos/Foundation/CBaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/CVsRect.h"           // complete type
#include "../../Visos/Graphics/CDrawingMark.h"
#include "../../Visos/Graphics/CGWnd.h" // complete type

class CGame;
// SIZE 0xe4
// VTABLE: LEMBALL 0x00496bb8 CGWnd
// VTABLE: LEMBALL 0x00496ba8 CBaseQueueHandler
class CMain2DDisplay : public CGWnd, public CBaseQueueHandler {
public:
	CMain2DDisplay(CGame* p_game);
	CVsRect GetUseRect(int p_x, int p_y);
	virtual bool GetMenu(int& p_count, MenuList*** p_menu); // vtable+0x0c
	bool IsWindowValid();
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	int GetReturnState();
	int QuitYet();
	unsigned int GetStyle();
	virtual void OnDriverChange(); // vtable+0x5c
	virtual void OnMove();         // vtable+0x58
	virtual void OnRestore();      // vtable+0x54
	void KillDrawer(eFlowProcesses p_flow);
	virtual void OnCreate(); // vtable+0x3c
	virtual void OnDestroy();
	virtual void OnPaint(const CVsRect& p_rect);
	virtual void OnSize();
	virtual void OnZoom(int p_zoom);
	void Process();
	void RefreshView();
	void StatusUpdate(eFlowProcesses p_flow);
	void ToggleResolution();
	~CMain2DDisplay();

	friend class CNetworkOptionsDrawer;
	friend class CGame;
	friend class CBaseFrontendProcess;
	friend class CMainOptions1Drawer;
	friend class CBaseFrontendDrawer;
	friend class CFrontendResourceLoader;
	friend class CCDLoadAnim;

private:
	unsigned int m_windowReady;    // 0xa0
	void* m_loadingDraw;           // 0xa4
	void* m_cursorResource;        // 0xa8
	void* m_gamePalette;           // 0xac
	void* m_titlePalette;          // 0xb0
	void* m_map;                   // 0xb4
	unsigned int m_frameCount;     // 0xb8
	unsigned int m_drawerClosing;  // 0xbc
	CGame* m_game;                 // 0xc0
	void* m_ai;                    // 0xc4
	unsigned int m_quitRequested;  // 0xc8
	eFlowProcesses m_currentFlow;  // 0xcc
	CDrawingMark m_drawingMark;    // 0xd0
	class CDrawer* m_drawer;       // 0xd4
	unsigned int m_resolutionMode; // 0xd8
	short m_lowWidth;              // 0xdc
	short m_lowHeight;             // 0xde
	short m_highWidth;             // 0xe0
	short m_highHeight;            // 0xe2
};

// SYNTHETIC: LEMBALL 0x004322e0
// CMain2DDisplay::`scalar deleting destructor'

#endif
