#ifndef LEMBALL_VISOS_GRAPHICS_PVWND_H
#define LEMBALL_VISOS_GRAPHICS_PVWND_H

#include "../../Common.h"
#include "../Foundation/VsPoint.h" // complete type
#include "../Foundation/VsRect.h"  // complete type
#include "../Foundation/VsSize.h"  // complete type

// SIZE 0x0c
struct WindowOwnerNode {
	class PvWnd* m_window;     // 0x00
	WindowOwnerNode* m_next;   // 0x04
	WindowOwnerNode* m_prev;   // 0x08
};

// SIZE 0x0c
struct WindowOwnerList {
	WindowOwnerNode* m_head; // 0x00
	WindowOwnerNode* m_tail; // 0x04
	int m_count;             // 0x08
};

extern WindowOwnerList* g_pWindowOwnerList;

// SIZE 0x3c
// VTABLE: LEMBALL 0x00499478
class PvWnd {
public:
	PvWnd();
	virtual void ReSetMenu();                                             // vtable+0x00
	virtual void Create(const VsRect& p_rect, class PvWnd* p_parent, char* p_title); // vtable+0x04
	virtual void SetInnerWindow(const VsRect& p_rect);                    // vtable+0x08
	virtual bool GetMenu(int& p_menuResourceId, MenuList*** p_menuLists); // vtable+0x0c
	virtual void SetMenu(int& p_menuResourceId, MenuList** p_menuLists);  // vtable+0x10
	virtual unsigned int InitHotAreaList();                               // vtable+0x14
	virtual void OnCreate();                                              // vtable+0x18
	virtual void BaseOnDestroy();                                         // vtable+0x1c
	virtual void OnSize();                                                // vtable+0x20
	virtual void BaseOnMove();                                            // vtable+0x24
	virtual void OnMove();                                                // vtable+0x28
	virtual void OnZoom(int p_oldZoom);                                   // vtable+0x2c
	virtual void SetRect(const VsRect& p_rect);                           // vtable+0x30
	virtual void SetRelTl(const VsPoint& p_point);                        // vtable+0x34
	virtual void Move(const VsPoint& p_point);                            // vtable+0x38
	virtual void Dummy3c();                                               // vtable+0x3c
	virtual void OnDestroy();                                             // vtable+0x40
	virtual void BaseOnSize();                                            // vtable+0x44
	virtual void Dummy48();                                               // vtable+0x48
	virtual void OnMinimise();                                            // vtable+0x4c
	virtual void OnMaximise();                                            // vtable+0x50
	virtual void OnRestore();                                             // vtable+0x54
	virtual void OnZoomBox();                                             // vtable+0x58
	virtual void OnDriverChange();                                        // vtable+0x5c
	virtual bool IsFocusWindow();                                         // vtable+0x60
	virtual unsigned int GetStyle();                                      // vtable+0x64
	virtual unsigned int GetSizeStatus();                                 // vtable+0x68
	virtual void SetSizeStatus(unsigned int p_status);                    // vtable+0x6c
	virtual void Refresh(VsRect* p_rect);                                 // vtable+0x70
	virtual void Destroy();                                               // vtable+0x74
	virtual void AddChild(class PvWnd* p_child);                          // vtable+0x78
	virtual void RemoveChild(class PvWnd* p_child);                       // vtable+0x7c
	virtual void OnVisibilityChange();                                    // vtable+0x80
	virtual void SetDontUpdateRect(const VsRect& p_rect);                 // vtable+0x84
	virtual void Resize(VsSize p_size);                                   // vtable+0x88
	void SetRectInnerZoom(const VsRect& p_rect, const VsRect& p_innerRect, int p_zoom);
	void SetZoom(int p_zoom);
	~PvWnd();

	friend class PauseWindow;
	friend class NetworkOptionsDrawer;
	friend class Wnd;
	friend class Main2DDisplay;
	friend class IntroAnimDrawer;
	friend class MainOptions1Drawer;
	friend class MainOptions2Drawer;
	friend class GWnd;
	friend class PvButton;
	friend class GraphicButton;
	friend class AnimWnd;
	friend class GunController;
	friend class HiliteController;
	friend class CdLoadAnim;
	friend class CdLoadAnimDraw;
	friend class C2D;
	friend class BaseCursor;
	friend int TargetWinGDrawCodec_Draw(struct TargetWinGDrawCodecState* p_state, void* p_request);

private:
	unsigned int m_lifecycleRefs; // 0x04
	VsRect m_rect;                // 0x08
	VsRect m_innerRect;           // 0x10
	VsPoint m_relativeTopLeft;    // 0x18
	HotAreaList* m_hotAreaList;   // 0x1c
	class PvWnd* m_parent;        // 0x20
	void* m_childList;            // 0x24
	void* m_childListTail;        // 0x28
	unsigned int m_childCount;    // 0x2c
	unsigned int m_sizeStatus;    // 0x30
	unsigned int m_active;        // 0x34
	unsigned int m_zoom;          // 0x38
};

#endif
