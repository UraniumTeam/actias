import os
import requests
import xml.etree.ElementTree as ET
import textwrap

def fetch_xml_data(url):
    response = requests.get(url)
    if response.status_code == 200:
        return response.text
    else:
        raise Exception("Failed to fetch XML data")

def parse_xml(xml_data):
    root = ET.fromstring(xml_data)
    functions = []
    for command in root.findall('.//command'):
        proto = command.find('proto')
        if proto is not None:
            return_type_elem = proto.find('type')
            if return_type_elem is not None:
                return_type = return_type_elem.text.strip()
                func_name_elem = proto.find('name')
                if func_name_elem is not None:
                    func_name = func_name_elem.text.strip()
                    params = []
                    for param in command.findall('param'):
                        param_type_elem = param.find('type')
                        if param_type_elem is not None:
                            param_type = param_type_elem.text.strip()
                            # Check if there's a '*' in the type tail
                            star = '*' if param_type_elem.tail and '*' in param_type_elem.tail else ''
                            # Check if there's 'const' in the type tail
                            const = 'const ' if param_type_elem.tail and 'const' in param_type_elem.tail else ''
                            param_name_elem = param.find('name')
                            if param_name_elem is not None:
                                param_name = param_name_elem.text.strip()
                                param_text = f"{const}{param_type}{star} {param_name}"
                                params.append(param_text)
                    functions.append({'name': func_name, 'return_type': return_type, 'params': params})
    return functions


def write_to_header_file(header_file_path, functions):
    with open(header_file_path, "w+") as file:
        file.write("#define VK_NO_PROTOTYPES\n#include <vulkan/vulkan.h>\n\n")
        for func in functions:
            params_str = ', '.join(func['params'])
            func_declaration = f"ACTIAS_SYSTEM_API {func['return_type']} ACTIAS_ABI {func['name']}({params_str});"
            file.write(func_declaration + '\n\n')

def write_to_source_file(source_file_path, functions):
    with open(source_file_path, "w+") as file:
        file.write("#include <volk.h>\n\n")
        for func in functions:
            params_str = ', '.join(func['params'])
            func_declaration = f"{func['return_type']} ACTIAS_ABI {func['name']}({params_str})\n"
            file.write(func_declaration)
            file.write("{\n")
            param_names = ', '.join(param.split()[-1] for param in func['params'])
            file.write(f"    return {func['name']}_volkImpl({param_names});\n")
            file.write("}\n\n")

def main():
    xml_url = "https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/main/xml/vk.xml"
    header_file_path = "./Actias/Actias/System/ActiasVulkan.h"
    source_file_path = "./Actias/Actias/System/ActiasVulkan.c"  

    try:
        xml_data = fetch_xml_data(xml_url)
        functions = parse_xml(xml_data)

        write_to_header_file(header_file_path, functions)
        write_to_source_file(source_file_path, functions)
        
    except Exception as e:
        print("An error occurred:", e)

if __name__ == "__main__":
    main()
