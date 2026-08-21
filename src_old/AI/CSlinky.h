#ifndef LEMBALL_CSLINKY_H
#define LEMBALL_CSLINKY_H

class CSlinky {
public:
	int m_vtable;                 // +0x00 real vptr (slot 0 = vtbl)
	int m_field04;
	int m_field08;
	int m_field0C;
	int m_field10;
	int m_field14;
	int m_field18;
	int m_field1C;
	int m_field20;
	int m_field24;
	int m_field28;
	int m_field2C;
	int m_field30;
	int m_field34;
	int m_field38;
	int m_field3C;
	int m_field40;
	int m_field44;
	int m_field48;
	int m_field4C;
	int m_field50;
	int m_field54;
	int m_field58;
	int m_field5C;
	int m_field60;
	int m_field64;
	int m_field68;
	int m_field6C;
	int m_field70;
	int m_field74;
	int m_field78;
	int m_field7C;
	int m_field80;
	int m_field84;
	int m_field88;
	int m_field8C;
	int m_field90;
	int m_motionTimeMs94;         // +0x94
	int m_field98;
	int m_xPosWorld9C;            // +0x9c (<<12 fixed point)
	int m_yPosWorldA0;            // +0xa0
	int m_zPosWorldA4;            // +0xa4
	int m_moveTargetXA8;          // +0xa8
	int m_moveTargetYAC;          // +0xac
	int m_moveTargetZB0;          // +0xb0
	int m_fieldB4;
	int m_fieldB8;
	unsigned short m_wFacingDirBC;   // +0xbc (0..3)
	int m_fieldC0;
	int m_fieldC4;
	int m_fieldC8;
	int m_nFrameClockTickCC;      // +0xcc
	int m_fieldD0;
	int m_fieldD4;
	int m_fieldD8;
	int m_fieldDC;
	int m_fieldE0;
	int m_fieldE4;
	int m_fieldE8;
	int m_fieldEC;
	int m_fieldF0;
	int m_fieldF4;
	int m_fieldF8;
	int m_fieldFC;
	int m_field100;
	int m_field104;
	int m_field108;
	int m_field10C;
	int m_field110;
	int m_field114;
	int m_field118;
	int m_field11C;
	int m_field120;
	int m_setTargetX124;          // +0x124
	int m_setTargetY128;          // +0x128
	int m_setTarget2C12C;         // +0x12c
	int m_setTarget2D130;         // +0x130

	void Set(int nSlinkX, int nSlinkY, int nSlinkZ, int nSlinkW);
	void Move(void);
	int Process(void);
};

#endif // LEMBALL_CSLINKY_H
