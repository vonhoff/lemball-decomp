#ifndef LEMBALL_AI_MANAGERS_CSLINKYMANAGER_H
#define LEMBALL_AI_MANAGERS_CSLINKYMANAGER_H

class CAI;
class CSlinky;
class CViewData;
// SIZE 0x10
class CSlinkyManager {
public:
	CSlinkyManager(CAI* p_ai, int p_capacity);
	int GetViewData(CViewData* p_viewData);
	void Add(int p_id, int p_minX, int p_minY, int p_maxX, int p_maxY);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();
	~CSlinkyManager();

	friend class CAI;

private:
	CAI* m_ai;           // 0x00
	CSlinky* m_slinkies; // 0x04
	int m_capacity;      // 0x08
	int m_count;         // 0x0c
};

#endif
