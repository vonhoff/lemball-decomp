#ifndef LEMBALL_GAME_EFF_STREAMS_H
#define LEMBALL_GAME_EFF_STREAMS_H

struct GAME_EffStreamVtable;

struct GAME_EffStream {
    GAME_EffStreamVtable *m_pVtable;
    int m_nEventCode;
    int m_pvOwnedBuffer;
    int m_pvBufferEnd;
    int m_cWriteSessions;
    int m_fOwnsBuffer;
    int m_cbSerializedLength;
    int m_pvWriteCursor;
    int m_pvReadCursor;
    int m_fHasPayload;
    int m_fWritePending;
    int m_nU32Payload;

    void ResetStateFields(void);
    int LoadEffStreamFromMemory(int nSourceBuffer);
    void BeginEffStreamWriteSession(void);
    void EndEffStreamWriteSession(void);
    void QueueEffStreamWriteEvent(int nPayload);
};

#endif
