#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/GDI/VSWINDOW.H"

// STRONG INFERENCE: callbacks relocated from the artificial aggregate by
// vtable owner, semantic role, and neighboring address/call-graph cluster.

// FUNCTION: LEMBALL 0x004749b0
void* __fastcall initialize_debug_text_line_record(void* pRecord)
{
	*(int*) ((char*) pRecord + 4) = 0;
	*(int*) pRecord = 0;
	*(int*) ((char*) pRecord + 8) = 0;
	return pRecord;
}
