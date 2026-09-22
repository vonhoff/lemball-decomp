#ifndef LEMBALL_AI_OBJECTS_CANIMSPECIAL_H
#define LEMBALL_AI_OBJECTS_CANIMSPECIAL_H

class CMap;
struct AnimSpecialEntry;
// SIZE 0x08
class CAnimSpecial {
public:
	// Inlined in CAi::Restart at 0x0041170d.
	CAnimSpecial() : m_entries(0), m_entryCount(0) {}
	// Inlined in CAi::~CAi at 0x00411a3e.
	~CAnimSpecial()
	{
		if (m_entries != 0) {
			operator delete(m_entries);
		}
	}
	unsigned int Initialise(CMap* p_map);
	friend class C2D;

private:
	AnimSpecialEntry* m_entries; // 0x00
	int m_entryCount;            // 0x04
};

#endif
