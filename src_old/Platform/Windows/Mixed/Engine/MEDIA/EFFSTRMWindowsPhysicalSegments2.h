#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_065)
// FUNCTION: LEMBALL 0x004550c0
void __fastcall DestroyNetworkLobbyTransportController(void* param_1)
{
	*(void**) param_1 = (void*) 0x4986e8;
	*(void**) ((char*) param_1 + 0xc) = (void*) 0x4986d8;
	*(int*) 0x4a0128 = 0;
	if (*(void**) 0x49f144 != 0) {
		int iVar1 = (*(int (**)(void))(*(void***) *(void**) 0x49f144 + 0x28 / 4))();
		if (iVar1 == 0) {
			((void(__fastcall*)(void*)) 0x4013a7)(param_1);
		}
		else {
			((void(__fastcall*)(void*)) 0x402c11)(param_1);
		}
	}
	if (*(void**) ((char*) param_1 + 0x30) != 0) {
		(*(void (**)(int))(*(void***) *(void**) ((char*) param_1 + 0x30) + 0x14 / 4))(1);
	}
	if (*(void**) ((char*) param_1 + 0x34) != 0) {
		(*(void (**)(int))(*(void***) *(void**) ((char*) param_1 + 0x34) + 0x14 / 4))(1);
	}
	((void(__fastcall*)(void*)) 0x403062)(param_1);
}
#endif
