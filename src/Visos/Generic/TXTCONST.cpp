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
	virtual void* Delete(unsigned int fDelete); // vtable+0x0
	virtual void AppendToQueue(void* pQueue);   // vtable+0x4
	virtual void Process(void* pContext);       // vtable+0x8

	void* m_pOwner08;  // 0x8
	int m_nFlags0C;    // 0xc
	int m_nValue10;    // 0x10
	int m_nReserved14; // 0x14
};

// FUNCTION: LEMBALL 0x00467ac0
LevelPackagedRectQueueEntry::LevelPackagedRectQueueEntry()
{
	m_x04 = 0;
}
