#include "EditString.h"

#include <new.h>
#include <string.h>

#pragma intrinsic(strcpy, strlen)

// 68K 0x108062e4 __ct__11CEditStringFi
// FUNCTION: LEMBALL 0x00453150
EditString::EditString(int p_capacity)
{
	m_text = (char*) operator new(p_capacity + 1);
	m_capacity = p_capacity;
	m_maxLength = p_capacity;
	m_length = 0;
	m_text[0] = 0;
}

// 68K 0x10806336 __apl__11CEditStringFc
// FUNCTION: LEMBALL 0x00453180
EditString& EditString::operator+=(char p_character)
{
	int length;

	if (m_maxLength > m_length) {
		m_text[m_length] = p_character;
		length = m_length + 1;
		m_length = length;
		m_text[length] = 0;
	}
	return *this;
}

// 68K 0x10806384 __as__11CEditStringFPCc
// FUNCTION: LEMBALL 0x00453200
EditString& EditString::operator=(const char* p_text)
{
	strcpy(m_text, p_text);
	m_length = strlen(m_text);
	return *this;
}
