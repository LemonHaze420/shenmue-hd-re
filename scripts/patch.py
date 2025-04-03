import os
import sys
import re

def parse_def_file(def_path):
    symbols = {}
    with open(def_path, 'r') as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) >= 2:
                addr_str, name = parts[0], parts[1]
                try:
                    addr = int(addr_str, 16)
                    symbols[name] = addr
                except ValueError:
                    continue
    return symbols

def patch_hooks_file(file_path, symbols):
    updated_lines = []
    changed = False

    pattern = re.compile(r'(\{\s*"([^"]+)",\s*)0x[0-9A-Fa-f]+(\s*,.*)', re.VERBOSE)

    with open(file_path, 'r') as f:
        for line in f:
            match = pattern.search(line)
            if match:
                full_prefix = match.group(1)
                symbol_name = match.group(2)
                suffix = match.group(3)

                if symbol_name in symbols:
                    new_offset = f"0x{symbols[symbol_name]:X}"
                    new_line = f"{full_prefix}{new_offset}{suffix}\n"
                    updated_lines.append(new_line)
                    changed = True
                    continue

            updated_lines.append(line)

    if changed:
        with open(file_path, 'w') as f:
            f.writelines(updated_lines)
        print(f"[+] Patched hooks: {file_path}")
    else:
        print(f"[-] No changes (hooks): {file_path}")

def patch_defines_file(file_path, symbols):
    updated_lines = []
    changed = False

    pattern = re.compile(r'#define\s+O_(\w+)\s+0x[0-9A-Fa-f]+')

    with open(file_path, 'r') as f:
        for line in f:
            match = pattern.match(line)
            if match:
                symbol_suffix = match.group(1)
                if symbol_suffix in symbols:
                    new_offset = f"0x{symbols[symbol_suffix]:X}"
                    new_line = f"#define O_{symbol_suffix} {new_offset}\n"
                    updated_lines.append(new_line)
                    changed = True
                    continue

            updated_lines.append(line)

    if changed:
        with open(file_path, 'w') as f:
            f.writelines(updated_lines)
        print(f"[+] Patched defines: {file_path}")
    else:
        print(f"[-] No changes (defines): {file_path}")

def main():
    if len(sys.argv) != 3:
        print("Usage: patch.py <sm1|sm2> <directory>")
        sys.exit(1)

    target = sys.argv[1]
    root_dir = sys.argv[2]

    if target not in ("sm1", "sm2"):
        print("Target must be 'sm1' or 'sm2'")
        sys.exit(1)

    def_path = os.path.join(os.path.dirname(__file__), f"..\\{target}.def")

    if not os.path.exists(def_path):
        print(f"DEF file not found: {def_path}")
        sys.exit(1)

    symbols = parse_def_file(def_path)

    for root, _, files in os.walk(root_dir):
        for file in files:
            full_path = os.path.join(root, file)
            if file.endswith("_hooks.h"):
                patch_hooks_file(full_path, symbols)
            elif file.endswith("_defines.h"):
                patch_defines_file(full_path, symbols)

if __name__ == "__main__":
    main()
