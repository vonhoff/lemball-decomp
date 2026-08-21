#include "AI/CBallManager.h"

#include "Visos/Generic/Memory.h"

// FUNCTION: LEMBALL 0x00421ef0
void CBallManager::Initialise(int nCapacity)
{
	void* pBalls;
	int i;
	void* pBall;

	m_nCapacity0C = nCapacity;
	m_nActiveCount08 = 0;
	if (nCapacity == 0) {
		m_apBalls04 = (void**) 0x0;
		return;
	}
	pBalls = m_apBalls04;
	if (pBalls == (void*) 0x0) {
		pBalls = AllocateVSMemBlock((unsigned int) nCapacity * 4u);
		m_apBalls04 = (void**) pBalls;
	}
	if (m_nCapacity0C > 0) {
		for (i = 0; i < m_nCapacity0C; i++) {
			pBall = (void*) ((void*(__cdecl*) (int) ) 0x45a780)(0x13c);
			if (pBall != (void*) 0x0) {
				pBall = ((void*(__fastcall*) (void*) ) 0x4015c8)(pBall);
				*((void**) pBalls + i) = pBall;
			}
			else {
				*((void**) pBalls + i) = (void*) 0x0;
			}
			pBall = *((void**) pBalls + i);
			((void(__fastcall*)(void*))(*(void***) pBall + 0x104 / 4))(pBall);
		}
	}
}

// FUNCTION: LEMBALL 0x00421ff0
void CBallManager::Delete(CBall* pBall)
{
	int n = m_nActiveCount08;
	int i = 0;
	if (n > 0) {
		int* pIdx = (int*) m_apBalls04;
		while (*pIdx != (int) pBall) {
			pIdx++;
			i++;
			if (n <= i) {
				return;
			}
		}
		m_nActiveCount08 = n - 1;
		if (i < n - 1) {
			int j;
			for (j = i; j < m_nActiveCount08; j++) {
				*((int*) m_apBalls04 + j) = *((int*) m_apBalls04 + j + 1);
			}
		}
		*((int*) m_apBalls04 + m_nActiveCount08) = (int) pBall;
	}
}

// FUNCTION: LEMBALL 0x00422060
void CBallManager::LoadLevel(unsigned short* pStream)
{
	unsigned short nStreamCount = *pStream;
	pStream = pStream + 1;
	unsigned int nBallCount = (unsigned int) nStreamCount;
	((void(__fastcall*)(void*, unsigned int)) 0x4032f6)(this, nBallCount);
	m_nActiveCount08 = nBallCount;
	if (nBallCount != 0) {
		unsigned int nBallOffset = 0;
		int iBall = 0;
		do {
			nBallOffset = nBallOffset + 4;
			iBall++;
			((void(__fastcall*)(void*, unsigned short**)) 0x403030)(*((void**) m_apBalls04 + iBall - 1), &pStream);
		} while (iBall < m_nActiveCount08);
	}
}
