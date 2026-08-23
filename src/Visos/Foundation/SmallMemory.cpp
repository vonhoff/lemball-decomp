#include "SmallMemory.h"

#include "../../Control/Support/PreInit.h"
#include "Bucket.h"

#include <string.h>

#pragma intrinsic(memset)

// GLOBAL: LEMBALL 0x004a1174
char* g_pCurrentAllocDescription = "new";

// GLOBAL: LEMBALL 0x004a1178
unsigned int g_maxSmallMemorySize = 0;

// 68K 0x10217b0c __ct__12CSmallMemoryFv
// FUNCTION: LEMBALL 0x00473180
SmallMemory::SmallMemory()
{
	int baseShift = 1 << (g_preInitActive.m_startBucket + 1);
	int limit = g_preInitActive.m_capabilityCount;
	m_bucketLimit = limit + g_preInitActive.m_startBucket;
	if (m_bucketLimit > 7) {
		m_bucketLimit = 7;
	}
	memset(m_buckets, 0, sizeof(m_buckets));
	for (int j = g_preInitActive.m_startBucket; j < (int) m_bucketLimit; j++) {
		m_sizeLimits[j] = 1 << baseShift;
		int smallMemEnabled = g_nSmallMemoryEnabled;
		g_nSmallMemoryEnabled = 0;
		if (g_preInitActive.m_capabilities[j] != 0) {
			m_buckets[j] = new Bucket(m_sizeLimits[j], g_preInitActive.m_capabilities[j], 0, 0);
			if (m_sizeLimits[j] > (int) g_maxSmallMemorySize) {
				g_maxSmallMemorySize = m_sizeLimits[j];
			}
		}
		g_nSmallMemoryEnabled = smallMemEnabled;
		baseShift++;
	}
}

// 68K 0x10217bf4 __dt__12CSmallMemoryFv
// FUNCTION: LEMBALL 0x00473280
SmallMemory::~SmallMemory()
{
	g_nSmallMemoryEnabled = 0;
	for (int i = g_preInitActive.m_startBucket; i < (int) m_bucketLimit; i++) {
		register Bucket* bucket = m_buckets[i];
		if (bucket != 0) {
			delete bucket;
		}
		m_buckets[i] = 0;
	}
}

// 68K 0x10217c66 Allocate__12CSmallMemoryFiPc
// FUNCTION: LEMBALL 0x004732d0
unsigned char* SmallMemory::Allocate(int p_size, char* p_description)
{
	register int prevLimit = 0;
	for (int i = g_preInitActive.m_startBucket; i < (int) m_bucketLimit; i++) {
		if (prevLimit < p_size && p_size <= m_sizeLimits[i] && m_buckets[i] != 0) {
			unsigned char* result;
			if (m_buckets[i] == 0 || !m_buckets[i]->Allocate(&result)) {
				return 0;
			}
			return result;
		}
		prevLimit = m_sizeLimits[i];
	}
	return 0;
}

// 68K 0x10217ce4 Free__12CSmallMemoryFPUc
// FUNCTION: LEMBALL 0x00473340
bool SmallMemory::Free(unsigned char* p_memory)
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
SmallMemory* g_pSmallMemory;

// GLOBAL: LEMBALL 0x004a217c
int g_nSmallMemoryEnabled = 1;

// GLOBAL: LEMBALL 0x004a2180
void* g_pMasterArenaMemory = 0;
