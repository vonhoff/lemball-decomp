#ifndef LEMBALL_VISOS_TARGET_SOUND_EFFWAVEHEADER_H
#define LEMBALL_VISOS_TARGET_SOUND_EFFWAVEHEADER_H

struct EffWaveHeader {
	unsigned int m_signature;
	unsigned short m_formatVersion;
	unsigned short m_unk6;
	unsigned int m_length;
	unsigned int m_loopStart;
	unsigned int m_loopEnd;
	unsigned int m_sampleRate;
	unsigned int m_pitchFrequencies[3];
	unsigned int m_unk24[2];
	unsigned short m_playbackFlags;
	unsigned short m_unk2e;
	unsigned int m_unk30[2];
};

#endif
