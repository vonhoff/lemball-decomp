#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_018)
// FUNCTION: LEMBALL 0x0044aa20
unsigned int __fastcall GetTimedFrameSequenceIndex(void* pObject)
{
	if (*(int*) ((char*) pObject + 0x1c) != 0) {
		return *(unsigned int*) ((char*) pObject + 8);
	}
	unsigned int nFrameValue = *(unsigned int*) ((char*) pObject + 0x10);
	unsigned int nElapsedTime = *(unsigned int*) ((char*) pObject + 0x14) - *(unsigned int*) ((char*) pObject + 0xc);
	if (nFrameValue <= nElapsedTime) {
		nFrameValue = *(unsigned int*) ((char*) pObject + 4) - 1;
		*(int*) ((char*) pObject + 0x1c) = 1;
		*(unsigned int*) ((char*) pObject + 8) = nFrameValue;
		return nFrameValue;
	}
	nFrameValue = ((nElapsedTime % nFrameValue) * *(unsigned int*) ((char*) pObject + 4)) / nFrameValue;
	if (*(int*) ((char*) pObject + 0x18) != 1) {
		nFrameValue = (*(unsigned int*) ((char*) pObject + 4) - nFrameValue) - 1;
	}
	return nFrameValue;
}
#endif
