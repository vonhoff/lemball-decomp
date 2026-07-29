"""Project-specific comparison compatibility for reccmp 0.1.6."""

from reccmp.compare import functions
from reccmp.compare.core import Compare
from reccmp.compare.db import EntityDb
from reccmp.compare.variables import VariableComparator
from reccmp.formats.exceptions import InvalidVirtualReadError
from reccmp.types import ImageId


ILT_START = 0x00401000
ILT_LAST_ENTRY = 0x0040388C
# These original vtable entries are linker thunks to reconstructed virtual
# methods, while manual compatibility tables still need the thunk identities.
VTABLE_METHOD_ILTS = {
    0x00401451,  # VsNetEffStreamCommon::NoopEffStream
    0x00401564,  # VsNetEffStreamCommon::AcceptEffStreamArgument
    0x00401730,  # ManagedEntityPacketBase::WriteEffStreamTaggedHeader
    0x004018C5,  # ManagedEntityStateResetPacket scalar deleting destructor
    0x0040217B,  # ManagedEntityPacketBase::ReverseEffStreamPayload
    0x004026AD,  # VsNetEffStreamCommon::AcceptEffStream
    0x004030E4,  # ManagedEntityPacketBase scalar deleting destructor
    0x00403288,  # ManagedEntityPacket2A scalar deleting destructor
}
# Original and rebuilt binaries place different unrelated symbols at these
# one-past array addresses. Limit normalization to independently verified
# functions that compare against those boundaries.
ONE_PAST_REFERENCES = {
    # function: (original boundary, rebuilt successor, shared identity)
    0x004595D0: (
        0x004A0F20,
        "g_pStartupFixedBufferStream",
        "g_aCommandLineOptions+112 (OFFSET)",
    ),
}
# Constructor vtable stores whose original and rebuilt data objects have
# independently proven identities but no shared source annotation.
FUNCTION_DATA_REFERENCES = {
    # function: (original data, rebuilt symbol, shared identity)
    0x00452510: (
        0x004985B8,
        "g_EFF_NetworkLobbyPeerClearCloseVtable",
        "network lobby peer clear-close vtable 004985b8",
    ),
    0x00452530: (
        0x004985D8,
        "g_EFF_NetworkLobbyPeerDirtyConfirmVtable",
        "network lobby peer dirty-confirm vtable 004985d8",
    ),
    0x0046DE70: (
        0x00499F30,
        "g_VSGDI_AnimWindowOwnerVtable",
        "animation window-owner vtable 00499f30",
    ),
}
# These real, source-owned adjustor thunks jump through original LINK 3.00
# ILT entries while rebuilt naked thunks jump directly to the same bodies.
# Normalize only the proven relocation in each listed function.
THUNK_ILT_REFERENCES = {
    # function: (original ILT entry, rebuilt destination, shared identity)
    0x00408040: (0x00401370, "DeleteDerivedBaseModeWrapperAuto", "adjustor target 00407fd0"),
    0x00413E60: (0x00403616, "DeleteLevelGameMode", "adjustor target 00413e30"),
    0x00413E70: (0x00403616, "DeleteLevelGameModeAuto", "adjustor target 00413e30"),
    0x00414010: (
        0x00402103,
        "LevelChunkObjectBaseView::InitializeLevelChunkObjectBase",
        "level chunk object base initializer 0041dda0",
    ),
    0x0040AC60: (
        0x004016FE,
        "destroy_invs_chunk_object",
        "INVS chunk object destructor target 00409d10",
    ),
    0x00426710: (
        0x00401E0B,
        "destroy_lift_chunk_object",
        "LIFT chunk object destructor target 00424d20",
    ),
    0x00427520: (
        0x00402FD1,
        "destroy_rock_chunk_object",
        "ROCK chunk object destructor target 00426830",
    ),
    0x00428800: (
        0x004018C0,
        "destroy_capture_trigger_chunk_object",
        "CAPT chunk object destructor target 00427b40",
    ),
    0x00429ED0: (
        0x004034D6,
        "destroy_lasr_chunk_object_vtable_thunk",
        "LASR chunk object destructor target 004288f0",
    ),
    0x0042C9E0: (
        0x00402E96,
        "destroy_pgun_chunk_object_vtable_thunk",
        "PGUN chunk object destructor target 0042bb20",
    ),
    0x0042FBD0: (
        0x00401726,
        "destroy_move_chunk_object",
        "MOVE chunk object destructor target 0042e640",
    ),
    0x00439850: (0x00403521, "DeleteReconstructedLevelScreenAuto", "adjustor target 00439490"),
    0x004472E0: (0x00401E1F, "DeleteBaseModeObjectAuto", "adjustor target 004472b0"),
    0x0044EFF0: (
        0x00401181,
        "DestroySelectorProgressOverlayChild",
        "deleting wrapper target 0044e8c0",
    ),
    0x0044F030: (
        0x00402AAE,
        "DestroySelectorProgressOverlayOwner",
        "deleting wrapper target 0044ed20",
    ),
    0x00452510: (
        0x00402810,
        "GameEffStream::ConstructNetworkLobbyU32PayloadStream",
        "network lobby U32 stream constructor 004524b0",
    ),
    0x00452530: (
        0x00402810,
        "GameEffStream::ConstructNetworkLobbyU32PayloadStream",
        "network lobby U32 stream constructor 004524b0",
    ),
    0x004551D0: (
        0x0040148D,
        "VsNetNetworkLobbyScreen::ClearPlayerEntryDirty",
        "lobby entry update target 00454b70",
    ),
    0x004491A0: (0x0040386E, "DeleteOptionsScreen", "adjustor target 00449170"),
    0x0044AA00: (0x00401BE5, "DeleteLevelSelectionModeThunk", "adjustor target 0044a9d0"),
    0x0044AA10: (0x004019B0, "DeleteLevelSelectionScreenAuto", "adjustor target 0044a940"),
    0x0044C1D0: (0x004023F1, "DeleteRegistrationInfoScreenAuto", "adjustor target 0044c0d0"),
    0x00451200: (0x00401B40, "DeletePaintballSequenceScreen", "adjustor target 004510e0"),
    0x004523A0: (0x00401122, "DeletePasswordEntryScreen", "adjustor target 00452370"),
    0x00455EE0: (0x004037C9, "DeleteNetworkLobbyScreen", "adjustor target 00455de0"),
    0x00455EF0: (
        0x004027E3,
        "DeleteNetworkLobbyTransportController",
        "adjustor target 00455eb0",
    ),
}


