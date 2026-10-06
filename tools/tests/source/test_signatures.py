"""C++ parameter normalization and unsupported declarators."""

import unittest

from lib.source.signatures import canonical_type, parameter_type


class SignatureTests(unittest.TestCase):
    def test_cpp_parameter_types(self):
        cases = {
            "int capacity = 42": "int",
            "Widget const* value": "const Widget*",
            "Widget* const value": "Widget*",
            "char buffer[12]": "char*",
            "Namespace::Widget": "Namespace::Widget",
            "unsigned long value": "unsigned long",
            "int (*callback)(char* text)": "int (*)(char*)",
            "void (*outer)(int (*inner)(const char* text))": "void (*)(int (*)(const char*))",
        }
        for source, expected in cases.items():
            self.assertEqual(parameter_type(source), expected)
        self.assertNotEqual(
            canonical_type("unsigned long"), canonical_type("unsigned int")
        )
        for source in (
            "void (__stdcall *callback)(int)",
            "void (Widget::*callback)(int)",
            "void (*callback)(void, int)",
        ):
            with self.assertRaises(ValueError):
                parameter_type(source)
