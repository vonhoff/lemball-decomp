#include "type18_chunk_stream.h"

#include "../engine/memory_arena.h"
#include "../network/stream.h"

extern int ReturnTrueVtableCallbackThunk(void);
extern int ReturnTrueVtableCallbackSecondaryThunk(void);
extern void NoopVtableCallbackThunk(void);
extern void noop_vtable_callback_0040adc0(void);
extern int PASCAL return_false_vtable_callback_0040ab80(
    void *pUnused1, void *pUnused2, void *pUnused3);
extern int PASCAL return_false_vtable_callback_0040ab90(void *pUnused);

extern void *g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;
extern int g_GAME_ManagedEntityRegistryTable[1000];
extern unsigned short g_GAME_ManagedEntityRegistryCount;
extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotId(
    int nManagedEntityObject);

struct LEVEL_Type18ChunkStreamEntry {
    char m_abPayload[0x4c];
};

class LEVEL_Type18ChunkObject {
public:
    virtual void *Delete(BYTE fDelete) = 0;
    virtual void Reserved01(void) = 0;
    virtual void Reserved02(void) = 0;
    virtual void EmitRenderEntry(LEVEL_Type18ChunkStreamEntry *pOutput) = 0;
    virtual void Reserved04(void) = 0;
    virtual int TestPreparedState(void) = 0;
    virtual void Reserved06(void) = 0;
    virtual void Reserved07(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved09(void) = 0;
    virtual void Reserved0A(void) = 0;
    virtual void Reserved0B(void) = 0;
    virtual void Reserved0C(void) = 0;
    virtual void Reserved0D(void) = 0;
    virtual void Reserved0E(void) = 0;
    virtual void Reserved0F(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved11(void) = 0;
    virtual void Reserved12(void) = 0;
    virtual void Reserved13(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved15(void) = 0;
    virtual void Reserved16(void) = 0;
    virtual void Reserved17(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved19(void) = 0;
    virtual void Reserved1A(void) = 0;
    virtual void Reserved1B(void) = 0;
    virtual void Reserved1C(void) = 0;
    virtual void Reserved1D(void) = 0;
    virtual void Reserved1E(void) = 0;
    virtual void Reserved1F(void) = 0;
    virtual void Reserved20(void) = 0;
    virtual void Reserved21(void) = 0;
    virtual void Reserved22(void) = 0;
    virtual void Reserved23(void) = 0;
    virtual void Reserved24(void) = 0;
    virtual void Reserved25(void) = 0;
    virtual void Reserved26(void) = 0;
    virtual void Reserved27(void) = 0;
    virtual void Reserved28(void) = 0;
    virtual void Reserved29(void) = 0;
    virtual void Reserved2A(void) = 0;
    virtual void Reserved2B(void) = 0;
    virtual void Reserved2C(void) = 0;
    virtual void Reserved2D(void) = 0;
    virtual void Reserved2E(void) = 0;
    virtual void Reserved2F(void) = 0;
    virtual void Reserved30(void) = 0;
    virtual void Reserved31(void) = 0;
    virtual void Reserved32(void) = 0;
    virtual void Reserved33(void) = 0;
    virtual void Reserved34(void) = 0;
    virtual void Reserved35(void) = 0;
    virtual void Reserved36(void) = 0;
    virtual void Reserved37(void) = 0;
    virtual void Reserved38(void) = 0;
    virtual void Reserved39(void) = 0;
    virtual void Reserved3A(void) = 0;
    virtual void Reserved3B(void) = 0;
    virtual void Reserved3C(void) = 0;
    virtual void Reserved3D(void) = 0;
    virtual void Reserved3E(void) = 0;
    virtual void Reserved3F(void) = 0;
    virtual void Reserved40(void) = 0;
    virtual void Reset(void) = 0;
    virtual int HandleChunkStreamCommand(
        unsigned short nCommand, LEVEL_Type18ChunkStream *pStream) = 0;

public:
    char m_abReserved04[0xb4];
    int m_nStateB8;
    char m_abReservedBC[0x68];
    int m_fPrepare124;
    char m_abReserved128[0x20];
    int m_fPending148;
    int m_nReserved14C;
};

class LEVEL_EffChunkStreamVirtualInterface {
public:
    virtual int Reserved00(void) = 0;
    virtual int Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved0C(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved1C(void) = 0;
    virtual int HandleUnhandledChunkStreamCommand(
        unsigned short nCommand,
        LEVEL_Type18ChunkObject *pObject,
        LEVEL_Type18ChunkStream *pStream) = 0;
    virtual int Reserved24(void) = 0;
};

// FUNCTION: LEMBALL 0x0040AC00
void *LEMBALL_FASTCALL DeleteEffStreamConstructionBase(
    void *pObject, void *pUnused, BYTE fDelete)
{
    (void)pUnused;
    ((NETWORK_EffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00401AAA
void *LEMBALL_FASTCALL DeleteEffStreamConstructionBaseThunk(
    void *pObject, void *pUnused, BYTE fDelete)
{
    return DeleteEffStreamConstructionBase(pObject, pUnused, fDelete);
}

// FUNCTION: LEMBALL 0x0040ABA0
void *LEMBALL_FASTCALL DeleteLevelEffChunkStreamBase(
    void *pObject, void *pUnused, BYTE fDelete)
{
    (void)pUnused;
    ((NETWORK_EffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00401645
void *LEMBALL_FASTCALL DeleteLevelEffChunkStreamBaseThunk(
    void *pObject, void *pUnused, BYTE fDelete)
{
    return DeleteLevelEffChunkStreamBase(pObject, pUnused, fDelete);
}

// FUNCTION: LEMBALL 0x0040AB70
void NoopLevelEffChunkStreamVirtual(void) {
}

// FUNCTION: LEMBALL 0x00403427
void NoopLevelEffChunkStreamVirtualThunk(void) {
    NoopLevelEffChunkStreamVirtual();
}

// FUNCTION: LEMBALL 0x00402E23
void NoopType18ChunkStreamWriteThunk(void) {
    noop_vtable_callback_0040adc0();
}

// FUNCTION: LEMBALL 0x00402DFB
int PASCAL ReturnFalseChunkStreamCommandThunk(
    void *pUnused1, void *pUnused2, void *pUnused3)
{
    return return_false_vtable_callback_0040ab80(
        pUnused1, pUnused2, pUnused3);
}

// FUNCTION: LEMBALL 0x004012B7
int PASCAL ReturnFalseChunkStreamRenderThunk(void *pUnused) {
    return return_false_vtable_callback_0040ab90(pUnused);
}

void LEMBALL_FASTCALL DispatchType18ChunkStreamCommands(
    LEVEL_Type18ChunkStream *pStream);
void LEMBALL_FASTCALL ResetType18ChunkStreamObjects(
    LEVEL_Type18ChunkStream *pStream);
void LEMBALL_FASTCALL PrepareType18ChunkStreamObjects(
    LEVEL_Type18ChunkStream *pStream);
void *LEMBALL_FASTCALL DeleteType18ChunkStream(
    LEVEL_Type18ChunkStream *pStream, void *pUnused, BYTE fDelete);

// FUNCTION: LEMBALL 0x00402586
void LEMBALL_FASTCALL DispatchType18ChunkStreamCommandsThunk(
    LEVEL_Type18ChunkStream *pStream)
{
    DispatchType18ChunkStreamCommands(pStream);
}

// FUNCTION: LEMBALL 0x00402BF8
void LEMBALL_FASTCALL ResetType18ChunkStreamObjectsThunk(
    LEVEL_Type18ChunkStream *pStream)
{
    ResetType18ChunkStreamObjects(pStream);
}

// FUNCTION: LEMBALL 0x00402A90
void LEMBALL_FASTCALL PrepareType18ChunkStreamObjectsThunk(
    LEVEL_Type18ChunkStream *pStream)
{
    PrepareType18ChunkStreamObjects(pStream);
}

// FUNCTION: LEMBALL 0x0040280B
int LEMBALL_FASTCALL EmitType18ChunkStreamEntriesThunk(
    LEVEL_Type18ChunkStream *pStream, void *pUnused, void *pOutput)
{
    (void)pUnused;
    return pStream->EmitEntries(
        (LEVEL_Type18ChunkStreamEntry *)pOutput);
}

// FUNCTION: LEMBALL 0x0040114A
void *LEMBALL_FASTCALL DeleteType18ChunkStreamThunk(
    LEVEL_Type18ChunkStream *pStream, void *pUnused, BYTE fDelete)
{
    return DeleteType18ChunkStream(pStream, pUnused, fDelete);
}

// GLOBAL: LEMBALL 0x004932c8
void *g_GAME_EffStreamConstructionVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)_purecall,
    (void *)NoopVtableCallbackThunk,
    (void *)_purecall,
    (void *)DeleteEffStreamConstructionBaseThunk,
};

// GLOBAL: LEMBALL 0x00493268
void *g_LEVEL_EffChunkStreamBaseVtable[10] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)DispatchType18ChunkStreamCommandsThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopType18ChunkStreamWriteThunk,
    (void *)DeleteLevelEffChunkStreamBaseThunk,
    (void *)NoopLevelEffChunkStreamVirtualThunk,
    (void *)_purecall,
    (void *)ReturnFalseChunkStreamCommandThunk,
    (void *)ReturnFalseChunkStreamRenderThunk,
};

// GLOBAL: LEMBALL 0x004936c8
static void *g_LEVEL_Type18ChunkStreamVtable[10] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)DispatchType18ChunkStreamCommandsThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopType18ChunkStreamWriteThunk,
    (void *)DeleteType18ChunkStreamThunk,
    (void *)ResetType18ChunkStreamObjectsThunk,
    (void *)PrepareType18ChunkStreamObjectsThunk,
    (void *)ReturnFalseChunkStreamCommandThunk,
    (void *)EmitType18ChunkStreamEntriesThunk,
};

// FUNCTION: LEMBALL 0x0040C750
LEVEL_Type18ChunkStream *LEMBALL_FASTCALL ConstructType18ChunkStream(
    LEVEL_Type18ChunkStream *pStream)
{
    int i;

    pStream->m_pVtable =
        (GAME_EffStreamVtable *)g_GAME_EffStreamConstructionVtable;
    pStream->m_nEventCode = 0x1e;
    pStream->ResetStateFields();
    pStream->m_nU32Payload = 0x13;
    pStream->m_pVtable =
        (GAME_EffStreamVtable *)g_LEVEL_EffChunkStreamBaseVtable;
    if (g_pActiveNetworkRuntimeWindow != 0) {
        pStream->m_fHasPayload = 1;
        pStream->m_cbSerializedLength += g_cbEffTransportMaxPacketBytes;
    }
    pStream->m_pVtable =
        (GAME_EffStreamVtable *)g_LEVEL_Type18ChunkStreamVtable;
    pStream->m_cObjects50 = 0;
    for (i = 0; i < 8; ++i) {
        pStream->m_apObjects30[i] = 0;
    }
    return pStream;
}

// FUNCTION: LEMBALL 0x00402734
LEVEL_Type18ChunkStream *LEMBALL_FASTCALL ConstructType18ChunkStreamThunk(
    LEVEL_Type18ChunkStream *pStream)
{
    return ConstructType18ChunkStream(pStream);
}

// FUNCTION: LEMBALL 0x0040C7B0
void LEMBALL_FASTCALL ResetType18ChunkStreamObjects(
    LEVEL_Type18ChunkStream *pStream)
{
    LEVEL_Type18ChunkObject **ppObject;
    int i;

    i = 0;
    if (0 < pStream->m_cObjects50) {
        ppObject = pStream->m_apObjects30;
        do {
            (*ppObject)->Reset();
            ++ppObject;
            ++i;
        } while (i < pStream->m_cObjects50);
    }
}

// FUNCTION: LEMBALL 0x0040C7E0
void LEMBALL_FASTCALL DestroyType18ChunkStream(
    LEVEL_Type18ChunkStream *pStream)
{
    LEVEL_Type18ChunkObject **ppObject;
    int i;

    ppObject = pStream->m_apObjects30;
    pStream->m_pVtable =
        (GAME_EffStreamVtable *)g_LEVEL_Type18ChunkStreamVtable;
    i = 8;
    do {
        if (*ppObject != 0) {
            (*ppObject)->Delete(1);
        }
        ++ppObject;
        --i;
    } while (i != 0);
    ((NETWORK_EffStreamBase *)pStream)->DestroyEffStreamBase();
}

// FUNCTION: LEMBALL 0x0040222A
void LEMBALL_FASTCALL DestroyType18ChunkStreamThunk(
    LEVEL_Type18ChunkStream *pStream)
{
    DestroyType18ChunkStream(pStream);
}

// FUNCTION: LEMBALL 0x0040CED0
void *LEMBALL_FASTCALL DeleteType18ChunkStream(
    LEVEL_Type18ChunkStream *pStream, void *pUnused, BYTE fDelete)
{
    (void)pUnused;
    DestroyType18ChunkStreamThunk(pStream);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pStream);
    }
    return pStream;
}

// FUNCTION: LEMBALL 0x0040C8F0
void LEMBALL_FASTCALL PrepareType18ChunkStreamObjects(
    LEVEL_Type18ChunkStream *pStream)
{
    int i;

    if (pStream->m_cObjects50 != 0) {
        i = 0;
        if (0 < pStream->m_cObjects50) {
            do {
                pStream->m_apObjects30[i]->m_fPrepare124 = 1;
                if (pStream->m_apObjects30[i]->m_fPending148 != 0 &&
                    pStream->m_apObjects30[i]->TestPreparedState() == 0) {
                    pStream->m_apObjects30[i]->m_fPending148 = 0;
                }
                ++i;
            } while (i < pStream->m_cObjects50);
        }
    }
}

// FUNCTION: LEMBALL 0x0040C890
int LEVEL_Type18ChunkStream::EmitEntries(
    LEVEL_Type18ChunkStreamEntry *pEntry)
{
    int cEntries;
    int i;
    LEVEL_Type18ChunkObject **ppObject;

    cEntries = 0;
    i = 0;
    if (0 < m_cObjects50) {
        ppObject = m_apObjects30;
        do {
            if ((*ppObject)->m_nStateB8 != 0x18 &&
                (*ppObject)->m_nStateB8 != 0x1e) {
                (*ppObject)->EmitRenderEntry(pEntry++);
                ++cEntries;
            }
            ++ppObject;
            ++i;
        } while (i < m_cObjects50);
    }
    return cEntries;
}

// FUNCTION: LEMBALL 0x00401794
unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(
    int nManagedEntityObject)
{
    return GetManagedEntitySlotId(nManagedEntityObject);
}

// FUNCTION: LEMBALL 0x0040AED0
void LEMBALL_FASTCALL DispatchType18ChunkStreamCommands(
    LEVEL_Type18ChunkStream *pStream)
{
    LEVEL_Type18ChunkObject *pObject;
    LEVEL_Type18ChunkObject *pCandidate;
    unsigned int i;
    unsigned short nCommand;
    unsigned short nSlotId;

    nCommand = ((NETWORK_EffStreamBase *)pStream)
        ->ReadEffStreamU16BEValue();
    while (nCommand != 0x2f) {
        nSlotId = ((NETWORK_EffStreamBase *)pStream)
            ->ReadEffStreamU16BEValue();
        pObject = 0;
        i = 0;
        if (0 < g_GAME_ManagedEntityRegistryCount) {
            do {
                pCandidate = (LEVEL_Type18ChunkObject *)(unsigned long)
                    g_GAME_ManagedEntityRegistryTable[(unsigned short)i];
                if (pCandidate != 0 &&
                    GetManagedEntitySlotIdThunk(
                        (int)(unsigned long)pCandidate) == nSlotId) {
                    pObject = pCandidate;
                    break;
                }
                ++i;
            } while ((int)i <
                     (int)(unsigned int)g_GAME_ManagedEntityRegistryCount);
        }
        if (pObject->HandleChunkStreamCommand(nCommand, pStream) == 0) {
            ((LEVEL_EffChunkStreamVirtualInterface *)pStream)
                ->HandleUnhandledChunkStreamCommand(
                    nCommand, pObject, pStream);
        }
        nCommand = ((NETWORK_EffStreamBase *)pStream)
            ->ReadEffStreamU16BEValue();
    }
}
