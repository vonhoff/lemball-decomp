"""Dump orig LEMBALL.EXE .rsrc dialogs/menu/version into src/LEMBALL.RC.

Does not extract RT_ICON / GROUP_ICON bitmaps (original game art).
"""
from __future__ import annotations

import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
EXE = ROOT / "data" / "LEMBALL.EXE"
SRC = ROOT / "src"

exe = EXE.read_bytes()


def u16(buf: bytes, o: int) -> int:
    return struct.unpack_from("<H", buf, o)[0]


def u32(buf: bytes, o: int) -> int:
    return struct.unpack_from("<I", buf, o)[0]


e_lfanew = u32(exe, 0x3C)
num_sec = u16(exe, e_lfanew + 6)
opt_size = u16(exe, e_lfanew + 20)
opt = e_lfanew + 24
rva_res = u32(exe, opt + 96 + 16)
sec0 = opt + opt_size
secs = []
for i in range(num_sec):
    o = sec0 + i * 40
    vsz, va, rsz, raw = struct.unpack_from("<IIII", exe, o + 8)
    secs.append((va, vsz, raw, rsz))


def rva2off(rva: int) -> int:
    for va, vsz, raw, rsz in secs:
        if va <= rva < va + max(vsz, rsz):
            return raw + (rva - va)
    raise ValueError(hex(rva))


def name_str(id_or_off: int):
    if id_or_off & 0x80000000:
        o = rva2off(rva_res + (id_or_off & 0x7FFFFFFF))
        n = u16(exe, o)
        return exe[o + 2 : o + 2 + n * 2].decode("utf-16le")
    return id_or_off


def walk(dir_rva: int, path: list):
    o = rva2off(dir_rva)
    named = u16(exe, o + 12)
    ids = u16(exe, o + 14)
    out = []
    for i in range(named + ids):
        e = o + 16 + i * 8
        name = u32(exe, e)
        off = u32(exe, e + 4)
        nm = name_str(name)
        if off & 0x80000000:
            out += walk(rva_res + (off & 0x7FFFFFFF), path + [nm])
        else:
            de = rva2off(rva_res + off)
            data_rva = u32(exe, de)
            size = u32(exe, de + 4)
            out.append((path + [nm], rva2off(data_rva), size))
    return out


items = walk(rva_res, [])

CLASS_ATOM = {
    0x80: "BUTTON",
    0x81: "EDIT",
    0x82: "STATIC",
    0x83: "LISTBOX",
    0x84: "SCROLLBAR",
    0x85: "COMBOBOX",
}

DS = [
    (0x0001, "DS_ABSALIGN"),
    (0x0002, "DS_SYSMODAL"),
    (0x0004, "DS_3DLOOK"),
    (0x0008, "DS_FIXEDSYS"),
    (0x0010, "DS_NOFAILCREATE"),
    (0x0020, "DS_LOCALEDIT"),
    (0x0040, "DS_SETFONT"),
    (0x0080, "DS_MODALFRAME"),
    (0x0100, "DS_NOIDLEMSG"),
    (0x0200, "DS_SETFOREGROUND"),
    (0x0400, "DS_CONTROL"),
    (0x0800, "DS_CENTER"),
    (0x1000, "DS_CENTERMOUSE"),
    (0x2000, "DS_CONTEXTHELP"),
]

WS = [
    (0x80000000, "WS_POPUP"),
    (0x40000000, "WS_CHILD"),
    (0x20000000, "WS_MINIMIZE"),
    (0x10000000, "WS_VISIBLE"),
    (0x08000000, "WS_DISABLED"),
    (0x04000000, "WS_CLIPSIBLINGS"),
    (0x02000000, "WS_CLIPCHILDREN"),
    (0x01000000, "WS_MAXIMIZE"),
    (0x00C00000, "WS_CAPTION"),
    (0x00800000, "WS_BORDER"),
    (0x00400000, "WS_DLGFRAME"),
    (0x00200000, "WS_VSCROLL"),
    (0x00100000, "WS_HSCROLL"),
    (0x00080000, "WS_SYSMENU"),
    (0x00040000, "WS_THICKFRAME"),
    (0x00020000, "WS_MINIMIZEBOX"),
    (0x00010000, "WS_MAXIMIZEBOX"),
]

