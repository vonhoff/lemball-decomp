#include "EditString.h"

#include <string.h>

#pragma intrinsic(strcpy, strlen)

// 68K 0x108062e4 __ct__11CEditStringFi
// STUB: LEMBALL 0x00453150
EditString::EditString(int p_arg0)
{
}

// 68K 0x10806336 __apl__11CEditStringFc
// STUB: LEMBALL 0x00453180
EditString& EditString::operator+=(char p_character)
{
	return *(EditString*) 0;
}

// 68K 0x10806384 __as__11CEditStringFPCc
// FUNCTION: LEMBALL 0x00453200
EditString& EditString::operator=(const char* p_text)
{
	strcpy(m_text, p_text);
	m_length = strlen(m_text);
	return *this;
}
