// FUNCTION: LEMBALL 0x0042a4e0
void __fastcall CBalloonPost_LoadLevel(void* pThis, int nUnused, unsigned short* pStream, int param_4, int param_5)
{
	unsigned short* pData;
	unsigned int* pDst;
	void** pObj;
	int i;

	*(unsigned short*) pThis = *pStream;
	pData = pStream + 1;
	pDst = (unsigned int*) ((char*) pThis + 4);
	pObj = (void**) ((char*) pThis + 0x34);
	for (i = 0; i < 4; i++) {
		unsigned int x = (unsigned int) pData[0] << 0xc;
		unsigned int y = (unsigned int) pData[1] << 0xc;
		unsigned int z = (unsigned int) pData[2] << 0xc;
		pData += 3;
		pDst[0] = x;
		pDst[1] = y;
		pDst[2] = z;
		*(unsigned int*) ((char*) *pObj + 0x9c) = x;
		*(unsigned int*) ((char*) *pObj + 0xa0) = y;
		*(unsigned int*) ((char*) *pObj + 0xa4) = z;
		*(int*) ((char*) *pObj + 0x124) = 0;
		pDst += 3;
		pObj++;
	}
	if (*(unsigned char*) pThis & 1) *(int*) ((char*) *(void**) ((char*) pThis + 0x34) + 0x124) = 1;
	if (*(unsigned char*) pThis & 2) *(int*) ((char*) *(void**) ((char*) pThis + 0x38) + 0x124) = 1;
	if (*(unsigned char*) pThis & 4) *(int*) ((char*) *(void**) ((char*) pThis + 0x3c) + 0x124) = 1;
	if (*(unsigned char*) pThis & 8) *(int*) ((char*) *(void**) ((char*) pThis + 0x40) + 0x124) = 1;
}
