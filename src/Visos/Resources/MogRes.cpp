#include "MogRes.h"

#include <new.h>
#include <string.h>

#include "../Animation/BaseStat.h"
#include "../Animation/StatManager.h"
#include "../Foundation/Arena.h"
#include "../Foundation/Chunk.h"
#include "../Foundation/ChunkInfo.h"
#include "../Foundation/VsFile.h"
#include "../Foundation/VsRange.h"
#include "../Foundation/VsTime.h"
#include "MogLoad.h"
#include "MogloadArena.h"
#include "RawRead.h"
#include "ResBase.h"

#define kPathSeparator '/'

// GLOBAL: LEMBALL 0x004a1d80
char g_mogRootPath[4];

// GLOBAL: LEMBALL 0x004a1d60
BaseStat* g_pMogloadStat = 0;

// 68K 0x10201fc8 __ct__7CMogResFPcPPcUl
// FUNCTION: LEMBALL 0x0045c630
MogRes::MogRes(char* p_path, unsigned long p_arenaSize)
{
	int offset;
	Arena* arena;

	g_mogRootPath[0] = kPathSeparator;
	g_pActiveMogRes = this;
	m_error = 0;
	m_resources = 0;
	m_workingPath = 0;
	m_rootDirectory = 0;
	m_workingDirectory = 0;
	m_resourceCount = 0;
	m_skipCleanup = 0;
	m_arenaSize = p_arenaSize;
	// STRING: LEMBALL 0x004a1da4 "Resource Data Arena"
	if (g_pMasterArena->AllocateArena(&arena, p_arenaSize, "Resource Data Arena") == 1) {
		m_externalArena = 0;
	}
	g_pMogloadArena = arena;
	// STRING: LEMBALL 0x004a1db8 "rb"
	if (!Open(p_path, "rb")) {
		m_error = 1;
		return;
	}
	CurrentMilliTimer();
	m_rootDirectory = new MogDir(0);
	CurrentMilliTimer();
	m_workingDirectory = m_rootDirectory;
	SetWd(g_mogRootPath);
	m_resources = (ResBase**) MogloadArena::operator new(kResourceHandleBytes);
	for (offset = 0; offset < 1024; offset++) {
		m_resources[offset] = 0;
	}
	// STRING: LEMBALL 0x004a1dbc "Mogload memory"
	g_pMogloadStat = new BaseStat("Mogload memory");
	g_pStatManager->Register(g_pMogloadStat);
	g_pMogloadArena->m_parentArena = (Arena*) g_pMogloadStat;
}

// 68K 0x1020213a __dt__7CMogResFv
// FUNCTION: LEMBALL 0x0045c770
MogRes::~MogRes()
{
	if (g_pMogFile != 0) {
		VsClose(g_pMogFile);
	}
	CheckAllUnloaded();
	if (m_skipCleanup == 0) {
		CleanUpResources();
	}
	if (m_resources != 0) {
		MogloadArena::operator delete(m_resources);
		m_resources = 0;
	}
	if (m_rootDirectory != 0) {
		delete m_rootDirectory;
		m_rootDirectory = 0;
	}
	if (m_workingPath != 0) {
		MogloadArena::operator delete(m_workingPath);
		m_workingPath = 0;
	}
	if (m_externalArena == 0) {
		g_pMasterArena->FreeArena(g_pMogloadArena);
	}
	g_pMogloadArena = 0;
}

// 68K 0x10202202 SetWD__7CMogResFPc
// FUNCTION: LEMBALL 0x0045c810
bool MogRes::SetWd(char* p_path)
{
	char* copy;
	char* cursor;
	MogDir* dir;

	if (*p_path == kPathSeparator) {
		m_workingDirectory = m_rootDirectory;
		copy = (char*) MogloadArena::operator new(strlen(p_path) + 1);
		strcpy(copy, p_path);
	}
	else {
		copy = (char*) MogloadArena::operator new(strlen(p_path) + 2);
		copy[0] = kPathSeparator;
		strcpy(copy + 1, p_path);
	}
	m_workingDirectory->m_currentDirIndex = m_workingDirectory->m_rootIndex;
	m_workingDirectory->m_currentDirChunk = m_workingDirectory->m_rootChunk;
	m_workingDirectory->m_currentDirIndex = -1;
	cursor = copy;
	while (cursor != 0) {
		cursor = strchr(cursor, kPathSeparator);
		if (cursor == 0) {
			break;
		}
		cursor++;
		if (*cursor != '\0') {
			do {
				dir = m_workingDirectory->GetNextDir();
				if (dir == 0) {
					break;
				}
			} while (!NameCmp((char*) m_workingDirectory->m_currentDirChunk->m_data, cursor));
			if (dir == 0) {
				break;
			}
			m_workingDirectory = dir;
		}
	}
	if (cursor == 0) {
		if (m_workingPath != 0) {
			MogloadArena::operator delete(m_workingPath);
			m_workingPath = 0;
		}
		m_workingPath = copy;
		return 1;
	}
	SetWd(m_workingPath);
	if (copy != 0) {
		MogloadArena::operator delete(copy);
	}
	return 0;
}

