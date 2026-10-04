#ifndef LEMBALL_CONTROL_SUPPORT_TPREVIEWDATA_H
#define LEMBALL_CONTROL_SUPPORT_TPREVIEWDATA_H

// SIZE 0x2a
struct tPreviewData {
	unsigned short m_playerCount;          // 0x00
	unsigned short m_lemmingCount;         // 0x02
	unsigned short m_opponentLemmingCount; // 0x04
	unsigned short m_timeLimit;            // 0x06
	char m_name[33];                       // 0x08
};

#endif
