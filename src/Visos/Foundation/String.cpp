#include "String.h"

#include <ctype.h>
#include <string.h>

// 68K 0x10200e84 __ct__7CStringFv
// FUNCTION: LEMBALL 0x0046e410
String::String()
{
	m_capacity = 1;
	m_text = new char[1];
	m_text[0] = '\0';
}

// 68K 0x10200ec4 __ct__7CStringFPc
// FUNCTION: LEMBALL 0x0046e430
String::String(char* p_text)
{
	m_capacity = strlen(p_text) + 1;
	m_text = new char[m_capacity];
	strcpy(m_text, p_text);
}

// 68K 0x10200f1e __ct__7CStringFR7CString
// FUNCTION: LEMBALL 0x0046e480
String::String(String& p_other)
{
	m_capacity = p_other.m_capacity;
	m_text = new char[m_capacity];
	strcpy(m_text, p_other.m_text);
}

// 68K 0x10200f78 __dt__7CStringFv
// FUNCTION: LEMBALL 0x0046e500
String::~String()
{
	delete[] m_text;
}

// 68K 0x10200fbc __as__7CStringFR7CString
// FUNCTION: LEMBALL 0x0046e510
String& String::operator=(String& p_other)
{
	delete[] m_text;
	m_text = new char[p_other.m_capacity];
	strcpy(m_text, p_other.m_text);
	m_capacity = p_other.m_capacity;
	return *this;
}

// 68K 0x10201024 __as__7CStringFPCc
// FUNCTION: LEMBALL 0x0046e570
String& String::operator=(const char* p_text)
{
	int len = strlen(p_text) + 1;
	if (m_capacity < len) {
		delete[] m_text;
		m_text = new char[len];
		m_capacity = len;
	}
	strcpy(m_text, p_text);
	return *this;
}

// 68K 0x1020108e __apl__7CStringFR7CString
// FUNCTION: LEMBALL 0x0046e5d0
String String::operator+=(String& p_other)
{
	String temp;
	delete[] temp.m_text;
	int newCap = strlen(m_text) + strlen(p_other.m_text) + 1;
	temp.m_text = new char[newCap];
	strcpy(temp.m_text, m_text);
	strcat(temp.m_text, p_other.m_text);
	temp.m_capacity = newCap;

	delete[] m_text;
	m_text = new char[newCap];
	m_capacity = newCap;
	strcpy(m_text, temp.m_text);
	return *this;
}

// 68K 0x1020116c __apl__7CStringFPCc
// FUNCTION: LEMBALL 0x0046e6e0
String String::operator+=(const char* p_text)
{
	String temp;
	delete[] temp.m_text;
	int newCap = strlen(m_text) + strlen(p_text) + 1;
	temp.m_text = new char[newCap];
	strcpy(temp.m_text, m_text);
	strcat(temp.m_text, p_text);
	temp.m_capacity = newCap;

	delete[] m_text;
	m_text = new char[newCap];
	m_capacity = newCap;
	strcpy(m_text, temp.m_text);
	return *this;
}

// 68K 0x1020127a getlength__7CStringFv
// FUNCTION: LEMBALL 0x0046ef00
int String::Getlength()
{
	return strlen(m_text);
}

// 68K 0x102012ae lower__7CStringFv
// FUNCTION: LEMBALL 0x0046efa0
String String::Lower()
{
	int len = strlen(m_text);
	for (int i = 0; i < len; ++i) {
		if (isupper((unsigned char) m_text[i])) {
			m_text[i] = (char) tolower((unsigned char) m_text[i]);
		}
	}
	return *this;
}
