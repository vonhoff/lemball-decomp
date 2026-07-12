#ifndef LEMBALL_GAME_TYPE18_CHUNK_STREAM_H
#define LEMBALL_GAME_TYPE18_CHUNK_STREAM_H

#include "../engine/common.h"
#include "eff_streams.h"

class LEVEL_Type18ChunkObject;
struct LEVEL_Type18ChunkStreamEntry;

struct LEVEL_Type18ChunkStream : public GAME_EffStream {
    LEVEL_Type18ChunkObject *m_apObjects30[8];
    int m_cObjects50;

    int EmitEntries(LEVEL_Type18ChunkStreamEntry *pEntry);
};

typedef char LEVEL_Type18ChunkStream_size_must_be_0x54[
    sizeof(LEVEL_Type18ChunkStream) == 0x54 ? 1 : -1];

LEVEL_Type18ChunkStream *LEMBALL_FASTCALL ConstructType18ChunkStream(
    LEVEL_Type18ChunkStream *pStream);
LEVEL_Type18ChunkStream *LEMBALL_FASTCALL ConstructType18ChunkStreamThunk(
    LEVEL_Type18ChunkStream *pStream);

#endif
