#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_006)
// FUNCTION: LEMBALL 0x00475f60
int __fastcall PointInHelperClipRect(void* pObject, int nUnused, int param_1, int param_2)
{
	int iVar1 = *(int*) (*(int*) ((char*) pObject + 0x40) + 4);
	int iVar2 = (int) *(short*) ((char*) pObject + iVar1 + 0x68);
	if (iVar2 <= param_1 && param_1 <= *(short*) ((char*) pObject + iVar1 + 100) + iVar2 - 1) {
		iVar2 = (int) *(short*) ((char*) pObject + iVar1 + 0x6a);
		if (iVar2 <= param_2 && param_2 <= *(short*) ((char*) pObject + iVar1 + 0x66) + iVar2 - 1) {
			return 1;
		}
	}
	return 0;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_008)
// FUNCTION: LEMBALL 0x0046bfa0
int __fastcall FindResourceGeometryHelperSlotByResourceOwner(void* pObject, int nUnused, int param_2)
{
	if (*(int*) ((char*) pObject + 0x10) > 0) {
		int i;
		for (i = 0; i < *(int*) ((char*) pObject + 0x10); i++) {
			if (*(int*) (*(int*) ((char*) pObject + 4) + 4 + i * 28) == param_2) {
				return i;
			}
		}
	}
	return -1;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_011)
// FUNCTION: LEMBALL 0x00466fd0
int __fastcall GetHelperUploadRectEntryAndState(void* pObject, int nUnused, int param_2, int* param_3)
{
	if (*(int*) ((char*) pObject + 4) == -1) {
		((void(__fastcall*)(void*)) 0x466ef0)(pObject);
	}
	*param_3 = *(int*) (*(int*) ((char*) pObject + 0xc) + 8 + param_2 * 0xc);
	return param_2 * 0xc + *(int*) ((char*) pObject + 0xc);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_088)
