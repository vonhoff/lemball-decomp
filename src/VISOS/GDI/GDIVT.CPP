#include "VISOS/WIN32.H"
#include "VISOS/VSWINDOW.H"
#include "LEMBALL/GAME.H"

// STRONG INFERENCE: callbacks relocated from the artificial aggregate by
// vtable owner, semantic role, and neighboring address/call-graph cluster.

// FUNCTION: LEMBALL 0x0046BF60
void lookup_resource_geometry_helper_slot(void) {
}

// FUNCTION: LEMBALL 0x004794C0
int PASCAL noop_visual_sciences_wing_draw_change_palette(void *pUnused1, void *pUnused2) {
    (void)pUnused1;
    (void)pUnused2;
    return 0;
}

// FUNCTION: LEMBALL 0x004794D0
int PASCAL noop_visual_sciences_wing_draw_end(void *pUnused) {
    (void)pUnused;
    return 0;
}

// FUNCTION: LEMBALL 0x00466990
int __fastcall get_helper_group_1_dirty_flag(void *pHelperGroup) {
    return *(int *)((char *)pHelperGroup - 0x10);
}

// FUNCTION: LEMBALL 0x0046DB30
int __fastcall get_helper_group_0_dirty_flag_local(void *pHelperGroup) {
    char *pOwner;
    char *pState;

    pOwner = *(char **)((char *)pHelperGroup - 4);
    pState = *(char **)(pOwner + 4);
    return *(int *)(pState + (int)(unsigned long)pHelperGroup + 0x3c);
}

// FUNCTION: LEMBALL 0x004756D0
void *__fastcall get_draw_command_rect_payload(void *pCommand) {
    return (char *)pCommand + 4;
}

// FUNCTION: LEMBALL 0x00467E40
void __fastcall set_overlay_owner_field_0xf8(void *pOwner, void *pUnused, int nValue) {
    (void)pUnused;
    *(int *)((char *)pOwner + 0xf8) = nValue;
}

// FUNCTION: LEMBALL 0x0046A070
void __fastcall clear_mapped_list_resource_table_pending_flag(void *pTable) {
    *(int *)((char *)pTable + 0x18) = 0;
}

// FUNCTION: LEMBALL 0x00469860
void __fastcall initialize_resource_sprite_window_owner_variant_c_state(void *pObject) {
    *(void **)((char *)pObject + 0xf4) = g_pSharedRenderDispatchQueue;
}
