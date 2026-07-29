extern void* g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage[];

class LevelQueuedRenderPointSink {
public:
	LevelQueuedRenderPointSink() {}
	~LevelQueuedRenderPointSink() {}
	virtual void* Delete(unsigned int fDelete) = 0;
	virtual void AppendToQueue(void* pQueue) = 0;
	virtual void Process(void* pContext) = 0;
};

class LevelQueuedRenderRectSink : public LevelQueuedRenderPointSink {
public:
	LevelQueuedRenderRectSink()
	{
		*(void**) this = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
		m_y06 = 0;
	}

	short m_x04;
	short m_y06;
};

class LevelPackagedRectQueueEntry : public LevelQueuedRenderRectSink {
public:
	LevelPackagedRectQueueEntry();
	~LevelPackagedRectQueueEntry() {}
	virtual void* Delete(unsigned int fDelete);
	virtual void AppendToQueue(void* pQueue);
	virtual void Process(void* pContext);

	void* m_pOwner08;
	int m_nFlags0C;
	int m_nValue10;
	int m_nReserved14;
};

// FUNCTION: LEMBALL 0x00467ac0
LevelPackagedRectQueueEntry::LevelPackagedRectQueueEntry()
{
	m_x04 = 0;
}
