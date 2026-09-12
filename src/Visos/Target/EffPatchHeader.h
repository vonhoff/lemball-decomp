#ifndef LEMBALL_VISOS_TARGET_EFFPATCHHEADER_H
#define LEMBALL_VISOS_TARGET_EFFPATCHHEADER_H

struct EffPatchHeader {
	unsigned int m_unk0;
	unsigned short m_unk4;
	char m_name[14];
	unsigned short m_waveCount;
	unsigned short m_unk16;
	unsigned int m_unk18;
};

#endif
