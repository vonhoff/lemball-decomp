#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_123)
// FUNCTION: LEMBALL 0x00469810
void* __fastcall construct_resource_sprite_window_owner_variant_c(void* pThis,
																  void* pUnusedEdx,
																  void* param_2,
																  void* param_3,
																  void* param_4,
																  void* param_5,
																  void* param_6)
{
	((void*(
		__fastcall*) (void*, int, void*, void*, void*, void*) ) 0x469070)(pThis, 0, param_3, param_4, param_5, param_6);
	*(void**) pThis = (void*) 0x499c18;
	*(void**) ((char*) pThis + 0x90) = (void*) 0x499bf8;
	((void(__fastcall*)(void*)) 0x469860)(pThis);
	*(void**) ((char*) pThis + 0xcc) = param_2;
	return pThis;
}
#endif
