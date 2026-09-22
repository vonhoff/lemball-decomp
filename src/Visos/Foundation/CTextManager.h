#ifndef LEMBALL_VISOS_FOUNDATION_CTEXTMANAGER_H
#define LEMBALL_VISOS_FOUNDATION_CTEXTMANAGER_H

#include "CString.h" // complete type

class CGdi;
class CRemap;
class CResFont;
class CText;
struct CVsPoint;
struct CVsSize;

// SIZE 0x24
class CTextManager {
public:
	CResFont* GetFont(unsigned long p_fontId);
	CTextManager(unsigned long p_fontIdCount, int p_fontCapacity, int p_primitiveCount, unsigned int p_maxStringLen);
	void DrawString(CGdi* p_gdi,
					CVsPoint& p_position,
					const CVsSize& p_advance,
					unsigned long p_fontId,
					CString p_text,
					unsigned long p_flags,
					CRemap* p_remap);
	void DrawString(CGdi* p_gdi,
					CVsPoint& p_position,
					const CVsSize& p_advance,
					unsigned long p_fontId,
					char* p_text,
					unsigned long p_flags,
					CRemap* p_remap);
	void LoadFont(unsigned long p_fontId);
	void ResetPrimitives();
	void UnLoadFont(unsigned long p_fontId);
	~CTextManager();

private:
	CResFont** m_fonts;             // 0x00
	short* m_fontIndices;           // 0x04
	unsigned int m_fontCapacity;    // 0x08
	unsigned int m_fontIdCount;     // 0x0c
	unsigned int m_loadedFontCount; // 0x10
	unsigned int m_primitiveCount;  // 0x14
	unsigned int m_nextPrimitive;   // 0x18
	unsigned int m_reserved;        // 0x1c
	CText** m_textPrimitives;       // 0x20
};

#endif
