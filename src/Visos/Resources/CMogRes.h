#ifndef LEMBALL_VISOS_RESOURCES_CMOGRES_H
#define LEMBALL_VISOS_RESOURCES_CMOGRES_H

#include "../Foundation/Chunk.h"     // complete type
#include "../Foundation/ChunkInfo.h" // complete type
#include "CRawRead.h"

class CResBase;
class CMogDir;
class CBaseStat;
struct CVsRange;

#define kResourceHandleCount 0x400
#define kResourceHandleBytes 0x1000
#define kMogDirAllocSize 0x38

class CMogRes : public CRawRead {
public:
	CMogRes(char* p_path, unsigned long p_arenaSize);
	CResBase* Find(unsigned int p_resourceId);
	bool CheckAllUnloaded();
	bool Load(const CVsRange& p_range, unsigned char*& p_data, CResBase* p_resource);
	bool Load(unsigned int p_resourceId, CResBase* p_resource, unsigned int p_recurse);
	bool Load(CResBase* p_resource, Chunk p_chunk);
	bool SetWd(char* p_path);
	int GetFreeHandle();
	int KillLeastResource(unsigned int p_requiredSize);
	void DeallocateMem(unsigned char* p_data, unsigned char p_owned);
	unsigned char* AllocateMainMem(unsigned int p_size);
	void AgeResources();
	void CleanUpResources();
	void Remove(CResBase* p_resource);
	~CMogRes();

	friend class CResBase;

private:
	CMogDir* m_rootDirectory;     // 0x00
	CMogDir* m_workingDirectory;  // 0x04
	unsigned int m_error;         // 0x08
	char* m_workingPath;          // 0x0c
	CResBase** m_resources;       // 0x10
	unsigned int m_resourceCount; // 0x14
	unsigned int m_arenaSize;     // 0x18
	unsigned int m_skipCleanup;   // 0x1c
	unsigned int m_unk0x20;       // 0x20
	unsigned int m_externalArena; // 0x24
};

extern CMogRes* g_pMogRes;
extern CMogRes* g_pActiveMogRes;
extern CBaseStat* g_pMogloadStat;
extern char g_mogRootPath[4];

#endif
