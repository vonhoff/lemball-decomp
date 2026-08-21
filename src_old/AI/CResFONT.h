#ifndef LEMBALL_CRESFONT_H
#define LEMBALL_CRESFONT_H

// Mac blueprint: CResFONT is a font resource manager holding a font-table
// object (0x78), an INT-code array (stride 0x54, 0x7c) and a ZRLE glyph array
// (stride 0x4c, 0x80).

class CResFONT {
public:
	int m_vtable00;                  // vtable pointer
	unsigned char m_abReserved04[0x74];

	unsigned char* m_pFontTable78;   // 0x78
	unsigned char* m_pIntCodeData7C; // 0x7c
	unsigned char* m_pGlyphData80;   // 0x80

	void AllocateResources(unsigned long cEntries);
	int DirectResources(unsigned long nIndex, unsigned char*& pIntCodeData, unsigned char*& pGlyphData);
	int DirectResources(unsigned long nIndex, unsigned char*& pGlyphData);
	int ForceLoadVram(unsigned long nIndex);
	void UnLoadResources(unsigned long nIndex, unsigned char fReleaseMode);
	void UnLoadVramData(unsigned long nIndex, unsigned char fReleaseMode);
	void OnLoad(void);
};

#endif // LEMBALL_CRESFONT_H
