#ifndef LEMBALL_VIEWS_PAUSE_CPAUSEWINDOW_H
#define LEMBALL_VIEWS_PAUSE_CPAUSEWINDOW_H

#include "../../Visos/Animation/CAnim.h"
#include "../../Visos/Foundation/CBaseQueueHandler.h"
#include "../../Visos/Foundation/CTextManager.h"
#include "../../Visos/Foundation/CVsPoint.h"
#include "../../Visos/Foundation/CVsRect.h"
#include "../../Visos/Foundation/CVsSize.h"
#include "../../Visos/Graphics/CGWnd.h"
#include "../../Visos/Graphics/CHotAreaHandler.h"
#include "../../Visos/Graphics/CLine.h"
#include "CPauseVramHandler.h"

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
	CVsRect CalculateWindow();
	virtual int ProcessMsg(Message* p_message);                            // vtable+0x08
	virtual void OnButtonDown(const CVsPoint& p_point, int p_flags);       // vtable+0x04
	virtual void FreeVram();                                               // vtable+0x00
	virtual void OnButtonUp(const CVsPoint& p_point, int p_flags);         // vtable+0x08
	virtual void OnDriverChange();                                         // vtable+0x5c
	virtual void OnExternalButtonUp(const CVsPoint& p_point, int p_flags); // vtable+0x0c
	virtual void OnInside(const CVsPoint& p_point);                        // vtable+0x18
	virtual void OnPaint(const CVsRect& p_rect);                           // vtable+0xa8
	void CreateTheWindow(const CVsRect& p_rect);
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
	CVsSize m_borderTiles;                 // 0x130
	int m_borderAnimCount;                 // 0x134
	CLine m_borderLine[1];                 // 0x138
	CVsPoint m_windowPadding;              // 0x148
	CVsPoint m_textSpacing;                // 0x14c
	CVsPoint m_borderPadding;              // 0x150
	CResANIM* m_horizontalBorderAnim;      // 0x154
	CResANIM* m_verticalBorderAnim;        // 0x158
	CAnim m_cornerAnims[4];                // 0x15c
	CAnim* m_borderAnims;                  // 0x1dc
	CBaseRemap* m_remaps[4];               // 0x1e0
	CResFONT* m_font;                      // 0x1f0
	CPauseVramHandler* m_vramSurface;      // 0x1f4
	CVsPoint* m_menuItemRects;             // 0x1f8
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
