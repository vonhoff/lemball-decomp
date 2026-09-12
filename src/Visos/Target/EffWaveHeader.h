#ifndef LEMBALL_VISOS_TARGET_EFFWAVEHEADER_H
#define LEMBALL_VISOS_TARGET_EFFWAVEHEADER_H

struct EffWaveHeader {
	unsigned int m_unk0;
	unsigned short m_unk4;
	unsigned short m_unk6;
	unsigned int m_length;
	unsigned int m_unk0c;
	unsigned int m_unk10;
	unsigned int m_sampleRate;
	unsigned int m_unk18;
	unsigned int m_unk1c[7];
};

#endif