# WS_GROUP/TABSTOP live in low word overlapping control styles; emit on controls separately.
WS_CTRL = [
    (0x10000000, "WS_VISIBLE"),
    (0x08000000, "WS_DISABLED"),
    (0x04000000, "WS_CLIPSIBLINGS"),
    (0x02000000, "WS_CLIPCHILDREN"),
    (0x00020000, "WS_GROUP"),
    (0x00010000, "WS_TABSTOP"),
    (0x40000000, "WS_CHILD"),
    (0x00800000, "WS_BORDER"),
    (0x00400000, "WS_DLGFRAME"),
    (0x00200000, "WS_VSCROLL"),
    (0x00100000, "WS_HSCROLL"),
]

BS = [
    (0x000B, "BS_OWNERDRAW"),
    (0x0009, "BS_AUTORADIOBUTTON"),
    (0x0007, "BS_GROUPBOX"),
    (0x0006, "BS_AUTO3STATE"),
    (0x0005, "BS_3STATE"),
    (0x0004, "BS_RADIOBUTTON"),
    (0x0003, "BS_AUTOCHECKBOX"),
    (0x0002, "BS_CHECKBOX"),
    (0x0001, "BS_DEFPUSHBUTTON"),
]
BS_EXTRA = [
    (0x8000, "BS_RIGHT"),
    (0x0100, "BS_LEFTTEXT"),
    (0x0020, "BS_LEFTTEXT"),
    (0x0800, "BS_TOP"),
    (0x0400, "BS_BOTTOM"),
    (0x0200, "BS_VCENTER"),
    (0x1000, "BS_MULTILINE"),
    (0x2000, "BS_NOTIFY"),
    (0x4000, "BS_FLAT"),
]

SS = [
    (0x0003, "SS_ICON"),
    (0x0004, "SS_BLACKRECT"),
    (0x0005, "SS_GRAYRECT"),
    (0x0006, "SS_WHITERECT"),
    (0x0007, "SS_BLACKFRAME"),
    (0x0008, "SS_GRAYFRAME"),
    (0x0009, "SS_WHITEFRAME"),
    (0x0010, "SS_ETCHEDHORZ"),
    (0x0011, "SS_ETCHEDVERT"),
    (0x0012, "SS_ETCHEDFRAME"),
    (0x0002, "SS_RIGHT"),
    (0x0001, "SS_CENTER"),
    (0x000E, "SS_BITMAP"),
    (0x000C, "SS_LEFTNOWORDWRAP"),
    (0x000B, "SS_SIMPLE"),
]
SS_EXTRA = [
    (0x0100, "SS_NOPREFIX"),
    (0x0200, "SS_NOTIFY"),
    (0x0400, "SS_CENTERIMAGE"),
    (0x0800, "SS_RIGHTJUST"),
    (0x1000, "SS_REALSIZEIMAGE"),
    (0x2000, "SS_SUNKEN"),
]

ES = [
    (0x0004, "ES_MULTILINE"),
    (0x0008, "ES_UPPERCASE"),
    (0x0010, "ES_LOWERCASE"),
    (0x0020, "ES_PASSWORD"),
    (0x0040, "ES_AUTOVSCROLL"),
    (0x0080, "ES_AUTOHSCROLL"),
    (0x0100, "ES_NOHIDESEL"),
    (0x0400, "ES_OEMCONVERT"),
    (0x0800, "ES_READONLY"),
    (0x1000, "ES_WANTRETURN"),
    (0x2000, "ES_NUMBER"),
    (0x0002, "ES_RIGHT"),
    (0x0001, "ES_CENTER"),
]

