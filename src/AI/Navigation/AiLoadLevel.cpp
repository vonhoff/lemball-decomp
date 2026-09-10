// Four rows of four 32-bit entries, indexed by trapdoor count minus one.
// The x86 folds the -1 into its 0x0049cf50 displacement; the data starts at 0x0049cf60.
// GLOBAL: LEMBALL 0x0049cf60
int g_anDefaultTrapDoorLemmings[4][4] = {{4, 0, 0, 0}, {3, 1, 0, 0}, {2, 1, 1, 0}, {1, 1, 1, 1}};
