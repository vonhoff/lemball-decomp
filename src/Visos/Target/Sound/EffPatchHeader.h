#ifndef LEMBALL_VISOS_TARGET_SOUND_EFFPATCHHEADER_H
#define LEMBALL_VISOS_TARGET_SOUND_EFFPATCHHEADER_H

struct EffPatchHeader {
	unsigned int m_signature;
	unsigned short m_formatVersion;
	char m_name[14];
	unsigned short m_waveCount;
	unsigned short m_unk16;
	unsigned int m_unk18;
};

#endif
