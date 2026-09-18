#ifndef LEMBALL_VISOS_TARGET_TARGETMASTERINPUTITEM_H
#define LEMBALL_VISOS_TARGET_TARGETMASTERINPUTITEM_H

class BaseQueue;
class VsOStream;
// VTABLE: LEMBALL 0x0049a458
class TargetMasterInputItem {
public:
	virtual VsOStream& StreamOut(VsOStream& p_stream); // vtable+0x00
	// Slot roles inferred from MasterInput::ProcessItems at 0x00472190.
	virtual int ProcessQueue(BaseQueue* p_queue) = 0; // vtable+0x04
	virtual int IsReady() = 0;                        // vtable+0x08
};

#endif
