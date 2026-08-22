#ifndef LEMBALL_VISOS_FOUNDATION_STRING_H
#define LEMBALL_VISOS_FOUNDATION_STRING_H

#include "../../Common.h"

// SIZE 0x08
class String {
public:
	String Lower();
	String operator+=(char* p_text);
	String operator+=(const class String& p_other);
	String& operator=(char* p_text);
	String& operator=(const class String& p_other);
	String();
	String(char* p_arg0);
	String(class String& p_arg0);
	int Getlength();
	~String();

private:
	char* m_text;   // 0x00
	int m_capacity; // 0x04
};

#endif
