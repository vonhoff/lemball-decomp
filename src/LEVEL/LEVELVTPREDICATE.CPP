extern int g_nLevelFrameClockTick;

typedef int(__fastcall* LevelManagedEntityNoArgPredicateProc)(void* pEntity);

struct LevelManagedEntityTimedPredicateView {
	unsigned char m_abReserved00[0xcc];
	unsigned int m_nActionDeadlineCC;
};

// FUNCTION: LEMBALL 0x00419ab0
int __cdecl IsManagedEntityVslot74False(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	return ((LevelManagedEntityNoArgPredicateProc) pVtable[0x74 / sizeof(void*)])(pEntity) == 0;
}

// FUNCTION: LEMBALL 0x00419ad0
int __cdecl IsManagedEntityVslot7CTrueAndVslot98FalseA(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	return ((LevelManagedEntityNoArgPredicateProc) pVtable[0x7c / sizeof(void*)])(pEntity) != 0 &&
		   ((LevelManagedEntityNoArgPredicateProc) pVtable[0x98 / sizeof(void*)])(pEntity) == 0;
}

// FUNCTION: LEMBALL 0x00419b10
int __cdecl IsManagedEntityHookActiveBeforeTimer(void* pUnused, LevelManagedEntityTimedPredicateView* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	return ((LevelManagedEntityNoArgPredicateProc) pVtable[0x7c / sizeof(void*)])(pEntity) != 0 &&
		   (unsigned int) g_nLevelFrameClockTick < pEntity->m_nActionDeadlineCC;
}

// FUNCTION: LEMBALL 0x00419b40
int __cdecl ForwardManagedEntityVslot80(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	return ((LevelManagedEntityNoArgPredicateProc) pVtable[0x80 / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419b50
int __cdecl IsManagedEntityVslot7CTrueAndVslot98FalseB(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	return ((LevelManagedEntityNoArgPredicateProc) pVtable[0x7c / sizeof(void*)])(pEntity) != 0 &&
		   ((LevelManagedEntityNoArgPredicateProc) pVtable[0x98 / sizeof(void*)])(pEntity) == 0;
}

// FUNCTION: LEMBALL 0x00419b90
int __cdecl IsManagedEntityActionActiveBeforeDelay(void* pUnused, LevelManagedEntityTimedPredicateView* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	return ((LevelManagedEntityNoArgPredicateProc) pVtable[0x7c / sizeof(void*)])(pEntity) != 0 &&
		   (unsigned int) g_nLevelFrameClockTick < pEntity->m_nActionDeadlineCC;
}
