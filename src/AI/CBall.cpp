extern void* g_pActiveManagedEntityOwner;
extern void* g_pLevelTileGrid;

// FUNCTION: LEMBALL 0x00421b40
void __fastcall CBall_SetHeightCorrect(void* pThis)
{
	int xq = *(int*) ((char*) pThis + 0x9c) >> 12;
	int yq = *(int*) ((char*) pThis + 0xa0) >> 12;
	int nTileX = *(int*) ((char*) pThis + 0x9c) >> 16;
	int nTileY = *(int*) ((char*) pThis + 0xa0) >> 16;
	unsigned int z = 0;
	if (!(xq < 0 || yq < 0 ||
	      *(int*) ((char*) g_pLevelTileGrid + 0x10) <= nTileX ||
	      *(int*) ((char*) g_pLevelTileGrid + 0x14) <= nTileY)) {
		z = ((unsigned int(__fastcall*)(void*, int, int)) 0x4029a5)(
			(void*) (((nTileY * *(int*) ((char*) g_pLevelTileGrid + 0x10) + nTileX) * 0xc) + *(int*) ((char*) g_pLevelTileGrid + 0xc)),
			xq & 0xf, yq & 0xf);
	}
	*(unsigned int*) ((char*) pThis + 0xa4) = (z & 0xffff) << 12;
}
// FUNCTION: LEMBALL 0x004216c0
void __fastcall CBall_Set(void* pObject, int nUnused, int param_1, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7)
{
	*(int*) ((char*) pObject + 0x9c) = param_1;
	*(int*) ((char*) pObject + 0xa0) = param_2;
	*(int*) ((char*) pObject + 0xa4) = param_3;
	*(int*) ((char*) pObject + 0x40) = param_1;
	*(int*) ((char*) pObject + 0x44) = param_2;
	*(int*) ((char*) pObject + 0x48) = param_3;
	*(int*) ((char*) pObject + 0x124) = param_4;
	*(int*) ((char*) pObject + 0x128) = param_5;
	*(int*) ((char*) pObject + 0x12c) = param_6;
	*(int*) ((char*) pObject + 0xb8) = 0x25;
	*(short*) ((char*) pObject + 0xbc) = 0;
	if (*(unsigned short*) ((char*) g_pActiveManagedEntityOwner + 0x54) < 7) {
		*(short*) ((char*) pObject + 0x130) = (short) *(int*) (*(int*) ((char*) pObject + 0x64) * 4 + 0x49d070);
	} else {
		*(short*) ((char*) pObject + 0x130) = (short) param_7;
	}
	if (*(unsigned short*) ((char*) pObject + 0x130) > 1) {
		*(int*) ((char*) pObject + 0x134) = 1;
	} else {
		*(short*) ((char*) pObject + 0x130) = (short) *(int*) (*(int*) ((char*) pObject + 0x64) * 4 + 0x49d070);
		*(int*) ((char*) pObject + 0x134) = 1;
	}
	*(int*) ((char*) pObject + 0x134) = 1;
}
