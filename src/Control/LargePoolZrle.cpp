#include <string.h>

#define GET_ZRLE_SOURCE(pStream) \
	((unsigned char* (__fastcall*)(void*)) (*(void***) (pStream))[0x28 / 4])((pStream))

// NOTE: 0x00476BF0 unmatchable: natural gated clipped decoder measured 14.17%.
// NOTE: 0x00476EE0 unmatchable: revised mirrored clipped decoder measured 24.31%.

// FUNCTION: LEMBALL 0x004766F0
void __fastcall decode_zrle_rows_clipped(
	void* pThis, int nUnusedEdx, short* param_1, int param_2, int* param_3, int param_4)
{
	unsigned char* pSrc;
	unsigned char* pDst;
	unsigned char* p;
	unsigned char* d;
	unsigned char b;
	unsigned int n;
	unsigned int nCopy;
	unsigned int nRun;
	int nStep;
	int nRowOffset;
	int nRow;
	int nSkipRows;
	int nLeft;
	int nRemaining;
	int nOverlap;
	short sDestinationX;

	nStep = 1;
	sDestinationX = param_1[2];
	nRowOffset = (int) param_1[3];
	pSrc = GET_ZRLE_SOURCE(param_3);

	if (param_4 == 0) {
		nSkipRows = (int) *(short*) (param_2 + 6);
	}
	else {
		nStep = -1;
		nRowOffset += param_1[1] - 1;
		nSkipRows =
			((int) *(short*) ((char*) param_3 + 0x4e) -
			 (int) *(short*) (param_2 + 6)) -
			(int) param_1[1];
	}

	if (nSkipRows > 0) {
		do {
			b = *pSrc++;
			if (b > 0x80) {
				b &= 0x7f;
				pSrc += b;
			}
			if (b == 0x80) {
				--nSkipRows;
			}
		} while (nSkipRows != 0);
	}

	nRow = 0;
	if (param_1[1] > 0) {
		nRowOffset <<= 2;
		nStep <<= 2;
		do {
			nRemaining = (int) param_1[0];
			nLeft = (int) *(short*) (param_2 + 4);
			pDst = (unsigned char*)
				(*(int*) (*(int*) ((char*) pThis + 4) + nRowOffset) +
				 (int) sDestinationX);

			do {
				b = *pSrc++;
				if (b < 0x80) {
					nLeft -= (unsigned int) b;
					if (nLeft < 0) {
						nRemaining += nLeft;
						pDst -= nLeft;
					}
				}
				else if (b > 0x80) {
					b &= 0x7f;
					nRun = (unsigned int) b;
					nLeft -= nRun;
					if (nLeft < 0) {
						nOverlap = -nLeft;
						nCopy = (unsigned int) nOverlap;
						if (nCopy > (unsigned int) nRemaining) {
							nCopy = (unsigned int) nRemaining;
						}
						p = pSrc + nRun + nLeft;
						d = pDst;
						for (n = nCopy >> 2; n != 0; --n) {
							*(unsigned int*) d = *(unsigned int*) p;
							p += 4;
							d += 4;
						}
						for (n = nCopy & 3; n != 0; --n) {
							*d++ = *p++;
						}
						nRemaining += nLeft;
						pDst += nOverlap;
					}
					pSrc += nRun;
				}
			} while (b != 0x80 && nLeft > 0);

			if (b != 0x80) {
				do {
					if (nRemaining < 1) {
						break;
					}
					b = *pSrc++;
					if (b < 0x80) {
						pDst += b;
						nRemaining -= (unsigned int) b;
					}
					else if (b > 0x80) {
						b &= 0x7f;
						nRun = (unsigned int) b;
						nCopy = nRun;
						if (nCopy > (unsigned int) nRemaining) {
							nCopy = (unsigned int) nRemaining;
						}
						p = pSrc;
						d = pDst;
						for (n = nCopy >> 2; n != 0; --n) {
							*(unsigned int*) d = *(unsigned int*) p;
							p += 4;
							d += 4;
						}
						for (n = nCopy & 3; n != 0; --n) {
							*d++ = *p++;
						}
						pDst += nCopy;
						nRemaining -= nCopy;
						pSrc += nRun;
					}
				} while (b != 0x80);

				while (b != 0x80) {
					b = *pSrc++;
					if (b > 0x80) {
						b &= 0x7f;
						pSrc += b;
					}
				}
			}

			nRowOffset += nStep;
			++nRow;
		} while (nRow < param_1[1]);
	}

	(void) nUnusedEdx;
}

