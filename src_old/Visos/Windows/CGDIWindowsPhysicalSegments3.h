#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_015)
extern "C"
{
	__declspec(dllimport) int __stdcall BitBlt(void*, int, int, int, int, void*, int, int, unsigned long);
}

extern void __stdcall WriteVgaSequencerMapMask(unsigned char nPlaneMask);
extern void __stdcall copy_low_bytes_from_dword_stride(unsigned char* pDst, const unsigned char* pSrc, int nCount);

// FUNCTION: LEMBALL 0x004570B0
int __fastcall vs_gdi_dib240_display_state_copy_display_rect(void* pState,
															 int,
															 void* pDst,
															 short* pRect,
															 void* pSrc,
															 short* pPoint)
{
	short nWidth = pRect[0];
	short nHeight = pRect[1];
	short nDstX = pRect[2];
	short nDstY = pRect[3];
	short nSrcX = pPoint[0];
	short nSrcY = pPoint[1];
	int nPitch;
	int nSourceOffset;
	int nDest;
	int nLast;
	int y;
	int plane;
	int pRow;
	int nPlaneDst;
	int nPlaneSrc;
	int nDwords;
	if (*(int*) 0x4a0770 != 0) {
		return BitBlt(*(void**) ((char*) pDst + 4),
					  nDstX,
					  nDstY,
					  nWidth,
					  nHeight,
					  *(void**) ((char*) pSrc + 4),
					  nSrcX,
					  nSrcY,
					  0xcc0020);
	}
	if (nDstX < 0) {
		nWidth += nDstX;
		nDstX = 0;
	}
	if (*(short*) ((char*) pState + 0x14) < (short) (nWidth + nDstX)) {
		nWidth = *(short*) ((char*) pState + 0x14) - nDstX;
	}
	if (nDstY < 0) {
		nHeight += nDstY;
		nDstY = 0;
	}
	if (*(short*) ((char*) pState + 0x16) < (short) (nHeight + nDstY)) {
		nHeight = *(short*) ((char*) pState + 0x16) - nDstY;
	}
	if (nWidth < 1 || nHeight < 1) {
		nWidth = nHeight = 0;
	}
	if (nWidth * nHeight == 0) {
		return 1;
	}
	{
		void* pLock = *(void**) ((char*) pSrc + 8);
		((void(__fastcall*)(void*, int))(*(void***) pLock)[4])(pLock, 0);
	}
	nLast = nSrcX + nWidth - 1;
	nPitch = ((int) *(short*) ((char*) pState + 0x14) + (((int) *(short*) ((char*) pState + 0x14) >> 31) & 3)) >> 2;
	nSourceOffset = nSrcY << 2;
	nDest = nDstY * nPitch + *(int*) ((char*) pState + 0x1c);
	for (y = 0; y < nHeight; ++y) {
		pRow = *(int*) (*(int*) (*(int*) ((char*) pState + 0x18) + 4) + nSourceOffset);
		for (plane = 0; plane < 4; ++plane) {
			nPlaneDst = nDstX + plane;
			nPlaneSrc = nSrcX + plane;
			if (nPlaneSrc <= nLast) {
				nDwords = ((nLast - nPlaneSrc + (((nLast - nPlaneSrc) >> 31) & 3)) >> 2) + 1;
				WriteVgaSequencerMapMask((unsigned char) (1 << (nPlaneDst & 3)));
				copy_low_bytes_from_dword_stride(
					(unsigned char*) (((nPlaneDst + ((nPlaneDst >> 31) & 3)) >> 2) + nDest),
					(unsigned char*) (nPlaneSrc + pRow),
					nDwords);
			}
		}
		nSourceOffset += 4;
		nDest += nPitch;
	}
	WriteVgaSequencerMapMask(0xf);
	return 1;
}
#endif
