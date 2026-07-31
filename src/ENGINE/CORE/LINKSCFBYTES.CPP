#include "ENGINE/CORE/COMMON.H"
#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/MEDIA/VSSTRM.H"

struct EffStreamU16FieldView {
	char m_abReserved00[0x30];
	unsigned short m_nValue30;

	void ReadValue(void);
};

// FUNCTION: LEMBALL 0x00417d30
void EffStreamU16FieldView::ReadValue(void)
{
	m_nValue30 = ((VsNetEffStreamBase*) this)->ReadEffStreamU16BEValue();
}

struct LevelModeValuesView {
	char m_abReserved00[0x110];
	int m_anValues110[1];

	int GetValue(int nIndex);
};

// FUNCTION: LEMBALL 0x00418c90
int LevelModeValuesView::GetValue(int nIndex)
{
	return m_anValues110[nIndex];
}

// FUNCTION: LEMBALL 0x00420b50
unsigned int ReadLittleEndianU32FromBytes(const unsigned char* pBytes)
{
	unsigned int nValue;

	nValue = pBytes[3];
	nValue <<= 16;
	nValue |= pBytes[1];
	nValue <<= 8;
	nValue |= (unsigned int) pBytes[2] << 16;
	nValue |= pBytes[0];
	return nValue;
}
