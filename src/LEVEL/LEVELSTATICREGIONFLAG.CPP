// GLOBAL: LEMBALL 0x0049ee28
int g_fLevelScreenStaticRegionPending = 1;

// FUNCTION: LEMBALL 0x0043b4b0
int ClearLevelScreenStaticRegionPending(void)
{
	if (g_fLevelScreenStaticRegionPending != 0) {
		g_fLevelScreenStaticRegionPending = 0;
	}
	return 0;
}
