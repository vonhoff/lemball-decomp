#include "HiliteWindow.h"

// 68K 0x10804fb6 __ct__13CHiliteWindowFv
// STUB: LEMBALL 0x0044f400
HiliteWindow::HiliteWindow()
{
}

// 68K 0x10804ff8 OnCreate__13CHiliteWindowFv
// FUNCTION: LEMBALL 0x0044f420
void HiliteWindow::OnCreate()
{
	char* target = *(char**) (*(int*) ((char*) this + 0x4c) + 0x0c);
	*(unsigned int*) (*(int*) (*(int*) (target + 0x40) + 4) + 0x74 + (int) target) = 1;
}

// GLOBAL: LEMBALL 0x004a7b64
unsigned int g_dwHiliteAnimationId;
