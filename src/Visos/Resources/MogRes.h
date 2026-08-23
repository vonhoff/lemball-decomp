#ifndef LEMBALL_VISOS_RESOURCES_MOGRES_H
#define LEMBALL_VISOS_RESOURCES_MOGRES_H

#include "../../Common.h"
#include "../Foundation/Chunk.h"     // complete type
#include "../Foundation/ChunkInfo.h" // complete type
#include "RawRead.h"

#define kResourceHandleCount 0x400
#define kResourceHandleBytes 0x1000
#define kMogDirAllocSize 0x38

class MogRes : public RawRead {
public:
	MogRes(char* p_path, unsigned long p_arenaSize);
	ResBase* Find(unsigned int p_resourceId);
	bool CheckAllUnloaded();
	bool Load(const VsRange& p_range, unsigned char*& p_data, ResBase* p_resource);
	bool Load(unsigned int p_resourceId, ResBase* p_resource, unsigned int p_recurse);
	bool Load(ResBase* p_resource, Chunk p_chunk);
	bool SetWd(char* p_path);
	int GetFreeHandle();
	int KillLeastResource(unsigned int p_requiredSize);
	void DeallocateMem(unsigned char* p_data, unsigned char p_owned);
	unsigned char* AllocateMainMem(unsigned int p_size);
	void AgeResources();
	void CleanUpResources();
	void Remove(ResBase* p_resource);
	~MogRes();

	friend class ResBase;

private:
	MogDir* m_rootDirectory;      // 0x00
	MogDir* m_workingDirectory;   // 0x04
	unsigned int m_error;         // 0x08
	char* m_workingPath;          // 0x0c
	ResBase** m_resources;        // 0x10
	unsigned int m_resourceCount; // 0x14
	unsigned int m_arenaSize;     // 0x18
	unsigned int m_skipCleanup;   // 0x1c
	undefined4 m_unk0x20;         // 0x20
	unsigned int m_externalArena; // 0x24
};

extern MogRes* g_pMogRes;
extern MogRes* g_pActiveMogRes;
extern BaseStat* g_pMogloadStat;
extern char g_mogRootPath[4];

#endif
