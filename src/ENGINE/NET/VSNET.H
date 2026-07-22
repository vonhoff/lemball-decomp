#ifndef LEMBALL_NETWORK_RUNTIME_H
#define LEMBALL_NETWORK_RUNTIME_H

struct VsNetPeerPayloadSender {
	virtual void Reserved00(void) = 0;
	virtual void Reserved04(void) = 0;
	virtual void Reserved08(void) = 0;
	virtual void Reserved0C(void) = 0;
	virtual void Reserved10(void) = 0;
	virtual void Reserved14(void) = 0;
	virtual void Reserved18(void) = 0;
	virtual void Reserved1C(void) = 0;
	virtual void WritePeerKey(void* pPeerKey) = 0;
	virtual int SendPayload(void* pvBuffer, int cbBuffer) = 0;
	virtual void SetAssignedPort(short nPort) = 0;

	void WriteEffStreamWithGlobalSession(void);
	int SendEffStreamPayloadWithTransportHeader(int nStream);
};

struct VsNetPeerPayloadSenderRaw {
	void* m_pVtable;
};

struct VsNetEffTransportPendingWriteState {};

void LEMBALL_FASTCALL ClearEffTransportPendingWriteState(void* pObject, void* pUnusedEdx, void* pUnused);
void LEMBALL_FASTCALL CloseAdjustedEffTransportPeerByKey(void* pObject, void* pUnusedEdx, void* pUnused);
void* LEMBALL_FASTCALL DeleteCompositeEffTransportRuntimeWrapper(void* pObject, void* pUnusedEdx, BYTE fFreeMemory);

void LEMBALL_FASTCALL ServiceEffTransportPeerEntry(void* pObject);
extern void* g_NETWORK_CompositeEffTransportVtable[4];

#endif
