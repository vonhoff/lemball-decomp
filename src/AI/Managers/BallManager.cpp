#include "BallManager.h"

#include "../Objects/Ball.h"

// GLOBAL: LEMBALL 0x0049e1bc
BallManager* g_pBallManager = 0;

// 68K 0x10603ef6 __ct__12CBallManagerFP3CAIi
// FUNCTION: LEMBALL 0x00421ea0
BallManager::BallManager(Ai* p_arg0, int p_arg1)
{
	m_ai = p_arg0;
	g_pBallManager = this;
	m_balls = 0;
	m_capacity = p_arg1;
}

// 68K 0x10603f34 Restart__12CBallManagerFv
// FUNCTION: LEMBALL 0x00421ec0
void BallManager::Restart()
{
	int i = 0;
	if (m_balls != 0) {
		while (i < m_capacity) {
			m_balls[i]->Restart();
			i++;
		}
	}
}

// 68K 0x10603f8c Initialise__12CBallManagerFi
// FUNCTION: LEMBALL 0x00421ef0
void BallManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_activeCount = 0;
	if (p_capacity == 0) {
		m_balls = 0;
		return;
	}
	if (m_balls == 0) {
		m_balls = new Ball*[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_balls[i] = new Ball();
			m_balls[i]->Restart();
		}
	}
}

// 68K 0x10604034 __dt__12CBallManagerFv
// FUNCTION: LEMBALL 0x00421f80
BallManager::~BallManager()
{
	int i = 0;
	if (m_balls != 0) {
		while (i < m_capacity) {
			if (m_balls[i] != 0) {
				delete m_balls[i];
			}
			i++;
		}
		delete[] m_balls;
	}
}

// 68K 0x106040be Process__12CBallManagerFv
// FUNCTION: LEMBALL 0x00421fc0
int BallManager::Process()
{
	int i = 0;
	if (0 < m_activeCount) {
		while (i < m_activeCount) {
			m_balls[i]->Process();
			i++;
		}
	}
	return 1;
}

// 68K 0x10604112 Delete__12CBallManagerFP5CBall
// FUNCTION: LEMBALL 0x00421ff0
void BallManager::Delete(Ball* p_ball)
{
	int i = 0;
	int activeCount = m_activeCount;
	if (0 < activeCount) {
		Ball** ball = m_balls;
		while (*ball != p_ball) {
			ball++;
			i++;
			if (activeCount <= i) {
				return;
			}
		}
		m_activeCount = activeCount - 1;
		if (i < activeCount - 1) {
			int j = i;
			do {
				m_balls[j] = m_balls[j + 1];
				j++;
			} while (j < m_activeCount);
		}
		m_balls[m_activeCount] = p_ball;
	}
}

// 68K 0x10604190 LoadLevel__12CBallManagerFPUciUc
// FUNCTION: LEMBALL 0x00422060
void BallManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short count = *(unsigned short*) p_data;
	int i = 0;
	p_data = p_data + 2;
	Initialise(count);
	m_activeCount = count;
	if (count > 0) {
		while (i < m_activeCount) {
			m_balls[i]->LoadLevel(p_data);
			i++;
		}
	}
}