// FUNCTION: LEMBALL 0x00457080
void __stdcall copy_low_bytes_from_dword_stride(unsigned char* pDst, const unsigned char* pSrc, int nCount)
{
	const unsigned char* pSource;
	int nRemaining;
	unsigned char* pDestination;
	nRemaining = nCount;
	if (nRemaining > 0) {
		pSource = pSrc;
		pDestination = pDst;
		do {
			*pDestination++ = *pSource;
			pSource += 4;
		} while (--nRemaining != 0);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_094)
// FUNCTION: LEMBALL 0x0046d560
void __fastcall apply_point_delta_to_child_helper(void* pThis, int nUnused, short* pDelta)
{
	int iBase = *(int*) (*(int*) ((char*) pThis + 0x40) + 4);
	short* pPt = (short*) (iBase + 0x50 + (int) pThis);
	char* pTmp = (char*) (iBase + 0x4c + (int) pThis);
	*pPt += pDelta[0];
	*(short*) (pTmp + 6) += pDelta[1];
	void* pOwner = (char*) pThis + *(int*) (*(int*) ((char*) pThis + 0x40) + 4) + 0x40;
	void* pArg = (iBase + 0x4c + (int) pThis == 1) ? 0 : pPt;
	(*(void (**)(void*))(*(void***) *(void**) pOwner + 0x2c / 4))(pArg);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_100)
// FUNCTION: LEMBALL 0x00457d00
int __fastcall set_directdraw_palette_entries(void* pObject, int param_2)
{
	unsigned int uResult;
	void* pStream;
	char* pError;
	uResult = ((unsigned int(__stdcall*)(void*, int, int, unsigned short))(
		*(void***) pObject)[0x18 / 4])(pObject, 0, 0, *(unsigned short*) ((char*) param_2 + 2));
	if (uResult != 0) {
		pError = ((char*(__cdecl*) (unsigned int) ) 0x456720)(uResult & 0xfff);
		pStream = *(void**) 0x4a93a8;
		pStream = ((void*(__fastcall*) (void*, int, const char*) ) 0x4585b0)(pStream,
																			 0,
																			 "Direct Draw Set Palette Entries failed ");
		pStream = ((void*(__fastcall*) (void*, int, const char*) ) 0x4585b0)(pStream, 0, pError);
		((void*(__fastcall*) (void*, int, const char*) ) 0x4585b0)(pStream, 0, "\n");
		return 0;
	}
	return 1;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_124)
// FUNCTION: LEMBALL 0x00475bc0
int __fastcall classify_circle_against_helper_clip_rect(void* pThis, int nEdxSlop, int nCX, int nCY, int nRad)
{
	int iRect = *(int*) (*(int*) ((char*) pThis + 0x40) + 4);
	char* pRect = (char*) pThis + iRect;
	short w = *(short*) (pRect + 0x64);
	short h = *(short*) (pRect + 0x66);
	int nLeft = *(short*) (pRect + 0x68);
	int nTop = *(short*) (pRect + 0x6a);
	if (h < 1 || w < 1) {
		return 1;
	}
	int nRight = nLeft + w - 1;
	int nBottom = nTop + h - 1;
	int nMinX = nCX - nRad;
	int nMaxX = nCX + nRad;
	int nMinY = nCY - nRad;
	int nMaxY = nCY + nRad;
	if (nMaxX < nLeft || nRight < nMinX || nMaxY < nTop || nBottom < nMinY) {
		return 1;
	}
	if (nLeft <= nMinX && nMaxX <= nRight && nTop <= nMinY && nMaxY <= nBottom) {
		return 2;
	}
	return 3;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_153)
#pragma auto_inline(on)

// FUNCTION: LEMBALL 0x00477440
void __fastcall decode_zrle_rows_gated_by_mask(void* pThis,
											   int nUnused,
											   int param_1,
											   int* param_2,
											   unsigned short param_3)
{
	short sRowBits = *(short*) (param_1 + 4);
	short sRowOff = *(short*) (param_1 + 6);
	unsigned char* pSrc = (unsigned char*) (*(unsigned char* (**) (void*) )((void***) param_2 + 0x28 / 4))(param_2);
	int rowIndex = 0;
	if (0 < *(short*) (param_1 + 2)) {
		int rowOffset = (int) sRowOff << 2;
		do {
			unsigned char* pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + (int) sRowBits);
			unsigned short* pMask =
				(unsigned short*) (*(int*) (*(int*) ((char*) pThis + 0x50) + rowOffset) + (int) sRowBits * 2);
			unsigned char b;
			do {
				b = *pSrc++;
				if (b < 0x80) {
					pDst = pDst + (unsigned char) b;
					pMask = pMask + (unsigned char) b;
				}
				else if (b > 0x80) {
					b = b & 0x7f;
					unsigned char* p = pSrc;
					unsigned char* d = pDst;
					unsigned short* m = pMask;
					for (unsigned char n = b; n != 0; n = n - 1) {
						if (*m <= param_3) {
							*d = *p;
						}
						d = d + 1;
						m = m + 1;
						p = p + 1;
					}
					pSrc = pSrc + (unsigned char) b;
					pDst = pDst + (unsigned char) b;
					pMask = pMask + (unsigned char) b;
				}
			} while (b != 0x80);
			rowIndex = rowIndex + 1;
			rowOffset = rowOffset + 4;
		} while (rowIndex < *(short*) (param_1 + 2));
	}
	(void) nUnused;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_156)
// FUNCTION: LEMBALL 0x00477130
void __fastcall decode_zrle_rows(void* pThis, int nUnused, int param_1, int* param_2, int param_3)
{
	short sRowBits = *(short*) (param_1 + 4);
	short sRowOff = *(short*) (param_1 + 6);
	int step = 1;
	int rowOffset = (int) sRowOff;
	int rowIndex = 0;
	unsigned char* pSrc = (unsigned char*) (*(unsigned char* (**) (void*) )((void***) param_2 + 0x28 / 4))(param_2);
	if (param_3 != 0) {
		step = -1;
		rowOffset = rowOffset + *(short*) (param_1 + 2) - 1;
	}
	if (0 < *(short*) (param_1 + 2)) {
		rowOffset = rowOffset << 2;
		do {
			unsigned char* pDst;
			unsigned char b;
			pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + (int) sRowBits);
			do {
				b = *pSrc++;
				if (b < 0x80) {
					pDst = pDst + (unsigned char) b;
				}
				else if (b > 0x80) {
					unsigned char* p;
					unsigned char* d;
					unsigned int nDwords;
					b = b & 0x7f;
					p = pSrc;
					d = pDst;
					for (nDwords = (unsigned int) b >> 2; nDwords != 0; nDwords = nDwords - 1) {
						*(unsigned int*) d = *(unsigned int*) p;
						p = p + 4;
						d = d + 4;
					}
					for (nDwords = (unsigned int) b & 3; nDwords != 0; nDwords = nDwords - 1) {
						*d = *p;
						p = p + 1;
						d = d + 1;
					}
					pSrc = pSrc + (unsigned char) b;
					pDst = pDst + (unsigned char) b;
				}
			} while (b != 0x80);
			rowIndex = rowIndex + 1;
			rowOffset = rowOffset + step * 4;
		} while (rowIndex < *(short*) (param_1 + 2));
	}
	(void) nUnused;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_157)
