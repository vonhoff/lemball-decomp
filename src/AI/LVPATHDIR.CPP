#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

// MACINTOSH: Direction(int, int, int, int)
// FUNCTION: LEMBALL 0x00423890
int ComputeReachabilityStepDirectionCode(int x, int y, int nNextX, int nNextY)
{
	return (y - nNextY) * 3 - nNextX + x + 4;
}
