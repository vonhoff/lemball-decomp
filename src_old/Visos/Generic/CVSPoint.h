#ifndef LEMBALL_CVSPOINT_H
#define LEMBALL_CVSPOINT_H

class CVSPoint {
public:
	short m_nX;
	short m_nY;

	int Equals(const CVSPoint* pOther);
};

#endif