// FUNCTION: LEMBALL 0x00477660
void __fastcall decode_zrle_rows_mirrored(void* pThis, int nUnused, int param_1, int* param_2, int param_3)
{
	int baseOffset;
	int step;
	int rowOffset;
	int rowIndex;
	unsigned char* pSrc;

	baseOffset = (int) *(short*) (param_1 + 4) + (int) *(short*) param_1 - 1;
	step = 1;
	rowOffset = (int) *(short*) (param_1 + 6);
	if (param_3 != 0) {
		step = -1;
		rowOffset += *(short*) (param_1 + 2) - 1;
	}
	pSrc = (unsigned char*) (*(unsigned char* (**) (void*) )((void***) param_2 + 0x28 / 4))(param_2);
	rowIndex = 0;
	if (0 < *(short*) (param_1 + 2)) {
		rowOffset <<= 2;
		step <<= 2;
		do {
			unsigned char* pDst;
			unsigned char b;
			unsigned char* p;
			unsigned char* d;
			int n;

			pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + baseOffset);
			do {
				b = *pSrc++;
				if (b < 0x80) {
					pDst -= (unsigned char) b;
				}
				else if (b > 0x80) {
					b &= 0x7f;
					p = pSrc;
					d = pDst;
					for (n = (int) b; n > 0; --n) {
						*d-- = *p++;
					}
					pSrc += (unsigned char) b;
					pDst -= (unsigned char) b;
				}
			} while (b != 0x80);
			rowOffset += step;
			++rowIndex;
		} while (rowIndex < *(short*) (param_1 + 2));
	}
	(void) nUnused;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_158)
// FUNCTION: LEMBALL 0x004781e0
void __fastcall decode_zrle_rows_palette(void* pThis,
										 int nUnused,
										 int param_1,
										 int* param_2,
										 int param_3,
										 unsigned char* pPalette)
{
	short sRowBits;
	int step;
	int rowOffset;
	int rowIndex;
	unsigned char* pSrc;

	sRowBits = *(short*) (param_1 + 4);
	rowOffset = (int) *(short*) (param_1 + 6);
	step = 1;
	if (param_3 != 0) {
		step = -1;
		rowOffset += *(short*) (param_1 + 2) - 1;
	}
	pSrc = (unsigned char*) (*(unsigned char* (**) (void*) )((void***) param_2 + 0x28 / 4))(param_2);
	rowIndex = 0;
	if (0 < *(short*) (param_1 + 2)) {
		rowOffset <<= 2;
		do {
			unsigned char* pDst;
			unsigned char b;
			unsigned char* p;
			unsigned char* d;
			unsigned int n;

			pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + (int) sRowBits);
			do {
				b = *pSrc++;
				if (b < 0x80) {
					pDst += (unsigned char) b;
				}
				else if (b > 0x80) {
					b &= 0x7f;
					p = pSrc;
					d = pDst;
					for (n = (unsigned int) b; n != 0; --n) {
						*d++ = pPalette[*p++];
					}
					pSrc += (unsigned char) b;
					pDst += (unsigned char) b;
				}
			} while (b != 0x80);
			rowOffset += step * 4;
			++rowIndex;
		} while (rowIndex < *(short*) (param_1 + 2));
	}
	(void) nUnused;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_159)
