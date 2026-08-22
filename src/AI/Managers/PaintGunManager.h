#ifndef LEMBALL_AI_MANAGERS_PAINTGUNMANAGER_H
#define LEMBALL_AI_MANAGERS_PAINTGUNMANAGER_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x40
// VTABLE: LEMBALL 0x00496888
class PaintGunManager : public NetworkMessage {
public:
	PaintGunManager(Ai* p_arg0, int p_arg1);
	int GetViewData(ViewData* p_viewData);
	virtual ~PaintGunManager(); // vtable+0x14
	void Add(unsigned short p_id, int p_x, int p_y, int p_z, int p_direction);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();

private:
	unsigned int m_messageType; // 0x2c
	int m_capacity;             // 0x30
	int m_count;                // 0x34
	PaintGun* m_paintGuns;      // 0x38
	Ai* m_ai;                   // 0x3c
};

#endif
