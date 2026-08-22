#ifndef LEMBALL_SCAFFOLD_AI_MANAGERS_SLINKYMANAGER_H
#define LEMBALL_SCAFFOLD_AI_MANAGERS_SLINKYMANAGER_H

#include "../../Common.h"

// SIZE 0x10
class SlinkyManager {
public:
	SlinkyManager(Ai* p_arg0, int p_arg1);
	int GetViewData(ViewData* p_viewData);
	void Add(int p_id, int p_minX, int p_minY, int p_maxX, int p_maxY);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();
	~SlinkyManager();

private:
	Ai* m_ai;           // 0x00
	Slinky* m_slinkies; // 0x04
	int m_capacity;     // 0x08
	int m_count;        // 0x0c
};

#endif
