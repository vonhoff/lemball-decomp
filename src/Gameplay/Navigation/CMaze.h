#ifndef LEMBALL_AI_NAVIGATION_CMAZE_H
#define LEMBALL_AI_NAVIGATION_CMAZE_H

class CMap;
struct Solution;

enum eMazeDistanceState {
	MAZE_DISTANCE_UNREACHED = 0xff00,
	MAZE_DISTANCE_BLOCKED = 0xffff
};

enum eMazeChangeBitmapLayout {
	MAZE_CHANGE_BITMAP_ROW_WIDTH_BITS = 128,
	MAZE_CHANGE_BITMAP_BITS_PER_BYTE = 8,
	MAZE_CHANGE_BITMAP_BYTE_INDEX_SHIFT = 3,
	MAZE_CHANGE_BITMAP_BIT_INDEX_MASK = MAZE_CHANGE_BITMAP_BITS_PER_BYTE - 1,
	MAZE_CHANGE_BITMAP_FIRST_COLUMN_MASK = 0x80
};

// SIZE 0x103c
class CMaze {
public:
	CMaze(CMap* p_map);
	bool BIteration(unsigned int& p_reached, unsigned int& p_noChanges);
	bool CalcNewDistance(int p_x, int p_y);
	bool FindSquare(unsigned short p_distance, int& p_x, int& p_y);
	void BInitialise(unsigned int p_resetStats, int p_startX, int p_startY, int p_endX, int p_endY);
	void BSolution(int& p_count, Solution* p_solution);
	void Clear(unsigned char* p_change);
	void Initialise();
	void ReInitialise();
	void SwapChange();
	void UpdateChangeNext(int p_x, int p_y);
	~CMaze();

	friend class CGameObject;

private:
	CMap* m_map;                    // 0x0000
	unsigned short** m_distances;   // 0x0004
	unsigned int m_routeSearchBusy; // 0x0008
	unsigned char m_changeA[0x800]; // 0x000c
	unsigned char m_changeB[0x800]; // 0x080c
	unsigned char m_changeSelect;   // 0x100c
	int m_width;                    // 0x1010
	int m_height;                   // 0x1014
	int m_startX;                   // 0x1018
	int m_startY;                   // 0x101c
	int m_endX;                     // 0x1020
	int m_endY;                     // 0x1024
	unsigned int m_unk0x1028;       // 0x1028
	unsigned long m_startTime;      // 0x102c
	unsigned long m_totalTime;      // 0x1030
	int m_solutionCount;            // 0x1034
	int m_radius;                   // 0x1038
};

extern CMaze* g_pMaze;

int Direction(int p_x0, int p_y0, int p_x1, int p_y1);
#endif
