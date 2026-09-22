#ifndef LEMBALL_AI_MANAGERS_CDOORMANAGER_H
#define LEMBALL_AI_MANAGERS_CDOORMANAGER_H

#include "../Base/AiCoord.h" // complete type
#include "../Base/ObjectTypes.h"
#include "../Objects/SwitchEntry.h"
#include "CBaseObjectManager.h" // complete type

class CAi;
class CDoor;
class CGameObject;
// SIZE 0x40
// VTABLE: LEMBALL 0x00493840
class CDoorManager : public CBaseObjectManager {
public:
	CDoorManager(CAi* p_arg0, int p_arg1);
	int Add(unsigned short p_id, eObjectType p_objectType, unsigned short p_doorType, int p_x, int p_y, int p_z);
	int GetViewData(CViewData* p_viewData);
	int Open(const AiCoord& p_position, CGameObject* p_object);
	unsigned short Id(int p_index);
	virtual ~CDoorManager(); // vtable+0x14
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();
	void Switch(swMessage p_message, int p_id);

	friend class CAi;

private:
	CAi* m_ai;      // 0x30
	int m_count;    // 0x34
	int m_capacity; // 0x38
	CDoor* m_doors; // 0x3c
};

// SYNTHETIC: LEMBALL 0x0040ebe0
// CDoorManager::`scalar deleting destructor'

#endif
