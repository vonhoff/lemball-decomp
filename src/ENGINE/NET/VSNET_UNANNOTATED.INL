/* Emergency transport callbacks used until the remaining vtables are recovered. */
static void NetworkRuntimeTransportUnavailable(void *pObject) {
    *(int *)((char *)pObject + 0x14) = 1;
    g_nEffTransportAsyncErrorStatus = 1;
}

static void NetworkRuntimeTransportNoop(void *pObject) {
    (void)pObject;
}

static int NetworkRuntimeCreateNoChannelOwner(void *pObject) {
    (void)pObject;
    return 0;
}

static void *NetworkRuntimeWindowCreateNoChannelOwner(void *pObject) {
    (void)pObject;
    return 0;
}

void VsNetCompositeEffTransportVtableModel::FatalSlot00(void) {
    _purecall();
}

void VsNetCompositeEffTransportVtableModel::FatalSlot02(void) {
    _purecall();
}

void VsNetCompositeEffTransportVtableModel::FatalSlot03(void) {
    _purecall();
}

void VsNetEffTransportRuntimeState::FallbackInitializeTransport(void) {
    m_fShutdownRequested = 1;
    g_nEffTransportAsyncErrorStatus = 1;
}

void VsNetEffTransportRuntimeState::FallbackServiceTransport(void) {
}

void *VsNetEffTransportRuntimeState::FallbackCreateChannelOwner(void) {
    return 0;
}

void VsNetEffTransportRuntimeState::FallbackWaitForTransportMessage(void) {
}

int VsNetEffTransportRuntimeState::FallbackRuntimeStateSlot(void) {
    return 0;
}

void *VsNetEffTransportRuntimeState::CreateFileNetworkAddress(void) {
    return AllocateFileNetworkAddress();
}

void *VsNetEffTransportRuntimeState::CreateTcpipChannelOwner(void) {
    return AllocateTcpipEffTransportRuntimeOwner();
}

void *VsNetEffTransportRuntimeState::CreateTcpipSecondaryOwner(void) {
    return AllocateTcpipEffTransportComposite();
}
