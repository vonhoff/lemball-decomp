#ifndef LEMBALL_VISOS_FOUNDATION_STRING_H
#define LEMBALL_VISOS_FOUNDATION_STRING_H

#include "../../Common.h"

// SIZE 0x08
class String {
public:
	String();
	String(const char* p_text);
	String(const String& p_other);
	~String();

	String& operator=(const String& p_other);
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

// FUNCTION: LEMBALL 0x0047fc00
// __isctype

// FUNCTION: LEMBALL 0x00480060
// _tolower


#endif

