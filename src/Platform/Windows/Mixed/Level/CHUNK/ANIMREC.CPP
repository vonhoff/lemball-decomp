struct AnimChunkRuntimeRecord {
	int m_nValue;
	short m_nVariant;
	unsigned char m_abReserved06[0x12];
};

#pragma pack(push, 1)
struct SerializedAnimChunkRecord {
	int m_nValue;
	short m_nVariant;
};
#pragma pack(pop)

class AnimChunkRecordManager {
public:
	int m_nReserved00;
	int m_nCount04;
	AnimChunkRuntimeRecord m_aRecords08[1];

	int Serialize(SerializedAnimChunkRecord* pOutput);
};

class LevelModeAnimChunkOwner {
public:
	unsigned char m_abReserved00[0x194];
	AnimChunkRecordManager* m_pManager194;

	void SerializeRecords(SerializedAnimChunkRecord* pOutput);
};

// FUNCTION: LEMBALL 0x0040d2b0
int AnimChunkRecordManager::Serialize(SerializedAnimChunkRecord* pOutput)
{
	int i;
	AnimChunkRuntimeRecord* pRecord;

	pRecord = m_aRecords08;
	for (i = 0; i < m_nCount04; ++i) {
		pOutput->m_nValue = pRecord->m_nValue;
		pOutput->m_nVariant = pRecord->m_nVariant;
		++pRecord;
		++pOutput;
	}
	return m_nCount04;
}

// FUNCTION: LEMBALL 0x00412e40
void LevelModeAnimChunkOwner::SerializeRecords(SerializedAnimChunkRecord* pOutput)
{
	m_pManager194->Serialize(pOutput);
}
