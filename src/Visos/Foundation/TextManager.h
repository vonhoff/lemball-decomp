#ifndef LEMBALL_SCAFFOLD_VISOS_FOUNDATION_TEXTMANAGER_H
#define LEMBALL_SCAFFOLD_VISOS_FOUNDATION_TEXTMANAGER_H

#include "../../Common.h"
#include "String.h" // complete type

// SIZE 0x24
class TextManager {
public:
	ResFont* GetFont(unsigned long p_fontId);
	TextManager(unsigned long p_arg0, int p_arg1, int p_arg2, unsigned int p_arg3);
	void DrawString(Gdi* p_gdi,
					VsPoint& p_position,
					const VsSize& p_advance,
					unsigned long p_fontId,
					String p_text,
					unsigned long p_flags,
					Remap* p_remap);
	void DrawString(Gdi* p_gdi,
					VsPoint& p_position,
					const VsSize& p_advance,
					unsigned long p_fontId,
					char* p_text,
					unsigned long p_flags,
					Remap* p_remap);
	void LoadFont(unsigned long p_fontId);
	void ResetPrimitives();
	void UnLoadFont(unsigned long p_fontId);
	~TextManager();
	TextManager();

private:
	ResFont** m_fonts;              // 0x00
	short* m_fontIndices;           // 0x04
	unsigned int m_fontCapacity;    // 0x08
	unsigned int m_fontIdCount;     // 0x0c
	unsigned int m_loadedFontCount; // 0x10
	unsigned int m_primitiveCount;  // 0x14
	unsigned int m_nextPrimitive;   // 0x18
	unsigned int m_reserved;        // 0x1c
	Text** m_textPrimitives;        // 0x20
};

#endif
