#ifndef LEMBALL_LIFTCHUNKMANAGERSERIALIZATIONVIEW_H
#define LEMBALL_LIFTCHUNKMANAGERSERIALIZATIONVIEW_H

// Lift-chunk endpoint types + serialization view. Consolidated from identical
// per-TU duplicate definitions (LINKSCF.CPP, LEVELRUN.CPP).

struct LiftChunkEndpointPoint {
	short m_x;
	short m_y;
	short m_z;
};

struct LiftChunkEndpointPair {
	LiftChunkEndpointPoint m_Start;
	LiftChunkEndpointPoint m_End;
};

struct LiftChunkManagerSerializationView {
	int SerializeLiftChunkEndpointPairs(LiftChunkEndpointPair* pPairs);
};

#endif