// FUNCTION: LEMBALL 0x00476910
void __fastcall decode_zrle_rows_clipped_write_mask(
	void* pThis, int nUnusedEdx, short* param_1, int param_2, int* param_3,
	unsigned short param_4)
{
	unsigned char* pSrc;
	unsigned char* pDst;
	unsigned char* p;
	unsigned char* d;
	unsigned short* pMask;
	unsigned int* pMaskDword;
	unsigned char b;
	unsigned int n;
	unsigned int nCopy;
	unsigned int nRun;
	unsigned int nMaskPattern;
	int nRowOffset;
	int nRow;
	int nSkipRows;
	int nLeft;
	int nRemaining;
	int nOverlap;
	short sDestinationX;

	pSrc = GET_ZRLE_SOURCE(param_3);
	nSkipRows = (int) *(short*) (param_2 + 6);
	if (nSkipRows > 0) {
		do {
			b = *pSrc++;
			if (b > 0x80) {
				b &= 0x7f;
				pSrc += b;
			}
			if (b == 0x80) {
				--nSkipRows;
			}
		} while (nSkipRows != 0);
	}

	sDestinationX = param_1[2];
	nMaskPattern = (unsigned int) param_4 |
		((unsigned int) param_4 << 16);
	nRow = 0;
	if (param_1[1] > 0) {
		nRowOffset = (int) param_1[3] << 2;
		do {
			nRemaining = (int) param_1[0];
			nLeft = (int) *(short*) (param_2 + 4);
			pMask = (unsigned short*)
				(*(int*) (*(int*) ((char*) pThis + 0x50) + nRowOffset) +
				 (int) sDestinationX * 2);
			pDst = (unsigned char*)
				(*(int*) (*(int*) ((char*) pThis + 4) + nRowOffset) +
				 (int) sDestinationX);

			do {
				b = *pSrc++;
				if (b < 0x80) {
					nLeft -= (unsigned int) b;
					if (nLeft < 0) {
						nRemaining += nLeft;
						pDst -= nLeft;
						pMask -= nLeft;
					}
				}
				else if (b > 0x80) {
					b &= 0x7f;
					nRun = (unsigned int) b;
					nLeft -= nRun;
					if (nLeft < 0) {
						nOverlap = -nLeft;
						nCopy = (unsigned int) nOverlap;
						if (nCopy > (unsigned int) nRemaining) {
							nCopy = (unsigned int) nRemaining;
						}

						p = pSrc + nRun + nLeft;
						d = pDst;
						for (n = nCopy >> 2; n != 0; --n) {
							*(unsigned int*) d = *(unsigned int*) p;
							p += 4;
							d += 4;
						}
						for (n = nCopy & 3; n != 0; --n) {
							*d++ = *p++;
						}

						if (nCopy > 0) {
							pMaskDword = (unsigned int*) pMask;
							for (n = nCopy >> 1; n != 0; --n) {
								*pMaskDword++ = nMaskPattern;
							}
							for (n = nCopy & 1; n != 0; --n) {
								*(unsigned short*) pMaskDword = param_4;
								pMaskDword = (unsigned int*)
									((char*) pMaskDword + 2);
							}
						}

						nRemaining += nLeft;
						pDst += nOverlap;
						pMask += nOverlap;
					}
					pSrc += nRun;
				}
			} while (b != 0x80 && nLeft > 0);

			if (b != 0x80) {
				do {
					if (nRemaining < 1) {
						break;
					}
					b = *pSrc++;
					if (b < 0x80) {
						nRun = (unsigned int) b;
						pDst += nRun;
						pMask += nRun;
						nRemaining -= nRun;
					}
					else if (b > 0x80) {
						b &= 0x7f;
						nRun = (unsigned int) b;
						nCopy = nRun;
						if (nCopy > (unsigned int) nRemaining) {
							nCopy = (unsigned int) nRemaining;
						}

						p = pSrc;
						d = pDst;
						for (n = nCopy >> 2; n != 0; --n) {
							*(unsigned int*) d = *(unsigned int*) p;
							p += 4;
							d += 4;
						}
						for (n = nCopy & 3; n != 0; --n) {
							*d++ = *p++;
						}

						if (nCopy > 0) {
							pMaskDword = (unsigned int*) pMask;
							for (n = nCopy >> 1; n != 0; --n) {
								*pMaskDword++ = nMaskPattern;
							}
							for (n = nCopy & 1; n != 0; --n) {
								*(unsigned short*) pMaskDword = param_4;
								pMaskDword = (unsigned int*)
									((char*) pMaskDword + 2);
							}
						}

						pDst += nCopy;
						pMask += nCopy;
						nRemaining -= nCopy;
						pSrc += nRun;
					}
				} while (b != 0x80);

				while (b != 0x80) {
					b = *pSrc++;
					if (b > 0x80) {
						b &= 0x7f;
						pSrc += b;
					}
				}
			}

			++nRow;
			nRowOffset += 4;
		} while (nRow < param_1[1]);
	}

	(void) nUnusedEdx;
}
