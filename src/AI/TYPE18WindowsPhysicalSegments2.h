#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_059)
// FUNCTION: LEMBALL 0x0040ca10
void __fastcall SetType18ChunkObjectPositionByIndex(void* pObject,
													int nUnused,
													int param_2,
													int param_3,
													int param_4,
													int param_5)
{
	if (param_5 < *(int*) ((char*) pObject + 0x50)) {
		((void(__cdecl*)(void*, int, int, int)) 0x401cdf)(*(void**) ((char*) pObject + 0x30 + param_5 * 4),
														  param_2,
														  param_3,
														  param_4);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_068)
// FUNCTION: LEMBALL 0x0040c950
int __fastcall GetType18ChunkObjectTilePosition(void* pThis, int nUnused, int* pOut, int nIndex)
{
	if (*(int*) ((char*) pThis + 0x50) == 0) {
		return 0;
	}
	int pObj = *(int*) ((char*) pThis + 0x30 + nIndex * 4);
	int x = *(int*) (pObj + 0x9c);
	pOut[0] = x;
	int y = *(int*) (pObj + 0xa0);
	pOut[1] = y;
	pOut[2] = *(int*) (pObj + 0xa4);
	unsigned int z = 0;
	if (!((x >> 12) < 0 || (y >> 12) < 0 || *(int*) ((char*) g_pLevelTileGrid + 0x10) <= (x >> 16) ||
		  *(int*) ((char*) g_pLevelTileGrid + 0x14) <= (y >> 16))) {
		z = ((unsigned int(__fastcall*)(void*, int, int)) 0x4029a5)(
			(void*) ((((y >> 16) * *(int*) ((char*) g_pLevelTileGrid + 0x10) + (x >> 16)) * 0xc) +
					 *(int*) ((char*) g_pLevelTileGrid + 0xc)),
			x >> 12 & 0xf,
			y >> 12 & 0xf);
	}
	pOut[2] = (z & 0xffff) << 12;
	return 1;
}
#endif
