#include "CString.h"

#include "CVsOStream.h"

#include <ctype.h>
#include <string.h>

// GLOBAL: LEMBALL 0x004a44b4
// __mb_cur_max

// GLOBAL: LEMBALL 0x004a42a8
// _pctype

// FUNCTION: LEMBALL 0x0046e410
CString::CString()
{
	m_capacity = 1;
	m_text = new char[1];
	m_text[0] = '\0';
}

// FUNCTION: LEMBALL 0x0046e430
CString::CString(const char* p_text)
{
	m_capacity = strlen(p_text) + 1;
	m_text = new char[m_capacity];
	strcpy(m_text, p_text);
}

// FUNCTION: LEMBALL 0x0046e480
CString::CString(const CString& p_other)
{
	m_capacity = p_other.m_capacity;
	m_text = new char[m_capacity];
	strcpy(m_text, p_other.m_text);
}

// FUNCTION: LEMBALL 0x0046e500
CString::~CString()
{
	delete[] m_text;
}

// FUNCTION: LEMBALL 0x0046e510
CString& CString::operator=(const CString& p_other)
{
	delete[] m_text;
	m_text = new char[p_other.m_capacity];
	strcpy(m_text, p_other.m_text);
	m_capacity = p_other.m_capacity;
	return *this;
}

// FUNCTION: LEMBALL 0x0046e570
CString& CString::operator=(const char* p_text)
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

// FUNCTION: LEMBALL 0x0046e5d0
CString CString::operator+=(CString& p_other)
{
	CString temp;
	delete[] temp.m_text;
	int length = strlen(m_text);
	int newCap = length + strlen(p_other.m_text) + 1;
	temp.m_text = new char[newCap];
	temp.m_capacity = newCap;
	strcpy(temp.m_text, m_text);
	strcat(temp.m_text, p_other.m_text);

	delete[] m_text;
	m_text = new char[newCap];
	m_capacity = newCap;
	strcpy(m_text, temp.m_text);
	return *this;
}

// FUNCTION: LEMBALL 0x0046e6e0
CString CString::operator+=(const char* p_text)
{
	CString temp;
	delete[] temp.m_text;
	int newCap = strlen(m_text) + strlen(p_text) + 1;
	temp.m_text = new char[newCap];
	temp.m_capacity = newCap;
	strcpy(temp.m_text, m_text);
	strcat(temp.m_text, p_text);

	delete[] m_text;
	m_text = new char[newCap];
	m_capacity = newCap;
	strcpy(m_text, temp.m_text);
	return *this;
}

// FUNCTION: LEMBALL 0x0046e7f0
CVsOStream& operator<<(CVsOStream& p_stream, CString& p_string)
{
	p_stream << p_string.m_text;
	return p_stream;
}

// FUNCTION: LEMBALL 0x0046e810
CString operator+(CString& p_left, CString& p_right)
{
	CString result;
	delete[] result.GetText();
	int length = p_right.Getlength();
	length = length + p_left.Getlength() + 1;
	result.SetTextPointer(new char[length]);
	result.SetCapacity(length);
	strcpy(result.GetText(), p_left.GetText());
	strcat(result.GetText(), p_right.GetText());
	return result;
}

// FUNCTION: LEMBALL 0x0046e900
CString operator+(CString& p_left, const char* p_right)
{
	int capacity;
	CString result;
	delete[] result.GetText();
	int length = p_left.Getlength();
	capacity = length + strlen(p_right) + 1;
	result.SetTextPointer(new char[capacity]);
	result.m_capacity = capacity;
	strcpy(result.m_text, p_left.m_text);
	strcat(result.m_text, p_right);
	return result;
}

// FUNCTION: LEMBALL 0x0046ef00
int CString::Getlength()
{
	return strlen(m_text);
}

// FUNCTION: LEMBALL 0x0046efa0
CString CString::Lower()
{
	char* p = m_text;
	int len = strlen(p);
	if (len > 0) {
		do {
			if (isupper(*p)) {
				*p = (char) tolower(*p);
			}
			p++;
			len--;
		} while (len != 0);
	}
	return *this;
}

// FUNCTION: LEMBALL 0x0046f030
void CString::SetCapacity(int p_capacity)
{
	m_capacity = p_capacity;
}

// FUNCTION: LEMBALL 0x0046f040
char* CString::GetText()
{
	return m_text;
}

// FUNCTION: LEMBALL 0x0046f050
void CString::SetTextPointer(char* p_text)
{
	m_text = p_text;
}
