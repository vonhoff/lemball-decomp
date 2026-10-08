"""Isolated compiler/linker evidence; no game objects or source files changed."""

import hashlib
import json
import os
from pathlib import Path
import re
import subprocess

from link_binary import CoffObject, win_short_path
from reccmp.formats import detect_image

ROOT = Path(__file__).resolve().parents[1]
HERE = ROOT / "build-msvc400/linker-thunks/primitive-probe"
BASE_FLAGS = [
    "/nologo",
    "/W3",
    "/WX",
    "/GX-",
    "/GR-",
    "/MT",
    "/Dbool=int",
    "/Dtrue=1",
    "/Dfalse=0",
    "/O2",
    "/Ob1",
    "/Oy",
    "/G4",
    "/Z7",
    "/c",
]
EXPECTED = "b0ebb9c650078fce8401adbe0042e9645206a2cdc781be3763c8cddfa3b85f7d"
env = os.environ.copy()
env["INCLUDE"] = win_short_path(str(ROOT / "msvc400/include"))
env["LIB"] = win_short_path(str(ROOT / "msvc400/lib"))
env["PATH"] = win_short_path(str(ROOT / "msvc400/bin")) + ";" + env["PATH"]


def write(path, text):
    path.write_bytes(text.encode("ascii"))


def run(folder, name, command):
    p = subprocess.run(
        command, cwd=folder, env=env, capture_output=True, text=True, errors="replace"
    )
    write(folder / (name + ".log"), p.stdout + p.stderr)
    if p.returncode:
        raise RuntimeError(f"{folder.name}/{name}: {p.stdout + p.stderr}")


def symbols(path):
    obj = CoffObject(path.read_bytes())
    result = []
    for index, (name, value, section, kind, storage) in obj.symbols.items():
        if not any(
            name.startswith(prefix + cls + "@@")
            for prefix in ["??_G", "??_E", "??1", "??_7"]
            for cls in ["CPrimitive", "CProbe"]
        ):
            continue
        row = dict(name=name, section=section, storage=storage, kind=kind)
        if name in obj.weak_defaults:
            row["weak_default"] = obj.canonical_name(name)
        if section > 0:
            desc = obj.sections[section - 1]
            following = [
                s[1]
                for s in obj.symbols.values()
                if s[2] == section and s[3] == 0x20 and s[1] > value
            ]
            size = (min(following) if following else desc[3]) - value
            raw = obj.data[desc[4] + value : desc[4] + value + size]
            selections = []
            for idx, s in obj.symbols.items():
                if s[2] == section and s[4] == 3 and s[1] == 0:
                    aux = obj.data[obj.symbol_start + idx * 18 + 17]
                    if aux:
                        selections.append(
                            obj.data[obj.symbol_start + (idx + 1) * 18 + 14]
                        )
            row.update(
                size=size,
                characteristics=hex(desc[9]),
                selection=selections,
                bytes=raw.hex(),
            )
            if kind == 0x20:
                row["fingerprint"] = obj.fingerprint(name, size)
                row["fingerprint_with_symbols"] = obj.fingerprint(
                    name, size, include_symbols=True
                )
                row["original_body_match"] = (
                    row["fingerprint"] == EXPECTED and size == 31
                )
                row["relocations"] = [
                    dict(offset=addr - value, kind=kind, symbol=obj.symbols[target][0])
                    for _, (addr, target, kind) in obj.relocations(section)
                    if value <= addr < value + size
                ]
        result.append(row)
    return result


def linked(folder, label, objects, options):
    cmd = [
        win_short_path(str(ROOT / "msvc400/bin/LINK.EXE")),
        "/nologo",
        "/DEBUG",
        "/INCREMENTAL:NO",
        "/SUBSYSTEM:CONSOLE",
        "/MAP:" + label + ".map",
        "/PDB:" + label + ".pdb",
        "/OUT:" + label + ".exe",
        *objects,
        *options,
    ]
    run(folder, label, cmd)
    publics = {}
    for line in (folder / (label + ".map")).read_text(errors="replace").splitlines():
        m = re.match(r"\s+[0-9a-fA-F]+:[0-9a-fA-F]+\s+(\S+)\s+([0-9a-fA-F]{8})\b", line)
        if m:
            publics[m[1]] = int(m[2], 16)
    image = detect_image(folder / (label + ".exe"))
    matches = []
    for region in image.get_code_regions():
        for pos in range(len(region.data) - 30):
            raw = bytearray(region.data[pos : pos + 31])
            raw[8:12] = bytes(4)
            raw[18:22] = bytes(4)
            if hashlib.sha256(raw).hexdigest() == EXPECTED:
                original = region.data[pos : pos + 31]
                matches.append(
                    dict(
                        address=hex(region.addr + pos),
                        symbols=[
                            n for n, a in publics.items() if a == region.addr + pos
                        ],
                        vtable=hex(int.from_bytes(original[8:12], "little")),
                        delete_target=hex(
                            (
                                region.addr
                                + pos
                                + 22
                                + int.from_bytes(original[18:22], "little")
                            )
                            & 0xFFFFFFFF
                        ),
                    )
                )
    shared_base = [
        m
        for m in matches
        if m["vtable"] == hex(publics.get("??_7CPrimitive@@6B@", 0))
        and m["delete_target"] == hex(publics.get("??3@YAXPAX@Z", 0))
    ]
    return dict(
        options=options,
        publics={
            n: hex(a)
            for n, a in publics.items()
            if "CPrimitive@@" in n or "CProbe@@" in n or n == "??3@YAXPAX@Z"
        },
        matching_bodies=shared_base,
        pattern_candidates=matches,
    )


