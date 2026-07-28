#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/GDI/VSWINDOW.H"
#include "GAME.H"

// STRONG INFERENCE: callbacks relocated from the artificial aggregate by
// vtable owner, semantic role, and neighboring address/call-graph cluster.

// FUNCTION: LEMBALL 0x00436690
void noop_render_entry_callback_00436690(void)
{
}
// FUNCTION: LEMBALL 0x004393a0
void PASCAL noop_render_entry_callback_004393a0(void* pUnused)
{
	(void) pUnused;
}

// FUNCTION: LEMBALL 0x004393b0
void noop_render_entry_callback_004393b0(void)
{
}

// FUNCTION: LEMBALL 0x004393d0
void noop_render_entry_callback_004393d0(void)
{
}

// FUNCTION: LEMBALL 0x004393f0
void noop_render_entry_callback_004393f0(void)
{
}

// FUNCTION: LEMBALL 0x00439430
void __fastcall set_render_entry_field_0x04(void* pEntry, void* pUnused, int nValue)
{
	(void) pUnused;
	*(int*) ((char*) pEntry + 4) = nValue;
}

// FUNCTION: LEMBALL 0x00439440
int PASCAL return_zero_status_render_callback(void)
{
	return 0;
}

// FUNCTION: LEMBALL 0x00439450
int __fastcall get_level_screen_transition_status(void* pScreen)
{
	return *(int*) ((char*) pScreen + 0xa78);
}

// FUNCTION: LEMBALL 0x00439460
int __fastcall get_level_screen_requested_mode(void* pScreen)
{
	return *(int*) ((char*) pScreen + 0x8fc);
}

// FUNCTION: LEMBALL 0x00439480
void* __fastcall clear_render_entry_word(void* pEntry)
{
	*(unsigned short*) pEntry = 0;
	return pEntry;
}

// FUNCTION: LEMBALL 0x0043a1c0
void noop_resource_window_owner_callback_0043a1c0(void)
{
}

extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);
extern void* g_pSharedRenderDispatchQueue;
extern void* g_GAME_RenderQueueNodeVtableSlots[4];

// Forward declarations for vtable reference.
void* LEMBALL_FASTCALL DeleteRegisteredRenderSlotArrayAuto(void* pObject, void*, BYTE fDelete);
void LEMBALL_FASTCALL FUN_0043a2e0(void*, int, unsigned short*);

// GLOBAL: LEMBALL 0x00497208
static void* g_RegisteredRenderSlotArrayVtable[4] = {
	(void*) InitializeNonZrleVariantRenderEntry,
	(void*) DeleteRegisteredRenderSlotArrayAuto,
	(void*) FUN_0043a2e0,
	0,
};

// FUNCTION: LEMBALL 0x0043a2b0
void LEMBALL_FASTCALL DestroyRegisteredRenderSlotArrayAuto(void* pObject)
{
	*(void***) pObject = g_RegisteredRenderSlotArrayVtable;
	((GameRenderDispatchQueue*) g_pSharedRenderDispatchQueue)->UnregisterOrderedRenderDispatchClient(pObject, -0x19);
	FreeVSMemBlock(*(void**) ((char*) pObject + 0x10));
	*(void***) pObject = g_GAME_RenderQueueNodeVtableSlots;
}

void LEMBALL_FASTCALL FUN_0043a2e0(void*, int, unsigned short*)
{
}

// Compatibility shim for original ILT call site.
void LEMBALL_FASTCALL DestroyRegisteredRenderSlotArrayAutoThunk(void* pObject)
{
	DestroyRegisteredRenderSlotArrayAuto(pObject);
}

// FUNCTION: LEMBALL 0x0043a490
void* LEMBALL_FASTCALL DeleteRegisteredRenderSlotArrayAuto(void* pObject, void*, BYTE fDelete)
{
	DestroyRegisteredRenderSlotArrayAutoThunk(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x0043a4c0
int PASCAL return_zero_screen_hook(void*, void*)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0043a4d0
void noop_window_owner_slot_18(void)
{
}

// FUNCTION: LEMBALL 0x0043a4e0
void noop_window_owner_slot_21(void)
{
}

// FUNCTION: LEMBALL 0x0043a4f0
void PASCAL noop_frontend_window_owner_slot_22(void*)
{
}

// FUNCTION: LEMBALL 0x0043a500
void noop_window_owner_slot_23(void)
{
}

// FUNCTION: LEMBALL 0x0043a510
void noop_frontend_window_owner_slot_15(void)
{
}

// FUNCTION: LEMBALL 0x0043a520
void noop_frontend_window_owner_slot_16(void)
{
}

// FUNCTION: LEMBALL 0x0043a530
void noop_frontend_window_owner_slot_17(void)
{
}

// FUNCTION: LEMBALL 0x0043a570
unsigned int return_hresult_80000801_screen_hook(void)
{
	return 0x80000801U;
}
// FUNCTION: LEMBALL 0x0043fe70
void noop_level_screen_related_hook_0043fe70(void)
{
}

// FUNCTION: LEMBALL 0x00440c00
void noop_level_screen_constructor_hook(void)
{
}

// FUNCTION: LEMBALL 0x00446990
int PASCAL return_zero_screen_callback_00446990(void* pUnused1, void* pUnused2, void* pUnused3)
{
	(void) pUnused1;
	(void) pUnused2;
	(void) pUnused3;
	return 0;
}

// FUNCTION: LEMBALL 0x00446f50
void update_options_screen(void)
{
}

// FUNCTION: LEMBALL 0x00446f60
int PASCAL return_zero_framed_screen_base_slot_15(void* pUnused)
{
	(void) pUnused;
	return 0;
}

// FUNCTION: LEMBALL 0x00446f70
void noop_screen_callback_00446f70(void)
{
}

// FUNCTION: LEMBALL 0x00446f80
void noop_screen_callback_00446f80(void)
{
}

// FUNCTION: LEMBALL 0x00446f90
void noop_screen_callback_00446f90(void)
{
}

// FUNCTION: LEMBALL 0x00446fa0
int PASCAL return_zero_screen_callback_00446fa0(void* pUnused)
{
	(void) pUnused;
	return 0;
}

// FUNCTION: LEMBALL 0x00446fb0
int __fastcall get_intro_sequence_screen_requested_mode(void* pScreen)
{
	return *(int*) ((char*) pScreen + 0x374);
}

// FUNCTION: LEMBALL 0x00446fc0
int __fastcall poll_intro_sequence_screen_transition_status(void* pScreen)
{
	return *(int*) ((char*) pScreen + 0x378);
}

// FUNCTION: LEMBALL 0x00447540
void noop_intro_sequence_screen_hook(void)
{
}

// FUNCTION: LEMBALL 0x00448610
void noop_main_menu_screen_hook(void)
{
}

// FUNCTION: LEMBALL 0x0044c0b0
int __fastcall get_registration_info_screen_requested_mode(void* pScreen)
{
	return *(int*) ((char*) pScreen + 0x14);
}

// FUNCTION: LEMBALL 0x0044efe0
unsigned int return_hresult_80000801_progress_overlay(void)
{
	return 0x80000801U;
}

extern void SetLevelScreenStatusIndicatorModeThunk(int nMode, int nValue);

void PASCAL ForwardLevelScreenStatusIndicatorMode(int nMode, int nValue)
{
	SetLevelScreenStatusIndicatorModeThunk(nMode, nValue);
}
