#ifndef LEMBALL_NETWORK_STREAM_H
#define LEMBALL_NETWORK_STREAM_H

struct NETWORK_EffStreamChannelState {
    void **m_pVtable;
    int m_nReserved04;
    int m_nSelectedPeer08;
    int m_nReserved0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nReserved18;
    int m_nReserved1c;
    unsigned short m_nSelectedPort20;
    unsigned short m_nReserved22;
    int m_nReserved24;
    void *m_pSideBuffer28;

    NETWORK_EffStreamChannelState *ConstructEffStreamChannelState(void);
    void DestroyEffStreamChannelState(void);
    void SetEffStreamChannelAsyncErrorStatus(int nStatus);
};

struct NETWORK_EffStreamCore {
    void **m_pVtable;
    int m_nReserved04;
    int m_pPayloadBuffer08;
    int m_fOwnsPayload0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nBufferEnd18;
    int m_nReserved1c;
    int m_nReserved20;
    int m_nReserved24;
    int m_fBusy28;
};

/*
 * 0045F750 first installs the shared GAME_EffStream-compatible table
 * (004932C8), calls GAME_EffStream::ResetStateFields with unchanged this,
 * then installs the EFF stream table (00498F40).  This proves a shared ABI
 * prefix, not a direct C++ base edge: file-backed record slots reuse the
 * prefix but place marker data at +2c.  The common prefix is modeled as
 * NETWORK_EffStreamCore; the GAME_EffStream class edge remains unproven.
 */
struct NETWORK_EffStreamBase : NETWORK_EffStreamCore {
    int *m_pTagBuffer2c;
    unsigned short m_nWord30;
    unsigned short m_nWord32;
    unsigned short m_nWord34;
    unsigned short m_nWord36;

    void WriteEffStreamU32BE(unsigned int nValue);
    void WriteEffStreamU16BE(unsigned short nValue);
    void WriteEffStreamBytes(const void *pvSource, unsigned int cbWrite);
    void WriteEffStreamCString(const char *pszSource);
    void ReadEffStreamU32BE(unsigned char *pbTarget);
    unsigned int ReadEffStreamU32BEValue(void);
    void ReadEffStreamU16BE(unsigned char *pbTarget);
    void ReadEffStreamBytes(void *pvTarget, unsigned int cbRead);
    void ReadEffStreamCString(char **ppszTarget);
    int SaveEffStreamToMemoryRange(int nTargetBuffer, int cbRange);
    NETWORK_EffStreamBase *ConstructEffStreamBase(void);
    void DestroyEffStreamBase(void);
    void *DeleteEffStreamBaseWrapper(BYTE fDeleteFlags);
    void DestroyEffStreamBaseThunk(void);
};

struct NETWORK_DualHandleEffStream : NETWORK_EffStreamBase {
    char m_abReserved38[0x0c];
    void **m_pChannelStateConstructionOffsets44;
    void *m_pPrimaryHandleArray48;
    void *m_pPrimaryHandleObject4c;
    void *m_pSecondaryHandleArray50;
    void *m_pSecondaryHandleObject54;
    unsigned char m_abChannelState58[0x2c];

    void ReleaseEffStreamPrimaryHandleGroup(void);
    void ReleaseEffStreamSecondaryHandleGroup(void);
    void ConfigureEffStreamPrimaryHandleGroup(int nPacketCount, int cbPayload, int nMode);
    void ConfigureEffStreamSecondaryHandleGroup(int nPacketCount, int cbPayload);
    void DestroyDualHandleEffStream(void);
};

struct NETWORK_TimedEffStream : NETWORK_EffStreamBase {
    int m_nReserved38;
    DWORD m_dwLastTick3c;
    int m_nReserved40;
    void **m_pChannelStateConstructionOffsets44;
    void *m_pPrimaryHandleArray48;
    void *m_pSecondaryHandleArray4c;
    void *m_pTertiaryHandleArray50;
    int m_nReserved54;
    int m_nReserved58;
    int m_nHandleIndex5c;
    int m_nReserved60;
    int m_nReserved64;
    int m_nReserved68;
    int m_nReserved6c;
    void *m_pRuntimeService70;
    unsigned char m_abChannelState78[0x2c];

    void ReleaseTimedEffStreamPrimaryHandle(void);
    void ReleaseTimedEffStreamPrimaryHandleThunk(int nUnused0, int nUnused1, int nUnused2);
    void ReleaseTimedEffStreamSecondaryHandles(void);
    void ConfigureTimedEffStreamSecondaryHandle(int nPacketCount);
    void InvokeTimedEffStreamServiceCallback(void *pArgument);
    NETWORK_TimedEffStream *ConstructTimedEffStream(int fConstructChannelState);
    void DestroyTimedEffStream(void);
};

/* Runtime stack contains three embedded stream subobjects.  Keep offsets
 * explicit: channel state +0x24, timed stream +0x54, dual stream +0xcc. */
struct NETWORK_RuntimeChannelStack {
    void **m_pVtable;
    void **m_pOuterConstructionOffsets04;
    int m_nReserved08;
    int m_nReserved0c;
    unsigned char *m_pbRuntimeFlags10;
    int m_nReserved14;
    int m_nReserved18;
    void *m_pRuntimeSideBuffer1c;
    unsigned char m_abChannelState24[0x30];
    unsigned char m_abTimedStream54[0x78];
    unsigned char m_abDualStreamcc[0x58];
    void **m_pEmbeddedConstructionOffsets124;
    unsigned char m_abDualStreamTail128[0x2c];

    void *ConstructEffTransportRuntimeChannelStack(int fConstructEmbeddedObjects);
};

struct NETWORK_RuntimeChannelStackReleaseFront {
    void Release(void);
};

#endif
