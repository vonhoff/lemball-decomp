"""Project-specific comparison compatibility for reccmp 0.1.6."""

import struct

from reccmp.compare import functions
from reccmp.compare.core import Compare
from reccmp.compare.db import EntityDb
from reccmp.compare.variables import VariableComparator
from reccmp.cvdump.symbols import CvdumpSymbolsParser
from reccmp.formats.exceptions import InvalidVirtualReadError
from reccmp.types import ImageId


if "S_GDATA32" not in CvdumpSymbolsParser._unhandled_symbols:
    CvdumpSymbolsParser._unhandled_symbols.append("S_GDATA32")


ILT_START = 0x00401000
ILT_LAST_ENTRY = 0x0040388C
# These original vtable entries are linker thunks to reconstructed virtual
# methods, while manual compatibility tables still need the thunk identities.
VTABLE_METHOD_ILTS = {
    0x00401069,  # ManagedEntityPacket23::ReverseEffStreamPayload
    0x00401217,  # ManagedEntityPacket25::ReverseEffStreamPayload
    0x00401316,  # ManagedEntityPacket28 scalar deleting destructor
    0x00401451,  # VsNetEffStreamCommon::NoopEffStream
    0x00401564,  # VsNetEffStreamCommon::AcceptEffStreamArgument
    0x00401573,  # ManagedEntityPacket23::WriteEffStreamTaggedHeader
    0x00401730,  # ManagedEntityPacketBase::WriteEffStreamTaggedHeader
    0x00401852,  # ManagedEntityPacket25 scalar deleting destructor
    0x004018C5,  # ManagedEntityStateResetPacket scalar deleting destructor
    0x00401C94,  # ManagedEntityPacket25::WriteEffStreamTaggedHeader
    0x00401DB6,  # ManagedEntityPacket23 scalar deleting destructor
    0x00401E15,  # ManagedEntityPacket24::ReverseEffStreamPayload
    0x00401E7E,  # ManagedEntityPacket28::WriteEffStreamTaggedHeader
    0x0040217B,  # ManagedEntityPacketBase::ReverseEffStreamPayload
    0x00402338,  # ManagedEntityPacket27::ReverseEffStreamPayload
    0x004025B3,  # ManagedEntityPacket26::ReverseEffStreamPayload
    0x004026AD,  # VsNetEffStreamCommon::AcceptEffStream
    0x00402A3B,  # ManagedEntityPacket24::WriteEffStreamTaggedHeader
    0x00402CE3,  # ManagedEntityPacket27::WriteEffStreamTaggedHeader
    0x00402DB0,  # ManagedEntityPacket26 scalar deleting destructor
    0x00402E41,  # ManagedEntityPacket26::WriteEffStreamTaggedHeader
    0x0040308F,  # ManagedEntityPacket27 scalar deleting destructor
    0x004030E4,  # ManagedEntityPacketBase scalar deleting destructor
    0x00403288,  # ManagedEntityPacket2A scalar deleting destructor
    0x0040335A,  # ManagedEntityPacket28::ReverseEffStreamPayload
    0x00403553,  # ManagedEntityPacket24 scalar deleting destructor
}
# These table-specific ILTs target byte-identical callback implementations.
# Keep aliases restricted to independently verified original ILT addresses and
# rebuilt callback symbols rather than conflating their function identities.
VARIABLE_ILT_DESTINATION_ALIASES = {
    0x0040150A: "DeleteStatusPointSinkVariantThunk",
    0x0040353A: "AppendStatusPointSinkToQueueThunk",
}
# Original and rebuilt binaries place different unrelated symbols at these
# one-past array addresses. Limit normalization to independently verified
# functions that compare against those boundaries.
ONE_PAST_REFERENCES = {
    # function: (original boundary, rebuilt successor, shared identity)
    0x004595D0: (
        0x004A0F20,
        "g_fSubsystemsReady",
        "g_aCommandLineOptions+112 (OFFSET)",
    ),
}
# Constructor vtable stores whose original and rebuilt data objects have
# independently proven identities but no shared source annotation.
FUNCTION_DATA_REFERENCES = {
    # function: (original data, rebuilt symbol, shared identity)
    0x004168F0: (
        0x0049CE08,
        "g_nLevelFrameClockTimeMs",
        "level frame clock time 0049ce08",
    ),
    0x0044C830: (
        0x00497E30,
        "g_MenuSelectorChildContextVtable",
        "menu selector child-context vtable 00497e30",
    ),
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
    0x0045A480: (
        (0x00498918, "g_MainMemoryArenaVtable", "main memory arena vtable 00498918"),
        (0x00498910, "g_aMainMemoryArenaLockVtable", "main memory arena lock vtable 00498910"),
    ),
    0x0045FCE0: (
        0x00498FB0,
        "vftable'{for `VsNetTimedEffStreamPrimary",
        "timed EFF stream primary vtable 00498fb0",
    ),
    0x004664B0: (
        (
            0x004995B0,
            "g_VSGDI_ResourceGeometryHelperGroup0Vtable",
            "resource geometry helper group-0 vtable 004995b0",
        ),
        (
            0x00499560,
            "g_VSGDI_ResourceGeometryHelperGroup0CompactSubobjectVtable",
            "resource geometry helper group-0 compact-subobject vtable 00499560",
        ),
    ),
    0x004666E0: (
        (
            0x00499618,
            "g_VSGDI_ResourceGeometryHelperGroup1Vtable",
            "resource geometry helper group-1 vtable 00499618",
        ),
        (
            0x004995C8,
            "g_VSGDI_ResourceGeometryHelperGroup1CompactSubobjectVtable",
            "resource geometry helper group-1 compact-subobject vtable 004995c8",
        ),
    ),
    0x0045FD80: (
        0x00498FB0,
        "vftable'{for `VsNetTimedEffStreamPrimary",
        "timed EFF stream primary vtable 00498fb0",
    ),
    0x0046BC90: (
        0x00499D78,
        "VsGdiResourceGeometryHelperSlotManager::`vftable'",
        "resource geometry helper slot-manager vtable 00499d78",
    ),
    0x0046DE70: (
        0x00499F30,
        "g_VSGDI_AnimWindowOwnerVtable",
        "animation window-owner vtable 00499f30",
    ),
    0x0046FD70: (
        0x0049A060,
        "VsNetSocketWindowEffChannel::`vftable'{for `VsNetSocketWindowPrimary'}",
        "socket-window primary vtable 0049a060",
    ),
    0x00470030: (
        0x0049A090,
        "VsNetTimedSocketEffChannelBundleLayout::`vftable'{for `VsNetTimedEffStreamPrimary'}",
        "timed socket bundle primary vtable 0049a090",
    ),
}
# These real, source-owned adjustor thunks jump through original LINK 3.00
# ILT entries while rebuilt naked thunks jump directly to the same bodies.
# Normalize only the proven relocation in each listed function.
THUNK_ILT_REFERENCES = {
    # function: (original ILT entry, rebuilt destination, shared identity)
    0x00416940: (
        0x00403107,
        "UpdateNetworkFrameClockFromNetworkTime",
        "network frame clock update target 00408190",
    ),
    0x00408040: (0x00401370, "DeleteDerivedBaseModeWrapperAuto", "adjustor target 00407fd0"),
    0x00413E60: (0x00403616, "DeleteLevelGameMode", "adjustor target 00413e30"),
    0x00413E70: (0x00403616, "DeleteLevelGameModeAuto", "adjustor target 00413e30"),
    0x00414010: (
        0x00402103,
        "LevelChunkObjectBaseView::InitializeLevelChunkObjectBase",
        "level chunk object base initializer 0041dda0",
    ),
    0x00420AE0: (
        0x00402103,
        "LevelChunkObjectBaseView::InitializeLevelChunkObjectBase",
        "level chunk object base initializer 0041dda0",
    ),
    0x00416CC0: (
        0x00402F22,
        "LevelVtSmallFunctionView::AddLevelScoreClamped",
        "level score clamp target 00413390",
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
        "CHand::~CHand",
        "CHand destructor target 00427b40",
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


def _thunk_destination(db, image_id, address, get=EntityDb.get):
    entity = get(db, image_id, address)
    if entity is None:
        return None
    key = "ref_orig" if image_id == ImageId.ORIG else "ref_recomp"
    destination = entity.get(key)
    return destination if isinstance(destination, int) else None


def _logical_thunk_name(entity):
    if entity is None:
        return None
    name = entity.best_name() or ""
    if name.startswith("Thunk of '") and name.endswith("'"):
        name = name[10:-1]
    return name.split("`vtordisp{", 1)[0]


def _matched_thunk_target(db, image_id, address, get=EntityDb.get):
    entity = get(db, image_id, address)
    if entity is None:
        return None
    name = entity.best_name() or ""
    if not name.startswith("Thunk of '") or not name.endswith("'"):
        return None
    target_name = name[10:-1]
    for candidate in db.all(image_id):
        if candidate.matched and candidate.best_name() == target_name:
            return candidate
    return None


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
        data_references = FUNCTION_DATA_REFERENCES.get(match.orig_addr)
        if data_references is None:
            data_references = ()
        elif isinstance(data_references[0], int):
            data_references = (data_references,)
        orig_lookup = self.orig_sanitize.name_lookup
        recomp_lookup = self.recomp_sanitize.name_lookup

        def _orig_lookup(address, exact=False, indirect=False):
            if ilt_reference is not None and address == ilt_reference[0]:
                return ilt_reference[2]
            for data_reference in data_references:
                if address == data_reference[0]:
                    return data_reference[2]
            if not exact and not indirect:
                if reference is not None and address == reference[0]:
                    return reference[2]
            name = orig_lookup(address, exact=exact, indirect=indirect)
            if name is not None and name.endswith(" (IMPORT_THUNK)"):
                return name[:-15] + " (IMPORT)"
            return name

        def _recomp_lookup(address, exact=False, indirect=False):
            name = recomp_lookup(address, exact=exact, indirect=indirect)
            if reference is not None and not exact and not indirect and name is not None:
                if reference[1] in name:
                    return reference[2]
            if ilt_reference is not None and name is not None:
                if ilt_reference[1] in name:
                    return ilt_reference[2]
            if name is not None:
                for data_reference in data_references:
                    if data_reference[1] in name:
                        return data_reference[2]
            if not indirect:
                destination = _thunk_destination(self.db, ImageId.RECOMP, address)
                if destination is not None:
                    entity = self.db.get(ImageId.RECOMP, destination)
                    if entity is not None and entity.orig_addr is not None:
                        resolved_name = entity.match_name()
                        if resolved_name is not None:
                            return resolved_name
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

        expected_name = VARIABLE_ILT_DESTINATION_ALIASES.get(orig_addr)
        if expected_name is not None:
            entity = self.db.get(ImageId.RECOMP, recomp_addr)
            if entity is not None and expected_name in (entity.best_name() or ""):
                return True

        destination = _ilt_destination(self, orig_addr)
        if destination is not None and self.db.is_match(destination, recomp_addr):
            return True

        orig_entity = self.db.get(ImageId.ORIG, orig_addr)
        recomp_destination = _thunk_destination(self.db, ImageId.RECOMP, recomp_addr)
        if (
            orig_entity is not None
            and recomp_destination is not None
            and orig_entity.recomp_addr == recomp_destination
        ):
            return True

        orig_destination = _ilt_destination(self, orig_addr) or _thunk_destination(
            self.db, ImageId.ORIG, orig_addr
        )
        return self.db.is_match(
            orig_destination or orig_addr,
            recomp_destination or recomp_addr,
        )

    VariableComparator.is_pointer_match = _is_pointer_match
    VariableComparator._lemball_ilt_aware = True


if not getattr(Compare, "_lemball_ilt_aware_vtables", False):
    _reccmp_compare_vtable = Compare._compare_vtable

    def _compare_vtable(self, match):
        """Compare original ILT slots by their destination method identity."""
        reccmp_get = EntityDb.get
        size = match.any_size() & ~3
        orig_table = self.orig_bin.read(match.orig_addr, size)
        recomp_table = self.recomp_bin.read(match.recomp_addr, size)
        slot_aliases = {}
        for (orig_addr,), (recomp_addr,) in zip(
            struct.iter_unpack("<L", orig_table),
            struct.iter_unpack("<L", recomp_table),
        ):
            orig_entity = reccmp_get(self._db, ImageId.ORIG, orig_addr)
            recomp_entity = reccmp_get(self._db, ImageId.RECOMP, recomp_addr)
            if _logical_thunk_name(orig_entity) == _logical_thunk_name(recomp_entity):
                alias = None
                if orig_entity is not None and orig_entity.matched:
                    alias = orig_entity
                elif recomp_entity is not None and recomp_entity.matched:
                    alias = recomp_entity
                else:
                    alias = _matched_thunk_target(
                        self._db, ImageId.RECOMP, recomp_addr
                    )
                    if alias is None:
                        destination = _thunk_destination(
                            self._db, ImageId.RECOMP, recomp_addr
                        )
                        candidate = (
                            reccmp_get(self._db, ImageId.RECOMP, destination)
                            if destination is not None
                            else None
                        )
                        if candidate is not None and candidate.matched:
                            alias = candidate
                if alias is not None:
                    slot_aliases[(ImageId.ORIG, orig_addr)] = alias
                    slot_aliases[(ImageId.RECOMP, recomp_addr)] = alias

        def _get(db, image_id, address, *, exact=True):
            alias = slot_aliases.get((image_id, address))
            if alias is not None:
                return alias
            if image_id == ImageId.RECOMP:
                destination = _thunk_destination(db, image_id, address)
                if destination is not None:
                    entity = reccmp_get(db, image_id, destination, exact=exact)
                    if (
                        entity is not None
                        and entity.orig_addr is not None
                        and entity.recomp_addr is not None
                    ):
                        return entity

                target = _matched_thunk_target(db, image_id, address)
                if target is not None:
                    return target

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