def _ilt_destination(comparator, address):
    if address < ILT_START or ILT_LAST_ENTRY < address:
        return None

    try:
        instruction = comparator.orig_bin.read(address, 5)
    except InvalidVirtualReadError:
        return None

    if len(instruction) != 5 or instruction[0] != 0xE9:
        return None

    displacement = int.from_bytes(instruction[1:5], byteorder="little", signed=True)
    return address + 5 + displacement


if not getattr(functions.FunctionComparator, "_lemball_relocation_aware", False):
    _reccmp_compare_function = functions.FunctionComparator.compare_function

    def _compare_function(self, match):
        reference = ONE_PAST_REFERENCES.get(match.orig_addr)
        ilt_reference = THUNK_ILT_REFERENCES.get(match.orig_addr)
        data_reference = FUNCTION_DATA_REFERENCES.get(match.orig_addr)
        if reference is None and ilt_reference is None and data_reference is None:
            return _reccmp_compare_function(self, match)

        orig_lookup = self.orig_sanitize.name_lookup
        recomp_lookup = self.recomp_sanitize.name_lookup

        def _orig_lookup(address, exact=False, indirect=False):
            if ilt_reference is not None and address == ilt_reference[0]:
                return ilt_reference[2]
            if data_reference is not None and address == data_reference[0]:
                return data_reference[2]
            if not exact and not indirect:
                if reference is not None and address == reference[0]:
                    return reference[2]
            return orig_lookup(address, exact=exact, indirect=indirect)

        def _recomp_lookup(address, exact=False, indirect=False):
            name = recomp_lookup(address, exact=exact, indirect=indirect)
            if reference is not None and not exact and not indirect and name is not None:
                if reference[1] in name:
                    return reference[2]
            if ilt_reference is not None and name is not None:
                if ilt_reference[1] in name:
                    return ilt_reference[2]
            if data_reference is not None and name is not None:
                if data_reference[1] in name:
                    return data_reference[2]
            return name

        self.orig_sanitize.name_lookup = _orig_lookup
        self.recomp_sanitize.name_lookup = _recomp_lookup
        try:
            return _reccmp_compare_function(self, match)
        finally:
            self.orig_sanitize.name_lookup = orig_lookup
            self.recomp_sanitize.name_lookup = recomp_lookup

    functions.FunctionComparator.compare_function = _compare_function
    functions.FunctionComparator._lemball_relocation_aware = True


if not getattr(VariableComparator, "_lemball_ilt_aware", False):
    _reccmp_is_pointer_match = VariableComparator.is_pointer_match

    def _is_pointer_match(self, orig_addr, recomp_addr):
        if _reccmp_is_pointer_match(self, orig_addr, recomp_addr):
            return True

        destination = _ilt_destination(self, orig_addr)
        return destination is not None and self.db.is_match(destination, recomp_addr)

    VariableComparator.is_pointer_match = _is_pointer_match
    VariableComparator._lemball_ilt_aware = True


if not getattr(Compare, "_lemball_ilt_aware_vtables", False):
    _reccmp_compare_vtable = Compare._compare_vtable

    def _compare_vtable(self, match):
        """Compare original ILT slots by their destination method identity."""
        reccmp_get = EntityDb.get

        def _get(db, image_id, address, *, exact=True):
            if image_id == ImageId.ORIG and address in VTABLE_METHOD_ILTS:
                destination = _ilt_destination(self, address)
                if destination is not None:
                    entity = reccmp_get(db, image_id, destination, exact=exact)
                    if entity is not None and entity.recomp_addr is not None:
                        return entity

            return reccmp_get(db, image_id, address, exact=exact)

        EntityDb.get = _get
        try:
            return _reccmp_compare_vtable(self, match)
        finally:
            EntityDb.get = reccmp_get

    Compare._compare_vtable = _compare_vtable
    Compare._lemball_ilt_aware_vtables = True
