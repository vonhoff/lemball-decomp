#ifndef LEMBALL_VIEWS_PAUSE_PAUSEWINDOW_H
#define LEMBALL_VIEWS_PAUSE_PAUSEWINDOW_H

#include "../../Common.h"
#include "../../Visos/Animation/Anim.h"              // complete type
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/TextManager.h"      // complete type
#include "../../Visos/Foundation/VsPoint.h"          // complete type
#include "../../Visos/Foundation/VsRect.h"           // complete type
#include "../../Visos/Graphics/HotAreaHandler.h"     // complete type
#include "../../Visos/Graphics/Line.h"               // complete type
#include "../../Visos/Graphics/PvGWnd.h"             // complete type
#include "PauseWindowFreeVramInterface.h"            // complete type

// SIZE 0x20c
// VTABLE: LEMBALL 0x00497798 PvGWnd
// VTABLE: LEMBALL 0x00497788 BaseQueueHandler
// VTABLE: LEMBALL 0x00497780 PauseWindowFreeVramInterface
// VTABLE: LEMBALL 0x00497758 HotAreaHandler
class PauseWindow : public PvGWnd, public BaseQueueHandler, public PauseWindowFreeVramInterface, public HotAreaHandler {
public:
	BaseRemap* Remap(int p_item);
	PauseWindow(ReceiveWindowState* p_arg0, PvGWnd* p_arg1, ePauseWindowMessages p_arg2);
	VsRect CalculateWindow();
	virtual int ProcessMsg(Message* p_message);                           // vtable+0x08
	virtual void OnButtonDown(const VsPoint& p_point, int p_flags);       // vtable+0x04
	virtual void FreeVram();                                              // vtable+0x00
	virtual void OnButtonUp(const VsPoint& p_point, int p_flags);         // vtable+0x08
	virtual void OnDriverChange();                                        // vtable+0x5c
	virtual void OnExternalButtonUp(const VsPoint& p_point, int p_flags); // vtable+0x0c
	virtual void OnInside(const VsPoint& p_point);                        // vtable+0x18
	virtual void OnPaint(const VsRect& p_rect);                           // vtable+0xa8
	void CreateTheWindow(const VsRect& p_rect);
	void Initialise();
	void Load();
	void RegisterRemaps();
	void Restart();
	void UnLoad();
	void UnRegisterRemaps();
	~PauseWindow();

private:
	TextManager m_textManager;             // 0xdc
	char** m_menuLabels;                   // 0x100
	int m_pauseMessage;                    // 0x104
	unsigned int m_cursorState;            // 0x108
	unsigned int m_lowResolution;          // 0x10c
	ReceiveWindowState* m_receiverState;   // 0x110
	PvGWnd* m_parentWindow;                // 0x114
	int m_selection;                       // 0x118
	int m_unavailableItems;                // 0x11c
	int m_menuItemCount;                   // 0x120
	int m_minimumSelection;                // 0x124
	int m_initialSelection;                // 0x128
	int m_verticalTextOffset;              // 0x12c
	short m_horizontalTiles;               // 0x130
	short m_verticalTiles;                 // 0x132
	int m_borderAnimCount;                 // 0x134
	Line m_borderLine;                     // 0x138
	VsPoint m_windowPadding;               // 0x148
	VsPoint m_textSpacing;                 // 0x14c
	VsPoint m_borderPadding;               // 0x150
	ResAnim* m_horizontalBorderAnim;       // 0x154
	ResAnim* m_verticalBorderAnim;         // 0x158
	Anim m_cornerAnims[4];                 // 0x15c
	Anim* m_borderAnims;                   // 0x1dc
	BaseRemap* m_remaps[4];                // 0x1e0
	ResFont* m_font;                       // 0x1f0
	void* m_vramSurface;                   // 0x1f4
	VsPoint* m_textSizes;                  // 0x1f8
	unsigned int m_horizontalBorderAnimId; // 0x1fc
	unsigned int m_verticalBorderAnimId;   // 0x200
	unsigned int m_fontId;                 // 0x204
	unsigned int m_loaded;                 // 0x208
};

// SYNTHETIC: LEMBALL 0x00445350
// PauseWindow::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00445390
// PauseWindow::`vector deleting destructor'

#endif
