#ifndef LEMBALL_SCAFFOLD_AI_NAVIGATION_MOVERMANAGER_H
#define LEMBALL_SCAFFOLD_AI_NAVIGATION_MOVERMANAGER_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x40
// VTABLE: LEMBALL 0x00496b78
class MoverManager : public NetworkMessage {
public:
	Mover* Find(int p_x, int p_y, int& p_height);
	MoverManager(Ai* p_arg0, int p_arg1);
	virtual int GetViewData(ViewData* p_viewData); // vtable+0x24
	virtual void Process();                        // vtable+0x1c
	virtual void Restart();                        // vtable+0x18
	virtual ~MoverManager();                       // vtable+0x14
	void Add(unsigned short p_id, int p_pathId, unsigned char p_movementMode, int p_startNode, int p_nodeCount);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Switch(int p_message, int p_id);

private:
	unsigned int m_messageType; // 0x2c
	int m_capacity;             // 0x30
	int m_count;                // 0x34
	Mover* m_movers;            // 0x38
	Ai* m_ai;                   // 0x3c
};

#endif
