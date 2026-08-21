struct GameLevelChunkStreamDispatcher {
	int m_nRenderQueueNode00;
	int m_nRenderQueueTag04;
	int m_nRenderQueueReserved08;
	int m_nRenderQueueReserved0C;
	void** m_ppRegisteredStreams10;
	int m_cChunkTypes14;
	int m_cRegisteredStreams18;
	int* m_pChunkTypeToIndex1C;

	void* GetRegisteredLevelChunkStream(int nChunkType);
};

// FUNCTION: LEMBALL 0x0040b1f0
void* GameLevelChunkStreamDispatcher::GetRegisteredLevelChunkStream(int nChunkType)
{
	int iStream;

	iStream = m_pChunkTypeToIndex1C[nChunkType];
	if (iStream != -1) {
		return m_ppRegisteredStreams10[iStream];
	}
	return 0;
}
