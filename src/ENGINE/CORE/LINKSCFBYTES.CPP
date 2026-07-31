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
