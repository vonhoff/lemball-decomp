typedef int(__fastcall* LevelManagedEntityNoArgPredicateProc)(void* pEntity);

// FUNCTION: LEMBALL 0x00419b50
int __cdecl IsManagedEntityVslot7CTrueAndVslot98False(void* pUnused, void* pEntity)
{
	void* pTarget;
	void** pVtable;

	(void) pUnused;
	pTarget = pEntity;
	pVtable = *(void***) pTarget;
	return ((LevelManagedEntityNoArgPredicateProc) pVtable[0x7c / sizeof(void*)])(pTarget) != 0 &&
		   ((LevelManagedEntityNoArgPredicateProc) pVtable[0x98 / sizeof(void*)])(pTarget) == 0;
}
