#ifndef LEMBALL_CFORMATIONMANAGER_H
#define LEMBALL_CFORMATIONMANAGER_H

struct CFormationVector {
	int x;
	int y;
};

class CFormationManager {
public:
	CFormationVector m_aFormationVectors00[24];
	CFormationVector m_aTransformedVectorsC0[8];
	int m_nVectorCursor100;

	void Restart(void);
};

#endif
