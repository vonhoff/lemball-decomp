#ifndef LEMBALL_DRAWTEXT_H
#define LEMBALL_DRAWTEXT_H

struct LevelScreenDrawEntry {
    char m_abReserved00[0x20];
    int m_nQueueState20;

    void AppendToQueue(void *pQueue);
};

#endif
