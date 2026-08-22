#ifndef LEMBALL_VISOS_FOUNDATION_STRING_H
#define LEMBALL_VISOS_FOUNDATION_STRING_H

#include "../../Common.h"

// SIZE 0x08
class String {
public:
	String();
	String(char* p_text);
	String(String& p_other);
	~String();

	String& operator=(String& p_other);
	String& operator=(const char* p_text);
	String operator+=(String& p_other);
	String operator+=(const char* p_text);

	int Getlength();
	String Lower();

	const char* GetText() const { return m_text; }
	operator const char*() const { return m_text; }

	char* m_text;   // 0x00
	int m_capacity; // 0x04
};

#endif
