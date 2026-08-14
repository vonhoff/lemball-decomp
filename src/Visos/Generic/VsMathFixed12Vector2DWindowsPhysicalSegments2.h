#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_089)
// FUNCTION: LEMBALL 0x0044b660
void __cdecl add_fixed12_2d_vectors(int* pOut, const int* pA, const int* pB)
{
	pOut[0] = pA[0] + pB[0];
	pOut[1] = pA[1] + pB[1];
}
#endif
