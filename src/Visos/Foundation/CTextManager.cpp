#include "CTextManager.h"

#include "../Resources/CResFONT.h"
#include "CCopyText.h"
#include "CText.h"
#include "CVsSize.h"
#include "Visos/Foundation/CString.h"

// FUNCTION: LEMBALL 0x00469c60
CTextManager::CTextManager(unsigned long p_fontIdCount,
						   int p_fontCapacity,
						   int p_primitiveCount,
						   unsigned int p_maxStringLen)
{
	m_nextPrimitive = 0;
	m_fontIdCount = p_fontIdCount;
	m_loadedFontCount = 0;
	m_fontCapacity = p_fontCapacity;
	m_fonts = new CResFONT*[m_fontCapacity];
	m_fontIndices = new short[m_fontIdCount];
	for (int i = 0; i < (int) m_fontCapacity; i++) {
		m_fonts[i] = 0;
	}
	for (int j = 0; j < (int) m_fontIdCount; j++) {
		m_fontIndices[j] = (short) m_fontCapacity;
	}
	m_primitiveCount = p_primitiveCount;
	m_textPrimitives = new CText*[p_primitiveCount];
	if (p_maxStringLen != 0) {
		for (int k = 0; k < (int) m_primitiveCount; k++) {
			m_textPrimitives[k] = new CCopyText(p_maxStringLen);
		}
	}
	else {
		for (int k = 0; k < (int) m_primitiveCount; k++) {
			m_textPrimitives[k] = new CText();
		}
	}
	m_nextPrimitive = 0;
}

// FUNCTION: LEMBALL 0x00469e20
CTextManager::~CTextManager()
{
	int slot = 0;
	if (m_loadedFontCount != 0) {
		for (int unloaded = 0; unloaded < (int) m_loadedFontCount; unloaded++) {
			while (m_fonts[slot] == 0) {
				slot++;
			}
			m_fonts[slot]->UnLoad();
			slot++;
		}
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

// FUNCTION: LEMBALL 0x00469eb0
void CTextManager::LoadFont(unsigned long p_fontId)
{
	CResFONT** fonts;
	int slot = 0;
	fonts = m_fonts;
	if (fonts[0] != 0) {
		do {
			slot++;
		} while (fonts[slot] != 0);
	}
	fonts[slot] = CResFONT::Load(p_fontId);
	m_fontIndices[p_fontId] = (short) slot;
	m_loadedFontCount++;
}

// FUNCTION: LEMBALL 0x00469ef0
CResFONT* CTextManager::GetFont(unsigned long p_fontId)
{
	return m_fonts[m_fontIndices[p_fontId]];
}

// FUNCTION: LEMBALL 0x00469f10
void CTextManager::UnLoadFont(unsigned long p_fontId)
{
	m_fonts[m_fontIndices[p_fontId]]->UnLoad();
	m_fonts[m_fontIndices[p_fontId]] = 0;
	m_loadedFontCount--;
}

// FUNCTION: LEMBALL 0x00469f50
void CTextManager::DrawString(CGDI* p_gdi,
							  CVsPoint& p_position,
							  const CVsSize& p_advance,
							  unsigned long p_fontId,
							  char* p_text,
							  unsigned long p_flags,
							  CRemap* p_remap)
{
	CText* text = m_textPrimitives[m_nextPrimitive++];
	if (p_advance.m_width != 0 || p_advance.m_height != 0) {
		text->m_offsetX = p_advance.m_width;
		text->m_offsetY = p_advance.m_height;
		p_flags |= 0x200;
	}
	text->Set(p_position, m_fonts[m_fontIndices[p_fontId]], p_text, p_flags, p_remap);
	text->Draw(p_gdi);
}

// FUNCTION: LEMBALL 0x00469fd0
void CTextManager::DrawString(CGDI* p_gdi,
							  CVsPoint& p_position,
							  const CVsSize& p_advance,
							  unsigned long p_fontId,
							  CString p_text,
							  unsigned long p_flags,
							  CRemap* p_remap)
{
	CText* text = m_textPrimitives[m_nextPrimitive++];
	if (p_advance.m_width != 0 || p_advance.m_height != 0) {
		text->m_offsetX = p_advance.m_width;
		text->m_offsetY = p_advance.m_height;
		p_flags |= 0x200;
	}
	text->Set(p_position, m_fonts[m_fontIndices[p_fontId]], p_text, p_flags, p_remap);
	text->Draw(p_gdi);
}

// FUNCTION: LEMBALL 0x0046a070
void CTextManager::ResetPrimitives()
{
	m_nextPrimitive = 0;
}
