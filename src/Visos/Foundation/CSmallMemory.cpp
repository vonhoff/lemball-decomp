#include "CSmallMemory.h"

#include "../../Control/Support/PreInit.h"
#include "CBucket.h"

#include <string.h>

#pragma intrinsic(memset)

// GLOBAL: LEMBALL 0x004a1174
char* g_pCurrentAllocDescription = "new";

// GLOBAL: LEMBALL 0x004a1178
unsigned int g_maxSmallMemorySize = 0;

// FUNCTION: LEMBALL 0x00473180
CSmallMemory::CSmallMemory()
{
	int baseShift = 1 << (g_preInitActive.m_startBucket + 1);
	int limit = g_preInitActive.m_capabilityCount + g_preInitActive.m_startBucket;
	m_bucketLimit = limit;
	if (7 < limit) {
		m_bucketLimit = 7;
	}
	memset(m_buckets, 0, sizeof(m_buckets));
	for (int j = g_preInitActive.m_startBucket; j < (int) m_bucketLimit; j++) {
		m_sizeLimits[j] = 1 << baseShift;
		int smallMemEnabled = g_nSmallMemoryEnabled;
		g_nSmallMemoryEnabled = 0;
		if (g_preInitActive.m_capabilities[j] != 0) {
			m_buckets[j] = new CBucket(m_sizeLimits[j], g_preInitActive.m_capabilities[j], 0, 0);
			if ((unsigned int) m_sizeLimits[j] > g_maxSmallMemorySize) {
				g_maxSmallMemorySize = m_sizeLimits[j];
			}
		}
		g_nSmallMemoryEnabled = smallMemEnabled;
		baseShift++;
	}
}

// FUNCTION: LEMBALL 0x00473280
CSmallMemory::~CSmallMemory()
{
	g_nSmallMemoryEnabled = 0;
	int i = g_preInitActive.m_startBucket;
	if (i < (int) m_bucketLimit) {
		CBucket** slot = &m_buckets[i];
		CBucket* bucket;
		do {
			bucket = *slot;
			if (bucket != 0) {
				delete bucket;
			}
			*slot++ = 0;
			i++;
		} while (i < (int) m_bucketLimit);
	}
}

// FUNCTION: LEMBALL 0x004732d0
unsigned char* CSmallMemory::Allocate(int p_size, char* p_description)
{
	int prevLimit = 0;
	int i = g_preInitActive.m_startBucket;
	int bucketLimit = m_bucketLimit;
	int* sizeLimit;
	CBucket* emptyBucket = 0;
	if (i < bucketLimit) {
		sizeLimit = &m_sizeLimits[i];
		do {
			if (prevLimit < p_size && p_size <= *sizeLimit && m_buckets[i] != emptyBucket) {
				unsigned char* result;
				if (m_buckets[i] == emptyBucket || !m_buckets[i]->Allocate(&result)) {
					break;
				}
				return result;
			}
			prevLimit = *sizeLimit++;
			i++;
		} while (i < bucketLimit);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00473340
bool CSmallMemory::Free(unsigned char* p_memory)
{
	for (int i = g_preInitActive.m_startBucket; i < (int) m_bucketLimit; i++) {
		if (m_buckets[i] != 0 && m_buckets[i]->CheckValidPointer(p_memory)) {
			m_buckets[i]->Free(p_memory);
			return true;
		}
	}
	return false;
}

// GLOBAL: LEMBALL 0x004aa0fc
CSmallMemory* g_pSmallMemory;

// GLOBAL: LEMBALL 0x004a217c
int g_nSmallMemoryEnabled = 1;

// GLOBAL: LEMBALL 0x004a2180
void* g_pMasterArenaMemory = 0;
