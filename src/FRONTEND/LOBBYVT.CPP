#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/GDI/VSWINDOW.H"

// STRONG INFERENCE: callbacks relocated from the artificial aggregate by
// vtable owner, semantic role, and neighboring address/call-graph cluster.

// FUNCTION: LEMBALL 0x00455dc0
void __fastcall mark_network_lobby_player_entry_child_active(void *pEntry) {
    *(int *)((char *)pEntry + 0x38) = 1;
}

// FUNCTION: LEMBALL 0x00455dd0
void __fastcall clear_network_lobby_player_entry_child_active(void *pEntry) {
    *(int *)((char *)pEntry + 0x38) = 0;
}