CBS = [
    (0x0003, "CBS_DROPDOWNLIST"),
    (0x0002, "CBS_DROPDOWN"),
    (0x0001, "CBS_SIMPLE"),
]
CBS_EXTRA = [
    (0x0040, "CBS_AUTOHSCROLL"),
    (0x0100, "CBS_DISABLENOSCROLL"),
    (0x0200, "CBS_UPPERCASE"),
    (0x0400, "CBS_LOWERCASE"),
    (0x2000, "CBS_SORT"),
    (0x0040, "CBS_AUTOHSCROLL"),
    (0x0010, "CBS_OEMCONVERT"),
    (0x0020, "CBS_SORT"),
    (0x0040, "CBS_HASSTRINGS"),
    (0x0080, "CBS_OWNERDRAWFIXED"),
    (0x0400, "CBS_NOINTEGRALHEIGHT"),
]

LBS = [
    (0x0001, "LBS_NOTIFY"),
    (0x0002, "LBS_SORT"),
    (0x0004, "LBS_NOREDRAW"),
    (0x0008, "LBS_MULTIPLESEL"),
    (0x0010, "LBS_OWNERDRAWFIXED"),
    (0x0020, "LBS_OWNERDRAWVARIABLE"),
    (0x0040, "LBS_HASSTRINGS"),
    (0x0080, "LBS_USETABSTOPS"),
    (0x0100, "LBS_NOINTEGRALHEIGHT"),
    (0x0200, "LBS_MULTICOLUMN"),
    (0x0400, "LBS_WANTKEYBOARDINPUT"),
    (0x0800, "LBS_EXTENDEDSEL"),
    (0x1000, "LBS_DISABLENOSCROLL"),
]


def flag_bits(val: int, table: list[tuple[int, str]], mask_used: bool = False) -> tuple[list[str], int]:
    names = []
    left = val
    for bit, name in table:
        if (left & bit) == bit:
            names.append(name)
            left &= ~bit
    return names, left


def dlg_style(val: int) -> str:
    # Prefer WS_CAPTION over BORDER+DLGFRAME when both set.
    names, left = flag_bits(val, DS)
    if (left & 0x00C00000) == 0x00C00000:
        names.append("WS_CAPTION")
        left &= ~0x00C00000
    more, left = flag_bits(left, WS)
    names += more
    if left:
        names.append("0x%X" % left)
    return " | ".join(names) if names else "0"


def ctrl_style(atom: int | None, val: int) -> str:
    names, left = flag_bits(val, WS_CTRL)
    low = left & 0xFFFF
    extra_hi = left & ~0xFFFF
    if atom == 0x80:
        bs_names, low = flag_bits(low, BS)
        more, low = flag_bits(low, BS_EXTRA)
        names += bs_names + more
    elif atom == 0x82:
        ss_names, low = flag_bits(low, SS)
        more, low = flag_bits(low, SS_EXTRA)
        names += ss_names + more
    elif atom == 0x81:
        es_names, low = flag_bits(low, ES)
        names += es_names
    elif atom == 0x85:
        cb_names, low = flag_bits(low, CBS)
        more, low = flag_bits(low, CBS_EXTRA)
        names += cb_names + more
    elif atom == 0x83:
        lb_names, low = flag_bits(low, LBS)
        names += lb_names
    left = extra_hi | low
    if left:
        names.append("0x%X" % left)
    return " | ".join(names) if names else "0"


def read_unicode(buf: bytes, i: int) -> tuple[str, int]:
    end = i
    while end + 1 < len(buf):
        w = struct.unpack_from("<H", buf, end)[0]
        if w == 0:
            return buf[i:end].decode("utf-16le"), end + 2
        end += 2
    return "", i


def align4(i: int) -> int:
    return (i + 3) & ~3


def esc(s: str) -> str:
    out = []
    for ch in s:
        o = ord(ch)
        if ch == '"':
            out.append('\\"')
        elif ch == "\\":
            out.append("\\\\")
        elif ch == "\n":
            out.append("\\n")
        elif ch == "\t":
            out.append("\\t")
        elif o == 0xA9:
            out.append("\\251")
        elif o == 0xAE:
            out.append("\\256")
        elif o < 32 or o > 126:
            out.append("\\%03o" % o)
        else:
            out.append(ch)
    return "".join(out)


