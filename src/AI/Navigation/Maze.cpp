#include "Maze.h"

// 68K 0x10615900 __ct__5CMazeFP4CMap
// STUB: LEMBALL 0x00423090
Maze::Maze(Map* p_arg0)
{
}

// 68K 0x1061593a __dt__5CMazeFv
// STUB: LEMBALL 0x004230c0
Maze::~Maze()
{
}

// 68K 0x106159a4 ReInitialise__5CMazeFv
// STUB: LEMBALL 0x00423110
void Maze::ReInitialise()
{
}

// 68K 0x10615a26 Initialise__5CMazeFv
// STUB: LEMBALL 0x00423190
void Maze::Initialise()
{
}

// 68K 0x10615ad8 CalcNewDistance__5CMazeFii
// STUB: LEMBALL 0x00423230
bool Maze::CalcNewDistance(int p_x, int p_y)
{
	return 0;
}

// 68K 0x10615bd0 FindSquare__5CMazeFUsRiRi
// STUB: LEMBALL 0x004232e0
bool Maze::FindSquare(unsigned short p_distance, int& p_x, int& p_y)
{
	return 0;
}

// 68K 0x10615c8c UpdateChangeNext__5CMazeFii
// STUB: LEMBALL 0x00423380
void Maze::UpdateChangeNext(int p_x, int p_y)
{
}

// 68K 0x10615da4 Clear__5CMazeFPUc
// STUB: LEMBALL 0x004234a0
void Maze::Clear(unsigned char* p_change)
{
}

// 68K 0x10615e04 SwapChange__5CMazeFv
// STUB: LEMBALL 0x004234f0
void Maze::SwapChange()
{
}

// 68K 0x10615e5a BInitialise__5CMazeFUciiii
// STUB: LEMBALL 0x00423530
void Maze::BInitialise(unsigned char p_resetStats, int p_startX, int p_startY, int p_endX, int p_endY)
{
}

// 68K 0x10615f52 BIteration__5CMazeFRUcRUc
// STUB: LEMBALL 0x00423650
bool Maze::BIteration(unsigned char& p_reached, unsigned char& p_noChanges)
{
	return 0;
}

// 68K 0x10616122 Direction__Fiiii
// FUNCTION: LEMBALL 0x00423890
int Direction(int p_x0, int p_y0, int p_x1, int p_y1)
{
	return ((p_y0 - p_y1) * 3 - p_x1) + 4 + p_x0;
}

// 68K 0x1061616a BSolution__5CMazeFRiP9tSolution
// STUB: LEMBALL 0x004238b0
void Maze::BSolution(int& p_count, Solution* p_solution)
{
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x0049cf58
Maze* g_pMaze;
