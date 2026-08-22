#include "MogLoad.h"

// 68K 0x1020189e __ct__7CMogDirFUl
// ASSERT: _VSRELassert("IsValidResourceFile", "MOGLOAD.CPP", 418)
// STUB: LEMBALL 0x0045bda0
MogDir::MogDir(unsigned long p_arg0)
{
	// STRING: LEMBALL 0x004a1d84 "MOGLOAD.CPP"
	// STRING: LEMBALL 0x004a1d90 "IsValidResourceFile"
}

// 68K 0x10201a68 __dt__7CMogDirFv
// STUB: LEMBALL 0x0045bf10
MogDir::~MogDir()
{
}

// 68K 0x10201b26 GetChunkInfo__7CMogDirFP12tagChunkInfo
// STUB: LEMBALL 0x0045bfa0
void MogDir::GetChunkInfo(ChunkInfo* p_info)
{
}

// 68K 0x10201be8 NewChunkInfo__7CMogDirFv
// STUB: LEMBALL 0x0045c030
ChunkInfo* MogDir::NewChunkInfo()
{
	return 0;
}

// 68K 0x10201c48 GetNextDir__7CMogDirFv
// STUB: LEMBALL 0x0045c060
MogDir* MogDir::GetNextDir()
{
	return 0;
}

// 68K 0x10201dd6 FindNext__7CMogDirFR8tagCHUNK7CHUNKID
// STUB: LEMBALL 0x0045c200
void MogDir::FindNext(Chunk& p_chunk, unsigned int p_type)
{
}

// 68K 0x10201e9a FindFirst__7CMogDirFR8tagCHUNK7CHUNKID
// STUB: LEMBALL 0x0045c2a0
void MogDir::FindFirst(Chunk& p_chunk, unsigned int p_type)
{
}

// 68K 0x10201ef8 Find__7CMogDirFR8tagCHUNKUl7RECURSE
// STUB: LEMBALL 0x0045c2d0
void MogDir::Find(Chunk& p_chunk, unsigned int p_id, unsigned int p_recurse)
{
}
