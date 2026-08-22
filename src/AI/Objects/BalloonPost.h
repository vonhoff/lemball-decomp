#ifndef LEMBALL_SCAFFOLD_AI_OBJECTS_BALLOONPOST_H
#define LEMBALL_SCAFFOLD_AI_OBJECTS_BALLOONPOST_H

#include "../../Common.h"
#include "../Base/AiCoord.h" // complete type

// SIZE 0x4c
class BalloonPost {
public:
	BalloonPost(Ai* p_arg0);
	bool FindPost(eObjectType p_objectType, AiCoord& p_position);
	int GetViewData(ViewData* p_viewData);
	unsigned int Process();
	unsigned short LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Restart();
	~BalloonPost();

private:
	unsigned short m_activeMask; // 0x00
	AiCoord m_positions[4];      // 0x04
	TheBalloonPost* m_posts[4];  // 0x34
	Ai* m_ai;                    // 0x44
	Map* m_map;                  // 0x48
};

#endif
