#ifndef LEMBALL_VSNETVSMEMOBJECTPOINTERARRAY_H
#define LEMBALL_VSNETVSMEMOBJECTPOINTERARRAY_H

// VsNet VSMem object pointer array. Consolidated from identical per-TU
// duplicate definitions (VSNET.CPP, VSNETSTR.CPP).

struct VsNetVSMemObjectPointerArray {
	int m_cObjects;
	int m_nReserved04;
	int m_pObjectArray08;

	void DestroyVSMemObjectPointerArray(void);
};

#endif
