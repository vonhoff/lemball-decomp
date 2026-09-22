#ifndef LEMBALL_VISOS_RESOURCES_CRESBASE_H
#define LEMBALL_VISOS_RESOURCES_CRESBASE_H

class CResBaseList;

// SIZE 0x48
// VTABLE: LEMBALL 0x00498980
class CResBase {
public:
	CResBase() { m_initialized = 0; }
	CResBase* CheckError();
	bool Direct(unsigned char*& p_cursor, CResBaseList* p_list);
	bool Direct(unsigned char*& p_headerCursor, unsigned char*& p_dataCursor, CResBaseList* p_list);
	virtual ~CResBase();                                                                       // vtable+0x00
	virtual void OnRead(unsigned char* p_source, unsigned char** p_data, unsigned int p_size); // vtable+0x04
	virtual void SetHeader();                                                                  // vtable+0x08
	virtual bool GetfVramLoaded();                                                             // vtable+0x0c
	virtual bool GetfVramSwappable();                                                          // vtable+0x10
	virtual bool GetfAnyVramLoaded();                                                          // vtable+0x14
	virtual bool ForceLoadVram();                                                              // vtable+0x18
	virtual void LoadData();                                                                   // vtable+0x1c
	virtual void UnLoadData(unsigned int p_force);                                             // vtable+0x20
	virtual void UnLoadVramData(unsigned int p_force);                                         // vtable+0x24
	virtual unsigned char* GetData();                                                          // vtable+0x28
	virtual void OnLoad();                                                                     // vtable+0x2c
	virtual void OnUnLoad();                                                                   // vtable+0x30
	virtual void SetType();                                                                    // vtable+0x34
	virtual unsigned int GetSizeUsed();                                                        // vtable+0x38
	void DoLoad(unsigned int p_resourceId);
	void Initialise();
	void UnLoad();
	void UnLoadExtData(unsigned int p_force);

	friend class CResAnim;
	friend class CResFont;
	friend class CMogRes;
	friend class CSoundManager;
	friend class CText;
	friend class CZrle;
	friend class CGame;
	friend class CBaseFrontendDrawer;
	friend class CGWnd;
	friend class CAnimsManager;
	friend class CAnim;
	friend class CAnimWnd;
	friend class CGraphicButton;
	friend class BitmapRes;
	friend class BitmapResBase;
	friend class CFrontendResourceLoader;
	friend class CBaseCursor;
	friend class CDemo;
	friend class TargetAboutScreen;
	friend class CLevelLoader;
	friend class MciMusicDevice;
	friend class CBaseRemap;

protected:
	CResBaseList* m_externalList;  // 0x04
	unsigned int m_directUseCount; // 0x08
	unsigned int m_referenceCount; // 0x0c
	unsigned int m_loaded;         // 0x10
	unsigned int m_vramLoaded;     // 0x14
	unsigned int m_initialized;    // 0x18
	unsigned int m_vramSwappable;  // 0x1c
	unsigned int m_unk0x20;        // 0x20
	unsigned int m_age;            // 0x24
	unsigned int m_dataSize;       // 0x28
	unsigned int m_fileOffset;     // 0x2c
	unsigned int m_resourceId;     // 0x30
	char* m_name;                  // 0x34
	unsigned char* m_data;         // 0x38
	unsigned int m_headerSkip;     // 0x3c
	unsigned int m_chunkType;      // 0x40
	unsigned int m_error;          // 0x44
};

// SYNTHETIC: LEMBALL 0x0045e660
// CResBase::`scalar deleting destructor'

#endif
