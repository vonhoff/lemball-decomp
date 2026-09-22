#ifndef LEMBALL_VISOS_FOUNDATION_CMASTERINPUT_H
#define LEMBALL_VISOS_FOUNDATION_CMASTERINPUT_H

class CBaseQueue;
class CVsOStream;

// SIZE 0x18
// VTABLE: LEMBALL 0x0049a468
class CMasterInput {
public:
	CMasterInput(CBaseQueue* p_arg0);
	bool AddItem(void* p_item);
	bool IsEmpty();
	bool ProcessItems();
	virtual CVsOStream& StreamOut(CVsOStream& p_arg0); // vtable+0x00
	virtual ~CMasterInput();                           // vtable+0x04

	friend bool InitInput();
	friend bool QuitInput();
	friend class CHotAreaHandler;
	friend class CBaseCursor;

private:
	unsigned int m_unk0x04;   // 0x04
	unsigned int m_itemCount; // 0x08
	CBaseQueue* m_queue;      // 0x0c
	void* m_firstItem;        // 0x10
	unsigned int m_state;     // 0x14
};

extern CMasterInput* g_pMasterInput;

// SYNTHETIC: LEMBALL 0x00472270
// CMasterInput::`scalar deleting destructor'

#endif
