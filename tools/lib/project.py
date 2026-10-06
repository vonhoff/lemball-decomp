"""Repository paths and reccmp target configuration."""

from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BUILD = ROOT / "build-msvc400"
SRC = ROOT / "src"
RECCMP_JSON = BUILD / "reccmp.json"
REPORT_JSON = BUILD / "report.json"
EFFECTIVE_JSON = BUILD / "effective.json"

TARGET_ID = "LEMBALL"


WINDOWS_NAME_REVIEWS = {
    (
        0x0043A500,
        "OnZoomBox__4CWndFUc",
        "CWnd::OnDriverChange()",
    ): "LEMBALL.EXE: CWnd vtable+0x5c at 0x0049942c points through "
    "0x00401028 to the zero-argument RET at 0x0043a500; CPVWnd's same "
    "slot points to OnDriverChange at 0x00466340.",
    (
        0x0045EDA0,
        "GetCDDir__FPCc",
        "CPlatformServices::GetCDDir(const char*)",
    ): "LEMBALL.EXE: caller 0x00406e60 loads the platform object into ECX "
    "before CALL 0x0045eda0; the callee returns with RET 4 at 0x0045ee61. "
    "Windows uses a member function for the catalog's free function.",
}
