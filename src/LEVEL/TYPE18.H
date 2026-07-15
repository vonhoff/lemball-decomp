#ifndef LEMBALL_GAME_TYPE18_CHUNK_STREAM_H
#define LEMBALL_GAME_TYPE18_CHUNK_STREAM_H

#include "CORE/COMMON.H"
#include "STREAM/EFFSTRM.H"

class LevelType18ChunkObject;
struct LevelType18ChunkStreamEntry;

// SIZE 0x54
struct LevelType18ChunkStream : public GameEffStream {
    LevelType18ChunkObject *m_apObjects30[8]; // 0x30
    int m_cObjects50;                        // 0x50

    int EmitEntries(LevelType18ChunkStreamEntry *pEntry);
};

LevelType18ChunkStream *LEMBALL_FASTCALL ConstructType18ChunkStream(
    LevelType18ChunkStream *pStream);
LevelType18ChunkStream *LEMBALL_FASTCALL ConstructType18ChunkStreamThunk(
    LevelType18ChunkStream *pStream);

#endif
