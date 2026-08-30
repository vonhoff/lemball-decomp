#ifndef LEMBALL_AI_MANAGERS_MINEMANAGER_H
#define LEMBALL_AI_MANAGERS_MINEMANAGER_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type
#include "../Base/AiCoord.h"                      // complete type

// SIZE 0x44
// VTABLE: LEMBALL 0x00495d30
class MineManager : public NetworkMessage {
public:
	MineManager(Ai* p_arg0, int p_arg1);
	int GetViewData(ViewData* p_viewData);
	virtual ~MineManager(); // vtable+0x14
	void Add(unsigned short p_id, AiCoord p_position);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();
	void StepOn(const AiCoord& p_position, GameObject* p_object);
	void Trigger(int p_index, int p_delay);
	void Triggered(Mine* p_mine);

private:
	unsigned int m_messageType; // 0x2c
	Ai* m_ai;                   // 0x30
	Mine* m_mines;              // 0x34
	Coord3d* m_positions;       // 0x38
	int m_count;                // 0x3c
	int m_capacity;             // 0x40
};

// SYNTHETIC: LEMBALL 0x00424c30
// MineManager::`scalar deleting destructor'

#endif
