#ifndef LEMBALL_AI_MANAGERS_CGODMANAGER_H
#define LEMBALL_AI_MANAGERS_CGODMANAGER_H

#include "../../Visos/Foundation/CBaseQueueHandler.h" // complete type

class CBaseObjectManager;
class CGameStateMessage;
class CReadPacket;
class CViewData;
// SIZE 0x24
// VTABLE: LEMBALL 0x004932e8
class CGodManager : public CBaseQueueHandler {
public:
	CGodManager(int p_capacity);
	bool TransportReceive(CReadPacket* p_packet);
	int GetViewData(CViewData* p_viewData);
	CBaseObjectManager* GetManagerForTransport(int p_transportId);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~CGodManager();                     // vtable+0x04
	void Process();
	void Register(CBaseObjectManager* p_manager);
	void Restart();

private:
	CBaseObjectManager** m_managers;       // 0x10
	int m_capacity;                        // 0x14
	int m_count;                           // 0x18
	int* m_transportMap;                   // 0x1c
	CGameStateMessage* m_gameStateMessage; // 0x20
};

extern CGodManager* g_pGodManager;

// SYNTHETIC: LEMBALL 0x0040b450
// CGodManager::`scalar deleting destructor'

#endif
