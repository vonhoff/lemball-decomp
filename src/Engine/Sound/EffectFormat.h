#ifndef LEMBALL_ENGINE_SOUND_EFFECTFORMAT_H
#define LEMBALL_ENGINE_SOUND_EFFECTFORMAT_H

enum {
	EFFECT_PATCH_SUPPORTED_WAVE_COUNT = 1
};

struct EffPatchHeader {
	unsigned int m_signature;
	unsigned short m_formatVersion;
	char m_name[14];
	unsigned short m_waveCount;
	unsigned short m_unk16;
	unsigned int m_unk18;
};

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

enum {
	PCM_8BIT_SIGN_BIT_MASK = 0x80
};

unsigned short SwapBytes16(unsigned short p_value);
unsigned int SwapBytes32(unsigned int p_value);

#endif
