#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/GDI/VSWINDOW.H"

// STRONG INFERENCE: callbacks relocated from the artificial aggregate by
// vtable owner, semantic role, and neighboring address/call-graph cluster.

// FUNCTION: LEMBALL 0x00436690
void noop_render_entry_callback_00436690(void) {
}

// FUNCTION: LEMBALL 0x004393A0
void PASCAL noop_render_entry_callback_004393a0(void *pUnused) {
    (void)pUnused;
}

// FUNCTION: LEMBALL 0x004393B0
void noop_render_entry_callback_004393b0(void) {
}

// FUNCTION: LEMBALL 0x004393D0
void noop_render_entry_callback_004393d0(void) {
}

// FUNCTION: LEMBALL 0x004393F0
void noop_render_entry_callback_004393f0(void) {
}

// FUNCTION: LEMBALL 0x0043A1C0
void noop_resource_window_owner_callback_0043a1c0(void) {
}

// FUNCTION: LEMBALL 0x0043A570
unsigned int return_hresult_80000801_screen_hook(void) {
    return 0x80000801U;
}

// FUNCTION: LEMBALL 0x0043FE70
void noop_level_screen_related_hook_0043fe70(void) {
}

// FUNCTION: LEMBALL 0x00440C00
void noop_level_screen_constructor_hook(void) {
}

// FUNCTION: LEMBALL 0x00446990
int PASCAL return_zero_screen_callback_00446990(void *pUnused1, void *pUnused2, void *pUnused3) {
    (void)pUnused1;
    (void)pUnused2;
    (void)pUnused3;
    return 0;
}

// FUNCTION: LEMBALL 0x00446F50
void update_options_screen(void) {
}

// FUNCTION: LEMBALL 0x00446F60
int PASCAL return_zero_screen_callback_00446f60(void *pUnused) {
    (void)pUnused;
    return 0;
}

// FUNCTION: LEMBALL 0x00446F70
void noop_screen_callback_00446f70(void) {
}

// FUNCTION: LEMBALL 0x00446F80
void noop_screen_callback_00446f80(void) {
}

// FUNCTION: LEMBALL 0x00446FA0
int PASCAL return_zero_screen_callback_00446fa0(void *pUnused) {
    (void)pUnused;
    return 0;
}

// FUNCTION: LEMBALL 0x00447540
void noop_intro_sequence_screen_hook(void) {
}

// FUNCTION: LEMBALL 0x00448610
void noop_main_menu_screen_hook(void) {
}

// FUNCTION: LEMBALL 0x0044EFE0
unsigned int return_hresult_80000801_progress_overlay(void) {
    return 0x80000801U;
}

// FUNCTION: LEMBALL 0x00439450
int __fastcall get_level_screen_transition_status(void *pScreen) {
    return *(int *)((char *)pScreen + 0xa78);
}

// FUNCTION: LEMBALL 0x00439460
int __fastcall get_level_screen_requested_mode(void *pScreen) {
    return *(int *)((char *)pScreen + 0x8fc);
}

// FUNCTION: LEMBALL 0x00446FB0
int __fastcall get_intro_sequence_screen_requested_mode(void *pScreen) {
    return *(int *)((char *)pScreen + 0x374);
}

// FUNCTION: LEMBALL 0x00446FC0
int __fastcall poll_intro_sequence_screen_transition_status(void *pScreen) {
    return *(int *)((char *)pScreen + 0x378);
}

// FUNCTION: LEMBALL 0x0044C0B0
int __fastcall get_registration_info_screen_requested_mode(void *pScreen) {
    return *(int *)((char *)pScreen + 0x14);
}

// FUNCTION: LEMBALL 0x00439430
void __fastcall set_render_entry_field_0x04(void *pEntry, void *pUnused, int nValue) {
    (void)pUnused;
    *(int *)((char *)pEntry + 4) = nValue;
}

// FUNCTION: LEMBALL 0x00439480
void *__fastcall clear_render_entry_word(void *pEntry) {
    *(unsigned short *)pEntry = 0;
    return pEntry;
}
