#ifndef LEMBALL_AI_OBJECTS_CGROUNDANIM_H
#define LEMBALL_AI_OBJECTS_CGROUNDANIM_H

#include "GroundAnimEntry.h"

struct tCoord3d;

enum {
	GROUND_ANIM_ENTRY_CAPACITY = 200,
	GROUND_ANIM_PROCESS_INTERVAL_TICKS = 2
};

// SIZE 0x12cc
class CGroundAnim {
public:
	CGroundAnim();
	bool Check(const tCoord3d& p_coordinate);
	bool CheckAllAnims();
	int ExportCoordinates(tCoord3d* p_records);
	void Add(const tCoord3d& p_coordinate, unsigned short p_startFrame, unsigned short p_endFrame);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();
	~CGroundAnim();
	void RemoveAtCoordinate(const tCoord3d& p_coordinate);
	void AddStaticGroundAnim(const tCoord3d& p_coordinate);

private:
	unsigned int m_nextProcessTick;                        // 0x0000
	int m_count;                                           // 0x0004
	GroundAnimEntry m_entries[GROUND_ANIM_ENTRY_CAPACITY]; // 0x0008
	unsigned int m_needsValidation;                        // 0x12c8
};

#endif
