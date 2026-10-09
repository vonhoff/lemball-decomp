#ifndef LEMBALL_VIEWS_PAUSE_CPAUSEWINDOW_H
#define LEMBALL_VIEWS_PAUSE_CPAUSEWINDOW_H

#include "../../Engine/Animation/CAnim.h"
#include "Engine/Graphics/Primitives/CSolidRect.h"
#include "Engine/Input/CHotAreaHandler.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"
#include "Engine/Queues/CBaseQueueHandler.h"
#include "Engine/Queues/tagMESSAGE.h"
#include "Engine/Text/CTextManager.h"
#include "Platform/Windows/Windowing/CGWnd.h"

// SIZE 0x04
// VTABLE: LEMBALL 0x00497750
class CPauseVramHandler {
public:
	virtual void FreeVram() = 0; // vtable+0x00
};

class CBaseRemap;
class CPVGWnd;
class CReceiveWindowState;
class CResANIM;
class CResFONT;

enum ePauseWindowMessages {
	PAUSE_MSG_PAUSED = 0,
	PAUSE_MSG_PLEASE_WAIT = 1,
	PAUSE_MSG_LOADING = 2,
	PAUSE_MSG_ARE_YOU_SURE = 3,
	PAUSE_MSG_CONNECTION_LOST = 4,
	PAUSE_MSG_NONE = 5
};

enum ePauseOptionSelection {
	PAUSE_OPTION_RESUME = 2,
	PAUSE_OPTION_RESTART = 3,
	PAUSE_OPTION_QUIT = 4
};

enum ePauseConfirmationSelection {
	PAUSE_CONFIRM_YES = 2
};

// SIZE 0x20c
// VTABLE: LEMBALL 0x00497798 CGWnd
// VTABLE: LEMBALL 0x00497788 CBaseQueueHandler
// VTABLE: LEMBALL 0x00497780 CPauseVramHandler
// VTABLE: LEMBALL 0x00497758 CHotAreaHandler
class CPauseWindow : public CGWnd,
					 public CTextManager,
					 public CBaseQueueHandler,
					 public CPauseVramHandler,
					 public CHotAreaHandler {
public:
	CBaseRemap* Remap(int p_item);
	CPauseWindow(CReceiveWindowState* p_receiverState, CPVGWnd* p_parentWindow, ePauseWindowMessages p_pauseMessage);
	CVSRect CalculateWindow();
	virtual int ProcessMsg(tagMESSAGE* p_message);                                  // vtable+0x08
	virtual void OnButtonDown(const CVSPoint& p_point, BUTTON_FLAGS p_flags);       // vtable+0x04
	virtual void FreeVram();                                                        // vtable+0x00
	virtual void OnButtonUp(const CVSPoint& p_point, BUTTON_FLAGS p_flags);         // vtable+0x08
	virtual void OnDriverChange();                                                  // vtable+0x5c
	virtual void OnExternalButtonUp(const CVSPoint& p_point, BUTTON_FLAGS p_flags); // vtable+0x0c
	virtual void OnInside(const CVSPoint& p_point);                                 // vtable+0x18
	virtual void OnPaint(const CVSRect& p_rect);                                    // vtable+0xa8
	void CreateTheWindow(const CVSRect& p_rect);
	void Initialise();
	void Load();
	void RegisterRemaps();
	void Restart();
	void UnLoad();
	void UnRegisterRemaps();
	~CPauseWindow();

private:
	char** m_menuLabels;                   // 0x100
	ePauseWindowMessages m_pauseMessage;   // 0x104
	unsigned int m_cursorState;            // 0x108
	unsigned int m_lowResolution;          // 0x10c
	CReceiveWindowState* m_receiverState;  // 0x110
	CPVGWnd* m_parentWindow;               // 0x114
	int m_selection;                       // 0x118
	int m_unavailableItems;                // 0x11c
	int m_menuItemCount;                   // 0x120
	int m_minimumSelection;                // 0x124
	int m_initialSelection;                // 0x128
	int m_verticalTextOffset;              // 0x12c
	CVSSize m_borderTiles;                 // 0x130
	int m_borderAnimCount;                 // 0x134
	CSolidRect m_borderLine[1];            // 0x138
	CVSPoint m_windowPadding;              // 0x148
	CVSPoint m_textSpacing;                // 0x14c
	CVSPoint m_borderPadding;              // 0x150
	CResANIM* m_horizontalBorderAnim;      // 0x154
	CResANIM* m_verticalBorderAnim;        // 0x158
	CAnim m_cornerAnims[4];                // 0x15c
	CAnim* m_borderAnims;                  // 0x1dc
	CBaseRemap* m_remaps[4];               // 0x1e0
	CResFONT* m_font;                      // 0x1f0
	CPauseVramHandler* m_vramSurface;      // 0x1f4
	CVSPoint* m_menuItemRects;             // 0x1f8
	unsigned int m_horizontalBorderAnimId; // 0x1fc
	unsigned int m_verticalBorderAnimId;   // 0x200
	unsigned int m_fontId;                 // 0x204
	unsigned int m_loaded;                 // 0x208
};

// SYNTHETIC: LEMBALL 0x00445350
// CPauseWindow::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00445390
// CPauseWindow::`vector deleting destructor'

#endif
