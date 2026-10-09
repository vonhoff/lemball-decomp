#include "CMogRes.h"

#include <string.h>

#pragma intrinsic(strcpy, strlen)

enum {
	RESOURCE_INDEX_NONE = -1
};

#include "CMogDir.h"
#include "CMogloadArena.h"
#include "CRawRead.h"
#include "CVSRange.h"
#include "tagCHUNK.h"
#include "tagChunkInfo.h"
#include "Engine/Files/VsFile.h"
#include "Engine/Memory/CArena.h"
#include "Engine/Resources/Archive/CMogloadStat.h"
#include "Engine/Resources/Types/CResBase.h"
#include "Engine/Statistics/CStatManager.h"
#include "Engine/Time/VsTime.h"

#define RESOURCE_PATH_SEPARATOR '/'

// GLOBAL: LEMBALL 0x004a1d80
char g_mogRootPath[4] = " ";

// GLOBAL: LEMBALL 0x004a1d60
CBaseStat* g_pMogloadStat = NULL;

// FUNCTION: LEMBALL 0x0045c630
CMogRes::CMogRes(char* p_path, unsigned long p_arenaSize)
{
	int offset;
	CArena* arena;

	g_mogRootPath[0] = RESOURCE_PATH_SEPARATOR;
	g_pActiveMogRes = this;
	m_error = 0;
	m_resources = NULL;
	m_workingPath = NULL;
	m_rootDirectory = NULL;
	m_workingDirectory = NULL;
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
	CurrentMilliTimer();
	m_rootDirectory = new CMogDir(0);
	CurrentMilliTimer();
	m_workingDirectory = m_rootDirectory;
	SetWD(g_mogRootPath);
	m_resources = (CResBase**) CMogloadArena::operator new(RESOURCE_HANDLE_COUNT * sizeof(*m_resources));
	for (offset = 0; offset < RESOURCE_HANDLE_COUNT; offset++) {
		m_resources[offset] = NULL;
	}
	g_pMogloadStat = new CMogloadStat("Mogload memory");
	g_pStatManager->Register(g_pMogloadStat);
	g_pMogloadArena->m_usageStat = g_pMogloadStat;
}

// FUNCTION: LEMBALL 0x0045c770
CMogRes::~CMogRes()
{
	if (g_pMogFile != NULL) {
		vsClose(g_pMogFile);
	}
	CheckAllUnloaded();
	if (m_skipCleanup == 0) {
		CleanUpResources();
	}
	if (m_resources != NULL) {
		CMogloadArena::operator delete(m_resources);
		m_resources = NULL;
	}
	if (m_rootDirectory != NULL) {
		delete m_rootDirectory;
		m_rootDirectory = NULL;
	}
	if (m_workingPath != NULL) {
		CMogloadArena::operator delete(m_workingPath);
		m_workingPath = NULL;
	}
	if (m_externalArena == 0) {
		g_pMasterArena->FreeArena(g_pMogloadArena);
	}
	g_pMogloadArena = NULL;
}

// FUNCTION: LEMBALL 0x0045c810
bool CMogRes::SetWD(char* p_path)
{
	register char* path = p_path;
	register char* copy;
	register char* cursor;
	CMogDir* dir;
	char* oldPath;

	if (*path == RESOURCE_PATH_SEPARATOR) {
		m_workingDirectory = m_rootDirectory;
		copy = (char*) CMogloadArena::operator new(strlen(path) + 1);
		strcpy(copy, path);
	}
	else {
		copy = (char*) CMogloadArena::operator new(strlen(path) + 2);
		copy[0] = RESOURCE_PATH_SEPARATOR;
		strcpy(copy + 1, path);
	}
	cursor = copy;
	{
		tagCHUNK* current = &m_workingDirectory->m_currentDir;
		*current = m_workingDirectory->m_root;
		current->m_index = CHUNK_INDEX_BEFORE_FIRST_ENTRY;
	}
	for (;;) {
		cursor = strchr(cursor, RESOURCE_PATH_SEPARATOR);
		if (cursor == NULL) {
			break;
		}
		cursor++;
		if (*cursor != '\0') {
			do {
				dir = m_workingDirectory->GetNextDir();
				if (dir == NULL) {
					goto done;
				}
			} while (NameCmp((char*) m_workingDirectory->m_currentDir.m_info->m_data, cursor) == 0);
			if (dir == NULL) {
				break;
			}
			m_workingDirectory = dir;
		}
		if (cursor == NULL) {
			break;
		}
	}
done:
	oldPath = m_workingPath;
	if (cursor == NULL) {
		if (oldPath != NULL) {
			CMogloadArena::operator delete(oldPath);
			m_workingPath = NULL;
		}
		m_workingPath = copy;
		return true;
	}
	SetWD(oldPath);
	if (copy != NULL) {
		CMogloadArena::operator delete(copy);
	}
	return false;
}

