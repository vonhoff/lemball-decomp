#ifndef LEMBALL_VISOS_FOUNDATION_CBASEPROCESS_H
#define LEMBALL_VISOS_FOUNDATION_CBASEPROCESS_H

enum eProcessResult {
	PROCESS_RESULT_CONTINUE = 0,
	PROCESS_RESULT_CHANGE_FLOW = 1,
	PROCESS_RESULT_QUIT = 2
};

enum eProcessReturnState {
	PROCESS_RETURN_STATE_NONE = 0
};

// SIZE 0x0c
// VTABLE: LEMBALL 0x004930a0
class CBaseProcess {
public:
	CBaseProcess()
	{
		m_processState = PROCESS_RESULT_CONTINUE;
		m_returnState = PROCESS_RETURN_STATE_NONE;
	}
	virtual ~CBaseProcess() {}  // vtable+0x00
	virtual void Process() = 0; // vtable+0x04

	friend class CGame;
	friend class CAI;
	friend class CBaseFrontendProcess;
	friend class CAbout;

private:
	int m_returnState;  // 0x04
	int m_processState; // 0x08
};

// SYNTHETIC: LEMBALL 0x00407ef0
// CBaseProcess::`scalar deleting destructor'

#endif
