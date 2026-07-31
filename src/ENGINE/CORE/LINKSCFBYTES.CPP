#include "ENGINE/CORE/COMMON.H"
#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/MEDIA/VSSTRM.H"

struct LevelTileGridCell {
	int m_anValues[3];
};

struct LevelTileGridView {
	void* m_pReserved00;
	LevelTileGridCell* m_pCells04;
	int m_nWidth08;

	LevelTileGridCell* GetCell(int x, int y);
};

// FUNCTION: LEMBALL 0x0040eb70
LevelTileGridCell* LevelTileGridView::GetCell(int x, int y)
{
	return &m_pCells04[x + m_nWidth08 * y];
}

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

struct FourDwordValue {
	int m_anValues[4];

	FourDwordValue* CopyFrom(const int* pSource);
};

// FUNCTION: LEMBALL 0x004267a0
FourDwordValue* FourDwordValue::CopyFrom(const int* pSource)
{
	m_anValues[0] = pSource[0];
	m_anValues[1] = pSource[1];
	m_anValues[2] = pSource[2];
	m_anValues[3] = pSource[3];
	return this;
}

struct ThreeDwordPoint {
	int m_nX;
	int m_nY;
	int m_nZ;

	void CopyFixedPointToPixels(const int* pSource);
	ThreeDwordPoint* Initialize(const int* pX, const int* pY, const int* pZ);
};

// FUNCTION: LEMBALL 0x00429e50
void ThreeDwordPoint::CopyFixedPointToPixels(const int* pSource)
{
	m_nX = pSource[0] >> 12;
	m_nY = pSource[1] >> 12;
	m_nZ = pSource[2] >> 12;
}

// FUNCTION: LEMBALL 0x0042b9c0
void AddDwordValues(int* pDestination, const int* pLeft, const int* pRight)
{
	*pDestination = *pRight + *pLeft;
}

// FUNCTION: LEMBALL 0x0042b9e0
ThreeDwordPoint* ThreeDwordPoint::Initialize(const int* pX, const int* pY, const int* pZ)
{
	m_nX = *pX;
	m_nY = *pY;
	m_nZ = *pZ;
	return this;
}
