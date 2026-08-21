#include "AI/CBalloonPost.h"

struct CGround {
	short GetZThunk(int nLocalX, int nLocalY);
};

#define UPDATE_BALLOON_POST_HEIGHT(BIT, INDEX) \
	if ((m_wActiveMask00 & (BIT)) != 0) { \
		int nX = m_anPostPos04[INDEX][0] >> 12; \
		int nY = m_anPostPos04[INDEX][1] >> 12; \
		int nTileX = nX >> 4; \
		int nTileY = nY >> 4; \
		unsigned short nHeight = 0; \
		if (nX >= 0 && nY >= 0 && \
			nTileX < *(int*) ((char*) m_pLevelTileGrid48 + 0x10) && \
			nTileY < *(int*) ((char*) m_pLevelTileGrid48 + 0x14)) { \
			CGround* pGround = (CGround*) \
				(*(int*) ((char*) m_pLevelTileGrid48 + 0xc) + \
				 (nTileY * *(int*) ((char*) m_pLevelTileGrid48 + 0x10) + nTileX) * 0xc); \
			nHeight = pGround->GetZThunk(nX & 0xf, nY & 0xf); \
		} \
		m_anPostPos04[INDEX][2] = (unsigned int) nHeight << 12; \
		*(int*) ((char*) m_apBalloons34[INDEX] + 0xa4) = (unsigned int) nHeight << 12; \
	}

// FUNCTION: LEMBALL 0x0042a170
void CBalloonPost::Process(void)
{
	UPDATE_BALLOON_POST_HEIGHT(1, 0);
	UPDATE_BALLOON_POST_HEIGHT(2, 1);
	UPDATE_BALLOON_POST_HEIGHT(4, 2);
	UPDATE_BALLOON_POST_HEIGHT(8, 3);
}

#undef UPDATE_BALLOON_POST_HEIGHT

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


// FUNCTION: LEMBALL 0x0042a0b0
unsigned int CBalloonPost::FindPost(unsigned int subtype, void** pOut)
{
	switch (subtype) {
	case 0x28:
		pOut[0] = *(void**) ((char*) this + 4);
		pOut[1] = *(void**) ((char*) this + 8);
		pOut[2] = *(void**) ((char*) this + 0xc);
		return *(unsigned short*) this & 1;
	case 0x2a:
		pOut[0] = *(void**) ((char*) this + 0x10);
		pOut[1] = *(void**) ((char*) this + 0x14);
		pOut[2] = *(void**) ((char*) this + 0x18);
		return *(unsigned short*) this & 2;
	case 0x2c:
		pOut[0] = *(void**) ((char*) this + 0x1c);
		pOut[1] = *(void**) ((char*) this + 0x20);
		pOut[2] = *(void**) ((char*) this + 0x24);
		return *(unsigned short*) this & 4;
	case 0x2e:
		pOut[0] = *(void**) ((char*) this + 0x28);
		pOut[1] = *(void**) ((char*) this + 0x2c);
		pOut[2] = *(void**) ((char*) this + 0x30);
		return *(unsigned short*) this & 8;
	}
	return 0;
}
