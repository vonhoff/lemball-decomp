#ifndef LEMBALL_AI_CBASEREMAP_H
#define LEMBALL_AI_CBASEREMAP_H

// CBaseRemap view modeled on the Macintosh blueprint class `CBaseRemap`.
// 8-byte remap object: identity table buffer at +0x00, remap/palette table at +0x04.
class CBaseRemap {
public:
	unsigned char* m_pTable00;    // 0x00 identity table buffer
	void* m_pRemapObject04;       // 0x04 remap/palette table object

	void MapRemap(const unsigned char* param_1);
	void CalculateGreyScale(void);
};

#endif
