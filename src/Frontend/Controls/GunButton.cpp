#include "GunButton.h"

// 68K 0x10802938 sgn__Fi
// FUNCTION: LEMBALL 0x0044c1e0
int Sgn(int p_value)
{
	int res = p_value;
	if (res == 0) {
		return res;
	}
	if (res < 0) {
		return -1;
	}
	return 1;
}

// 68K 0x1080295e OnReleased__10CGunButtonF12BUTTON_FLAGS
// STUB: LEMBALL 0x0044c200
void GunButton::OnReleased(int p_flags)
{
}

// 68K 0x108029bc OnPressed__10CGunButtonF12BUTTON_FLAGS
// STUB: LEMBALL 0x0044c230
void GunButton::OnPressed(int p_flags)
{
}

// 68K 0x1011c1a2 __dt__10CGunButtonFv
GunButton::~GunButton()
{
}

