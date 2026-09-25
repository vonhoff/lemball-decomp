#ifndef LEMBALL_VISOS_TARGET_INPUT_CMASTERINPUTITEM_H
#define LEMBALL_VISOS_TARGET_INPUT_CMASTERINPUTITEM_H

class CBaseQueue;
class CVSOStream;
// VTABLE: LEMBALL 0x0049a458
class CMasterInputItem {
public:
	CMasterInputItem();
	virtual CVSOStream& StreamOut(CVSOStream& p_stream); // vtable+0x00
	virtual int ProcessQueue(CBaseQueue* p_queue) = 0; // vtable+0x04
	virtual int IsReady() = 0;                         // vtable+0x08

private:
	unsigned char m_unknown0x04[4]; // 0x04
	unsigned int m_unk0x08;         // 0x08
};

#endif
