#ifndef LEMBALL_AI_MANAGERS_COLLECTABLEMANAGER_H
#define LEMBALL_AI_MANAGERS_COLLECTABLEMANAGER_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x40
// VTABLE: LEMBALL 0x004955a0
class CollectableManager : public NetworkMessage {
public:
	CollectableManager(Ai* p_arg0, int p_arg1);
	virtual int GetViewData(ViewData* p_viewData); // vtable+0x24
	virtual ~CollectableManager();                 // vtable+0x14
	void Add(unsigned short p_id, int p_x, int p_y, int p_z, eObjectType p_type);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();

private:
	unsigned int m_messageType;   // 0x2c
	Ai* m_ai;                     // 0x30
	Collectable** m_collectables; // 0x34
	int m_capacity;               // 0x38
	int m_count;                  // 0x3c
};

#endif
