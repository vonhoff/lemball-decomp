#ifndef LEMBALL_CAICURSOR_H
#define LEMBALL_CAICURSOR_H

struct tagMESSAGE;

class CAICursor {
public:
	int m_vtable00;                    // vtable pointer (slot 0 = vtbl)
	char m_abReserved04[0x8];
	int m_nUnhandledCount0C;
	int m_xPos10;
	int m_yPos14;
	int m_maxX18;
	int m_maxY1C;

	int ProcessMsg(tagMESSAGE* pMsg);
};

#endif // LEMBALL_CAICURSOR_H