// 68K 0x10202332 KillLeastResource__7CMogResFUl
// FUNCTION: LEMBALL 0x0045c940
int MogRes::KillLeastResource(unsigned int p_requiredSize)
{
	int i = 0;
	unsigned int bestRefs = 0xffffffff;
	int scanned = 0;
	int bestIndex = -1;
	unsigned int bestSize = 0;

	if ((int) m_resourceCount > 0) {
		do {
			if (m_resources[i] == 0) {
				ResBase** slot = &m_resources[i];
				do {
					slot++;
					i++;
				} while (*slot == 0);
			}
			ResBase* resource = m_resources[i];
			if (resource->m_loaded != 0 && resource->m_directUseCount == 0) {
				unsigned int used = resource->GetSizeUsed();
				unsigned int refs = m_resources[i]->m_referenceCount;
				if (used < p_requiredSize) {
				check_refs:
					if (bestRefs <= refs) {
						goto next;
					}
				}
				else if (used <= bestSize && bestRefs <= refs) {
					if (used >= p_requiredSize) {
						goto next;
					}
					goto check_refs;
				}
				bestRefs = refs;
				bestSize = used;
				bestIndex = i;
			}
		next:
			i++;
			scanned++;
		} while (scanned < (int) m_resourceCount);
	}
	return bestIndex;
}

// 68K 0x102023fe GetFreeHandle__7CMogResFv
// FUNCTION: LEMBALL 0x0045c9d0
int MogRes::GetFreeHandle()
{
	int i = 0;
	int handle = -1;

	if ((int) m_resourceCount > 0) {
		if ((int) m_resourceCount < kResourceHandleCount) {
			while (i < kResourceHandleCount && m_resources[i] != 0) {
				i++;
			}
		}
		else {
			while (i < kResourceHandleCount && m_resources[i]->m_referenceCount != 0) {
				i++;
			}
		}
		if (i < kResourceHandleCount) {
			handle = i;
		}
		return handle;
	}
	return 0;
}

// 68K 0x1020247e AllocateMainMem__7CMogResFUl
// FUNCTION: LEMBALL 0x0045ca30
unsigned char* MogRes::AllocateMainMem(unsigned int p_size)
{
	register MogRes* const self = this;
	register unsigned int size = p_size;
	unsigned char* memory;

	do {
		memory = (unsigned char*) MogloadArena::operator new(size);
		if (memory == 0) {
			int needed = size - g_pMogloadArena->GetFreeSize();
			if (needed < 0) {
				needed = size;
			}
			int handle = self->KillLeastResource(needed);
			if (handle != -1) {
				self->m_resources[handle]->UnLoadData(1);
			}
			if (handle == -1) {
				int i = 0;
				int count = self->m_resourceCount;
				if (count > 0) {
					do {
						if (self->m_resources[i] == 0) {
							ResBase** slot = self->m_resources + i;
							do {
								slot++;
								i++;
							} while (*slot == 0);
						}
						i++;
						count--;
					} while (count != 0);
				}
			}
		}
	} while (memory == 0);
	return memory;
}

// 68K 0x1020253c Find__7CMogResFUl
// FUNCTION: LEMBALL 0x0045cab0
ResBase* MogRes::Find(unsigned int p_resourceId)
{
	int i = 0;
	int count = m_resourceCount;
	int remaining = count;

	while (remaining > 0) {
		if (m_resources[i] == 0) {
			ResBase** slot = &m_resources[i];
			do {
				slot++;
				i++;
			} while (*slot == 0);
		}
		if (m_resources[i]->m_resourceId == p_resourceId) {
			break;
		}
		remaining--;
		i++;
	}
	if (count != 0 && remaining > 0) {
		AgeResources();
		m_resources[i]->LoadData();
		m_resources[i]->m_referenceCount++;
		return m_resources[i];
	}
	return 0;
}

// 68K 0x102025d4 Load__7CMogResFP8CResBase8tagCHUNK
// FUNCTION: LEMBALL 0x0045cb50
bool MogRes::Load(ResBase* p_resource, Chunk p_chunk)
{
	if (p_chunk.m_info->m_type != p_resource->m_chunkType) {
		return 0;
	}
	p_resource->m_dataSize = p_chunk.m_info->m_size;
	p_resource->m_fileOffset = p_chunk.m_info->m_fileOffset;
	p_resource->m_name = p_chunk.m_info->m_name;
	return 1;
}

