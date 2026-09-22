#ifndef LEMBALL_AI_MANAGERS_CHANDMANAGER_H
#define LEMBALL_AI_MANAGERS_CHANDMANAGER_H

#include "CBaseObjectManager.h" // complete type

class CAi;
class AiCoord;
class CGameObject;
class CHand;
// SIZE 0x40
// VTABLE: LEMBALL 0x004962e0
class CHandManager : public CBaseObjectManager {
public:
	CHandManager(CAi* p_ai, int p_capacity);
	bool StepOn(const AiCoord& p_position, CGameObject* p_object);
	virtual int GetViewData(CViewData* p_viewData); // vtable+0x24
	virtual void Process();                         // vtable+0x1c
	virtual ~CHandManager();                        // vtable+0x14
	void Restart();
	void ResetCount();
	void Add(unsigned short p_id, int p_x, int p_y, int p_z);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);

private:
	int m_capacity; // 0x30
	int m_count;    // 0x34
	CHand* m_hands; // 0x38
	CAi* m_ai;      // 0x3c
};

// SYNTHETIC: LEMBALL 0x004287d0
// CHandManager::`scalar deleting destructor'

#endif
