#ifndef LEMBALL_VISOS_FOUNDATION_CBASEPROCESS_H
#define LEMBALL_VISOS_FOUNDATION_CBASEPROCESS_H

// SIZE 0x0c
// VTABLE: LEMBALL 0x004930a0
class CBaseProcess {
public:
	CBaseProcess()
	{
		m_processState = 0;
		m_returnState = 0;
	}
	virtual ~CBaseProcess() {}  // vtable+0x00
	virtual void Process() = 0; // vtable+0x04

	friend class CGame;
	friend class CAi;
	friend class CBaseFrontendProcess;
	friend class TargetAbout;

private:
	int m_returnState;  // 0x04
	int m_processState; // 0x08
};

// SYNTHETIC: LEMBALL 0x00407ef0
// CBaseProcess::`scalar deleting destructor'

#endif
