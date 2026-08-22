#include "MasterInput.h"

// 68K 0x10201326 __ct__12CMasterInputFP10CBaseQueue
// STUB: LEMBALL 0x00472070
MasterInput::MasterInput(BaseQueue* p_arg0)
{
}

// 68K 0x102013e8 StreamOut__12CMasterInputFR10CVSOStream
// STUB: LEMBALL 0x00472210
VsOStream& MasterInput::StreamOut(VsOStream& p_arg0)
{
	return *(VsOStream*) 0;
}

// 68K 0x1020137c __dt__12CMasterInputFv
// SYNTHETIC: LEMBALL 0x00472270
// MasterInput::`scalar deleting destructor'
MasterInput::~MasterInput()
{
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x004a279c
MasterInput* g_pMasterInput;
