#ifndef LEMBALL_FRONTEND_RESOURCES_FRONTENDPRIMITIVERECORD_H
#define LEMBALL_FRONTEND_RESOURCES_FRONTENDPRIMITIVERECORD_H

#include "../../Common.h"
#include "../../Visos/Graphics/Primitive.h"

// SIZE 0x24
// VTABLE: LEMBALL 0x00497918
class FrontendPrimitiveRecord : public Primitive {
public:
	inline FrontendPrimitiveRecord()
		: m_s1(0), m_s0(0), m_s3(0), m_s2(0), m_s5(0), m_s4(0), m_s7(0), m_s6(0),
		  m_s9(0), m_s8(0), m_s11(0), m_s10(0), m_s13(0), m_s12(0), m_s15(0), m_s14(0)
	{
	}
	inline virtual void Draw(Gdi* p_gdi) {}
	inline virtual void Render(Gdi* p_gdi) {}
	inline virtual ~FrontendPrimitiveRecord() {}

	short m_s0;  // 0x04
	short m_s1;  // 0x06
	short m_s2;  // 0x08
	short m_s3;  // 0x0a
	short m_s4;  // 0x0c
	short m_s5;  // 0x0e
	short m_s6;  // 0x10
	short m_s7;  // 0x12
	short m_s8;  // 0x14
	short m_s9;  // 0x16
	short m_s10; // 0x18
	short m_s11; // 0x1a
	short m_s12; // 0x1c
	short m_s13; // 0x1e
	short m_s14; // 0x20
	short m_s15; // 0x22
};

#endif