// FUNCTION: LEMBALL 0x004782d0
void __fastcall decode_zrle_rows_palette_mirrored(void* pThis,
												  int nUnused,
												  int param_1,
												  int* param_2,
												  int param_3,
												  unsigned char* pPalette)
{
	short sRowBits;
	short sBaseX;
	int step;
	int rowOffset;
	int rowIndex;
	unsigned char* pSrc;

	sRowBits = *(short*) (param_1 + 4);
	sBaseX = *(short*) param_1;
	rowOffset = (int) *(short*) (param_1 + 6);
	step = 1;
	if (param_3 != 0) {
		step = -1;
		rowOffset += *(short*) (param_1 + 2) - 1;
	}
	pSrc = (unsigned char*) (*(unsigned char* (**) (void*) )((void***) param_2 + 0x28 / 4))(param_2);
	rowIndex = 0;
	if (0 < *(short*) (param_1 + 2)) {
		rowOffset <<= 2;
		do {
			unsigned char* pDst;
			unsigned char b;
			unsigned char* p;
			unsigned char* d;
			unsigned int n;

			pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + (int) sBaseX + (int) sRowBits -
									 1);
			do {
				b = *pSrc++;
				if (b < 0x80) {
					pDst -= (unsigned char) b;
				}
				else if (b > 0x80) {
					b &= 0x7f;
					p = pSrc;
					d = pDst;
					for (n = (unsigned int) b; n != 0; --n) {
						*d-- = pPalette[*p++];
					}
					pSrc += (unsigned char) b;
					pDst -= (unsigned char) b;
				}
			} while (b != 0x80);
			rowOffset += step * 4;
			++rowIndex;
		} while (rowIndex < *(short*) (param_1 + 2));
	}
	(void) nUnused;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_160)
// FUNCTION: LEMBALL 0x00477200
void __fastcall decode_zrle_rows_write_mask(void* pThis,
											int nUnused,
											int param_1,
											int* param_2,
											unsigned short nMaskValue)
{
	short sRowBits;
	short sRowOff;
	unsigned char* pSrc;
	int rowIndex;
	int rowOffset;
	unsigned int nMaskPattern;

	sRowBits = *(short*) (param_1 + 4);
	sRowOff = *(short*) (param_1 + 6);
	pSrc = (unsigned char*) (*(unsigned char* (**) (void*) )((void***) param_2 + 0x28 / 4))(param_2);
	rowIndex = 0;
	nMaskPattern = (unsigned int) nMaskValue | ((unsigned int) nMaskValue << 16);
	if (0 < *(short*) (param_1 + 2)) {
		rowOffset = (int) sRowOff << 2;
		do {
			unsigned char* pDst;
			unsigned short* pMask;
			unsigned char b;
			unsigned char* p;
			unsigned char* d;
			unsigned int* m;
			unsigned int n;
			unsigned int nCount;

			pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + (int) sRowBits);
			pMask = (unsigned short*) (*(int*) (*(int*) ((char*) pThis + 0x50) + rowOffset) + (int) sRowBits * 2);
			do {
				b = *pSrc++;
				if (b < 0x80) {
					pDst += (unsigned char) b;
					pMask += (unsigned char) b;
				}
				else if (b > 0x80) {
					b &= 0x7f;
					nCount = (unsigned int) b;
					p = pSrc;
					d = pDst;
					for (n = nCount >> 2; n != 0; --n) {
						*(unsigned int*) d = *(unsigned int*) p;
						p += 4;
						d += 4;
					}
					for (n = nCount & 3; n != 0; --n) {
						*d++ = *p++;
					}
					if (b != 0) {
						m = (unsigned int*) pMask;
						for (n = nCount >> 1; n != 0; --n) {
							*m++ = nMaskPattern;
						}
						for (n = nCount & 1; n != 0; --n) {
							*(unsigned short*) m = nMaskValue;
							m = (unsigned int*) ((char*) m + 2);
						}
					}
					pDst += nCount;
					pMask += nCount;
					pSrc += nCount;
				}
			} while (b != 0x80);
			++rowIndex;
			rowOffset += 4;
		} while (rowIndex < *(short*) (param_1 + 2));
	}
	(void) nUnused;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_161)
