#ifndef LEMBALL_CNETWORKMESSAGE_H
#define LEMBALL_CNETWORKMESSAGE_H

// Macintosh: Visos (Generic) — CNetworkMessage
//
// CNetworkMessage is the portable serialised network-message class.  Its
// Windows reconstruction lives in the shared VsNetEffStreamBase hierarchy
// (see EFFSTRM.H, VSSTRM.H, EFFSTRM.CPP, VSNETSTR.CPP).  All 25 portable
// Macintosh methods (19 exact) are evidenced across the stream base,
// read/write, stream-lifecycle, and transport families.
//
// This header declares the canonical class identity.  The physical
// implementation remains in the existing shared-stream translation units
// to preserve MSVC 4.00 ABI, link order, and shared base-class compilation.

class CConnect;

class CNetworkMessage {
public:
	// Lifecycle
	void Initialise(void);

	// Write (big-endian network serialisation)
	void Add(unsigned int nValue);
	void Add(unsigned short nValue);
	void Add(unsigned char nValue);
	void Add(const char* pszValue);
	void Add(const unsigned char* pbValue, int cbValue);

	// Read helpers
	unsigned int GetDWORD(void);
	void Get(unsigned int& nValue);
	void ConvertDWORD(void);
	unsigned short GetWORD(void);
	void Get(unsigned short& nValue);
	void ConvertWORD(void);
	unsigned char GetBYTE(void);
	void Get(unsigned char& nValue);
	void ConvertBYTE(void);
	void Get(const char*& ppszValue);
	void Get(const unsigned char*& ppbValue, int cbValue);
	void GetCopy(char* pszTarget);
	void GetCopy(unsigned char* pbTarget, int cbTarget);
	int CopyDataStream(int nTargetBuffer, int cbRange);

	// Stream lifecycle
	void OpenDataStream(void);
	void CloseDataStream(void);

	// Transport
	void Set(unsigned char* pbSource);
	void Send(CConnect* pPeer);
};

#endif
