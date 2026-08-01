struct LevelVtSmallFunctionView {
	void AddLevelScoreClamped(int nValue);
};

extern void* g_pActiveManagedEntityOwner;

// FUNCTION: LEMBALL 0x00422c40
int AwardCollScore1000(void)
{
	((LevelVtSmallFunctionView*) g_pActiveManagedEntityOwner)->AddLevelScoreClamped(1000);
	return 1;
}
