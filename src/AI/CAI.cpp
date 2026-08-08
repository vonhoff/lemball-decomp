extern void* g_pLevelDemoPlaybackController;
extern int g_nSelectedNetworkLobbyPeerId;
extern void* g_pActiveNetworkRuntimeWindow;
extern void __fastcall AppendType18ChunkObject(void* pStream, int nUnused, unsigned short param_1, void* param_2, int param_3, int param_4);
extern "C" unsigned long __stdcall timeGetTime();

// FUNCTION: LEMBALL 0x00412eb0
void __fastcall CAI_AddNewTrapDoor(void* pObject, int nUnused, int param_1, int param_2, int param_3, int param_4)
{
	int nSlot = ((int(__fastcall*)()) 0x40227a)();
	int local_c = param_1 << 12;
	int local_8 = param_2 << 12;
	int local_4 = param_3 << 12;
	AppendType18ChunkObject(*(void**) ((char*) pObject + 0x1c4), nUnused, (unsigned short) nSlot, &local_c, 0, param_4);
}
// FUNCTION: LEMBALL 0x004125c0
void __fastcall CAI_HitTrampoline(void* pObject, int nUnused, void* param_1, void* param_2)
{
	((void(__fastcall*)(void*, void*, void*)) 0x402bd5)(*(void**) ((char*) pObject + 0x1b4), param_1, param_2);
}
// FUNCTION: LEMBALL 0x004130d0
void __fastcall CAI_FindMoverHeight(void* pObject, int nUnused, void* param_1, void* param_2, void* param_3)
{
	((void(__fastcall*)(void*, void*, void*, void*)) 0x402e55)(*(void**) ((char*) pObject + 0x1c0), param_1, param_2, param_3);
}
// FUNCTION: LEMBALL 0x004127e0
void __fastcall CAI_BulletCheckGroupIntersection(void* pObject, int nUnused, void* param_1, void* param_2)
{
	((void(__fastcall*)(void*, void*, void*)) 0x402680)(*(void**) ((char*) pObject + 0x168), param_1, param_2);
}
// FUNCTION: LEMBALL 0x00411b10
void __fastcall CAI_Start(void* param_1)
{
	if (*(int*) ((char*) param_1 + 0x64) != 0) {
		*(int*) ((char*) param_1 + 0x6c) = 1;
		*(int*) ((char*) param_1 + 0x70) = 0;
		void* pRuntime = *(void**) 0x4a0120;
		*(int*) ((char*) pRuntime + 0x48) = 3;
		*(int*) ((char*) pRuntime + 0x4c) = 0;
		return;
	}
	if (g_pLevelDemoPlaybackController != 0 && *(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) != 0) {
		*(int*) ((char*) g_pLevelDemoPlaybackController + 0x38) = (int) ((unsigned(__fastcall*)()) 0x462e80)();
		*(int*) ((char*) g_pLevelDemoPlaybackController + 0x3c) = 0;
	}
	((void(__fastcall*)(void*, int)) 0x4013ed)(param_1, 2);
	*(int*) ((char*) param_1 + 0x68) = 1;
}
// FUNCTION: LEMBALL 0x004131e0
void __fastcall CAI_SetNetworkTrapDoors(void* pThis, int nUnused, int param_1, int param_2, int param_3, int param_4, int param_5)
{
	*(int*) ((char*) pThis + 0xd0) = param_1;
	if (param_2 == -1) {
		*(int*) ((char*) pThis + 0xc0) = *(int*) (0x49cf50 + (param_1 << 4));
		*(int*) ((char*) pThis + 0xc4) = *(int*) (0x49cf54 + (param_1 << 4));
		*(int*) ((char*) pThis + 0xc8) = *(int*) (0x49cf58 + (param_1 << 4));
		*(int*) ((char*) pThis + 0xcc) = *(int*) (0x49cf5c + (param_1 << 4));
	} else {
		*(int*) ((char*) pThis + 0xc0) = param_2;
		*(int*) ((char*) pThis + 0xc4) = param_3;
		*(int*) ((char*) pThis + 0xc8) = param_4;
		*(int*) ((char*) pThis + 0xcc) = param_5;
	}
	if (param_1 > 0) {
		int* pi = (int*) ((char*) pThis + 0x90);
		int v = 0;
		int count = param_1;
		do {
			if (pi[0] > 0x400 || pi[0] < 0) {
				pi[0] = v;
			}
			if (pi[4] > 0x400 || pi[4] < 0) {
				pi[4] = v;
			}
			pi++;
			v += 0x10;
		} while (--count != 0);
	}
}
// FUNCTION: LEMBALL 0x00412660
int __fastcall CAI_ProcessMsg(void* pThis, int nUnused, short* param_1)
{
	if (param_1[0] == 4) {
		((void(__fastcall*)(void*, int, int)) 0x402d74)(
			*(void**) ((char*) pThis + 0x15c), *(int*) (param_1 + 4), *(int*) (param_1 + 6));
		return 0;
	}
	if (*(int*) ((char*) pThis + 0x58) == 0) {
		return 1;
	}
	switch (param_1[0]) {
	case 2:
		((void(__fastcall*)(void*, int, int)) 0x40201d)(
			*(void**) ((char*) pThis + 0x15c), *(int*) (param_1 + 4), *(int*) (param_1 + 6));
		return 0;
	case 3:
		((void(__fastcall*)(void*)) 0x40121c)(*(void**) ((char*) pThis + 0x15c));
		return 0;
	case 6:
		((void(__fastcall*)(void*)) 0x402d47)(*(void**) ((char*) pThis + 0x15c));
		return 0;
	case 7:
		((void(__fastcall*)(void*)) 0x402509)(*(void**) ((char*) pThis + 0x15c));
		return 0;
	case 8:
		((void(__fastcall*)(void*, int)) 0x40378d)(
			*(void**) ((char*) pThis + 0x15c), *(int*) (param_1 + 4));
		return 0;
	case 5:
		(*( void(**)(void*, int, int)) (*(void***) *(void**) ((char*) pThis + 0x15c) + 0x34 / 4))(
			*(void**) ((char*) pThis + 0x15c), *(int*) (param_1 + 4), *(int*) (param_1 + 6));
		return 0;
	default:
		*(int*) ((char*) pThis + 0xc) = *(int*) ((char*) pThis + 0xc) + 1;
		return 0;
	}
}
// FUNCTION: LEMBALL 0x00411b70
void __fastcall CAI_SendGameState(void* pThis, int nUnused, int param_1, int param_2)
{
	if (g_nSelectedNetworkLobbyPeerId == 0) {
		return;
	}
	if (*(int*) (*(int*) ((char*) pThis + 0x74) + 0x28) != 0) {
		unsigned long start = timeGetTime();
		while (*(int*) (*(int*) ((char*) pThis + 0x74) + 0x28) != 0 &&
		       timeGetTime() - start < 2000) {
			(*(void(**)(void*)) (*(int*) *(int*) g_pActiveNetworkRuntimeWindow + 0x30))(*(void**) g_pActiveNetworkRuntimeWindow);
		}
	}
	if (*(int*) (*(int*) ((char*) pThis + 0x74) + 0x28) == 0) {
		*(int*) ((char*) pThis + 0x6c) = 1;
		*(int*) (*(int*) ((char*) pThis + 0x74) + 0x2c) = param_1;
		*(int*) (*(int*) ((char*) pThis + 0x74) + 0x30) = param_2;
		*(int*) (*(int*) ((char*) pThis + 0x74) + 0x34) = *(int*) ((char*) pThis + 0xe8);
		*(int*) (*(int*) ((char*) pThis + 0x74) + 0x38) = *(int*) ((char*) pThis + 0xf0);
		((void(__fastcall*) (void*, int)) 0x45f2b0)(*(void**) ((char*) pThis + 0x74), g_nSelectedNetworkLobbyPeerId);
	}
}
// FUNCTION: LEMBALL 0x00412ad0
int __fastcall CAI_OpenDoor(void* pThis, int nUnused, int* param_1, int* param_2, unsigned char param_3)
{
	int tileX = ((param_1[0] >> 12) + ((param_1[0] >> 12) >> 31 & 0xf)) >> 4;
	int tileY = ((param_1[1] >> 12) + ((param_1[1] >> 12) >> 31 & 0xf)) >> 4;
	unsigned short tile = 0x3;
	if (tileX >= 0 && tileY >= 0) {
		void* grid = *(void**) ((char*) pThis + 0x110);
		if (tileX < *(int*) ((char*) grid + 0x10) && tileY < *(int*) ((char*) grid + 0x14)) {
			tile = *(unsigned short*) (*(int*) ((char*) grid + 0xc) + 6 + (*(int*) ((char*) grid + 0x10) * tileY + tileX) * 0xc);
		}
	}
	if ((tile & 0x8000) && (param_3 & 0x20)) {
		return ((int(__fastcall*) (void*, int, void*, void*)) 0x401fa5)(*(void**) ((char*) pThis + 0x190), 0, param_1, param_2);
	}
	return 0;
}
