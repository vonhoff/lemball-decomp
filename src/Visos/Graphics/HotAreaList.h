#ifndef LEMBALL_VISOS_GRAPHICS_HOTAREALIST_H
#define LEMBALL_VISOS_GRAPHICS_HOTAREALIST_H

#include "../../Common.h"
#include "../Foundation/BaseQueueHandler.h" // complete type
#include "../Foundation/VsPoint.h"          // complete type
#include "HotAreaHandler.h"                 // complete type

// SIZE 0x60
// VTABLE: LEMBALL 0x00499d18 HotAreaHandler
// VTABLE: LEMBALL 0x00499d08 BaseQueueHandler
class HotAreaList : public HotAreaHandler, public BaseQueueHandler {
public:
	HotAreaList(const VsRect& p_arg0, const VsPoint& p_arg1, const VsPoint& p_arg2);
	void Set(const VsRect& p_rect, VsPoint p_point0, const VsPoint& p_point1);
	bool RemoveFromList(HotAreaHandler* p_handler);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual void OnExit();                      // vtable+0x14
	virtual ~HotAreaList();                     // vtable+0x00
	void AddToList(HotAreaHandler* p_handler);
	void DeleteEntry(HotAreaElement* p_entry);
	void ProcessHandlers(const VsPoint& p_point, Message* p_message);
	void UpdateHandlers();

private:
	HotAreaElement* m_head;           // 0x48
	HotAreaElement* m_tail;           // 0x4c
	unsigned int m_scale;             // 0x50
	VsPoint m_point0;                 // 0x54
	VsPoint m_point1;                 // 0x58
	HotAreaHandler* m_currentHandler; // 0x5c
};

// SYNTHETIC: LEMBALL 0x0046aa50
// HotAreaList::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0046aa70
// HotAreaList::`vector deleting destructor'

#endif
