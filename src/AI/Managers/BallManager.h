#ifndef LEMBALL_AI_MANAGERS_BALLMANAGER_H
#define LEMBALL_AI_MANAGERS_BALLMANAGER_H

#include "../../Common.h"

// SIZE 0x10
class BallManager {
public:
	BallManager(Ai* p_arg0, int p_arg1);
	int Process();
	void Delete(Ball* p_ball);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Restart();
	~BallManager();

private:
	Ai* m_ai;          // 0x00
	Ball** m_balls;    // 0x04
	int m_activeCount; // 0x08
	int m_capacity;    // 0x0c
};

#endif
