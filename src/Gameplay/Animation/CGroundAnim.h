#ifndef LEMBALL_AI_OBJECTS_CGROUNDANIM_H
#define LEMBALL_AI_OBJECTS_CGROUNDANIM_H

#include "Gameplay/Geometry/tCoord3d.h"

class CGround;

// SIZE 0x18
struct GroundAnimEntry {
	tCoord3d m_coordinate;             // 0x00
	unsigned short m_alignmentPadding; // 0x06
	CGround* m_mapCell;                // 0x08
	short m_currentFrame;              // 0x0c
	short m_startFrame;                // 0x0e
	short m_endFrame;                  // 0x10
	short m_direction;                 // 0x12
	unsigned int m_active;             // 0x14
};

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
