import requests
import xml.etree.ElementTree as ET


skipped_endings = [
    "IMG",
    "AMD",
    "AMDX",
    "ARM",
    "FSL",
    "BRCM",
    "NXP",
    "NV",
    "NVX",
    "VIV",
    "VSI",
    "KDAB",
    "ANDROID",
    "CHROMIUM",
    "FUCHSIA",
    "GGP",
    "GOOGLE",
    "QCOM",
    "LUNARG",
    "NZXT",
    "SAMSUNG",
    "SEC",
    "TIZEN",
    "RENDERDOC",
    "NN",
    "MVK",
    "KHR",
    "KHX",
    "EXT",
    "MESA",
    "INTEL",
    "HUAWEI",
    "VALVE",
    "QNX",
    "JUICE",
    "FB",
    "RASTERGRID",
    "MSFT",
        
    "vkGetCommandPoolMemoryConsumption",
    "vkGetFaultData",
    ]

exceptions = [
    "vkCreateSwapchainKHR",
    "vkGetSwapchainImagesKHR",
    "vkDestroySwapchainKHR",
    "vkDestroySurfaceKHR",
    "vkAcquireNextImageKHR",
    "vkQueuePresentKHR",
    "vkGetPhysicalDeviceSurfaceCapabilitiesKHR",
    "vkGetPhysicalDeviceSurfacePresentModesKHR",
    "vkGetPhysicalDeviceSurfaceFormatsKHR",
    "vkGetPhysicalDeviceSurfaceSupportKHR",
    ]

def fetch_xml_data(url):
    response = requests.get(url)
    response.raise_for_status()
    return response.text

def parse_xml(xml_data):
    root = ET.fromstring(xml_data)
    functions = []
    function_names = set()
    for command in root.findall('.//command'):
        proto = command.find('proto')
        if proto is not None:
            return_type_elem = proto.find('type')
            func_name_elem = proto.find('name')
            if return_type_elem is not None and func_name_elem is not None:
                return_type = return_type_elem.text.strip()
                func_name = func_name_elem.text.strip()
                if func_name in function_names:
                    continue
                params = []
                for param in command.findall('param'):
                    param_type_elem = param.find('type')
                    param_name_elem = param.find('name')
                    if param_type_elem is not None and param_name_elem is not None:
                        param_type = param_type_elem.text.strip()
                        ptr_sign = '*' if param_type_elem.tail and '*' in param_type_elem.tail else ''
                        const = 'const ' if param_type_elem.tail and 'const' in param_type_elem.tail else ''
                        param_name = param_name_elem.text.strip()
                        array = param.find('name').tail
                        if array and '[' in array and ']' in array:
                            param_type += '*'
                            param_name = param.find('name').text.strip()
                        params.append(f"{const}{param_type}{ptr_sign} {param_name}")
                functions.append({'name': func_name, 'return_type': return_type, 'params': params})
                function_names.add(func_name)
    return functions

def write_to_header_file(header_file_path, functions):
    with open(header_file_path, "w+") as file:
        file.write("#pragma once\n#define VK_NO_PROTOTYPES\n#include <Actias/System/Base.h>\n#include <Actias/System/Platform/Common/volk.h>\n\n")
        file.write("ACTIAS_BEGIN_C\n\n")
        for func in functions:
            do_skip = False
            for ending in skipped_endings:
                if func['name'].endswith(ending) and not func['name'] in exceptions:
                    do_skip = True
                    break
            if do_skip:
                continue
            params = list(dict.fromkeys(func['params']))
            params_str = ', '.join(params)
            func_declaration = f"ACTIAS_SYSTEM_API {func['return_type']} ACTIAS_ABI {func['name']}({params_str});\n"
            file.write(func_declaration + '\n')
        file.write("ACTIAS_END_C\n")

def write_to_source_file(source_file_path, functions):
    with open(source_file_path, "w+") as file:
        file.write("#include <Actias/System/ActiasVulkan.h>\n\n")
        for func in functions:
            do_skip = False
            for ending in skipped_endings:
                if func['name'].endswith(ending) and not func['name'] in exceptions:
                    do_skip = True
                    break
            if do_skip:
                continue
            params = list(dict.fromkeys(func['params']))
            params_str = ', '.join(params)
            func_declaration = f"{func['return_type']} ACTIAS_ABI {func['name']}({params_str})\n"
            file.write(func_declaration)
            file.write("{\n")
            if f"{func['return_type']}" != "void":
                param_names = ', '.join(param.split()[-1] for param in params)
                file.write(f"    return {func['name']}_volkImpl({param_names});\n")
                file.write("}\n\n")
            else:
                param_names = ', '.join(param.split()[-1] for param in params)
                file.write(f"    {func['name']}_volkImpl({param_names});\n")
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
        
    except requests.RequestException as e:
        print("An error occurred during the request:", e)
    except ET.ParseError as e:
        print("An error occurred while parsing XML:", e)
    except Exception as e:
        print("An error occurred:", e)

if __name__ == "__main__":
    main()
