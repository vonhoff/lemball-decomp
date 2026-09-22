#ifndef LEMBALL_AI_MANAGERS_CICEMANAGER_H
#define LEMBALL_AI_MANAGERS_CICEMANAGER_H

#include "CBaseObjectManager.h" // complete type

class CAI;
class AiCoord;
class CGameObject;
class CIce;
struct Coord3d;
// SIZE 0x40
// VTABLE: LEMBALL 0x00496a00
class CIceManager : public CBaseObjectManager {
public:
	CIceManager(CAI* p_ai, int p_capacity);
	bool StepOn(const AiCoord& p_position, CGameObject* p_object);
	virtual int GetViewData(CViewData* p_viewData); // vtable+0x24
	virtual void Process();                         // vtable+0x1c
	virtual ~CIceManager();                         // vtable+0x14
	void Restart();
	void Add(unsigned short p_id,
			 const Coord3d& p_cornerA,
			 const Coord3d& p_cornerB,
			 int p_velocityX,
			 int p_velocityY,
			 unsigned int p_initialSwitched);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Switch(int p_message, int p_id);
	int ExportIceRecords(Coord3d p_records[][2]);

private:
	int m_capacity; // 0x30
	int m_count;    // 0x34
	CIce* m_ice;    // 0x38
	CAI* m_ai;      // 0x3c
};

// SYNTHETIC: LEMBALL 0x0042e4d0
// CIceManager::`scalar deleting destructor'

#endif
