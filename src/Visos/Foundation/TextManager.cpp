#include "TextManager.h"

#include "../Resources/ResFont.h"
#include "CopyText.h"
#include "Text.h"
#include "VsSize.h"

// 68K 0x102054aa __ct__12CTextManagerFUliiUi
// FUNCTION: LEMBALL 0x00469c60
TextManager::TextManager(unsigned long p_fontIdCount,
						 int p_fontCapacity,
						 int p_primitiveCount,
						 unsigned int p_maxStringLen)
{
	m_nextPrimitive = 0;
	m_fontIdCount = p_fontIdCount;
	m_loadedFontCount = 0;
	m_fontCapacity = p_fontCapacity;
	m_fonts = new ResFont*[p_fontCapacity];
	m_fontIndices = new short[p_fontIdCount];
	for (int i = 0; i < p_fontCapacity; i++) {
		m_fonts[i] = 0;
	}
	for (unsigned int j = 0; j < p_fontIdCount; j++) {
		m_fontIndices[j] = (short) p_fontCapacity;
	}
	m_primitiveCount = p_primitiveCount;
	m_textPrimitives = new Text*[p_primitiveCount];
	if (p_maxStringLen != 0) {
		for (int k = 0; k < p_primitiveCount; k++) {
			m_textPrimitives[k] = new CopyText(p_maxStringLen);
		}
	}
	else {
		for (int k = 0; k < p_primitiveCount; k++) {
			m_textPrimitives[k] = new Text();
		}
	}
}

// 68K 0x1020568a __dt__12CTextManagerFv
// FUNCTION: LEMBALL 0x00469e20
TextManager::~TextManager()
{
	if ((int) m_loadedFontCount > 0) {
		int slot = 0;
		int unloaded = 0;
		do {
			while (m_fonts[slot] == 0) {
				slot++;
			}
			m_fonts[slot]->UnLoad();
			slot++;
			unloaded++;
		} while ((int) m_loadedFontCount > unloaded);
	}
	if (m_fonts != 0) {
		delete[] m_fonts;
	}
	if (m_fontIndices != 0) {
		delete[] m_fontIndices;
	}
	if (m_textPrimitives != 0) {
		for (int j = 0; j < (int) m_primitiveCount; j++) {
			delete m_textPrimitives[j];
		}
		delete[] m_textPrimitives;
	}
}

// 68K 0x10205762 LoadFont__12CTextManagerFUl
// FUNCTION: LEMBALL 0x00469eb0
void TextManager::LoadFont(unsigned long p_fontId)
{
	int slot = 0;
	ResFont** fonts = m_fonts;
	if (fonts[0] != 0) {
		do {
			slot++;
		} while (fonts[slot] != 0);
	}
	fonts[slot] = ResFont::Load(p_fontId);
	m_fontIndices[p_fontId] = (short) slot;
	m_loadedFontCount++;
}

// 68K 0x102057c2 GetFont__12CTextManagerFUl
// FUNCTION: LEMBALL 0x00469ef0
ResFont* TextManager::GetFont(unsigned long p_fontId)
{
	return m_fonts[m_fontIndices[p_fontId]];
}

// 68K 0x10205814 UnLoadFont__12CTextManagerFUl
// FUNCTION: LEMBALL 0x00469f10
void TextManager::UnLoadFont(unsigned long p_fontId)
{
	m_fonts[m_fontIndices[p_fontId]]->UnLoad();
	m_fonts[m_fontIndices[p_fontId]] = 0;
	m_loadedFontCount--;
}

// 68K 0x10205874 DrawString__12CTextManagerFP4CGDIR8CVSPointRC7CVSSizeUlPcUlP6CRemap
// FUNCTION: LEMBALL 0x00469f50
void TextManager::DrawString(Gdi* p_gdi,
							 VsPoint& p_position,
							 const VsSize& p_advance,
							 unsigned long p_fontId,
							 char* p_text,
							 unsigned long p_flags,
							 Remap* p_remap)
{
	Text* text = m_textPrimitives[m_nextPrimitive++];
	if (p_advance.m_width != 0 || p_advance.m_height != 0) {
		text->m_offsetX = p_advance.m_width;
		text->m_offsetY = p_advance.m_height;
		p_flags |= 0x200;
	}
	text->Set(p_position, m_fonts[m_fontIndices[p_fontId]], p_text, p_flags, p_remap);
	text->Draw(p_gdi);
}

// 68K 0x10205946 DrawString__12CTextManagerFP4CGDIR8CVSPointRC7CVSSizeUl7CStringUlP6CRemap
// FUNCTION: LEMBALL 0x00469fd0
void TextManager::DrawString(Gdi* p_gdi,
							 VsPoint& p_position,
							 const VsSize& p_advance,
							 unsigned long p_fontId,
							 String p_text,
							 unsigned long p_flags,
							 Remap* p_remap)
{
	Text* text = m_textPrimitives[m_nextPrimitive++];
	if (p_advance.m_width != 0 || p_advance.m_height != 0) {
		text->m_offsetX = p_advance.m_width;
		text->m_offsetY = p_advance.m_height;
		p_flags |= 0x200;
	}
	text->Set(p_position, m_fonts[m_fontIndices[p_fontId]], p_text, p_flags, p_remap);
	text->Draw(p_gdi);
}

// 68K 0x10205a3a ResetPrimitives__12CTextManagerFv
// FUNCTION: LEMBALL 0x0046a070
void TextManager::ResetPrimitives()
{
	m_nextPrimitive = 0;
}
