#ifndef LEMBALL_AI_MANAGERS_BASEOBJECTMANAGER_H
#define LEMBALL_AI_MANAGERS_BASEOBJECTMANAGER_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x00493268
class BaseObjectManager : public NetworkMessage {
public:
	virtual void GetData();                        // vtable+0x08
	virtual void AddData();                        // vtable+0x10
	virtual ~BaseObjectManager();                  // vtable+0x14
	virtual void Restart();                        // vtable+0x18
	virtual bool Receive(unsigned short p_messageId,
						 GlobalGameObject* p_object,
						 NetworkMessage* p_message); // vtable+0x20
	virtual int GetViewData(ViewData* p_viewData);   // vtable+0x24
	void Add(NetworkMessage* p_message);
	void ProcessNetwork();

private:
	int m_transportId; // 0x2c
};

// SYNTHETIC: LEMBALL 0x0040aba0
// BaseObjectManager::`scalar deleting destructor'


#endif

