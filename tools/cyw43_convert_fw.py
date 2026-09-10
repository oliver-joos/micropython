#!/usr/bin/env python3
"""Convert CYW43 firmware .h header files to raw binary .bin files.

Usage:
    python3 tools/cyw43_convert_fw.py <output_dir> <wifi_combined.h> <nvram.h> <bt_firmware.h>

The tool parses existing .h header files (produced by xxd -i) and produces
raw binary .bin files ready for deployment to the filesystem on a board.

Output files:
    wifi_fw.bin  - WiFi firmware + CLM blob (raw binary, no headers)
    nvram.bin    - NVRAM configuration content (raw bytes)
    bt_fw.bin    - Bluetooth firmware (raw binary)

The C runtime loader expects these as raw binary with no headers or magic bytes.
"""

import argparse
import os
import re
import subprocess
import sys


def parse_hex_array(content):
    """Parse a C hex byte array like { 0x00, 0x01, ... } and return raw bytes."""
    match = re.search(r'\{([^}]*)\}', content, re.DOTALL)
    if not match:
        raise ValueError("No hex array content found (no '{...}' block)")

    array_content = match.group(1)
    hex_values = re.findall(r'0x([0-9a-fA-F]{2})', array_content)
    return bytes(int(v, 16) for v in hex_values)


def expand_macros(content):
    """Expand C #define macros by replacing macro names with their values."""
    defines = {}
    for m in re.finditer(r'#define\s+(\w+)\s+(.+?)$', content, re.MULTILINE):
        name = m.group(1)
        value = m.group(2).strip().rstrip(';').rstrip()
        defines[name] = value

    result = content
    for name, value in sorted(defines.items(), key=lambda x: len(x[0]), reverse=True):
        escaped_name = re.escape(name)
        # Use lambda to avoid regex escape issues with \x sequences
        result = re.sub(r'\b' + escaped_name + r'\b', lambda m: value, result)

    # Remove #define lines so their string values aren't double-counted.
    # The macro values have already been substituted into the array body.
    result = re.sub(r'^#define.*$', '', result, flags=re.MULTILINE)

    return result


def parse_string_literals(content):
    """Parse C string literals like "str\x00" "str2\x00" and return raw bytes."""
    strings = re.findall(r'"([^"]*)"', content)
    result = bytearray()
    for s in strings:
        i = 0
        while i < len(s):
            if s[i] == '\\' and i + 1 < len(s):
                next_char = s[i + 1]
                if next_char == 'x' and i + 3 < len(s):
                    hex_str = s[i + 2:i + 4]
                    try:
                        result.append(int(hex_str, 16))
                        i += 4
                        continue
                    except ValueError:
                        pass
                elif next_char == '\\':
                    result.append(ord('\\'))
                    i += 2
                    continue
                elif next_char == 'n':
                    result.append(ord('\n'))
                    i += 2
                    continue
                elif next_char == 't':
                    result.append(ord('\t'))
                    i += 2
                    continue
                elif next_char == '"':
                    result.append(ord('"'))
                    i += 2
                    continue
                elif next_char == '0':
                    result.append(0)
                    i += 2
                    continue
            result.append(ord(s[i]))
            i += 1
    return bytes(result)


def preprocess_with_gcc(filepath):
    """Preprocess an .h file using gcc -E to expand macros."""
    try:
        proc = subprocess.run(
            ['gcc', '-E', '-P', filepath],
            capture_output=True, text=True, timeout=30
        )
        if proc.returncode == 0:
            return proc.stdout
    except (FileNotFoundError, subprocess.TimeoutExpired):
        pass
    return None


def detect_and_parse(content, is_nvram=False):
    """Detect format and parse .h file content to raw bytes."""
    if re.search(r'(static\s+const\s+unsigned\s+char|const\s+unsigned\s+char)\s+\w+\s*\[.*\]\s*(\w+\s*)?\=\s*\{', content):
        return parse_hex_array(content)

    if is_nvram or re.search(r'(static\s+const\s+uint8_t|static\s+const\s+char)\s+\w+\s*\[.*\]\s*(\w+\s*)?\=', content):
        # Expand macros before parsing string literals for NVRAM files
        expanded = expand_macros(content)
        return parse_string_literals(expanded)

    if '{' in content and '}' in content:
        try:
            return parse_hex_array(content)
        except ValueError:
            pass
    if '"' in content:
        try:
            return parse_string_literals(expand_macros(content))
        except Exception:
            pass

    raise ValueError('Unable to detect format in .h file')


def convert_firmware_h(filepath, output_path, is_nvram=False):
    """Convert a single .h file to binary and write to output_path."""
    # Try preprocessing with gcc first for NVRAM files
    if is_nvram:
        preprocessed = preprocess_with_gcc(filepath)
        if preprocessed:
            data = detect_and_parse(preprocessed, is_nvram=True)
        else:
            with open(filepath, 'r') as f:
                content = f.read()
            data = detect_and_parse(content, is_nvram=True)
    else:
        with open(filepath, 'r') as f:
            content = f.read()
        data = detect_and_parse(content, is_nvram=False)

    with open(output_path, 'wb') as f:
        f.write(data)

    return len(data)


def main():
    parser = argparse.ArgumentParser(
        description='Convert CYW43 firmware .h header files to raw binary .bin files.'
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
            size = convert_firmware_h(h_path, out_path, is_nvram=is_nvram)
            print(f'  {bin_name}: {size} bytes ({size / 1024:.1f} KB)')
        except Exception as e:
            print(f'  ERROR converting {h_path}: {e}', file=sys.stderr)
            sys.exit(1)

    print()
    total = sum(os.path.getsize(os.path.join(args.output_dir, name)) for _, name, _ in files)
    print(f'Total: {total} bytes ({total / 1024:.1f} KB)')


if __name__ == '__main__':
    main()
