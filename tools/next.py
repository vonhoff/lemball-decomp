#!/usr/bin/env python3
"""Rank unfinished functions from the canonical report."""

import argparse
import json
import sys

from lib import REPORT_JSON


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--kind', choices=('near', 'gain'), default='near')
    parser.add_argument('--limit', type=int, default=40, help='rows; 0 = unlimited')
    args = parser.parse_args()
    try:
        report = json.loads(REPORT_JSON.read_text(encoding='utf-8'))
    except (OSError, ValueError) as error:
        sys.exit(f'cannot read report: {error}')
    rows = [{**function, 'unit': unit['name']}
            for unit in report['units'] for function in unit['functions']
            if function['fuzzy_match_percent'] < 100]
    if args.kind == 'near':
        rows.sort(key=lambda f: (-f['fuzzy_match_percent'], int(f['size']), int(f['metadata']['virtual_address'])))
    else:
        rows.sort(key=lambda f: (-int(f['size']) * f['fuzzy_match_percent'],
                                 -int(f['size']), int(f['metadata']['virtual_address'])))
    for function in rows[:args.limit] if args.limit > 0 else rows:
        address = int(function['metadata']['virtual_address'])
        print(f"0x{address:08x} {function['fuzzy_match_percent']:6.2f}% "
              f"size={int(function['size']):4d} {function['unit']} "
              f"{function['metadata'].get('demangled_name', function['name'])}")
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
