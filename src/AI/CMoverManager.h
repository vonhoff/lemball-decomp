#ifndef LEMBALL_CMOVERMANAGER_H
#define LEMBALL_CMOVERMANAGER_H

struct CMover;

// Shared CMoverManager modeled on the Macintosh blueprint class `CMoverManager`.
// Consolidates the formerly-duplicate local struct definitions (LEVELSTAT.CPP, LEVELVT.CPP).

struct CMoverManager {
	char m_abReserved00[0x34];
	int m_cObjects34;
	CMover* m_pObjects38;

	CMover* Find(int x, int y, int* pHeight);
	int HasTriggeredObject(void* pEntity, const int* pPosition);
	void ResetObjectCount(void);
};

#endif
