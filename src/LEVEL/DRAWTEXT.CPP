#include "LEVEL/DRAWTEXT.H"

extern void AppendPointerQueueEntry(void *pQueue, void *pEntry);

// FUNCTION: LEMBALL 0x00469B40
void LevelScreenDrawEntry::AppendToQueue(void *pQueue) {
    m_nQueueState20 = 0;
    AppendPointerQueueEntry(pQueue, this);
}
