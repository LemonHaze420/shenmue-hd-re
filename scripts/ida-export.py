# LemonHaze - 2025
import idautils
import idc
import ida_pro

def main():
    if len(idc.ARGV) < 2:
        ida_pro.qexit(1)

    output_path = idc.ARGV[1]
    try:
        with open(output_path, "w") as f:
            function_addrs = set(idautils.Functions())
            for ea in function_addrs:
                name = idc.get_func_name(ea)
                f.write(f"{ea - 0x140000000:X} {name}\n")
                
            for ea, name in idautils.Names():
                if ea not in function_addrs:
                    f.write(f"{ea - 0x140000000:X} {name}\n")
        ida_pro.qexit(0)

    except Exception as e:
        print(f"[!] Error: {e}")
        ida_pro.qexit(1)

main()
