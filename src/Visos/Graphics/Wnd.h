#ifndef LEMBALL_VISOS_GRAPHICS_WND_H
#define LEMBALL_VISOS_GRAPHICS_WND_H

#include "../../Common.h"
#include "PvWnd.h" // complete type

// SIZE 0x4c
// VTABLE: LEMBALL 0x004993d0
class Wnd : public PvWnd {
public:
	Wnd();
	int SelectMenu(unsigned int p_message, unsigned int p_wParam, unsigned int p_lParam);
	static long __stdcall ProcessMessage(void* p_hwnd,
										 unsigned int p_message,
										 unsigned int p_wParam,
										 unsigned int p_lParam);
	static void ProcessMouseMoves();
	virtual bool IsFocusWindow();                                              // vtable+0x60
	virtual unsigned int GetStyle();                                           // vtable+0x64
	virtual void Create(const VsRect& p_rect, PvWnd* p_parent, char* p_title); // vtable+0x04
	virtual void Destroy();                                                    // vtable+0x74
	virtual void Move(const VsPoint& p_point);                                 // vtable+0x38
	virtual void MoveAbsolute(const VsPoint& p_point);                         // vtable+0x8c
	virtual void SetFocusWindow();                                             // vtable+0x90
	virtual void Dummy94();                                                    // vtable+0x94
	virtual void Dummy98();                                                    // vtable+0x98
	virtual void Dummy9c();                                                    // vtable+0x9c
	virtual int ProcessOtherMessages(unsigned int p_message,
									 unsigned int p_wParam,
									 unsigned int p_lParam);             // vtable+0xa0
	virtual void Dummy3c();                                              // vtable+0x3c
	virtual void OnDestroy();                                            // vtable+0x40
	virtual void OnMaximise();                                           // vtable+0x50
	virtual void OnMinimise();                                           // vtable+0x4c
	virtual void OnMove();                                               // vtable+0x48
	virtual void OnRestore();                                            // vtable+0x54
	virtual void OnSize();                                               // vtable+0x44
	virtual void _OnZoom(int p_oldZoom);                                 // vtable+0x2c
	virtual void OnZoom(int p_oldZoom);                                  // vtable+0x58
	virtual void OnDriverChange();                                       // vtable+0x5c
	virtual void ReSetMenu();                                            // vtable+0x00
	virtual void Refresh(VsRect* p_rect);                                // vtable+0x70
	virtual void SetMenu(int& p_menuResourceId, MenuList** p_menuLists); // vtable+0x10
	virtual void _SetRect(const VsRect& p_rect);                         // vtable+0x30
	virtual void SetRelTl(const VsPoint& p_point);                       // vtable+0x34
	~Wnd();

	friend class GWnd;
	friend class AnimWnd;
	friend class Main2DDisplay;

private:
	void* m_menuLists;             // 0x3c
	unsigned int m_menuResourceId; // 0x40
	void* m_nativeWindow;          // 0x44
	PvWnd* m_createRect;           // 0x48
};

extern Wnd* g_pFocusWindow;
extern int g_nMouseCaptureCount;

#endif
