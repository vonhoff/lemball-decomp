#include "StatManager.h"

// 68K 0x10218c1a __ct__12CStatManagerFi
// STUB: LEMBALL 0x0045aaf0
StatManager::StatManager(int p_arg0)
{
}

// 68K 0x10218c74 __dt__12CStatManagerFv
// STUB: LEMBALL 0x0045ab30
StatManager::~StatManager()
{
}

// 68K 0x10218d00 Register__12CStatManagerFP9CBaseStat
// STUB: LEMBALL 0x0045ab90
void StatManager::Register(BaseStat* p_stat)
{
}

// 68K 0x10218d50 StreamOut__12CStatManagerFR10CVSOStream
// STUB: LEMBALL 0x0045abb0
VsOStream& StatManager::StreamOut(VsOStream& p_stream)
{
	// STRING: LEMBALL 0x004a1bd4 "Stats Output\n"
	// STRING: LEMBALL 0x004a1be4 "---------------------------------------------------\n"
	// STRING: LEMBALL 0x004a1c1c "Average  Total    Max      Min      nSamples Desc\n"
	// STRING: LEMBALL 0x004a1c50 "---------------------------------------------------\n"
	return *(VsOStream*) 0;
}

// GLOBAL: LEMBALL 0x004a1bd0
StatManager* g_pStatManager = 0;
