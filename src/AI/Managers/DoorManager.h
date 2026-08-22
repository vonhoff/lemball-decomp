#ifndef LEMBALL_AI_MANAGERS_DOORMANAGER_H
#define LEMBALL_AI_MANAGERS_DOORMANAGER_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x40
// VTABLE: LEMBALL 0x00493840
class DoorManager : public NetworkMessage {
public:
	DoorManager(Ai* p_arg0, int p_arg1);
	int Add(unsigned short p_id, eObjectType p_objectType, unsigned short p_doorType, int p_x, int p_y, int p_z);
	int GetViewData(ViewData* p_viewData);
	int Open(const AiCoord& p_position, GameObject* p_object);
	unsigned short Id(int p_index);
	virtual ~DoorManager(); // vtable+0x14
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();
	void Switch(swMessage p_message, int p_id);

private:
	unsigned int m_messageType; // 0x2c
	Ai* m_ai;                   // 0x30
	int m_count;                // 0x34
	int m_capacity;             // 0x38
	Door* m_doors;              // 0x3c
};

#endif
