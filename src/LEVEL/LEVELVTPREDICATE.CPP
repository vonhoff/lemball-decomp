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

// FUNCTION: LEMBALL 0x00419b80
int __cdecl ForwardTargetManagedEntityVslot7C(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	return ((LevelManagedEntityNoArgPredicateProc) pVtable[0x7c / sizeof(void*)])(pEntity);
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

typedef void(__fastcall* LevelManagedEntityNoArgVirtualProc)(void* pEntity);

struct LevelManagedEntityDropStateView {
	unsigned char m_abReserved00[0x104];
	int m_fDropToSavedPointArmed104;
	int m_fDropToTileCenterArmed108;
};

// FUNCTION: LEMBALL 0x00419c40
int __cdecl IsManagedEntityDelayElapsed(void* pUnused, LevelManagedEntityTimedPredicateView* pEntity)
{
	(void) pUnused;
	return (unsigned int) g_nLevelFrameClockTick >= pEntity->m_nActionDeadlineCC;
}

// FUNCTION: LEMBALL 0x00419c60
int __cdecl IsManagedEntityDelayActive(void* pUnused, LevelManagedEntityTimedPredicateView* pEntity)
{
	(void) pUnused;
	return (unsigned int) g_nLevelFrameClockTick < pEntity->m_nActionDeadlineCC;
}

// FUNCTION: LEMBALL 0x00419c90
void __cdecl ForwardManagedEntityVslot5C(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x5c / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419ca0
int __cdecl IsDropToSavedPointArmed(void* pUnused, LevelManagedEntityDropStateView* pEntity)
{
	(void) pUnused;
	return pEntity->m_fDropToSavedPointArmed104;
}

// FUNCTION: LEMBALL 0x00419cb0
int __cdecl IsDropToTileCenterArmed(void* pUnused, LevelManagedEntityDropStateView* pEntity)
{
	(void) pUnused;
	return pEntity->m_fDropToTileCenterArmed108;
}

// FUNCTION: LEMBALL 0x00419cc0
void __cdecl ForwardManagedEntityVslotE0(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0xe0 / sizeof(void*)])(pEntity);
}

void __cdecl ForwardManagedEntityVslot30(void* pUnused, void* pEntity);
typedef void(__cdecl* LevelManagedEntityThreeArgVirtualProc)(void* pUnused, void* pEntity, void* pUnused2);

// FUNCTION: LEMBALL 0x00419cd0
void __cdecl DispatchManagedEntityVslot78ThenVslot30(void* pUnused, void* pEntity, void* pUnused2)
{
	void** pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x78 / sizeof(void*)])(pEntity);
	((LevelManagedEntityThreeArgVirtualProc) ForwardManagedEntityVslot30)(pUnused, pEntity, pUnused2);
}

// FUNCTION: LEMBALL 0x00419d00
void __cdecl DispatchManagedEntityVslot9CThenVslot30(void* pUnused, void* pEntity, void* pUnused2)
{
	void** pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x9c / sizeof(void*)])(pEntity);
	((LevelManagedEntityThreeArgVirtualProc) ForwardManagedEntityVslot30)(pUnused, pEntity, pUnused2);
}

// FUNCTION: LEMBALL 0x00419d50
void __cdecl DispatchManagedEntityVslot90ThenVslot30(void* pUnused, void* pEntity, void* pUnused2)
{
	void** pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x90 / sizeof(void*)])(pEntity);
	((LevelManagedEntityThreeArgVirtualProc) ForwardManagedEntityVslot30)(pUnused, pEntity, pUnused2);
}

// FUNCTION: LEMBALL 0x00419e00
void __cdecl ForwardTargetManagedEntityVslotF0(void* pUnused, void* pTargetEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pTargetEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0xf0 / sizeof(void*)])(pTargetEntity);
}

// FUNCTION: LEMBALL 0x00419e20
void __cdecl DispatchManagedEntityPreUpdateCallbacks(void* pUnused, void* pEntity, void* pUnused2)
{
	void** pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x6c / sizeof(void*)])(pEntity);
	((LevelManagedEntityThreeArgVirtualProc) ForwardManagedEntityVslot30)(pUnused, pEntity, pUnused2);
}
