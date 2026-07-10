#ifndef LEMBALL_NETWORK_RUNTIME_H
#define LEMBALL_NETWORK_RUNTIME_H

struct NETWORK_PeerPayloadSender {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved0C(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved1C(void) = 0;
    virtual void WritePeerKey(void *pPeerKey) = 0;
    virtual int SendPayload(void *pvBuffer, int cbBuffer) = 0;
    virtual void SetAssignedPort(short nPort) = 0;

    void WriteEffStreamWithGlobalSession(void);
    int SendEffStreamPayloadWithTransportHeader(int nStream);
};

struct NETWORK_PeerPayloadSenderRaw {
    void *m_pVtable;
};

struct NETWORK_EffTransportPendingWriteState {
    void Clear(void *pUnused);
};

#endif
