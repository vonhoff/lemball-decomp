extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;

// FUNCTION: LEMBALL 0x00426840
void __fastcall CRocket_Set(void* pObject, int nUnused, unsigned short slotId, int* pVec)
{
	((void(__fastcall*) (void*, unsigned short)) 0x402293)(pObject, slotId);
	*(int*) ((char*) pObject + 0x9c) = pVec[0];
	*(int*) ((char*) pObject + 0xa0) = pVec[1];
	*(int*) ((char*) pObject + 0x13c) = 1;
	*(int*) ((char*) pObject + 0xb8) = 0x18;
	*(int*) ((char*) pObject + 0xa4) = pVec[2];
	int xtile = (pVec[0] >> 12) / 16;
	if (xtile < 0) return;
	int ytile = (pVec[1] >> 12) / 16;
	if (ytile < 0) return;
	void* pGrid = *(void**) 0x4a74b4;
	if (*(int*) ((char*) pGrid + 0x10) <= xtile) return;
	if (*(int*) ((char*) pGrid + 0x14) <= ytile) return;
	int nIndex = *(int*) ((char*) pGrid + 0x10) * ytile + xtile;
	*(char*) (*(int*) ((char*) pGrid + 0xc) + nIndex * 12 + 7) |= (char) 0x80;
}
// FUNCTION: LEMBALL 0x004269d0
int __fastcall CRocket_StepOn(void* pObject, int nUnused, int* pOther, void* param_2)
{
	int nDist = ((int(__cdecl*) (int, int, int, int)) 0x40254a)(*(int*) ((char*) pObject + 0x9c) >> 12, *(int*) ((char*) pObject + 0xa0) >> 12, pOther[0] >> 12, pOther[1] >> 12);
	if (nDist < 0x20) {
		*(int*) ((char*) pObject + 0x9c) = pOther[0] + 0x4000;
		*(int*) ((char*) pObject + 0xa0) = pOther[1] + 0x4000;
		*(int*) ((char*) pObject + 0xa4) = pOther[2];
		*(int*) ((char*) pObject + 0x5c) = (int) param_2;
		*(int*) ((char*) pObject + 0x140) = pOther[2] >> 12;
		*(int*) ((char*) pObject + 0xc8) = 0x30;
		((void(__fastcall*) (void*, int)) 0x401f3c)(pObject, 0x1b);
		return 1;
	}
	return 0;
}
// FUNCTION: LEMBALL 0x00426a60
void __fastcall CRocket_DoActivate(void* pObject, int nUnused)
{
	*(int*) ((char*) pObject + 0xc8) += g_nLevelFrameClockTick;
	*(int*) ((char*) pObject + 0x94) = g_nLevelFrameClockTimeMs;
	void* pTarget = *(void**) ((char*) pObject + 0x5c);
	((void(__fastcall*) (void*, int)) (*(void***) pTarget + 0x8 / 4))(pTarget, 0x15);
	*(int*) ((char*) pTarget + 0xcc) = g_nLevelFrameClockTick + 0x3c;
	((void(__fastcall*) (void*, int)) (*(void***) pObject + 0x34 / 4))(pObject, 0x12);
	if (*(int*) 0x4a011c != 0) {
		((void(__fastcall*) (void*, void*)) 0x4032dd)(*(void**) 0x49d128, pObject);
	}
}
