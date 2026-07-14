#ifndef LEMBALL_NETWORK_SAFE_VTABLE_H
#define LEMBALL_NETWORK_SAFE_VTABLE_H

/* Compiler-owned emergency dispatch table.  Used only while original
 * read-only tables remain unrecovered.  Every slot is valid, so partial
 * reconstruction cannot turn an indirect call into address zero. */
struct VsNetSafeVtableModel {
    virtual void *Slot00(void) { return this; }
    virtual void *Slot01(void) { return this; }
    virtual void *Slot02(void) { return this; }
    virtual void *Slot03(void) { return this; }
    virtual void *Slot04(void) { return this; }
    virtual void *Slot05(void) { return this; }
    virtual void *Slot06(void) { return this; }
    virtual void *Slot07(void) { return this; }
    virtual void *Slot08(void) { return this; }
    virtual void *Slot09(void) { return this; }
    virtual void *Slot10(void) { return this; }
    virtual void *Slot11(void) { return this; }
    virtual void *Slot12(void) { return this; }
    virtual void *Slot13(void) { return this; }
    virtual void *Slot14(void) { return this; }
    virtual void *Slot15(void) { return this; }
    virtual void *Slot16(void) { return this; }
    virtual void *Slot17(void) { return this; }
    virtual void *Slot18(void) { return this; }
    virtual void *Slot19(void) { return this; }
    virtual void *Slot20(void) { return this; }
    virtual void *Slot21(void) { return this; }
    virtual void *Slot22(void) { return this; }
    virtual void *Slot23(void) { return this; }
    virtual void *Slot24(void) { return this; }
    virtual void *Slot25(void) { return this; }
    virtual void *Slot26(void) { return this; }
    virtual void *Slot27(void) { return this; }
    virtual void *Slot28(void) { return this; }
    virtual void *Slot29(void) { return this; }
    virtual void *Slot30(void) { return this; }
    virtual void *Slot31(void) { return this; }
};

static void NetworkSafeVtableNoop(void *pObject) {
    (void)pObject;
}

static void *NetworkGetSafeVtable(void) {
    static VsNetSafeVtableModel model;
    return *(void ***)&model;
}

#endif
