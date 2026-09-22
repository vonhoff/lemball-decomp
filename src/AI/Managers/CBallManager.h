#ifndef LEMBALL_AI_MANAGERS_CBALLMANAGER_H
#define LEMBALL_AI_MANAGERS_CBALLMANAGER_H

class CAi;
class CBall;
// SIZE 0x10
class CBallManager {
public:
	CBallManager(CAi* p_ai, int p_capacity);
	int Process();
	void Delete(CBall* p_ball);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Restart();
	~CBallManager();
	void Clear();

	friend class CAi;

private:
	CAi* m_ai;         // 0x00
	CBall** m_balls;   // 0x04
	int m_activeCount; // 0x08
	int m_capacity;    // 0x0c
};

extern CBallManager* g_pBallManager;

#endif
