#ifndef LEMBALL_AI_MANAGERS_CROCKETMANAGER_H
#define LEMBALL_AI_MANAGERS_CROCKETMANAGER_H

#include "CBaseObjectManager.h" // complete type

class CAi;
class AiCoord;
class CGameObject;
class CRocket;
// SIZE 0x40
// VTABLE: LEMBALL 0x00496020
class CRocketManager : public CBaseObjectManager {
public:
	CRocketManager(CAi* p_ai, int p_capacity);
	int GetViewData(CViewData* p_viewData);
	int StepOn(const AiCoord& p_position, CGameObject* p_object);
	virtual ~CRocketManager(); // vtable+0x14
	void Add(unsigned short p_id, int p_x, int p_y, int p_z);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();
	void ResetCount();

	friend class CAi;

private:
	int m_capacity;     // 0x30
	int m_count;        // 0x34
	CRocket* m_rockets; // 0x38
	CAi* m_ai;          // 0x3c
};

// SYNTHETIC: LEMBALL 0x004274f0
// CRocketManager::`scalar deleting destructor'

#endif
