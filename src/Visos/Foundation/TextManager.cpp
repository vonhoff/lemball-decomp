#include "TextManager.h"

TextManager::TextManager()
{
}

// 68K 0x102054aa __ct__12CTextManagerFUliiUi
// STUB: LEMBALL 0x00469c60
TextManager::TextManager(unsigned long p_arg0, int p_arg1, int p_arg2, unsigned int p_arg3)
{
}

// 68K 0x1020568a __dt__12CTextManagerFv
// STUB: LEMBALL 0x00469e20
TextManager::~TextManager()
{
}

// 68K 0x10205762 LoadFont__12CTextManagerFUl
// STUB: LEMBALL 0x00469eb0
void TextManager::LoadFont(unsigned long p_fontId)
{
}

// 68K 0x102057c2 GetFont__12CTextManagerFUl
// STUB: LEMBALL 0x00469ef0
ResFont* TextManager::GetFont(unsigned long p_fontId)
{
	return 0;
}

// 68K 0x10205814 UnLoadFont__12CTextManagerFUl
// STUB: LEMBALL 0x00469f10
void TextManager::UnLoadFont(unsigned long p_fontId)
{
}

// 68K 0x10205874 DrawString__12CTextManagerFP4CGDIR8CVSPointRC7CVSSizeUlPcUlP6CRemap
// STUB: LEMBALL 0x00469f50
void TextManager::DrawString(Gdi* p_gdi,
							 VsPoint& p_position,
							 const VsSize& p_advance,
							 unsigned long p_fontId,
							 char* p_text,
							 unsigned long p_flags,
							 Remap* p_remap)
{
}

// 68K 0x10205946 DrawString__12CTextManagerFP4CGDIR8CVSPointRC7CVSSizeUl7CStringUlP6CRemap
// STUB: LEMBALL 0x00469fd0
void TextManager::DrawString(Gdi* p_gdi,
							 VsPoint& p_position,
							 const VsSize& p_advance,
							 unsigned long p_fontId,
							 String p_text,
							 unsigned long p_flags,
							 Remap* p_remap)
{
}

// 68K 0x10205a3a ResetPrimitives__12CTextManagerFv
// STUB: LEMBALL 0x0046a070
void TextManager::ResetPrimitives()
{
}
