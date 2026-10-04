#ifndef LEMBALL_AI_BASE_TAGLOADENEMYDATA_H
#define LEMBALL_AI_BASE_TAGLOADENEMYDATA_H

// SIZE 0x0c
struct tagLoadEnemyData {
	unsigned short m_x;      // 0x00
	unsigned short m_y;      // 0x02
	unsigned char m_facing;  // 0x04
	unsigned char m_action0; // 0x05
	unsigned char m_rule0;   // 0x06
	unsigned char m_action1; // 0x07
	unsigned char m_rule1;   // 0x08
	unsigned char m_action2; // 0x09
	unsigned char m_rule2;   // 0x0a
};

#endif
