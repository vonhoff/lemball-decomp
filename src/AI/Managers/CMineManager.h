#ifndef LEMBALL_AI_MANAGERS_CMINEMANAGER_H
#define LEMBALL_AI_MANAGERS_CMINEMANAGER_H

#include "../Base/AiCoord.h"    // complete type
#include "CBaseObjectManager.h" // complete type

class CAi;
class CGameObject;
class CMine;
struct Coord3d;
// SIZE 0x44
// VTABLE: LEMBALL 0x00495d30
class CMineManager : public CBaseObjectManager {
public:
	CMineManager(CAi* p_arg0, int p_arg1);
	int GetViewData(CViewData* p_viewData);
	virtual ~CMineManager(); // vtable+0x14
	void Add(unsigned short p_id, AiCoord p_position);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();
	void StepOn(const AiCoord& p_position, CGameObject* p_object);
	void Trigger(int p_index, int p_delay);
	void Triggered(CMine* p_mine);

	friend class CAi;

private:
	CAi* m_ai;            // 0x30
	CMine* m_mines;       // 0x34
	Coord3d* m_positions; // 0x38
	int m_count;          // 0x3c
	int m_capacity;       // 0x40
};

// SYNTHETIC: LEMBALL 0x00424c30
// CMineManager::`scalar deleting destructor'

#endif
