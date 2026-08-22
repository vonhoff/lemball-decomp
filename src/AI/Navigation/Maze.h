#ifndef LEMBALL_AI_NAVIGATION_MAZE_H
#define LEMBALL_AI_NAVIGATION_MAZE_H

#include "../../Common.h"

// SIZE 0x103c
class Maze {
public:
	Maze(Map* p_arg0);
	bool BIteration(unsigned char& p_reached, unsigned char& p_noChanges);
	bool CalcNewDistance(int p_x, int p_y);
	bool FindSquare(unsigned short p_distance, int& p_x, int& p_y);
	void BInitialise(unsigned char p_resetStats, int p_startX, int p_startY, int p_endX, int p_endY);
	void BSolution(int& p_count, Solution* p_solution);
	void Clear(unsigned char* p_change);
	void Initialise();
	void ReInitialise();
	void SwapChange();
	void UpdateChangeNext(int p_x, int p_y);
	~Maze();

private:
	Map* m_map;                     // 0x0000
	unsigned short** m_distances;   // 0x0004
	unsigned int m_reserved;        // 0x0008
	unsigned char m_changeA[0x800]; // 0x000c
	unsigned char m_changeB[0x800]; // 0x080c
	unsigned char m_changeSelect;   // 0x100c
	int m_width;                    // 0x1010
	int m_height;                   // 0x1014
	int m_startX;                   // 0x1018
	int m_startY;                   // 0x101c
	int m_endX;                     // 0x1020
	int m_endY;                     // 0x1024
	unsigned int m_startTime;       // 0x102c
	unsigned int m_totalTime;       // 0x1030
	int m_solutionCount;            // 0x1034
	int m_radius;                   // 0x1038
};

extern Maze* g_pMaze;

int Direction(int p_x0, int p_y0, int p_x1, int p_y1);
#endif