def decode_dlg(off: int, size: int) -> dict:
    buf = exe[off : off + size]
    ver = u16(buf, 0)
    sig = u16(buf, 2)
    i = 0
    ex = False
    if ver == 1 and sig == 0xFFFF:
        ex = True
        exstyle = u32(buf, 8)
        style = u32(buf, 12)
        cdit = u16(buf, 16)
        x, y, cx, cy = struct.unpack_from("<hhhh", buf, 18)
        i = 26
    else:
        style = u32(buf, 0)
        exstyle = u32(buf, 4)
        cdit = u16(buf, 8)
        x, y, cx, cy = struct.unpack_from("<hhhh", buf, 10)
        i = 18

    if buf[i] == 0xFF and buf[i + 1] == 0xFF:
        menu = ("ord", u16(buf, i + 2))
        i += 4
    elif u16(buf, i) == 0:
        menu = ("none",)
        i += 2
    else:
        s, i = read_unicode(buf, i)
        menu = ("str", s)

    if buf[i] == 0xFF and buf[i + 1] == 0xFF:
        cls = ("ord", u16(buf, i + 2))
        i += 4
    elif u16(buf, i) == 0:
        cls = ("none",)
        i += 2
    else:
        s, i = read_unicode(buf, i)
        cls = ("str", s)

    if u16(buf, i) == 0:
        title = ""
        i += 2
    else:
        title, i = read_unicode(buf, i)

    font = None
    if style & 0x40:
        pts = u16(buf, i)
        i += 2
        if ex:
            weight = u16(buf, i)
            italic = buf[i + 2]
            charset = buf[i + 3]
            i += 4
            face, i = read_unicode(buf, i)
            font = (pts, weight, italic, charset, face)
        else:
            face, i = read_unicode(buf, i)
            font = (pts, face)

    items = []
    for _ in range(cdit):
        i = align4(i)
        if ex:
            i += 4
            iex = u32(buf, i)
            i += 4
            istyle = u32(buf, i)
            i += 4
            ix, iy, icx, icy = struct.unpack_from("<hhhh", buf, i)
            i += 8
            iid = u32(buf, i)
            i += 4
        else:
            istyle = u32(buf, i)
            iex = u32(buf, i + 4)
            ix, iy, icx, icy = struct.unpack_from("<hhhh", buf, i + 8)
            iid = u16(buf, i + 16)
            i += 18
        if buf[i] == 0xFF and buf[i + 1] == 0xFF:
            iclass = ("atom", u16(buf, i + 2))
            i += 4
        else:
            s, i = read_unicode(buf, i)
            iclass = ("str", s)
        if buf[i] == 0xFF and buf[i + 1] == 0xFF:
            ititle = ("ord", u16(buf, i + 2))
            i += 4
        else:
            s, i = read_unicode(buf, i)
            ititle = ("str", s)
        extra = u16(buf, i)
        i += 2
        extra_bytes = buf[i : i + extra]
        i += extra
        items.append((istyle, iex, ix, iy, icx, icy, iid, iclass, ititle, extra_bytes))
    return {
        "ex": ex,
        "style": style,
        "exstyle": exstyle,
        "cdit": cdit,
        "x": x,
        "y": y,
        "cx": cx,
        "cy": cy,
        "menu": menu,
        "cls": cls,
        "title": title,
        "font": font,
        "items": items,
    }


def id_text(iid: int) -> str:
    if iid == 1:
        return "IDOK"
    if iid == 2:
        return "IDCANCEL"
    if iid == 0xFFFF:
        return "-1"
    return str(iid)


