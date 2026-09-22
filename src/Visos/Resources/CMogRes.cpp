#include "CMogRes.h"

#include <string.h>

extern "C" unsigned long __stdcall timeGetTime(void);

#pragma intrinsic(strcpy, strlen)

#include "../Animation/CStatManager.h"
#include "../Foundation/CArena.h"
#include "../Foundation/CVsRange.h"
#include "../Foundation/Chunk.h"
#include "../Foundation/ChunkInfo.h"
#include "../Foundation/VsFile.h"
#include "CMogloadArena.h"
#include "CRawRead.h"
#include "CResBase.h"
#include "MogLoad.h"
#include "Visos/Animation/MogloadStat.h"

#define kPathSeparator '/'

// GLOBAL: LEMBALL 0x004a1d80
char g_mogRootPath[4];

// GLOBAL: LEMBALL 0x004a1d60
CBaseStat* g_pMogloadStat = 0;

// FUNCTION: LEMBALL 0x0045c630
CMogRes::CMogRes(char* p_path, unsigned long p_arenaSize)
{
	int offset;
	CArena* arena;

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
	if (g_pMasterArena->AllocateArena(&arena, p_arenaSize, "Resource Data Arena") == 1) {
		m_externalArena = 0;
	}
	g_pMogloadArena = arena;
	if (!Open(p_path, "rb")) {
		m_error = 1;
		return;
	}
	timeGetTime();
	m_rootDirectory = new CMogDir(0);
	timeGetTime();
	m_workingDirectory = m_rootDirectory;
	SetWd(g_mogRootPath);
	m_resources = (CResBase**) CMogloadArena::operator new(kResourceHandleBytes);
	for (offset = 0; offset < 1024; offset++) {
		m_resources[offset] = 0;
	}
	g_pMogloadStat = new MogloadStat("Mogload memory");
	g_pStatManager->Register(g_pMogloadStat);
	g_pMogloadArena->m_parentArena = (CArena*) g_pMogloadStat;
}

// FUNCTION: LEMBALL 0x0045c770
CMogRes::~CMogRes()
{
	if (g_pMogFile != 0) {
		VsClose(g_pMogFile);
	}
	CheckAllUnloaded();
	if (m_skipCleanup == 0) {
		CleanUpResources();
	}
	if (m_resources != 0) {
		CMogloadArena::operator delete(m_resources);
		m_resources = 0;
	}
	if (m_rootDirectory != 0) {
		delete m_rootDirectory;
		m_rootDirectory = 0;
	}
	if (m_workingPath != 0) {
		CMogloadArena::operator delete(m_workingPath);
		m_workingPath = 0;
	}
	if (m_externalArena == 0) {
		g_pMasterArena->FreeArena(g_pMogloadArena);
	}
	g_pMogloadArena = 0;
}

