#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

#include <string.h>

extern void* AllocateVSMemBlock(unsigned int cbBlock);

class CEditString {
public:
	int m_nCapacity00;
	int m_nLength04;
	int m_nInitialCapacity08;
	char* m_pszText0C;

	CEditString(int nCapacity);
	CEditString* operator+=(char ch);
	CEditString* Assign(const CEditString* pSource);
	CEditString* operator=(const char* pszText);
};

// FUNCTION: LEMBALL 0x00453150
CEditString::CEditString(int nCapacity)
{
	m_pszText0C = (char*) AllocateVSMemBlock(nCapacity + 1);
	m_nInitialCapacity08 = nCapacity;
	m_nCapacity00 = nCapacity;
	m_nLength04 = 0;
	m_pszText0C[0] = 0;
}

// FUNCTION: LEMBALL 0x00453180
CEditString* CEditString::operator+=(char ch)
{
	if (m_nLength04 < m_nCapacity00) {
		m_pszText0C[m_nLength04] = ch;
		++m_nLength04;
		m_pszText0C[m_nLength04] = 0;
	}
	return this;
}

// FUNCTION: LEMBALL 0x004531b0
CEditString* CEditString::Assign(const CEditString* pSource)
{
	strcpy(m_pszText0C, pSource->m_pszText0C);
	m_nLength04 = strlen(m_pszText0C);
	return this;
}

// FUNCTION: LEMBALL 0x00453200
CEditString* CEditString::operator=(const char* pszText)
{
	strcpy(m_pszText0C, pszText);
	m_nLength04 = strlen(m_pszText0C);
	return this;
}
