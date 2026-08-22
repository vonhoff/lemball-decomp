#ifndef LEMBALL_FRONTEND_SUPPORT_EDITSTRING_H
#define LEMBALL_FRONTEND_SUPPORT_EDITSTRING_H

#include "../../Common.h"

// SIZE 0x10
class EditString {
public:
	EditString& operator+=(char p_character);
	EditString& operator=(const char* p_text);
	EditString(int p_arg0);

private:
	int m_maxLength; // 0x00
	int m_length;    // 0x04
	int m_capacity;  // 0x08
	char* m_text;    // 0x0c
};

#endif