// FUNCTION: LEMBALL 0x00477540
void __fastcall decode_zrle_rows_palette_gated_by_mask(void* pThis,
													   int nUnused,
													   int param_1,
													   int* param_2,
													   unsigned short nMaskLimit,
													   unsigned char* pPalette)
{
	short sRowBits;
	short sRowOff;
	unsigned char* pSrc;
	int rowIndex;
	int rowOffset;

	sRowBits = *(short*) (param_1 + 4);
	sRowOff = *(short*) (param_1 + 6);
	pSrc = (unsigned char*) (*(unsigned char* (**) (void*) )((void***) param_2 + 0x28 / 4))(param_2);
	rowIndex = 0;
	if (0 < *(short*) (param_1 + 2)) {
		rowOffset = (int) sRowOff << 2;
		do {
			unsigned char* pDst;
			unsigned short* pMask;
			unsigned char b;
			unsigned char* p;
			unsigned char* d;
			unsigned short* m;
			unsigned int n;
			unsigned int nCount;

			pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + (int) sRowBits);
			pMask = (unsigned short*) (*(int*) (*(int*) ((char*) pThis + 0x50) + rowOffset) + (int) sRowBits * 2);
			do {
				b = *pSrc++;
				if (b < 0x80) {
					nCount = (unsigned int) b;
					pDst += nCount;
					pMask += nCount;
				}
				else if (b > 0x80) {
					b &= 0x7f;
					nCount = (unsigned int) b;
					p = pSrc;
					d = pDst;
					m = pMask;
					for (n = nCount; n != 0; --n) {
						if (*m <= nMaskLimit) {
							*d = pPalette[*p];
						}
						++m;
						++d;
						++p;
					}
					pSrc += nCount;
					pDst += nCount;
					pMask += nCount;
				}
			} while (b != 0x80);
			++rowIndex;
			rowOffset += 4;
		} while (rowIndex < *(short*) (param_1 + 2));
	}
	(void) nUnused;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_162)
// FUNCTION: LEMBALL 0x00477310
void __fastcall decode_zrle_rows_palette_write_mask(void* pThis,
													int nUnused,
													int param_1,
													int* param_2,
													unsigned short nMaskValue,
													unsigned char* pPalette)
{
	short sRowBits;
	short sRowOff;
	unsigned char* pSrc;
	int rowIndex;
	int rowOffset;
	unsigned int nMaskPattern;

	sRowBits = *(short*) (param_1 + 4);
	sRowOff = *(short*) (param_1 + 6);
	pSrc = (unsigned char*) (*(unsigned char* (**) (void*) )((void***) param_2 + 0x28 / 4))(param_2);
	rowIndex = 0;
	nMaskPattern = (unsigned int) nMaskValue | ((unsigned int) nMaskValue << 16);
	if (0 < *(short*) (param_1 + 2)) {
		rowOffset = (int) sRowOff << 2;
		do {
			unsigned char* pDst;
			unsigned short* pMask;
			unsigned char b;
			unsigned char* p;
			unsigned char* d;
			unsigned int* m;
			unsigned int n;
			unsigned int nCount;

			pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + (int) sRowBits);
			pMask = (unsigned short*) (*(int*) (*(int*) ((char*) pThis + 0x50) + rowOffset) + (int) sRowBits * 2);
			do {
				b = *pSrc++;
				if (b < 0x80) {
					pDst += (unsigned char) b;
					pMask += (unsigned char) b;
				}
				else if (b > 0x80) {
					b &= 0x7f;
					nCount = (unsigned int) b;
					p = pSrc;
					d = pDst;
					for (n = nCount; n != 0; --n) {
						*d++ = pPalette[*p++];
					}
					if (b != 0) {
						m = (unsigned int*) pMask;
						for (n = nCount >> 1; n != 0; --n) {
							*m++ = nMaskPattern;
						}
						for (n = nCount & 1; n != 0; --n) {
							*(unsigned short*) m = nMaskValue;
							m = (unsigned int*) ((char*) m + 2);
						}
					}
					pDst += nCount;
					pMask += nCount;
					pSrc += nCount;
				}
			} while (b != 0x80);
			++rowIndex;
			rowOffset += 4;
		} while (rowIndex < *(short*) (param_1 + 2));
	}
	(void) nUnused;
}
#endif
