#include "CBallManager.h"

#include "../Objects/CBall.h"

// GLOBAL: LEMBALL 0x0049e1bc
CBallManager* g_pBallManager = 0;

// FUNCTION: LEMBALL 0x00421ea0
CBallManager::CBallManager(CAI* p_ai, int p_capacity)
{
	m_ai = p_ai;
	g_pBallManager = this;
	m_balls = 0;
	m_capacity = p_capacity;
}

// FUNCTION: LEMBALL 0x00421ec0
void CBallManager::Restart()
{
	int i = 0;
	if (m_balls != 0) {
		while (i < m_capacity) {
			m_balls[i]->Restart();
			i++;
		}
	}
}

// FUNCTION: LEMBALL 0x00421ef0
void CBallManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_activeCount = 0;
	if (p_capacity == 0) {
		m_balls = 0;
		return;
	}
	if (m_balls == 0) {
		m_balls = new CBall*[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_balls[i] = new CBall();
			m_balls[i]->Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x00421f80
CBallManager::~CBallManager()
{
	int i = 0;
	if (m_balls != 0) {
		while (i < m_capacity) {
			CBall* ball = m_balls[i];
			if (ball != 0) {
				delete ball;
			}
			i++;
		}
		delete[] m_balls;
	}
}

// FUNCTION: LEMBALL 0x00421fc0
int CBallManager::Process()
{
	int i = 0;
	while (i < m_activeCount) {
		m_balls[i]->Process();
		i++;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00421ff0
void CBallManager::Delete(CBall* p_ball)
{
	for (int i = 0; i < m_activeCount; i++) {
		if (m_balls[i] == p_ball) {
			m_activeCount--;
			for (; i < m_activeCount; i++) {
				m_balls[i] = m_balls[i + 1];
			}
			m_balls[m_activeCount] = p_ball;
			return;
		}
	}
}

// Ownership inferred from adjacent manager functions and the count at 0x08.
// FUNCTION: LEMBALL 0x00422050
void CBallManager::Clear()
{
	m_activeCount = 0;
}

// FUNCTION: LEMBALL 0x00422060
void CBallManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short count = *(unsigned short*) p_data;
	p_data += 2;
	Initialise(count);
	m_activeCount = count;
	for (int i = 0; i < m_activeCount; i++) {
		m_balls[i]->LoadLevel(p_data);
	}
}
