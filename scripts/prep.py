# LemonHaze - 2025
import os
import re

def prepare_files(directory):
    define_pattern = re.compile(r'(#define\s+O_\w+\s+)0x[0-9A-Fa-f]+')
    hook_pattern = re.compile(r'(\{\s*".+?",\s*)0x[0-9A-Fa-f]+(,\s*.+?\s*,\s*.+?\s*\})')

    for root, _, files in os.walk(directory):
        for file in files:
            full_path = os.path.join(root, file)
            modified = False
            new_lines = []

            if file.endswith('_defines.h'):
                with open(full_path, 'r', encoding='utf-8') as f:
                    lines = f.readlines()

                for line in lines:
                    new_line = define_pattern.sub(r'\g<1>0x0', line)
                    if new_line != line:
                        modified = True
                    new_lines.append(new_line)

            elif file.endswith('_hooks.h'):
                with open(full_path, 'r', encoding='utf-8') as f:
                    lines = f.readlines()

                for line in lines:
                    new_line = hook_pattern.sub(r'\g<1>0x0\g<2>', line)
                    if new_line != line:
                        modified = True
                    new_lines.append(new_line)

            if modified:
                with open(full_path, 'w', encoding='utf-8') as f:
                    f.writelines(new_lines)

if __name__ == "__main__":
    import sys
    if len(sys.argv) == 2:
        prepare_files(sys.argv[1])
