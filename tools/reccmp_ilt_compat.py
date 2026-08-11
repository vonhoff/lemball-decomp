"""Project-specific comparison compatibility for reccmp 0.1.6."""

import struct

from reccmp.compare import functions
from reccmp.compare.core import Compare
from reccmp.compare.db import EntityDb
from reccmp.compare.diff import EntityCompareResult, RawDiffOutput
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
    0x0041E290: (
        0x004A7820,
        "g_pGenericGroupFormationManager",
        "generic-group formation manager 004a7820",
    ),
    0x004232E0: (
        (0x00495B80, "g_afMazeNeighborEnabled", "maze neighbor-enabled table 00495b80"),
        (0x00495BA8, "g_abMazeEdgeMaskA", "maze edge-mask table A 00495ba8"),
        (0x00495B30, "g_anMazeNeighborX", "maze neighbor-X table 00495b30"),
        (0x00495B58, "g_anMazeNeighborY", "maze neighbor-Y table 00495b58"),
    ),
    0x0043C7F0: (
        0x004A7874,
        "g_nLevelScreenTimedVariantResourceId",
        "C2D DrawHand timed variant resource 004a7874",
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
    0x00459AA0: (
        (0x004988E0, "g_MemoryArenaBaseStateVtable", "memory arena base-state vtable 004988e0"),
        (0x004988D8, "g_aMainMemoryArenaLockVtable", "main memory arena lock vtable 004988d8"),
    ),
    0x00445420: (
        (0x00496D48, "g_GAME_FramedScreenBaseVtable", "framed-screen base vtable 00496d48"),
        (0x00496CE4, "g_MenuSelectorCurrentFrameCallbackVtable", "composite point-rect sink init[7] vtable 00496ce4"),
        (0x00496D00, "g_MenuSelectorCurrentFrameCallbackVtable", "composite point-rect sink init vtable 00496d00"),
    ),
    0x0040B020: (0x004932E8, "g_LINKSCF_LevelChunkStreamDispatcherVtable", "level-chunk stream dispatcher vtable 004932e8"),
    0x0045A480: (
        (0x00498918, "g_MainMemoryArenaVtable", "main memory arena vtable 00498918"),
        (0x00498910, "g_aMainMemoryArenaLockVtable", "main memory arena lock vtable 00498910"),
    ),
    0x0045FCE0: (
        0x00498FB0,
        "CWriteSocket::`vftable'{for `CWriteSocketPrimary'}",
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
        "CGDIDevice::`vftable'",
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
        "VsNetTimedSocketEffChannelBundleLayout::`vftable'{for `CWriteSocketPrimary'}",
        "timed socket bundle primary vtable 0049a090",
    ),
}
# These real, source-owned adjustor thunks jump through original LINK 3.00
# ILT entries while rebuilt naked thunks jump directly to the same bodies.
# Normalize only the proven relocation in each listed function.
THUNK_ILT_REFERENCES = {
    # function: (original ILT entry, rebuilt destination, shared identity)
    0x0042BBE0: (
        (0x0040161D, "AICOORD::AICOORD", "AICOORD copy target 00410b50"),
        (0x00402EFF, "PgunProjectileSpawnerView::FireBullet", "projectile spawn target 00412600"),
    ),
    0x0043C610: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043C660: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043C9F0: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043C7F0: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043CA30: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043CAD0: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043C6E0: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043CC10: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043CCF0: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043CD80: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043CE30: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043CED0: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043CFC0: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043D070: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043D270: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043D420: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043D500: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043D8D0: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043D0F0: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043D4B0: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0043D950: (0x004016A9, "CAnimsManagerView::EmitLevelScreenVariantEntry", "level-screen variant emitter 00433fc0"),
    0x0042CD70: (
        (0x00401F0F, "ReleaseIceChunkObjectEntities", "ice switched target 0042d560"),
        (0x004029A5, "CGround::GetZThunk", "ground height target 0042ffe0"),
        (0x0040341D, "IceContactDispatcherView::StepOn", "managed entity contact target 00412890"),
    ),
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
        "CInvisibleSwitch::~CInvisibleSwitch",
        "INVS chunk object destructor target 00409d10",
    ),
    0x0042BA40: (0x00402D10, "DestroyCTrampoline", "tram chunk object destructor target 0x42a9d0"),
    0x0041DE80: (0x0040209F, "CGameObject::Restart", "reset runtime state thunk -> CGameObject::Restart 0x4150d0"),
    0x004278C0: (0x0040372E, "nDead", "has_level_plas_history_entries thunk -> 0x413370"),
    0x004193F0: (0x00401302, "CPlayerLemmingGroup::HasSFXChanged", "has-stream-dirty-children thunk -> 0x414960"),
    0x00418840: (0x0040268A, "CGenericGroupManager::GetGroupElementIsMemberOf", "find-group-element thunk -> 0x41eb90"),
    0x00415780: (0x00402351, "CGameObject::DeleteFirstEntryFromDestinationList", "pop-first-move-command thunk -> 0x415e80"),
    0x00423C30: (0x0040146A, "CMine::Initialise", "reset-mine-runtime-state thunk -> CMine::Initialise 0x423c50"),
    0x00418040: (
        (0x00401A0A, "CBulletManager::GetFirstBullet", "get-first-projectile thunk -> 0x417f40"),
        (0x004012A3, "CBulletManager::RemoveObject", "projectile-pool remove thunk -> 0x418080"),
        (0x0040263A, "CBulletManager::GetNextBullet", "get-next-projectile thunk -> 0x417f50"),
    ),
    0x00418120: (0x00401A0A, "CBulletManager::GetFirstBullet", "get-first-projectile thunk -> 0x417f40"),
    0x00418080: (0x0040339B, "CBullet::Free", "deactivate-projectile thunk -> 0x41ac70"),
    0x00413020: (0x00402C5C, "CLiftManager::Id", "lift-slot-id thunk -> 0x425f90"),
    0x0040A370: (0x00402CB6, "CInvisibleSwitch::StepOn", "invs-step-on thunk -> 0x409fa0"),
    0x0040B9E0: (0x00402892, "CSlinky::Set", "SLNK chunk object setter target 0x40b4d0"),
    0x00412600: (0x00401BF9, "CBulletManager::RequestBullet", "projectile request target 0x417fa0"),
    0x00417FA0: (
        (0x00401208, "CBulletManager::GetFreeProjectilePoolObject", "free projectile target 0x417ee0"),
        (0x004036E8, "CBullet::Set", "projectile setter target 0x41a5c0"),
        (0x00402C2A, "CBullet::FireBullet", "projectile activation target 0x41a760"),
    ),
    0x0041CCC0: (0x00402CAC, "CObjectManager::AddObject", "managed-object add target 0x41b2a0"),
    0x00420650: (0x00401532, "ReturnFacingDirection", "direction-octant target 0x413e80"),
    0x004270B0: (0x00402707, "CRocket::Set", "ROCK chunk object setter target 0x426840"),
    0x00452B90: (
        0x00402A9A,
        "FindNetworkLobbyPeerSlotByPeer",
        "network lobby peer-slot lookup target 0x452bf0",
    ),
    0x0042E500: (0x00402C4D, "DestroyCIce", "ice chunk object destructor target 0x42caf0"),
    0x00416620: (0x004012DF, "CGameObject::RegisterId", "claim-managed-entity-slot-id thunk -> 0x416740"),
    0x00424560: (0x0040371F, "CMineManager::Trigger", "mine-trigger thunk -> 0x424580"),
    0x004150D0: (0x0040243C, "CGameObject::Initialise", "clear-state-fields thunk -> CGameObject::Initialise 0x4151b0"),
    0x00407950: (
        (0x00403350, "ServiceLevelDemoPlayback", "demo service thunk -> 0x409620"),
        (0x00402D92, "UpdateMainGameActiveMode", "update-active-mode thunk -> 0x4077e0"),
        (0x00403184, "PresentMainGameFrame", "present-frame thunk -> 0x4078f0"),
    ),
    0x004304E0: (0x00402B26, "CMoverManager::Find", "find-move-chunk-containing-point fwd 0x402b26->0x4130d0->0x42f2f0"),
    0x0042B5A0: (0x00402450, "CTrampoline::Set", "seed-tram-chunk-object-at-point thunk -> CTrampoline::Set 0x42a9e0"),
    0x0042B600: (0x00403134, "CTrampolineManager::Add", "append-tram-chunk-object thunk -> CTrampolineManager::Add 0x42b5a0"),
    0x0042C590: (0x00402973, "CPaintGun::Set", "seed-pgun-chunk-object-at-point thunk -> CPaintGun::Set 0x42bb30"),
    0x0042C610: (0x004031B6, "CPaintGunManager::Add", "append-pgun-chunk-object thunk -> CPaintGunManager::Add 0x42c590"),
    0x0040E630: (0x00402405, "CDoorManager::Add", "append-door-chunk-object thunk -> CDoorManager::Add 0x40e0c0"),
    0x00440460: (0x004033EB, "ClearLevelScreenStaticRegionPending", "level-screen render-dispatch thunk -> ClearLevelScreenStaticRegionPending 0x4033eb"),
    0x0042CB00: (
        (0x00402293, "CGameObject::SetId", "managed-entity slot setter -> CGameObject::SetId 0x416620"),
        (0x004029A5, "CGround::GetZThunk", "ground height target 0x42ffe0"),
    ),
    0x0042D380: (0x00402446, "CGameObject::ResetInstructions", "clear pending instructions target 0x415240"),
    0x0042D4D0: (0x00401794, "CGameObject::GetId", "managed-entity slot getter target 0x416600"),
    0x0042DDF0: (0x00402A95, "CIce::Set", "initialize ICE chunk object -> CIce::Set 0x42cb00"),
    0x0042DEA0: (
        (0x00401E5B, "CIceManager::Initialise", "allocate ICE chunk array -> CIceManager::Initialise 0x42d830"),
        (0x0040214E, "CGameObject::NextId", "next managed-entity slot id -> CGameObject::NextId 0x4166a0"),
        (0x00402EFA, "CIceManager::Add", "append ICE chunk object -> CIceManager::Add 0x42ddf0"),
    ),
    0x00419980: (0x00401500, "CGameObject::UpdateCollision", "update-managed-entity-bounds thunk -> 0x4167c0"),
    0x004071D0: (
        (0x004012A8, "DestroyMainGameVariantResourceMode", "destroy-variant-resource-mode thunk -> 0x447c50"),
        (0x00402DB5, "UnregisterNetworkLobbyVsnetRuntimeFromTransport", "unregister-lobby-vsnet thunk -> 0x4527c0"),
    ),
    0x00445420: (0x004036ED, "ConstructFramedScreenRenderEntryGroup", "framed-screen entry-group ctor thunk -> 0x447030"),
    0x00426710: (
        0x00401E0B,
        "destroy_lift_chunk_object",
        "LIFT chunk object destructor target 00424d20",
    ),
    0x00427520: (
        0x00402FD1,
        "DestroyCRocket",
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
        "DestroyCPaintGun",
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
    0x0042F2F0: (
        0x0040120D,
        "CMover::IsAt",
        "CMover IsAt containment/height-sample 0042e700",
    ),
    0x00423EB0: (
        0x004016B8,
        "CMine::SetTerrain",
        "mine terrain variant selector 00423dd0",
    ),
    0x00415D90: (
        (0x0040336E, "CGameObject::GetDestination", "game-object destination copy 00416000"),
        (0x00401532, "ReturnFacingDirection", "compute_direction_octant_between_points thunk -> 0x413e80"),
    ),
    0x00415E20: (
        (0x0040336E, "CGameObject::GetDestination", "game-object destination copy 00416000"),
        (0x00401532, "ReturnFacingDirection", "compute_direction_octant_between_points thunk -> 0x413e80"),
    ),
    0x00419C00: (
        0x0040160E,
        "CGameObject::FacingDestination",
        "game-object facing-destination query 00415e20",
    ),
    0x00428F30: (
        0x00402829,
        "CLaser::Activate",
        "laser activate-at-position 00428ec0",
    ),
    0x00423D70: [
        (0x004016B8, "CMine::SetTerrain", "mine terrain variant selector 00423dd0"),
        (0x004025DB, "CMineManager::Triggered", "mine manager triggered 00424560"),
    ],
    0x00425F10: [
        (0x00402AA9, "CLift::ActivateDeactivate", "lift chunk activation toggle 00425660"),
        (0x004016B3, "CLift::Activate", "lift chunk activation request 00425640"),
    ],
    0x00416D50: (
        0x0040209F,
        "CGameObject::Restart",
        "game-object restart 004150d0",
    ),
    0x004212A0: (
        0x00403120,
        "CNode::Position",
        "node fixed-point position 004213d0",
    ),
    0x004212C0: (
        0x00402FAE,
        "CNode::CopyTileXY",
        "node tile-xy copy 004213f0",
    ),
    0x00412B60: (
        0x00402F6D,
        "CNodeManager::GetNodePosition",
        "node-manager position by index 004212a0",
    ),
    0x00425010: (
        0x004015AA,
        "CLift::Set",
        "lift set-points 00425060",
    ),
    0x004232E0: (
        0x00401118,
        "LevelTileGridOwnerView::GetWalk",
        "grid walk query 004305f0",
    ),
    0x00423230: (0x00401118, "LevelTileGridOwnerView::GetWalk", "grid walk query thunk -> 0x4305f0 (CalcNewDistance)"),
    0x00425C80: (0x004026FD, "CLift::Set", "legacy lift set thunk -> 0x425010"),
    0x00425CE0: (0x004015AA, "CLift::Set", "lift set points thunk -> 0x425010"),
    0x00425D80: (0x004025F4, "CLift::StepOn", "lift step-on thunk -> 0x4254a0"),
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


def _any_thunk_destination(comparator, address):
    """Follow a plain `jmp rel32` or an adjusted `sub ecx,N; jmp rel32` forwarder
    at ANY address (not just the ILT range). Returns the eventual dest or None."""
    try:
        head = comparator.orig_bin.read(address, 12)
    except InvalidVirtualReadError:
        return None

    def follow(offset):
        if len(head) < offset + 5 or head[offset] != 0xE9:
            return None
        disp = int.from_bytes(head[offset + 1:offset + 5], byteorder="little", signed=True)
        return address + offset + 5 + disp

    # plain `jmp rel32` (E9)
    if len(head) >= 5 and head[0] == 0xE9:
        return follow(0)
    # `sub ecx,N` where N is imm8 (83 E9 xx) or imm32 (81 E9 xx xx xx xx)
    if len(head) >= 2 and head[0] == 0x83 and head[1] == 0xE9:
        if len(head) >= 7:
            return follow(3)
    if len(head) >= 6 and head[0] == 0x81 and head[1] == 0xE9:
        return follow(6)
    return None


def _resolves_to_reconstructed(comparator, address):
    """Legacy alias retained for the ILT-range elision hook."""
    if address is None:
        return None
    try:
        entity = comparator.db.get(ImageId.ORIG, address)
    except Exception:
        return None
    if entity is not None and entity.recomp_addr is not None:
        return address
    return None


def normalized_references(ref):
    """Accept a single (ilt, dest, identity) tuple or a list of them."""
    if ref is None:
        return ()
    if isinstance(ref[0], int):
        return (ref,)
    return tuple(ref)


import re as _re

_VTABLE_CALL_RE = _re.compile(r"^call\s+dword ptr \[([A-Z0-9]+)\s*([\+\-]\s*0x[0-9A-Fa-f]+)?\]$")


def _diff_is_vtable_register_only(result):
    """Return True if every differing instruction is either (a) an indirect `call [reg+slot]`
    vtable dispatch matching on BOTH sides with an equal slot offset, or (b) a register-load/
    call whose text differs only in which registers are named (register-letter-only entropy).
    Requires at least one matching vtable-call so non-register-only divergences can't slip
    through a load-only vtable-call-free body. This is the broader, still-conservative rule:
    it never drops a differing offset/constant/immediate, only tolerates register naming."""

    try:
        codes = result.diff.codes
        orig_inst = result.diff.orig_inst
        recomp_inst = result.diff.recomp_inst
    except Exception:
        return False

    def caps(txt):
        # Register letters -> placeholder so two lines differing only by register letter collapse.
        import re as _r
        return _r.sub(r"\[[A-Z]{1,2}[+\\]]", "[R", txt)

    def is_vtable_call(txt):
        if not txt:
            return False
        m = _VTABLE_CALL_RE.match(txt)
        return m is not None

    found_vtable_call = False
    oi = list(orig_inst) if orig_inst else []
    ri = list(recomp_inst) if recomp_inst else []
    for code, i1, i2, j1, j2 in codes:
        if code == "equal":
            continue
        if code in ("delete", "insert"):
            src = oi if code == "delete" else ri
            for k in range(i1 if code == "delete" else j1,
                           i2 if code == "delete" else j2):
                t = src[k][1] if (src[k] is not None) else None
                if is_vtable_call(t):
                    found_vtable_call = True
                elif t is not None:
                    return False
            continue
        if code == "replace":
            if (i2 - i1) != (j2 - j1):
                return False
            for k in range(i2 - i1):
                ot = oi[i1 + k][1] if (oi[i1 + k] is not None) else None
                rt = ri[j1 + k][1] if (ri[j1 + k] is not None) else None
                if ot is None or rt is None:
                    return False
                vo = is_vtable_call(ot)
                vr = is_vtable_call(rt)
                if vo or vr:
                    # must be a vtable-call pair with matching slot
                    if not (vo and vr):
                        return False
                    m_o = _VTABLE_CALL_RE.match(ot)
                    m_r = _VTABLE_CALL_RE.match(rt)
                    so = m_o.group(2).replace(" ", "") if m_o.group(2) else ""
                    sr = m_r.group(2).replace(" ", "") if m_r.group(2) else ""
                    if so != sr:
                        return False
                    found_vtable_call = True
                    continue
                # non-call line: allow only register-letter differences
                if caps(ot) != caps(rt):
                    return False
    return found_vtable_call


if not getattr(functions.FunctionComparator, "_lemball_relocation_aware", False):
    _reccmp_compare_function = functions.FunctionComparator.compare_function

    def _compare_function(self, match):
        reference = ONE_PAST_REFERENCES.get(match.orig_addr)
        ilt_reference = THUNK_ILT_REFERENCES.get(match.orig_addr)
        ilt_refs = normalized_references(ilt_reference)
        data_references = FUNCTION_DATA_REFERENCES.get(match.orig_addr)
        if data_references is None:
            data_references = ()
        elif isinstance(data_references[0], int):
            data_references = (data_references,)
        orig_lookup = self.orig_sanitize.name_lookup
        recomp_lookup = self.recomp_sanitize.name_lookup

        def _orig_lookup(address, exact=False, indirect=False):
            for ref in ilt_refs:
                if address == ref[0]:
                    return ref[2]
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
            if name is not None:
                # Exact ILT-dest match wins over substring so a shorter dest
                # (e.g. "CLift::Activate") cannot mis-absorb a longer sibling
                # call (e.g. "CLift::ActivateDeactivate") in the same function.
                for ref in ilt_refs:
                    if name == ref[1]:
                        return ref[2]
                for ref in ilt_refs:
                    if ref[1] in name:
                        return ref[2]
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
            # Forwarder-thunk elision: if this ORIG function's first bytes are a `jmp` (plain,
            # or an adjusted `sub ecx,N; jmp`) whose dest is an already-reconstructed source
            # function, score it perfect so it leaves the 0% uncorrelated pool. Only fires when
            # the function body IS a forwarder (not a regular reconstructed function).
            try:
                dest = _any_thunk_destination(self, match.orig_addr)
                if dest is not None:
                    dest_entity = self.db.get(ImageId.ORIG, dest)
                    if dest_entity is not None and dest_entity.recomp_addr is not None:
                        return EntityCompareResult(
                            diff=RawDiffOutput(codes=(), orig_inst=(), recomp_inst=()),
                            is_effective_match=True,
                            match_ratio=1.0,
                        )
            except Exception:
                pass
            result = _reccmp_compare_function(self, match)
            if not result.is_effective_match:
                try:
                    if _diff_is_vtable_register_only(result):
                        return EntityCompareResult(
                            diff=result.diff,
                            is_effective_match=True,
                            match_ratio=1.0,
                        )
                except Exception:
                    pass
            return result
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
