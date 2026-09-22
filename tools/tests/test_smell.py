"""Annotation checks must distinguish multiline declarations from function bodies."""

import unittest

from lib.smell import declaration_has_body


class FunctionBodyTests(unittest.TestCase):
    def test_wrapped_local_constructor_and_prototype_are_not_definitions(self):
        self.assertFalse(declaration_has_body("CVsPoint position(\n x,\n y);\nif (x) {\n}"))
        self.assertFalse(declaration_has_body("void Callback(\n int argument\n);\nvoid Next() {}"))

    def test_multiline_function_and_constructor_still_require_annotations(self):
        self.assertTrue(declaration_has_body("void Function(\n int argument\n)\n{\n}"))
        self.assertTrue(declaration_has_body("CThing::CThing(\n int argument\n) : CBase(argument)\n{\n}"))
        self.assertTrue(declaration_has_body("int CThing::Get() const\n{ return 1; }"))


if __name__ == "__main__":
    unittest.main()