def emit_control(it) -> str:
    istyle, iex, ix, iy, icx, icy, iid, iclass, ititle, extra = it
    atom = iclass[1] if iclass[0] == "atom" else None
    title = ""
    if ititle[0] == "str":
        title = '"%s"' % esc(ititle[1])
    else:
        title = str(ititle[1])
    cid = id_text(iid)
    st = ctrl_style(atom, istyle)

    def kw_extra(default_mask: int) -> str:
        leftover = istyle & ~default_mask
        names = []
        if leftover & 0x00020000:
            names.append("WS_GROUP")
            leftover &= ~0x00020000
        if leftover & 0x00010000:
            names.append("WS_TABSTOP")
            leftover &= ~0x00010000
        if leftover & 0x08000000:
            names.append("WS_DISABLED")
            leftover &= ~0x08000000
        if leftover & 0x00000020:
            names.append("BS_LEFTTEXT")
            leftover &= ~0x00000020
        if leftover & 0x10000000:
            leftover &= ~0x10000000
        if leftover & 0x40000000:
            leftover &= ~0x40000000
        if leftover:
            names.append("0x%X" % leftover)
        if not names:
            return ""
        return ", " + " | ".join(names)

    if atom == 0x80:
        low = istyle & 0x0F
        # keyword controls already imply WS_CHILD|WS_VISIBLE; pushbuttons also TABSTOP.
        btn_def = 0x50000000
        push_def = 0x50010000
        if low == 1:
            return "    DEFPUSHBUTTON %s, %s, %d, %d, %d, %d%s" % (
                title, cid, ix, iy, icx, icy, kw_extra(push_def | 1)
            )
        if low == 0:
            return "    PUSHBUTTON %s, %s, %d, %d, %d, %d%s" % (
                title, cid, ix, iy, icx, icy, kw_extra(push_def)
            )
        if low == 3:
            return "    AUTOCHECKBOX %s, %s, %d, %d, %d, %d%s" % (
                title, cid, ix, iy, icx, icy, kw_extra(btn_def | 3)
            )
        if low == 2:
            return "    CHECKBOX %s, %s, %d, %d, %d, %d%s" % (
                title, cid, ix, iy, icx, icy, kw_extra(btn_def | 2)
            )
        if low == 9:
            return "    AUTORADIOBUTTON %s, %s, %d, %d, %d, %d%s" % (
                title, cid, ix, iy, icx, icy, kw_extra(btn_def | 9)
            )
        if low == 4:
            return "    RADIOBUTTON %s, %s, %d, %d, %d, %d%s" % (
                title, cid, ix, iy, icx, icy, kw_extra(btn_def | 4)
            )
        if low == 7:
            return "    GROUPBOX %s, %s, %d, %d, %d, %d%s" % (
                title, cid, ix, iy, icx, icy, kw_extra(btn_def | 7)
            )
        cname = "BUTTON"
    elif atom == 0x82:
        low = istyle & 0x1F
        if low == 0:
            return '    LTEXT %s, %s, %d, %d, %d, %d' % (title, cid, ix, iy, icx, icy)
        if low == 1:
            return '    CTEXT %s, %s, %d, %d, %d, %d' % (title, cid, ix, iy, icx, icy)
        if low == 2:
            return '    RTEXT %s, %s, %d, %d, %d, %d' % (title, cid, ix, iy, icx, icy)
        if low == 3:
            return '    ICON %s, %s, %d, %d, %d, %d' % (title, cid, ix, iy, icx, icy)
        cname = "STATIC"
    elif atom == 0x81:
        return '    EDITTEXT %s, %d, %d, %d, %d, %s' % (cid, ix, iy, icx, icy, st)
    elif atom == 0x85:
        return '    COMBOBOX %s, %d, %d, %d, %d, %s' % (cid, ix, iy, icx, icy, st)
    elif atom == 0x83:
        return '    LISTBOX %s, %d, %d, %d, %d, %s' % (cid, ix, iy, icx, icy, st)
    elif atom == 0x84:
        cname = "SCROLLBAR"
    else:
        cname = iclass[1] if iclass[0] == "str" else "CONTROL"
    return '    CONTROL %s, %s, "%s", %s, %d, %d, %d, %d' % (
        title,
        cid,
        cname,
        st,
        ix,
        iy,
        icx,
        icy,
    )


