#include "PasswordHiliteWindow.h"

// 68K 0x1080d510 __ct__21CPasswordHiliteWindowFv
// STUB: LEMBALL 0x00451fb0
PasswordHiliteWindow::PasswordHiliteWindow()
{
}

// 68K 0x1080d55c OnCreate__21CPasswordHiliteWindowFv
// FUNCTION: LEMBALL 0x00451fd0
void PasswordHiliteWindow::OnCreate()
{
	char* target = *(char**) (*(int*) ((char*) this + 0x4c) + 0x0c);
	*(unsigned int*) (*(int*) (*(int*) (target + 0x40) + 4) + 0x74 + (int) target) = 1;
}
