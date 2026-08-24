#ifndef LEMBALL_VISOS_GRAPHICS_HOTAREAHANDLER_H
#define LEMBALL_VISOS_GRAPHICS_HOTAREAHANDLER_H

#include "../../Common.h"

// SIZE 0x38
// VTABLE: LEMBALL 0x00496ea0
class HotAreaHandler {
public:
	HotAreaHandler();
	HotAreaHandler(const VsRect& p_arg0);
	unsigned int ProcessArea(Message* p_message, const VsPoint& p_point, class HotAreaHandler* p_currentHandler);
	virtual ~HotAreaHandler();                                              // vtable+0x00
	virtual unsigned int OnButtonDown(const VsPoint& p_point, int p_flags); // vtable+0x04
	virtual void OnButtonUp(const VsPoint& p_point, int p_flags);           // vtable+0x08
	virtual void OnExternalButtonUp(const VsPoint& p_point, int p_flags);   // vtable+0x0c
	virtual void OnEnter();                                                 // vtable+0x10
	virtual void OnExit();                                                  // vtable+0x14
	virtual void OnInside(const VsPoint& p_point);                          // vtable+0x18
	virtual bool InArea(const VsPoint& p_point);                            // vtable+0x1c
	void Initialise();
	void Reset();
	void SetActive(unsigned int p_active);
	void SetParent(HotAreaList* p_parent);

	friend class NetworkOptionsDrawer;

private:
	unsigned int m_active;          // 0x04
	unsigned int m_entered;         // 0x08
	HotAreaList* m_parent;          // 0x0c
	unsigned int m_buttonState[6];  // 0x10
	unsigned int m_externalEnabled; // 0x28
	unsigned int m_reserved;        // 0x2c
	unsigned short m_width;         // 0x30
	unsigned short m_height;        // 0x32
	short m_x;                      // 0x34
	short m_y;                      // 0x36
};

#endif
