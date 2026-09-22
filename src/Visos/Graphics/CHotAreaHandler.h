#ifndef LEMBALL_VISOS_GRAPHICS_CHOTAREAHANDLER_H
#define LEMBALL_VISOS_GRAPHICS_CHOTAREAHANDLER_H

#include "../Foundation/CVsRect.h"
struct CVsPoint;

struct Message;
class CHotAreaList;

// SIZE 0x38
// VTABLE: LEMBALL 0x00496ea0
class CHotAreaHandler {
public:
	CHotAreaHandler();
	CHotAreaHandler(const CVsRect& p_arg0);
	void ProcessArea(Message* p_message, const CVsPoint& p_point, class CHotAreaHandler* p_currentHandler);
	virtual ~CHotAreaHandler() {}                                          // vtable+0x00
	virtual void OnButtonDown(const CVsPoint& p_point, int p_flags);       // vtable+0x04
	virtual void OnButtonUp(const CVsPoint& p_point, int p_flags);         // vtable+0x08
	virtual void OnExternalButtonUp(const CVsPoint& p_point, int p_flags); // vtable+0x0c
	virtual void OnEnter();                                                // vtable+0x10
	virtual void OnExit();                                                 // vtable+0x14
	virtual void OnInside(const CVsPoint& p_point);                        // vtable+0x18
	virtual bool InArea(const CVsPoint& p_point);                          // vtable+0x1c
	void Initialise();
	void Reset();
	void SetActive(unsigned int p_active);
	void SetParent(CHotAreaList* p_parent);

	friend class CNetworkOptionsDrawer;
	friend class CHotAreaList;
	friend class CGraphicButton;
	friend class CPvButton;
	friend class CDepressedButton;
	friend class CFramedButton;
	friend class CTextButton;
	friend class CPanelPauseButton;
	friend class C2D;
	friend class CTrackWindow;
	friend class CTrackerButton;
	friend class CPauseWindow;
	friend class CPanelButton;

private:
	unsigned int m_active;          // 0x04
	unsigned int m_entered;         // 0x08
	CHotAreaList* m_parent;         // 0x0c
	unsigned int m_buttonState[6];  // 0x10
	unsigned int m_externalEnabled; // 0x28
	unsigned int m_reserved;        // 0x2c
	CVsRect m_bounds;               // 0x30
};

// SYNTHETIC: LEMBALL 0x00439a40
// CHotAreaHandler::`scalar deleting destructor'

#endif
