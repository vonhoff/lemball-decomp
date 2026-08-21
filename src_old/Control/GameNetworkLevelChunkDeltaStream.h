#ifndef LEMBALL_GAMENETWORKLEVELCHUNKDELTASTREAM_H
#define LEMBALL_GAMENETWORKLEVELCHUNKDELTASTREAM_H

// Network level-chunk delta stream. Consolidated from identical per-TU
// duplicate definitions (CGame.cpp, LEVELRUN.CPP).

struct GameNetworkLevelChunkDeltaStream {
	void* ConstructNetworkLevelChunkDeltaStream(int nOwner);
	void* ConstructNetworkLevelChunkDeltaStreamEntry(int nOwner);
};

#endif
