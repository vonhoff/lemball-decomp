#ifndef LEMBALL_AI_BASE_GLOBALGAMEOBJECT_H
#define LEMBALL_AI_BASE_GLOBALGAMEOBJECT_H

#include "../../Common.h"
#include "GameObject.h" // complete type

// SIZE 0x138
// VTABLE: LEMBALL 0x00493ea0
class GlobalGameObject : public GameObject {
public:
	GlobalGameObject(eObjectType p_arg0, unsigned short p_arg1, unsigned short p_arg2);
	bool Receive(unsigned short p_arg0, NetworkMessage* p_arg1);
	virtual int UsableState();                       // vtable+0xfc
	virtual void Action(eAction p_arg0);             // vtable+0x08
	virtual void Action(eAction p_arg0, int p_arg1); // vtable+0x04
	virtual void DoActivate();                       // vtable+0x10c
	virtual void Restart();                          // vtable+0x104
	virtual void SendCancel();                       // vtable+0xf8
	virtual void SendRemove();                       // vtable+0xf4
	void CancelRequest();
	void DeleteMessages();
	void RequestAction(eAction p_arg0);
	void SetMessages();
	~GlobalGameObject();
	GlobalGameObject();

private:
	unsigned int m_requestEnabled; // 0x124
	eAction m_pendingAction;       // 0x128
	eAction m_requestedAction;     // 0x12c
	unsigned int m_requestActive;  // 0x130
	unsigned int m_usableState;    // 0x134
};

#endif
