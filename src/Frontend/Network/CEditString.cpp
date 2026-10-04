#include "CEditString.h"

#include <string.h>

#pragma intrinsic(strcpy, strlen)

// FUNCTION: LEMBALL 0x00453150
CEditString::CEditString(int p_capacity)
{
	m_text = (char*) operator new(p_capacity + 1);
	m_capacity = p_capacity;
	m_maxLength = p_capacity;
	m_length = 0;
	m_text[0] = 0;
}

// FUNCTION: LEMBALL 0x00453180
CEditString& CEditString::operator+=(char p_character)
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

// FUNCTION: LEMBALL 0x004531b0
CEditString& CEditString::operator=(const CEditString& p_other)
{
	strcpy(m_text, p_other.m_text);
	m_length = strlen(m_text);
	return *this;
}

// FUNCTION: LEMBALL 0x00453200
CEditString& CEditString::operator=(const char* p_text)
{
	strcpy(m_text, p_text);
	m_length = strlen(m_text);
	return *this;
}
