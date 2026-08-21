#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_145)
// FUNCTION: LEMBALL 0x00471e80
void* __fastcall delete_timed_socket_window_channel_stack_wrapper(void* pThis, void* nUnused, char param_1)
{
	((void(__fastcall*)(void*)) 0x46fd70)((char*) pThis + 0xc8);
	((void(__fastcall*)(void*)) 0x45fd80)((char*) pThis + 0x30);
	((void(__fastcall*)(void*)) 0x45f6c0)(pThis);
	if (param_1 & 1) {
		((void(__cdecl*)(void*)) 0x45a790)((char*) pThis - 0x18);
	}
	return (char*) pThis - 0x18;
	(void) nUnused;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_173)
// FUNCTION: LEMBALL 0x00471cd0
void* __fastcall delete_tcpip_socket_channel_stack_wrapper(void* pThis, void* nUnused, char param_1)
{
	((void(__fastcall*)(void*)) 0x46fd70)((char*) pThis + 0x128);
	((void(__fastcall*)(void*)) 0x45f8a0)((char*) pThis + 0xa8);
	((void(__fastcall*)(void*)) 0x45fd80)((char*) pThis + 0x30);
	((void(__fastcall*)(void*)) 0x45f6c0)(pThis);
	if (param_1 & 1) {
		((void(__cdecl*)(void*)) 0x45a790)((char*) pThis - 0x8);
	}
	return (char*) pThis - 0x8;
	(void) nUnused;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_174)
// FUNCTION: LEMBALL 0x00471f60
void* __fastcall delete_tcpip_eff_transport_composite_wrapper(void* pThis, void* nUnused, char param_1)
{
	((void(__fastcall*)(void*)) 0x4704e0)(pThis);
	((void(__fastcall*)(void*)) 0x46fd70)((char*) pThis + 0x128);
	((void(__fastcall*)(void*)) 0x45f8a0)((char*) pThis + 0xa8);
	((void(__fastcall*)(void*)) 0x45fd80)((char*) pThis + 0x30);
	((void(__fastcall*)(void*)) 0x45f6c0)(pThis);
	if (param_1 & 1) {
		((void(__cdecl*)(void*)) 0x45a790)((char*) pThis - 0x30);
	}
	return (char*) pThis - 0x30;
	(void) nUnused;
}
#endif
