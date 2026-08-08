#include "AI/CBaseRemap.h"

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
