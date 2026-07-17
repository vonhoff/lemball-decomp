#include "ENGINE/GDI/VSWINDOW.H"

extern void *LEMBALL_FASTCALL DeleteGeometryOwnerRect(void *, int, int);
extern void LEMBALL_FASTCALL GeometryOwnerButtonTransitionNoop(
    void *, int, const VS_XYPair *, int);
extern void LEMBALL_FASTCALL GeometryOwnerButtonInsideNoop(
    void *, int, const VS_XYPair *, int);
extern void LEMBALL_FASTCALL ClearGeometryOwnerButtonState(
    void *, int, const VS_XYPair *, int);
extern void LEMBALL_FASTCALL GeometryOwnerPointerEnterNoop(void *);
extern void LEMBALL_FASTCALL ResetGeometryOwnerButtonsIfOutsideDispatchDisabled(
    void *);
extern void LEMBALL_FASTCALL GeometryOwnerPointerMoveNoop(
    void *, int, const VS_XYPair *);
extern int LEMBALL_FASTCALL GeometryOwnerContainsPoint(
    void *, int, const VS_XYPair *);

// ABI-only translation unit: moving these forwarding entries changes linker
// layout and breaks the reconstructed vtable addresses.

// FUNCTION: LEMBALL 0x004023a6
void LEMBALL_FASTCALL GeometryOwnerPointerEnterThunk(void *pOwner)
{
    GeometryOwnerPointerEnterNoop(pOwner);
}

// FUNCTION: LEMBALL 0x00402ad1
int LEMBALL_FASTCALL GeometryOwnerContainsPointThunk(
    void *pOwner, int nUnused, const VS_XYPair *pPoint)
{
    return GeometryOwnerContainsPoint(pOwner, nUnused, pPoint);
}

// FUNCTION: LEMBALL 0x00403300
void LEMBALL_FASTCALL GeometryOwnerPointerMoveThunk(
    void *pOwner, int nUnused, const VS_XYPair *pPoint)
{
    GeometryOwnerPointerMoveNoop(pOwner, nUnused, pPoint);
}

// FUNCTION: LEMBALL 0x00403751
void LEMBALL_FASTCALL ResetGeometryOwnerButtonsOutsideThunk(void *pOwner)
{
    ResetGeometryOwnerButtonsIfOutsideDispatchDisabled(pOwner);
}
