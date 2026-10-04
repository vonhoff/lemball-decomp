"""Source-policy tripwires and legitimate low-level operations."""

import contextlib
import io
import tempfile
import unittest
from pathlib import Path

from lib.policy import check_policy, violations


class PolicyTests(unittest.TestCase):
    def test_rejected_operations(self):
        cases = (
            "*(int*)((char*)this + 0x34) = value;",
            "return *(short*)((unsigned char*)object + 6);",
            "return *reinterpret_cast<int*>(reinterpret_cast<char*>(object) + 4);",
            "base = (char*)this - 0x10;",
            "base = reinterpret_cast<char*>(this) + 0x10;",
            "base = (unsigned long)this + 16;",
            "base = (char*)this + 0x34U;",
            "table = *(void***)this;",
            "object->__vfptr[2](object);",
            "offset = object->__vbptr[1];",
            "value = *(int*)0x00401000;",
            "value = *((int*)0x00401000);",
            "*reinterpret_cast<int*>(0x00401000) = value;",
            "((void (__cdecl*)())0x00401000)();",
            "auto function = reinterpret_cast<int (*)(int)>(0x00401000);",
            "__asm { mov eax, ecx }",
            "_emit 0x90;",
            "*(int*) (\n  (char*)object + 0x34\n) = value;",
        )
        for code in cases:
            with self.subTest(code=code):
                self.assertTrue(list(violations(code)))

    def test_legitimate_operations(self):
        cases = (
            "cursor += 4; pixels[y * stride + x] = colour;",
            "value = *reinterpret_cast<unsigned short*>(p_data);",
            "value = *(unsigned short*)p_data;",
            "value = *(int*)this->data;",
            "value = *(int*)(this)->data;",
            "m_data = (unsigned char*)(this + 1);",
            "start = (unsigned char*)this + GetSizeOf();",
            "dst = *(unsigned char**)((unsigned char*)m_lines + lineOffset) + startX;",
            "LoadIconA(instance, (char*)0x75);",
            "message.m_source = (void*)40001;",
            'text = "__asm // prose *(int*)0x00401000";',
            "int assembly = 0; char slash = '/';",
        )
        for code in cases:
            with self.subTest(code=code):
                self.assertEqual(list(violations(code)), [])

    def test_functional_comments(self):
        text = (
            "// SIZE 0x08\nint m_value; // 0x04\n"
            "// MINIMUM SIZE 0x04\nvoid Method(); // vtable+0x08\n"
            "// clang-format off\n// clang-format on\n"
            "// FUNCTION: LEMBALL 0x00401000\n// __isctype\n"
            "// FUNCTION: LEMBALL 0x00401010 SYMBOL\n// ??0CVSRect@@QAE@FFFF@Z\n"
            "// SYNTHETIC: LEMBALL 0x00401020\n// CThing::`scalar deleting destructor'\n"
            '// STRING: LEMBALL 0x00401030\n// "some text"\n'
            "// VTABLE: LEMBALL 0x00401040 CBase\n"
            "// VTABLE: LEMBALL 0x00401050 CDerived's `CBase\n"
            "// STUB: LEMBALL 0x00401060\nvoid Stub() {}\n"
        )
        self.assertEqual(list(violations(text)), [])

    def test_rejected_comments(self):
        cases = (
            "// Explain the previous implementation.",
            "int value; // increment this later",
            "/* raw offset access is justified here */",
            "// clang-format off because matching",
            "int value; // clang-format off",
            "// SIZE 0x08 probably",
            "// UnknownSymbol",
            "// FUNCTION: LEMBALL 0x00401000\n// some prose",
            "// FUNCTION: LEMBALL 0x00401000 prose suffix",
            "// FUNCTION: LEMBALL 0x00401000\n\n// UnknownSymbol",
            "// FUNCTION: LEMBALL 0x00401000\nvoid Function(); // UnknownSymbol",
            "// FUNCTION: LEMBALL 0x00401000\n// Symbol\n// AnotherSymbol",
        )
        for code in cases:
            with self.subTest(code=code):
                self.assertTrue(list(violations(code)))

    def test_source_extensions_and_diagnostic_lines(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for suffix in (".cpp", ".h", ".inl", ".RC"):
                path = root / f"Fixture{suffix}"
                path.write_text('\n"__asm";\n__asm nop;\n', encoding="utf-8")
            with contextlib.redirect_stdout(io.StringIO()) as output:
                self.assertEqual(check_policy([root]), 1)
            self.assertEqual(output.getvalue().count(":3: assembly:"), 4)
