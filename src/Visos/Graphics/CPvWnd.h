#ifndef LEMBALL_VISOS_GRAPHICS_CPVWND_H
#define LEMBALL_VISOS_GRAPHICS_CPVWND_H

#include "../Foundation/CVsPoint.h" // complete type
#include "../Foundation/CVsRect.h"  // complete type
#include "../Foundation/CVsSize.h"  // complete type
struct MenuList;

class CHotAreaList;

// SIZE 0x0c
struct WindowOwnerNode {
	class CPvWnd* m_window;  // 0x00
	WindowOwnerNode* m_next; // 0x04
	WindowOwnerNode* m_prev; // 0x08
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
class CPvWnd {
public:
	CPvWnd();
	virtual void ReSetMenu();                                                              // vtable+0x00
	virtual void Create(const CVsRect& p_rect, class CPvWnd* p_parent, char* p_title) = 0; // vtable+0x04
	virtual void SetInnerWindow(const CVsRect& p_rect);                                    // vtable+0x08
	virtual bool GetMenu(int& p_menuResourceId, MenuList*** p_menuLists);                  // vtable+0x0c
	virtual void SetMenu(int& p_menuResourceId, MenuList** p_menuLists);                   // vtable+0x10
	virtual void InitHotAreaList();                                                        // vtable+0x14
	virtual void InternalOnCreate();                                                       // vtable+0x18
	virtual void InternalOnDestroy();                                                      // vtable+0x1c
	virtual void InternalOnSize();                                                         // vtable+0x20
	virtual void InternalOnMove();                                                         // vtable+0x28
	virtual void InternalOnMove(CVsPoint p_point);                                         // vtable+0x24
	virtual void InternalOnZoom(int p_oldZoom);                                            // vtable+0x2c
	virtual void InternalSetRect(const CVsRect& p_rect);                                   // vtable+0x30
	virtual void InternalSetRelTL(const CVsPoint& p_point);                                // vtable+0x34
	virtual void Move(const CVsPoint& p_point) = 0;                                        // vtable+0x38
	virtual void OnCreate() = 0;                                                           // vtable+0x3c
	virtual void OnDestroy() = 0;                                                          // vtable+0x40
	virtual void OnSize() = 0;                                                             // vtable+0x44
	virtual void OnMove() = 0;                                                             // vtable+0x48
	virtual void OnMinimise() = 0;                                                         // vtable+0x4c
	virtual void OnMaximise() = 0;                                                         // vtable+0x50
	virtual void OnRestore() = 0;                                                          // vtable+0x54
	virtual void OnZoom(int p_oldZoom) = 0;                                                // vtable+0x58
	virtual void OnDriverChange();                                                         // vtable+0x5c
	virtual bool IsFocusWindow();                                                          // vtable+0x60
	virtual unsigned int GetStyle() = 0;                                                   // vtable+0x64
	virtual unsigned int GetSizeStatus();                                                  // vtable+0x68
	virtual void SetSizeStatus(unsigned int p_status);                                     // vtable+0x6c
	virtual void Refresh(CVsRect* p_rect) = 0;                                             // vtable+0x70
	virtual void Destroy() = 0;                                                            // vtable+0x74
	virtual void AddChild(class CPvWnd* p_child);                                          // vtable+0x78
	virtual void RemoveChild(class CPvWnd* p_child);                                       // vtable+0x7c
	virtual void OnVisibilityChange();                                                     // vtable+0x80
	virtual void SetDontUpdateRect(const CVsRect& p_rect);                                 // vtable+0x84
	virtual void Resize(CVsSize p_size);                                                   // vtable+0x88
	void SetRect(const CVsRect& p_rect);
	void SetRectInnerZoom(const CVsRect& p_rect, const CVsRect& p_innerRect, int p_zoom);
	void SetZoom(int p_zoom);
	~CPvWnd();

	friend class CPauseWindow;
	friend class CDemo;
	friend class CPanel;
	friend class CNetworkOptionsDrawer;
	friend class CWnd;
	friend class CMain2DDisplay;
	friend class CIntroAnimDrawer;
	friend class CMainOptions1Drawer;
	friend class CMainOptions2Drawer;
	friend class CGWnd;
	friend class CPvGWnd;
	friend class CPvButton;
	friend class CGraphicButton;
	friend class CAnimWnd;
	friend class CGunController;
	friend class CHiliteController;
	friend class CPasswordDrawer;
	friend class CSuccFailDrawer;
	friend class CCdLoadAnim;
	friend class CFramedButton;
	friend class CTextButton;

	friend class C2D;
	friend class CPanelButton;
	friend class CBaseCursor;
	friend class CTrackWindow;
	friend class CTrackerButton;
	friend int __stdcall WinGDrawFrame(struct WinGDrawState* p_state, void* p_request, long p_param2);

private:
	unsigned int m_lifecycleRefs; // 0x04
	CVsRect m_rect;               // 0x08
	CVsRect m_innerRect;          // 0x10
	CVsPoint m_relativeTopLeft;   // 0x18
	CHotAreaList* m_hotAreaList;  // 0x1c
	class CPvWnd* m_parent;       // 0x20
	void* m_childList;            // 0x24
	void* m_childListTail;        // 0x28
	unsigned int m_childCount;    // 0x2c
	unsigned int m_sizeStatus;    // 0x30
	unsigned int m_active;        // 0x34
	unsigned int m_zoom;          // 0x38
};

#endif
