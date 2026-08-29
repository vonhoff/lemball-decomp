#ifndef LEMBALL_AI_MANAGERS_HANDMANAGER_H
#define LEMBALL_AI_MANAGERS_HANDMANAGER_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x40
// VTABLE: LEMBALL 0x004962e0
class HandManager : public NetworkMessage {
public:
	HandManager(Ai* p_arg0, int p_arg1);
	bool StepOn(const AiCoord& p_position, GameObject* p_object);
	virtual int GetViewData(ViewData* p_viewData); // vtable+0x24
	virtual void Process();                        // vtable+0x1c
	virtual ~HandManager();                        // vtable+0x14
	void Restart();
	void Add(unsigned short p_id, int p_x, int p_y, int p_z);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);

private:
	unsigned int m_messageType; // 0x2c
	int m_capacity;             // 0x30
	int m_count;                // 0x34
	Hand* m_hands;              // 0x38
	Ai* m_ai;                   // 0x3c
};

// SYNTHETIC: LEMBALL 0x004287d0
// HandManager::`scalar deleting destructor'


#endif

