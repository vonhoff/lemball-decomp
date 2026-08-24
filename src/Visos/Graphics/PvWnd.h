#ifndef LEMBALL_VISOS_GRAPHICS_PVWND_H
#define LEMBALL_VISOS_GRAPHICS_PVWND_H

#include "../../Common.h"
#include "../Foundation/VsPoint.h" // complete type
#include "../Foundation/VsRect.h"  // complete type
#include "../Foundation/VsSize.h"  // complete type

// SIZE 0x3c
// VTABLE: LEMBALL 0x00499478
class PvWnd {
public:
	PvWnd();
	virtual bool GetMenu(int& p_menuResourceId, MenuList*** p_menuLists); // vtable+0x0c
	virtual bool IsFocusWindow();                                         // vtable+0x60
	virtual unsigned int GetSizeStatus();                                 // vtable+0x68
	virtual unsigned int InitHotAreaList();                               // vtable+0x14
	virtual void AddChild(class PvWnd* p_child);                          // vtable+0x78
	virtual void OnCreate();                                              // vtable+0x18
	virtual void OnDestroy();                                             // vtable+0x1c
	virtual void OnDriverChange();                                        // vtable+0x5c
	virtual void OnMove();                                                // vtable+0x28
	virtual void OnMove(VsPoint p_offset);                                // vtable+0x24
	virtual void OnSize();                                                // vtable+0x20
	virtual void OnVisibilityChange();                                    // vtable+0x80
	virtual void OnZoom(int p_oldZoom);                                   // vtable+0x2c
	virtual void ReSetMenu();                                             // vtable+0x00
	virtual void RemoveChild(class PvWnd* p_child);                       // vtable+0x7c
	virtual void Resize(VsSize p_size);                                   // vtable+0x88
	virtual void SetDontUpdateRect(const VsRect& p_rect);                 // vtable+0x84
	virtual void SetInnerWindow(const VsRect& p_rect);                    // vtable+0x08
	virtual void SetMenu(int& p_menuResourceId, MenuList** p_menuLists);  // vtable+0x10
	virtual void SetRelTl(const VsPoint& p_point);                        // vtable+0x34
	virtual void SetSizeStatus(unsigned int p_status);                    // vtable+0x6c
	void SetRect(const VsRect& p_rect);
	void SetRectInnerZoom(const VsRect& p_rect, const VsRect& p_innerRect, int p_zoom);
	void SetZoom(int p_zoom);
	~PvWnd();

	friend class PauseWindow;

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
