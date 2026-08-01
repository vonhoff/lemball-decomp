#include "ENGINE/CORE/WIN32.H"

class CTextWindowSyncInterface {
public:
	virtual void BeginUpdate(void);
	virtual void EndUpdate(void);
};

class CTextWindow {
	void* m_pVtable00;
	unsigned char m_abReserved04[0x18];
	HWND m_hWindow1C;

public:
	void Invalidate(void);
};

// FUNCTION: LEMBALL 0x00473f60
void CTextWindow::Invalidate(void)
{
	CTextWindowSyncInterface* pSync;

	pSync = (CTextWindowSyncInterface*) this;
	pSync->BeginUpdate();
	InvalidateRect(m_hWindow1C, 0, 0);
	pSync->EndUpdate();
}
