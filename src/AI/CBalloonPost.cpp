#include "AI/CBalloonPost.h"

// FUNCTION: LEMBALL 0x0042a4e0
void CBalloonPost::LoadLevel(unsigned short* pStream)
{
	unsigned short* pData;
	unsigned int* pDst;
	void** pObj;
	int i;

	m_wActiveMask00 = *pStream;
	pData = pStream + 1;
	pDst = (unsigned int*) ((char*) this + 4);
	pObj = (void**) ((char*) this + 0x34);
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
	if (*(unsigned char*) this & 1) {
		*(int*) ((char*) *(void**) ((char*) this + 0x34) + 0x124) = 1;
	}
	if (*(unsigned char*) this & 2) {
		*(int*) ((char*) *(void**) ((char*) this + 0x38) + 0x124) = 1;
	}
	if (*(unsigned char*) this & 4) {
		*(int*) ((char*) *(void**) ((char*) this + 0x3c) + 0x124) = 1;
	}
	if (*(unsigned char*) this & 8) {
		*(int*) ((char*) *(void**) ((char*) this + 0x40) + 0x124) = 1;
	}
}
