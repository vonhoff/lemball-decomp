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
	unsigned int offset;
	Arena* arena;
	MogDir* rootDir;

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
	rootDir = (MogDir*) MogloadArena::operator new(kMogDirAllocSize);
	if (rootDir == 0) {
		m_rootDirectory = 0;
	}
	else {
		m_rootDirectory = new (rootDir) MogDir(0);
	}
	CurrentMilliTimer();
	m_workingDirectory = m_rootDirectory;
	SetWd(g_mogRootPath);
	m_resources = (ResBase**) MogloadArena::operator new(kResourceHandleBytes);
	offset = 0;
	do {
		offset += sizeof(ResBase*);
		*(ResBase**) ((unsigned char*) m_resources + offset - sizeof(ResBase*)) = 0;
	} while ((int) offset < kResourceHandleBytes);
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
		m_rootDirectory->~MogDir();
		MogloadArena::operator delete(m_rootDirectory);
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
	MogDir* dir;
	char* cursor;
	int* current;

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
	current = &m_workingDirectory->m_currentDirIndex;
	current[0] = m_workingDirectory->m_rootIndex;
	current[1] = (int) m_workingDirectory->m_rootChunk;
	*current = -1;
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
			while (m_resources[i] == 0) {
				i++;
			}
			if (m_resources[i]->m_loaded != 0 && m_resources[i]->m_directUseCount == 0) {
				unsigned int used = m_resources[i]->GetSizeUsed();
				unsigned int refs = m_resources[i]->m_referenceCount;
				if (used >= p_requiredSize) {
					if (used > bestSize || bestRefs > refs) {
						bestRefs = refs;
						bestSize = used;
						bestIndex = i;
					}
				}
				else if (bestRefs > refs) {
					bestRefs = refs;
					bestSize = used;
					bestIndex = i;
				}
			}
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
	unsigned char* memory;
	unsigned int needed;
	int index;

	while ((memory = (unsigned char*) MogloadArena::operator new(p_size)) == 0) {
		needed = p_size - g_pMogloadArena->GetFreeSize();
		if ((int) needed < 0) {
			needed = p_size;
		}
		index = KillLeastResource(needed);
		if (index == -1) {
			break;
		}
		m_resources[index]->UnLoadData(1);
		if (index == -1) {
			break;
		}
	}
	return memory;
}

// 68K 0x1020253c Find__7CMogResFUl
// FUNCTION: LEMBALL 0x0045cab0
ResBase* MogRes::Find(unsigned int p_resourceId)
{
	int i = 0;
	int remaining = m_resourceCount;

	if ((int) m_resourceCount > 0) {
		do {
			while (m_resources[i] == 0) {
				i++;
			}
			if (m_resources[i]->m_resourceId == p_resourceId) {
				break;
			}
			remaining--;
			i++;
		} while (remaining > 0);
	}
	if (m_resourceCount != 0 && remaining > 0) {
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
	if (p_resource->m_chunkType == p_chunk.m_info->m_type) {
		p_resource->m_dataSize = p_chunk.m_info->m_size;
		p_resource->m_fileOffset = p_chunk.m_info->m_fileOffset;
		p_resource->m_name = p_chunk.m_info->m_name;
		return 1;
	}
	return 0;
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
		if (m_resources[handle] != 0) {
			delete m_resources[handle];
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
	int i = 0;
	int loaded = 0;
	unsigned int remaining = m_resourceCount;

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
	int i = 0;
	int scanned = 0;

	if ((int) m_resourceCount > 0) {
		do {
			while (m_resources[i] == 0) {
				i++;
			}
			if (m_resources[i]->m_loaded == 0) {
				if (!m_resources[i]->GetfVramLoaded()) {
					goto next_age;
				}
			}
			m_resources[i]->m_age++;
		next_age:
			scanned++;
			i++;
		} while (scanned < (int) m_resourceCount);
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
	unsigned int scanned = 0;
	int i = 0;

	if ((int) m_resourceCount > 0) {
		do {
			while (m_resources[i] == 0) {
				i++;
			}
			if (m_resources[i] == p_resource) {
				m_resources[i] = 0;
				break;
			}
			scanned++;
			i++;
		} while ((int) scanned < (int) m_resourceCount);
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