// FUNCTION: LEMBALL 0x0045c940
int CMogRes::KillLeastResource(unsigned long p_requiredSize)
{
	enum eResourceEvictionInitialBound {
		RESOURCE_REFERENCE_COUNT_INITIAL_UPPER_BOUND = 0xffffffffUL
	};
	register int scanned = 0;
	register int i = 0;
	register unsigned int bestRefs = RESOURCE_REFERENCE_COUNT_INITIAL_UPPER_BOUND;
	int bestIndex = RESOURCE_INDEX_NONE;
	unsigned int bestSize = 0;

	if (m_resourceCount > i) {
		do {
			if (m_resources[i] == NULL) {
				CResBase** slot = &m_resources[i];
				do {
					slot++;
					i++;
				} while (*slot == NULL);
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
		} while (scanned < m_resourceCount);
	}
	return bestIndex;
}

// FUNCTION: LEMBALL 0x0045c9d0
int CMogRes::GetFreeHandle()
{
	int i = 0;
	int handle = RESOURCE_INDEX_NONE;

	if (m_resourceCount > 0) {
		if (m_resourceCount < RESOURCE_HANDLE_COUNT) {
			while (i < RESOURCE_HANDLE_COUNT && m_resources[i] != NULL) {
				i++;
			}
		}
		else {
			while (i < RESOURCE_HANDLE_COUNT && m_resources[i]->m_referenceCount != 0) {
				i++;
			}
		}
		if (i < RESOURCE_HANDLE_COUNT) {
			handle = i;
		}
		return handle;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0045ca30
unsigned char* CMogRes::AllocateMainMem(unsigned long p_size)
{
	register unsigned char* memory;
	register unsigned int size = p_size;

	do {
		memory = (unsigned char*) CMogloadArena::operator new(size);
		if (memory == NULL) {
			int needed = size;
			needed -= g_pMogloadArena->GetFreeSize();
			if (needed < 0) {
				needed = size;
			}
			int handle = KillLeastResource(needed);
			if (handle != RESOURCE_INDEX_NONE) {
				m_resources[handle]->UnLoadData(1);
				if (handle != RESOURCE_INDEX_NONE) {
					continue;
				}
			}
			int i = 0;
			int remaining = m_resourceCount;
			if (remaining > 0) {
				do {
					if (m_resources[i] == NULL) {
						do {
							i++;
						} while (m_resources[i] == NULL);
					}
					i++;
					remaining--;
				} while (remaining != 0);
			}
		}
	} while (memory == NULL);
	return memory;
}

// FUNCTION: LEMBALL 0x0045cab0
CResBase* CMogRes::Find(unsigned long p_resourceId)
{
	register int i = 0;
	register int count = m_resourceCount;
	register int remaining = count;

	if (count > i) {
		unsigned int resourceId = p_resourceId;
		do {
			if (m_resources[i] == NULL) {
				CResBase** slot = &m_resources[i];
				do {
					slot++;
					i++;
				} while (*slot == NULL);
			}
			if (m_resources[i]->m_resourceId == resourceId) {
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
	return NULL;
}

// FUNCTION: LEMBALL 0x0045cb50
bool CMogRes::Load(CResBase* p_resource, tagCHUNK p_chunk)
{
	if (p_chunk.m_info->m_type != p_resource->m_chunkType) {
		return false;
	}
	p_resource->m_dataSize = p_chunk.m_info->m_size;
	p_resource->m_fileOffset = p_chunk.m_info->m_fileOffset;
	p_resource->m_name = p_chunk.m_info->m_name;
	return true;
}

// FUNCTION: LEMBALL 0x0045cb80
bool CMogRes::Load(unsigned int p_resourceId, CResBase* p_resource, RECURSE p_recurse)
{
	tagCHUNK chunk;
	int handle;

	m_workingDirectory->Find(chunk, p_resourceId, p_recurse);
	if (chunk.m_info != NULL) {
		handle = GetFreeHandle();
		CResBase* res = m_resources[handle];
		if (res != NULL) {
			delete res;
			m_resources[handle] = NULL;
			m_resourceCount--;
		}
		m_resources[handle] = p_resource;
		m_resourceCount++;
		return Load(p_resource, chunk);
	}
	return false;
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
			while (m_resources[i] == NULL && i < RESOURCE_HANDLE_COUNT) {
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

	if (m_resourceCount > zero) {
		do {
			if (m_resources[i] == NULL) {
				do {
					i++;
				} while (m_resources[i] == NULL);
			}
			if (m_resources[i]->m_loaded != 0 || m_resources[i]->GetfVramLoaded()) {
				m_resources[i]->m_age++;
			}
			scanned++;
			i++;
		} while (m_resourceCount > scanned);
	}
}

// FUNCTION: LEMBALL 0x0045ce00
bool CMogRes::Load(const CVSRange& p_range, unsigned char*& p_data, CResBase* p_resource)
{
	p_data = AllocateMainMem(p_range.m_size);
	vsSeek(g_pMogFile, p_range.m_offset + 8, 0);
	vsRead(g_pMogFile, p_data, p_range.m_size);
	return true;
}

// FUNCTION: LEMBALL 0x0045ce50
void CMogRes::CleanUpResources()
{
	unsigned int count = m_resourceCount;
	int i = 0;
	int scanned = 0;

	if ((int) count > 0) {
		do {
			while (m_resources[i] == NULL) {
				i++;
			}
			if (i == RESOURCE_HANDLE_COUNT) {
				return;
			}
			if (m_resources[i]->m_referenceCount == 0) {
				if (m_resources[i] != NULL) {
					delete m_resources[i];
				}
				m_resources[i] = NULL;
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
	int scanned = 0;
	int i = 0;

	if (m_resourceCount > scanned) {
		do {
			if (m_resources[i] == NULL) {
				do {
					i++;
				} while (m_resources[i] == NULL);
			}
			if (m_resources[i] == p_resource) {
				m_resources[i] = NULL;
				break;
			}
			scanned++;
			i++;
		} while (scanned < m_resourceCount);
	}
	int total = m_resourceCount;
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
CMogRes* g_pMogRes = NULL;

// GLOBAL: LEMBALL 0x004a1d5c
CMogRes* g_pActiveMogRes = NULL;
