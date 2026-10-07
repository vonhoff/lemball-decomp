"""CodeWarrior symbol grammar and rejected encodings."""

import unittest

from lib.source.codewarrior import decode_signature


class CodeWarriorTests(unittest.TestCase):
    def test_catalog_encodings(self):
        cases = {
            "__ct__12CPadToButtonFi": "CPadToButton::CPadToButton(int)",
            "__dt__Q214CPreviewDrawer8tagPRIMSFv": "CPreviewDrawer::tagPRIMS::~tagPRIMS()",
            "Get__6CFixedCFv": "CFixed::Get() const",
            "Callback__FPFPc_Uc": "Callback(unsigned char (*)(char*))",
            "__op7CVector__8CVSPointCFv": "CVSPoint::operator CVector() const",
            "main": "main(?)",
        }
        for symbol, expected in cases.items():
            self.assertEqual(decode_signature(symbol).display(), expected)
        for symbol in ("Wrong__FP", "Callback__FPFPc", "Callback__FPFPc_"):
            with self.subTest(symbol=symbol), self.assertRaises(ValueError):
                decode_signature(symbol)
