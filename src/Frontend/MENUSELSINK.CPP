#include "Frontend/GameMenuActionFramePointSink.h"
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

struct GameMenuActionFramePointSinkVtableData {
	void*(LEMBALL_FASTCALL* m_pDelete)(void*, int, unsigned int);
	void (GameMenuActionFramePointSink::*m_pBindToContext)(void*);
	void (GameMenuActionFramePointSink::*m_pDispatchToRenderObject)(void*);
};



extern void* g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage[8];
extern void* LEMBALL_FASTCALL DestroyBaseModeActionButtonPointSinkArray(void* pObject,
																		int nUnusedEdx,
																		unsigned int fDelete);

// GLOBAL: LEMBALL 0x00497730
static GameMenuActionFramePointSinkVtableData g_MenuActionFramePointSinkVtable = {
	&DestroyBaseModeActionButtonPointSinkArray,
	&GameMenuActionFramePointSink::BindToContext,
	&GameMenuActionFramePointSink::DispatchToRenderObject,
};

// Split from MENUSEL.CPP to preserve compiler state in the original translation unit.

// FUNCTION: LEMBALL 0x00443990
void* LEMBALL_FASTCALL InitializeMenuActionFramePointSink(void* pObject)
{
	GameMenuActionFramePointSink* pEntry;

	pEntry = (GameMenuActionFramePointSink*) pObject;
	pEntry->m_pVtable00 = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
	pEntry->m_nReserved06 = 0;
	pEntry->m_pVtable00 = (void**) &g_MenuActionFramePointSinkVtable;
	pEntry->m_nReserved04 = 0;
	return pEntry;
}
