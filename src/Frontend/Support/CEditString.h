#ifndef LEMBALL_FRONTEND_SUPPORT_CEDITSTRING_H
#define LEMBALL_FRONTEND_SUPPORT_CEDITSTRING_H

// SIZE 0x10
class CEditString {
public:
	CEditString& operator+=(char p_character);
	CEditString& operator=(const CEditString& p_other);
	CEditString& operator=(const char* p_text);
	CEditString(int p_arg0);

	friend class CNetworkOptionsDrawer;

private:
	int m_maxLength; // 0x00
	int m_length;    // 0x04
	int m_capacity;  // 0x08
	char* m_text;    // 0x0c
};

#endif
