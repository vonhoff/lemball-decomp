#ifndef LEMBALL_AI_MANAGERS_CBASEOBJECTMANAGER_H
#define LEMBALL_AI_MANAGERS_CBASEOBJECTMANAGER_H

#include "Visos/Network/Protocol/CNetworkMessage.h"

#include <stddef.h>

class CGlobalGameObject;
class CViewData;
class CBaseNetwork;
extern CBaseNetwork* g_pBaseNetwork;
extern unsigned int g_networkPacketSize;

enum eObjectManagerTransportId {
	OBJECT_MANAGER_TRANSPORT_OBJECTS = 1,
	OBJECT_MANAGER_TRANSPORT_MINES = 2,
	OBJECT_MANAGER_TRANSPORT_COLLECTABLES = 6,
	OBJECT_MANAGER_TRANSPORT_LIFTS = 7,
	OBJECT_MANAGER_TRANSPORT_DOORS = 8,
	OBJECT_MANAGER_TRANSPORT_ROCKETS = 9,
	OBJECT_MANAGER_TRANSPORT_LASERS = 10,
	OBJECT_MANAGER_TRANSPORT_HANDS = 11,
	OBJECT_MANAGER_TRANSPORT_TRAMPOLINES = 13,
	OBJECT_MANAGER_TRANSPORT_ICE = 14,
	OBJECT_MANAGER_TRANSPORT_TRAP_DOORS = 19,
	OBJECT_MANAGER_TRANSPORT_PAINT_GUNS = 20,
	OBJECT_MANAGER_TRANSPORT_INVISIBLE_SWITCHES = 21,
	OBJECT_MANAGER_TRANSPORT_BULLETS = 22,
	OBJECT_MANAGER_TRANSPORT_MOVERS = 15,
	OBJECT_MANAGER_TRANSPORT_PLAYER_LEMMING_GROUPS = 23
};

enum {
	NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE = 0x0b
};

// SIZE 0x30
// VTABLE: LEMBALL 0x00493268
class CBaseObjectManager : public CNetworkMessage {
public:
	inline CBaseObjectManager(unsigned long p_messageId, int p_transportId) : CNetworkMessage(p_messageId)
	{
		m_transportId = p_transportId;
		if (g_pBaseNetwork != NULL) {
			m_headerEnabled = 1;
			m_payloadCapacity += g_networkPacketSize;
		}
	}
	virtual void GetData();     // vtable+0x08
	virtual void AddData();     // vtable+0x10
	virtual void Restart();     // vtable+0x18
	virtual void Process() = 0; // vtable+0x1c
	virtual bool Receive(unsigned short p_messageId,
						 CGlobalGameObject* p_object,
						 CNetworkMessage* p_message); // vtable+0x20
	virtual int GetViewData(CViewData* p_viewData);   // vtable+0x24
	void Add(CNetworkMessage* p_message);
	void ProcessNetwork();

	friend class CGodManager;

protected:
	int m_transportId; // 0x2c
};

// SYNTHETIC: LEMBALL 0x0040aba0
// CBaseObjectManager::`scalar deleting destructor'

#endif