// FUNCTION: LEMBALL 0x0045c810
bool CMogRes::SetWd(char* p_path)
{
	register char* path = p_path;
	register char* copy;
	register char* cursor;
	CMogDir* dir;

	if (*path == kPathSeparator) {
		m_workingDirectory = m_rootDirectory;
		copy = (char*) CMogloadArena::operator new(strlen(path) + 1);
		strcpy(copy, path);
	}
	else {
		copy = (char*) CMogloadArena::operator new(strlen(path) + 2);
		copy[0] = kPathSeparator;
		strcpy(copy + 1, path);
	}
	cursor = copy;
	{
		int* current = &m_workingDirectory->m_currentDirIndex;
		current[0] = m_workingDirectory->m_rootIndex;
		current[1] = (int) m_workingDirectory->m_rootChunk;
		*current = -1;
	}
	for (;;) {
		cursor = strchr(cursor, kPathSeparator);
		if (cursor == 0) {
			break;
		}
		cursor++;
		if (*cursor != '\0') {
			do {
				dir = m_workingDirectory->GetNextDir();
				if (dir == 0) {
					goto done;
				}
			} while (NameCmp((char*) m_workingDirectory->m_currentDirChunk->m_data, cursor) == 0);
			if (dir == 0) {
				break;
			}
			m_workingDirectory = dir;
		}
		if (cursor == 0) {
			break;
		}
	}
done:
	if (cursor == 0) {
		if (m_workingPath != 0) {
			CMogloadArena::operator delete(m_workingPath);
			m_workingPath = 0;
		}
		m_workingPath = copy;
		return 1;
	}
	SetWd(m_workingPath);
	if (copy != 0) {
		CMogloadArena::operator delete(copy);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0045c940
int CMogRes::KillLeastResource(unsigned int p_requiredSize)
{
	register int scanned = 0;
	register int i = 0;
	register unsigned int bestRefs = 0xffffffff;
	int bestIndex = -1;
	unsigned int bestSize = 0;

	if ((int) m_resourceCount > i) {
		do {
			if (m_resources[i] == 0) {
				CResBase** slot = &m_resources[i];
				do {
					slot++;
					i++;
				} while (*slot == 0);
			}
			CResBase* resource = m_resources[i];
			if (resource->m_loaded != 0) {
				if (resource->m_directUseCount == 0) {
					unsigned int used = resource->GetSizeUsed();
					unsigned int refs = m_resources[i]->m_referenceCount;
					if (used >= p_requiredSize) {
						if (used > bestSize || bestRefs > refs) {
							goto update;
						}
						if (used >= p_requiredSize) {
							goto next;
						}
					}
					if (bestRefs > refs) {
					update:
						bestRefs = refs;
						bestSize = used;
						bestIndex = i;
					}
				}
			}
		next:
			i++;
			scanned++;
		} while (scanned < (int) m_resourceCount);
	}
	return bestIndex;
}

// FUNCTION: LEMBALL 0x0045c9d0
int CMogRes::GetFreeHandle()
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

// FUNCTION: LEMBALL 0x0045ca30
unsigned char* CMogRes::AllocateMainMem(unsigned int p_size)
{
	register unsigned char* memory;
	register unsigned int size = p_size;

	do {
		memory = (unsigned char*) CMogloadArena::operator new(size);
		if (memory == 0) {
			int needed = size;
			needed -= g_pMogloadArena->GetFreeSize();
			if (needed < 0) {
				needed = size;
			}
			int handle = KillLeastResource(needed);
			if (handle != -1) {
				m_resources[handle]->UnLoadData(1);
				if (handle != -1) {
					continue;
				}
			}
			int i = 0;
			unsigned int remaining = m_resourceCount;
			if ((int) remaining > 0) {
				do {
					if (m_resources[i] == 0) {
						do {
							i++;
						} while (m_resources[i] == 0);
					}
					i++;
					remaining--;
				} while (remaining != 0);
			}
		}
	} while (memory == 0);
	return memory;
}

// FUNCTION: LEMBALL 0x0045cab0
CResBase* CMogRes::Find(unsigned int p_resourceId)
{
	register int i = 0;
	register int count = m_resourceCount;
	register int remaining = count;

	if (count > i) {
		do {
			if (m_resources[i] == 0) {
				CResBase** slot = &m_resources[i];
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
		} while (remaining > 0);
	}
	if (count != 0 && remaining > 0) {
		AgeResources();
		m_resources[i]->LoadData();
		m_resources[i]->m_referenceCount++;
		return m_resources[i];
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0045cb50
bool CMogRes::Load(CResBase* p_resource, Chunk p_chunk)
{
	if (p_chunk.m_info->m_type != p_resource->m_chunkType) {
		return 0;
	}
	p_resource->m_dataSize = p_chunk.m_info->m_size;
	p_resource->m_fileOffset = p_chunk.m_info->m_fileOffset;
	p_resource->m_name = p_chunk.m_info->m_name;
	return 1;
}

// FUNCTION: LEMBALL 0x0045cb80
bool CMogRes::Load(unsigned int p_resourceId, CResBase* p_resource, unsigned int p_recurse)
{
	Chunk chunk;
	int handle;

	m_workingDirectory->Find(chunk, p_resourceId, p_recurse);
	if (chunk.m_info != 0) {
		handle = GetFreeHandle();
		CResBase* res = m_resources[handle];
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

// FUNCTION: LEMBALL 0x0045cd60
bool CMogRes::CheckAllUnloaded()
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

// FUNCTION: LEMBALL 0x0045cdb0
void CMogRes::AgeResources()
{
	int i = 0;
	int zero = 0;
	int scanned = 0;

	if ((int) m_resourceCount > zero) {
		do {
			if (m_resources[i] == 0) {
				do {
					i++;
				} while (m_resources[i] == 0);
			}
			if (m_resources[i]->m_loaded != 0 || m_resources[i]->GetfVramLoaded()) {
				m_resources[i]->m_age++;
			}
			scanned++;
			i++;
		} while ((int) m_resourceCount > scanned);
	}
}

// FUNCTION: LEMBALL 0x0045ce00
bool CMogRes::Load(const CVsRange& p_range, unsigned char*& p_data, CResBase* p_resource)
{
	p_data = AllocateMainMem(p_range.m_size);
	VsSeek(g_pMogFile, p_range.m_offset + 8, 0);
	VsRead(g_pMogFile, p_data, p_range.m_size);
	return 1;
}

// FUNCTION: LEMBALL 0x0045ce50
void CMogRes::CleanUpResources()
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

// FUNCTION: LEMBALL 0x0045ceb0
void CMogRes::Remove(CResBase* p_resource)
{
	unsigned int count = m_resourceCount;
	unsigned int scanned = 0;
	unsigned int i = 0;

	if (count > scanned) {
		do {
			if (m_resources[i] == 0) {
				do {
					i++;
				} while (m_resources[i] == 0);
			}
			if (m_resources[i] == p_resource) {
				m_resources[i] = 0;
				break;
			}
			scanned++;
			i++;
		} while (scanned < count);
	}
	unsigned int total = m_resourceCount;
	if (scanned != total) {
		total--;
		m_resourceCount = total;
	}
}

// FUNCTION: LEMBALL 0x0045cf10
void CMogRes::DeallocateMem(unsigned char* p_data, unsigned char p_owned)
{
	CMogloadArena::operator delete(p_data);
}

// GLOBAL: LEMBALL 0x004a1d58
CMogRes* g_pMogRes = 0;

// GLOBAL: LEMBALL 0x004a1d5c
CMogRes* g_pActiveMogRes = 0;
