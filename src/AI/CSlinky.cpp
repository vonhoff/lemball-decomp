extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;
extern void* g_pLevelTileGrid;

// FUNCTION: LEMBALL 0x0040b4d0
void __fastcall CSlinky_Set(void* pThis, int nUnused, int param_1, int param_2, int param_3, int param_4)
{
	*(int*) ((char*) pThis + 0x124) = param_1;
	*(int*) ((char*) pThis + 0x128) = param_3;
	*(int*) ((char*) pThis + 0x12c) = param_2;
	*(int*) ((char*) pThis + 0x130) = param_4;
	unsigned int z = 0;
	if (param_1 >= 0 && param_3 >= 0 &&
	    (param_1 >> 4) < *(int*) ((char*) g_pLevelTileGrid + 0x10) &&
	    (param_3 >> 4) < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
		z = ((unsigned int(__fastcall*)(void*, int, int)) 0x4029a5)(
			(void*) ((((param_3 >> 4) * *(int*) ((char*) g_pLevelTileGrid + 0x10) + (param_1 >> 4)) * 0xc) + *(int*) ((char*) g_pLevelTileGrid + 0xc)),
			param_1 & 0xf, param_3 & 0xf);
	}
	*(unsigned int*) ((char*) pThis + 0xa4) = (z & 0xffff) << 12;
	*(int*) ((char*) pThis + 0x9c) = *(int*) ((char*) pThis + 0x124) << 12;
	*(int*) ((char*) pThis + 0xa0) = *(int*) ((char*) pThis + 0x128) << 12;
	*(int*) ((char*) pThis + 0xcc) = g_nLevelFrameClockTick;
	*(int*) ((char*) pThis + 0x94) = g_nLevelFrameClockTimeMs;
	((void(__fastcall*)(void*, int)) (*(void***) pThis)[2])(pThis, 0x18);
}
// FUNCTION: LEMBALL 0x0040b670
void __fastcall CSlinky_Move(void* pThis, int nUnused)
{
	int* pState = *(int**) 0x4a1bcc;
	int v = (*pState * 45 + 31) & 0x7fffff;
	*pState = v;
	int heading = v % 4;
	*(short*) ((char*) pThis + 0xbc) = (short) heading;
	int dx, dy;
	switch (heading) {
	case 0:
		dx = 0x10; dy = 0;
		break;
	case 1:
		dx = -0x10; dy = 0;
		break;
	case 2:
		dx = 0; dy = 0x10;
		break;
	default:
		dx = 0; dy = -0x10;
		break;
	}
	int i = 0;
	do {
		i++;
		*(int*) ((char*) pThis + 0xa8) = ((*(int*) ((char*) pThis + 0x9c) >> 12) + dx) << 12;
		*(int*) ((char*) pThis + 0xac) = ((*(int*) ((char*) pThis + 0xa0) >> 12) + dy) << 12;
		*(int*) ((char*) pThis + 0xb0) = (*(int*) ((char*) pThis + 0xa4) >> 12) << 12;
	} while (i < 8 && ((int(__fastcall*)(void*, int*)) 0x403229)(pThis, (int*) ((char*) pThis + 0xa8)) == 0);
}
