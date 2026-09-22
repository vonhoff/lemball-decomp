// Opaque receiver type for the original single-pointer thiscall callbacks.
class CArrayElementCallback {};

typedef void (CArrayElementCallback::*ArrayCallback)();

// FUNCTION: LEMBALL 0x00406130
void __stdcall DestroyArrayElementsForward(char* p_array, int p_stride, int p_count, ArrayCallback p_callback)
{
	while (--p_count >= 0) {
		(((CArrayElementCallback*) p_array)->*p_callback)();
		p_array += p_stride;
	}
}

// FUNCTION: LEMBALL 0x0040acf0
void __stdcall DestroyArrayElementsReverse(char* p_array, int p_stride, int p_count, ArrayCallback p_callback)
{
	p_array += p_count * p_stride;
	while (--p_count >= 0) {
		p_array -= p_stride;
		(((CArrayElementCallback*) p_array)->*p_callback)();
	}
}
