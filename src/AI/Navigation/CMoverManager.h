#ifndef LEMBALL_AI_NAVIGATION_CMOVERMANAGER_H
#define LEMBALL_AI_NAVIGATION_CMOVERMANAGER_H

#include "../Managers/CBaseObjectManager.h"

class CAI;
class CMover;
// SIZE 0x40
// VTABLE: LEMBALL 0x00496b78
class CMoverManager : public CBaseObjectManager {
public:
	CMover* Find(int p_x, int p_y, int& p_height);
	CMoverManager(CAI* p_ai, int p_capacity);
	virtual int GetViewData(CViewData* p_viewData); // vtable+0x24
	virtual void Process();                         // vtable+0x1c
	virtual void Restart();                         // vtable+0x18
	virtual ~CMoverManager();                       // vtable+0x14
	void Add(unsigned short p_id, int p_pathId, unsigned int p_movementMode, int p_startNode, int p_nodeCount);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Switch(int p_message, int p_id);
	void RemoveMover(CMover* p_mover);

private:
	int m_capacity;   // 0x30
	int m_count;      // 0x34
	CMover* m_movers; // 0x38
	CAI* m_ai;        // 0x3c
};

// SYNTHETIC: LEMBALL 0x0042fba0
// CMoverManager::`scalar deleting destructor'

#endif
