#ifndef LEMBALL_AI_MANAGERS_CBASEOBJECTMANAGER_H
#define LEMBALL_AI_MANAGERS_CBASEOBJECTMANAGER_H

#include "../../Visos/Messaging/CNetworkMessage.h"

class CGlobalGameObject;
class CViewData;
class CBaseNetwork;
extern CBaseNetwork* g_pBaseNetwork;
extern unsigned int g_networkPacketSize;

// SIZE 0x30
// VTABLE: LEMBALL 0x00493268
class CBaseObjectManager : public CNetworkMessage {
public:
	inline CBaseObjectManager(unsigned long p_messageId, int p_messageType) : CNetworkMessage(p_messageId)
	{
		m_messageType = p_messageType;
		if (g_pBaseNetwork != 0) {
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
	int m_messageType; // 0x2c
};

// SYNTHETIC: LEMBALL 0x0040aba0
// CBaseObjectManager::`scalar deleting destructor'

#endif
