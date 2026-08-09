#ifndef LEMBALL_AI_CBALLOONPOST_H
#define LEMBALL_AI_CBALLOONPOST_H

class CBalloon;

// Aerial balloon post: active mask, four fixed-point post positions, four child balloons.
class CBalloonPost {
public:
	short m_wActiveMask00;
	char m_abReserved02[2];
	int m_anPostPos04[4][3];     // +0x04 4 posts x (x,y,z) fixed point
	CBalloon* m_apBalloons34[4]; // +0x34 the 4 child balloon objects
	void* m_pAI44;
	void* m_pLevelTileGrid48;

	void Process(void);
	void LoadLevel(unsigned short* pStream);
	unsigned int FindPost(unsigned int subtype, void** pOut);
};

#endif