def emit_dialog(name, d: dict) -> str:
    if isinstance(name, str):
        header_id = name
    else:
        header_id = str(name)
    kw = "DIALOGEX" if d["ex"] else "DIALOG"
    caption = ""
    if d["title"]:
        caption = 'CAPTION "%s"\n' % esc(d["title"])
    font = ""
    if d["font"]:
        if d["ex"] and len(d["font"]) == 5:
            pts, weight, italic, charset, face = d["font"]
            font = 'FONT %d, "%s", %d, %d, %d\n' % (pts, esc(face), weight, italic, charset)
        else:
            pts, face = d["font"]
            font = 'FONT %d, "%s"\n' % (pts, esc(face))
    exs = ""
    if d["exstyle"]:
        exs = "EXSTYLE 0x%X\n" % d["exstyle"]
    lines = [
        "%s %s %d, %d, %d, %d" % (header_id, kw, d["x"], d["y"], d["cx"], d["cy"]),
        "STYLE %s" % dlg_style(d["style"]),
    ]
    if exs:
        lines.append(exs.rstrip())
    if caption:
        lines.append(caption.rstrip())
    if font:
        lines.append(font.rstrip())
    lines.append("BEGIN")
    for it in d["items"]:
        lines.append(emit_control(it))
    lines.append("END")
    return "\n".join(lines) + "\n"


RC_PREFIX = r'''#include <windows.h>

#define IDR_MAIN_MENU 115

#define IDM_FILE_EXIT 40001
#define IDM_HELP_CONTENTS 40003
#define IDM_HELP_ABOUT 40011
#define IDM_OPTIONS_FULLSCREEN 40012
#define IDM_HELP_ON_HELP 40013
#define IDM_HELP_SEARCH_TOPIC 40016

LANGUAGE 9, 2

IDR_MAIN_MENU MENU
BEGIN
    POPUP "&File"
    BEGIN
        MENUITEM "E&xit", IDM_FILE_EXIT
    END
    POPUP "&Options"
    BEGIN
        MENUITEM "Full Screen        F4", IDM_OPTIONS_FULLSCREEN
    END
    POPUP "&Help"
    BEGIN
        MENUITEM "&Contents", IDM_HELP_CONTENTS
        MENUITEM "&Search Topic", IDM_HELP_SEARCH_TOPIC
        MENUITEM "&Help on Help", IDM_HELP_ON_HELP
        MENUITEM SEPARATOR
        MENUITEM "&About Paint Ball...", IDM_HELP_ABOUT
    END
END

'''

RC_VERSION = r'''
VS_VERSION_INFO VERSIONINFO
FILEVERSION 1,0,0,0
PRODUCTVERSION 1,0,0,0
FILEFLAGSMASK 0x3fL
FILEFLAGS 0xbL
FILEOS 0x4L
FILETYPE 0x1L
FILESUBTYPE 0x0L
BEGIN
    BLOCK "StringFileInfo"
    BEGIN
        BLOCK "080904e4"
        BEGIN
            VALUE "CompanyName", "Visual Sciences Ltd.\0"
            VALUE "FileDescription", "Lemmings Paintball(TM)\0"
            VALUE "FileVersion", "1.0.0.0\0"
            VALUE "InternalName", "Lemmings Paintball\0"
            VALUE "LegalCopyright", "Copyright \251 Psygnosis Ltd. 1990-1995\0"
            VALUE "LegalTrademarks", "Lemmings\256 is a registered trademark of Psygnosis Ltd. Windows(TM) is a trademark of Microsoft Corporation.\0"
            VALUE "OriginalFilename", "LEMBALL.EXE\0"
            VALUE "ProductName", "Lemmings\0"
            VALUE "ProductVersion", "1.0.0.0\0"
        END
    END
    BLOCK "VarFileInfo"
    BEGIN
        VALUE "Translation", 0x809, 1252
    END
END
'''


def write_rc(dialogs) -> None:
    chunks = [RC_PREFIX]
    for p, o, s in dialogs:
        chunks.append(emit_dialog(p[1], decode_dlg(o, s)))
        chunks.append("\n")
    chunks.append(RC_VERSION)
    (SRC / "LEMBALL.RC").write_text("".join(chunks), encoding="ascii")


def main() -> None:
    import sys

    dialogs = [(p, o, s) for p, o, s in items if p[0] == 5]
    write_rc(dialogs)
    print("dialogs", len(dialogs), "rc", SRC / "LEMBALL.RC")
    if "--verbose" in sys.argv:
        for p, o, s in dialogs:
            d = decode_dlg(o, s)
            print("---", p[1], repr(d["title"]))
            print(emit_dialog(p[1], d))


if __name__ == "__main__":
    main()
