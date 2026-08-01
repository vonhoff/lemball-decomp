#include "ENGINE/CORE/VSINIT.H"

#include <string.h>

extern void* AllocateVSMemBlock(unsigned int cbBlock);

class BoundedTextBuffer {
public:
	int m_nCapacity00;
	int m_nLength04;
	int m_nInitialCapacity08;
	char* m_pszText0C;

	BoundedTextBuffer* Initialize(int nCapacity);
	BoundedTextBuffer* Append(char ch);
	BoundedTextBuffer* Assign(const BoundedTextBuffer* pSource);
	BoundedTextBuffer* Copy(const char* pszText);
};

// FUNCTION: LEMBALL 0x00453150
BoundedTextBuffer* BoundedTextBuffer::Initialize(int nCapacity)
{
	m_pszText0C = (char*) AllocateVSMemBlock(nCapacity + 1);
	m_nInitialCapacity08 = nCapacity;
	m_nCapacity00 = nCapacity;
	m_nLength04 = 0;
	m_pszText0C[0] = 0;
	return this;
}

// FUNCTION: LEMBALL 0x00453180
BoundedTextBuffer* BoundedTextBuffer::Append(char ch)
{
	if (m_nLength04 < m_nCapacity00) {
		m_pszText0C[m_nLength04] = ch;
		++m_nLength04;
		m_pszText0C[m_nLength04] = 0;
	}
	return this;
}

// FUNCTION: LEMBALL 0x004531b0
BoundedTextBuffer* BoundedTextBuffer::Assign(const BoundedTextBuffer* pSource)
{
	strcpy(m_pszText0C, pSource->m_pszText0C);
	m_nLength04 = strlen(m_pszText0C);
	return this;
}

// FUNCTION: LEMBALL 0x00453200
BoundedTextBuffer* BoundedTextBuffer::Copy(const char* pszText)
{
	strcpy(m_pszText0C, pszText);
	m_nLength04 = strlen(m_pszText0C);
	return this;
}
