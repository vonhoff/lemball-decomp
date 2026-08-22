#ifndef LEMBALL_SCAFFOLD_AI_MANAGERS_TRAMPOLINEMANAGER_H
#define LEMBALL_SCAFFOLD_AI_MANAGERS_TRAMPOLINEMANAGER_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x40
// VTABLE: LEMBALL 0x00496710
class TrampolineManager : public NetworkMessage {
public:
	TrampolineManager(Ai* p_arg0, int p_arg1);
	int GetViewData(ViewData* p_viewData);
	int Hit(const AiCoord& p_position, GameObject* p_object);
	virtual ~TrampolineManager(); // vtable+0x14
	void Add(unsigned short p_id, int p_x, int p_y, int p_z);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();

private:
	unsigned int m_messageType; // 0x2c
	int m_capacity;             // 0x30
	int m_count;                // 0x34
	Trampoline* m_trampolines;  // 0x38
	Ai* m_ai;                   // 0x3c
};

#endif
