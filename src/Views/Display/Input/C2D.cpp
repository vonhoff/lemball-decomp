#include "../C2D.h"

#include "../../../Control/Game/CDemo.h"
#include "../CMain2DDisplay.h"

// FUNCTION: LEMBALL 0x00437b60
int C2D::ProcessMsg(Message* p_message)
{
	if ((g_pDemo == 0 || g_pDemo->m_demoMode == 0) && !m_display->IsFocusWindow()) {
		return 0;
	}
	if (m_paused != 0) {
		return 0;
	}

	switch ((unsigned int) p_message->type) {
	case 4:
	case 0xc:
		switch (p_message->code) {
		case 3:
			PrevGroup();
			return 1;
		case 4:
			NextGroup();
			return 1;
		case 0x39:
			UseBalloon(3);
			return 1;
		case 0x3a:
			SelectLemming(0);
			return 1;
		case 0x3b:
			SelectLemming(1);
			return 1;
		case 0x3c:
			SelectLemming(2);
			return 1;
		case 0x3d:
			SelectLemming(3);
			return 1;
		case 0x40:
			UseBalloon(0);
			return 1;
		case 0x41:
			UseBalloon(1);
			return 1;
		case 0x42:
			UseBalloon(2);
			return 1;
		}
		return 0;
	default:
		m_processedCount++;
		return 0;
	}
}
