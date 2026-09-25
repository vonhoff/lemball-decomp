#ifndef LEMBALL_VISOS_GRAPHICS_CHOTAREALIST_H
#define LEMBALL_VISOS_GRAPHICS_CHOTAREALIST_H

#include "../Foundation/CBaseQueueHandler.h"
#include "../Foundation/CVsPoint.h"
#include "CHotAreaHandler.h"

class CVsRect;
struct Message;
struct CHotAreaElement;

// SIZE 0x60
// VTABLE: LEMBALL 0x00499d18 CHotAreaHandler
// VTABLE: LEMBALL 0x00499d08 CBaseQueueHandler
class CHotAreaList : public CHotAreaHandler, public CBaseQueueHandler {
public:
	CHotAreaList(const CVsRect& p_rect, const CVsPoint& p_relativeTopLeft, const CVsPoint& p_innerOrigin);
	void Set(const CVsRect& p_rect, CVsPoint p_relativeTopLeft, const CVsPoint& p_innerOrigin);
	void RemoveFromList(CHotAreaHandler* p_handler);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual void OnExit();                      // vtable+0x14
	virtual ~CHotAreaList();                    // vtable+0x00
	void AddToList(CHotAreaHandler* p_handler);
	void DeleteEntry(CHotAreaElement* p_entry);
	void ProcessHandlers(const CVsPoint& p_point, Message* p_message);
	void UpdateHandlers();

	friend class CPVWnd;

private:
	CHotAreaElement* m_head;           // 0x48
	CHotAreaElement* m_tail;           // 0x4c
	unsigned int m_scale;              // 0x50
	CVsPoint m_relativeTopLeft;        // 0x54
	CVsPoint m_innerOrigin;            // 0x58
	CHotAreaHandler* m_currentHandler; // 0x5c
};

// SYNTHETIC: LEMBALL 0x0046aa50
// CHotAreaList::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0046aa70
// CHotAreaList::`vector deleting destructor'

#endif
