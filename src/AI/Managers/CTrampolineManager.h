#ifndef LEMBALL_AI_MANAGERS_CTRAMPOLINEMANAGER_H
#define LEMBALL_AI_MANAGERS_CTRAMPOLINEMANAGER_H

#include "CBaseObjectManager.h"

class CAI;
class AiCoord;
class CGameObject;
class CTrampoline;
// SIZE 0x40
// VTABLE: LEMBALL 0x00496710
class CTrampolineManager : public CBaseObjectManager {
public:
	CTrampolineManager(CAI* p_ai, int p_capacity);
	int GetViewData(CViewData* p_viewData);
	int Hit(const AiCoord& p_position, CGameObject* p_object);
	virtual ~CTrampolineManager(); // vtable+0x14
	void Add(unsigned short p_id, int p_x, int p_y, int p_z);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();
	void ResetCount();
	int TryEnableNear(const AiCoord& p_position, CGameObject* p_object);

private:
	int m_capacity;             // 0x30
	int m_count;                // 0x34
	CTrampoline* m_trampolines; // 0x38
	CAI* m_ai;                  // 0x3c
};

// SYNTHETIC: LEMBALL 0x0042ba10
// CTrampolineManager::`scalar deleting destructor'

#endif
