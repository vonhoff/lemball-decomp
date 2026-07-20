#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/GDI/VSWINDOW.H"
#include "GAME.H"

// STRONG INFERENCE: callbacks relocated from the artificial aggregate by
// vtable owner, semantic role, and neighboring address/call-graph cluster.

// FUNCTION: LEMBALL 0x00467e40
void __fastcall set_overlay_owner_field_0xf8(void* pOwner, void* pUnused, int nValue)
{
	(void) pUnused;
	*(int*) ((char*) pOwner + 0xf8) = nValue;
}
// FUNCTION: LEMBALL 0x00469860
void __fastcall initialize_resource_sprite_window_owner_variant_c_state(void* pObject)
{
	*(void**) ((char*) pObject + 0xf4) = g_pSharedRenderDispatchQueue;
}
// FUNCTION: LEMBALL 0x0046a070
void __fastcall clear_mapped_list_resource_table_pending_flag(void* pTable)
{
	*(int*) ((char*) pTable + 0x18) = 0;
}

// FUNCTION: LEMBALL 0x0046bf60
void lookup_resource_geometry_helper_slot(void)
{
}

// FUNCTION: LEMBALL 0x0046db30
int __fastcall get_helper_group_0_dirty_flag_local(void* pHelperGroup)
{
	char* pOwner;
	char* pState;

	pOwner = *(char**) ((char*) pHelperGroup - 4);
	pState = *(char**) (pOwner + 4);
	return *(int*) (pState + (int) (unsigned long) pHelperGroup + 0x3c);
}

// FUNCTION: LEMBALL 0x004756d0
void* __fastcall get_draw_command_rect_payload(void* pCommand)
{
	return (char*) pCommand + 4;
}

// FUNCTION: LEMBALL 0x004794c0
int PASCAL noop_visual_sciences_wing_draw_change_palette(void* pUnused1, void* pUnused2)
{
	(void) pUnused1;
	(void) pUnused2;
	return 0;
}

// FUNCTION: LEMBALL 0x004794d0
int PASCAL noop_visual_sciences_wing_draw_end(void* pUnused)
{
	(void) pUnused;
	return 0;
}
