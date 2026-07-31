#include "ENGINE/CORE/COMMON.H"

// FUNCTION: LEMBALL 0x00458180
int LEMBALL_FASTCALL IsDisplayModeWindowedRange(const int* pnDisplayMode)
{
	switch (*pnDisplayMode) {
	case 2:
	case 3:
	case 4:
	case 5:
		return 1;
	default:
		return 0;
	}
}
