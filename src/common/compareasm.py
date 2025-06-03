import sys
import re
from collections import defaultdict
from pathlib import Path

ignorelist = ('.comment', 'vtable', '.str', 'conststr')
should_ignore = lambda s: any(sub in s for sub in ignorelist)

def parse_objdump(lines):
    current_symbol = None
    symbol_instr_counts = defaultdict(int)
    symbol_instr_types = defaultdict(lambda: defaultdict(int))

    for line in lines:
        symbol_match = re.match(r'^<(.+)>:', line)
        instr_match = re.match(r'^\s+([0-9a-f ]+)\s+(\S+)', line)

        if symbol_match:
            current_symbol = symbol_match.group(1)
            continue

        if current_symbol and instr_match:
            mnemonic = instr_match.group(2)
            if mnemonic == '.word':
                continue
            symbol_instr_counts[current_symbol] += 1
            symbol_instr_types[current_symbol][mnemonic] += 1

    return symbol_instr_counts, symbol_instr_types

def print_comparison(counts1, types1, counts2, types2):
    all_symbols = sorted(set(counts1) | set(counts2))
    for sym in all_symbols:
        if should_ignore(sym):
            continue
        c1 = counts1.get(sym, 0)
        c2 = counts2.get(sym, 0)
        print(f"{sym}:\n    Total   : {c1:<5} | {c2:<5}")

        all_instrs = sorted(set(types1.get(sym, {})) | set(types2.get(sym, {})))
        for instr in all_instrs:
            i1 = types1.get(sym, {}).get(instr, 0)
            i2 = types2.get(sym, {}).get(instr, 0)
            print(f"    {instr:<8}: {i1:<5} | {i2:<5}")
        print()


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 compareasm.py file1.s file2.s")
        sys.exit(1)

    with open(sys.argv[1]) as f1, open(sys.argv[2]) as f2:
        counts1, types1 = parse_objdump(f1)
        counts2, types2 = parse_objdump(f2)

    print_comparison(counts1, types1, counts2, types2)

    # Aggregate total counts per mnemonic for each file
    total_1 = defaultdict(int)
    total_2 = defaultdict(int)
    for symbol in types1:
        if should_ignore(symbol):
            continue
        for mnem, count in types1[symbol].items():
            total_1[mnem] += count

    for symbol in types2:
        if should_ignore(symbol):
            continue
        for mnem, count in types2[symbol].items():
            total_2[mnem] += count

    # Get union of all mnemonics
    all_mnemonics = sorted(
        set(total_1) | set(total_2),
        key=lambda m: (total_2.get(m, 0) - total_1.get(m, 0), m),
    )

    # Print side-by-side summary
    print(f"\n{'Mnemonic':<12} {'File 1':>8} {'File 2':>8} {'Δ':>6}")
    print("-" * 36)

    total_count_1 = 0
    total_count_2 = 0

    for mnem in all_mnemonics:
        c1 = total_1.get(mnem, 0)
        c2 = total_2.get(mnem, 0)
        total_count_1 += c1
        total_count_2 += c2
        print(f"{mnem:<12} {c1:8} {c2:8} {c2 - c1:6}")

    # Step 4: Print totals
    print("-" * 36)
    print(f"{'TOTAL':<12} {total_count_1:8} {total_count_2:8} {total_count_2 - total_count_1:6}")
