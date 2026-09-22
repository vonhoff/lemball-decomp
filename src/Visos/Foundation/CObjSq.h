#ifndef LEMBALL_VISOS_FOUNDATION_COBJSQ_H
#define LEMBALL_VISOS_FOUNDATION_COBJSQ_H

// SIZE 0x0a
class CObjSq {
public:
	CObjSq();
	friend class C2D;

private:
	short m_objectCount;    // 0x00
	short m_viewIndices[4]; // 0x02
};

#endif
