struct ManagedEntityGroupChildTableView {
	unsigned char m_abReserved00[0x36];
	unsigned short m_cChildren36;
	unsigned char m_abReserved38[4];
	void** m_ppChildren3C;

	void* FindChildByTypeInRect(short* pRect, int nType);
};

// FUNCTION: LEMBALL 0x0041BB10
void* ManagedEntityGroupChildTableView::FindChildByTypeInRect(short* pRect, int nType)
{
	int nWidth;
	int nHeight;
	int nRight;
	int nBottom;
	int cChildren;
	int i;

	nWidth = (int) pRect[2];
	nHeight = (int) pRect[3];
	nRight = (int) pRect[0] + nWidth;
	nBottom = (int) pRect[1] + nHeight;
	cChildren = (int) m_cChildren36;
	for (i = 0; i < cChildren; ++i) {
		char* pChild;
		int nX;
		int nY;

		pChild = (char*) m_ppChildren3C[i];
		if (*(int*) (pChild + 0x64) != nType) {
			continue;
		}
		nX = *(int*) (pChild + 0x9c) >> 12;
		nY = *(int*) (pChild + 0xa0) >> 12;
		if (nX - 8 < nRight && nWidth < nX && nY - 8 < nBottom && nHeight < nY) {
			return m_ppChildren3C[i];
		}
	}
	return 0;
}
