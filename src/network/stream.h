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
};

struct NETWORK_DualHandleEffStream {
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
    int m_nState2c;
    int m_nReserved30;
    int m_nReserved34;
    int m_nReserved38;
    int m_nReserved3c;
    int m_nReserved40;
    void **m_pChannelStateThunk44;
    void *m_pPrimaryHandleArray48;
    void *m_pPrimaryHandleObject4c;
    void *m_pSecondaryHandleArray50;
    void *m_pSecondaryHandleObject54;
    unsigned char m_abChannelState58[0x2c];

    void DestroyDualHandleEffStream(void);
};

struct NETWORK_EffStreamBase {
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
    int m_pTagBuffer2c;
    unsigned short m_nWord30;
    unsigned short m_nWord32;
    unsigned short m_nWord34;
    unsigned short m_nWord36;

    void WriteEffStreamU32BE(unsigned int nValue);
    void WriteEffStreamU16BE(unsigned short nValue);
    void WriteEffStreamBytes(const void *pvSource, unsigned int cbWrite);
    void ReadEffStreamU32BE(unsigned char *pbTarget);
    void ReadEffStreamU16BE(unsigned char *pbTarget);
    void ReadEffStreamBytes(void *pvTarget, unsigned int cbRead);
    void SaveEffStreamToMemoryRange(int nTargetBuffer, int cbRange);
    NETWORK_EffStreamBase *ConstructEffStreamBase(void);
    void DestroyEffStreamBase(void);
    void DestroyEffStreamBaseThunk(void);
};

struct NETWORK_RuntimeChannelStackReleaseFront {
    void Release(void);
};

#endif
