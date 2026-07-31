extern int g_nLevelFrameClockTick;
extern int g_fLevelEndStateRequestLatched;

typedef int(__fastcall* LevelManagedEntityNoArgPredicateProc)(void* pEntity);

struct LevelManagedEntityCommandQueueView;

struct LevelManagedEntityMoveCommandView {
	int IsFacingTowardFirstMoveCommand(void);
};

int __cdecl CallHasPendingManagedEntityCommands(void* pUnused, LevelManagedEntityCommandQueueView* pEntity);
typedef int(__cdecl* LevelManagedEntityThreeArgPredicateProc)(void* pUnused,
															  LevelManagedEntityCommandQueueView* pEntity,
															  void* pUnused2);

struct LevelManagedEntityTimedPredicateView {
	unsigned char m_abReserved00[0xcc];
	unsigned int m_nActionDeadlineCC;
};

struct LevelEndStatePredicateView {
	unsigned char m_abReserved00[0x108];
	int m_nEndState108;
};

struct LevelMoveDirtyPredicateView {
	unsigned char m_abReserved00[8];
	int m_fMoveDirty08;
};

struct LevelManagedEntityRuntimeValueView {
	unsigned char m_abReserved00[0x2c];
	int m_nRuntimeValue2C;
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

// FUNCTION: LEMBALL 0x00419bc0
int __cdecl IsLevelEndStateFourLatched(LevelEndStatePredicateView* pState)
{
	return g_fLevelEndStateRequestLatched != 0 && pState->m_nEndState108 == 4;
}

// FUNCTION: LEMBALL 0x00419be0
int __cdecl IsEntityMoveDirty(void* pUnused, LevelMoveDirtyPredicateView* pState)
{
	(void) pUnused;
	return pState->m_fMoveDirty08;
}

// FUNCTION: LEMBALL 0x00419bf0
int __cdecl GetManagedEntityRuntimeValue(void* pUnused, LevelManagedEntityRuntimeValueView* pEntity)
{
	(void) pUnused;
	return pEntity->m_nRuntimeValue2C;
}

// FUNCTION: LEMBALL 0x00419c00
int __cdecl HasPendingCommandsNeedingFacingUpdate(void* pUnused,
												  LevelManagedEntityMoveCommandView* pEntity,
												  void* pUnused2)
{
	if (((LevelManagedEntityThreeArgPredicateProc) CallHasPendingManagedEntityCommands)(
			pUnused,
			(LevelManagedEntityCommandQueueView*) pEntity,
			pUnused2) == 0) {
		return 0;
	}
	return pEntity->IsFacingTowardFirstMoveCommand() == 0;
}
