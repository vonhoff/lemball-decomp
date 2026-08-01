#include "ENGINE/CORE/COMMON.H"

struct GameEventTypeView {
	unsigned short m_nType;
};

struct GameUnhandledEventCounterView {
	unsigned char m_abReserved00[0x0c];
	int m_cUnhandledEvents0C;

	int FilterEvent(const GameEventTypeView* pEvent);
};

// FUNCTION: LEMBALL 0x0044be80
int GameUnhandledEventCounterView::FilterEvent(const GameEventTypeView* pEvent)
{
	switch (pEvent->m_nType) {
	case 4:
		return 1;
	default:
		++m_cUnhandledEvents0C;
		return 0;
	}
}
