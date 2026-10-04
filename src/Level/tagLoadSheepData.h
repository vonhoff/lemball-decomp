#ifndef LEMBALL_AI_BASE_TAGLOADSHEEPDATA_H
#define LEMBALL_AI_BASE_TAGLOADSHEEPDATA_H

// SIZE 0x06
struct tagLoadSheepData {
	unsigned char m_sheepCount;     // 0x00
	unsigned char m_formationIndex; // 0x01
	unsigned short m_x;             // 0x02
	unsigned short m_y;             // 0x04
};

#endif
