#ifndef LEMBALL_AI_MANAGERS_CLIFTMANAGER_H
#define LEMBALL_AI_MANAGERS_CLIFTMANAGER_H

#include "Gameplay/Objects/CBaseObjectManager.h"
#include "SwitchEntry.h"

class CAI;
class AICOORD;
class CGameObject;
class CLift;
struct tCoord3d;
struct LiftEndpointRecord;
// SIZE 0x40
// VTABLE: LEMBALL 0x00495ea8
class CLiftManager : public CBaseObjectManager {
public:
	CLiftManager(CAI* p_ai, int p_capacity);
	int GetViewData(CViewData* p_viewData);
	unsigned short Id(int p_index);
	virtual ~CLiftManager(); // vtable+0x14
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();
	void AddLiftFromXyz(unsigned short p_id, int p_x, int p_y, int p_z);
	void AddLiftFromEndpoints(unsigned short p_id, tCoord3d& p_start, tCoord3d& p_end);
	int ExportEndpoints(LiftEndpointRecord* p_records);
	int ExportLiftStartCoordinates(tCoord3d* p_records);
	void CalculateAllLiftCliffs();
	void RemoveLift(CLift* p_lift);
	void StepOn(const AICOORD& p_position, CGameObject* p_object);
	void Switch(swMessage p_message, int p_id, int p_legacyA, int p_legacyB);

private:
	CAI* m_ai;      // 0x30
	int m_count;    // 0x34
	int m_capacity; // 0x38
	CLift* m_lifts; // 0x3c
};

// SYNTHETIC: LEMBALL 0x004266e0
// CLiftManager::`scalar deleting destructor'

extern unsigned short g_wMovingLiftCount;

#endif
