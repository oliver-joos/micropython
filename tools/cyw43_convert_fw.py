#!/usr/bin/env python3
"""Convert CYW43 firmware .h header files to raw binary .bin files.

Usage:
    python3 tools/cyw43_convert_fw.py <output_dir> <wifi_combined.h> <nvram.h> <bt_firmware.h>

Output files:
    wifi_fw.bin  - WiFi firmware + CLM blob (raw binary)
    nvram.bin    - NVRAM configuration content (raw bytes)
    bt_fw.bin    - Bluetooth firmware (raw binary)
"""

import argparse
import os
import re
import subprocess
import sys

ESCAPES = {'\\': 92, 'n': 10, 't': 9, 'r': 13, '0': 0, '"': 34}


def parse_hex_array(content):
    """Extract raw bytes from a C hex byte array like { 0x00, 0x01, ... }."""
    blocks = re.findall(r'\{([^}]*)\}', content, re.DOTALL)
    if not blocks:
        raise ValueError("No hex array found (no '{...}' block)")
    data = max(blocks, key=len)
    hex_values = re.findall(r'0x([0-9a-fA-F]{2})', data)
    return bytes(int(v, 16) for v in hex_values)


def parse_string_literals(content):
    """Parse C string literals like "str\\x00" into raw bytes."""
    result = bytearray()
    for s in re.findall(r'"([^"]*)"', content):
        i = 0
        while i < len(s):
            if s[i] == '\\' and i + 1 < len(s):
                c = s[i + 1]
                if c == 'x' and i + 3 < len(s):
                    try:
                        result.append(int(s[i + 2:i + 4], 16))
                        i += 4
                        continue
                    except ValueError:
                        pass
                if c in ESCAPES:
                    result.append(ESCAPES[c])
                    i += 2
                    continue
            result.append(ord(s[i]))
            i += 1
    return bytes(result)


def parse_nvram(content):
    """Parse NVRAM file with string literals and optional macro expansion."""
    # Try gcc preprocessing first (expands macros)
    try:
        proc = subprocess.run(['gcc', '-E', '-P', '-'], input=content, check=False,
                             capture_output=True, text=True, timeout=30)
        if proc.returncode == 0:
            content = proc.stdout
    except (FileNotFoundError, subprocess.TimeoutExpired):
        pass

    # Expand remaining #define macros manually
    defines = {}
    for m in re.finditer(r'#define\s+(\w+)\s+(.+?)$', content, re.MULTILINE):
        defines[m.group(1)] = m.group(2).strip()

    # Replace macro names (longer first to avoid partial replacements)
    for name, value in sorted(defines.items(), key=lambda x: len(x[0]), reverse=True):
        content = re.sub(r'\b' + re.escape(name) + r'\b', lambda m: value, content)  # pylint: disable=cell-var-from-loop
    content = re.sub(r'^#define.*$', '', content, flags=re.MULTILINE)

    return parse_string_literals(content)


def convert_firmware(filepath, output_path, is_nvram=False):
    """Convert a single .h file to binary."""
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()

    if is_nvram:
        data = parse_nvram(content)
    else:
        data = parse_hex_array(content)

    with open(output_path, 'wb') as f:
        f.write(data)

    return len(data)


def main():
    parser = argparse.ArgumentParser(
        description='Convert CYW43 firmware .h files to binary .bin files.'
    )
    parser.add_argument('output_dir', help='Directory to write .bin output files')
    parser.add_argument('wifi_combined_h', help='WiFi+CLM combined firmware header (.h)')
    parser.add_argument('nvram_h', help='NVRAM configuration header (.h)')
    parser.add_argument('bt_firmware_h', help='Bluetooth firmware header (.h)')

    args = parser.parse_args()
    os.makedirs(args.output_dir, exist_ok=True)

    files = [
        (args.wifi_combined_h, 'wifi_fw.bin', False),
        (args.nvram_h, 'nvram.bin', True),
        (args.bt_firmware_h, 'bt_fw.bin', False),
    ]

    print(f'Converting firmware headers to {args.output_dir}/')
    print()

    for h_path, bin_name, is_nvram in files:
        out_path = os.path.join(args.output_dir, bin_name)
        try:
            size = convert_firmware(h_path, out_path, is_nvram=is_nvram)
            print(f'  {bin_name}: {size} bytes ({size / 1024:.1f} KB)')
        except Exception as e:
            print(f'  ERROR converting {h_path}: {e}', file=sys.stderr)
            sys.exit(1)

    print()
    total = sum(os.path.getsize(os.path.join(args.output_dir, name))
                for _, name, _ in files)
    print(f'Total: {total} bytes ({total / 1024:.1f} KB)')


if __name__ == '__main__':
    main()
