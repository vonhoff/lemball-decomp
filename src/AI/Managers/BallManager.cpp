#include "BallManager.h"

#include "../Objects/Ball.h"

// 68K 0x10603ef6 __ct__12CBallManagerFP3CAIi
// STUB: LEMBALL 0x00421ea0
BallManager::BallManager(Ai* p_arg0, int p_arg1)
{
}

// 68K 0x10603f34 Restart__12CBallManagerFv
// FUNCTION: LEMBALL 0x00421ec0
void BallManager::Restart()
{
	Ball** balls = m_balls;
	if (balls != 0) {
		for (int i = 0; i < m_capacity; i++) {
			balls[i]->Restart();
		}
	}
}

// 68K 0x10603f8c Initialise__12CBallManagerFi
// STUB: LEMBALL 0x00421ef0
void BallManager::Initialise(int p_capacity)
{
}

// 68K 0x10604034 __dt__12CBallManagerFv
// STUB: LEMBALL 0x00421f80
BallManager::~BallManager()
{
}

// 68K 0x106040be Process__12CBallManagerFv
// STUB: LEMBALL 0x00421fc0
int BallManager::Process()
{
	return 0;
}

// 68K 0x10604112 Delete__12CBallManagerFP5CBall
// STUB: LEMBALL 0x00421ff0
void BallManager::Delete(Ball* p_ball)
{
}

// 68K 0x10604190 LoadLevel__12CBallManagerFPUciUc
// STUB: LEMBALL 0x00422060
void BallManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
}
