#include "MogRes.h"

// 68K 0x10201fc8 __ct__7CMogResFPcPPcUl
// STUB: LEMBALL 0x0045c630
MogRes::MogRes(char* p_arg0, char** p_arg1, unsigned long p_arg2)
{
	// STRING: LEMBALL 0x004a1da4 "Resource Data Arena"
	// STRING: LEMBALL 0x004a1dbc "Mogload memory"
}

// 68K 0x1020213a __dt__7CMogResFv
// STUB: LEMBALL 0x0045c770
MogRes::~MogRes()
{
}

// 68K 0x10202202 SetWD__7CMogResFPc
// STUB: LEMBALL 0x0045c810
bool MogRes::SetWd(char* p_path)
{
	return 0;
}

// 68K 0x10202332 KillLeastResource__7CMogResFUl
// STUB: LEMBALL 0x0045c940
int MogRes::KillLeastResource(unsigned int p_requiredSize)
{
	return 0;
}

// 68K 0x102023fe GetFreeHandle__7CMogResFv
// STUB: LEMBALL 0x0045c9d0
int MogRes::GetFreeHandle()
{
	return 0;
}

// 68K 0x1020247e AllocateMainMem__7CMogResFUl
// STUB: LEMBALL 0x0045ca30
unsigned char* MogRes::AllocateMainMem(unsigned int p_size)
{
	return 0;
}

// 68K 0x1020253c Find__7CMogResFUl
// STUB: LEMBALL 0x0045cab0
ResBase* MogRes::Find(unsigned int p_resourceId)
{
	return 0;
}

// 68K 0x102025d4 Load__7CMogResFP8CResBase8tagCHUNK
// STUB: LEMBALL 0x0045cb50
bool MogRes::Load(ResBase* p_resource, Chunk p_chunk)
{
	return 0;
}

// 68K 0x1020264a Load__7CMogResFUlP8CResBase7RECURSE
// STUB: LEMBALL 0x0045cb80
bool MogRes::Load(unsigned int p_resourceId, ResBase* p_resource, unsigned int p_recurse)
{
	return 0;
}

// 68K 0x10202716 CheckAllUnloaded__7CMogResFv
// STUB: LEMBALL 0x0045cd60
bool MogRes::CheckAllUnloaded()
{
	return 0;
}

// 68K 0x1020278a AgeResources__7CMogResFv
// STUB: LEMBALL 0x0045cdb0
void MogRes::AgeResources()
{
}

// 68K 0x1020280a Load__7CMogResFRC8CVSRangeRPUcP8CResBase
// STUB: LEMBALL 0x0045ce00
bool MogRes::Load(const VsRange& p_range, unsigned char*& p_data, ResBase* p_resource)
{
	return 0;
}

// 68K 0x10202896 CleanUpResources__7CMogResFv
// STUB: LEMBALL 0x0045ce50
void MogRes::CleanUpResources()
{
}

// 68K 0x10202932 Remove__7CMogResFP8CResBase
// STUB: LEMBALL 0x0045ceb0
void MogRes::Remove(ResBase* p_resource)
{
}

// 68K 0x1020299e DeallocateMem__7CMogResFPUcUc
// STUB: LEMBALL 0x0045cf10
void MogRes::DeallocateMem(unsigned char* p_data, unsigned char p_owned)
{
}

// GLOBAL: LEMBALL 0x004a1d58
MogRes* g_pMogRes = 0;

// GLOBAL: LEMBALL 0x004a1d5c
MogRes* g_pActiveMogRes = 0;
