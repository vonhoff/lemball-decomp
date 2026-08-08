#ifndef LEMBALL_AI_CSHEEPGROUP_H
#define LEMBALL_AI_CSHEEPGROUP_H

// CSheepGroup view modeled on the Macintosh blueprint class `CSheepGroup`.
// Derived from CGenericGroup (0x16c layout); count at 0x124, child table at 0x128.
class CSheepGroup {
public:
	void* m_pVtable00;
	char m_abReserved04[0x120];
	int m_nGroupElementCount124;   // 0x124
	void** m_apChildObjects128;    // 0x128 child object pointer table

	int Process(void);
#ifdef LEMBALL_CSHEEPGROUP_HARDTAIL_METHODS
	void CheckAgainstLemmings(void);
#endif
};

#endif
