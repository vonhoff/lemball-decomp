#include "SpriteWindow.h"

// 68K 0x108030c2 __ct__13CSpriteWindowFv
// STUB: LEMBALL 0x0044c830
SpriteWindow::SpriteWindow()
{
}

// 68K 0x10803104 OnCreate__13CSpriteWindowFv
// FUNCTION: LEMBALL 0x0044c850
void SpriteWindow::OnCreate()
{
	char* target = *(char**) (*(int*) ((char*) this + 0x4c) + 0x0c);
	*(unsigned int*) (*(int*) (*(int*) (target + 0x40) + 4) + 0x74 + (int) target) = 1;
}