CASES = [
    ("baseline", {}, []),
    ("abstract_array_delete", dict(array_delete=True), []),
    ("derived_array", dict(derived_array=True), []),
    (
        "out_of_line",
        dict(dtor="virtual ~CPrimitive();", definition="CPrimitive::~CPrimitive() {}"),
        [],
    ),
    (
        "pure_destructor",
        dict(
            dtor="virtual ~CPrimitive() = 0;", definition="CPrimitive::~CPrimitive() {}"
        ),
        [],
    ),
    ("export_class", dict(export="__declspec(dllexport)"), []),
    (
        "export_destructor",
        dict(dtor="__declspec(dllexport) virtual ~CPrimitive() {}"),
        [],
    ),
    ("no_inline", {}, ["/Ob0"]),
    ("no_function_packaging", {}, ["/Gy-"]),
    ("exception_handling", {}, ["/GX"]),
    ("no_debug_info", {}, ["/Zd"]),
    ("concrete_array", dict(concrete=True, array_delete=True), []),
]


def main():
    HERE.mkdir(parents=True, exist_ok=True)
    results = []
    for name, opts, extra in CASES:
        folder = HERE / name
        folder.mkdir(exist_ok=True)
        if not opts.get("export") and not opts.get("dtor") and not opts.get("concrete"):
            base = '#include "Engine/Graphics/Primitives/CPrimitive.h"\n'
        else:
            dtor = opts.get("dtor", "virtual ~CPrimitive() {}")
            virtuals = (
                "virtual void Draw(CGDI*) {}\n virtual void Render(CGDI*) {}"
                if opts.get("concrete")
                else "virtual void Draw(CGDI*) = 0;\n virtual void Render(CGDI*) = 0;"
            )
            base = (
                "class CGDI;\nclass "
                + opts.get("export", "")
                + " CPrimitive {\npublic:\n inline CPrimitive() {}\n "
                + dtor
                + "\n "
                + virtuals
                + "\n};\n"
            )
        header = (
            "#ifndef PROBE_SHARED_H\n#define PROBE_SHARED_H\n"
            + base
            + """class CProbe : public CPrimitive {
    public:
     CProbe() {}
     virtual ~CProbe() {}
     virtual void Draw(CGDI*) {}
     virtual void Render(CGDI*) {}
    };
    CPrimitive* MakeA();
    CPrimitive* MakeB();
    void DeleteA(CPrimitive*);
    void DeleteB(CPrimitive*);
    #endif
    """
        )
        write(folder / "shared.h", header)
        a = (
            '#include "shared.h"\n'
            + opts.get("definition", "")
            + "\nCPrimitive* MakeA() { return new CProbe; }\nvoid DeleteA(CPrimitive* p) { delete p; }\n"
        )
        b = '#include "shared.h"\nCPrimitive* MakeB() { return new CProbe; }\nvoid DeleteB(CPrimitive* p) { delete p; }\n'
        if opts.get("array_delete"):
            b += "void DeleteArray(CPrimitive* p) { delete[] p; }\n"
        if opts.get("derived_array"):
            b += "CProbe* MakeArray(int count) { return new CProbe[count]; }\nvoid DeleteArray(CProbe* p) { delete[] p; }\n"
        if opts.get("concrete"):
            b += "CPrimitive* MakeBaseArray(int count) { return new CPrimitive[count]; }\n"
        write(folder / "a.cpp", a)
        write(folder / "b.cpp", b)
        write(
            folder / "main.cpp",
            '#include "shared.h"\nint main() { CPrimitive* a=MakeA(); CPrimitive* b=MakeB(); DeleteB(a); DeleteA(b); return 0; }\n',
        )
        row = dict(case=name, source_options=opts, extra_flags=extra)
        try:
            for source in ["a", "b", "main"]:
                run(
                    folder,
                    source,
                    [
                        win_short_path(str(ROOT / "msvc400/bin/CL.EXE")),
                        *BASE_FLAGS,
                        *extra,
                        "/I" + win_short_path(str(ROOT / "src")),
                        "/Fo" + source + ".obj",
                        source + ".cpp",
                    ],
                )
            row["objects"] = {
                n: symbols(folder / (n + ".obj")) for n in ["a", "b", "main"]
            }
            row["links"] = {
                label: linked(folder, label, objects, options)
                for label, objects, options in [
                    ("normal", ["a.obj", "b.obj", "main.obj"], []),
                    ("reversed", ["b.obj", "a.obj", "main.obj"], []),
                    ("retain", ["a.obj", "b.obj", "main.obj"], ["/OPT:NOREF"]),
                ]
            }
        except Exception as ex:
            row["error"] = str(ex)
        results.append(row)
        write(
            HERE / "results.json",
            json.dumps(
                dict(
                    compiler_flags=BASE_FLAGS,
                    expected_fingerprint=EXPECTED,
                    cases=results,
                ),
                indent=2,
            )
            + "\n",
        )
        print(
            json.dumps(
                dict(
                    case=name,
                    error=row.get("error"),
                    bodies={
                        n: len(x["matching_bodies"])
                        for n, x in row.get("links", {}).items()
                    },
                    wrappers={
                        n: [
                            dict(
                                symbol=s["name"],
                                size=s.get("size"),
                                match=s.get("original_body_match"),
                                weak=s.get("weak_default"),
                            )
                            for s in v
                            if s["name"].startswith(("??_G", "??_E"))
                            and (s["section"] > 0 or s.get("weak_default"))
                        ]
                        for n, v in row.get("objects", {}).items()
                    },
                )
            ),
            flush=True,
        )

    return int(any("error" in row for row in results))


if __name__ == "__main__":
    raise SystemExit(main())
