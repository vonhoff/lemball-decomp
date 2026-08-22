#ifndef LEMBALL_AI_OBJECTS_GROUNDANIM_H
#define LEMBALL_AI_OBJECTS_GROUNDANIM_H

#include "../../Common.h"
#include "GroundAnimEntry.h" // complete type

// SIZE 0x12cc
class GroundAnim {
public:
	GroundAnim();
	bool Check(const Coord3d& p_coordinate);
	bool CheckAllAnims();
	void Add(const Coord3d& p_coordinate, unsigned short p_startFrame, unsigned short p_endFrame);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();
	~GroundAnim();

private:
	unsigned int m_nextProcessTick; // 0x0000
	int m_count;                    // 0x0004
	GroundAnimEntry m_entries[200]; // 0x0008
	unsigned int m_needsValidation; // 0x12c8
};

#endif