// 68K 0x1020264a Load__7CMogResFUlP8CResBase7RECURSE
// FUNCTION: LEMBALL 0x0045cb80
bool MogRes::Load(unsigned int p_resourceId, ResBase* p_resource, unsigned int p_recurse)
{
	Chunk chunk;
	int handle;

	m_workingDirectory->Find(chunk, p_resourceId, p_recurse);
	if (chunk.m_info != 0) {
		handle = GetFreeHandle();
		ResBase* res = m_resources[handle];
		if (res != 0) {
			delete res;
			m_resources[handle] = 0;
			m_resourceCount--;
		}
		m_resources[handle] = p_resource;
		m_resourceCount++;
		return Load(p_resource, chunk);
	}
	return 0;
}

// 68K 0x10202716 CheckAllUnloaded__7CMogResFv
// FUNCTION: LEMBALL 0x0045cd60
bool MogRes::CheckAllUnloaded()
{
	int loaded;
	int remaining;
	int i;

	i = 0;
	loaded = 0;
	remaining = m_resourceCount;

	if (remaining != 0) {
		do {
			while (m_resources[i] == 0 && i < kResourceHandleCount) {
				i++;
			}
			if (m_resources[i]->m_referenceCount != 0) {
				loaded = 1;
			}
			i++;
			remaining--;
		} while (remaining != 0);
	}
	return loaded == 0;
}

// 68K 0x1020278a AgeResources__7CMogResFv
// FUNCTION: LEMBALL 0x0045cdb0
void MogRes::AgeResources()
{
	int i;
	int zero;
	int scanned;

	i = 0;
	zero = 0;
	scanned = 0;

	if ((int) m_resourceCount > zero) {
		do {
			ResBase* resource = m_resources[i];
			if (resource == 0) {
				ResBase** slot = &m_resources[i];
				do {
					slot++;
					i++;
				} while (*slot == 0);
			}
			if (m_resources[i]->m_loaded != 0 || m_resources[i]->GetfVramLoaded()) {
				m_resources[i]->m_age++;
			}
			scanned++;
			i++;
		} while ((int) m_resourceCount > scanned);
	}
}

// 68K 0x1020280a Load__7CMogResFRC8CVSRangeRPUcP8CResBase
// FUNCTION: LEMBALL 0x0045ce00
bool MogRes::Load(const VsRange& p_range, unsigned char*& p_data, ResBase* p_resource)
{
	p_data = AllocateMainMem(p_range.m_size);
	VsSeek(g_pMogFile, p_range.m_offset + 8, 0);
	VsRead(g_pMogFile, p_data, p_range.m_size);
	return 1;
}

// 68K 0x10202896 CleanUpResources__7CMogResFv
// FUNCTION: LEMBALL 0x0045ce50
void MogRes::CleanUpResources()
{
	unsigned int count = m_resourceCount;
	int i = 0;
	int scanned = 0;

	if ((int) count > 0) {
		do {
			while (m_resources[i] == 0) {
				i++;
			}
			if (i == kResourceHandleCount) {
				return;
			}
			if (m_resources[i]->m_referenceCount == 0) {
				if (m_resources[i] != 0) {
					delete m_resources[i];
				}
				m_resources[i] = 0;
				m_resourceCount--;
			}
			scanned++;
			i++;
		} while (scanned < (int) count);
	}
}

// 68K 0x10202932 Remove__7CMogResFP8CResBase
// FUNCTION: LEMBALL 0x0045ceb0
void MogRes::Remove(ResBase* p_resource)
{
	int count = m_resourceCount;
	int scanned = 0;
	int i = 0;

	if (count > scanned) {
		do {
			if (m_resources[i] == 0) {
				ResBase** slot = m_resources + i;
				do {
					slot++;
					i++;
				} while (*slot == 0);
			}
			if (m_resources[i] == p_resource) {
				m_resources[i] = 0;
				break;
			}
			scanned++;
			i++;
		} while (scanned < count);
	}
	if (scanned != m_resourceCount) {
		m_resourceCount--;
	}
}

// 68K 0x1020299e DeallocateMem__7CMogResFPUcUc
// FUNCTION: LEMBALL 0x0045cf10
void MogRes::DeallocateMem(unsigned char* p_data, unsigned char p_owned)
{
	MogloadArena::operator delete(p_data);
}

// GLOBAL: LEMBALL 0x004a1d58
MogRes* g_pMogRes = 0;

// GLOBAL: LEMBALL 0x004a1d5c
MogRes* g_pActiveMogRes = 0;
