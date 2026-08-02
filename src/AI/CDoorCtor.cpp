#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/WIN32.H"
#include "LEVEL/CHUNKOBJVT.H"

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nVariant, unsigned short nStateId);
};

struct CDoor : LevelChunkObjectBaseView {
	CDoor(void);
	void DoActivate(void);
};

extern void* g_LEVELVT_DoorChunkObjectVtable[16];

// Split from LEVELVT.CPP to preserve compiler state in the original translation unit.

void* LEMBALL_FASTCALL DeleteDoorChunkObjectArrayVtableThunk(void* pObject, void* pUnused, BYTE fDelete)
{
	return DeleteDoorChunkObjectAuto(pObject, pUnused, fDelete);
}

// LINKERILT: LEMBALL 0x004011c2
void LEMBALL_FASTCALL DoActivateCDoorThunk(void* pObject)
{
	((CDoor*) pObject)->DoActivate();
}

// FUNCTION: LEMBALL 0x0040d470
CDoor::CDoor(void)
{
	InitializeLevelChunkObjectBase(0x19, 0, 0);
	*(void**) this = g_LEVELVT_DoorChunkObjectVtable;
}
