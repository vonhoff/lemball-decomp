#ifndef LEMBALL_AI_MANAGERS_TRAPDOORMANAGER_H
#define LEMBALL_AI_MANAGERS_TRAPDOORMANAGER_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x54
// VTABLE: LEMBALL 0x004936c8
class TrapDoorManager : public NetworkMessage {
public:
	TrapDoorManager();
	virtual int GetViewData(ViewData* p_viewData); // vtable+0x24
	virtual void Process();                        // vtable+0x1c
	virtual ~TrapDoorManager();                    // vtable+0x14
	void Restart();
	void AddNewDoor(unsigned short p_id, const AiCoord& p_position, unsigned char p_mode, unsigned long p_deadline);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);

private:
	unsigned int m_messageType; // 0x2c
	TrapDoor* m_doors[8];       // 0x30
	int m_count;                // 0x50
};

// SYNTHETIC: LEMBALL 0x0040ced0
// TrapDoorManager::`scalar deleting destructor'

#endif
