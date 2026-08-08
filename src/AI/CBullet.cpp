extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;

// FUNCTION: LEMBALL 0x0041aca0
int __fastcall CBullet_Receive(void* pObject, int nUnused, short param_1, void* param_2)
{
	if (param_1 != 0x2b) {
		return ((int(__fastcall*)(void*, short, void*)) 0x402342)(pObject, param_1, param_2);
	}
	int iVar2 = ((int(__fastcall*)(void*, void*)) 0x45f280)((void*) ((char*) pObject + 0x138), *(void**) ((char*) param_2 + 0x20));
	if (iVar2 != 0) {
		*(void**) ((char*) param_2 + 0x20) = *(void**) ((char*) pObject + 0x158);
	}
	((void(__fastcall*)(void*, void*)) 0x401299)(*(void**) ((char*) pObject + 0x60), pObject);
	((void(__fastcall*)(void*)) 0x40303a)(pObject);
	return 1;
}
// FUNCTION: LEMBALL 0x0041a760
void __fastcall CBullet_FireBullet(void* param_1)
{
	*(int*) ((char*) param_1 + 0xc8) = g_nLevelFrameClockTick;
	((void(__fastcall*)(void*)) 0x40303a)(param_1);
	if (*(int*) 0x4a011c != 0) {
		((void(__fastcall*)(void*, void*)) 0x40126c)(*(void**) ((char*) param_1 + 0x60), param_1 != 0 ? (char*) param_1 + 0x138 : 0);
	}
}
// FUNCTION: LEMBALL 0x0041a6d0
void __fastcall CBullet_TriggerBullet(void* pThis, int nUnused)
{
	int aPos[3] = {
		*(int*) ((char*) pThis + 0x9c) >> 12,
		*(int*) ((char*) pThis + 0xa0) >> 12,
		*(int*) ((char*) pThis + 0xa4) >> 12
	};
	int aDest[3] = {
		*(int*) ((char*) pThis + 0xa8) >> 12,
		*(int*) ((char*) pThis + 0xac) >> 12,
		*(int*) ((char*) pThis + 0xb0) >> 12
	};
	((void(__fastcall*) (void*, int*, int*, int, int)) 0x4027de)((char*) pThis + 0x184, aPos, aDest, *(int*) ((char*) pThis + 0xc8), 0xc);
	*(int*) ((char*) pThis + 0xb8) = 0x1b;
	*(int*) ((char*) pThis + 0xcc) = *(int*) ((char*) pThis + 0xc8) + 10;
}
// FUNCTION: LEMBALL 0x0041aaa0
void __fastcall CBullet_AddData(void* pThis, int nUnused)
{
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, 0x2b);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(unsigned short*) ((char*) pThis - 0xcc));
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(pThis, nUnused, g_nLevelFrameClockTimeMs);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(int*) ((char*) pThis - 0x9c) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(int*) ((char*) pThis - 0x98) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(int*) ((char*) pThis - 0x94) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(int*) ((char*) pThis - 0x90) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(int*) ((char*) pThis - 0x8c) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(int*) ((char*) pThis - 0x88) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(unsigned short*) ((char*) pThis - 0x84));
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(pThis, nUnused, *(unsigned int*) ((char*) pThis - 0xa0));
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(pThis, nUnused, *(unsigned int*) ((char*) pThis - 0x70));
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(pThis, nUnused, *(unsigned int*) ((char*) pThis + 0x30));
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(pThis, nUnused, *(unsigned int*) ((char*) pThis + 0x34));
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(unsigned short*) ((char*) pThis + 0x38));
}
// FUNCTION: LEMBALL 0x0041ab80
void __fastcall CBullet_GetData(void* pThis, int nUnused)
{
	((void(__cdecl*) (unsigned int)) 0x403107)(((unsigned int(__fastcall*) (void*)) 0x45eff0)(pThis));
	*(unsigned int*) ((char*) pThis - 0x9c) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis)) << 12;
	*(unsigned int*) ((char*) pThis - 0x98) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis)) << 12;
	*(unsigned int*) ((char*) pThis - 0x94) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis)) << 12;
	*(unsigned int*) ((char*) pThis - 0x90) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis)) << 12;
	*(unsigned int*) ((char*) pThis - 0x8c) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis)) << 12;
	*(unsigned int*) ((char*) pThis - 0x88) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis)) << 12;
	*(unsigned short*) ((char*) pThis - 0x84) = ((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis);
	*(unsigned int*) ((char*) pThis - 0xa0) = ((unsigned int(__fastcall*) (void*)) 0x45eff0)(pThis);
	*(unsigned int*) ((char*) pThis - 0x70) = ((unsigned int(__fastcall*) (void*)) 0x45eff0)(pThis);
	*(unsigned int*) ((char*) pThis + 0x30) = ((unsigned int(__fastcall*) (void*)) 0x45eff0)(pThis);
	unsigned int nData = ((unsigned int(__fastcall*) (void*)) 0x45eff0)(pThis);
	*(unsigned int*) ((char*) pThis + 0x34) = nData;
	if (nData == 0) {
		*(unsigned int*) ((char*) pThis + 0x34) = 2;
	}
	*(unsigned short*) ((char*) pThis + 0x38) = ((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis);
	*(int*) ((char*) pThis + 0x2c) = 1;
	*(int*) ((char*) pThis - 0x24) = 1;
}
