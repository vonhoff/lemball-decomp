#include "Platform/Windows/Mixed/Level/DRAWTEXT.H"

// FUNCTION: LEMBALL 0x00467570
short* CAnimsManager::GetAnimSize(short* pExtent, int nResourceId, int iFrame)
{
	int* pResource;
	short* pFrame;

	pResource = (int*) m_ppResourceObjects24[m_pResourceIds28[nResourceId]];
	if (pResource[0x10] != 0x5a524c45) {
		pFrame = (short*) (unsigned long) (pResource[0x1e] + 0x4c + iFrame * 0x54);
		pExtent[0] = pFrame[0];
		pExtent[1] = pFrame[1];
	}
	else {
		pExtent[0] = *(short*) ((char*) pResource + 0x4c);
		pExtent[1] = *(short*) ((char*) pResource + 0x4e);
	}
	return pExtent;
}
