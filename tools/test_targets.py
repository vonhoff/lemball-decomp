"""Conservative original-binary target sizing and ranking tests."""

import unittest
from unittest.mock import patch

from targets import Func, add_original_evidence, inspect_original, ranked_gain, resolve_original_target, tiny_stubs


class Image:
    def __init__(self, data, base=0x1000):
        self.data, self.base = data, base

    def read(self, address, size):
        offset = address - self.base
        return self.data[offset:offset + size] if 0 <= offset < len(self.data) else b""


class OriginalEvidenceTests(unittest.TestCase):
    def test_rebuilt_stub_is_not_original_size(self):
        evidence = inspect_original(Image(bytes.fromhex('558becb8010000005dc3')), 0x1000, {0x1000})
        self.assertEqual(evidence.size, 10)

    def test_reject_truncated_instruction(self):
        self.assertIsNone(inspect_original(Image(b'\xe9\x01'), 0x1000, {0x1000}).size)

    def test_reject_indirect_jump_without_table_evidence(self):
        self.assertIsNone(inspect_original(Image(bytes.fromhex('ff248500200000')), 0x1000, {0x1000}).size)

    def test_reject_branch_into_instruction(self):
        self.assertIsNone(inspect_original(Image(bytes.fromhex('7501b801000000c3')), 0x1000, {0x1000}).size)

    def test_reject_fallthrough_into_next_function(self):
        self.assertIsNone(inspect_original(Image(b'\x90\xc3'), 0x1000, {0x1000, 0x1001}).size)

    def test_follow_both_conditional_edges(self):
        evidence = inspect_original(Image(bytes.fromhex('7503c20400c3')), 0x1000, {0x1000})
        self.assertEqual(evidence.size, 6)

    def test_tail_jump_does_not_swallow_callee(self):
        evidence = inspect_original(Image(bytes.fromhex('e900000000c3')), 0x1000, {0x1000, 0x1005})
        self.assertEqual(evidence.size, 5)
        self.assertEqual(evidence.callees, (0x1005,))

    def test_unknown_sizes_are_not_tiny(self):
        f = Func(0x1000, 'LargeStub', 3, 0.0, 'unit', 'STUB')
        self.assertEqual(tiny_stubs([f], 5), [])

    def test_gain_uses_readiness_and_original_size(self):
        ready = Func(1, 'Ready', 3, 0.0, 'unit', 'STUB', 100, 1.0)
        blocked = Func(2, 'Blocked', 3000, 0.0, 'unit', 'STUB', 1000, 0.0)
        self.assertEqual(ranked_gain([blocked, ready])[0], ready)
        self.assertEqual(ready.expected_gain, 100)
        self.assertEqual(blocked.expected_gain, 0)

    def test_stub_callee_is_not_ready(self):
        image = Image(bytes.fromhex('e80b000000c3') + b'\x90' * 10 + b'\xc3')
        caller = Func(0x1000, 'Caller', 3, 0.0, 'unit', 'STUB')
        callee = Func(0x1010, 'Callee', 3, 0.0, 'unit', 'STUB')
        with patch('check.load_original_image', return_value=image):
            self.assertEqual(add_original_evidence([caller, callee])[0].readiness, 0)
            callee = Func(0x1010, 'Callee', 1, 100.0, 'unit', 'FUNCTION')
            self.assertEqual(add_original_evidence([caller, callee])[0].readiness, 1)

    def test_indirect_call_is_not_assumed_ready(self):
        image = Image(bytes.fromhex('ff10c3'))
        caller = Func(0x1000, 'Caller', 3, 0.0, 'unit', 'STUB')
        with patch('check.load_original_image', return_value=image):
            result = add_original_evidence([caller])[0]
        self.assertEqual(result.original_size, 3)
        self.assertEqual(result.readiness, 0)

    def test_reject_thunk_cycle(self):
        self.assertIsNone(resolve_original_target(Image(bytes.fromhex('ebfe')), 0x1000))


if __name__ == '__main__':
    unittest.main()
