#ifndef LEMBALL_GAME_EFF_STREAMS_H
#define LEMBALL_GAME_EFF_STREAMS_H

struct GameEffStreamVtable;

extern void* g_GAME_EffStreamConstructionVtable[6];
extern void* g_LEVEL_EffChunkStreamBaseVtable[10];

struct GameEffStream {
	GameEffStreamVtable* m_pVtable;
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
	GameEffStream* ConstructNetworkLobbyU32PayloadStream(int nEventCode);
};

struct GameEffStreamPayloadSize8 : public GameEffStream {
	int m_nSecondaryU32Payload;

	GameEffStreamPayloadSize8* ConstructPayloadSize8Thunk(void);
	GameEffStreamPayloadSize8* ConstructPayloadSize8(void);
	void WritePayloadSize8(void);
	void ReadPayloadSize8(void);
	GameEffStreamPayloadSize8* DeletePayloadSize8(unsigned char fDelete);
};

void LEMBALL_FASTCALL StartNetworkLobbyTransportSession(void* pController);
void LEMBALL_FASTCALL StopNetworkLobbyTransportSession(void* pController);
void ConfigureNetworkLobbyTransportHostName(void* pRuntime, char* pszHostName);

#endif
