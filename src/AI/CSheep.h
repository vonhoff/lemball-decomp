#ifndef LEMBALL_AI_CSHEEP_H
#define LEMBALL_AI_CSHEEP_H

// CSheep view modeled on the Macintosh blueprint class `CSheep`.
class CSheep {
public:
	void* m_pVtable00;
	char m_abReserved04[0x10];
	int m_anBounds14[6];
	char m_abReserved2C[0x14];
	int m_nSpawnX40;
	int m_nSpawnY44;
	int m_nSpawnZ48;
	char m_abReserved4C[0x50];
	int m_nWorldX9C;
	int m_nWorldYA0;
	int m_nWorldZA4;
	char m_abReservedA8[0x0c];
	short m_sHeadingB4;
	short m_sSavedHeadingB6;

	void Restart(void);
	void HitMine(void);
};

#endif
