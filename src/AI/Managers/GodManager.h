#ifndef LEMBALL_SCAFFOLD_AI_MANAGERS_GODMANAGER_H
#define LEMBALL_SCAFFOLD_AI_MANAGERS_GODMANAGER_H

#include "../../Common.h"
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type

// SIZE 0x24
// VTABLE: LEMBALL 0x004932e8
class GodManager : public BaseQueueHandler {
public:
	GodManager(int p_arg0);
	bool TransportReceive(ReadPacket* p_packet);
	int GetViewData(ViewData* p_viewData);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~GodManager();                      // vtable+0x04
	void Process();
	void Register(BaseObjectManager* p_manager);
	void Restart();

private:
	BaseObjectManager** m_managers;       // 0x10
	int m_capacity;                       // 0x14
	int m_count;                          // 0x18
	int* m_transportMap;                  // 0x1c
	GameStateMessage* m_gameStateMessage; // 0x20
};

// Confirmed class-scoped globals.
extern GodManager* g_pGodManager;
#endif
