#ifndef LEMBALL_VISOS_FOUNDATION_CSTRING_H
#define LEMBALL_VISOS_FOUNDATION_CSTRING_H

// SIZE 0x08
class CString {
public:
	CString();
	CString(const char* p_text);
	CString(const CString& p_other);
	~CString();

	CString& operator=(const CString& p_other);
	CString& operator=(const char* p_text);
	CString operator+=(CString& p_other);
	CString operator+=(const char* p_text);

	int getlength();
	CString lower();
	void SetCapacity(int p_capacity);
	char* GetText();
	void SetTextPointer(char* p_text);

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
