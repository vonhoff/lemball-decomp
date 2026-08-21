#include "AI/CBaseRemap.h"

#include "Visos/Generic/Memory.h"

// FUNCTION: LEMBALL 0x0046aaf0
void CBaseRemap::MapRemap(const unsigned char* param_1)
{
	int* pi = (int*) m_pRemapObject04;
	if (pi[4] == 0) {
		((void(__fastcall*) (void*)) *(int*) (pi + 0x1c / 4))((char*) pi);
	}
	else {
		pi[9] = 0;
	}
	pi[2] = pi[2] + 1;
	unsigned int nSize = *(unsigned int*) ((char*) m_pRemapObject04 + 0x48);
	void* pBuf = ((void* (__cdecl*) (unsigned int)) 0x45a780)(nSize);
	m_pTable00 = (unsigned char*) pBuf;
	int i = 0;
	unsigned int nPairs = (unsigned int) param_1[0];
	if (0 < (int) nSize) {
		do {
			m_pTable00[i] = (char) i;
			i = i + 1;
		} while (i < (int) nSize);
	}
	i = 0;
	if (nPairs != 0) {
		do {
			if (param_1[nPairs + i + 1] != 0) {
				*(unsigned char*) ((unsigned int) param_1[i + 1] + (unsigned int) m_pTable00) = param_1[nPairs + i + 1];
			}
			i = i + 1;
		} while (i < (int) nPairs);
	}
	*(int*) ((char*) m_pRemapObject04 + 8) = *(int*) ((char*) m_pRemapObject04 + 8) - 1;
}

// FUNCTION: LEMBALL 0x0046ab70
void CBaseRemap::CalculateGreyScale(void)
{
	int* pRemap = (int*) m_pRemapObject04;
	if (pRemap[4] != 0) {
		pRemap[9] = 0;
	}
	else {
		((void(__fastcall*)(void*))(*(void***) pRemap)[7])(pRemap);
	}
	pRemap[2] = pRemap[2] + 1;

	int nEntries = *(int*) ((char*) m_pRemapObject04 + 0x48);
	m_pTable00 = (unsigned char*) AllocateVSMemBlock((unsigned int) nEntries);
	unsigned char* pGreys = (unsigned char*) AllocateVSMemBlock((unsigned int) nEntries);
	int nGreys = 0;
	int i = 1;
	if (i < nEntries - 1) {
		do {
			unsigned char* pColour = *(unsigned char**) ((char*) m_pRemapObject04 + 0x38) + i * 4;
			if (pColour[0] == pColour[1] && pColour[2] == pColour[1]) {
				pGreys[nGreys] = (unsigned char) i;
				nGreys++;
			}
			i++;
		} while (i < nEntries - 1);
	}

	i = 1;
	if (i < nEntries - 1) {
		do {
			unsigned char* pPalette = *(unsigned char**) ((char*) m_pRemapObject04 + 0x38);
			unsigned char* pColour = pPalette + i * 4;
			short nGrey = (short) (((short) ((unsigned short) pColour[0] * 2) +
									(short) ((unsigned short) pColour[1] * 7) + pColour[2]) /
								   10);
			int nClosest = -1;
			int nDistance = 0x7fffffff;
			for (int j = 0; j < nGreys; j++) {
				int nCandidate = pPalette[pGreys[j] * 4] - nGrey;
				int nSign = nCandidate >> 31;
				nCandidate = (nCandidate ^ nSign) - nSign;
				if (nCandidate < nDistance) {
					nDistance = nCandidate;
					nClosest = pGreys[j];
				}
			}
			m_pTable00[i] = (unsigned char) nClosest;
			i++;
		} while (i < nEntries - 1);
	}
	FreeVSMemBlock(pGreys);
	*(int*) ((char*) m_pRemapObject04 + 8) = *(int*) ((char*) m_pRemapObject04 + 8) - 1;
}
