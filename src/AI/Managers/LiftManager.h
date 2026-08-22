#ifndef LEMBALL_AI_MANAGERS_LIFTMANAGER_H
#define LEMBALL_AI_MANAGERS_LIFTMANAGER_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x40
// VTABLE: LEMBALL 0x00495ea8
class LiftManager : public NetworkMessage {
public:
	LiftManager(Ai* p_arg0, int p_arg1);
	int GetViewData(ViewData* p_viewData);
	unsigned short Id(int p_index);
	virtual ~LiftManager(); // vtable+0x14
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();
	void StepOn(const AiCoord& p_position, GameObject* p_object);
	void Switch(swMessage p_message, int p_id, int p_legacyA, int p_legacyB);

private:
	unsigned int m_messageType; // 0x2c
	Ai* m_ai;                   // 0x30
	int m_count;                // 0x34
	int m_capacity;             // 0x38
	Lift* m_lifts;              // 0x3c
};

#endif
