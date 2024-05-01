def edit_header(input_file_h, output_file_h, substrings_to_replace):
    try:
        with open(input_file_h, "r") as f_in_h, open(output_file_h, "w+") as f_out_h:
            for line_h in f_in_h:
                try:
                    if "volk" in line_h or "Volk" in line_h or "typedef" in line_h or '<' in line_h:
                        f_out_h.write(line_h)
                    else:
                        parts = line_h.split()
                        if len(parts) >= 2 and "vk" in line_h:
                            if len(parts) == 2:
                                substrings_to_replace.append(parts[1].rstrip(';'))
                                parts[1] = parts[1].replace(";", "_volkImpl;")
                                f_out_h.write(" ".join(parts) + "\n")
                            elif len(parts) == 3:
                                substrings_to_replace.append(parts[2].rstrip(';'))
                                parts[2] = parts[2].replace(";", "_volkImpl;")
                                f_out_h.write(" ".join(parts) + "\n")
                        else:
                            f_out_h.write(line_h)
                except Exception as e:
                    print("Error in processing volk.h:", e)
    except Exception as e:
        print("An error occurred while processing the header file:", e)


def edit_c(input_file_c, output_file_c, substrings_to_replace):
    try:
        with open(input_file_c, "r") as f_in_c, open(output_file_c, "w+") as f_out_c:
            substrings_to_replace.sort(key=len, reverse=True)
            for line_c in f_in_c:
                try:
                    if not line_c.strip().endswith(("_volkImpl", "_volkImplStub")):
                        for substring in substrings_to_replace:
                            if substring in line_c:
                                longer_substrings = [s for s in substrings_to_replace if len(s) > len(substring)]
                                for longer_substring in longer_substrings:
                                    if longer_substring in line_c:
                                        break
                                else:
                                    line_c = line_c.replace(substring, substring + '_volkImpl')
                                    if substring + '_volkImplStub' in line_c and substring != substrings_to_replace[-1]:
                                        line_c = line_c.replace(substring + '_volkImplStub', substring + 'Stub')
                                    if f'PFN_{substring}_volkImpl' in line_c:
                                        line_c = line_c.replace(f'PFN_{substring}_volkImpl', f'PFN_{substring}')
                                    if f'"{substring}_volkImpl"':
                                        line_c = line_c.replace(f'"{substring}_volkImpl"', f'"{substring}"')
                                    break
                    f_out_c.write(line_c)
                except Exception as e:
                    print("Error in processing volk.c:", e)
    except Exception as e:
        print("An error occurred while processing the source file:", e)


def main():
    input_file_h = "./ThirdParty/Volk/volk.h"
    input_file_c = "./ThirdParty/Volk/volk.c"
    output_file_h = "./Actias/Actias/System/Platform/Common/volk.h"
    output_file_c = "./Actias/Actias/System/Platform/Common/volk.c"
    substrings_to_replace = []

    edit_header(input_file_h, output_file_h, substrings_to_replace)
    edit_c(input_file_c, output_file_c, substrings_to_replace)

if __name__ == "__main__":
    main()
