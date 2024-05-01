#include <Actias/System/Base.h>
/* This file is part of volk library; see volk.h for version/license details */
/* clang-format off */
#include "volk.h"

#ifdef _WIN32
	typedef const char* LPCSTR;
	typedef struct HINSTANCE__* HINSTANCE;
	typedef HINSTANCE HMODULE;
	#if defined(_MINWINDEF_)
		/* minwindef.h defines FARPROC, and attempting to redefine it may conflict with -Wstrict-prototypes */
	#elif defined(_WIN64)
		typedef __int64 (__stdcall* FARPROC)(void);
	#else
		typedef int (__stdcall* FARPROC)(void);
	#endif
#else
#	include <dlfcn.h>
#endif

#ifdef __APPLE__
#	include <stdlib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllimport) HMODULE __stdcall LoadLibraryA(LPCSTR);
__declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE, LPCSTR);
__declspec(dllimport) int __stdcall FreeLibrary(HMODULE);
#endif

static void* loadedModule = NULL;
static VkInstance loadedInstance = VK_NULL_HANDLE;
static VkDevice loadedDevice = VK_NULL_HANDLE;

static void volkGenLoadLoader(void* context, PFN_vkVoidFunction (*load)(void*, const char*));
static void volkGenLoadInstance(void* context, PFN_vkVoidFunction (*load)(void*, const char*));
static void volkGenLoadDevice(void* context, PFN_vkVoidFunction (*load)(void*, const char*));
static void volkGenLoadDeviceTable(struct VolkDeviceTable* table, void* context, PFN_vkVoidFunction (*load)(void*, const char*));

static PFN_vkVoidFunction vkGetInstanceProcAddrStub(void* context, const char* name)
{
	return vkGetInstanceProcAddr_volkImpl((VkInstance)context, name);
}

static PFN_vkVoidFunction vkGetDeviceProcAddrStub(void* context, const char* name)
{
	return vkGetDeviceProcAddr_volkImpl((VkDevice)context, name);
}

static PFN_vkVoidFunction nullProcAddrStub(void* context, const char* name)
{
	(void)context;
	(void)name;
	return NULL;
}

VkResult volkInitialize(void)
{
#if defined(_WIN32)
	HMODULE module = LoadLibraryA("vulkan-1.dll");
	if (!module)
		return VK_ERROR_INITIALIZATION_FAILED;

	// note: function pointer is cast through void function pointer to silence cast-function-type warning on gcc8
	vkGetInstanceProcAddr_volkImpl = (PFN_vkGetInstanceProcAddr)(void(*)(void))GetProcAddress(module, "vkGetInstanceProcAddr");
#elif defined(__APPLE__)
	void* module = dlopen("libvulkan.dylib", RTLD_NOW | RTLD_LOCAL);
	if (!module)
		module = dlopen("libvulkan.1.dylib", RTLD_NOW | RTLD_LOCAL);
	if (!module)
		module = dlopen("libMoltenVK.dylib", RTLD_NOW | RTLD_LOCAL);
    // Add support for using Vulkan and MoltenVK in a Framework. App store rules for iOS
    // strictly enforce no .dylib's. If they aren't found it just falls through
    if (!module)
        module = dlopen("vulkan.framework/vulkan", RTLD_NOW | RTLD_LOCAL);
    if (!module)
        module = dlopen("MoltenVK.framework/MoltenVK", RTLD_NOW | RTLD_LOCAL);
	// modern versions of macOS don't search /usr/local/lib automatically contrary to what man dlopen says
	// Vulkan SDK uses this as the system-wide installation location, so we're going to fallback to this if all else fails
	if (!module && getenv("DYLD_FALLBACK_LIBRARY_PATH") == NULL)
		module = dlopen("/usr/local/lib/libvulkan.dylib", RTLD_NOW | RTLD_LOCAL);
	if (!module)
		return VK_ERROR_INITIALIZATION_FAILED;

	vkGetInstanceProcAddr_volkImpl = (PFN_vkGetInstanceProcAddr)dlsym(module, "vkGetInstanceProcAddr");
#else
	void* module = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
	if (!module)
		module = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
	if (!module)
		return VK_ERROR_INITIALIZATION_FAILED;

	vkGetInstanceProcAddr_volkImpl = (PFN_vkGetInstanceProcAddr)dlsym(module, "vkGetInstanceProcAddr");
#endif

	loadedModule = module;
	volkGenLoadLoader(NULL, vkGetInstanceProcAddrStub);

	return VK_SUCCESS;
}

void volkInitializeCustom(PFN_vkGetInstanceProcAddr handler)
{
	vkGetInstanceProcAddr_volkImpl = handler;

	loadedModule = NULL;
	volkGenLoadLoader(NULL, vkGetInstanceProcAddrStub);
}

void volkFinalize(void)
{
	if (loadedModule)
	{
#if defined(_WIN32)
		FreeLibrary((HMODULE)loadedModule);
#else
		dlclose(loadedModule);
#endif
	}

	vkGetInstanceProcAddr_volkImpl = NULL;
	volkGenLoadLoader(NULL, nullProcAddrStub);
	volkGenLoadInstance(NULL, nullProcAddrStub);
	volkGenLoadDevice(NULL, nullProcAddrStub);

	loadedModule = NULL;
	loadedInstance = VK_NULL_HANDLE;
	loadedDevice = VK_NULL_HANDLE;
}

uint32_t volkGetInstanceVersion(void)
{
#if defined(VK_VERSION_1_1)
	uint32_t apiVersion = 0;
	if (vkEnumerateInstanceVersion_volkImpl && vkEnumerateInstanceVersion_volkImpl(&apiVersion) == VK_SUCCESS)
		return apiVersion;
#endif

	if (vkCreateInstance_volkImpl)
		return VK_API_VERSION_1_0;

	return 0;
}

void volkLoadInstance(VkInstance instance)
{
	loadedInstance = instance;
	volkGenLoadInstance(instance, vkGetInstanceProcAddrStub);
	volkGenLoadDevice(instance, vkGetInstanceProcAddrStub);
}

void volkLoadInstanceOnly(VkInstance instance)
{
	loadedInstance = instance;
	volkGenLoadInstance(instance, vkGetInstanceProcAddrStub);
}

VkInstance volkGetLoadedInstance(void)
{
	return loadedInstance;
}

void volkLoadDevice(VkDevice device)
{
	loadedDevice = device;
	volkGenLoadDevice(device, vkGetDeviceProcAddrStub);
}

VkDevice volkGetLoadedDevice(void)
{
	return loadedDevice;
}

void volkLoadDeviceTable(struct VolkDeviceTable* table, VkDevice device)
{
	volkGenLoadDeviceTable(table, device, vkGetDeviceProcAddrStub);
}

static void volkGenLoadLoader(void* context, PFN_vkVoidFunction (*load)(void*, const char*))
{
	/* VOLK_GENERATE_LOAD_LOADER */
#if defined(VK_VERSION_1_0)
	vkCreateInstance_volkImpl = (PFN_vkCreateInstance)load(context, "vkCreateInstance");
	vkEnumerateInstanceExtensionProperties_volkImpl = (PFN_vkEnumerateInstanceExtensionProperties)load(context, "vkEnumerateInstanceExtensionProperties");
	vkEnumerateInstanceLayerProperties_volkImpl = (PFN_vkEnumerateInstanceLayerProperties)load(context, "vkEnumerateInstanceLayerProperties");
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
	vkEnumerateInstanceVersion_volkImpl = (PFN_vkEnumerateInstanceVersion)load(context, "vkEnumerateInstanceVersion");
#endif /* defined(VK_VERSION_1_1) */
	/* VOLK_GENERATE_LOAD_LOADER */
}

static void volkGenLoadInstance(void* context, PFN_vkVoidFunction (*load)(void*, const char*))
{
	/* VOLK_GENERATE_LOAD_INSTANCE */
#if defined(VK_VERSION_1_0)
	vkCreateDevice_volkImpl = (PFN_vkCreateDevice)load(context, "vkCreateDevice");
	vkDestroyInstance_volkImpl = (PFN_vkDestroyInstance)load(context, "vkDestroyInstance");
	vkEnumerateDeviceExtensionProperties_volkImpl = (PFN_vkEnumerateDeviceExtensionProperties)load(context, "vkEnumerateDeviceExtensionProperties");
	vkEnumerateDeviceLayerProperties_volkImpl = (PFN_vkEnumerateDeviceLayerProperties)load(context, "vkEnumerateDeviceLayerProperties");
	vkEnumeratePhysicalDevices_volkImpl = (PFN_vkEnumeratePhysicalDevices)load(context, "vkEnumeratePhysicalDevices");
	vkGetDeviceProcAddr_volkImpl = (PFN_vkGetDeviceProcAddr)load(context, "vkGetDeviceProcAddr");
	vkGetPhysicalDeviceFeatures_volkImpl = (PFN_vkGetPhysicalDeviceFeatures)load(context, "vkGetPhysicalDeviceFeatures");
	vkGetPhysicalDeviceFormatProperties_volkImpl = (PFN_vkGetPhysicalDeviceFormatProperties)load(context, "vkGetPhysicalDeviceFormatProperties");
	vkGetPhysicalDeviceImageFormatProperties_volkImpl = (PFN_vkGetPhysicalDeviceImageFormatProperties)load(context, "vkGetPhysicalDeviceImageFormatProperties");
	vkGetPhysicalDeviceMemoryProperties_volkImpl = (PFN_vkGetPhysicalDeviceMemoryProperties)load(context, "vkGetPhysicalDeviceMemoryProperties");
	vkGetPhysicalDeviceProperties_volkImpl = (PFN_vkGetPhysicalDeviceProperties)load(context, "vkGetPhysicalDeviceProperties");
	vkGetPhysicalDeviceQueueFamilyProperties_volkImpl = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)load(context, "vkGetPhysicalDeviceQueueFamilyProperties");
	vkGetPhysicalDeviceSparseImageFormatProperties_volkImpl = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties)load(context, "vkGetPhysicalDeviceSparseImageFormatProperties");
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
	vkEnumeratePhysicalDeviceGroups_volkImpl = (PFN_vkEnumeratePhysicalDeviceGroups)load(context, "vkEnumeratePhysicalDeviceGroups");
	vkGetPhysicalDeviceExternalBufferProperties_volkImpl = (PFN_vkGetPhysicalDeviceExternalBufferProperties)load(context, "vkGetPhysicalDeviceExternalBufferProperties");
	vkGetPhysicalDeviceExternalFenceProperties_volkImpl = (PFN_vkGetPhysicalDeviceExternalFenceProperties)load(context, "vkGetPhysicalDeviceExternalFenceProperties");
	vkGetPhysicalDeviceExternalSemaphoreProperties_volkImpl = (PFN_vkGetPhysicalDeviceExternalSemaphoreProperties)load(context, "vkGetPhysicalDeviceExternalSemaphoreProperties");
	vkGetPhysicalDeviceFeatures2_volkImpl = (PFN_vkGetPhysicalDeviceFeatures2)load(context, "vkGetPhysicalDeviceFeatures2");
	vkGetPhysicalDeviceFormatProperties2_volkImpl = (PFN_vkGetPhysicalDeviceFormatProperties2)load(context, "vkGetPhysicalDeviceFormatProperties2");
	vkGetPhysicalDeviceImageFormatProperties2_volkImpl = (PFN_vkGetPhysicalDeviceImageFormatProperties2)load(context, "vkGetPhysicalDeviceImageFormatProperties2");
	vkGetPhysicalDeviceMemoryProperties2_volkImpl = (PFN_vkGetPhysicalDeviceMemoryProperties2)load(context, "vkGetPhysicalDeviceMemoryProperties2");
	vkGetPhysicalDeviceProperties2_volkImpl = (PFN_vkGetPhysicalDeviceProperties2)load(context, "vkGetPhysicalDeviceProperties2");
	vkGetPhysicalDeviceQueueFamilyProperties2_volkImpl = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2)load(context, "vkGetPhysicalDeviceQueueFamilyProperties2");
	vkGetPhysicalDeviceSparseImageFormatProperties2_volkImpl = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2)load(context, "vkGetPhysicalDeviceSparseImageFormatProperties2");
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_3)
	vkGetPhysicalDeviceToolProperties_volkImpl = (PFN_vkGetPhysicalDeviceToolProperties)load(context, "vkGetPhysicalDeviceToolProperties");
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_EXT_acquire_drm_display)
	vkAcquireDrmDisplayEXT_volkImpl = (PFN_vkAcquireDrmDisplayEXT)load(context, "vkAcquireDrmDisplayEXT");
	vkGetDrmDisplayEXT_volkImpl = (PFN_vkGetDrmDisplayEXT)load(context, "vkGetDrmDisplayEXT");
#endif /* defined(VK_EXT_acquire_drm_display) */
#if defined(VK_EXT_acquire_xlib_display)
	vkAcquireXlibDisplayEXT_volkImpl = (PFN_vkAcquireXlibDisplayEXT)load(context, "vkAcquireXlibDisplayEXT");
	vkGetRandROutputDisplayEXT_volkImpl = (PFN_vkGetRandROutputDisplayEXT)load(context, "vkGetRandROutputDisplayEXT");
#endif /* defined(VK_EXT_acquire_xlib_display) */
#if defined(VK_EXT_calibrated_timestamps)
	vkGetPhysicalDeviceCalibrateableTimeDomainsEXT_volkImpl = (PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT)load(context, "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT");
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_debug_report)
	vkCreateDebugReportCallbackEXT_volkImpl = (PFN_vkCreateDebugReportCallbackEXT)load(context, "vkCreateDebugReportCallbackEXT");
	vkDebugReportMessageEXT_volkImpl = (PFN_vkDebugReportMessageEXT)load(context, "vkDebugReportMessageEXT");
	vkDestroyDebugReportCallbackEXT_volkImpl = (PFN_vkDestroyDebugReportCallbackEXT)load(context, "vkDestroyDebugReportCallbackEXT");
#endif /* defined(VK_EXT_debug_report) */
#if defined(VK_EXT_debug_utils)
	vkCmdBeginDebugUtilsLabelEXT_volkImpl = (PFN_vkCmdBeginDebugUtilsLabelEXT)load(context, "vkCmdBeginDebugUtilsLabelEXT");
	vkCmdEndDebugUtilsLabelEXT_volkImpl = (PFN_vkCmdEndDebugUtilsLabelEXT)load(context, "vkCmdEndDebugUtilsLabelEXT");
	vkCmdInsertDebugUtilsLabelEXT_volkImpl = (PFN_vkCmdInsertDebugUtilsLabelEXT)load(context, "vkCmdInsertDebugUtilsLabelEXT");
	vkCreateDebugUtilsMessengerEXT_volkImpl = (PFN_vkCreateDebugUtilsMessengerEXT)load(context, "vkCreateDebugUtilsMessengerEXT");
	vkDestroyDebugUtilsMessengerEXT_volkImpl = (PFN_vkDestroyDebugUtilsMessengerEXT)load(context, "vkDestroyDebugUtilsMessengerEXT");
	vkQueueBeginDebugUtilsLabelEXT_volkImpl = (PFN_vkQueueBeginDebugUtilsLabelEXT)load(context, "vkQueueBeginDebugUtilsLabelEXT");
	vkQueueEndDebugUtilsLabelEXT_volkImpl = (PFN_vkQueueEndDebugUtilsLabelEXT)load(context, "vkQueueEndDebugUtilsLabelEXT");
	vkQueueInsertDebugUtilsLabelEXT_volkImpl = (PFN_vkQueueInsertDebugUtilsLabelEXT)load(context, "vkQueueInsertDebugUtilsLabelEXT");
	vkSetDebugUtilsObjectNameEXT_volkImpl = (PFN_vkSetDebugUtilsObjectNameEXT)load(context, "vkSetDebugUtilsObjectNameEXT");
	vkSetDebugUtilsObjectTagEXT_volkImpl = (PFN_vkSetDebugUtilsObjectTagEXT)load(context, "vkSetDebugUtilsObjectTagEXT");
	vkSubmitDebugUtilsMessageEXT_volkImpl = (PFN_vkSubmitDebugUtilsMessageEXT)load(context, "vkSubmitDebugUtilsMessageEXT");
#endif /* defined(VK_EXT_debug_utils) */
#if defined(VK_EXT_direct_mode_display)
	vkReleaseDisplayEXT_volkImpl = (PFN_vkReleaseDisplayEXT)load(context, "vkReleaseDisplayEXT");
#endif /* defined(VK_EXT_direct_mode_display) */
#if defined(VK_EXT_directfb_surface)
	vkCreateDirectFBSurfaceEXT_volkImpl = (PFN_vkCreateDirectFBSurfaceEXT)load(context, "vkCreateDirectFBSurfaceEXT");
	vkGetPhysicalDeviceDirectFBPresentationSupportEXT_volkImpl = (PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT)load(context, "vkGetPhysicalDeviceDirectFBPresentationSupportEXT");
#endif /* defined(VK_EXT_directfb_surface) */
#if defined(VK_EXT_display_surface_counter)
	vkGetPhysicalDeviceSurfaceCapabilities2EXT_volkImpl = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT)load(context, "vkGetPhysicalDeviceSurfaceCapabilities2EXT");
#endif /* defined(VK_EXT_display_surface_counter) */
#if defined(VK_EXT_full_screen_exclusive)
	vkGetPhysicalDeviceSurfacePresentModes2EXT_volkImpl = (PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT)load(context, "vkGetPhysicalDeviceSurfacePresentModes2EXT");
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_headless_surface)
	vkCreateHeadlessSurfaceEXT_volkImpl = (PFN_vkCreateHeadlessSurfaceEXT)load(context, "vkCreateHeadlessSurfaceEXT");
#endif /* defined(VK_EXT_headless_surface) */
#if defined(VK_EXT_metal_surface)
	vkCreateMetalSurfaceEXT_volkImpl = (PFN_vkCreateMetalSurfaceEXT)load(context, "vkCreateMetalSurfaceEXT");
#endif /* defined(VK_EXT_metal_surface) */
#if defined(VK_EXT_sample_locations)
	vkGetPhysicalDeviceMultisamplePropertiesEXT_volkImpl = (PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT)load(context, "vkGetPhysicalDeviceMultisamplePropertiesEXT");
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_tooling_info)
	vkGetPhysicalDeviceToolPropertiesEXT_volkImpl = (PFN_vkGetPhysicalDeviceToolPropertiesEXT)load(context, "vkGetPhysicalDeviceToolPropertiesEXT");
#endif /* defined(VK_EXT_tooling_info) */
#if defined(VK_FUCHSIA_imagepipe_surface)
	vkCreateImagePipeSurfaceFUCHSIA_volkImpl = (PFN_vkCreateImagePipeSurfaceFUCHSIA)load(context, "vkCreateImagePipeSurfaceFUCHSIA");
#endif /* defined(VK_FUCHSIA_imagepipe_surface) */
#if defined(VK_GGP_stream_descriptor_surface)
	vkCreateStreamDescriptorSurfaceGGP_volkImpl = (PFN_vkCreateStreamDescriptorSurfaceGGP)load(context, "vkCreateStreamDescriptorSurfaceGGP");
#endif /* defined(VK_GGP_stream_descriptor_surface) */
#if defined(VK_KHR_android_surface)
	vkCreateAndroidSurfaceKHR_volkImpl = (PFN_vkCreateAndroidSurfaceKHR)load(context, "vkCreateAndroidSurfaceKHR");
#endif /* defined(VK_KHR_android_surface) */
#if defined(VK_KHR_calibrated_timestamps)
	vkGetPhysicalDeviceCalibrateableTimeDomainsKHR_volkImpl = (PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR)load(context, "vkGetPhysicalDeviceCalibrateableTimeDomainsKHR");
#endif /* defined(VK_KHR_calibrated_timestamps) */
#if defined(VK_KHR_cooperative_matrix)
	vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR)load(context, "vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR");
#endif /* defined(VK_KHR_cooperative_matrix) */
#if defined(VK_KHR_device_group_creation)
	vkEnumeratePhysicalDeviceGroupsKHR_volkImpl = (PFN_vkEnumeratePhysicalDeviceGroupsKHR)load(context, "vkEnumeratePhysicalDeviceGroupsKHR");
#endif /* defined(VK_KHR_device_group_creation) */
#if defined(VK_KHR_display)
	vkCreateDisplayModeKHR_volkImpl = (PFN_vkCreateDisplayModeKHR)load(context, "vkCreateDisplayModeKHR");
	vkCreateDisplayPlaneSurfaceKHR_volkImpl = (PFN_vkCreateDisplayPlaneSurfaceKHR)load(context, "vkCreateDisplayPlaneSurfaceKHR");
	vkGetDisplayModePropertiesKHR_volkImpl = (PFN_vkGetDisplayModePropertiesKHR)load(context, "vkGetDisplayModePropertiesKHR");
	vkGetDisplayPlaneCapabilitiesKHR_volkImpl = (PFN_vkGetDisplayPlaneCapabilitiesKHR)load(context, "vkGetDisplayPlaneCapabilitiesKHR");
	vkGetDisplayPlaneSupportedDisplaysKHR_volkImpl = (PFN_vkGetDisplayPlaneSupportedDisplaysKHR)load(context, "vkGetDisplayPlaneSupportedDisplaysKHR");
	vkGetPhysicalDeviceDisplayPlanePropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR)load(context, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR");
	vkGetPhysicalDeviceDisplayPropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceDisplayPropertiesKHR)load(context, "vkGetPhysicalDeviceDisplayPropertiesKHR");
#endif /* defined(VK_KHR_display) */
#if defined(VK_KHR_external_fence_capabilities)
	vkGetPhysicalDeviceExternalFencePropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR)load(context, "vkGetPhysicalDeviceExternalFencePropertiesKHR");
#endif /* defined(VK_KHR_external_fence_capabilities) */
#if defined(VK_KHR_external_memory_capabilities)
	vkGetPhysicalDeviceExternalBufferPropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR)load(context, "vkGetPhysicalDeviceExternalBufferPropertiesKHR");
#endif /* defined(VK_KHR_external_memory_capabilities) */
#if defined(VK_KHR_external_semaphore_capabilities)
	vkGetPhysicalDeviceExternalSemaphorePropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR)load(context, "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR");
#endif /* defined(VK_KHR_external_semaphore_capabilities) */
#if defined(VK_KHR_fragment_shading_rate)
	vkGetPhysicalDeviceFragmentShadingRatesKHR_volkImpl = (PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR)load(context, "vkGetPhysicalDeviceFragmentShadingRatesKHR");
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_display_properties2)
	vkGetDisplayModeProperties2KHR_volkImpl = (PFN_vkGetDisplayModeProperties2KHR)load(context, "vkGetDisplayModeProperties2KHR");
	vkGetDisplayPlaneCapabilities2KHR_volkImpl = (PFN_vkGetDisplayPlaneCapabilities2KHR)load(context, "vkGetDisplayPlaneCapabilities2KHR");
	vkGetPhysicalDeviceDisplayPlaneProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR)load(context, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR");
	vkGetPhysicalDeviceDisplayProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceDisplayProperties2KHR)load(context, "vkGetPhysicalDeviceDisplayProperties2KHR");
#endif /* defined(VK_KHR_get_display_properties2) */
#if defined(VK_KHR_get_physical_device_properties2)
	vkGetPhysicalDeviceFeatures2KHR_volkImpl = (PFN_vkGetPhysicalDeviceFeatures2KHR)load(context, "vkGetPhysicalDeviceFeatures2KHR");
	vkGetPhysicalDeviceFormatProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceFormatProperties2KHR)load(context, "vkGetPhysicalDeviceFormatProperties2KHR");
	vkGetPhysicalDeviceImageFormatProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceImageFormatProperties2KHR)load(context, "vkGetPhysicalDeviceImageFormatProperties2KHR");
	vkGetPhysicalDeviceMemoryProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceMemoryProperties2KHR)load(context, "vkGetPhysicalDeviceMemoryProperties2KHR");
	vkGetPhysicalDeviceProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceProperties2KHR)load(context, "vkGetPhysicalDeviceProperties2KHR");
	vkGetPhysicalDeviceQueueFamilyProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR)load(context, "vkGetPhysicalDeviceQueueFamilyProperties2KHR");
	vkGetPhysicalDeviceSparseImageFormatProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR)load(context, "vkGetPhysicalDeviceSparseImageFormatProperties2KHR");
#endif /* defined(VK_KHR_get_physical_device_properties2) */
#if defined(VK_KHR_get_surface_capabilities2)
	vkGetPhysicalDeviceSurfaceCapabilities2KHR_volkImpl = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR)load(context, "vkGetPhysicalDeviceSurfaceCapabilities2KHR");
	vkGetPhysicalDeviceSurfaceFormats2KHR_volkImpl = (PFN_vkGetPhysicalDeviceSurfaceFormats2KHR)load(context, "vkGetPhysicalDeviceSurfaceFormats2KHR");
#endif /* defined(VK_KHR_get_surface_capabilities2) */
#if defined(VK_KHR_performance_query)
	vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR_volkImpl = (PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR)load(context, "vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR");
	vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR_volkImpl = (PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR)load(context, "vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR");
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_surface)
	vkDestroySurfaceKHR_volkImpl = (PFN_vkDestroySurfaceKHR)load(context, "vkDestroySurfaceKHR");
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)load(context, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
	vkGetPhysicalDeviceSurfaceFormatsKHR_volkImpl = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)load(context, "vkGetPhysicalDeviceSurfaceFormatsKHR");
	vkGetPhysicalDeviceSurfacePresentModesKHR_volkImpl = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)load(context, "vkGetPhysicalDeviceSurfacePresentModesKHR");
	vkGetPhysicalDeviceSurfaceSupportKHR_volkImpl = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)load(context, "vkGetPhysicalDeviceSurfaceSupportKHR");
#endif /* defined(VK_KHR_surface) */
#if defined(VK_KHR_video_encode_queue)
	vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR)load(context, "vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR");
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
	vkGetPhysicalDeviceVideoCapabilitiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR)load(context, "vkGetPhysicalDeviceVideoCapabilitiesKHR");
	vkGetPhysicalDeviceVideoFormatPropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR)load(context, "vkGetPhysicalDeviceVideoFormatPropertiesKHR");
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_KHR_wayland_surface)
	vkCreateWaylandSurfaceKHR_volkImpl = (PFN_vkCreateWaylandSurfaceKHR)load(context, "vkCreateWaylandSurfaceKHR");
	vkGetPhysicalDeviceWaylandPresentationSupportKHR_volkImpl = (PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR)load(context, "vkGetPhysicalDeviceWaylandPresentationSupportKHR");
#endif /* defined(VK_KHR_wayland_surface) */
#if defined(VK_KHR_win32_surface)
	vkCreateWin32SurfaceKHR_volkImpl = (PFN_vkCreateWin32SurfaceKHR)load(context, "vkCreateWin32SurfaceKHR");
	vkGetPhysicalDeviceWin32PresentationSupportKHR_volkImpl = (PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)load(context, "vkGetPhysicalDeviceWin32PresentationSupportKHR");
#endif /* defined(VK_KHR_win32_surface) */
#if defined(VK_KHR_xcb_surface)
	vkCreateXcbSurfaceKHR_volkImpl = (PFN_vkCreateXcbSurfaceKHR)load(context, "vkCreateXcbSurfaceKHR");
	vkGetPhysicalDeviceXcbPresentationSupportKHR_volkImpl = (PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)load(context, "vkGetPhysicalDeviceXcbPresentationSupportKHR");
#endif /* defined(VK_KHR_xcb_surface) */
#if defined(VK_KHR_xlib_surface)
	vkCreateXlibSurfaceKHR_volkImpl = (PFN_vkCreateXlibSurfaceKHR)load(context, "vkCreateXlibSurfaceKHR");
	vkGetPhysicalDeviceXlibPresentationSupportKHR_volkImpl = (PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)load(context, "vkGetPhysicalDeviceXlibPresentationSupportKHR");
#endif /* defined(VK_KHR_xlib_surface) */
#if defined(VK_MVK_ios_surface)
	vkCreateIOSSurfaceMVK_volkImpl = (PFN_vkCreateIOSSurfaceMVK)load(context, "vkCreateIOSSurfaceMVK");
#endif /* defined(VK_MVK_ios_surface) */
#if defined(VK_MVK_macos_surface)
	vkCreateMacOSSurfaceMVK_volkImpl = (PFN_vkCreateMacOSSurfaceMVK)load(context, "vkCreateMacOSSurfaceMVK");
#endif /* defined(VK_MVK_macos_surface) */
#if defined(VK_NN_vi_surface)
	vkCreateViSurfaceNN_volkImpl = (PFN_vkCreateViSurfaceNN)load(context, "vkCreateViSurfaceNN");
#endif /* defined(VK_NN_vi_surface) */
#if defined(VK_NV_acquire_winrt_display)
	vkAcquireWinrtDisplayNV_volkImpl = (PFN_vkAcquireWinrtDisplayNV)load(context, "vkAcquireWinrtDisplayNV");
	vkGetWinrtDisplayNV_volkImpl = (PFN_vkGetWinrtDisplayNV)load(context, "vkGetWinrtDisplayNV");
#endif /* defined(VK_NV_acquire_winrt_display) */
#if defined(VK_NV_cooperative_matrix)
	vkGetPhysicalDeviceCooperativeMatrixPropertiesNV_volkImpl = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV)load(context, "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV");
#endif /* defined(VK_NV_cooperative_matrix) */
#if defined(VK_NV_coverage_reduction_mode)
	vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV_volkImpl = (PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV)load(context, "vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV");
#endif /* defined(VK_NV_coverage_reduction_mode) */
#if defined(VK_NV_external_memory_capabilities)
	vkGetPhysicalDeviceExternalImageFormatPropertiesNV_volkImpl = (PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV)load(context, "vkGetPhysicalDeviceExternalImageFormatPropertiesNV");
#endif /* defined(VK_NV_external_memory_capabilities) */
#if defined(VK_NV_optical_flow)
	vkGetPhysicalDeviceOpticalFlowImageFormatsNV_volkImpl = (PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV)load(context, "vkGetPhysicalDeviceOpticalFlowImageFormatsNV");
#endif /* defined(VK_NV_optical_flow) */
#if defined(VK_QNX_screen_surface)
	vkCreateScreenSurfaceQNX_volkImpl = (PFN_vkCreateScreenSurfaceQNX)load(context, "vkCreateScreenSurfaceQNX");
	vkGetPhysicalDeviceScreenPresentationSupportQNX_volkImpl = (PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX)load(context, "vkGetPhysicalDeviceScreenPresentationSupportQNX");
#endif /* defined(VK_QNX_screen_surface) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	vkGetPhysicalDevicePresentRectanglesKHR_volkImpl = (PFN_vkGetPhysicalDevicePresentRectanglesKHR)load(context, "vkGetPhysicalDevicePresentRectanglesKHR");
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
	/* VOLK_GENERATE_LOAD_INSTANCE */
}

static void volkGenLoadDevice(void* context, PFN_vkVoidFunction (*load)(void*, const char*))
{
	/* VOLK_GENERATE_LOAD_DEVICE */
#if defined(VK_VERSION_1_0)
	vkAllocateCommandBuffers_volkImpl = (PFN_vkAllocateCommandBuffers)load(context, "vkAllocateCommandBuffers");
	vkAllocateDescriptorSets_volkImpl = (PFN_vkAllocateDescriptorSets)load(context, "vkAllocateDescriptorSets");
	vkAllocateMemory_volkImpl = (PFN_vkAllocateMemory)load(context, "vkAllocateMemory");
	vkBeginCommandBuffer_volkImpl = (PFN_vkBeginCommandBuffer)load(context, "vkBeginCommandBuffer");
	vkBindBufferMemory_volkImpl = (PFN_vkBindBufferMemory)load(context, "vkBindBufferMemory");
	vkBindImageMemory_volkImpl = (PFN_vkBindImageMemory)load(context, "vkBindImageMemory");
	vkCmdBeginQuery_volkImpl = (PFN_vkCmdBeginQuery)load(context, "vkCmdBeginQuery");
	vkCmdBeginRenderPass_volkImpl = (PFN_vkCmdBeginRenderPass)load(context, "vkCmdBeginRenderPass");
	vkCmdBindDescriptorSets_volkImpl = (PFN_vkCmdBindDescriptorSets)load(context, "vkCmdBindDescriptorSets");
	vkCmdBindIndexBuffer_volkImpl = (PFN_vkCmdBindIndexBuffer)load(context, "vkCmdBindIndexBuffer");
	vkCmdBindPipeline_volkImpl = (PFN_vkCmdBindPipeline)load(context, "vkCmdBindPipeline");
	vkCmdBindVertexBuffers_volkImpl = (PFN_vkCmdBindVertexBuffers)load(context, "vkCmdBindVertexBuffers");
	vkCmdBlitImage_volkImpl = (PFN_vkCmdBlitImage)load(context, "vkCmdBlitImage");
	vkCmdClearAttachments_volkImpl = (PFN_vkCmdClearAttachments)load(context, "vkCmdClearAttachments");
	vkCmdClearColorImage_volkImpl = (PFN_vkCmdClearColorImage)load(context, "vkCmdClearColorImage");
	vkCmdClearDepthStencilImage_volkImpl = (PFN_vkCmdClearDepthStencilImage)load(context, "vkCmdClearDepthStencilImage");
	vkCmdCopyBuffer_volkImpl = (PFN_vkCmdCopyBuffer)load(context, "vkCmdCopyBuffer");
	vkCmdCopyBufferToImage_volkImpl = (PFN_vkCmdCopyBufferToImage)load(context, "vkCmdCopyBufferToImage");
	vkCmdCopyImage_volkImpl = (PFN_vkCmdCopyImage)load(context, "vkCmdCopyImage");
	vkCmdCopyImageToBuffer_volkImpl = (PFN_vkCmdCopyImageToBuffer)load(context, "vkCmdCopyImageToBuffer");
	vkCmdCopyQueryPoolResults_volkImpl = (PFN_vkCmdCopyQueryPoolResults)load(context, "vkCmdCopyQueryPoolResults");
	vkCmdDispatch_volkImpl = (PFN_vkCmdDispatch)load(context, "vkCmdDispatch");
	vkCmdDispatchIndirect_volkImpl = (PFN_vkCmdDispatchIndirect)load(context, "vkCmdDispatchIndirect");
	vkCmdDraw_volkImpl = (PFN_vkCmdDraw)load(context, "vkCmdDraw");
	vkCmdDrawIndexed_volkImpl = (PFN_vkCmdDrawIndexed)load(context, "vkCmdDrawIndexed");
	vkCmdDrawIndexedIndirect_volkImpl = (PFN_vkCmdDrawIndexedIndirect)load(context, "vkCmdDrawIndexedIndirect");
	vkCmdDrawIndirect_volkImpl = (PFN_vkCmdDrawIndirect)load(context, "vkCmdDrawIndirect");
	vkCmdEndQuery_volkImpl = (PFN_vkCmdEndQuery)load(context, "vkCmdEndQuery");
	vkCmdEndRenderPass_volkImpl = (PFN_vkCmdEndRenderPass)load(context, "vkCmdEndRenderPass");
	vkCmdExecuteCommands_volkImpl = (PFN_vkCmdExecuteCommands)load(context, "vkCmdExecuteCommands");
	vkCmdFillBuffer_volkImpl = (PFN_vkCmdFillBuffer)load(context, "vkCmdFillBuffer");
	vkCmdNextSubpass_volkImpl = (PFN_vkCmdNextSubpass)load(context, "vkCmdNextSubpass");
	vkCmdPipelineBarrier_volkImpl = (PFN_vkCmdPipelineBarrier)load(context, "vkCmdPipelineBarrier");
	vkCmdPushConstants_volkImpl = (PFN_vkCmdPushConstants)load(context, "vkCmdPushConstants");
	vkCmdResetEvent_volkImpl = (PFN_vkCmdResetEvent)load(context, "vkCmdResetEvent");
	vkCmdResetQueryPool_volkImpl = (PFN_vkCmdResetQueryPool)load(context, "vkCmdResetQueryPool");
	vkCmdResolveImage_volkImpl = (PFN_vkCmdResolveImage)load(context, "vkCmdResolveImage");
	vkCmdSetBlendConstants_volkImpl = (PFN_vkCmdSetBlendConstants)load(context, "vkCmdSetBlendConstants");
	vkCmdSetDepthBias_volkImpl = (PFN_vkCmdSetDepthBias)load(context, "vkCmdSetDepthBias");
	vkCmdSetDepthBounds_volkImpl = (PFN_vkCmdSetDepthBounds)load(context, "vkCmdSetDepthBounds");
	vkCmdSetEvent_volkImpl = (PFN_vkCmdSetEvent)load(context, "vkCmdSetEvent");
	vkCmdSetLineWidth_volkImpl = (PFN_vkCmdSetLineWidth)load(context, "vkCmdSetLineWidth");
	vkCmdSetScissor_volkImpl = (PFN_vkCmdSetScissor)load(context, "vkCmdSetScissor");
	vkCmdSetStencilCompareMask_volkImpl = (PFN_vkCmdSetStencilCompareMask)load(context, "vkCmdSetStencilCompareMask");
	vkCmdSetStencilReference_volkImpl = (PFN_vkCmdSetStencilReference)load(context, "vkCmdSetStencilReference");
	vkCmdSetStencilWriteMask_volkImpl = (PFN_vkCmdSetStencilWriteMask)load(context, "vkCmdSetStencilWriteMask");
	vkCmdSetViewport_volkImpl = (PFN_vkCmdSetViewport)load(context, "vkCmdSetViewport");
	vkCmdUpdateBuffer_volkImpl = (PFN_vkCmdUpdateBuffer)load(context, "vkCmdUpdateBuffer");
	vkCmdWaitEvents_volkImpl = (PFN_vkCmdWaitEvents)load(context, "vkCmdWaitEvents");
	vkCmdWriteTimestamp_volkImpl = (PFN_vkCmdWriteTimestamp)load(context, "vkCmdWriteTimestamp");
	vkCreateBuffer_volkImpl = (PFN_vkCreateBuffer)load(context, "vkCreateBuffer");
	vkCreateBufferView_volkImpl = (PFN_vkCreateBufferView)load(context, "vkCreateBufferView");
	vkCreateCommandPool_volkImpl = (PFN_vkCreateCommandPool)load(context, "vkCreateCommandPool");
	vkCreateComputePipelines_volkImpl = (PFN_vkCreateComputePipelines)load(context, "vkCreateComputePipelines");
	vkCreateDescriptorPool_volkImpl = (PFN_vkCreateDescriptorPool)load(context, "vkCreateDescriptorPool");
	vkCreateDescriptorSetLayout_volkImpl = (PFN_vkCreateDescriptorSetLayout)load(context, "vkCreateDescriptorSetLayout");
	vkCreateEvent_volkImpl = (PFN_vkCreateEvent)load(context, "vkCreateEvent");
	vkCreateFence_volkImpl = (PFN_vkCreateFence)load(context, "vkCreateFence");
	vkCreateFramebuffer_volkImpl = (PFN_vkCreateFramebuffer)load(context, "vkCreateFramebuffer");
	vkCreateGraphicsPipelines_volkImpl = (PFN_vkCreateGraphicsPipelines)load(context, "vkCreateGraphicsPipelines");
	vkCreateImage_volkImpl = (PFN_vkCreateImage)load(context, "vkCreateImage");
	vkCreateImageView_volkImpl = (PFN_vkCreateImageView)load(context, "vkCreateImageView");
	vkCreatePipelineCache_volkImpl = (PFN_vkCreatePipelineCache)load(context, "vkCreatePipelineCache");
	vkCreatePipelineLayout_volkImpl = (PFN_vkCreatePipelineLayout)load(context, "vkCreatePipelineLayout");
	vkCreateQueryPool_volkImpl = (PFN_vkCreateQueryPool)load(context, "vkCreateQueryPool");
	vkCreateRenderPass_volkImpl = (PFN_vkCreateRenderPass)load(context, "vkCreateRenderPass");
	vkCreateSampler_volkImpl = (PFN_vkCreateSampler)load(context, "vkCreateSampler");
	vkCreateSemaphore_volkImpl = (PFN_vkCreateSemaphore)load(context, "vkCreateSemaphore");
	vkCreateShaderModule_volkImpl = (PFN_vkCreateShaderModule)load(context, "vkCreateShaderModule");
	vkDestroyBuffer_volkImpl = (PFN_vkDestroyBuffer)load(context, "vkDestroyBuffer");
	vkDestroyBufferView_volkImpl = (PFN_vkDestroyBufferView)load(context, "vkDestroyBufferView");
	vkDestroyCommandPool_volkImpl = (PFN_vkDestroyCommandPool)load(context, "vkDestroyCommandPool");
	vkDestroyDescriptorPool_volkImpl = (PFN_vkDestroyDescriptorPool)load(context, "vkDestroyDescriptorPool");
	vkDestroyDescriptorSetLayout_volkImpl = (PFN_vkDestroyDescriptorSetLayout)load(context, "vkDestroyDescriptorSetLayout");
	vkDestroyDevice_volkImpl = (PFN_vkDestroyDevice)load(context, "vkDestroyDevice");
	vkDestroyEvent_volkImpl = (PFN_vkDestroyEvent)load(context, "vkDestroyEvent");
	vkDestroyFence_volkImpl = (PFN_vkDestroyFence)load(context, "vkDestroyFence");
	vkDestroyFramebuffer_volkImpl = (PFN_vkDestroyFramebuffer)load(context, "vkDestroyFramebuffer");
	vkDestroyImage_volkImpl = (PFN_vkDestroyImage)load(context, "vkDestroyImage");
	vkDestroyImageView_volkImpl = (PFN_vkDestroyImageView)load(context, "vkDestroyImageView");
	vkDestroyPipeline_volkImpl = (PFN_vkDestroyPipeline)load(context, "vkDestroyPipeline");
	vkDestroyPipelineCache_volkImpl = (PFN_vkDestroyPipelineCache)load(context, "vkDestroyPipelineCache");
	vkDestroyPipelineLayout_volkImpl = (PFN_vkDestroyPipelineLayout)load(context, "vkDestroyPipelineLayout");
	vkDestroyQueryPool_volkImpl = (PFN_vkDestroyQueryPool)load(context, "vkDestroyQueryPool");
	vkDestroyRenderPass_volkImpl = (PFN_vkDestroyRenderPass)load(context, "vkDestroyRenderPass");
	vkDestroySampler_volkImpl = (PFN_vkDestroySampler)load(context, "vkDestroySampler");
	vkDestroySemaphore_volkImpl = (PFN_vkDestroySemaphore)load(context, "vkDestroySemaphore");
	vkDestroyShaderModule_volkImpl = (PFN_vkDestroyShaderModule)load(context, "vkDestroyShaderModule");
	vkDeviceWaitIdle_volkImpl = (PFN_vkDeviceWaitIdle)load(context, "vkDeviceWaitIdle");
	vkEndCommandBuffer_volkImpl = (PFN_vkEndCommandBuffer)load(context, "vkEndCommandBuffer");
	vkFlushMappedMemoryRanges_volkImpl = (PFN_vkFlushMappedMemoryRanges)load(context, "vkFlushMappedMemoryRanges");
	vkFreeCommandBuffers_volkImpl = (PFN_vkFreeCommandBuffers)load(context, "vkFreeCommandBuffers");
	vkFreeDescriptorSets_volkImpl = (PFN_vkFreeDescriptorSets)load(context, "vkFreeDescriptorSets");
	vkFreeMemory_volkImpl = (PFN_vkFreeMemory)load(context, "vkFreeMemory");
	vkGetBufferMemoryRequirements_volkImpl = (PFN_vkGetBufferMemoryRequirements)load(context, "vkGetBufferMemoryRequirements");
	vkGetDeviceMemoryCommitment_volkImpl = (PFN_vkGetDeviceMemoryCommitment)load(context, "vkGetDeviceMemoryCommitment");
	vkGetDeviceQueue_volkImpl = (PFN_vkGetDeviceQueue)load(context, "vkGetDeviceQueue");
	vkGetEventStatus_volkImpl = (PFN_vkGetEventStatus)load(context, "vkGetEventStatus");
	vkGetFenceStatus_volkImpl = (PFN_vkGetFenceStatus)load(context, "vkGetFenceStatus");
	vkGetImageMemoryRequirements_volkImpl = (PFN_vkGetImageMemoryRequirements)load(context, "vkGetImageMemoryRequirements");
	vkGetImageSparseMemoryRequirements_volkImpl = (PFN_vkGetImageSparseMemoryRequirements)load(context, "vkGetImageSparseMemoryRequirements");
	vkGetImageSubresourceLayout_volkImpl = (PFN_vkGetImageSubresourceLayout)load(context, "vkGetImageSubresourceLayout");
	vkGetPipelineCacheData_volkImpl = (PFN_vkGetPipelineCacheData)load(context, "vkGetPipelineCacheData");
	vkGetQueryPoolResults_volkImpl = (PFN_vkGetQueryPoolResults)load(context, "vkGetQueryPoolResults");
	vkGetRenderAreaGranularity_volkImpl = (PFN_vkGetRenderAreaGranularity)load(context, "vkGetRenderAreaGranularity");
	vkInvalidateMappedMemoryRanges_volkImpl = (PFN_vkInvalidateMappedMemoryRanges)load(context, "vkInvalidateMappedMemoryRanges");
	vkMapMemory_volkImpl = (PFN_vkMapMemory)load(context, "vkMapMemory");
	vkMergePipelineCaches_volkImpl = (PFN_vkMergePipelineCaches)load(context, "vkMergePipelineCaches");
	vkQueueBindSparse_volkImpl = (PFN_vkQueueBindSparse)load(context, "vkQueueBindSparse");
	vkQueueSubmit_volkImpl = (PFN_vkQueueSubmit)load(context, "vkQueueSubmit");
	vkQueueWaitIdle_volkImpl = (PFN_vkQueueWaitIdle)load(context, "vkQueueWaitIdle");
	vkResetCommandBuffer_volkImpl = (PFN_vkResetCommandBuffer)load(context, "vkResetCommandBuffer");
	vkResetCommandPool_volkImpl = (PFN_vkResetCommandPool)load(context, "vkResetCommandPool");
	vkResetDescriptorPool_volkImpl = (PFN_vkResetDescriptorPool)load(context, "vkResetDescriptorPool");
	vkResetEvent_volkImpl = (PFN_vkResetEvent)load(context, "vkResetEvent");
	vkResetFences_volkImpl = (PFN_vkResetFences)load(context, "vkResetFences");
	vkSetEvent_volkImpl = (PFN_vkSetEvent)load(context, "vkSetEvent");
	vkUnmapMemory_volkImpl = (PFN_vkUnmapMemory)load(context, "vkUnmapMemory");
	vkUpdateDescriptorSets_volkImpl = (PFN_vkUpdateDescriptorSets)load(context, "vkUpdateDescriptorSets");
	vkWaitForFences_volkImpl = (PFN_vkWaitForFences)load(context, "vkWaitForFences");
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
	vkBindBufferMemory2_volkImpl = (PFN_vkBindBufferMemory2)load(context, "vkBindBufferMemory2");
	vkBindImageMemory2_volkImpl = (PFN_vkBindImageMemory2)load(context, "vkBindImageMemory2");
	vkCmdDispatchBase_volkImpl = (PFN_vkCmdDispatchBase)load(context, "vkCmdDispatchBase");
	vkCmdSetDeviceMask_volkImpl = (PFN_vkCmdSetDeviceMask)load(context, "vkCmdSetDeviceMask");
	vkCreateDescriptorUpdateTemplate_volkImpl = (PFN_vkCreateDescriptorUpdateTemplate)load(context, "vkCreateDescriptorUpdateTemplate");
	vkCreateSamplerYcbcrConversion_volkImpl = (PFN_vkCreateSamplerYcbcrConversion)load(context, "vkCreateSamplerYcbcrConversion");
	vkDestroyDescriptorUpdateTemplate_volkImpl = (PFN_vkDestroyDescriptorUpdateTemplate)load(context, "vkDestroyDescriptorUpdateTemplate");
	vkDestroySamplerYcbcrConversion_volkImpl = (PFN_vkDestroySamplerYcbcrConversion)load(context, "vkDestroySamplerYcbcrConversion");
	vkGetBufferMemoryRequirements2_volkImpl = (PFN_vkGetBufferMemoryRequirements2)load(context, "vkGetBufferMemoryRequirements2");
	vkGetDescriptorSetLayoutSupport_volkImpl = (PFN_vkGetDescriptorSetLayoutSupport)load(context, "vkGetDescriptorSetLayoutSupport");
	vkGetDeviceGroupPeerMemoryFeatures_volkImpl = (PFN_vkGetDeviceGroupPeerMemoryFeatures)load(context, "vkGetDeviceGroupPeerMemoryFeatures");
	vkGetDeviceQueue2_volkImpl = (PFN_vkGetDeviceQueue2)load(context, "vkGetDeviceQueue2");
	vkGetImageMemoryRequirements2_volkImpl = (PFN_vkGetImageMemoryRequirements2)load(context, "vkGetImageMemoryRequirements2");
	vkGetImageSparseMemoryRequirements2_volkImpl = (PFN_vkGetImageSparseMemoryRequirements2)load(context, "vkGetImageSparseMemoryRequirements2");
	vkTrimCommandPool_volkImpl = (PFN_vkTrimCommandPool)load(context, "vkTrimCommandPool");
	vkUpdateDescriptorSetWithTemplate_volkImpl = (PFN_vkUpdateDescriptorSetWithTemplate)load(context, "vkUpdateDescriptorSetWithTemplate");
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_2)
	vkCmdBeginRenderPass2_volkImpl = (PFN_vkCmdBeginRenderPass2)load(context, "vkCmdBeginRenderPass2");
	vkCmdDrawIndexedIndirectCount_volkImpl = (PFN_vkCmdDrawIndexedIndirectCount)load(context, "vkCmdDrawIndexedIndirectCount");
	vkCmdDrawIndirectCount_volkImpl = (PFN_vkCmdDrawIndirectCount)load(context, "vkCmdDrawIndirectCount");
	vkCmdEndRenderPass2_volkImpl = (PFN_vkCmdEndRenderPass2)load(context, "vkCmdEndRenderPass2");
	vkCmdNextSubpass2_volkImpl = (PFN_vkCmdNextSubpass2)load(context, "vkCmdNextSubpass2");
	vkCreateRenderPass2_volkImpl = (PFN_vkCreateRenderPass2)load(context, "vkCreateRenderPass2");
	vkGetBufferDeviceAddress_volkImpl = (PFN_vkGetBufferDeviceAddress)load(context, "vkGetBufferDeviceAddress");
	vkGetBufferOpaqueCaptureAddress_volkImpl = (PFN_vkGetBufferOpaqueCaptureAddress)load(context, "vkGetBufferOpaqueCaptureAddress");
	vkGetDeviceMemoryOpaqueCaptureAddress_volkImpl = (PFN_vkGetDeviceMemoryOpaqueCaptureAddress)load(context, "vkGetDeviceMemoryOpaqueCaptureAddress");
	vkGetSemaphoreCounterValue_volkImpl = (PFN_vkGetSemaphoreCounterValue)load(context, "vkGetSemaphoreCounterValue");
	vkResetQueryPool_volkImpl = (PFN_vkResetQueryPool)load(context, "vkResetQueryPool");
	vkSignalSemaphore_volkImpl = (PFN_vkSignalSemaphore)load(context, "vkSignalSemaphore");
	vkWaitSemaphores_volkImpl = (PFN_vkWaitSemaphores)load(context, "vkWaitSemaphores");
#endif /* defined(VK_VERSION_1_2) */
#if defined(VK_VERSION_1_3)
	vkCmdBeginRendering_volkImpl = (PFN_vkCmdBeginRendering)load(context, "vkCmdBeginRendering");
	vkCmdBindVertexBuffers2_volkImpl = (PFN_vkCmdBindVertexBuffers2)load(context, "vkCmdBindVertexBuffers2");
	vkCmdBlitImage2_volkImpl = (PFN_vkCmdBlitImage2)load(context, "vkCmdBlitImage2");
	vkCmdCopyBuffer2_volkImpl = (PFN_vkCmdCopyBuffer2)load(context, "vkCmdCopyBuffer2");
	vkCmdCopyBufferToImage2_volkImpl = (PFN_vkCmdCopyBufferToImage2)load(context, "vkCmdCopyBufferToImage2");
	vkCmdCopyImage2_volkImpl = (PFN_vkCmdCopyImage2)load(context, "vkCmdCopyImage2");
	vkCmdCopyImageToBuffer2_volkImpl = (PFN_vkCmdCopyImageToBuffer2)load(context, "vkCmdCopyImageToBuffer2");
	vkCmdEndRendering_volkImpl = (PFN_vkCmdEndRendering)load(context, "vkCmdEndRendering");
	vkCmdPipelineBarrier2_volkImpl = (PFN_vkCmdPipelineBarrier2)load(context, "vkCmdPipelineBarrier2");
	vkCmdResetEvent2_volkImpl = (PFN_vkCmdResetEvent2)load(context, "vkCmdResetEvent2");
	vkCmdResolveImage2_volkImpl = (PFN_vkCmdResolveImage2)load(context, "vkCmdResolveImage2");
	vkCmdSetCullMode_volkImpl = (PFN_vkCmdSetCullMode)load(context, "vkCmdSetCullMode");
	vkCmdSetDepthBiasEnable_volkImpl = (PFN_vkCmdSetDepthBiasEnable)load(context, "vkCmdSetDepthBiasEnable");
	vkCmdSetDepthBoundsTestEnable_volkImpl = (PFN_vkCmdSetDepthBoundsTestEnable)load(context, "vkCmdSetDepthBoundsTestEnable");
	vkCmdSetDepthCompareOp_volkImpl = (PFN_vkCmdSetDepthCompareOp)load(context, "vkCmdSetDepthCompareOp");
	vkCmdSetDepthTestEnable_volkImpl = (PFN_vkCmdSetDepthTestEnable)load(context, "vkCmdSetDepthTestEnable");
	vkCmdSetDepthWriteEnable_volkImpl = (PFN_vkCmdSetDepthWriteEnable)load(context, "vkCmdSetDepthWriteEnable");
	vkCmdSetEvent2_volkImpl = (PFN_vkCmdSetEvent2)load(context, "vkCmdSetEvent2");
	vkCmdSetFrontFace_volkImpl = (PFN_vkCmdSetFrontFace)load(context, "vkCmdSetFrontFace");
	vkCmdSetPrimitiveRestartEnable_volkImpl = (PFN_vkCmdSetPrimitiveRestartEnable)load(context, "vkCmdSetPrimitiveRestartEnable");
	vkCmdSetPrimitiveTopology_volkImpl = (PFN_vkCmdSetPrimitiveTopology)load(context, "vkCmdSetPrimitiveTopology");
	vkCmdSetRasterizerDiscardEnable_volkImpl = (PFN_vkCmdSetRasterizerDiscardEnable)load(context, "vkCmdSetRasterizerDiscardEnable");
	vkCmdSetScissorWithCount_volkImpl = (PFN_vkCmdSetScissorWithCount)load(context, "vkCmdSetScissorWithCount");
	vkCmdSetStencilOp_volkImpl = (PFN_vkCmdSetStencilOp)load(context, "vkCmdSetStencilOp");
	vkCmdSetStencilTestEnable_volkImpl = (PFN_vkCmdSetStencilTestEnable)load(context, "vkCmdSetStencilTestEnable");
	vkCmdSetViewportWithCount_volkImpl = (PFN_vkCmdSetViewportWithCount)load(context, "vkCmdSetViewportWithCount");
	vkCmdWaitEvents2_volkImpl = (PFN_vkCmdWaitEvents2)load(context, "vkCmdWaitEvents2");
	vkCmdWriteTimestamp2_volkImpl = (PFN_vkCmdWriteTimestamp2)load(context, "vkCmdWriteTimestamp2");
	vkCreatePrivateDataSlot_volkImpl = (PFN_vkCreatePrivateDataSlot)load(context, "vkCreatePrivateDataSlot");
	vkDestroyPrivateDataSlot_volkImpl = (PFN_vkDestroyPrivateDataSlot)load(context, "vkDestroyPrivateDataSlot");
	vkGetDeviceBufferMemoryRequirements_volkImpl = (PFN_vkGetDeviceBufferMemoryRequirements)load(context, "vkGetDeviceBufferMemoryRequirements");
	vkGetDeviceImageMemoryRequirements_volkImpl = (PFN_vkGetDeviceImageMemoryRequirements)load(context, "vkGetDeviceImageMemoryRequirements");
	vkGetDeviceImageSparseMemoryRequirements_volkImpl = (PFN_vkGetDeviceImageSparseMemoryRequirements)load(context, "vkGetDeviceImageSparseMemoryRequirements");
	vkGetPrivateData_volkImpl = (PFN_vkGetPrivateData)load(context, "vkGetPrivateData");
	vkQueueSubmit2_volkImpl = (PFN_vkQueueSubmit2)load(context, "vkQueueSubmit2");
	vkSetPrivateData_volkImpl = (PFN_vkSetPrivateData)load(context, "vkSetPrivateData");
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_AMDX_shader_enqueue)
	vkCmdDispatchGraphAMDX_volkImpl = (PFN_vkCmdDispatchGraphAMDX)load(context, "vkCmdDispatchGraphAMDX");
	vkCmdDispatchGraphIndirectAMDX_volkImpl = (PFN_vkCmdDispatchGraphIndirectAMDX)load(context, "vkCmdDispatchGraphIndirectAMDX");
	vkCmdDispatchGraphIndirectCountAMDX_volkImpl = (PFN_vkCmdDispatchGraphIndirectCountAMDX)load(context, "vkCmdDispatchGraphIndirectCountAMDX");
	vkCmdInitializeGraphScratchMemoryAMDX_volkImpl = (PFN_vkCmdInitializeGraphScratchMemoryAMDX)load(context, "vkCmdInitializeGraphScratchMemoryAMDX");
	vkCreateExecutionGraphPipelinesAMDX_volkImpl = (PFN_vkCreateExecutionGraphPipelinesAMDX)load(context, "vkCreateExecutionGraphPipelinesAMDX");
	vkGetExecutionGraphPipelineNodeIndexAMDX_volkImpl = (PFN_vkGetExecutionGraphPipelineNodeIndexAMDX)load(context, "vkGetExecutionGraphPipelineNodeIndexAMDX");
	vkGetExecutionGraphPipelineScratchSizeAMDX_volkImpl = (PFN_vkGetExecutionGraphPipelineScratchSizeAMDX)load(context, "vkGetExecutionGraphPipelineScratchSizeAMDX");
#endif /* defined(VK_AMDX_shader_enqueue) */
#if defined(VK_AMD_buffer_marker)
	vkCmdWriteBufferMarkerAMD_volkImpl = (PFN_vkCmdWriteBufferMarkerAMD)load(context, "vkCmdWriteBufferMarkerAMD");
#endif /* defined(VK_AMD_buffer_marker) */
#if defined(VK_AMD_display_native_hdr)
	vkSetLocalDimmingAMD_volkImpl = (PFN_vkSetLocalDimmingAMD)load(context, "vkSetLocalDimmingAMD");
#endif /* defined(VK_AMD_display_native_hdr) */
#if defined(VK_AMD_draw_indirect_count)
	vkCmdDrawIndexedIndirectCountAMD_volkImpl = (PFN_vkCmdDrawIndexedIndirectCountAMD)load(context, "vkCmdDrawIndexedIndirectCountAMD");
	vkCmdDrawIndirectCountAMD_volkImpl = (PFN_vkCmdDrawIndirectCountAMD)load(context, "vkCmdDrawIndirectCountAMD");
#endif /* defined(VK_AMD_draw_indirect_count) */
#if defined(VK_AMD_shader_info)
	vkGetShaderInfoAMD_volkImpl = (PFN_vkGetShaderInfoAMD)load(context, "vkGetShaderInfoAMD");
#endif /* defined(VK_AMD_shader_info) */
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
	vkGetAndroidHardwareBufferPropertiesANDROID_volkImpl = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID)load(context, "vkGetAndroidHardwareBufferPropertiesANDROID");
	vkGetMemoryAndroidHardwareBufferANDROID_volkImpl = (PFN_vkGetMemoryAndroidHardwareBufferANDROID)load(context, "vkGetMemoryAndroidHardwareBufferANDROID");
#endif /* defined(VK_ANDROID_external_memory_android_hardware_buffer) */
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state)
	vkCmdSetAttachmentFeedbackLoopEnableEXT_volkImpl = (PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT)load(context, "vkCmdSetAttachmentFeedbackLoopEnableEXT");
#endif /* defined(VK_EXT_attachment_feedback_loop_dynamic_state) */
#if defined(VK_EXT_buffer_device_address)
	vkGetBufferDeviceAddressEXT_volkImpl = (PFN_vkGetBufferDeviceAddressEXT)load(context, "vkGetBufferDeviceAddressEXT");
#endif /* defined(VK_EXT_buffer_device_address) */
#if defined(VK_EXT_calibrated_timestamps)
	vkGetCalibratedTimestampsEXT_volkImpl = (PFN_vkGetCalibratedTimestampsEXT)load(context, "vkGetCalibratedTimestampsEXT");
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_color_write_enable)
	vkCmdSetColorWriteEnableEXT_volkImpl = (PFN_vkCmdSetColorWriteEnableEXT)load(context, "vkCmdSetColorWriteEnableEXT");
#endif /* defined(VK_EXT_color_write_enable) */
#if defined(VK_EXT_conditional_rendering)
	vkCmdBeginConditionalRenderingEXT_volkImpl = (PFN_vkCmdBeginConditionalRenderingEXT)load(context, "vkCmdBeginConditionalRenderingEXT");
	vkCmdEndConditionalRenderingEXT_volkImpl = (PFN_vkCmdEndConditionalRenderingEXT)load(context, "vkCmdEndConditionalRenderingEXT");
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_debug_marker)
	vkCmdDebugMarkerBeginEXT_volkImpl = (PFN_vkCmdDebugMarkerBeginEXT)load(context, "vkCmdDebugMarkerBeginEXT");
	vkCmdDebugMarkerEndEXT_volkImpl = (PFN_vkCmdDebugMarkerEndEXT)load(context, "vkCmdDebugMarkerEndEXT");
	vkCmdDebugMarkerInsertEXT_volkImpl = (PFN_vkCmdDebugMarkerInsertEXT)load(context, "vkCmdDebugMarkerInsertEXT");
	vkDebugMarkerSetObjectNameEXT_volkImpl = (PFN_vkDebugMarkerSetObjectNameEXT)load(context, "vkDebugMarkerSetObjectNameEXT");
	vkDebugMarkerSetObjectTagEXT_volkImpl = (PFN_vkDebugMarkerSetObjectTagEXT)load(context, "vkDebugMarkerSetObjectTagEXT");
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_depth_bias_control)
	vkCmdSetDepthBias2EXT_volkImpl = (PFN_vkCmdSetDepthBias2EXT)load(context, "vkCmdSetDepthBias2EXT");
#endif /* defined(VK_EXT_depth_bias_control) */
#if defined(VK_EXT_descriptor_buffer)
	vkCmdBindDescriptorBufferEmbeddedSamplersEXT_volkImpl = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT)load(context, "vkCmdBindDescriptorBufferEmbeddedSamplersEXT");
	vkCmdBindDescriptorBuffersEXT_volkImpl = (PFN_vkCmdBindDescriptorBuffersEXT)load(context, "vkCmdBindDescriptorBuffersEXT");
	vkCmdSetDescriptorBufferOffsetsEXT_volkImpl = (PFN_vkCmdSetDescriptorBufferOffsetsEXT)load(context, "vkCmdSetDescriptorBufferOffsetsEXT");
	vkGetBufferOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT)load(context, "vkGetBufferOpaqueCaptureDescriptorDataEXT");
	vkGetDescriptorEXT_volkImpl = (PFN_vkGetDescriptorEXT)load(context, "vkGetDescriptorEXT");
	vkGetDescriptorSetLayoutBindingOffsetEXT_volkImpl = (PFN_vkGetDescriptorSetLayoutBindingOffsetEXT)load(context, "vkGetDescriptorSetLayoutBindingOffsetEXT");
	vkGetDescriptorSetLayoutSizeEXT_volkImpl = (PFN_vkGetDescriptorSetLayoutSizeEXT)load(context, "vkGetDescriptorSetLayoutSizeEXT");
	vkGetImageOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetImageOpaqueCaptureDescriptorDataEXT)load(context, "vkGetImageOpaqueCaptureDescriptorDataEXT");
	vkGetImageViewOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT)load(context, "vkGetImageViewOpaqueCaptureDescriptorDataEXT");
	vkGetSamplerOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT)load(context, "vkGetSamplerOpaqueCaptureDescriptorDataEXT");
#endif /* defined(VK_EXT_descriptor_buffer) */
#if defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing))
	vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT)load(context, "vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT");
#endif /* defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)) */
#if defined(VK_EXT_device_fault)
	vkGetDeviceFaultInfoEXT_volkImpl = (PFN_vkGetDeviceFaultInfoEXT)load(context, "vkGetDeviceFaultInfoEXT");
#endif /* defined(VK_EXT_device_fault) */
#if defined(VK_EXT_discard_rectangles)
	vkCmdSetDiscardRectangleEXT_volkImpl = (PFN_vkCmdSetDiscardRectangleEXT)load(context, "vkCmdSetDiscardRectangleEXT");
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2
	vkCmdSetDiscardRectangleEnableEXT_volkImpl = (PFN_vkCmdSetDiscardRectangleEnableEXT)load(context, "vkCmdSetDiscardRectangleEnableEXT");
	vkCmdSetDiscardRectangleModeEXT_volkImpl = (PFN_vkCmdSetDiscardRectangleModeEXT)load(context, "vkCmdSetDiscardRectangleModeEXT");
#endif /* defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2 */
#if defined(VK_EXT_display_control)
	vkDisplayPowerControlEXT_volkImpl = (PFN_vkDisplayPowerControlEXT)load(context, "vkDisplayPowerControlEXT");
	vkGetSwapchainCounterEXT_volkImpl = (PFN_vkGetSwapchainCounterEXT)load(context, "vkGetSwapchainCounterEXT");
	vkRegisterDeviceEventEXT_volkImpl = (PFN_vkRegisterDeviceEventEXT)load(context, "vkRegisterDeviceEventEXT");
	vkRegisterDisplayEventEXT_volkImpl = (PFN_vkRegisterDisplayEventEXT)load(context, "vkRegisterDisplayEventEXT");
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_external_memory_host)
	vkGetMemoryHostPointerPropertiesEXT_volkImpl = (PFN_vkGetMemoryHostPointerPropertiesEXT)load(context, "vkGetMemoryHostPointerPropertiesEXT");
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_full_screen_exclusive)
	vkAcquireFullScreenExclusiveModeEXT_volkImpl = (PFN_vkAcquireFullScreenExclusiveModeEXT)load(context, "vkAcquireFullScreenExclusiveModeEXT");
	vkReleaseFullScreenExclusiveModeEXT_volkImpl = (PFN_vkReleaseFullScreenExclusiveModeEXT)load(context, "vkReleaseFullScreenExclusiveModeEXT");
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_hdr_metadata)
	vkSetHdrMetadataEXT_volkImpl = (PFN_vkSetHdrMetadataEXT)load(context, "vkSetHdrMetadataEXT");
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_host_image_copy)
	vkCopyImageToImageEXT_volkImpl = (PFN_vkCopyImageToImageEXT)load(context, "vkCopyImageToImageEXT");
	vkCopyImageToMemoryEXT_volkImpl = (PFN_vkCopyImageToMemoryEXT)load(context, "vkCopyImageToMemoryEXT");
	vkCopyMemoryToImageEXT_volkImpl = (PFN_vkCopyMemoryToImageEXT)load(context, "vkCopyMemoryToImageEXT");
	vkTransitionImageLayoutEXT_volkImpl = (PFN_vkTransitionImageLayoutEXT)load(context, "vkTransitionImageLayoutEXT");
#endif /* defined(VK_EXT_host_image_copy) */
#if defined(VK_EXT_host_query_reset)
	vkResetQueryPoolEXT_volkImpl = (PFN_vkResetQueryPoolEXT)load(context, "vkResetQueryPoolEXT");
#endif /* defined(VK_EXT_host_query_reset) */
#if defined(VK_EXT_image_drm_format_modifier)
	vkGetImageDrmFormatModifierPropertiesEXT_volkImpl = (PFN_vkGetImageDrmFormatModifierPropertiesEXT)load(context, "vkGetImageDrmFormatModifierPropertiesEXT");
#endif /* defined(VK_EXT_image_drm_format_modifier) */
#if defined(VK_EXT_line_rasterization)
	vkCmdSetLineStippleEXT_volkImpl = (PFN_vkCmdSetLineStippleEXT)load(context, "vkCmdSetLineStippleEXT");
#endif /* defined(VK_EXT_line_rasterization) */
#if defined(VK_EXT_mesh_shader)
	vkCmdDrawMeshTasksEXT_volkImpl = (PFN_vkCmdDrawMeshTasksEXT)load(context, "vkCmdDrawMeshTasksEXT");
	vkCmdDrawMeshTasksIndirectCountEXT_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectCountEXT)load(context, "vkCmdDrawMeshTasksIndirectCountEXT");
	vkCmdDrawMeshTasksIndirectEXT_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectEXT)load(context, "vkCmdDrawMeshTasksIndirectEXT");
#endif /* defined(VK_EXT_mesh_shader) */
#if defined(VK_EXT_metal_objects)
	vkExportMetalObjectsEXT_volkImpl = (PFN_vkExportMetalObjectsEXT)load(context, "vkExportMetalObjectsEXT");
#endif /* defined(VK_EXT_metal_objects) */
#if defined(VK_EXT_multi_draw)
	vkCmdDrawMultiEXT_volkImpl = (PFN_vkCmdDrawMultiEXT)load(context, "vkCmdDrawMultiEXT");
	vkCmdDrawMultiIndexedEXT_volkImpl = (PFN_vkCmdDrawMultiIndexedEXT)load(context, "vkCmdDrawMultiIndexedEXT");
#endif /* defined(VK_EXT_multi_draw) */
#if defined(VK_EXT_opacity_micromap)
	vkBuildMicromapsEXT_volkImpl = (PFN_vkBuildMicromapsEXT)load(context, "vkBuildMicromapsEXT");
	vkCmdBuildMicromapsEXT_volkImpl = (PFN_vkCmdBuildMicromapsEXT)load(context, "vkCmdBuildMicromapsEXT");
	vkCmdCopyMemoryToMicromapEXT_volkImpl = (PFN_vkCmdCopyMemoryToMicromapEXT)load(context, "vkCmdCopyMemoryToMicromapEXT");
	vkCmdCopyMicromapEXT_volkImpl = (PFN_vkCmdCopyMicromapEXT)load(context, "vkCmdCopyMicromapEXT");
	vkCmdCopyMicromapToMemoryEXT_volkImpl = (PFN_vkCmdCopyMicromapToMemoryEXT)load(context, "vkCmdCopyMicromapToMemoryEXT");
	vkCmdWriteMicromapsPropertiesEXT_volkImpl = (PFN_vkCmdWriteMicromapsPropertiesEXT)load(context, "vkCmdWriteMicromapsPropertiesEXT");
	vkCopyMemoryToMicromapEXT_volkImpl = (PFN_vkCopyMemoryToMicromapEXT)load(context, "vkCopyMemoryToMicromapEXT");
	vkCopyMicromapEXT_volkImpl = (PFN_vkCopyMicromapEXT)load(context, "vkCopyMicromapEXT");
	vkCopyMicromapToMemoryEXT_volkImpl = (PFN_vkCopyMicromapToMemoryEXT)load(context, "vkCopyMicromapToMemoryEXT");
	vkCreateMicromapEXT_volkImpl = (PFN_vkCreateMicromapEXT)load(context, "vkCreateMicromapEXT");
	vkDestroyMicromapEXT_volkImpl = (PFN_vkDestroyMicromapEXT)load(context, "vkDestroyMicromapEXT");
	vkGetDeviceMicromapCompatibilityEXT_volkImpl = (PFN_vkGetDeviceMicromapCompatibilityEXT)load(context, "vkGetDeviceMicromapCompatibilityEXT");
	vkGetMicromapBuildSizesEXT_volkImpl = (PFN_vkGetMicromapBuildSizesEXT)load(context, "vkGetMicromapBuildSizesEXT");
	vkWriteMicromapsPropertiesEXT_volkImpl = (PFN_vkWriteMicromapsPropertiesEXT)load(context, "vkWriteMicromapsPropertiesEXT");
#endif /* defined(VK_EXT_opacity_micromap) */
#if defined(VK_EXT_pageable_device_local_memory)
	vkSetDeviceMemoryPriorityEXT_volkImpl = (PFN_vkSetDeviceMemoryPriorityEXT)load(context, "vkSetDeviceMemoryPriorityEXT");
#endif /* defined(VK_EXT_pageable_device_local_memory) */
#if defined(VK_EXT_pipeline_properties)
	vkGetPipelinePropertiesEXT_volkImpl = (PFN_vkGetPipelinePropertiesEXT)load(context, "vkGetPipelinePropertiesEXT");
#endif /* defined(VK_EXT_pipeline_properties) */
#if defined(VK_EXT_private_data)
	vkCreatePrivateDataSlotEXT_volkImpl = (PFN_vkCreatePrivateDataSlotEXT)load(context, "vkCreatePrivateDataSlotEXT");
	vkDestroyPrivateDataSlotEXT_volkImpl = (PFN_vkDestroyPrivateDataSlotEXT)load(context, "vkDestroyPrivateDataSlotEXT");
	vkGetPrivateDataEXT_volkImpl = (PFN_vkGetPrivateDataEXT)load(context, "vkGetPrivateDataEXT");
	vkSetPrivateDataEXT_volkImpl = (PFN_vkSetPrivateDataEXT)load(context, "vkSetPrivateDataEXT");
#endif /* defined(VK_EXT_private_data) */
#if defined(VK_EXT_sample_locations)
	vkCmdSetSampleLocationsEXT_volkImpl = (PFN_vkCmdSetSampleLocationsEXT)load(context, "vkCmdSetSampleLocationsEXT");
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_shader_module_identifier)
	vkGetShaderModuleCreateInfoIdentifierEXT_volkImpl = (PFN_vkGetShaderModuleCreateInfoIdentifierEXT)load(context, "vkGetShaderModuleCreateInfoIdentifierEXT");
	vkGetShaderModuleIdentifierEXT_volkImpl = (PFN_vkGetShaderModuleIdentifierEXT)load(context, "vkGetShaderModuleIdentifierEXT");
#endif /* defined(VK_EXT_shader_module_identifier) */
#if defined(VK_EXT_shader_object)
	vkCmdBindShadersEXT_volkImpl = (PFN_vkCmdBindShadersEXT)load(context, "vkCmdBindShadersEXT");
	vkCreateShadersEXT_volkImpl = (PFN_vkCreateShadersEXT)load(context, "vkCreateShadersEXT");
	vkDestroyShaderEXT_volkImpl = (PFN_vkDestroyShaderEXT)load(context, "vkDestroyShaderEXT");
	vkGetShaderBinaryDataEXT_volkImpl = (PFN_vkGetShaderBinaryDataEXT)load(context, "vkGetShaderBinaryDataEXT");
#endif /* defined(VK_EXT_shader_object) */
#if defined(VK_EXT_swapchain_maintenance1)
	vkReleaseSwapchainImagesEXT_volkImpl = (PFN_vkReleaseSwapchainImagesEXT)load(context, "vkReleaseSwapchainImagesEXT");
#endif /* defined(VK_EXT_swapchain_maintenance1) */
#if defined(VK_EXT_transform_feedback)
	vkCmdBeginQueryIndexedEXT_volkImpl = (PFN_vkCmdBeginQueryIndexedEXT)load(context, "vkCmdBeginQueryIndexedEXT");
	vkCmdBeginTransformFeedbackEXT_volkImpl = (PFN_vkCmdBeginTransformFeedbackEXT)load(context, "vkCmdBeginTransformFeedbackEXT");
	vkCmdBindTransformFeedbackBuffersEXT_volkImpl = (PFN_vkCmdBindTransformFeedbackBuffersEXT)load(context, "vkCmdBindTransformFeedbackBuffersEXT");
	vkCmdDrawIndirectByteCountEXT_volkImpl = (PFN_vkCmdDrawIndirectByteCountEXT)load(context, "vkCmdDrawIndirectByteCountEXT");
	vkCmdEndQueryIndexedEXT_volkImpl = (PFN_vkCmdEndQueryIndexedEXT)load(context, "vkCmdEndQueryIndexedEXT");
	vkCmdEndTransformFeedbackEXT_volkImpl = (PFN_vkCmdEndTransformFeedbackEXT)load(context, "vkCmdEndTransformFeedbackEXT");
#endif /* defined(VK_EXT_transform_feedback) */
#if defined(VK_EXT_validation_cache)
	vkCreateValidationCacheEXT_volkImpl = (PFN_vkCreateValidationCacheEXT)load(context, "vkCreateValidationCacheEXT");
	vkDestroyValidationCacheEXT_volkImpl = (PFN_vkDestroyValidationCacheEXT)load(context, "vkDestroyValidationCacheEXT");
	vkGetValidationCacheDataEXT_volkImpl = (PFN_vkGetValidationCacheDataEXT)load(context, "vkGetValidationCacheDataEXT");
	vkMergeValidationCachesEXT_volkImpl = (PFN_vkMergeValidationCachesEXT)load(context, "vkMergeValidationCachesEXT");
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_FUCHSIA_buffer_collection)
	vkCreateBufferCollectionFUCHSIA_volkImpl = (PFN_vkCreateBufferCollectionFUCHSIA)load(context, "vkCreateBufferCollectionFUCHSIA");
	vkDestroyBufferCollectionFUCHSIA_volkImpl = (PFN_vkDestroyBufferCollectionFUCHSIA)load(context, "vkDestroyBufferCollectionFUCHSIA");
	vkGetBufferCollectionPropertiesFUCHSIA_volkImpl = (PFN_vkGetBufferCollectionPropertiesFUCHSIA)load(context, "vkGetBufferCollectionPropertiesFUCHSIA");
	vkSetBufferCollectionBufferConstraintsFUCHSIA_volkImpl = (PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA)load(context, "vkSetBufferCollectionBufferConstraintsFUCHSIA");
	vkSetBufferCollectionImageConstraintsFUCHSIA_volkImpl = (PFN_vkSetBufferCollectionImageConstraintsFUCHSIA)load(context, "vkSetBufferCollectionImageConstraintsFUCHSIA");
#endif /* defined(VK_FUCHSIA_buffer_collection) */
#if defined(VK_FUCHSIA_external_memory)
	vkGetMemoryZirconHandleFUCHSIA_volkImpl = (PFN_vkGetMemoryZirconHandleFUCHSIA)load(context, "vkGetMemoryZirconHandleFUCHSIA");
	vkGetMemoryZirconHandlePropertiesFUCHSIA_volkImpl = (PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA)load(context, "vkGetMemoryZirconHandlePropertiesFUCHSIA");
#endif /* defined(VK_FUCHSIA_external_memory) */
#if defined(VK_FUCHSIA_external_semaphore)
	vkGetSemaphoreZirconHandleFUCHSIA_volkImpl = (PFN_vkGetSemaphoreZirconHandleFUCHSIA)load(context, "vkGetSemaphoreZirconHandleFUCHSIA");
	vkImportSemaphoreZirconHandleFUCHSIA_volkImpl = (PFN_vkImportSemaphoreZirconHandleFUCHSIA)load(context, "vkImportSemaphoreZirconHandleFUCHSIA");
#endif /* defined(VK_FUCHSIA_external_semaphore) */
#if defined(VK_GOOGLE_display_timing)
	vkGetPastPresentationTimingGOOGLE_volkImpl = (PFN_vkGetPastPresentationTimingGOOGLE)load(context, "vkGetPastPresentationTimingGOOGLE");
	vkGetRefreshCycleDurationGOOGLE_volkImpl = (PFN_vkGetRefreshCycleDurationGOOGLE)load(context, "vkGetRefreshCycleDurationGOOGLE");
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_HUAWEI_cluster_culling_shader)
	vkCmdDrawClusterHUAWEI_volkImpl = (PFN_vkCmdDrawClusterHUAWEI)load(context, "vkCmdDrawClusterHUAWEI");
	vkCmdDrawClusterIndirectHUAWEI_volkImpl = (PFN_vkCmdDrawClusterIndirectHUAWEI)load(context, "vkCmdDrawClusterIndirectHUAWEI");
#endif /* defined(VK_HUAWEI_cluster_culling_shader) */
#if defined(VK_HUAWEI_invocation_mask)
	vkCmdBindInvocationMaskHUAWEI_volkImpl = (PFN_vkCmdBindInvocationMaskHUAWEI)load(context, "vkCmdBindInvocationMaskHUAWEI");
#endif /* defined(VK_HUAWEI_invocation_mask) */
#if defined(VK_HUAWEI_subpass_shading)
	vkCmdSubpassShadingHUAWEI_volkImpl = (PFN_vkCmdSubpassShadingHUAWEI)load(context, "vkCmdSubpassShadingHUAWEI");
	vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI_volkImpl = (PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI)load(context, "vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI");
#endif /* defined(VK_HUAWEI_subpass_shading) */
#if defined(VK_INTEL_performance_query)
	vkAcquirePerformanceConfigurationINTEL_volkImpl = (PFN_vkAcquirePerformanceConfigurationINTEL)load(context, "vkAcquirePerformanceConfigurationINTEL");
	vkCmdSetPerformanceMarkerINTEL_volkImpl = (PFN_vkCmdSetPerformanceMarkerINTEL)load(context, "vkCmdSetPerformanceMarkerINTEL");
	vkCmdSetPerformanceOverrideINTEL_volkImpl = (PFN_vkCmdSetPerformanceOverrideINTEL)load(context, "vkCmdSetPerformanceOverrideINTEL");
	vkCmdSetPerformanceStreamMarkerINTEL_volkImpl = (PFN_vkCmdSetPerformanceStreamMarkerINTEL)load(context, "vkCmdSetPerformanceStreamMarkerINTEL");
	vkGetPerformanceParameterINTEL_volkImpl = (PFN_vkGetPerformanceParameterINTEL)load(context, "vkGetPerformanceParameterINTEL");
	vkInitializePerformanceApiINTEL_volkImpl = (PFN_vkInitializePerformanceApiINTEL)load(context, "vkInitializePerformanceApiINTEL");
	vkQueueSetPerformanceConfigurationINTEL_volkImpl = (PFN_vkQueueSetPerformanceConfigurationINTEL)load(context, "vkQueueSetPerformanceConfigurationINTEL");
	vkReleasePerformanceConfigurationINTEL_volkImpl = (PFN_vkReleasePerformanceConfigurationINTEL)load(context, "vkReleasePerformanceConfigurationINTEL");
	vkUninitializePerformanceApiINTEL_volkImpl = (PFN_vkUninitializePerformanceApiINTEL)load(context, "vkUninitializePerformanceApiINTEL");
#endif /* defined(VK_INTEL_performance_query) */
#if defined(VK_KHR_acceleration_structure)
	vkBuildAccelerationStructuresKHR_volkImpl = (PFN_vkBuildAccelerationStructuresKHR)load(context, "vkBuildAccelerationStructuresKHR");
	vkCmdBuildAccelerationStructuresIndirectKHR_volkImpl = (PFN_vkCmdBuildAccelerationStructuresIndirectKHR)load(context, "vkCmdBuildAccelerationStructuresIndirectKHR");
	vkCmdBuildAccelerationStructuresKHR_volkImpl = (PFN_vkCmdBuildAccelerationStructuresKHR)load(context, "vkCmdBuildAccelerationStructuresKHR");
	vkCmdCopyAccelerationStructureKHR_volkImpl = (PFN_vkCmdCopyAccelerationStructureKHR)load(context, "vkCmdCopyAccelerationStructureKHR");
	vkCmdCopyAccelerationStructureToMemoryKHR_volkImpl = (PFN_vkCmdCopyAccelerationStructureToMemoryKHR)load(context, "vkCmdCopyAccelerationStructureToMemoryKHR");
	vkCmdCopyMemoryToAccelerationStructureKHR_volkImpl = (PFN_vkCmdCopyMemoryToAccelerationStructureKHR)load(context, "vkCmdCopyMemoryToAccelerationStructureKHR");
	vkCmdWriteAccelerationStructuresPropertiesKHR_volkImpl = (PFN_vkCmdWriteAccelerationStructuresPropertiesKHR)load(context, "vkCmdWriteAccelerationStructuresPropertiesKHR");
	vkCopyAccelerationStructureKHR_volkImpl = (PFN_vkCopyAccelerationStructureKHR)load(context, "vkCopyAccelerationStructureKHR");
	vkCopyAccelerationStructureToMemoryKHR_volkImpl = (PFN_vkCopyAccelerationStructureToMemoryKHR)load(context, "vkCopyAccelerationStructureToMemoryKHR");
	vkCopyMemoryToAccelerationStructureKHR_volkImpl = (PFN_vkCopyMemoryToAccelerationStructureKHR)load(context, "vkCopyMemoryToAccelerationStructureKHR");
	vkCreateAccelerationStructureKHR_volkImpl = (PFN_vkCreateAccelerationStructureKHR)load(context, "vkCreateAccelerationStructureKHR");
	vkDestroyAccelerationStructureKHR_volkImpl = (PFN_vkDestroyAccelerationStructureKHR)load(context, "vkDestroyAccelerationStructureKHR");
	vkGetAccelerationStructureBuildSizesKHR_volkImpl = (PFN_vkGetAccelerationStructureBuildSizesKHR)load(context, "vkGetAccelerationStructureBuildSizesKHR");
	vkGetAccelerationStructureDeviceAddressKHR_volkImpl = (PFN_vkGetAccelerationStructureDeviceAddressKHR)load(context, "vkGetAccelerationStructureDeviceAddressKHR");
	vkGetDeviceAccelerationStructureCompatibilityKHR_volkImpl = (PFN_vkGetDeviceAccelerationStructureCompatibilityKHR)load(context, "vkGetDeviceAccelerationStructureCompatibilityKHR");
	vkWriteAccelerationStructuresPropertiesKHR_volkImpl = (PFN_vkWriteAccelerationStructuresPropertiesKHR)load(context, "vkWriteAccelerationStructuresPropertiesKHR");
#endif /* defined(VK_KHR_acceleration_structure) */
#if defined(VK_KHR_bind_memory2)
	vkBindBufferMemory2KHR_volkImpl = (PFN_vkBindBufferMemory2KHR)load(context, "vkBindBufferMemory2KHR");
	vkBindImageMemory2KHR_volkImpl = (PFN_vkBindImageMemory2KHR)load(context, "vkBindImageMemory2KHR");
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_buffer_device_address)
	vkGetBufferDeviceAddressKHR_volkImpl = (PFN_vkGetBufferDeviceAddressKHR)load(context, "vkGetBufferDeviceAddressKHR");
	vkGetBufferOpaqueCaptureAddressKHR_volkImpl = (PFN_vkGetBufferOpaqueCaptureAddressKHR)load(context, "vkGetBufferOpaqueCaptureAddressKHR");
	vkGetDeviceMemoryOpaqueCaptureAddressKHR_volkImpl = (PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR)load(context, "vkGetDeviceMemoryOpaqueCaptureAddressKHR");
#endif /* defined(VK_KHR_buffer_device_address) */
#if defined(VK_KHR_calibrated_timestamps)
	vkGetCalibratedTimestampsKHR_volkImpl = (PFN_vkGetCalibratedTimestampsKHR)load(context, "vkGetCalibratedTimestampsKHR");
#endif /* defined(VK_KHR_calibrated_timestamps) */
#if defined(VK_KHR_copy_commands2)
	vkCmdBlitImage2KHR_volkImpl = (PFN_vkCmdBlitImage2KHR)load(context, "vkCmdBlitImage2KHR");
	vkCmdCopyBuffer2KHR_volkImpl = (PFN_vkCmdCopyBuffer2KHR)load(context, "vkCmdCopyBuffer2KHR");
	vkCmdCopyBufferToImage2KHR_volkImpl = (PFN_vkCmdCopyBufferToImage2KHR)load(context, "vkCmdCopyBufferToImage2KHR");
	vkCmdCopyImage2KHR_volkImpl = (PFN_vkCmdCopyImage2KHR)load(context, "vkCmdCopyImage2KHR");
	vkCmdCopyImageToBuffer2KHR_volkImpl = (PFN_vkCmdCopyImageToBuffer2KHR)load(context, "vkCmdCopyImageToBuffer2KHR");
	vkCmdResolveImage2KHR_volkImpl = (PFN_vkCmdResolveImage2KHR)load(context, "vkCmdResolveImage2KHR");
#endif /* defined(VK_KHR_copy_commands2) */
#if defined(VK_KHR_create_renderpass2)
	vkCmdBeginRenderPass2KHR_volkImpl = (PFN_vkCmdBeginRenderPass2KHR)load(context, "vkCmdBeginRenderPass2KHR");
	vkCmdEndRenderPass2KHR_volkImpl = (PFN_vkCmdEndRenderPass2KHR)load(context, "vkCmdEndRenderPass2KHR");
	vkCmdNextSubpass2KHR_volkImpl = (PFN_vkCmdNextSubpass2KHR)load(context, "vkCmdNextSubpass2KHR");
	vkCreateRenderPass2KHR_volkImpl = (PFN_vkCreateRenderPass2KHR)load(context, "vkCreateRenderPass2KHR");
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_deferred_host_operations)
	vkCreateDeferredOperationKHR_volkImpl = (PFN_vkCreateDeferredOperationKHR)load(context, "vkCreateDeferredOperationKHR");
	vkDeferredOperationJoinKHR_volkImpl = (PFN_vkDeferredOperationJoinKHR)load(context, "vkDeferredOperationJoinKHR");
	vkDestroyDeferredOperationKHR_volkImpl = (PFN_vkDestroyDeferredOperationKHR)load(context, "vkDestroyDeferredOperationKHR");
	vkGetDeferredOperationMaxConcurrencyKHR_volkImpl = (PFN_vkGetDeferredOperationMaxConcurrencyKHR)load(context, "vkGetDeferredOperationMaxConcurrencyKHR");
	vkGetDeferredOperationResultKHR_volkImpl = (PFN_vkGetDeferredOperationResultKHR)load(context, "vkGetDeferredOperationResultKHR");
#endif /* defined(VK_KHR_deferred_host_operations) */
#if defined(VK_KHR_descriptor_update_template)
	vkCreateDescriptorUpdateTemplateKHR_volkImpl = (PFN_vkCreateDescriptorUpdateTemplateKHR)load(context, "vkCreateDescriptorUpdateTemplateKHR");
	vkDestroyDescriptorUpdateTemplateKHR_volkImpl = (PFN_vkDestroyDescriptorUpdateTemplateKHR)load(context, "vkDestroyDescriptorUpdateTemplateKHR");
	vkUpdateDescriptorSetWithTemplateKHR_volkImpl = (PFN_vkUpdateDescriptorSetWithTemplateKHR)load(context, "vkUpdateDescriptorSetWithTemplateKHR");
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
	vkCmdDispatchBaseKHR_volkImpl = (PFN_vkCmdDispatchBaseKHR)load(context, "vkCmdDispatchBaseKHR");
	vkCmdSetDeviceMaskKHR_volkImpl = (PFN_vkCmdSetDeviceMaskKHR)load(context, "vkCmdSetDeviceMaskKHR");
	vkGetDeviceGroupPeerMemoryFeaturesKHR_volkImpl = (PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR)load(context, "vkGetDeviceGroupPeerMemoryFeaturesKHR");
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_display_swapchain)
	vkCreateSharedSwapchainsKHR_volkImpl = (PFN_vkCreateSharedSwapchainsKHR)load(context, "vkCreateSharedSwapchainsKHR");
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
	vkCmdDrawIndexedIndirectCountKHR_volkImpl = (PFN_vkCmdDrawIndexedIndirectCountKHR)load(context, "vkCmdDrawIndexedIndirectCountKHR");
	vkCmdDrawIndirectCountKHR_volkImpl = (PFN_vkCmdDrawIndirectCountKHR)load(context, "vkCmdDrawIndirectCountKHR");
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_dynamic_rendering)
	vkCmdBeginRenderingKHR_volkImpl = (PFN_vkCmdBeginRenderingKHR)load(context, "vkCmdBeginRenderingKHR");
	vkCmdEndRenderingKHR_volkImpl = (PFN_vkCmdEndRenderingKHR)load(context, "vkCmdEndRenderingKHR");
#endif /* defined(VK_KHR_dynamic_rendering) */
#if defined(VK_KHR_dynamic_rendering_local_read)
	vkCmdSetRenderingAttachmentLocationsKHR_volkImpl = (PFN_vkCmdSetRenderingAttachmentLocationsKHR)load(context, "vkCmdSetRenderingAttachmentLocationsKHR");
	vkCmdSetRenderingInputAttachmentIndicesKHR_volkImpl = (PFN_vkCmdSetRenderingInputAttachmentIndicesKHR)load(context, "vkCmdSetRenderingInputAttachmentIndicesKHR");
#endif /* defined(VK_KHR_dynamic_rendering_local_read) */
#if defined(VK_KHR_external_fence_fd)
	vkGetFenceFdKHR_volkImpl = (PFN_vkGetFenceFdKHR)load(context, "vkGetFenceFdKHR");
	vkImportFenceFdKHR_volkImpl = (PFN_vkImportFenceFdKHR)load(context, "vkImportFenceFdKHR");
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
	vkGetFenceWin32HandleKHR_volkImpl = (PFN_vkGetFenceWin32HandleKHR)load(context, "vkGetFenceWin32HandleKHR");
	vkImportFenceWin32HandleKHR_volkImpl = (PFN_vkImportFenceWin32HandleKHR)load(context, "vkImportFenceWin32HandleKHR");
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_fd)
	vkGetMemoryFdKHR_volkImpl = (PFN_vkGetMemoryFdKHR)load(context, "vkGetMemoryFdKHR");
	vkGetMemoryFdPropertiesKHR_volkImpl = (PFN_vkGetMemoryFdPropertiesKHR)load(context, "vkGetMemoryFdPropertiesKHR");
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
	vkGetMemoryWin32HandleKHR_volkImpl = (PFN_vkGetMemoryWin32HandleKHR)load(context, "vkGetMemoryWin32HandleKHR");
	vkGetMemoryWin32HandlePropertiesKHR_volkImpl = (PFN_vkGetMemoryWin32HandlePropertiesKHR)load(context, "vkGetMemoryWin32HandlePropertiesKHR");
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_fd)
	vkGetSemaphoreFdKHR_volkImpl = (PFN_vkGetSemaphoreFdKHR)load(context, "vkGetSemaphoreFdKHR");
	vkImportSemaphoreFdKHR_volkImpl = (PFN_vkImportSemaphoreFdKHR)load(context, "vkImportSemaphoreFdKHR");
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
	vkGetSemaphoreWin32HandleKHR_volkImpl = (PFN_vkGetSemaphoreWin32HandleKHR)load(context, "vkGetSemaphoreWin32HandleKHR");
	vkImportSemaphoreWin32HandleKHR_volkImpl = (PFN_vkImportSemaphoreWin32HandleKHR)load(context, "vkImportSemaphoreWin32HandleKHR");
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_fragment_shading_rate)
	vkCmdSetFragmentShadingRateKHR_volkImpl = (PFN_vkCmdSetFragmentShadingRateKHR)load(context, "vkCmdSetFragmentShadingRateKHR");
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_memory_requirements2)
	vkGetBufferMemoryRequirements2KHR_volkImpl = (PFN_vkGetBufferMemoryRequirements2KHR)load(context, "vkGetBufferMemoryRequirements2KHR");
	vkGetImageMemoryRequirements2KHR_volkImpl = (PFN_vkGetImageMemoryRequirements2KHR)load(context, "vkGetImageMemoryRequirements2KHR");
	vkGetImageSparseMemoryRequirements2KHR_volkImpl = (PFN_vkGetImageSparseMemoryRequirements2KHR)load(context, "vkGetImageSparseMemoryRequirements2KHR");
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_line_rasterization)
	vkCmdSetLineStippleKHR_volkImpl = (PFN_vkCmdSetLineStippleKHR)load(context, "vkCmdSetLineStippleKHR");
#endif /* defined(VK_KHR_line_rasterization) */
#if defined(VK_KHR_maintenance1)
	vkTrimCommandPoolKHR_volkImpl = (PFN_vkTrimCommandPoolKHR)load(context, "vkTrimCommandPoolKHR");
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance3)
	vkGetDescriptorSetLayoutSupportKHR_volkImpl = (PFN_vkGetDescriptorSetLayoutSupportKHR)load(context, "vkGetDescriptorSetLayoutSupportKHR");
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_maintenance4)
	vkGetDeviceBufferMemoryRequirementsKHR_volkImpl = (PFN_vkGetDeviceBufferMemoryRequirementsKHR)load(context, "vkGetDeviceBufferMemoryRequirementsKHR");
	vkGetDeviceImageMemoryRequirementsKHR_volkImpl = (PFN_vkGetDeviceImageMemoryRequirementsKHR)load(context, "vkGetDeviceImageMemoryRequirementsKHR");
	vkGetDeviceImageSparseMemoryRequirementsKHR_volkImpl = (PFN_vkGetDeviceImageSparseMemoryRequirementsKHR)load(context, "vkGetDeviceImageSparseMemoryRequirementsKHR");
#endif /* defined(VK_KHR_maintenance4) */
#if defined(VK_KHR_maintenance5)
	vkCmdBindIndexBuffer2KHR_volkImpl = (PFN_vkCmdBindIndexBuffer2KHR)load(context, "vkCmdBindIndexBuffer2KHR");
	vkGetDeviceImageSubresourceLayoutKHR_volkImpl = (PFN_vkGetDeviceImageSubresourceLayoutKHR)load(context, "vkGetDeviceImageSubresourceLayoutKHR");
	vkGetImageSubresourceLayout2KHR_volkImpl = (PFN_vkGetImageSubresourceLayout2KHR)load(context, "vkGetImageSubresourceLayout2KHR");
	vkGetRenderingAreaGranularityKHR_volkImpl = (PFN_vkGetRenderingAreaGranularityKHR)load(context, "vkGetRenderingAreaGranularityKHR");
#endif /* defined(VK_KHR_maintenance5) */
#if defined(VK_KHR_maintenance6)
	vkCmdBindDescriptorSets2KHR_volkImpl = (PFN_vkCmdBindDescriptorSets2KHR)load(context, "vkCmdBindDescriptorSets2KHR");
	vkCmdPushConstants2KHR_volkImpl = (PFN_vkCmdPushConstants2KHR)load(context, "vkCmdPushConstants2KHR");
#endif /* defined(VK_KHR_maintenance6) */
#if defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor)
	vkCmdPushDescriptorSet2KHR_volkImpl = (PFN_vkCmdPushDescriptorSet2KHR)load(context, "vkCmdPushDescriptorSet2KHR");
	vkCmdPushDescriptorSetWithTemplate2KHR_volkImpl = (PFN_vkCmdPushDescriptorSetWithTemplate2KHR)load(context, "vkCmdPushDescriptorSetWithTemplate2KHR");
#endif /* defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer)
	vkCmdBindDescriptorBufferEmbeddedSamplers2EXT_volkImpl = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT)load(context, "vkCmdBindDescriptorBufferEmbeddedSamplers2EXT");
	vkCmdSetDescriptorBufferOffsets2EXT_volkImpl = (PFN_vkCmdSetDescriptorBufferOffsets2EXT)load(context, "vkCmdSetDescriptorBufferOffsets2EXT");
#endif /* defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer) */
#if defined(VK_KHR_map_memory2)
	vkMapMemory2KHR_volkImpl = (PFN_vkMapMemory2KHR)load(context, "vkMapMemory2KHR");
	vkUnmapMemory2KHR_volkImpl = (PFN_vkUnmapMemory2KHR)load(context, "vkUnmapMemory2KHR");
#endif /* defined(VK_KHR_map_memory2) */
#if defined(VK_KHR_performance_query)
	vkAcquireProfilingLockKHR_volkImpl = (PFN_vkAcquireProfilingLockKHR)load(context, "vkAcquireProfilingLockKHR");
	vkReleaseProfilingLockKHR_volkImpl = (PFN_vkReleaseProfilingLockKHR)load(context, "vkReleaseProfilingLockKHR");
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_pipeline_executable_properties)
	vkGetPipelineExecutableInternalRepresentationsKHR_volkImpl = (PFN_vkGetPipelineExecutableInternalRepresentationsKHR)load(context, "vkGetPipelineExecutableInternalRepresentationsKHR");
	vkGetPipelineExecutablePropertiesKHR_volkImpl = (PFN_vkGetPipelineExecutablePropertiesKHR)load(context, "vkGetPipelineExecutablePropertiesKHR");
	vkGetPipelineExecutableStatisticsKHR_volkImpl = (PFN_vkGetPipelineExecutableStatisticsKHR)load(context, "vkGetPipelineExecutableStatisticsKHR");
#endif /* defined(VK_KHR_pipeline_executable_properties) */
#if defined(VK_KHR_present_wait)
	vkWaitForPresentKHR_volkImpl = (PFN_vkWaitForPresentKHR)load(context, "vkWaitForPresentKHR");
#endif /* defined(VK_KHR_present_wait) */
#if defined(VK_KHR_push_descriptor)
	vkCmdPushDescriptorSetKHR_volkImpl = (PFN_vkCmdPushDescriptorSetKHR)load(context, "vkCmdPushDescriptorSetKHR");
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline)
	vkCmdTraceRaysIndirect2KHR_volkImpl = (PFN_vkCmdTraceRaysIndirect2KHR)load(context, "vkCmdTraceRaysIndirect2KHR");
#endif /* defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_ray_tracing_pipeline)
	vkCmdSetRayTracingPipelineStackSizeKHR_volkImpl = (PFN_vkCmdSetRayTracingPipelineStackSizeKHR)load(context, "vkCmdSetRayTracingPipelineStackSizeKHR");
	vkCmdTraceRaysIndirectKHR_volkImpl = (PFN_vkCmdTraceRaysIndirectKHR)load(context, "vkCmdTraceRaysIndirectKHR");
	vkCmdTraceRaysKHR_volkImpl = (PFN_vkCmdTraceRaysKHR)load(context, "vkCmdTraceRaysKHR");
	vkCreateRayTracingPipelinesKHR_volkImpl = (PFN_vkCreateRayTracingPipelinesKHR)load(context, "vkCreateRayTracingPipelinesKHR");
	vkGetRayTracingCaptureReplayShaderGroupHandlesKHR_volkImpl = (PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR)load(context, "vkGetRayTracingCaptureReplayShaderGroupHandlesKHR");
	vkGetRayTracingShaderGroupHandlesKHR_volkImpl = (PFN_vkGetRayTracingShaderGroupHandlesKHR)load(context, "vkGetRayTracingShaderGroupHandlesKHR");
	vkGetRayTracingShaderGroupStackSizeKHR_volkImpl = (PFN_vkGetRayTracingShaderGroupStackSizeKHR)load(context, "vkGetRayTracingShaderGroupStackSizeKHR");
#endif /* defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
	vkCreateSamplerYcbcrConversionKHR_volkImpl = (PFN_vkCreateSamplerYcbcrConversionKHR)load(context, "vkCreateSamplerYcbcrConversionKHR");
	vkDestroySamplerYcbcrConversionKHR_volkImpl = (PFN_vkDestroySamplerYcbcrConversionKHR)load(context, "vkDestroySamplerYcbcrConversionKHR");
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
	vkGetSwapchainStatusKHR_volkImpl = (PFN_vkGetSwapchainStatusKHR)load(context, "vkGetSwapchainStatusKHR");
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_swapchain)
	vkAcquireNextImageKHR_volkImpl = (PFN_vkAcquireNextImageKHR)load(context, "vkAcquireNextImageKHR");
	vkCreateSwapchainKHR_volkImpl = (PFN_vkCreateSwapchainKHR)load(context, "vkCreateSwapchainKHR");
	vkDestroySwapchainKHR_volkImpl = (PFN_vkDestroySwapchainKHR)load(context, "vkDestroySwapchainKHR");
	vkGetSwapchainImagesKHR_volkImpl = (PFN_vkGetSwapchainImagesKHR)load(context, "vkGetSwapchainImagesKHR");
	vkQueuePresentKHR_volkImpl = (PFN_vkQueuePresentKHR)load(context, "vkQueuePresentKHR");
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_synchronization2)
	vkCmdPipelineBarrier2KHR_volkImpl = (PFN_vkCmdPipelineBarrier2KHR)load(context, "vkCmdPipelineBarrier2KHR");
	vkCmdResetEvent2KHR_volkImpl = (PFN_vkCmdResetEvent2KHR)load(context, "vkCmdResetEvent2KHR");
	vkCmdSetEvent2KHR_volkImpl = (PFN_vkCmdSetEvent2KHR)load(context, "vkCmdSetEvent2KHR");
	vkCmdWaitEvents2KHR_volkImpl = (PFN_vkCmdWaitEvents2KHR)load(context, "vkCmdWaitEvents2KHR");
	vkCmdWriteTimestamp2KHR_volkImpl = (PFN_vkCmdWriteTimestamp2KHR)load(context, "vkCmdWriteTimestamp2KHR");
	vkQueueSubmit2KHR_volkImpl = (PFN_vkQueueSubmit2KHR)load(context, "vkQueueSubmit2KHR");
#endif /* defined(VK_KHR_synchronization2) */
#if defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker)
	vkCmdWriteBufferMarker2AMD_volkImpl = (PFN_vkCmdWriteBufferMarker2AMD)load(context, "vkCmdWriteBufferMarker2AMD");
#endif /* defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker) */
#if defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints)
	vkGetQueueCheckpointData2NV_volkImpl = (PFN_vkGetQueueCheckpointData2NV)load(context, "vkGetQueueCheckpointData2NV");
#endif /* defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_KHR_timeline_semaphore)
	vkGetSemaphoreCounterValueKHR_volkImpl = (PFN_vkGetSemaphoreCounterValueKHR)load(context, "vkGetSemaphoreCounterValueKHR");
	vkSignalSemaphoreKHR_volkImpl = (PFN_vkSignalSemaphoreKHR)load(context, "vkSignalSemaphoreKHR");
	vkWaitSemaphoresKHR_volkImpl = (PFN_vkWaitSemaphoresKHR)load(context, "vkWaitSemaphoresKHR");
#endif /* defined(VK_KHR_timeline_semaphore) */
#if defined(VK_KHR_video_decode_queue)
	vkCmdDecodeVideoKHR_volkImpl = (PFN_vkCmdDecodeVideoKHR)load(context, "vkCmdDecodeVideoKHR");
#endif /* defined(VK_KHR_video_decode_queue) */
#if defined(VK_KHR_video_encode_queue)
	vkCmdEncodeVideoKHR_volkImpl = (PFN_vkCmdEncodeVideoKHR)load(context, "vkCmdEncodeVideoKHR");
	vkGetEncodedVideoSessionParametersKHR_volkImpl = (PFN_vkGetEncodedVideoSessionParametersKHR)load(context, "vkGetEncodedVideoSessionParametersKHR");
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
	vkBindVideoSessionMemoryKHR_volkImpl = (PFN_vkBindVideoSessionMemoryKHR)load(context, "vkBindVideoSessionMemoryKHR");
	vkCmdBeginVideoCodingKHR_volkImpl = (PFN_vkCmdBeginVideoCodingKHR)load(context, "vkCmdBeginVideoCodingKHR");
	vkCmdControlVideoCodingKHR_volkImpl = (PFN_vkCmdControlVideoCodingKHR)load(context, "vkCmdControlVideoCodingKHR");
	vkCmdEndVideoCodingKHR_volkImpl = (PFN_vkCmdEndVideoCodingKHR)load(context, "vkCmdEndVideoCodingKHR");
	vkCreateVideoSessionKHR_volkImpl = (PFN_vkCreateVideoSessionKHR)load(context, "vkCreateVideoSessionKHR");
	vkCreateVideoSessionParametersKHR_volkImpl = (PFN_vkCreateVideoSessionParametersKHR)load(context, "vkCreateVideoSessionParametersKHR");
	vkDestroyVideoSessionKHR_volkImpl = (PFN_vkDestroyVideoSessionKHR)load(context, "vkDestroyVideoSessionKHR");
	vkDestroyVideoSessionParametersKHR_volkImpl = (PFN_vkDestroyVideoSessionParametersKHR)load(context, "vkDestroyVideoSessionParametersKHR");
	vkGetVideoSessionMemoryRequirementsKHR_volkImpl = (PFN_vkGetVideoSessionMemoryRequirementsKHR)load(context, "vkGetVideoSessionMemoryRequirementsKHR");
	vkUpdateVideoSessionParametersKHR_volkImpl = (PFN_vkUpdateVideoSessionParametersKHR)load(context, "vkUpdateVideoSessionParametersKHR");
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_NVX_binary_import)
	vkCmdCuLaunchKernelNVX_volkImpl = (PFN_vkCmdCuLaunchKernelNVX)load(context, "vkCmdCuLaunchKernelNVX");
	vkCreateCuFunctionNVX_volkImpl = (PFN_vkCreateCuFunctionNVX)load(context, "vkCreateCuFunctionNVX");
	vkCreateCuModuleNVX_volkImpl = (PFN_vkCreateCuModuleNVX)load(context, "vkCreateCuModuleNVX");
	vkDestroyCuFunctionNVX_volkImpl = (PFN_vkDestroyCuFunctionNVX)load(context, "vkDestroyCuFunctionNVX");
	vkDestroyCuModuleNVX_volkImpl = (PFN_vkDestroyCuModuleNVX)load(context, "vkDestroyCuModuleNVX");
#endif /* defined(VK_NVX_binary_import) */
#if defined(VK_NVX_image_view_handle)
	vkGetImageViewAddressNVX_volkImpl = (PFN_vkGetImageViewAddressNVX)load(context, "vkGetImageViewAddressNVX");
	vkGetImageViewHandleNVX_volkImpl = (PFN_vkGetImageViewHandleNVX)load(context, "vkGetImageViewHandleNVX");
#endif /* defined(VK_NVX_image_view_handle) */
#if defined(VK_NV_clip_space_w_scaling)
	vkCmdSetViewportWScalingNV_volkImpl = (PFN_vkCmdSetViewportWScalingNV)load(context, "vkCmdSetViewportWScalingNV");
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_copy_memory_indirect)
	vkCmdCopyMemoryIndirectNV_volkImpl = (PFN_vkCmdCopyMemoryIndirectNV)load(context, "vkCmdCopyMemoryIndirectNV");
	vkCmdCopyMemoryToImageIndirectNV_volkImpl = (PFN_vkCmdCopyMemoryToImageIndirectNV)load(context, "vkCmdCopyMemoryToImageIndirectNV");
#endif /* defined(VK_NV_copy_memory_indirect) */
#if defined(VK_NV_cuda_kernel_launch)
	vkCmdCudaLaunchKernelNV_volkImpl = (PFN_vkCmdCudaLaunchKernelNV)load(context, "vkCmdCudaLaunchKernelNV");
	vkCreateCudaFunctionNV_volkImpl = (PFN_vkCreateCudaFunctionNV)load(context, "vkCreateCudaFunctionNV");
	vkCreateCudaModuleNV_volkImpl = (PFN_vkCreateCudaModuleNV)load(context, "vkCreateCudaModuleNV");
	vkDestroyCudaFunctionNV_volkImpl = (PFN_vkDestroyCudaFunctionNV)load(context, "vkDestroyCudaFunctionNV");
	vkDestroyCudaModuleNV_volkImpl = (PFN_vkDestroyCudaModuleNV)load(context, "vkDestroyCudaModuleNV");
	vkGetCudaModuleCacheNV_volkImpl = (PFN_vkGetCudaModuleCacheNV)load(context, "vkGetCudaModuleCacheNV");
#endif /* defined(VK_NV_cuda_kernel_launch) */
#if defined(VK_NV_device_diagnostic_checkpoints)
	vkCmdSetCheckpointNV_volkImpl = (PFN_vkCmdSetCheckpointNV)load(context, "vkCmdSetCheckpointNV");
	vkGetQueueCheckpointDataNV_volkImpl = (PFN_vkGetQueueCheckpointDataNV)load(context, "vkGetQueueCheckpointDataNV");
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_device_generated_commands)
	vkCmdBindPipelineShaderGroupNV_volkImpl = (PFN_vkCmdBindPipelineShaderGroupNV)load(context, "vkCmdBindPipelineShaderGroupNV");
	vkCmdExecuteGeneratedCommandsNV_volkImpl = (PFN_vkCmdExecuteGeneratedCommandsNV)load(context, "vkCmdExecuteGeneratedCommandsNV");
	vkCmdPreprocessGeneratedCommandsNV_volkImpl = (PFN_vkCmdPreprocessGeneratedCommandsNV)load(context, "vkCmdPreprocessGeneratedCommandsNV");
	vkCreateIndirectCommandsLayoutNV_volkImpl = (PFN_vkCreateIndirectCommandsLayoutNV)load(context, "vkCreateIndirectCommandsLayoutNV");
	vkDestroyIndirectCommandsLayoutNV_volkImpl = (PFN_vkDestroyIndirectCommandsLayoutNV)load(context, "vkDestroyIndirectCommandsLayoutNV");
	vkGetGeneratedCommandsMemoryRequirementsNV_volkImpl = (PFN_vkGetGeneratedCommandsMemoryRequirementsNV)load(context, "vkGetGeneratedCommandsMemoryRequirementsNV");
#endif /* defined(VK_NV_device_generated_commands) */
#if defined(VK_NV_device_generated_commands_compute)
	vkCmdUpdatePipelineIndirectBufferNV_volkImpl = (PFN_vkCmdUpdatePipelineIndirectBufferNV)load(context, "vkCmdUpdatePipelineIndirectBufferNV");
	vkGetPipelineIndirectDeviceAddressNV_volkImpl = (PFN_vkGetPipelineIndirectDeviceAddressNV)load(context, "vkGetPipelineIndirectDeviceAddressNV");
	vkGetPipelineIndirectMemoryRequirementsNV_volkImpl = (PFN_vkGetPipelineIndirectMemoryRequirementsNV)load(context, "vkGetPipelineIndirectMemoryRequirementsNV");
#endif /* defined(VK_NV_device_generated_commands_compute) */
#if defined(VK_NV_external_memory_rdma)
	vkGetMemoryRemoteAddressNV_volkImpl = (PFN_vkGetMemoryRemoteAddressNV)load(context, "vkGetMemoryRemoteAddressNV");
#endif /* defined(VK_NV_external_memory_rdma) */
#if defined(VK_NV_external_memory_win32)
	vkGetMemoryWin32HandleNV_volkImpl = (PFN_vkGetMemoryWin32HandleNV)load(context, "vkGetMemoryWin32HandleNV");
#endif /* defined(VK_NV_external_memory_win32) */
#if defined(VK_NV_fragment_shading_rate_enums)
	vkCmdSetFragmentShadingRateEnumNV_volkImpl = (PFN_vkCmdSetFragmentShadingRateEnumNV)load(context, "vkCmdSetFragmentShadingRateEnumNV");
#endif /* defined(VK_NV_fragment_shading_rate_enums) */
#if defined(VK_NV_low_latency2)
	vkGetLatencyTimingsNV_volkImpl = (PFN_vkGetLatencyTimingsNV)load(context, "vkGetLatencyTimingsNV");
	vkLatencySleepNV_volkImpl = (PFN_vkLatencySleepNV)load(context, "vkLatencySleepNV");
	vkQueueNotifyOutOfBandNV_volkImpl = (PFN_vkQueueNotifyOutOfBandNV)load(context, "vkQueueNotifyOutOfBandNV");
	vkSetLatencyMarkerNV_volkImpl = (PFN_vkSetLatencyMarkerNV)load(context, "vkSetLatencyMarkerNV");
	vkSetLatencySleepModeNV_volkImpl = (PFN_vkSetLatencySleepModeNV)load(context, "vkSetLatencySleepModeNV");
#endif /* defined(VK_NV_low_latency2) */
#if defined(VK_NV_memory_decompression)
	vkCmdDecompressMemoryIndirectCountNV_volkImpl = (PFN_vkCmdDecompressMemoryIndirectCountNV)load(context, "vkCmdDecompressMemoryIndirectCountNV");
	vkCmdDecompressMemoryNV_volkImpl = (PFN_vkCmdDecompressMemoryNV)load(context, "vkCmdDecompressMemoryNV");
#endif /* defined(VK_NV_memory_decompression) */
#if defined(VK_NV_mesh_shader)
	vkCmdDrawMeshTasksIndirectCountNV_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectCountNV)load(context, "vkCmdDrawMeshTasksIndirectCountNV");
	vkCmdDrawMeshTasksIndirectNV_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectNV)load(context, "vkCmdDrawMeshTasksIndirectNV");
	vkCmdDrawMeshTasksNV_volkImpl = (PFN_vkCmdDrawMeshTasksNV)load(context, "vkCmdDrawMeshTasksNV");
#endif /* defined(VK_NV_mesh_shader) */
#if defined(VK_NV_optical_flow)
	vkBindOpticalFlowSessionImageNV_volkImpl = (PFN_vkBindOpticalFlowSessionImageNV)load(context, "vkBindOpticalFlowSessionImageNV");
	vkCmdOpticalFlowExecuteNV_volkImpl = (PFN_vkCmdOpticalFlowExecuteNV)load(context, "vkCmdOpticalFlowExecuteNV");
	vkCreateOpticalFlowSessionNV_volkImpl = (PFN_vkCreateOpticalFlowSessionNV)load(context, "vkCreateOpticalFlowSessionNV");
	vkDestroyOpticalFlowSessionNV_volkImpl = (PFN_vkDestroyOpticalFlowSessionNV)load(context, "vkDestroyOpticalFlowSessionNV");
#endif /* defined(VK_NV_optical_flow) */
#if defined(VK_NV_ray_tracing)
	vkBindAccelerationStructureMemoryNV_volkImpl = (PFN_vkBindAccelerationStructureMemoryNV)load(context, "vkBindAccelerationStructureMemoryNV");
	vkCmdBuildAccelerationStructureNV_volkImpl = (PFN_vkCmdBuildAccelerationStructureNV)load(context, "vkCmdBuildAccelerationStructureNV");
	vkCmdCopyAccelerationStructureNV_volkImpl = (PFN_vkCmdCopyAccelerationStructureNV)load(context, "vkCmdCopyAccelerationStructureNV");
	vkCmdTraceRaysNV_volkImpl = (PFN_vkCmdTraceRaysNV)load(context, "vkCmdTraceRaysNV");
	vkCmdWriteAccelerationStructuresPropertiesNV_volkImpl = (PFN_vkCmdWriteAccelerationStructuresPropertiesNV)load(context, "vkCmdWriteAccelerationStructuresPropertiesNV");
	vkCompileDeferredNV_volkImpl = (PFN_vkCompileDeferredNV)load(context, "vkCompileDeferredNV");
	vkCreateAccelerationStructureNV_volkImpl = (PFN_vkCreateAccelerationStructureNV)load(context, "vkCreateAccelerationStructureNV");
	vkCreateRayTracingPipelinesNV_volkImpl = (PFN_vkCreateRayTracingPipelinesNV)load(context, "vkCreateRayTracingPipelinesNV");
	vkDestroyAccelerationStructureNV_volkImpl = (PFN_vkDestroyAccelerationStructureNV)load(context, "vkDestroyAccelerationStructureNV");
	vkGetAccelerationStructureHandleNV_volkImpl = (PFN_vkGetAccelerationStructureHandleNV)load(context, "vkGetAccelerationStructureHandleNV");
	vkGetAccelerationStructureMemoryRequirementsNV_volkImpl = (PFN_vkGetAccelerationStructureMemoryRequirementsNV)load(context, "vkGetAccelerationStructureMemoryRequirementsNV");
	vkGetRayTracingShaderGroupHandlesNV_volkImpl = (PFN_vkGetRayTracingShaderGroupHandlesNV)load(context, "vkGetRayTracingShaderGroupHandlesNV");
#endif /* defined(VK_NV_ray_tracing) */
#if defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2
	vkCmdSetExclusiveScissorEnableNV_volkImpl = (PFN_vkCmdSetExclusiveScissorEnableNV)load(context, "vkCmdSetExclusiveScissorEnableNV");
#endif /* defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2 */
#if defined(VK_NV_scissor_exclusive)
	vkCmdSetExclusiveScissorNV_volkImpl = (PFN_vkCmdSetExclusiveScissorNV)load(context, "vkCmdSetExclusiveScissorNV");
#endif /* defined(VK_NV_scissor_exclusive) */
#if defined(VK_NV_shading_rate_image)
	vkCmdBindShadingRateImageNV_volkImpl = (PFN_vkCmdBindShadingRateImageNV)load(context, "vkCmdBindShadingRateImageNV");
	vkCmdSetCoarseSampleOrderNV_volkImpl = (PFN_vkCmdSetCoarseSampleOrderNV)load(context, "vkCmdSetCoarseSampleOrderNV");
	vkCmdSetViewportShadingRatePaletteNV_volkImpl = (PFN_vkCmdSetViewportShadingRatePaletteNV)load(context, "vkCmdSetViewportShadingRatePaletteNV");
#endif /* defined(VK_NV_shading_rate_image) */
#if defined(VK_QCOM_tile_properties)
	vkGetDynamicRenderingTilePropertiesQCOM_volkImpl = (PFN_vkGetDynamicRenderingTilePropertiesQCOM)load(context, "vkGetDynamicRenderingTilePropertiesQCOM");
	vkGetFramebufferTilePropertiesQCOM_volkImpl = (PFN_vkGetFramebufferTilePropertiesQCOM)load(context, "vkGetFramebufferTilePropertiesQCOM");
#endif /* defined(VK_QCOM_tile_properties) */
#if defined(VK_QNX_external_memory_screen_buffer)
	vkGetScreenBufferPropertiesQNX_volkImpl = (PFN_vkGetScreenBufferPropertiesQNX)load(context, "vkGetScreenBufferPropertiesQNX");
#endif /* defined(VK_QNX_external_memory_screen_buffer) */
#if defined(VK_VALVE_descriptor_set_host_mapping)
	vkGetDescriptorSetHostMappingVALVE_volkImpl = (PFN_vkGetDescriptorSetHostMappingVALVE)load(context, "vkGetDescriptorSetHostMappingVALVE");
	vkGetDescriptorSetLayoutHostMappingInfoVALVE_volkImpl = (PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE)load(context, "vkGetDescriptorSetLayoutHostMappingInfoVALVE");
#endif /* defined(VK_VALVE_descriptor_set_host_mapping) */
#if (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object))
	vkCmdBindVertexBuffers2EXT_volkImpl = (PFN_vkCmdBindVertexBuffers2EXT)load(context, "vkCmdBindVertexBuffers2EXT");
	vkCmdSetCullModeEXT_volkImpl = (PFN_vkCmdSetCullModeEXT)load(context, "vkCmdSetCullModeEXT");
	vkCmdSetDepthBoundsTestEnableEXT_volkImpl = (PFN_vkCmdSetDepthBoundsTestEnableEXT)load(context, "vkCmdSetDepthBoundsTestEnableEXT");
	vkCmdSetDepthCompareOpEXT_volkImpl = (PFN_vkCmdSetDepthCompareOpEXT)load(context, "vkCmdSetDepthCompareOpEXT");
	vkCmdSetDepthTestEnableEXT_volkImpl = (PFN_vkCmdSetDepthTestEnableEXT)load(context, "vkCmdSetDepthTestEnableEXT");
	vkCmdSetDepthWriteEnableEXT_volkImpl = (PFN_vkCmdSetDepthWriteEnableEXT)load(context, "vkCmdSetDepthWriteEnableEXT");
	vkCmdSetFrontFaceEXT_volkImpl = (PFN_vkCmdSetFrontFaceEXT)load(context, "vkCmdSetFrontFaceEXT");
	vkCmdSetPrimitiveTopologyEXT_volkImpl = (PFN_vkCmdSetPrimitiveTopologyEXT)load(context, "vkCmdSetPrimitiveTopologyEXT");
	vkCmdSetScissorWithCountEXT_volkImpl = (PFN_vkCmdSetScissorWithCountEXT)load(context, "vkCmdSetScissorWithCountEXT");
	vkCmdSetStencilOpEXT_volkImpl = (PFN_vkCmdSetStencilOpEXT)load(context, "vkCmdSetStencilOpEXT");
	vkCmdSetStencilTestEnableEXT_volkImpl = (PFN_vkCmdSetStencilTestEnableEXT)load(context, "vkCmdSetStencilTestEnableEXT");
	vkCmdSetViewportWithCountEXT_volkImpl = (PFN_vkCmdSetViewportWithCountEXT)load(context, "vkCmdSetViewportWithCountEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object))
	vkCmdSetDepthBiasEnableEXT_volkImpl = (PFN_vkCmdSetDepthBiasEnableEXT)load(context, "vkCmdSetDepthBiasEnableEXT");
	vkCmdSetLogicOpEXT_volkImpl = (PFN_vkCmdSetLogicOpEXT)load(context, "vkCmdSetLogicOpEXT");
	vkCmdSetPatchControlPointsEXT_volkImpl = (PFN_vkCmdSetPatchControlPointsEXT)load(context, "vkCmdSetPatchControlPointsEXT");
	vkCmdSetPrimitiveRestartEnableEXT_volkImpl = (PFN_vkCmdSetPrimitiveRestartEnableEXT)load(context, "vkCmdSetPrimitiveRestartEnableEXT");
	vkCmdSetRasterizerDiscardEnableEXT_volkImpl = (PFN_vkCmdSetRasterizerDiscardEnableEXT)load(context, "vkCmdSetRasterizerDiscardEnableEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object))
	vkCmdSetAlphaToCoverageEnableEXT_volkImpl = (PFN_vkCmdSetAlphaToCoverageEnableEXT)load(context, "vkCmdSetAlphaToCoverageEnableEXT");
	vkCmdSetAlphaToOneEnableEXT_volkImpl = (PFN_vkCmdSetAlphaToOneEnableEXT)load(context, "vkCmdSetAlphaToOneEnableEXT");
	vkCmdSetColorBlendEnableEXT_volkImpl = (PFN_vkCmdSetColorBlendEnableEXT)load(context, "vkCmdSetColorBlendEnableEXT");
	vkCmdSetColorBlendEquationEXT_volkImpl = (PFN_vkCmdSetColorBlendEquationEXT)load(context, "vkCmdSetColorBlendEquationEXT");
	vkCmdSetColorWriteMaskEXT_volkImpl = (PFN_vkCmdSetColorWriteMaskEXT)load(context, "vkCmdSetColorWriteMaskEXT");
	vkCmdSetDepthClampEnableEXT_volkImpl = (PFN_vkCmdSetDepthClampEnableEXT)load(context, "vkCmdSetDepthClampEnableEXT");
	vkCmdSetLogicOpEnableEXT_volkImpl = (PFN_vkCmdSetLogicOpEnableEXT)load(context, "vkCmdSetLogicOpEnableEXT");
	vkCmdSetPolygonModeEXT_volkImpl = (PFN_vkCmdSetPolygonModeEXT)load(context, "vkCmdSetPolygonModeEXT");
	vkCmdSetRasterizationSamplesEXT_volkImpl = (PFN_vkCmdSetRasterizationSamplesEXT)load(context, "vkCmdSetRasterizationSamplesEXT");
	vkCmdSetSampleMaskEXT_volkImpl = (PFN_vkCmdSetSampleMaskEXT)load(context, "vkCmdSetSampleMaskEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object))
	vkCmdSetTessellationDomainOriginEXT_volkImpl = (PFN_vkCmdSetTessellationDomainOriginEXT)load(context, "vkCmdSetTessellationDomainOriginEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback))
	vkCmdSetRasterizationStreamEXT_volkImpl = (PFN_vkCmdSetRasterizationStreamEXT)load(context, "vkCmdSetRasterizationStreamEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization))
	vkCmdSetConservativeRasterizationModeEXT_volkImpl = (PFN_vkCmdSetConservativeRasterizationModeEXT)load(context, "vkCmdSetConservativeRasterizationModeEXT");
	vkCmdSetExtraPrimitiveOverestimationSizeEXT_volkImpl = (PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT)load(context, "vkCmdSetExtraPrimitiveOverestimationSizeEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable))
	vkCmdSetDepthClipEnableEXT_volkImpl = (PFN_vkCmdSetDepthClipEnableEXT)load(context, "vkCmdSetDepthClipEnableEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations))
	vkCmdSetSampleLocationsEnableEXT_volkImpl = (PFN_vkCmdSetSampleLocationsEnableEXT)load(context, "vkCmdSetSampleLocationsEnableEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced))
	vkCmdSetColorBlendAdvancedEXT_volkImpl = (PFN_vkCmdSetColorBlendAdvancedEXT)load(context, "vkCmdSetColorBlendAdvancedEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex))
	vkCmdSetProvokingVertexModeEXT_volkImpl = (PFN_vkCmdSetProvokingVertexModeEXT)load(context, "vkCmdSetProvokingVertexModeEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization))
	vkCmdSetLineRasterizationModeEXT_volkImpl = (PFN_vkCmdSetLineRasterizationModeEXT)load(context, "vkCmdSetLineRasterizationModeEXT");
	vkCmdSetLineStippleEnableEXT_volkImpl = (PFN_vkCmdSetLineStippleEnableEXT)load(context, "vkCmdSetLineStippleEnableEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control))
	vkCmdSetDepthClipNegativeOneToOneEXT_volkImpl = (PFN_vkCmdSetDepthClipNegativeOneToOneEXT)load(context, "vkCmdSetDepthClipNegativeOneToOneEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling))
	vkCmdSetViewportWScalingEnableNV_volkImpl = (PFN_vkCmdSetViewportWScalingEnableNV)load(context, "vkCmdSetViewportWScalingEnableNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle))
	vkCmdSetViewportSwizzleNV_volkImpl = (PFN_vkCmdSetViewportSwizzleNV)load(context, "vkCmdSetViewportSwizzleNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color))
	vkCmdSetCoverageToColorEnableNV_volkImpl = (PFN_vkCmdSetCoverageToColorEnableNV)load(context, "vkCmdSetCoverageToColorEnableNV");
	vkCmdSetCoverageToColorLocationNV_volkImpl = (PFN_vkCmdSetCoverageToColorLocationNV)load(context, "vkCmdSetCoverageToColorLocationNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples))
	vkCmdSetCoverageModulationModeNV_volkImpl = (PFN_vkCmdSetCoverageModulationModeNV)load(context, "vkCmdSetCoverageModulationModeNV");
	vkCmdSetCoverageModulationTableEnableNV_volkImpl = (PFN_vkCmdSetCoverageModulationTableEnableNV)load(context, "vkCmdSetCoverageModulationTableEnableNV");
	vkCmdSetCoverageModulationTableNV_volkImpl = (PFN_vkCmdSetCoverageModulationTableNV)load(context, "vkCmdSetCoverageModulationTableNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image))
	vkCmdSetShadingRateImageEnableNV_volkImpl = (PFN_vkCmdSetShadingRateImageEnableNV)load(context, "vkCmdSetShadingRateImageEnableNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test))
	vkCmdSetRepresentativeFragmentTestEnableNV_volkImpl = (PFN_vkCmdSetRepresentativeFragmentTestEnableNV)load(context, "vkCmdSetRepresentativeFragmentTestEnableNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode))
	vkCmdSetCoverageReductionModeNV_volkImpl = (PFN_vkCmdSetCoverageReductionModeNV)load(context, "vkCmdSetCoverageReductionModeNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode)) */
#if (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1))
	vkGetDeviceGroupSurfacePresentModes2EXT_volkImpl = (PFN_vkGetDeviceGroupSurfacePresentModes2EXT)load(context, "vkGetDeviceGroupSurfacePresentModes2EXT");
#endif /* (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1)) */
#if (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control))
	vkGetImageSubresourceLayout2EXT_volkImpl = (PFN_vkGetImageSubresourceLayout2EXT)load(context, "vkGetImageSubresourceLayout2EXT");
#endif /* (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control)) */
#if (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state))
	vkCmdSetVertexInputEXT_volkImpl = (PFN_vkCmdSetVertexInputEXT)load(context, "vkCmdSetVertexInputEXT");
#endif /* (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state)) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template))
	vkCmdPushDescriptorSetWithTemplateKHR_volkImpl = (PFN_vkCmdPushDescriptorSetWithTemplateKHR)load(context, "vkCmdPushDescriptorSetWithTemplateKHR");
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	vkGetDeviceGroupPresentCapabilitiesKHR_volkImpl = (PFN_vkGetDeviceGroupPresentCapabilitiesKHR)load(context, "vkGetDeviceGroupPresentCapabilitiesKHR");
	vkGetDeviceGroupSurfacePresentModesKHR_volkImpl = (PFN_vkGetDeviceGroupSurfacePresentModesKHR)load(context, "vkGetDeviceGroupSurfacePresentModesKHR");
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	vkAcquireNextImage2KHR_volkImpl = (PFN_vkAcquireNextImage2KHR)load(context, "vkAcquireNextImage2KHR");
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
	/* VOLK_GENERATE_LOAD_DEVICE */
}

static void volkGenLoadDeviceTable(struct VolkDeviceTable* table, void* context, PFN_vkVoidFunction (*load)(void*, const char*))
{
	/* VOLK_GENERATE_LOAD_DEVICE_TABLE */
#if defined(VK_VERSION_1_0)
	table->vkAllocateCommandBuffers_volkImpl = (PFN_vkAllocateCommandBuffers)load(context, "vkAllocateCommandBuffers");
	table->vkAllocateDescriptorSets_volkImpl = (PFN_vkAllocateDescriptorSets)load(context, "vkAllocateDescriptorSets");
	table->vkAllocateMemory_volkImpl = (PFN_vkAllocateMemory)load(context, "vkAllocateMemory");
	table->vkBeginCommandBuffer_volkImpl = (PFN_vkBeginCommandBuffer)load(context, "vkBeginCommandBuffer");
	table->vkBindBufferMemory_volkImpl = (PFN_vkBindBufferMemory)load(context, "vkBindBufferMemory");
	table->vkBindImageMemory_volkImpl = (PFN_vkBindImageMemory)load(context, "vkBindImageMemory");
	table->vkCmdBeginQuery_volkImpl = (PFN_vkCmdBeginQuery)load(context, "vkCmdBeginQuery");
	table->vkCmdBeginRenderPass_volkImpl = (PFN_vkCmdBeginRenderPass)load(context, "vkCmdBeginRenderPass");
	table->vkCmdBindDescriptorSets_volkImpl = (PFN_vkCmdBindDescriptorSets)load(context, "vkCmdBindDescriptorSets");
	table->vkCmdBindIndexBuffer_volkImpl = (PFN_vkCmdBindIndexBuffer)load(context, "vkCmdBindIndexBuffer");
	table->vkCmdBindPipeline_volkImpl = (PFN_vkCmdBindPipeline)load(context, "vkCmdBindPipeline");
	table->vkCmdBindVertexBuffers_volkImpl = (PFN_vkCmdBindVertexBuffers)load(context, "vkCmdBindVertexBuffers");
	table->vkCmdBlitImage_volkImpl = (PFN_vkCmdBlitImage)load(context, "vkCmdBlitImage");
	table->vkCmdClearAttachments_volkImpl = (PFN_vkCmdClearAttachments)load(context, "vkCmdClearAttachments");
	table->vkCmdClearColorImage_volkImpl = (PFN_vkCmdClearColorImage)load(context, "vkCmdClearColorImage");
	table->vkCmdClearDepthStencilImage_volkImpl = (PFN_vkCmdClearDepthStencilImage)load(context, "vkCmdClearDepthStencilImage");
	table->vkCmdCopyBuffer_volkImpl = (PFN_vkCmdCopyBuffer)load(context, "vkCmdCopyBuffer");
	table->vkCmdCopyBufferToImage_volkImpl = (PFN_vkCmdCopyBufferToImage)load(context, "vkCmdCopyBufferToImage");
	table->vkCmdCopyImage_volkImpl = (PFN_vkCmdCopyImage)load(context, "vkCmdCopyImage");
	table->vkCmdCopyImageToBuffer_volkImpl = (PFN_vkCmdCopyImageToBuffer)load(context, "vkCmdCopyImageToBuffer");
	table->vkCmdCopyQueryPoolResults_volkImpl = (PFN_vkCmdCopyQueryPoolResults)load(context, "vkCmdCopyQueryPoolResults");
	table->vkCmdDispatch_volkImpl = (PFN_vkCmdDispatch)load(context, "vkCmdDispatch");
	table->vkCmdDispatchIndirect_volkImpl = (PFN_vkCmdDispatchIndirect)load(context, "vkCmdDispatchIndirect");
	table->vkCmdDraw_volkImpl = (PFN_vkCmdDraw)load(context, "vkCmdDraw");
	table->vkCmdDrawIndexed_volkImpl = (PFN_vkCmdDrawIndexed)load(context, "vkCmdDrawIndexed");
	table->vkCmdDrawIndexedIndirect_volkImpl = (PFN_vkCmdDrawIndexedIndirect)load(context, "vkCmdDrawIndexedIndirect");
	table->vkCmdDrawIndirect_volkImpl = (PFN_vkCmdDrawIndirect)load(context, "vkCmdDrawIndirect");
	table->vkCmdEndQuery_volkImpl = (PFN_vkCmdEndQuery)load(context, "vkCmdEndQuery");
	table->vkCmdEndRenderPass_volkImpl = (PFN_vkCmdEndRenderPass)load(context, "vkCmdEndRenderPass");
	table->vkCmdExecuteCommands_volkImpl = (PFN_vkCmdExecuteCommands)load(context, "vkCmdExecuteCommands");
	table->vkCmdFillBuffer_volkImpl = (PFN_vkCmdFillBuffer)load(context, "vkCmdFillBuffer");
	table->vkCmdNextSubpass_volkImpl = (PFN_vkCmdNextSubpass)load(context, "vkCmdNextSubpass");
	table->vkCmdPipelineBarrier_volkImpl = (PFN_vkCmdPipelineBarrier)load(context, "vkCmdPipelineBarrier");
	table->vkCmdPushConstants_volkImpl = (PFN_vkCmdPushConstants)load(context, "vkCmdPushConstants");
	table->vkCmdResetEvent_volkImpl = (PFN_vkCmdResetEvent)load(context, "vkCmdResetEvent");
	table->vkCmdResetQueryPool_volkImpl = (PFN_vkCmdResetQueryPool)load(context, "vkCmdResetQueryPool");
	table->vkCmdResolveImage_volkImpl = (PFN_vkCmdResolveImage)load(context, "vkCmdResolveImage");
	table->vkCmdSetBlendConstants_volkImpl = (PFN_vkCmdSetBlendConstants)load(context, "vkCmdSetBlendConstants");
	table->vkCmdSetDepthBias_volkImpl = (PFN_vkCmdSetDepthBias)load(context, "vkCmdSetDepthBias");
	table->vkCmdSetDepthBounds_volkImpl = (PFN_vkCmdSetDepthBounds)load(context, "vkCmdSetDepthBounds");
	table->vkCmdSetEvent_volkImpl = (PFN_vkCmdSetEvent)load(context, "vkCmdSetEvent");
	table->vkCmdSetLineWidth_volkImpl = (PFN_vkCmdSetLineWidth)load(context, "vkCmdSetLineWidth");
	table->vkCmdSetScissor_volkImpl = (PFN_vkCmdSetScissor)load(context, "vkCmdSetScissor");
	table->vkCmdSetStencilCompareMask_volkImpl = (PFN_vkCmdSetStencilCompareMask)load(context, "vkCmdSetStencilCompareMask");
	table->vkCmdSetStencilReference_volkImpl = (PFN_vkCmdSetStencilReference)load(context, "vkCmdSetStencilReference");
	table->vkCmdSetStencilWriteMask_volkImpl = (PFN_vkCmdSetStencilWriteMask)load(context, "vkCmdSetStencilWriteMask");
	table->vkCmdSetViewport_volkImpl = (PFN_vkCmdSetViewport)load(context, "vkCmdSetViewport");
	table->vkCmdUpdateBuffer_volkImpl = (PFN_vkCmdUpdateBuffer)load(context, "vkCmdUpdateBuffer");
	table->vkCmdWaitEvents_volkImpl = (PFN_vkCmdWaitEvents)load(context, "vkCmdWaitEvents");
	table->vkCmdWriteTimestamp_volkImpl = (PFN_vkCmdWriteTimestamp)load(context, "vkCmdWriteTimestamp");
	table->vkCreateBuffer_volkImpl = (PFN_vkCreateBuffer)load(context, "vkCreateBuffer");
	table->vkCreateBufferView_volkImpl = (PFN_vkCreateBufferView)load(context, "vkCreateBufferView");
	table->vkCreateCommandPool_volkImpl = (PFN_vkCreateCommandPool)load(context, "vkCreateCommandPool");
	table->vkCreateComputePipelines_volkImpl = (PFN_vkCreateComputePipelines)load(context, "vkCreateComputePipelines");
	table->vkCreateDescriptorPool_volkImpl = (PFN_vkCreateDescriptorPool)load(context, "vkCreateDescriptorPool");
	table->vkCreateDescriptorSetLayout_volkImpl = (PFN_vkCreateDescriptorSetLayout)load(context, "vkCreateDescriptorSetLayout");
	table->vkCreateEvent_volkImpl = (PFN_vkCreateEvent)load(context, "vkCreateEvent");
	table->vkCreateFence_volkImpl = (PFN_vkCreateFence)load(context, "vkCreateFence");
	table->vkCreateFramebuffer_volkImpl = (PFN_vkCreateFramebuffer)load(context, "vkCreateFramebuffer");
	table->vkCreateGraphicsPipelines_volkImpl = (PFN_vkCreateGraphicsPipelines)load(context, "vkCreateGraphicsPipelines");
	table->vkCreateImage_volkImpl = (PFN_vkCreateImage)load(context, "vkCreateImage");
	table->vkCreateImageView_volkImpl = (PFN_vkCreateImageView)load(context, "vkCreateImageView");
	table->vkCreatePipelineCache_volkImpl = (PFN_vkCreatePipelineCache)load(context, "vkCreatePipelineCache");
	table->vkCreatePipelineLayout_volkImpl = (PFN_vkCreatePipelineLayout)load(context, "vkCreatePipelineLayout");
	table->vkCreateQueryPool_volkImpl = (PFN_vkCreateQueryPool)load(context, "vkCreateQueryPool");
	table->vkCreateRenderPass_volkImpl = (PFN_vkCreateRenderPass)load(context, "vkCreateRenderPass");
	table->vkCreateSampler_volkImpl = (PFN_vkCreateSampler)load(context, "vkCreateSampler");
	table->vkCreateSemaphore_volkImpl = (PFN_vkCreateSemaphore)load(context, "vkCreateSemaphore");
	table->vkCreateShaderModule_volkImpl = (PFN_vkCreateShaderModule)load(context, "vkCreateShaderModule");
	table->vkDestroyBuffer_volkImpl = (PFN_vkDestroyBuffer)load(context, "vkDestroyBuffer");
	table->vkDestroyBufferView_volkImpl = (PFN_vkDestroyBufferView)load(context, "vkDestroyBufferView");
	table->vkDestroyCommandPool_volkImpl = (PFN_vkDestroyCommandPool)load(context, "vkDestroyCommandPool");
	table->vkDestroyDescriptorPool_volkImpl = (PFN_vkDestroyDescriptorPool)load(context, "vkDestroyDescriptorPool");
	table->vkDestroyDescriptorSetLayout_volkImpl = (PFN_vkDestroyDescriptorSetLayout)load(context, "vkDestroyDescriptorSetLayout");
	table->vkDestroyDevice_volkImpl = (PFN_vkDestroyDevice)load(context, "vkDestroyDevice");
	table->vkDestroyEvent_volkImpl = (PFN_vkDestroyEvent)load(context, "vkDestroyEvent");
	table->vkDestroyFence_volkImpl = (PFN_vkDestroyFence)load(context, "vkDestroyFence");
	table->vkDestroyFramebuffer_volkImpl = (PFN_vkDestroyFramebuffer)load(context, "vkDestroyFramebuffer");
	table->vkDestroyImage_volkImpl = (PFN_vkDestroyImage)load(context, "vkDestroyImage");
	table->vkDestroyImageView_volkImpl = (PFN_vkDestroyImageView)load(context, "vkDestroyImageView");
	table->vkDestroyPipeline_volkImpl = (PFN_vkDestroyPipeline)load(context, "vkDestroyPipeline");
	table->vkDestroyPipelineCache_volkImpl = (PFN_vkDestroyPipelineCache)load(context, "vkDestroyPipelineCache");
	table->vkDestroyPipelineLayout_volkImpl = (PFN_vkDestroyPipelineLayout)load(context, "vkDestroyPipelineLayout");
	table->vkDestroyQueryPool_volkImpl = (PFN_vkDestroyQueryPool)load(context, "vkDestroyQueryPool");
	table->vkDestroyRenderPass_volkImpl = (PFN_vkDestroyRenderPass)load(context, "vkDestroyRenderPass");
	table->vkDestroySampler_volkImpl = (PFN_vkDestroySampler)load(context, "vkDestroySampler");
	table->vkDestroySemaphore_volkImpl = (PFN_vkDestroySemaphore)load(context, "vkDestroySemaphore");
	table->vkDestroyShaderModule_volkImpl = (PFN_vkDestroyShaderModule)load(context, "vkDestroyShaderModule");
	table->vkDeviceWaitIdle_volkImpl = (PFN_vkDeviceWaitIdle)load(context, "vkDeviceWaitIdle");
	table->vkEndCommandBuffer_volkImpl = (PFN_vkEndCommandBuffer)load(context, "vkEndCommandBuffer");
	table->vkFlushMappedMemoryRanges_volkImpl = (PFN_vkFlushMappedMemoryRanges)load(context, "vkFlushMappedMemoryRanges");
	table->vkFreeCommandBuffers_volkImpl = (PFN_vkFreeCommandBuffers)load(context, "vkFreeCommandBuffers");
	table->vkFreeDescriptorSets_volkImpl = (PFN_vkFreeDescriptorSets)load(context, "vkFreeDescriptorSets");
	table->vkFreeMemory_volkImpl = (PFN_vkFreeMemory)load(context, "vkFreeMemory");
	table->vkGetBufferMemoryRequirements_volkImpl = (PFN_vkGetBufferMemoryRequirements)load(context, "vkGetBufferMemoryRequirements");
	table->vkGetDeviceMemoryCommitment_volkImpl = (PFN_vkGetDeviceMemoryCommitment)load(context, "vkGetDeviceMemoryCommitment");
	table->vkGetDeviceQueue_volkImpl = (PFN_vkGetDeviceQueue)load(context, "vkGetDeviceQueue");
	table->vkGetEventStatus_volkImpl = (PFN_vkGetEventStatus)load(context, "vkGetEventStatus");
	table->vkGetFenceStatus_volkImpl = (PFN_vkGetFenceStatus)load(context, "vkGetFenceStatus");
	table->vkGetImageMemoryRequirements_volkImpl = (PFN_vkGetImageMemoryRequirements)load(context, "vkGetImageMemoryRequirements");
	table->vkGetImageSparseMemoryRequirements_volkImpl = (PFN_vkGetImageSparseMemoryRequirements)load(context, "vkGetImageSparseMemoryRequirements");
	table->vkGetImageSubresourceLayout_volkImpl = (PFN_vkGetImageSubresourceLayout)load(context, "vkGetImageSubresourceLayout");
	table->vkGetPipelineCacheData_volkImpl = (PFN_vkGetPipelineCacheData)load(context, "vkGetPipelineCacheData");
	table->vkGetQueryPoolResults_volkImpl = (PFN_vkGetQueryPoolResults)load(context, "vkGetQueryPoolResults");
	table->vkGetRenderAreaGranularity_volkImpl = (PFN_vkGetRenderAreaGranularity)load(context, "vkGetRenderAreaGranularity");
	table->vkInvalidateMappedMemoryRanges_volkImpl = (PFN_vkInvalidateMappedMemoryRanges)load(context, "vkInvalidateMappedMemoryRanges");
	table->vkMapMemory_volkImpl = (PFN_vkMapMemory)load(context, "vkMapMemory");
	table->vkMergePipelineCaches_volkImpl = (PFN_vkMergePipelineCaches)load(context, "vkMergePipelineCaches");
	table->vkQueueBindSparse_volkImpl = (PFN_vkQueueBindSparse)load(context, "vkQueueBindSparse");
	table->vkQueueSubmit_volkImpl = (PFN_vkQueueSubmit)load(context, "vkQueueSubmit");
	table->vkQueueWaitIdle_volkImpl = (PFN_vkQueueWaitIdle)load(context, "vkQueueWaitIdle");
	table->vkResetCommandBuffer_volkImpl = (PFN_vkResetCommandBuffer)load(context, "vkResetCommandBuffer");
	table->vkResetCommandPool_volkImpl = (PFN_vkResetCommandPool)load(context, "vkResetCommandPool");
	table->vkResetDescriptorPool_volkImpl = (PFN_vkResetDescriptorPool)load(context, "vkResetDescriptorPool");
	table->vkResetEvent_volkImpl = (PFN_vkResetEvent)load(context, "vkResetEvent");
	table->vkResetFences_volkImpl = (PFN_vkResetFences)load(context, "vkResetFences");
	table->vkSetEvent_volkImpl = (PFN_vkSetEvent)load(context, "vkSetEvent");
	table->vkUnmapMemory_volkImpl = (PFN_vkUnmapMemory)load(context, "vkUnmapMemory");
	table->vkUpdateDescriptorSets_volkImpl = (PFN_vkUpdateDescriptorSets)load(context, "vkUpdateDescriptorSets");
	table->vkWaitForFences_volkImpl = (PFN_vkWaitForFences)load(context, "vkWaitForFences");
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
	table->vkBindBufferMemory2_volkImpl = (PFN_vkBindBufferMemory2)load(context, "vkBindBufferMemory2");
	table->vkBindImageMemory2_volkImpl = (PFN_vkBindImageMemory2)load(context, "vkBindImageMemory2");
	table->vkCmdDispatchBase_volkImpl = (PFN_vkCmdDispatchBase)load(context, "vkCmdDispatchBase");
	table->vkCmdSetDeviceMask_volkImpl = (PFN_vkCmdSetDeviceMask)load(context, "vkCmdSetDeviceMask");
	table->vkCreateDescriptorUpdateTemplate_volkImpl = (PFN_vkCreateDescriptorUpdateTemplate)load(context, "vkCreateDescriptorUpdateTemplate");
	table->vkCreateSamplerYcbcrConversion_volkImpl = (PFN_vkCreateSamplerYcbcrConversion)load(context, "vkCreateSamplerYcbcrConversion");
	table->vkDestroyDescriptorUpdateTemplate_volkImpl = (PFN_vkDestroyDescriptorUpdateTemplate)load(context, "vkDestroyDescriptorUpdateTemplate");
	table->vkDestroySamplerYcbcrConversion_volkImpl = (PFN_vkDestroySamplerYcbcrConversion)load(context, "vkDestroySamplerYcbcrConversion");
	table->vkGetBufferMemoryRequirements2_volkImpl = (PFN_vkGetBufferMemoryRequirements2)load(context, "vkGetBufferMemoryRequirements2");
	table->vkGetDescriptorSetLayoutSupport_volkImpl = (PFN_vkGetDescriptorSetLayoutSupport)load(context, "vkGetDescriptorSetLayoutSupport");
	table->vkGetDeviceGroupPeerMemoryFeatures_volkImpl = (PFN_vkGetDeviceGroupPeerMemoryFeatures)load(context, "vkGetDeviceGroupPeerMemoryFeatures");
	table->vkGetDeviceQueue2_volkImpl = (PFN_vkGetDeviceQueue2)load(context, "vkGetDeviceQueue2");
	table->vkGetImageMemoryRequirements2_volkImpl = (PFN_vkGetImageMemoryRequirements2)load(context, "vkGetImageMemoryRequirements2");
	table->vkGetImageSparseMemoryRequirements2_volkImpl = (PFN_vkGetImageSparseMemoryRequirements2)load(context, "vkGetImageSparseMemoryRequirements2");
	table->vkTrimCommandPool_volkImpl = (PFN_vkTrimCommandPool)load(context, "vkTrimCommandPool");
	table->vkUpdateDescriptorSetWithTemplate_volkImpl = (PFN_vkUpdateDescriptorSetWithTemplate)load(context, "vkUpdateDescriptorSetWithTemplate");
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_2)
	table->vkCmdBeginRenderPass2_volkImpl = (PFN_vkCmdBeginRenderPass2)load(context, "vkCmdBeginRenderPass2");
	table->vkCmdDrawIndexedIndirectCount_volkImpl = (PFN_vkCmdDrawIndexedIndirectCount)load(context, "vkCmdDrawIndexedIndirectCount");
	table->vkCmdDrawIndirectCount_volkImpl = (PFN_vkCmdDrawIndirectCount)load(context, "vkCmdDrawIndirectCount");
	table->vkCmdEndRenderPass2_volkImpl = (PFN_vkCmdEndRenderPass2)load(context, "vkCmdEndRenderPass2");
	table->vkCmdNextSubpass2_volkImpl = (PFN_vkCmdNextSubpass2)load(context, "vkCmdNextSubpass2");
	table->vkCreateRenderPass2_volkImpl = (PFN_vkCreateRenderPass2)load(context, "vkCreateRenderPass2");
	table->vkGetBufferDeviceAddress_volkImpl = (PFN_vkGetBufferDeviceAddress)load(context, "vkGetBufferDeviceAddress");
	table->vkGetBufferOpaqueCaptureAddress_volkImpl = (PFN_vkGetBufferOpaqueCaptureAddress)load(context, "vkGetBufferOpaqueCaptureAddress");
	table->vkGetDeviceMemoryOpaqueCaptureAddress_volkImpl = (PFN_vkGetDeviceMemoryOpaqueCaptureAddress)load(context, "vkGetDeviceMemoryOpaqueCaptureAddress");
	table->vkGetSemaphoreCounterValue_volkImpl = (PFN_vkGetSemaphoreCounterValue)load(context, "vkGetSemaphoreCounterValue");
	table->vkResetQueryPool_volkImpl = (PFN_vkResetQueryPool)load(context, "vkResetQueryPool");
	table->vkSignalSemaphore_volkImpl = (PFN_vkSignalSemaphore)load(context, "vkSignalSemaphore");
	table->vkWaitSemaphores_volkImpl = (PFN_vkWaitSemaphores)load(context, "vkWaitSemaphores");
#endif /* defined(VK_VERSION_1_2) */
#if defined(VK_VERSION_1_3)
	table->vkCmdBeginRendering_volkImpl = (PFN_vkCmdBeginRendering)load(context, "vkCmdBeginRendering");
	table->vkCmdBindVertexBuffers2_volkImpl = (PFN_vkCmdBindVertexBuffers2)load(context, "vkCmdBindVertexBuffers2");
	table->vkCmdBlitImage2_volkImpl = (PFN_vkCmdBlitImage2)load(context, "vkCmdBlitImage2");
	table->vkCmdCopyBuffer2_volkImpl = (PFN_vkCmdCopyBuffer2)load(context, "vkCmdCopyBuffer2");
	table->vkCmdCopyBufferToImage2_volkImpl = (PFN_vkCmdCopyBufferToImage2)load(context, "vkCmdCopyBufferToImage2");
	table->vkCmdCopyImage2_volkImpl = (PFN_vkCmdCopyImage2)load(context, "vkCmdCopyImage2");
	table->vkCmdCopyImageToBuffer2_volkImpl = (PFN_vkCmdCopyImageToBuffer2)load(context, "vkCmdCopyImageToBuffer2");
	table->vkCmdEndRendering_volkImpl = (PFN_vkCmdEndRendering)load(context, "vkCmdEndRendering");
	table->vkCmdPipelineBarrier2_volkImpl = (PFN_vkCmdPipelineBarrier2)load(context, "vkCmdPipelineBarrier2");
	table->vkCmdResetEvent2_volkImpl = (PFN_vkCmdResetEvent2)load(context, "vkCmdResetEvent2");
	table->vkCmdResolveImage2_volkImpl = (PFN_vkCmdResolveImage2)load(context, "vkCmdResolveImage2");
	table->vkCmdSetCullMode_volkImpl = (PFN_vkCmdSetCullMode)load(context, "vkCmdSetCullMode");
	table->vkCmdSetDepthBiasEnable_volkImpl = (PFN_vkCmdSetDepthBiasEnable)load(context, "vkCmdSetDepthBiasEnable");
	table->vkCmdSetDepthBoundsTestEnable_volkImpl = (PFN_vkCmdSetDepthBoundsTestEnable)load(context, "vkCmdSetDepthBoundsTestEnable");
	table->vkCmdSetDepthCompareOp_volkImpl = (PFN_vkCmdSetDepthCompareOp)load(context, "vkCmdSetDepthCompareOp");
	table->vkCmdSetDepthTestEnable_volkImpl = (PFN_vkCmdSetDepthTestEnable)load(context, "vkCmdSetDepthTestEnable");
	table->vkCmdSetDepthWriteEnable_volkImpl = (PFN_vkCmdSetDepthWriteEnable)load(context, "vkCmdSetDepthWriteEnable");
	table->vkCmdSetEvent2_volkImpl = (PFN_vkCmdSetEvent2)load(context, "vkCmdSetEvent2");
	table->vkCmdSetFrontFace_volkImpl = (PFN_vkCmdSetFrontFace)load(context, "vkCmdSetFrontFace");
	table->vkCmdSetPrimitiveRestartEnable_volkImpl = (PFN_vkCmdSetPrimitiveRestartEnable)load(context, "vkCmdSetPrimitiveRestartEnable");
	table->vkCmdSetPrimitiveTopology_volkImpl = (PFN_vkCmdSetPrimitiveTopology)load(context, "vkCmdSetPrimitiveTopology");
	table->vkCmdSetRasterizerDiscardEnable_volkImpl = (PFN_vkCmdSetRasterizerDiscardEnable)load(context, "vkCmdSetRasterizerDiscardEnable");
	table->vkCmdSetScissorWithCount_volkImpl = (PFN_vkCmdSetScissorWithCount)load(context, "vkCmdSetScissorWithCount");
	table->vkCmdSetStencilOp_volkImpl = (PFN_vkCmdSetStencilOp)load(context, "vkCmdSetStencilOp");
	table->vkCmdSetStencilTestEnable_volkImpl = (PFN_vkCmdSetStencilTestEnable)load(context, "vkCmdSetStencilTestEnable");
	table->vkCmdSetViewportWithCount_volkImpl = (PFN_vkCmdSetViewportWithCount)load(context, "vkCmdSetViewportWithCount");
	table->vkCmdWaitEvents2_volkImpl = (PFN_vkCmdWaitEvents2)load(context, "vkCmdWaitEvents2");
	table->vkCmdWriteTimestamp2_volkImpl = (PFN_vkCmdWriteTimestamp2)load(context, "vkCmdWriteTimestamp2");
	table->vkCreatePrivateDataSlot_volkImpl = (PFN_vkCreatePrivateDataSlot)load(context, "vkCreatePrivateDataSlot");
	table->vkDestroyPrivateDataSlot_volkImpl = (PFN_vkDestroyPrivateDataSlot)load(context, "vkDestroyPrivateDataSlot");
	table->vkGetDeviceBufferMemoryRequirements_volkImpl = (PFN_vkGetDeviceBufferMemoryRequirements)load(context, "vkGetDeviceBufferMemoryRequirements");
	table->vkGetDeviceImageMemoryRequirements_volkImpl = (PFN_vkGetDeviceImageMemoryRequirements)load(context, "vkGetDeviceImageMemoryRequirements");
	table->vkGetDeviceImageSparseMemoryRequirements_volkImpl = (PFN_vkGetDeviceImageSparseMemoryRequirements)load(context, "vkGetDeviceImageSparseMemoryRequirements");
	table->vkGetPrivateData_volkImpl = (PFN_vkGetPrivateData)load(context, "vkGetPrivateData");
	table->vkQueueSubmit2_volkImpl = (PFN_vkQueueSubmit2)load(context, "vkQueueSubmit2");
	table->vkSetPrivateData_volkImpl = (PFN_vkSetPrivateData)load(context, "vkSetPrivateData");
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_AMDX_shader_enqueue)
	table->vkCmdDispatchGraphAMDX_volkImpl = (PFN_vkCmdDispatchGraphAMDX)load(context, "vkCmdDispatchGraphAMDX");
	table->vkCmdDispatchGraphIndirectAMDX_volkImpl = (PFN_vkCmdDispatchGraphIndirectAMDX)load(context, "vkCmdDispatchGraphIndirectAMDX");
	table->vkCmdDispatchGraphIndirectCountAMDX_volkImpl = (PFN_vkCmdDispatchGraphIndirectCountAMDX)load(context, "vkCmdDispatchGraphIndirectCountAMDX");
	table->vkCmdInitializeGraphScratchMemoryAMDX_volkImpl = (PFN_vkCmdInitializeGraphScratchMemoryAMDX)load(context, "vkCmdInitializeGraphScratchMemoryAMDX");
	table->vkCreateExecutionGraphPipelinesAMDX_volkImpl = (PFN_vkCreateExecutionGraphPipelinesAMDX)load(context, "vkCreateExecutionGraphPipelinesAMDX");
	table->vkGetExecutionGraphPipelineNodeIndexAMDX_volkImpl = (PFN_vkGetExecutionGraphPipelineNodeIndexAMDX)load(context, "vkGetExecutionGraphPipelineNodeIndexAMDX");
	table->vkGetExecutionGraphPipelineScratchSizeAMDX_volkImpl = (PFN_vkGetExecutionGraphPipelineScratchSizeAMDX)load(context, "vkGetExecutionGraphPipelineScratchSizeAMDX");
#endif /* defined(VK_AMDX_shader_enqueue) */
#if defined(VK_AMD_buffer_marker)
	table->vkCmdWriteBufferMarkerAMD_volkImpl = (PFN_vkCmdWriteBufferMarkerAMD)load(context, "vkCmdWriteBufferMarkerAMD");
#endif /* defined(VK_AMD_buffer_marker) */
#if defined(VK_AMD_display_native_hdr)
	table->vkSetLocalDimmingAMD_volkImpl = (PFN_vkSetLocalDimmingAMD)load(context, "vkSetLocalDimmingAMD");
#endif /* defined(VK_AMD_display_native_hdr) */
#if defined(VK_AMD_draw_indirect_count)
	table->vkCmdDrawIndexedIndirectCountAMD_volkImpl = (PFN_vkCmdDrawIndexedIndirectCountAMD)load(context, "vkCmdDrawIndexedIndirectCountAMD");
	table->vkCmdDrawIndirectCountAMD_volkImpl = (PFN_vkCmdDrawIndirectCountAMD)load(context, "vkCmdDrawIndirectCountAMD");
#endif /* defined(VK_AMD_draw_indirect_count) */
#if defined(VK_AMD_shader_info)
	table->vkGetShaderInfoAMD_volkImpl = (PFN_vkGetShaderInfoAMD)load(context, "vkGetShaderInfoAMD");
#endif /* defined(VK_AMD_shader_info) */
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
	table->vkGetAndroidHardwareBufferPropertiesANDROID_volkImpl = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID)load(context, "vkGetAndroidHardwareBufferPropertiesANDROID");
	table->vkGetMemoryAndroidHardwareBufferANDROID_volkImpl = (PFN_vkGetMemoryAndroidHardwareBufferANDROID)load(context, "vkGetMemoryAndroidHardwareBufferANDROID");
#endif /* defined(VK_ANDROID_external_memory_android_hardware_buffer) */
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state)
	table->vkCmdSetAttachmentFeedbackLoopEnableEXT_volkImpl = (PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT)load(context, "vkCmdSetAttachmentFeedbackLoopEnableEXT");
#endif /* defined(VK_EXT_attachment_feedback_loop_dynamic_state) */
#if defined(VK_EXT_buffer_device_address)
	table->vkGetBufferDeviceAddressEXT_volkImpl = (PFN_vkGetBufferDeviceAddressEXT)load(context, "vkGetBufferDeviceAddressEXT");
#endif /* defined(VK_EXT_buffer_device_address) */
#if defined(VK_EXT_calibrated_timestamps)
	table->vkGetCalibratedTimestampsEXT_volkImpl = (PFN_vkGetCalibratedTimestampsEXT)load(context, "vkGetCalibratedTimestampsEXT");
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_color_write_enable)
	table->vkCmdSetColorWriteEnableEXT_volkImpl = (PFN_vkCmdSetColorWriteEnableEXT)load(context, "vkCmdSetColorWriteEnableEXT");
#endif /* defined(VK_EXT_color_write_enable) */
#if defined(VK_EXT_conditional_rendering)
	table->vkCmdBeginConditionalRenderingEXT_volkImpl = (PFN_vkCmdBeginConditionalRenderingEXT)load(context, "vkCmdBeginConditionalRenderingEXT");
	table->vkCmdEndConditionalRenderingEXT_volkImpl = (PFN_vkCmdEndConditionalRenderingEXT)load(context, "vkCmdEndConditionalRenderingEXT");
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_debug_marker)
	table->vkCmdDebugMarkerBeginEXT_volkImpl = (PFN_vkCmdDebugMarkerBeginEXT)load(context, "vkCmdDebugMarkerBeginEXT");
	table->vkCmdDebugMarkerEndEXT_volkImpl = (PFN_vkCmdDebugMarkerEndEXT)load(context, "vkCmdDebugMarkerEndEXT");
	table->vkCmdDebugMarkerInsertEXT_volkImpl = (PFN_vkCmdDebugMarkerInsertEXT)load(context, "vkCmdDebugMarkerInsertEXT");
	table->vkDebugMarkerSetObjectNameEXT_volkImpl = (PFN_vkDebugMarkerSetObjectNameEXT)load(context, "vkDebugMarkerSetObjectNameEXT");
	table->vkDebugMarkerSetObjectTagEXT_volkImpl = (PFN_vkDebugMarkerSetObjectTagEXT)load(context, "vkDebugMarkerSetObjectTagEXT");
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_depth_bias_control)
	table->vkCmdSetDepthBias2EXT_volkImpl = (PFN_vkCmdSetDepthBias2EXT)load(context, "vkCmdSetDepthBias2EXT");
#endif /* defined(VK_EXT_depth_bias_control) */
#if defined(VK_EXT_descriptor_buffer)
	table->vkCmdBindDescriptorBufferEmbeddedSamplersEXT_volkImpl = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT)load(context, "vkCmdBindDescriptorBufferEmbeddedSamplersEXT");
	table->vkCmdBindDescriptorBuffersEXT_volkImpl = (PFN_vkCmdBindDescriptorBuffersEXT)load(context, "vkCmdBindDescriptorBuffersEXT");
	table->vkCmdSetDescriptorBufferOffsetsEXT_volkImpl = (PFN_vkCmdSetDescriptorBufferOffsetsEXT)load(context, "vkCmdSetDescriptorBufferOffsetsEXT");
	table->vkGetBufferOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT)load(context, "vkGetBufferOpaqueCaptureDescriptorDataEXT");
	table->vkGetDescriptorEXT_volkImpl = (PFN_vkGetDescriptorEXT)load(context, "vkGetDescriptorEXT");
	table->vkGetDescriptorSetLayoutBindingOffsetEXT_volkImpl = (PFN_vkGetDescriptorSetLayoutBindingOffsetEXT)load(context, "vkGetDescriptorSetLayoutBindingOffsetEXT");
	table->vkGetDescriptorSetLayoutSizeEXT_volkImpl = (PFN_vkGetDescriptorSetLayoutSizeEXT)load(context, "vkGetDescriptorSetLayoutSizeEXT");
	table->vkGetImageOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetImageOpaqueCaptureDescriptorDataEXT)load(context, "vkGetImageOpaqueCaptureDescriptorDataEXT");
	table->vkGetImageViewOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT)load(context, "vkGetImageViewOpaqueCaptureDescriptorDataEXT");
	table->vkGetSamplerOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT)load(context, "vkGetSamplerOpaqueCaptureDescriptorDataEXT");
#endif /* defined(VK_EXT_descriptor_buffer) */
#if defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing))
	table->vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT)load(context, "vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT");
#endif /* defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)) */
#if defined(VK_EXT_device_fault)
	table->vkGetDeviceFaultInfoEXT_volkImpl = (PFN_vkGetDeviceFaultInfoEXT)load(context, "vkGetDeviceFaultInfoEXT");
#endif /* defined(VK_EXT_device_fault) */
#if defined(VK_EXT_discard_rectangles)
	table->vkCmdSetDiscardRectangleEXT_volkImpl = (PFN_vkCmdSetDiscardRectangleEXT)load(context, "vkCmdSetDiscardRectangleEXT");
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2
	table->vkCmdSetDiscardRectangleEnableEXT_volkImpl = (PFN_vkCmdSetDiscardRectangleEnableEXT)load(context, "vkCmdSetDiscardRectangleEnableEXT");
	table->vkCmdSetDiscardRectangleModeEXT_volkImpl = (PFN_vkCmdSetDiscardRectangleModeEXT)load(context, "vkCmdSetDiscardRectangleModeEXT");
#endif /* defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2 */
#if defined(VK_EXT_display_control)
	table->vkDisplayPowerControlEXT_volkImpl = (PFN_vkDisplayPowerControlEXT)load(context, "vkDisplayPowerControlEXT");
	table->vkGetSwapchainCounterEXT_volkImpl = (PFN_vkGetSwapchainCounterEXT)load(context, "vkGetSwapchainCounterEXT");
	table->vkRegisterDeviceEventEXT_volkImpl = (PFN_vkRegisterDeviceEventEXT)load(context, "vkRegisterDeviceEventEXT");
	table->vkRegisterDisplayEventEXT_volkImpl = (PFN_vkRegisterDisplayEventEXT)load(context, "vkRegisterDisplayEventEXT");
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_external_memory_host)
	table->vkGetMemoryHostPointerPropertiesEXT_volkImpl = (PFN_vkGetMemoryHostPointerPropertiesEXT)load(context, "vkGetMemoryHostPointerPropertiesEXT");
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_full_screen_exclusive)
	table->vkAcquireFullScreenExclusiveModeEXT_volkImpl = (PFN_vkAcquireFullScreenExclusiveModeEXT)load(context, "vkAcquireFullScreenExclusiveModeEXT");
	table->vkReleaseFullScreenExclusiveModeEXT_volkImpl = (PFN_vkReleaseFullScreenExclusiveModeEXT)load(context, "vkReleaseFullScreenExclusiveModeEXT");
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_hdr_metadata)
	table->vkSetHdrMetadataEXT_volkImpl = (PFN_vkSetHdrMetadataEXT)load(context, "vkSetHdrMetadataEXT");
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_host_image_copy)
	table->vkCopyImageToImageEXT_volkImpl = (PFN_vkCopyImageToImageEXT)load(context, "vkCopyImageToImageEXT");
	table->vkCopyImageToMemoryEXT_volkImpl = (PFN_vkCopyImageToMemoryEXT)load(context, "vkCopyImageToMemoryEXT");
	table->vkCopyMemoryToImageEXT_volkImpl = (PFN_vkCopyMemoryToImageEXT)load(context, "vkCopyMemoryToImageEXT");
	table->vkTransitionImageLayoutEXT_volkImpl = (PFN_vkTransitionImageLayoutEXT)load(context, "vkTransitionImageLayoutEXT");
#endif /* defined(VK_EXT_host_image_copy) */
#if defined(VK_EXT_host_query_reset)
	table->vkResetQueryPoolEXT_volkImpl = (PFN_vkResetQueryPoolEXT)load(context, "vkResetQueryPoolEXT");
#endif /* defined(VK_EXT_host_query_reset) */
#if defined(VK_EXT_image_drm_format_modifier)
	table->vkGetImageDrmFormatModifierPropertiesEXT_volkImpl = (PFN_vkGetImageDrmFormatModifierPropertiesEXT)load(context, "vkGetImageDrmFormatModifierPropertiesEXT");
#endif /* defined(VK_EXT_image_drm_format_modifier) */
#if defined(VK_EXT_line_rasterization)
	table->vkCmdSetLineStippleEXT_volkImpl = (PFN_vkCmdSetLineStippleEXT)load(context, "vkCmdSetLineStippleEXT");
#endif /* defined(VK_EXT_line_rasterization) */
#if defined(VK_EXT_mesh_shader)
	table->vkCmdDrawMeshTasksEXT_volkImpl = (PFN_vkCmdDrawMeshTasksEXT)load(context, "vkCmdDrawMeshTasksEXT");
	table->vkCmdDrawMeshTasksIndirectCountEXT_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectCountEXT)load(context, "vkCmdDrawMeshTasksIndirectCountEXT");
	table->vkCmdDrawMeshTasksIndirectEXT_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectEXT)load(context, "vkCmdDrawMeshTasksIndirectEXT");
#endif /* defined(VK_EXT_mesh_shader) */
#if defined(VK_EXT_metal_objects)
	table->vkExportMetalObjectsEXT_volkImpl = (PFN_vkExportMetalObjectsEXT)load(context, "vkExportMetalObjectsEXT");
#endif /* defined(VK_EXT_metal_objects) */
#if defined(VK_EXT_multi_draw)
	table->vkCmdDrawMultiEXT_volkImpl = (PFN_vkCmdDrawMultiEXT)load(context, "vkCmdDrawMultiEXT");
	table->vkCmdDrawMultiIndexedEXT_volkImpl = (PFN_vkCmdDrawMultiIndexedEXT)load(context, "vkCmdDrawMultiIndexedEXT");
#endif /* defined(VK_EXT_multi_draw) */
#if defined(VK_EXT_opacity_micromap)
	table->vkBuildMicromapsEXT_volkImpl = (PFN_vkBuildMicromapsEXT)load(context, "vkBuildMicromapsEXT");
	table->vkCmdBuildMicromapsEXT_volkImpl = (PFN_vkCmdBuildMicromapsEXT)load(context, "vkCmdBuildMicromapsEXT");
	table->vkCmdCopyMemoryToMicromapEXT_volkImpl = (PFN_vkCmdCopyMemoryToMicromapEXT)load(context, "vkCmdCopyMemoryToMicromapEXT");
	table->vkCmdCopyMicromapEXT_volkImpl = (PFN_vkCmdCopyMicromapEXT)load(context, "vkCmdCopyMicromapEXT");
	table->vkCmdCopyMicromapToMemoryEXT_volkImpl = (PFN_vkCmdCopyMicromapToMemoryEXT)load(context, "vkCmdCopyMicromapToMemoryEXT");
	table->vkCmdWriteMicromapsPropertiesEXT_volkImpl = (PFN_vkCmdWriteMicromapsPropertiesEXT)load(context, "vkCmdWriteMicromapsPropertiesEXT");
	table->vkCopyMemoryToMicromapEXT_volkImpl = (PFN_vkCopyMemoryToMicromapEXT)load(context, "vkCopyMemoryToMicromapEXT");
	table->vkCopyMicromapEXT_volkImpl = (PFN_vkCopyMicromapEXT)load(context, "vkCopyMicromapEXT");
	table->vkCopyMicromapToMemoryEXT_volkImpl = (PFN_vkCopyMicromapToMemoryEXT)load(context, "vkCopyMicromapToMemoryEXT");
	table->vkCreateMicromapEXT_volkImpl = (PFN_vkCreateMicromapEXT)load(context, "vkCreateMicromapEXT");
	table->vkDestroyMicromapEXT_volkImpl = (PFN_vkDestroyMicromapEXT)load(context, "vkDestroyMicromapEXT");
	table->vkGetDeviceMicromapCompatibilityEXT_volkImpl = (PFN_vkGetDeviceMicromapCompatibilityEXT)load(context, "vkGetDeviceMicromapCompatibilityEXT");
	table->vkGetMicromapBuildSizesEXT_volkImpl = (PFN_vkGetMicromapBuildSizesEXT)load(context, "vkGetMicromapBuildSizesEXT");
	table->vkWriteMicromapsPropertiesEXT_volkImpl = (PFN_vkWriteMicromapsPropertiesEXT)load(context, "vkWriteMicromapsPropertiesEXT");
#endif /* defined(VK_EXT_opacity_micromap) */
#if defined(VK_EXT_pageable_device_local_memory)
	table->vkSetDeviceMemoryPriorityEXT_volkImpl = (PFN_vkSetDeviceMemoryPriorityEXT)load(context, "vkSetDeviceMemoryPriorityEXT");
#endif /* defined(VK_EXT_pageable_device_local_memory) */
#if defined(VK_EXT_pipeline_properties)
	table->vkGetPipelinePropertiesEXT_volkImpl = (PFN_vkGetPipelinePropertiesEXT)load(context, "vkGetPipelinePropertiesEXT");
#endif /* defined(VK_EXT_pipeline_properties) */
#if defined(VK_EXT_private_data)
	table->vkCreatePrivateDataSlotEXT_volkImpl = (PFN_vkCreatePrivateDataSlotEXT)load(context, "vkCreatePrivateDataSlotEXT");
	table->vkDestroyPrivateDataSlotEXT_volkImpl = (PFN_vkDestroyPrivateDataSlotEXT)load(context, "vkDestroyPrivateDataSlotEXT");
	table->vkGetPrivateDataEXT_volkImpl = (PFN_vkGetPrivateDataEXT)load(context, "vkGetPrivateDataEXT");
	table->vkSetPrivateDataEXT_volkImpl = (PFN_vkSetPrivateDataEXT)load(context, "vkSetPrivateDataEXT");
#endif /* defined(VK_EXT_private_data) */
#if defined(VK_EXT_sample_locations)
	table->vkCmdSetSampleLocationsEXT_volkImpl = (PFN_vkCmdSetSampleLocationsEXT)load(context, "vkCmdSetSampleLocationsEXT");
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_shader_module_identifier)
	table->vkGetShaderModuleCreateInfoIdentifierEXT_volkImpl = (PFN_vkGetShaderModuleCreateInfoIdentifierEXT)load(context, "vkGetShaderModuleCreateInfoIdentifierEXT");
	table->vkGetShaderModuleIdentifierEXT_volkImpl = (PFN_vkGetShaderModuleIdentifierEXT)load(context, "vkGetShaderModuleIdentifierEXT");
#endif /* defined(VK_EXT_shader_module_identifier) */
#if defined(VK_EXT_shader_object)
	table->vkCmdBindShadersEXT_volkImpl = (PFN_vkCmdBindShadersEXT)load(context, "vkCmdBindShadersEXT");
	table->vkCreateShadersEXT_volkImpl = (PFN_vkCreateShadersEXT)load(context, "vkCreateShadersEXT");
	table->vkDestroyShaderEXT_volkImpl = (PFN_vkDestroyShaderEXT)load(context, "vkDestroyShaderEXT");
	table->vkGetShaderBinaryDataEXT_volkImpl = (PFN_vkGetShaderBinaryDataEXT)load(context, "vkGetShaderBinaryDataEXT");
#endif /* defined(VK_EXT_shader_object) */
#if defined(VK_EXT_swapchain_maintenance1)
	table->vkReleaseSwapchainImagesEXT_volkImpl = (PFN_vkReleaseSwapchainImagesEXT)load(context, "vkReleaseSwapchainImagesEXT");
#endif /* defined(VK_EXT_swapchain_maintenance1) */
#if defined(VK_EXT_transform_feedback)
	table->vkCmdBeginQueryIndexedEXT_volkImpl = (PFN_vkCmdBeginQueryIndexedEXT)load(context, "vkCmdBeginQueryIndexedEXT");
	table->vkCmdBeginTransformFeedbackEXT_volkImpl = (PFN_vkCmdBeginTransformFeedbackEXT)load(context, "vkCmdBeginTransformFeedbackEXT");
	table->vkCmdBindTransformFeedbackBuffersEXT_volkImpl = (PFN_vkCmdBindTransformFeedbackBuffersEXT)load(context, "vkCmdBindTransformFeedbackBuffersEXT");
	table->vkCmdDrawIndirectByteCountEXT_volkImpl = (PFN_vkCmdDrawIndirectByteCountEXT)load(context, "vkCmdDrawIndirectByteCountEXT");
	table->vkCmdEndQueryIndexedEXT_volkImpl = (PFN_vkCmdEndQueryIndexedEXT)load(context, "vkCmdEndQueryIndexedEXT");
	table->vkCmdEndTransformFeedbackEXT_volkImpl = (PFN_vkCmdEndTransformFeedbackEXT)load(context, "vkCmdEndTransformFeedbackEXT");
#endif /* defined(VK_EXT_transform_feedback) */
#if defined(VK_EXT_validation_cache)
	table->vkCreateValidationCacheEXT_volkImpl = (PFN_vkCreateValidationCacheEXT)load(context, "vkCreateValidationCacheEXT");
	table->vkDestroyValidationCacheEXT_volkImpl = (PFN_vkDestroyValidationCacheEXT)load(context, "vkDestroyValidationCacheEXT");
	table->vkGetValidationCacheDataEXT_volkImpl = (PFN_vkGetValidationCacheDataEXT)load(context, "vkGetValidationCacheDataEXT");
	table->vkMergeValidationCachesEXT_volkImpl = (PFN_vkMergeValidationCachesEXT)load(context, "vkMergeValidationCachesEXT");
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_FUCHSIA_buffer_collection)
	table->vkCreateBufferCollectionFUCHSIA_volkImpl = (PFN_vkCreateBufferCollectionFUCHSIA)load(context, "vkCreateBufferCollectionFUCHSIA");
	table->vkDestroyBufferCollectionFUCHSIA_volkImpl = (PFN_vkDestroyBufferCollectionFUCHSIA)load(context, "vkDestroyBufferCollectionFUCHSIA");
	table->vkGetBufferCollectionPropertiesFUCHSIA_volkImpl = (PFN_vkGetBufferCollectionPropertiesFUCHSIA)load(context, "vkGetBufferCollectionPropertiesFUCHSIA");
	table->vkSetBufferCollectionBufferConstraintsFUCHSIA_volkImpl = (PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA)load(context, "vkSetBufferCollectionBufferConstraintsFUCHSIA");
	table->vkSetBufferCollectionImageConstraintsFUCHSIA_volkImpl = (PFN_vkSetBufferCollectionImageConstraintsFUCHSIA)load(context, "vkSetBufferCollectionImageConstraintsFUCHSIA");
#endif /* defined(VK_FUCHSIA_buffer_collection) */
#if defined(VK_FUCHSIA_external_memory)
	table->vkGetMemoryZirconHandleFUCHSIA_volkImpl = (PFN_vkGetMemoryZirconHandleFUCHSIA)load(context, "vkGetMemoryZirconHandleFUCHSIA");
	table->vkGetMemoryZirconHandlePropertiesFUCHSIA_volkImpl = (PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA)load(context, "vkGetMemoryZirconHandlePropertiesFUCHSIA");
#endif /* defined(VK_FUCHSIA_external_memory) */
#if defined(VK_FUCHSIA_external_semaphore)
	table->vkGetSemaphoreZirconHandleFUCHSIA_volkImpl = (PFN_vkGetSemaphoreZirconHandleFUCHSIA)load(context, "vkGetSemaphoreZirconHandleFUCHSIA");
	table->vkImportSemaphoreZirconHandleFUCHSIA_volkImpl = (PFN_vkImportSemaphoreZirconHandleFUCHSIA)load(context, "vkImportSemaphoreZirconHandleFUCHSIA");
#endif /* defined(VK_FUCHSIA_external_semaphore) */
#if defined(VK_GOOGLE_display_timing)
	table->vkGetPastPresentationTimingGOOGLE_volkImpl = (PFN_vkGetPastPresentationTimingGOOGLE)load(context, "vkGetPastPresentationTimingGOOGLE");
	table->vkGetRefreshCycleDurationGOOGLE_volkImpl = (PFN_vkGetRefreshCycleDurationGOOGLE)load(context, "vkGetRefreshCycleDurationGOOGLE");
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_HUAWEI_cluster_culling_shader)
	table->vkCmdDrawClusterHUAWEI_volkImpl = (PFN_vkCmdDrawClusterHUAWEI)load(context, "vkCmdDrawClusterHUAWEI");
	table->vkCmdDrawClusterIndirectHUAWEI_volkImpl = (PFN_vkCmdDrawClusterIndirectHUAWEI)load(context, "vkCmdDrawClusterIndirectHUAWEI");
#endif /* defined(VK_HUAWEI_cluster_culling_shader) */
#if defined(VK_HUAWEI_invocation_mask)
	table->vkCmdBindInvocationMaskHUAWEI_volkImpl = (PFN_vkCmdBindInvocationMaskHUAWEI)load(context, "vkCmdBindInvocationMaskHUAWEI");
#endif /* defined(VK_HUAWEI_invocation_mask) */
#if defined(VK_HUAWEI_subpass_shading)
	table->vkCmdSubpassShadingHUAWEI_volkImpl = (PFN_vkCmdSubpassShadingHUAWEI)load(context, "vkCmdSubpassShadingHUAWEI");
	table->vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI_volkImpl = (PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI)load(context, "vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI");
#endif /* defined(VK_HUAWEI_subpass_shading) */
#if defined(VK_INTEL_performance_query)
	table->vkAcquirePerformanceConfigurationINTEL_volkImpl = (PFN_vkAcquirePerformanceConfigurationINTEL)load(context, "vkAcquirePerformanceConfigurationINTEL");
	table->vkCmdSetPerformanceMarkerINTEL_volkImpl = (PFN_vkCmdSetPerformanceMarkerINTEL)load(context, "vkCmdSetPerformanceMarkerINTEL");
	table->vkCmdSetPerformanceOverrideINTEL_volkImpl = (PFN_vkCmdSetPerformanceOverrideINTEL)load(context, "vkCmdSetPerformanceOverrideINTEL");
	table->vkCmdSetPerformanceStreamMarkerINTEL_volkImpl = (PFN_vkCmdSetPerformanceStreamMarkerINTEL)load(context, "vkCmdSetPerformanceStreamMarkerINTEL");
	table->vkGetPerformanceParameterINTEL_volkImpl = (PFN_vkGetPerformanceParameterINTEL)load(context, "vkGetPerformanceParameterINTEL");
	table->vkInitializePerformanceApiINTEL_volkImpl = (PFN_vkInitializePerformanceApiINTEL)load(context, "vkInitializePerformanceApiINTEL");
	table->vkQueueSetPerformanceConfigurationINTEL_volkImpl = (PFN_vkQueueSetPerformanceConfigurationINTEL)load(context, "vkQueueSetPerformanceConfigurationINTEL");
	table->vkReleasePerformanceConfigurationINTEL_volkImpl = (PFN_vkReleasePerformanceConfigurationINTEL)load(context, "vkReleasePerformanceConfigurationINTEL");
	table->vkUninitializePerformanceApiINTEL_volkImpl = (PFN_vkUninitializePerformanceApiINTEL)load(context, "vkUninitializePerformanceApiINTEL");
#endif /* defined(VK_INTEL_performance_query) */
#if defined(VK_KHR_acceleration_structure)
	table->vkBuildAccelerationStructuresKHR_volkImpl = (PFN_vkBuildAccelerationStructuresKHR)load(context, "vkBuildAccelerationStructuresKHR");
	table->vkCmdBuildAccelerationStructuresIndirectKHR_volkImpl = (PFN_vkCmdBuildAccelerationStructuresIndirectKHR)load(context, "vkCmdBuildAccelerationStructuresIndirectKHR");
	table->vkCmdBuildAccelerationStructuresKHR_volkImpl = (PFN_vkCmdBuildAccelerationStructuresKHR)load(context, "vkCmdBuildAccelerationStructuresKHR");
	table->vkCmdCopyAccelerationStructureKHR_volkImpl = (PFN_vkCmdCopyAccelerationStructureKHR)load(context, "vkCmdCopyAccelerationStructureKHR");
	table->vkCmdCopyAccelerationStructureToMemoryKHR_volkImpl = (PFN_vkCmdCopyAccelerationStructureToMemoryKHR)load(context, "vkCmdCopyAccelerationStructureToMemoryKHR");
	table->vkCmdCopyMemoryToAccelerationStructureKHR_volkImpl = (PFN_vkCmdCopyMemoryToAccelerationStructureKHR)load(context, "vkCmdCopyMemoryToAccelerationStructureKHR");
	table->vkCmdWriteAccelerationStructuresPropertiesKHR_volkImpl = (PFN_vkCmdWriteAccelerationStructuresPropertiesKHR)load(context, "vkCmdWriteAccelerationStructuresPropertiesKHR");
	table->vkCopyAccelerationStructureKHR_volkImpl = (PFN_vkCopyAccelerationStructureKHR)load(context, "vkCopyAccelerationStructureKHR");
	table->vkCopyAccelerationStructureToMemoryKHR_volkImpl = (PFN_vkCopyAccelerationStructureToMemoryKHR)load(context, "vkCopyAccelerationStructureToMemoryKHR");
	table->vkCopyMemoryToAccelerationStructureKHR_volkImpl = (PFN_vkCopyMemoryToAccelerationStructureKHR)load(context, "vkCopyMemoryToAccelerationStructureKHR");
	table->vkCreateAccelerationStructureKHR_volkImpl = (PFN_vkCreateAccelerationStructureKHR)load(context, "vkCreateAccelerationStructureKHR");
	table->vkDestroyAccelerationStructureKHR_volkImpl = (PFN_vkDestroyAccelerationStructureKHR)load(context, "vkDestroyAccelerationStructureKHR");
	table->vkGetAccelerationStructureBuildSizesKHR_volkImpl = (PFN_vkGetAccelerationStructureBuildSizesKHR)load(context, "vkGetAccelerationStructureBuildSizesKHR");
	table->vkGetAccelerationStructureDeviceAddressKHR_volkImpl = (PFN_vkGetAccelerationStructureDeviceAddressKHR)load(context, "vkGetAccelerationStructureDeviceAddressKHR");
	table->vkGetDeviceAccelerationStructureCompatibilityKHR_volkImpl = (PFN_vkGetDeviceAccelerationStructureCompatibilityKHR)load(context, "vkGetDeviceAccelerationStructureCompatibilityKHR");
	table->vkWriteAccelerationStructuresPropertiesKHR_volkImpl = (PFN_vkWriteAccelerationStructuresPropertiesKHR)load(context, "vkWriteAccelerationStructuresPropertiesKHR");
#endif /* defined(VK_KHR_acceleration_structure) */
#if defined(VK_KHR_bind_memory2)
	table->vkBindBufferMemory2KHR_volkImpl = (PFN_vkBindBufferMemory2KHR)load(context, "vkBindBufferMemory2KHR");
	table->vkBindImageMemory2KHR_volkImpl = (PFN_vkBindImageMemory2KHR)load(context, "vkBindImageMemory2KHR");
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_buffer_device_address)
	table->vkGetBufferDeviceAddressKHR_volkImpl = (PFN_vkGetBufferDeviceAddressKHR)load(context, "vkGetBufferDeviceAddressKHR");
	table->vkGetBufferOpaqueCaptureAddressKHR_volkImpl = (PFN_vkGetBufferOpaqueCaptureAddressKHR)load(context, "vkGetBufferOpaqueCaptureAddressKHR");
	table->vkGetDeviceMemoryOpaqueCaptureAddressKHR_volkImpl = (PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR)load(context, "vkGetDeviceMemoryOpaqueCaptureAddressKHR");
#endif /* defined(VK_KHR_buffer_device_address) */
#if defined(VK_KHR_calibrated_timestamps)
	table->vkGetCalibratedTimestampsKHR_volkImpl = (PFN_vkGetCalibratedTimestampsKHR)load(context, "vkGetCalibratedTimestampsKHR");
#endif /* defined(VK_KHR_calibrated_timestamps) */
#if defined(VK_KHR_copy_commands2)
	table->vkCmdBlitImage2KHR_volkImpl = (PFN_vkCmdBlitImage2KHR)load(context, "vkCmdBlitImage2KHR");
	table->vkCmdCopyBuffer2KHR_volkImpl = (PFN_vkCmdCopyBuffer2KHR)load(context, "vkCmdCopyBuffer2KHR");
	table->vkCmdCopyBufferToImage2KHR_volkImpl = (PFN_vkCmdCopyBufferToImage2KHR)load(context, "vkCmdCopyBufferToImage2KHR");
	table->vkCmdCopyImage2KHR_volkImpl = (PFN_vkCmdCopyImage2KHR)load(context, "vkCmdCopyImage2KHR");
	table->vkCmdCopyImageToBuffer2KHR_volkImpl = (PFN_vkCmdCopyImageToBuffer2KHR)load(context, "vkCmdCopyImageToBuffer2KHR");
	table->vkCmdResolveImage2KHR_volkImpl = (PFN_vkCmdResolveImage2KHR)load(context, "vkCmdResolveImage2KHR");
#endif /* defined(VK_KHR_copy_commands2) */
#if defined(VK_KHR_create_renderpass2)
	table->vkCmdBeginRenderPass2KHR_volkImpl = (PFN_vkCmdBeginRenderPass2KHR)load(context, "vkCmdBeginRenderPass2KHR");
	table->vkCmdEndRenderPass2KHR_volkImpl = (PFN_vkCmdEndRenderPass2KHR)load(context, "vkCmdEndRenderPass2KHR");
	table->vkCmdNextSubpass2KHR_volkImpl = (PFN_vkCmdNextSubpass2KHR)load(context, "vkCmdNextSubpass2KHR");
	table->vkCreateRenderPass2KHR_volkImpl = (PFN_vkCreateRenderPass2KHR)load(context, "vkCreateRenderPass2KHR");
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_deferred_host_operations)
	table->vkCreateDeferredOperationKHR_volkImpl = (PFN_vkCreateDeferredOperationKHR)load(context, "vkCreateDeferredOperationKHR");
	table->vkDeferredOperationJoinKHR_volkImpl = (PFN_vkDeferredOperationJoinKHR)load(context, "vkDeferredOperationJoinKHR");
	table->vkDestroyDeferredOperationKHR_volkImpl = (PFN_vkDestroyDeferredOperationKHR)load(context, "vkDestroyDeferredOperationKHR");
	table->vkGetDeferredOperationMaxConcurrencyKHR_volkImpl = (PFN_vkGetDeferredOperationMaxConcurrencyKHR)load(context, "vkGetDeferredOperationMaxConcurrencyKHR");
	table->vkGetDeferredOperationResultKHR_volkImpl = (PFN_vkGetDeferredOperationResultKHR)load(context, "vkGetDeferredOperationResultKHR");
#endif /* defined(VK_KHR_deferred_host_operations) */
#if defined(VK_KHR_descriptor_update_template)
	table->vkCreateDescriptorUpdateTemplateKHR_volkImpl = (PFN_vkCreateDescriptorUpdateTemplateKHR)load(context, "vkCreateDescriptorUpdateTemplateKHR");
	table->vkDestroyDescriptorUpdateTemplateKHR_volkImpl = (PFN_vkDestroyDescriptorUpdateTemplateKHR)load(context, "vkDestroyDescriptorUpdateTemplateKHR");
	table->vkUpdateDescriptorSetWithTemplateKHR_volkImpl = (PFN_vkUpdateDescriptorSetWithTemplateKHR)load(context, "vkUpdateDescriptorSetWithTemplateKHR");
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
	table->vkCmdDispatchBaseKHR_volkImpl = (PFN_vkCmdDispatchBaseKHR)load(context, "vkCmdDispatchBaseKHR");
	table->vkCmdSetDeviceMaskKHR_volkImpl = (PFN_vkCmdSetDeviceMaskKHR)load(context, "vkCmdSetDeviceMaskKHR");
	table->vkGetDeviceGroupPeerMemoryFeaturesKHR_volkImpl = (PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR)load(context, "vkGetDeviceGroupPeerMemoryFeaturesKHR");
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_display_swapchain)
	table->vkCreateSharedSwapchainsKHR_volkImpl = (PFN_vkCreateSharedSwapchainsKHR)load(context, "vkCreateSharedSwapchainsKHR");
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
	table->vkCmdDrawIndexedIndirectCountKHR_volkImpl = (PFN_vkCmdDrawIndexedIndirectCountKHR)load(context, "vkCmdDrawIndexedIndirectCountKHR");
	table->vkCmdDrawIndirectCountKHR_volkImpl = (PFN_vkCmdDrawIndirectCountKHR)load(context, "vkCmdDrawIndirectCountKHR");
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_dynamic_rendering)
	table->vkCmdBeginRenderingKHR_volkImpl = (PFN_vkCmdBeginRenderingKHR)load(context, "vkCmdBeginRenderingKHR");
	table->vkCmdEndRenderingKHR_volkImpl = (PFN_vkCmdEndRenderingKHR)load(context, "vkCmdEndRenderingKHR");
#endif /* defined(VK_KHR_dynamic_rendering) */
#if defined(VK_KHR_dynamic_rendering_local_read)
	table->vkCmdSetRenderingAttachmentLocationsKHR_volkImpl = (PFN_vkCmdSetRenderingAttachmentLocationsKHR)load(context, "vkCmdSetRenderingAttachmentLocationsKHR");
	table->vkCmdSetRenderingInputAttachmentIndicesKHR_volkImpl = (PFN_vkCmdSetRenderingInputAttachmentIndicesKHR)load(context, "vkCmdSetRenderingInputAttachmentIndicesKHR");
#endif /* defined(VK_KHR_dynamic_rendering_local_read) */
#if defined(VK_KHR_external_fence_fd)
	table->vkGetFenceFdKHR_volkImpl = (PFN_vkGetFenceFdKHR)load(context, "vkGetFenceFdKHR");
	table->vkImportFenceFdKHR_volkImpl = (PFN_vkImportFenceFdKHR)load(context, "vkImportFenceFdKHR");
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
	table->vkGetFenceWin32HandleKHR_volkImpl = (PFN_vkGetFenceWin32HandleKHR)load(context, "vkGetFenceWin32HandleKHR");
	table->vkImportFenceWin32HandleKHR_volkImpl = (PFN_vkImportFenceWin32HandleKHR)load(context, "vkImportFenceWin32HandleKHR");
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_fd)
	table->vkGetMemoryFdKHR_volkImpl = (PFN_vkGetMemoryFdKHR)load(context, "vkGetMemoryFdKHR");
	table->vkGetMemoryFdPropertiesKHR_volkImpl = (PFN_vkGetMemoryFdPropertiesKHR)load(context, "vkGetMemoryFdPropertiesKHR");
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
	table->vkGetMemoryWin32HandleKHR_volkImpl = (PFN_vkGetMemoryWin32HandleKHR)load(context, "vkGetMemoryWin32HandleKHR");
	table->vkGetMemoryWin32HandlePropertiesKHR_volkImpl = (PFN_vkGetMemoryWin32HandlePropertiesKHR)load(context, "vkGetMemoryWin32HandlePropertiesKHR");
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_fd)
	table->vkGetSemaphoreFdKHR_volkImpl = (PFN_vkGetSemaphoreFdKHR)load(context, "vkGetSemaphoreFdKHR");
	table->vkImportSemaphoreFdKHR_volkImpl = (PFN_vkImportSemaphoreFdKHR)load(context, "vkImportSemaphoreFdKHR");
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
	table->vkGetSemaphoreWin32HandleKHR_volkImpl = (PFN_vkGetSemaphoreWin32HandleKHR)load(context, "vkGetSemaphoreWin32HandleKHR");
	table->vkImportSemaphoreWin32HandleKHR_volkImpl = (PFN_vkImportSemaphoreWin32HandleKHR)load(context, "vkImportSemaphoreWin32HandleKHR");
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_fragment_shading_rate)
	table->vkCmdSetFragmentShadingRateKHR_volkImpl = (PFN_vkCmdSetFragmentShadingRateKHR)load(context, "vkCmdSetFragmentShadingRateKHR");
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_memory_requirements2)
	table->vkGetBufferMemoryRequirements2KHR_volkImpl = (PFN_vkGetBufferMemoryRequirements2KHR)load(context, "vkGetBufferMemoryRequirements2KHR");
	table->vkGetImageMemoryRequirements2KHR_volkImpl = (PFN_vkGetImageMemoryRequirements2KHR)load(context, "vkGetImageMemoryRequirements2KHR");
	table->vkGetImageSparseMemoryRequirements2KHR_volkImpl = (PFN_vkGetImageSparseMemoryRequirements2KHR)load(context, "vkGetImageSparseMemoryRequirements2KHR");
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_line_rasterization)
	table->vkCmdSetLineStippleKHR_volkImpl = (PFN_vkCmdSetLineStippleKHR)load(context, "vkCmdSetLineStippleKHR");
#endif /* defined(VK_KHR_line_rasterization) */
#if defined(VK_KHR_maintenance1)
	table->vkTrimCommandPoolKHR_volkImpl = (PFN_vkTrimCommandPoolKHR)load(context, "vkTrimCommandPoolKHR");
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance3)
	table->vkGetDescriptorSetLayoutSupportKHR_volkImpl = (PFN_vkGetDescriptorSetLayoutSupportKHR)load(context, "vkGetDescriptorSetLayoutSupportKHR");
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_maintenance4)
	table->vkGetDeviceBufferMemoryRequirementsKHR_volkImpl = (PFN_vkGetDeviceBufferMemoryRequirementsKHR)load(context, "vkGetDeviceBufferMemoryRequirementsKHR");
	table->vkGetDeviceImageMemoryRequirementsKHR_volkImpl = (PFN_vkGetDeviceImageMemoryRequirementsKHR)load(context, "vkGetDeviceImageMemoryRequirementsKHR");
	table->vkGetDeviceImageSparseMemoryRequirementsKHR_volkImpl = (PFN_vkGetDeviceImageSparseMemoryRequirementsKHR)load(context, "vkGetDeviceImageSparseMemoryRequirementsKHR");
#endif /* defined(VK_KHR_maintenance4) */
#if defined(VK_KHR_maintenance5)
	table->vkCmdBindIndexBuffer2KHR_volkImpl = (PFN_vkCmdBindIndexBuffer2KHR)load(context, "vkCmdBindIndexBuffer2KHR");
	table->vkGetDeviceImageSubresourceLayoutKHR_volkImpl = (PFN_vkGetDeviceImageSubresourceLayoutKHR)load(context, "vkGetDeviceImageSubresourceLayoutKHR");
	table->vkGetImageSubresourceLayout2KHR_volkImpl = (PFN_vkGetImageSubresourceLayout2KHR)load(context, "vkGetImageSubresourceLayout2KHR");
	table->vkGetRenderingAreaGranularityKHR_volkImpl = (PFN_vkGetRenderingAreaGranularityKHR)load(context, "vkGetRenderingAreaGranularityKHR");
#endif /* defined(VK_KHR_maintenance5) */
#if defined(VK_KHR_maintenance6)
	table->vkCmdBindDescriptorSets2KHR_volkImpl = (PFN_vkCmdBindDescriptorSets2KHR)load(context, "vkCmdBindDescriptorSets2KHR");
	table->vkCmdPushConstants2KHR_volkImpl = (PFN_vkCmdPushConstants2KHR)load(context, "vkCmdPushConstants2KHR");
#endif /* defined(VK_KHR_maintenance6) */
#if defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor)
	table->vkCmdPushDescriptorSet2KHR_volkImpl = (PFN_vkCmdPushDescriptorSet2KHR)load(context, "vkCmdPushDescriptorSet2KHR");
	table->vkCmdPushDescriptorSetWithTemplate2KHR_volkImpl = (PFN_vkCmdPushDescriptorSetWithTemplate2KHR)load(context, "vkCmdPushDescriptorSetWithTemplate2KHR");
#endif /* defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer)
	table->vkCmdBindDescriptorBufferEmbeddedSamplers2EXT_volkImpl = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT)load(context, "vkCmdBindDescriptorBufferEmbeddedSamplers2EXT");
	table->vkCmdSetDescriptorBufferOffsets2EXT_volkImpl = (PFN_vkCmdSetDescriptorBufferOffsets2EXT)load(context, "vkCmdSetDescriptorBufferOffsets2EXT");
#endif /* defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer) */
#if defined(VK_KHR_map_memory2)
	table->vkMapMemory2KHR_volkImpl = (PFN_vkMapMemory2KHR)load(context, "vkMapMemory2KHR");
	table->vkUnmapMemory2KHR_volkImpl = (PFN_vkUnmapMemory2KHR)load(context, "vkUnmapMemory2KHR");
#endif /* defined(VK_KHR_map_memory2) */
#if defined(VK_KHR_performance_query)
	table->vkAcquireProfilingLockKHR_volkImpl = (PFN_vkAcquireProfilingLockKHR)load(context, "vkAcquireProfilingLockKHR");
	table->vkReleaseProfilingLockKHR_volkImpl = (PFN_vkReleaseProfilingLockKHR)load(context, "vkReleaseProfilingLockKHR");
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_pipeline_executable_properties)
	table->vkGetPipelineExecutableInternalRepresentationsKHR_volkImpl = (PFN_vkGetPipelineExecutableInternalRepresentationsKHR)load(context, "vkGetPipelineExecutableInternalRepresentationsKHR");
	table->vkGetPipelineExecutablePropertiesKHR_volkImpl = (PFN_vkGetPipelineExecutablePropertiesKHR)load(context, "vkGetPipelineExecutablePropertiesKHR");
	table->vkGetPipelineExecutableStatisticsKHR_volkImpl = (PFN_vkGetPipelineExecutableStatisticsKHR)load(context, "vkGetPipelineExecutableStatisticsKHR");
#endif /* defined(VK_KHR_pipeline_executable_properties) */
#if defined(VK_KHR_present_wait)
	table->vkWaitForPresentKHR_volkImpl = (PFN_vkWaitForPresentKHR)load(context, "vkWaitForPresentKHR");
#endif /* defined(VK_KHR_present_wait) */
#if defined(VK_KHR_push_descriptor)
	table->vkCmdPushDescriptorSetKHR_volkImpl = (PFN_vkCmdPushDescriptorSetKHR)load(context, "vkCmdPushDescriptorSetKHR");
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline)
	table->vkCmdTraceRaysIndirect2KHR_volkImpl = (PFN_vkCmdTraceRaysIndirect2KHR)load(context, "vkCmdTraceRaysIndirect2KHR");
#endif /* defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_ray_tracing_pipeline)
	table->vkCmdSetRayTracingPipelineStackSizeKHR_volkImpl = (PFN_vkCmdSetRayTracingPipelineStackSizeKHR)load(context, "vkCmdSetRayTracingPipelineStackSizeKHR");
	table->vkCmdTraceRaysIndirectKHR_volkImpl = (PFN_vkCmdTraceRaysIndirectKHR)load(context, "vkCmdTraceRaysIndirectKHR");
	table->vkCmdTraceRaysKHR_volkImpl = (PFN_vkCmdTraceRaysKHR)load(context, "vkCmdTraceRaysKHR");
	table->vkCreateRayTracingPipelinesKHR_volkImpl = (PFN_vkCreateRayTracingPipelinesKHR)load(context, "vkCreateRayTracingPipelinesKHR");
	table->vkGetRayTracingCaptureReplayShaderGroupHandlesKHR_volkImpl = (PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR)load(context, "vkGetRayTracingCaptureReplayShaderGroupHandlesKHR");
	table->vkGetRayTracingShaderGroupHandlesKHR_volkImpl = (PFN_vkGetRayTracingShaderGroupHandlesKHR)load(context, "vkGetRayTracingShaderGroupHandlesKHR");
	table->vkGetRayTracingShaderGroupStackSizeKHR_volkImpl = (PFN_vkGetRayTracingShaderGroupStackSizeKHR)load(context, "vkGetRayTracingShaderGroupStackSizeKHR");
#endif /* defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
	table->vkCreateSamplerYcbcrConversionKHR_volkImpl = (PFN_vkCreateSamplerYcbcrConversionKHR)load(context, "vkCreateSamplerYcbcrConversionKHR");
	table->vkDestroySamplerYcbcrConversionKHR_volkImpl = (PFN_vkDestroySamplerYcbcrConversionKHR)load(context, "vkDestroySamplerYcbcrConversionKHR");
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
	table->vkGetSwapchainStatusKHR_volkImpl = (PFN_vkGetSwapchainStatusKHR)load(context, "vkGetSwapchainStatusKHR");
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_swapchain)
	table->vkAcquireNextImageKHR_volkImpl = (PFN_vkAcquireNextImageKHR)load(context, "vkAcquireNextImageKHR");
	table->vkCreateSwapchainKHR_volkImpl = (PFN_vkCreateSwapchainKHR)load(context, "vkCreateSwapchainKHR");
	table->vkDestroySwapchainKHR_volkImpl = (PFN_vkDestroySwapchainKHR)load(context, "vkDestroySwapchainKHR");
	table->vkGetSwapchainImagesKHR_volkImpl = (PFN_vkGetSwapchainImagesKHR)load(context, "vkGetSwapchainImagesKHR");
	table->vkQueuePresentKHR_volkImpl = (PFN_vkQueuePresentKHR)load(context, "vkQueuePresentKHR");
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_synchronization2)
	table->vkCmdPipelineBarrier2KHR_volkImpl = (PFN_vkCmdPipelineBarrier2KHR)load(context, "vkCmdPipelineBarrier2KHR");
	table->vkCmdResetEvent2KHR_volkImpl = (PFN_vkCmdResetEvent2KHR)load(context, "vkCmdResetEvent2KHR");
	table->vkCmdSetEvent2KHR_volkImpl = (PFN_vkCmdSetEvent2KHR)load(context, "vkCmdSetEvent2KHR");
	table->vkCmdWaitEvents2KHR_volkImpl = (PFN_vkCmdWaitEvents2KHR)load(context, "vkCmdWaitEvents2KHR");
	table->vkCmdWriteTimestamp2KHR_volkImpl = (PFN_vkCmdWriteTimestamp2KHR)load(context, "vkCmdWriteTimestamp2KHR");
	table->vkQueueSubmit2KHR_volkImpl = (PFN_vkQueueSubmit2KHR)load(context, "vkQueueSubmit2KHR");
#endif /* defined(VK_KHR_synchronization2) */
#if defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker)
	table->vkCmdWriteBufferMarker2AMD_volkImpl = (PFN_vkCmdWriteBufferMarker2AMD)load(context, "vkCmdWriteBufferMarker2AMD");
#endif /* defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker) */
#if defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints)
	table->vkGetQueueCheckpointData2NV_volkImpl = (PFN_vkGetQueueCheckpointData2NV)load(context, "vkGetQueueCheckpointData2NV");
#endif /* defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_KHR_timeline_semaphore)
	table->vkGetSemaphoreCounterValueKHR_volkImpl = (PFN_vkGetSemaphoreCounterValueKHR)load(context, "vkGetSemaphoreCounterValueKHR");
	table->vkSignalSemaphoreKHR_volkImpl = (PFN_vkSignalSemaphoreKHR)load(context, "vkSignalSemaphoreKHR");
	table->vkWaitSemaphoresKHR_volkImpl = (PFN_vkWaitSemaphoresKHR)load(context, "vkWaitSemaphoresKHR");
#endif /* defined(VK_KHR_timeline_semaphore) */
#if defined(VK_KHR_video_decode_queue)
	table->vkCmdDecodeVideoKHR_volkImpl = (PFN_vkCmdDecodeVideoKHR)load(context, "vkCmdDecodeVideoKHR");
#endif /* defined(VK_KHR_video_decode_queue) */
#if defined(VK_KHR_video_encode_queue)
	table->vkCmdEncodeVideoKHR_volkImpl = (PFN_vkCmdEncodeVideoKHR)load(context, "vkCmdEncodeVideoKHR");
	table->vkGetEncodedVideoSessionParametersKHR_volkImpl = (PFN_vkGetEncodedVideoSessionParametersKHR)load(context, "vkGetEncodedVideoSessionParametersKHR");
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
	table->vkBindVideoSessionMemoryKHR_volkImpl = (PFN_vkBindVideoSessionMemoryKHR)load(context, "vkBindVideoSessionMemoryKHR");
	table->vkCmdBeginVideoCodingKHR_volkImpl = (PFN_vkCmdBeginVideoCodingKHR)load(context, "vkCmdBeginVideoCodingKHR");
	table->vkCmdControlVideoCodingKHR_volkImpl = (PFN_vkCmdControlVideoCodingKHR)load(context, "vkCmdControlVideoCodingKHR");
	table->vkCmdEndVideoCodingKHR_volkImpl = (PFN_vkCmdEndVideoCodingKHR)load(context, "vkCmdEndVideoCodingKHR");
	table->vkCreateVideoSessionKHR_volkImpl = (PFN_vkCreateVideoSessionKHR)load(context, "vkCreateVideoSessionKHR");
	table->vkCreateVideoSessionParametersKHR_volkImpl = (PFN_vkCreateVideoSessionParametersKHR)load(context, "vkCreateVideoSessionParametersKHR");
	table->vkDestroyVideoSessionKHR_volkImpl = (PFN_vkDestroyVideoSessionKHR)load(context, "vkDestroyVideoSessionKHR");
	table->vkDestroyVideoSessionParametersKHR_volkImpl = (PFN_vkDestroyVideoSessionParametersKHR)load(context, "vkDestroyVideoSessionParametersKHR");
	table->vkGetVideoSessionMemoryRequirementsKHR_volkImpl = (PFN_vkGetVideoSessionMemoryRequirementsKHR)load(context, "vkGetVideoSessionMemoryRequirementsKHR");
	table->vkUpdateVideoSessionParametersKHR_volkImpl = (PFN_vkUpdateVideoSessionParametersKHR)load(context, "vkUpdateVideoSessionParametersKHR");
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_NVX_binary_import)
	table->vkCmdCuLaunchKernelNVX_volkImpl = (PFN_vkCmdCuLaunchKernelNVX)load(context, "vkCmdCuLaunchKernelNVX");
	table->vkCreateCuFunctionNVX_volkImpl = (PFN_vkCreateCuFunctionNVX)load(context, "vkCreateCuFunctionNVX");
	table->vkCreateCuModuleNVX_volkImpl = (PFN_vkCreateCuModuleNVX)load(context, "vkCreateCuModuleNVX");
	table->vkDestroyCuFunctionNVX_volkImpl = (PFN_vkDestroyCuFunctionNVX)load(context, "vkDestroyCuFunctionNVX");
	table->vkDestroyCuModuleNVX_volkImpl = (PFN_vkDestroyCuModuleNVX)load(context, "vkDestroyCuModuleNVX");
#endif /* defined(VK_NVX_binary_import) */
#if defined(VK_NVX_image_view_handle)
	table->vkGetImageViewAddressNVX_volkImpl = (PFN_vkGetImageViewAddressNVX)load(context, "vkGetImageViewAddressNVX");
	table->vkGetImageViewHandleNVX_volkImpl = (PFN_vkGetImageViewHandleNVX)load(context, "vkGetImageViewHandleNVX");
#endif /* defined(VK_NVX_image_view_handle) */
#if defined(VK_NV_clip_space_w_scaling)
	table->vkCmdSetViewportWScalingNV_volkImpl = (PFN_vkCmdSetViewportWScalingNV)load(context, "vkCmdSetViewportWScalingNV");
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_copy_memory_indirect)
	table->vkCmdCopyMemoryIndirectNV_volkImpl = (PFN_vkCmdCopyMemoryIndirectNV)load(context, "vkCmdCopyMemoryIndirectNV");
	table->vkCmdCopyMemoryToImageIndirectNV_volkImpl = (PFN_vkCmdCopyMemoryToImageIndirectNV)load(context, "vkCmdCopyMemoryToImageIndirectNV");
#endif /* defined(VK_NV_copy_memory_indirect) */
#if defined(VK_NV_cuda_kernel_launch)
	table->vkCmdCudaLaunchKernelNV_volkImpl = (PFN_vkCmdCudaLaunchKernelNV)load(context, "vkCmdCudaLaunchKernelNV");
	table->vkCreateCudaFunctionNV_volkImpl = (PFN_vkCreateCudaFunctionNV)load(context, "vkCreateCudaFunctionNV");
	table->vkCreateCudaModuleNV_volkImpl = (PFN_vkCreateCudaModuleNV)load(context, "vkCreateCudaModuleNV");
	table->vkDestroyCudaFunctionNV_volkImpl = (PFN_vkDestroyCudaFunctionNV)load(context, "vkDestroyCudaFunctionNV");
	table->vkDestroyCudaModuleNV_volkImpl = (PFN_vkDestroyCudaModuleNV)load(context, "vkDestroyCudaModuleNV");
	table->vkGetCudaModuleCacheNV_volkImpl = (PFN_vkGetCudaModuleCacheNV)load(context, "vkGetCudaModuleCacheNV");
#endif /* defined(VK_NV_cuda_kernel_launch) */
#if defined(VK_NV_device_diagnostic_checkpoints)
	table->vkCmdSetCheckpointNV_volkImpl = (PFN_vkCmdSetCheckpointNV)load(context, "vkCmdSetCheckpointNV");
	table->vkGetQueueCheckpointDataNV_volkImpl = (PFN_vkGetQueueCheckpointDataNV)load(context, "vkGetQueueCheckpointDataNV");
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_device_generated_commands)
	table->vkCmdBindPipelineShaderGroupNV_volkImpl = (PFN_vkCmdBindPipelineShaderGroupNV)load(context, "vkCmdBindPipelineShaderGroupNV");
	table->vkCmdExecuteGeneratedCommandsNV_volkImpl = (PFN_vkCmdExecuteGeneratedCommandsNV)load(context, "vkCmdExecuteGeneratedCommandsNV");
	table->vkCmdPreprocessGeneratedCommandsNV_volkImpl = (PFN_vkCmdPreprocessGeneratedCommandsNV)load(context, "vkCmdPreprocessGeneratedCommandsNV");
	table->vkCreateIndirectCommandsLayoutNV_volkImpl = (PFN_vkCreateIndirectCommandsLayoutNV)load(context, "vkCreateIndirectCommandsLayoutNV");
	table->vkDestroyIndirectCommandsLayoutNV_volkImpl = (PFN_vkDestroyIndirectCommandsLayoutNV)load(context, "vkDestroyIndirectCommandsLayoutNV");
	table->vkGetGeneratedCommandsMemoryRequirementsNV_volkImpl = (PFN_vkGetGeneratedCommandsMemoryRequirementsNV)load(context, "vkGetGeneratedCommandsMemoryRequirementsNV");
#endif /* defined(VK_NV_device_generated_commands) */
#if defined(VK_NV_device_generated_commands_compute)
	table->vkCmdUpdatePipelineIndirectBufferNV_volkImpl = (PFN_vkCmdUpdatePipelineIndirectBufferNV)load(context, "vkCmdUpdatePipelineIndirectBufferNV");
	table->vkGetPipelineIndirectDeviceAddressNV_volkImpl = (PFN_vkGetPipelineIndirectDeviceAddressNV)load(context, "vkGetPipelineIndirectDeviceAddressNV");
	table->vkGetPipelineIndirectMemoryRequirementsNV_volkImpl = (PFN_vkGetPipelineIndirectMemoryRequirementsNV)load(context, "vkGetPipelineIndirectMemoryRequirementsNV");
#endif /* defined(VK_NV_device_generated_commands_compute) */
#if defined(VK_NV_external_memory_rdma)
	table->vkGetMemoryRemoteAddressNV_volkImpl = (PFN_vkGetMemoryRemoteAddressNV)load(context, "vkGetMemoryRemoteAddressNV");
#endif /* defined(VK_NV_external_memory_rdma) */
#if defined(VK_NV_external_memory_win32)
	table->vkGetMemoryWin32HandleNV_volkImpl = (PFN_vkGetMemoryWin32HandleNV)load(context, "vkGetMemoryWin32HandleNV");
#endif /* defined(VK_NV_external_memory_win32) */
#if defined(VK_NV_fragment_shading_rate_enums)
	table->vkCmdSetFragmentShadingRateEnumNV_volkImpl = (PFN_vkCmdSetFragmentShadingRateEnumNV)load(context, "vkCmdSetFragmentShadingRateEnumNV");
#endif /* defined(VK_NV_fragment_shading_rate_enums) */
#if defined(VK_NV_low_latency2)
	table->vkGetLatencyTimingsNV_volkImpl = (PFN_vkGetLatencyTimingsNV)load(context, "vkGetLatencyTimingsNV");
	table->vkLatencySleepNV_volkImpl = (PFN_vkLatencySleepNV)load(context, "vkLatencySleepNV");
	table->vkQueueNotifyOutOfBandNV_volkImpl = (PFN_vkQueueNotifyOutOfBandNV)load(context, "vkQueueNotifyOutOfBandNV");
	table->vkSetLatencyMarkerNV_volkImpl = (PFN_vkSetLatencyMarkerNV)load(context, "vkSetLatencyMarkerNV");
	table->vkSetLatencySleepModeNV_volkImpl = (PFN_vkSetLatencySleepModeNV)load(context, "vkSetLatencySleepModeNV");
#endif /* defined(VK_NV_low_latency2) */
#if defined(VK_NV_memory_decompression)
	table->vkCmdDecompressMemoryIndirectCountNV_volkImpl = (PFN_vkCmdDecompressMemoryIndirectCountNV)load(context, "vkCmdDecompressMemoryIndirectCountNV");
	table->vkCmdDecompressMemoryNV_volkImpl = (PFN_vkCmdDecompressMemoryNV)load(context, "vkCmdDecompressMemoryNV");
#endif /* defined(VK_NV_memory_decompression) */
#if defined(VK_NV_mesh_shader)
	table->vkCmdDrawMeshTasksIndirectCountNV_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectCountNV)load(context, "vkCmdDrawMeshTasksIndirectCountNV");
	table->vkCmdDrawMeshTasksIndirectNV_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectNV)load(context, "vkCmdDrawMeshTasksIndirectNV");
	table->vkCmdDrawMeshTasksNV_volkImpl = (PFN_vkCmdDrawMeshTasksNV)load(context, "vkCmdDrawMeshTasksNV");
#endif /* defined(VK_NV_mesh_shader) */
#if defined(VK_NV_optical_flow)
	table->vkBindOpticalFlowSessionImageNV_volkImpl = (PFN_vkBindOpticalFlowSessionImageNV)load(context, "vkBindOpticalFlowSessionImageNV");
	table->vkCmdOpticalFlowExecuteNV_volkImpl = (PFN_vkCmdOpticalFlowExecuteNV)load(context, "vkCmdOpticalFlowExecuteNV");
	table->vkCreateOpticalFlowSessionNV_volkImpl = (PFN_vkCreateOpticalFlowSessionNV)load(context, "vkCreateOpticalFlowSessionNV");
	table->vkDestroyOpticalFlowSessionNV_volkImpl = (PFN_vkDestroyOpticalFlowSessionNV)load(context, "vkDestroyOpticalFlowSessionNV");
#endif /* defined(VK_NV_optical_flow) */
#if defined(VK_NV_ray_tracing)
	table->vkBindAccelerationStructureMemoryNV_volkImpl = (PFN_vkBindAccelerationStructureMemoryNV)load(context, "vkBindAccelerationStructureMemoryNV");
	table->vkCmdBuildAccelerationStructureNV_volkImpl = (PFN_vkCmdBuildAccelerationStructureNV)load(context, "vkCmdBuildAccelerationStructureNV");
	table->vkCmdCopyAccelerationStructureNV_volkImpl = (PFN_vkCmdCopyAccelerationStructureNV)load(context, "vkCmdCopyAccelerationStructureNV");
	table->vkCmdTraceRaysNV_volkImpl = (PFN_vkCmdTraceRaysNV)load(context, "vkCmdTraceRaysNV");
	table->vkCmdWriteAccelerationStructuresPropertiesNV_volkImpl = (PFN_vkCmdWriteAccelerationStructuresPropertiesNV)load(context, "vkCmdWriteAccelerationStructuresPropertiesNV");
	table->vkCompileDeferredNV_volkImpl = (PFN_vkCompileDeferredNV)load(context, "vkCompileDeferredNV");
	table->vkCreateAccelerationStructureNV_volkImpl = (PFN_vkCreateAccelerationStructureNV)load(context, "vkCreateAccelerationStructureNV");
	table->vkCreateRayTracingPipelinesNV_volkImpl = (PFN_vkCreateRayTracingPipelinesNV)load(context, "vkCreateRayTracingPipelinesNV");
	table->vkDestroyAccelerationStructureNV_volkImpl = (PFN_vkDestroyAccelerationStructureNV)load(context, "vkDestroyAccelerationStructureNV");
	table->vkGetAccelerationStructureHandleNV_volkImpl = (PFN_vkGetAccelerationStructureHandleNV)load(context, "vkGetAccelerationStructureHandleNV");
	table->vkGetAccelerationStructureMemoryRequirementsNV_volkImpl = (PFN_vkGetAccelerationStructureMemoryRequirementsNV)load(context, "vkGetAccelerationStructureMemoryRequirementsNV");
	table->vkGetRayTracingShaderGroupHandlesNV_volkImpl = (PFN_vkGetRayTracingShaderGroupHandlesNV)load(context, "vkGetRayTracingShaderGroupHandlesNV");
#endif /* defined(VK_NV_ray_tracing) */
#if defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2
	table->vkCmdSetExclusiveScissorEnableNV_volkImpl = (PFN_vkCmdSetExclusiveScissorEnableNV)load(context, "vkCmdSetExclusiveScissorEnableNV");
#endif /* defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2 */
#if defined(VK_NV_scissor_exclusive)
	table->vkCmdSetExclusiveScissorNV_volkImpl = (PFN_vkCmdSetExclusiveScissorNV)load(context, "vkCmdSetExclusiveScissorNV");
#endif /* defined(VK_NV_scissor_exclusive) */
#if defined(VK_NV_shading_rate_image)
	table->vkCmdBindShadingRateImageNV_volkImpl = (PFN_vkCmdBindShadingRateImageNV)load(context, "vkCmdBindShadingRateImageNV");
	table->vkCmdSetCoarseSampleOrderNV_volkImpl = (PFN_vkCmdSetCoarseSampleOrderNV)load(context, "vkCmdSetCoarseSampleOrderNV");
	table->vkCmdSetViewportShadingRatePaletteNV_volkImpl = (PFN_vkCmdSetViewportShadingRatePaletteNV)load(context, "vkCmdSetViewportShadingRatePaletteNV");
#endif /* defined(VK_NV_shading_rate_image) */
#if defined(VK_QCOM_tile_properties)
	table->vkGetDynamicRenderingTilePropertiesQCOM_volkImpl = (PFN_vkGetDynamicRenderingTilePropertiesQCOM)load(context, "vkGetDynamicRenderingTilePropertiesQCOM");
	table->vkGetFramebufferTilePropertiesQCOM_volkImpl = (PFN_vkGetFramebufferTilePropertiesQCOM)load(context, "vkGetFramebufferTilePropertiesQCOM");
#endif /* defined(VK_QCOM_tile_properties) */
#if defined(VK_QNX_external_memory_screen_buffer)
	table->vkGetScreenBufferPropertiesQNX_volkImpl = (PFN_vkGetScreenBufferPropertiesQNX)load(context, "vkGetScreenBufferPropertiesQNX");
#endif /* defined(VK_QNX_external_memory_screen_buffer) */
#if defined(VK_VALVE_descriptor_set_host_mapping)
	table->vkGetDescriptorSetHostMappingVALVE_volkImpl = (PFN_vkGetDescriptorSetHostMappingVALVE)load(context, "vkGetDescriptorSetHostMappingVALVE");
	table->vkGetDescriptorSetLayoutHostMappingInfoVALVE_volkImpl = (PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE)load(context, "vkGetDescriptorSetLayoutHostMappingInfoVALVE");
#endif /* defined(VK_VALVE_descriptor_set_host_mapping) */
#if (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object))
	table->vkCmdBindVertexBuffers2EXT_volkImpl = (PFN_vkCmdBindVertexBuffers2EXT)load(context, "vkCmdBindVertexBuffers2EXT");
	table->vkCmdSetCullModeEXT_volkImpl = (PFN_vkCmdSetCullModeEXT)load(context, "vkCmdSetCullModeEXT");
	table->vkCmdSetDepthBoundsTestEnableEXT_volkImpl = (PFN_vkCmdSetDepthBoundsTestEnableEXT)load(context, "vkCmdSetDepthBoundsTestEnableEXT");
	table->vkCmdSetDepthCompareOpEXT_volkImpl = (PFN_vkCmdSetDepthCompareOpEXT)load(context, "vkCmdSetDepthCompareOpEXT");
	table->vkCmdSetDepthTestEnableEXT_volkImpl = (PFN_vkCmdSetDepthTestEnableEXT)load(context, "vkCmdSetDepthTestEnableEXT");
	table->vkCmdSetDepthWriteEnableEXT_volkImpl = (PFN_vkCmdSetDepthWriteEnableEXT)load(context, "vkCmdSetDepthWriteEnableEXT");
	table->vkCmdSetFrontFaceEXT_volkImpl = (PFN_vkCmdSetFrontFaceEXT)load(context, "vkCmdSetFrontFaceEXT");
	table->vkCmdSetPrimitiveTopologyEXT_volkImpl = (PFN_vkCmdSetPrimitiveTopologyEXT)load(context, "vkCmdSetPrimitiveTopologyEXT");
	table->vkCmdSetScissorWithCountEXT_volkImpl = (PFN_vkCmdSetScissorWithCountEXT)load(context, "vkCmdSetScissorWithCountEXT");
	table->vkCmdSetStencilOpEXT_volkImpl = (PFN_vkCmdSetStencilOpEXT)load(context, "vkCmdSetStencilOpEXT");
	table->vkCmdSetStencilTestEnableEXT_volkImpl = (PFN_vkCmdSetStencilTestEnableEXT)load(context, "vkCmdSetStencilTestEnableEXT");
	table->vkCmdSetViewportWithCountEXT_volkImpl = (PFN_vkCmdSetViewportWithCountEXT)load(context, "vkCmdSetViewportWithCountEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object))
	table->vkCmdSetDepthBiasEnableEXT_volkImpl = (PFN_vkCmdSetDepthBiasEnableEXT)load(context, "vkCmdSetDepthBiasEnableEXT");
	table->vkCmdSetLogicOpEXT_volkImpl = (PFN_vkCmdSetLogicOpEXT)load(context, "vkCmdSetLogicOpEXT");
	table->vkCmdSetPatchControlPointsEXT_volkImpl = (PFN_vkCmdSetPatchControlPointsEXT)load(context, "vkCmdSetPatchControlPointsEXT");
	table->vkCmdSetPrimitiveRestartEnableEXT_volkImpl = (PFN_vkCmdSetPrimitiveRestartEnableEXT)load(context, "vkCmdSetPrimitiveRestartEnableEXT");
	table->vkCmdSetRasterizerDiscardEnableEXT_volkImpl = (PFN_vkCmdSetRasterizerDiscardEnableEXT)load(context, "vkCmdSetRasterizerDiscardEnableEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object))
	table->vkCmdSetAlphaToCoverageEnableEXT_volkImpl = (PFN_vkCmdSetAlphaToCoverageEnableEXT)load(context, "vkCmdSetAlphaToCoverageEnableEXT");
	table->vkCmdSetAlphaToOneEnableEXT_volkImpl = (PFN_vkCmdSetAlphaToOneEnableEXT)load(context, "vkCmdSetAlphaToOneEnableEXT");
	table->vkCmdSetColorBlendEnableEXT_volkImpl = (PFN_vkCmdSetColorBlendEnableEXT)load(context, "vkCmdSetColorBlendEnableEXT");
	table->vkCmdSetColorBlendEquationEXT_volkImpl = (PFN_vkCmdSetColorBlendEquationEXT)load(context, "vkCmdSetColorBlendEquationEXT");
	table->vkCmdSetColorWriteMaskEXT_volkImpl = (PFN_vkCmdSetColorWriteMaskEXT)load(context, "vkCmdSetColorWriteMaskEXT");
	table->vkCmdSetDepthClampEnableEXT_volkImpl = (PFN_vkCmdSetDepthClampEnableEXT)load(context, "vkCmdSetDepthClampEnableEXT");
	table->vkCmdSetLogicOpEnableEXT_volkImpl = (PFN_vkCmdSetLogicOpEnableEXT)load(context, "vkCmdSetLogicOpEnableEXT");
	table->vkCmdSetPolygonModeEXT_volkImpl = (PFN_vkCmdSetPolygonModeEXT)load(context, "vkCmdSetPolygonModeEXT");
	table->vkCmdSetRasterizationSamplesEXT_volkImpl = (PFN_vkCmdSetRasterizationSamplesEXT)load(context, "vkCmdSetRasterizationSamplesEXT");
	table->vkCmdSetSampleMaskEXT_volkImpl = (PFN_vkCmdSetSampleMaskEXT)load(context, "vkCmdSetSampleMaskEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object))
	table->vkCmdSetTessellationDomainOriginEXT_volkImpl = (PFN_vkCmdSetTessellationDomainOriginEXT)load(context, "vkCmdSetTessellationDomainOriginEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback))
	table->vkCmdSetRasterizationStreamEXT_volkImpl = (PFN_vkCmdSetRasterizationStreamEXT)load(context, "vkCmdSetRasterizationStreamEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization))
	table->vkCmdSetConservativeRasterizationModeEXT_volkImpl = (PFN_vkCmdSetConservativeRasterizationModeEXT)load(context, "vkCmdSetConservativeRasterizationModeEXT");
	table->vkCmdSetExtraPrimitiveOverestimationSizeEXT_volkImpl = (PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT)load(context, "vkCmdSetExtraPrimitiveOverestimationSizeEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable))
	table->vkCmdSetDepthClipEnableEXT_volkImpl = (PFN_vkCmdSetDepthClipEnableEXT)load(context, "vkCmdSetDepthClipEnableEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations))
	table->vkCmdSetSampleLocationsEnableEXT_volkImpl = (PFN_vkCmdSetSampleLocationsEnableEXT)load(context, "vkCmdSetSampleLocationsEnableEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced))
	table->vkCmdSetColorBlendAdvancedEXT_volkImpl = (PFN_vkCmdSetColorBlendAdvancedEXT)load(context, "vkCmdSetColorBlendAdvancedEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex))
	table->vkCmdSetProvokingVertexModeEXT_volkImpl = (PFN_vkCmdSetProvokingVertexModeEXT)load(context, "vkCmdSetProvokingVertexModeEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization))
	table->vkCmdSetLineRasterizationModeEXT_volkImpl = (PFN_vkCmdSetLineRasterizationModeEXT)load(context, "vkCmdSetLineRasterizationModeEXT");
	table->vkCmdSetLineStippleEnableEXT_volkImpl = (PFN_vkCmdSetLineStippleEnableEXT)load(context, "vkCmdSetLineStippleEnableEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control))
	table->vkCmdSetDepthClipNegativeOneToOneEXT_volkImpl = (PFN_vkCmdSetDepthClipNegativeOneToOneEXT)load(context, "vkCmdSetDepthClipNegativeOneToOneEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling))
	table->vkCmdSetViewportWScalingEnableNV_volkImpl = (PFN_vkCmdSetViewportWScalingEnableNV)load(context, "vkCmdSetViewportWScalingEnableNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle))
	table->vkCmdSetViewportSwizzleNV_volkImpl = (PFN_vkCmdSetViewportSwizzleNV)load(context, "vkCmdSetViewportSwizzleNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color))
	table->vkCmdSetCoverageToColorEnableNV_volkImpl = (PFN_vkCmdSetCoverageToColorEnableNV)load(context, "vkCmdSetCoverageToColorEnableNV");
	table->vkCmdSetCoverageToColorLocationNV_volkImpl = (PFN_vkCmdSetCoverageToColorLocationNV)load(context, "vkCmdSetCoverageToColorLocationNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples))
	table->vkCmdSetCoverageModulationModeNV_volkImpl = (PFN_vkCmdSetCoverageModulationModeNV)load(context, "vkCmdSetCoverageModulationModeNV");
	table->vkCmdSetCoverageModulationTableEnableNV_volkImpl = (PFN_vkCmdSetCoverageModulationTableEnableNV)load(context, "vkCmdSetCoverageModulationTableEnableNV");
	table->vkCmdSetCoverageModulationTableNV_volkImpl = (PFN_vkCmdSetCoverageModulationTableNV)load(context, "vkCmdSetCoverageModulationTableNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image))
	table->vkCmdSetShadingRateImageEnableNV_volkImpl = (PFN_vkCmdSetShadingRateImageEnableNV)load(context, "vkCmdSetShadingRateImageEnableNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test))
	table->vkCmdSetRepresentativeFragmentTestEnableNV_volkImpl = (PFN_vkCmdSetRepresentativeFragmentTestEnableNV)load(context, "vkCmdSetRepresentativeFragmentTestEnableNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode))
	table->vkCmdSetCoverageReductionModeNV_volkImpl = (PFN_vkCmdSetCoverageReductionModeNV)load(context, "vkCmdSetCoverageReductionModeNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode)) */
#if (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1))
	table->vkGetDeviceGroupSurfacePresentModes2EXT_volkImpl = (PFN_vkGetDeviceGroupSurfacePresentModes2EXT)load(context, "vkGetDeviceGroupSurfacePresentModes2EXT");
#endif /* (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1)) */
#if (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control))
	table->vkGetImageSubresourceLayout2EXT_volkImpl = (PFN_vkGetImageSubresourceLayout2EXT)load(context, "vkGetImageSubresourceLayout2EXT");
#endif /* (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control)) */
#if (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state))
	table->vkCmdSetVertexInputEXT_volkImpl = (PFN_vkCmdSetVertexInputEXT)load(context, "vkCmdSetVertexInputEXT");
#endif /* (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state)) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template))
	table->vkCmdPushDescriptorSetWithTemplateKHR_volkImpl = (PFN_vkCmdPushDescriptorSetWithTemplateKHR)load(context, "vkCmdPushDescriptorSetWithTemplateKHR");
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	table->vkGetDeviceGroupPresentCapabilitiesKHR_volkImpl = (PFN_vkGetDeviceGroupPresentCapabilitiesKHR)load(context, "vkGetDeviceGroupPresentCapabilitiesKHR");
	table->vkGetDeviceGroupSurfacePresentModesKHR_volkImpl = (PFN_vkGetDeviceGroupSurfacePresentModesKHR)load(context, "vkGetDeviceGroupSurfacePresentModesKHR");
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	table->vkAcquireNextImage2KHR_volkImpl = (PFN_vkAcquireNextImage2KHR)load(context, "vkAcquireNextImage2KHR");
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
	/* VOLK_GENERATE_LOAD_DEVICE_TABLE */
}

#ifdef __GNUC__
#ifdef VOLK_DEFAULT_VISIBILITY
#	pragma GCC visibility push(default)
#else
#	pragma GCC visibility push(hidden)
#endif
#endif

/* VOLK_GENERATE_PROTOTYPES_C */
#if defined(VK_VERSION_1_0)
PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers_volkImpl;
PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets_volkImpl;
PFN_vkAllocateMemory vkAllocateMemory_volkImpl;
PFN_vkBeginCommandBuffer vkBeginCommandBuffer_volkImpl;
PFN_vkBindBufferMemory vkBindBufferMemory_volkImpl;
PFN_vkBindImageMemory vkBindImageMemory_volkImpl;
PFN_vkCmdBeginQuery vkCmdBeginQuery_volkImpl;
PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass_volkImpl;
PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets_volkImpl;
PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer_volkImpl;
PFN_vkCmdBindPipeline vkCmdBindPipeline_volkImpl;
PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers_volkImpl;
PFN_vkCmdBlitImage vkCmdBlitImage_volkImpl;
PFN_vkCmdClearAttachments vkCmdClearAttachments_volkImpl;
PFN_vkCmdClearColorImage vkCmdClearColorImage_volkImpl;
PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage_volkImpl;
PFN_vkCmdCopyBuffer vkCmdCopyBuffer_volkImpl;
PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage_volkImpl;
PFN_vkCmdCopyImage vkCmdCopyImage_volkImpl;
PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer_volkImpl;
PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults_volkImpl;
PFN_vkCmdDispatch vkCmdDispatch_volkImpl;
PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect_volkImpl;
PFN_vkCmdDraw vkCmdDraw_volkImpl;
PFN_vkCmdDrawIndexed vkCmdDrawIndexed_volkImpl;
PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect_volkImpl;
PFN_vkCmdDrawIndirect vkCmdDrawIndirect_volkImpl;
PFN_vkCmdEndQuery vkCmdEndQuery_volkImpl;
PFN_vkCmdEndRenderPass vkCmdEndRenderPass_volkImpl;
PFN_vkCmdExecuteCommands vkCmdExecuteCommands_volkImpl;
PFN_vkCmdFillBuffer vkCmdFillBuffer_volkImpl;
PFN_vkCmdNextSubpass vkCmdNextSubpass_volkImpl;
PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier_volkImpl;
PFN_vkCmdPushConstants vkCmdPushConstants_volkImpl;
PFN_vkCmdResetEvent vkCmdResetEvent_volkImpl;
PFN_vkCmdResetQueryPool vkCmdResetQueryPool_volkImpl;
PFN_vkCmdResolveImage vkCmdResolveImage_volkImpl;
PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants_volkImpl;
PFN_vkCmdSetDepthBias vkCmdSetDepthBias_volkImpl;
PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds_volkImpl;
PFN_vkCmdSetEvent vkCmdSetEvent_volkImpl;
PFN_vkCmdSetLineWidth vkCmdSetLineWidth_volkImpl;
PFN_vkCmdSetScissor vkCmdSetScissor_volkImpl;
PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask_volkImpl;
PFN_vkCmdSetStencilReference vkCmdSetStencilReference_volkImpl;
PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask_volkImpl;
PFN_vkCmdSetViewport vkCmdSetViewport_volkImpl;
PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer_volkImpl;
PFN_vkCmdWaitEvents vkCmdWaitEvents_volkImpl;
PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp_volkImpl;
PFN_vkCreateBuffer vkCreateBuffer_volkImpl;
PFN_vkCreateBufferView vkCreateBufferView_volkImpl;
PFN_vkCreateCommandPool vkCreateCommandPool_volkImpl;
PFN_vkCreateComputePipelines vkCreateComputePipelines_volkImpl;
PFN_vkCreateDescriptorPool vkCreateDescriptorPool_volkImpl;
PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout_volkImpl;
PFN_vkCreateDevice vkCreateDevice_volkImpl;
PFN_vkCreateEvent vkCreateEvent_volkImpl;
PFN_vkCreateFence vkCreateFence_volkImpl;
PFN_vkCreateFramebuffer vkCreateFramebuffer_volkImpl;
PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines_volkImpl;
PFN_vkCreateImage vkCreateImage_volkImpl;
PFN_vkCreateImageView vkCreateImageView_volkImpl;
PFN_vkCreateInstance vkCreateInstance_volkImpl;
PFN_vkCreatePipelineCache vkCreatePipelineCache_volkImpl;
PFN_vkCreatePipelineLayout vkCreatePipelineLayout_volkImpl;
PFN_vkCreateQueryPool vkCreateQueryPool_volkImpl;
PFN_vkCreateRenderPass vkCreateRenderPass_volkImpl;
PFN_vkCreateSampler vkCreateSampler_volkImpl;
PFN_vkCreateSemaphore vkCreateSemaphore_volkImpl;
PFN_vkCreateShaderModule vkCreateShaderModule_volkImpl;
PFN_vkDestroyBuffer vkDestroyBuffer_volkImpl;
PFN_vkDestroyBufferView vkDestroyBufferView_volkImpl;
PFN_vkDestroyCommandPool vkDestroyCommandPool_volkImpl;
PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool_volkImpl;
PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout_volkImpl;
PFN_vkDestroyDevice vkDestroyDevice_volkImpl;
PFN_vkDestroyEvent vkDestroyEvent_volkImpl;
PFN_vkDestroyFence vkDestroyFence_volkImpl;
PFN_vkDestroyFramebuffer vkDestroyFramebuffer_volkImpl;
PFN_vkDestroyImage vkDestroyImage_volkImpl;
PFN_vkDestroyImageView vkDestroyImageView_volkImpl;
PFN_vkDestroyInstance vkDestroyInstance_volkImpl;
PFN_vkDestroyPipeline vkDestroyPipeline_volkImpl;
PFN_vkDestroyPipelineCache vkDestroyPipelineCache_volkImpl;
PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout_volkImpl;
PFN_vkDestroyQueryPool vkDestroyQueryPool_volkImpl;
PFN_vkDestroyRenderPass vkDestroyRenderPass_volkImpl;
PFN_vkDestroySampler vkDestroySampler_volkImpl;
PFN_vkDestroySemaphore vkDestroySemaphore_volkImpl;
PFN_vkDestroyShaderModule vkDestroyShaderModule_volkImpl;
PFN_vkDeviceWaitIdle vkDeviceWaitIdle_volkImpl;
PFN_vkEndCommandBuffer vkEndCommandBuffer_volkImpl;
PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties_volkImpl;
PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties_volkImpl;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties_volkImpl;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties_volkImpl;
PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices_volkImpl;
PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges_volkImpl;
PFN_vkFreeCommandBuffers vkFreeCommandBuffers_volkImpl;
PFN_vkFreeDescriptorSets vkFreeDescriptorSets_volkImpl;
PFN_vkFreeMemory vkFreeMemory_volkImpl;
PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements_volkImpl;
PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment_volkImpl;
PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr_volkImpl;
PFN_vkGetDeviceQueue vkGetDeviceQueue_volkImpl;
PFN_vkGetEventStatus vkGetEventStatus_volkImpl;
PFN_vkGetFenceStatus vkGetFenceStatus_volkImpl;
PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements_volkImpl;
PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements_volkImpl;
PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout_volkImpl;
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr_volkImpl;
PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures_volkImpl;
PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties_volkImpl;
PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties_volkImpl;
PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties_volkImpl;
PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties_volkImpl;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties_volkImpl;
PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties_volkImpl;
PFN_vkGetPipelineCacheData vkGetPipelineCacheData_volkImpl;
PFN_vkGetQueryPoolResults vkGetQueryPoolResults_volkImpl;
PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity_volkImpl;
PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges_volkImpl;
PFN_vkMapMemory vkMapMemory_volkImpl;
PFN_vkMergePipelineCaches vkMergePipelineCaches_volkImpl;
PFN_vkQueueBindSparse vkQueueBindSparse_volkImpl;
PFN_vkQueueSubmit vkQueueSubmit_volkImpl;
PFN_vkQueueWaitIdle vkQueueWaitIdle_volkImpl;
PFN_vkResetCommandBuffer vkResetCommandBuffer_volkImpl;
PFN_vkResetCommandPool vkResetCommandPool_volkImpl;
PFN_vkResetDescriptorPool vkResetDescriptorPool_volkImpl;
PFN_vkResetEvent vkResetEvent_volkImpl;
PFN_vkResetFences vkResetFences_volkImpl;
PFN_vkSetEvent vkSetEvent_volkImpl;
PFN_vkUnmapMemory vkUnmapMemory_volkImpl;
PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets_volkImpl;
PFN_vkWaitForFences vkWaitForFences_volkImpl;
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
PFN_vkBindBufferMemory2 vkBindBufferMemory2_volkImpl;
PFN_vkBindImageMemory2 vkBindImageMemory2_volkImpl;
PFN_vkCmdDispatchBase vkCmdDispatchBase_volkImpl;
PFN_vkCmdSetDeviceMask vkCmdSetDeviceMask_volkImpl;
PFN_vkCreateDescriptorUpdateTemplate vkCreateDescriptorUpdateTemplate_volkImpl;
PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion_volkImpl;
PFN_vkDestroyDescriptorUpdateTemplate vkDestroyDescriptorUpdateTemplate_volkImpl;
PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion_volkImpl;
PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion_volkImpl;
PFN_vkEnumeratePhysicalDeviceGroups vkEnumeratePhysicalDeviceGroups_volkImpl;
PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2_volkImpl;
PFN_vkGetDescriptorSetLayoutSupport vkGetDescriptorSetLayoutSupport_volkImpl;
PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures_volkImpl;
PFN_vkGetDeviceQueue2 vkGetDeviceQueue2_volkImpl;
PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2_volkImpl;
PFN_vkGetImageSparseMemoryRequirements2 vkGetImageSparseMemoryRequirements2_volkImpl;
PFN_vkGetPhysicalDeviceExternalBufferProperties vkGetPhysicalDeviceExternalBufferProperties_volkImpl;
PFN_vkGetPhysicalDeviceExternalFenceProperties vkGetPhysicalDeviceExternalFenceProperties_volkImpl;
PFN_vkGetPhysicalDeviceExternalSemaphoreProperties vkGetPhysicalDeviceExternalSemaphoreProperties_volkImpl;
PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2_volkImpl;
PFN_vkGetPhysicalDeviceFormatProperties2 vkGetPhysicalDeviceFormatProperties2_volkImpl;
PFN_vkGetPhysicalDeviceImageFormatProperties2 vkGetPhysicalDeviceImageFormatProperties2_volkImpl;
PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2_volkImpl;
PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2_volkImpl;
PFN_vkGetPhysicalDeviceQueueFamilyProperties2 vkGetPhysicalDeviceQueueFamilyProperties2_volkImpl;
PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 vkGetPhysicalDeviceSparseImageFormatProperties2_volkImpl;
PFN_vkTrimCommandPool vkTrimCommandPool_volkImpl;
PFN_vkUpdateDescriptorSetWithTemplate vkUpdateDescriptorSetWithTemplate_volkImpl;
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_2)
PFN_vkCmdBeginRenderPass2 vkCmdBeginRenderPass2_volkImpl;
PFN_vkCmdDrawIndexedIndirectCount vkCmdDrawIndexedIndirectCount_volkImpl;
PFN_vkCmdDrawIndirectCount vkCmdDrawIndirectCount_volkImpl;
PFN_vkCmdEndRenderPass2 vkCmdEndRenderPass2_volkImpl;
PFN_vkCmdNextSubpass2 vkCmdNextSubpass2_volkImpl;
PFN_vkCreateRenderPass2 vkCreateRenderPass2_volkImpl;
PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress_volkImpl;
PFN_vkGetBufferOpaqueCaptureAddress vkGetBufferOpaqueCaptureAddress_volkImpl;
PFN_vkGetDeviceMemoryOpaqueCaptureAddress vkGetDeviceMemoryOpaqueCaptureAddress_volkImpl;
PFN_vkGetSemaphoreCounterValue vkGetSemaphoreCounterValue_volkImpl;
PFN_vkResetQueryPool vkResetQueryPool_volkImpl;
PFN_vkSignalSemaphore vkSignalSemaphore_volkImpl;
PFN_vkWaitSemaphores vkWaitSemaphores_volkImpl;
#endif /* defined(VK_VERSION_1_2) */
#if defined(VK_VERSION_1_3)
PFN_vkCmdBeginRendering vkCmdBeginRendering_volkImpl;
PFN_vkCmdBindVertexBuffers2 vkCmdBindVertexBuffers2_volkImpl;
PFN_vkCmdBlitImage2 vkCmdBlitImage2_volkImpl;
PFN_vkCmdCopyBuffer2 vkCmdCopyBuffer2_volkImpl;
PFN_vkCmdCopyBufferToImage2 vkCmdCopyBufferToImage2_volkImpl;
PFN_vkCmdCopyImage2 vkCmdCopyImage2_volkImpl;
PFN_vkCmdCopyImageToBuffer2 vkCmdCopyImageToBuffer2_volkImpl;
PFN_vkCmdEndRendering vkCmdEndRendering_volkImpl;
PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2_volkImpl;
PFN_vkCmdResetEvent2 vkCmdResetEvent2_volkImpl;
PFN_vkCmdResolveImage2 vkCmdResolveImage2_volkImpl;
PFN_vkCmdSetCullMode vkCmdSetCullMode_volkImpl;
PFN_vkCmdSetDepthBiasEnable vkCmdSetDepthBiasEnable_volkImpl;
PFN_vkCmdSetDepthBoundsTestEnable vkCmdSetDepthBoundsTestEnable_volkImpl;
PFN_vkCmdSetDepthCompareOp vkCmdSetDepthCompareOp_volkImpl;
PFN_vkCmdSetDepthTestEnable vkCmdSetDepthTestEnable_volkImpl;
PFN_vkCmdSetDepthWriteEnable vkCmdSetDepthWriteEnable_volkImpl;
PFN_vkCmdSetEvent2 vkCmdSetEvent2_volkImpl;
PFN_vkCmdSetFrontFace vkCmdSetFrontFace_volkImpl;
PFN_vkCmdSetPrimitiveRestartEnable vkCmdSetPrimitiveRestartEnable_volkImpl;
PFN_vkCmdSetPrimitiveTopology vkCmdSetPrimitiveTopology_volkImpl;
PFN_vkCmdSetRasterizerDiscardEnable vkCmdSetRasterizerDiscardEnable_volkImpl;
PFN_vkCmdSetScissorWithCount vkCmdSetScissorWithCount_volkImpl;
PFN_vkCmdSetStencilOp vkCmdSetStencilOp_volkImpl;
PFN_vkCmdSetStencilTestEnable vkCmdSetStencilTestEnable_volkImpl;
PFN_vkCmdSetViewportWithCount vkCmdSetViewportWithCount_volkImpl;
PFN_vkCmdWaitEvents2 vkCmdWaitEvents2_volkImpl;
PFN_vkCmdWriteTimestamp2 vkCmdWriteTimestamp2_volkImpl;
PFN_vkCreatePrivateDataSlot vkCreatePrivateDataSlot_volkImpl;
PFN_vkDestroyPrivateDataSlot vkDestroyPrivateDataSlot_volkImpl;
PFN_vkGetDeviceBufferMemoryRequirements vkGetDeviceBufferMemoryRequirements_volkImpl;
PFN_vkGetDeviceImageMemoryRequirements vkGetDeviceImageMemoryRequirements_volkImpl;
PFN_vkGetDeviceImageSparseMemoryRequirements vkGetDeviceImageSparseMemoryRequirements_volkImpl;
PFN_vkGetPhysicalDeviceToolProperties vkGetPhysicalDeviceToolProperties_volkImpl;
PFN_vkGetPrivateData vkGetPrivateData_volkImpl;
PFN_vkQueueSubmit2 vkQueueSubmit2_volkImpl;
PFN_vkSetPrivateData vkSetPrivateData_volkImpl;
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_AMDX_shader_enqueue)
PFN_vkCmdDispatchGraphAMDX vkCmdDispatchGraphAMDX_volkImpl;
PFN_vkCmdDispatchGraphIndirectAMDX vkCmdDispatchGraphIndirectAMDX_volkImpl;
PFN_vkCmdDispatchGraphIndirectCountAMDX vkCmdDispatchGraphIndirectCountAMDX_volkImpl;
PFN_vkCmdInitializeGraphScratchMemoryAMDX vkCmdInitializeGraphScratchMemoryAMDX_volkImpl;
PFN_vkCreateExecutionGraphPipelinesAMDX vkCreateExecutionGraphPipelinesAMDX_volkImpl;
PFN_vkGetExecutionGraphPipelineNodeIndexAMDX vkGetExecutionGraphPipelineNodeIndexAMDX_volkImpl;
PFN_vkGetExecutionGraphPipelineScratchSizeAMDX vkGetExecutionGraphPipelineScratchSizeAMDX_volkImpl;
#endif /* defined(VK_AMDX_shader_enqueue) */
#if defined(VK_AMD_buffer_marker)
PFN_vkCmdWriteBufferMarkerAMD vkCmdWriteBufferMarkerAMD_volkImpl;
#endif /* defined(VK_AMD_buffer_marker) */
#if defined(VK_AMD_display_native_hdr)
PFN_vkSetLocalDimmingAMD vkSetLocalDimmingAMD_volkImpl;
#endif /* defined(VK_AMD_display_native_hdr) */
#if defined(VK_AMD_draw_indirect_count)
PFN_vkCmdDrawIndexedIndirectCountAMD vkCmdDrawIndexedIndirectCountAMD_volkImpl;
PFN_vkCmdDrawIndirectCountAMD vkCmdDrawIndirectCountAMD_volkImpl;
#endif /* defined(VK_AMD_draw_indirect_count) */
#if defined(VK_AMD_shader_info)
PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD_volkImpl;
#endif /* defined(VK_AMD_shader_info) */
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID_volkImpl;
PFN_vkGetMemoryAndroidHardwareBufferANDROID vkGetMemoryAndroidHardwareBufferANDROID_volkImpl;
#endif /* defined(VK_ANDROID_external_memory_android_hardware_buffer) */
#if defined(VK_EXT_acquire_drm_display)
PFN_vkAcquireDrmDisplayEXT vkAcquireDrmDisplayEXT_volkImpl;
PFN_vkGetDrmDisplayEXT vkGetDrmDisplayEXT_volkImpl;
#endif /* defined(VK_EXT_acquire_drm_display) */
#if defined(VK_EXT_acquire_xlib_display)
PFN_vkAcquireXlibDisplayEXT vkAcquireXlibDisplayEXT_volkImpl;
PFN_vkGetRandROutputDisplayEXT vkGetRandROutputDisplayEXT_volkImpl;
#endif /* defined(VK_EXT_acquire_xlib_display) */
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state)
PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT vkCmdSetAttachmentFeedbackLoopEnableEXT_volkImpl;
#endif /* defined(VK_EXT_attachment_feedback_loop_dynamic_state) */
#if defined(VK_EXT_buffer_device_address)
PFN_vkGetBufferDeviceAddressEXT vkGetBufferDeviceAddressEXT_volkImpl;
#endif /* defined(VK_EXT_buffer_device_address) */
#if defined(VK_EXT_calibrated_timestamps)
PFN_vkGetCalibratedTimestampsEXT vkGetCalibratedTimestampsEXT_volkImpl;
PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT vkGetPhysicalDeviceCalibrateableTimeDomainsEXT_volkImpl;
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_color_write_enable)
PFN_vkCmdSetColorWriteEnableEXT vkCmdSetColorWriteEnableEXT_volkImpl;
#endif /* defined(VK_EXT_color_write_enable) */
#if defined(VK_EXT_conditional_rendering)
PFN_vkCmdBeginConditionalRenderingEXT vkCmdBeginConditionalRenderingEXT_volkImpl;
PFN_vkCmdEndConditionalRenderingEXT vkCmdEndConditionalRenderingEXT_volkImpl;
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_debug_marker)
PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT_volkImpl;
PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT_volkImpl;
PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT_volkImpl;
PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT_volkImpl;
PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT_volkImpl;
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_debug_report)
PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT_volkImpl;
PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT_volkImpl;
PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT_volkImpl;
#endif /* defined(VK_EXT_debug_report) */
#if defined(VK_EXT_debug_utils)
PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT_volkImpl;
PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT_volkImpl;
PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT_volkImpl;
PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT_volkImpl;
PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT_volkImpl;
PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT_volkImpl;
PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT_volkImpl;
PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT_volkImpl;
PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT_volkImpl;
PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT_volkImpl;
PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT_volkImpl;
#endif /* defined(VK_EXT_debug_utils) */
#if defined(VK_EXT_depth_bias_control)
PFN_vkCmdSetDepthBias2EXT vkCmdSetDepthBias2EXT_volkImpl;
#endif /* defined(VK_EXT_depth_bias_control) */
#if defined(VK_EXT_descriptor_buffer)
PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT vkCmdBindDescriptorBufferEmbeddedSamplersEXT_volkImpl;
PFN_vkCmdBindDescriptorBuffersEXT vkCmdBindDescriptorBuffersEXT_volkImpl;
PFN_vkCmdSetDescriptorBufferOffsetsEXT vkCmdSetDescriptorBufferOffsetsEXT_volkImpl;
PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT vkGetBufferOpaqueCaptureDescriptorDataEXT_volkImpl;
PFN_vkGetDescriptorEXT vkGetDescriptorEXT_volkImpl;
PFN_vkGetDescriptorSetLayoutBindingOffsetEXT vkGetDescriptorSetLayoutBindingOffsetEXT_volkImpl;
PFN_vkGetDescriptorSetLayoutSizeEXT vkGetDescriptorSetLayoutSizeEXT_volkImpl;
PFN_vkGetImageOpaqueCaptureDescriptorDataEXT vkGetImageOpaqueCaptureDescriptorDataEXT_volkImpl;
PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT vkGetImageViewOpaqueCaptureDescriptorDataEXT_volkImpl;
PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT vkGetSamplerOpaqueCaptureDescriptorDataEXT_volkImpl;
#endif /* defined(VK_EXT_descriptor_buffer) */
#if defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing))
PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT_volkImpl;
#endif /* defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)) */
#if defined(VK_EXT_device_fault)
PFN_vkGetDeviceFaultInfoEXT vkGetDeviceFaultInfoEXT_volkImpl;
#endif /* defined(VK_EXT_device_fault) */
#if defined(VK_EXT_direct_mode_display)
PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT_volkImpl;
#endif /* defined(VK_EXT_direct_mode_display) */
#if defined(VK_EXT_directfb_surface)
PFN_vkCreateDirectFBSurfaceEXT vkCreateDirectFBSurfaceEXT_volkImpl;
PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT vkGetPhysicalDeviceDirectFBPresentationSupportEXT_volkImpl;
#endif /* defined(VK_EXT_directfb_surface) */
#if defined(VK_EXT_discard_rectangles)
PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT_volkImpl;
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2
PFN_vkCmdSetDiscardRectangleEnableEXT vkCmdSetDiscardRectangleEnableEXT_volkImpl;
PFN_vkCmdSetDiscardRectangleModeEXT vkCmdSetDiscardRectangleModeEXT_volkImpl;
#endif /* defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2 */
#if defined(VK_EXT_display_control)
PFN_vkDisplayPowerControlEXT vkDisplayPowerControlEXT_volkImpl;
PFN_vkGetSwapchainCounterEXT vkGetSwapchainCounterEXT_volkImpl;
PFN_vkRegisterDeviceEventEXT vkRegisterDeviceEventEXT_volkImpl;
PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT_volkImpl;
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_display_surface_counter)
PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT_volkImpl;
#endif /* defined(VK_EXT_display_surface_counter) */
#if defined(VK_EXT_external_memory_host)
PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT_volkImpl;
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_full_screen_exclusive)
PFN_vkAcquireFullScreenExclusiveModeEXT vkAcquireFullScreenExclusiveModeEXT_volkImpl;
PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT vkGetPhysicalDeviceSurfacePresentModes2EXT_volkImpl;
PFN_vkReleaseFullScreenExclusiveModeEXT vkReleaseFullScreenExclusiveModeEXT_volkImpl;
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_hdr_metadata)
PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT_volkImpl;
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_headless_surface)
PFN_vkCreateHeadlessSurfaceEXT vkCreateHeadlessSurfaceEXT_volkImpl;
#endif /* defined(VK_EXT_headless_surface) */
#if defined(VK_EXT_host_image_copy)
PFN_vkCopyImageToImageEXT vkCopyImageToImageEXT_volkImpl;
PFN_vkCopyImageToMemoryEXT vkCopyImageToMemoryEXT_volkImpl;
PFN_vkCopyMemoryToImageEXT vkCopyMemoryToImageEXT_volkImpl;
PFN_vkTransitionImageLayoutEXT vkTransitionImageLayoutEXT_volkImpl;
#endif /* defined(VK_EXT_host_image_copy) */
#if defined(VK_EXT_host_query_reset)
PFN_vkResetQueryPoolEXT vkResetQueryPoolEXT_volkImpl;
#endif /* defined(VK_EXT_host_query_reset) */
#if defined(VK_EXT_image_drm_format_modifier)
PFN_vkGetImageDrmFormatModifierPropertiesEXT vkGetImageDrmFormatModifierPropertiesEXT_volkImpl;
#endif /* defined(VK_EXT_image_drm_format_modifier) */
#if defined(VK_EXT_line_rasterization)
PFN_vkCmdSetLineStippleEXT vkCmdSetLineStippleEXT_volkImpl;
#endif /* defined(VK_EXT_line_rasterization) */
#if defined(VK_EXT_mesh_shader)
PFN_vkCmdDrawMeshTasksEXT vkCmdDrawMeshTasksEXT_volkImpl;
PFN_vkCmdDrawMeshTasksIndirectCountEXT vkCmdDrawMeshTasksIndirectCountEXT_volkImpl;
PFN_vkCmdDrawMeshTasksIndirectEXT vkCmdDrawMeshTasksIndirectEXT_volkImpl;
#endif /* defined(VK_EXT_mesh_shader) */
#if defined(VK_EXT_metal_objects)
PFN_vkExportMetalObjectsEXT vkExportMetalObjectsEXT_volkImpl;
#endif /* defined(VK_EXT_metal_objects) */
#if defined(VK_EXT_metal_surface)
PFN_vkCreateMetalSurfaceEXT vkCreateMetalSurfaceEXT_volkImpl;
#endif /* defined(VK_EXT_metal_surface) */
#if defined(VK_EXT_multi_draw)
PFN_vkCmdDrawMultiEXT vkCmdDrawMultiEXT_volkImpl;
PFN_vkCmdDrawMultiIndexedEXT vkCmdDrawMultiIndexedEXT_volkImpl;
#endif /* defined(VK_EXT_multi_draw) */
#if defined(VK_EXT_opacity_micromap)
PFN_vkBuildMicromapsEXT vkBuildMicromapsEXT_volkImpl;
PFN_vkCmdBuildMicromapsEXT vkCmdBuildMicromapsEXT_volkImpl;
PFN_vkCmdCopyMemoryToMicromapEXT vkCmdCopyMemoryToMicromapEXT_volkImpl;
PFN_vkCmdCopyMicromapEXT vkCmdCopyMicromapEXT_volkImpl;
PFN_vkCmdCopyMicromapToMemoryEXT vkCmdCopyMicromapToMemoryEXT_volkImpl;
PFN_vkCmdWriteMicromapsPropertiesEXT vkCmdWriteMicromapsPropertiesEXT_volkImpl;
PFN_vkCopyMemoryToMicromapEXT vkCopyMemoryToMicromapEXT_volkImpl;
PFN_vkCopyMicromapEXT vkCopyMicromapEXT_volkImpl;
PFN_vkCopyMicromapToMemoryEXT vkCopyMicromapToMemoryEXT_volkImpl;
PFN_vkCreateMicromapEXT vkCreateMicromapEXT_volkImpl;
PFN_vkDestroyMicromapEXT vkDestroyMicromapEXT_volkImpl;
PFN_vkGetDeviceMicromapCompatibilityEXT vkGetDeviceMicromapCompatibilityEXT_volkImpl;
PFN_vkGetMicromapBuildSizesEXT vkGetMicromapBuildSizesEXT_volkImpl;
PFN_vkWriteMicromapsPropertiesEXT vkWriteMicromapsPropertiesEXT_volkImpl;
#endif /* defined(VK_EXT_opacity_micromap) */
#if defined(VK_EXT_pageable_device_local_memory)
PFN_vkSetDeviceMemoryPriorityEXT vkSetDeviceMemoryPriorityEXT_volkImpl;
#endif /* defined(VK_EXT_pageable_device_local_memory) */
#if defined(VK_EXT_pipeline_properties)
PFN_vkGetPipelinePropertiesEXT vkGetPipelinePropertiesEXT_volkImpl;
#endif /* defined(VK_EXT_pipeline_properties) */
#if defined(VK_EXT_private_data)
PFN_vkCreatePrivateDataSlotEXT vkCreatePrivateDataSlotEXT_volkImpl;
PFN_vkDestroyPrivateDataSlotEXT vkDestroyPrivateDataSlotEXT_volkImpl;
PFN_vkGetPrivateDataEXT vkGetPrivateDataEXT_volkImpl;
PFN_vkSetPrivateDataEXT vkSetPrivateDataEXT_volkImpl;
#endif /* defined(VK_EXT_private_data) */
#if defined(VK_EXT_sample_locations)
PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT_volkImpl;
PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT_volkImpl;
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_shader_module_identifier)
PFN_vkGetShaderModuleCreateInfoIdentifierEXT vkGetShaderModuleCreateInfoIdentifierEXT_volkImpl;
PFN_vkGetShaderModuleIdentifierEXT vkGetShaderModuleIdentifierEXT_volkImpl;
#endif /* defined(VK_EXT_shader_module_identifier) */
#if defined(VK_EXT_shader_object)
PFN_vkCmdBindShadersEXT vkCmdBindShadersEXT_volkImpl;
PFN_vkCreateShadersEXT vkCreateShadersEXT_volkImpl;
PFN_vkDestroyShaderEXT vkDestroyShaderEXT_volkImpl;
PFN_vkGetShaderBinaryDataEXT vkGetShaderBinaryDataEXT_volkImpl;
#endif /* defined(VK_EXT_shader_object) */
#if defined(VK_EXT_swapchain_maintenance1)
PFN_vkReleaseSwapchainImagesEXT vkReleaseSwapchainImagesEXT_volkImpl;
#endif /* defined(VK_EXT_swapchain_maintenance1) */
#if defined(VK_EXT_tooling_info)
PFN_vkGetPhysicalDeviceToolPropertiesEXT vkGetPhysicalDeviceToolPropertiesEXT_volkImpl;
#endif /* defined(VK_EXT_tooling_info) */
#if defined(VK_EXT_transform_feedback)
PFN_vkCmdBeginQueryIndexedEXT vkCmdBeginQueryIndexedEXT_volkImpl;
PFN_vkCmdBeginTransformFeedbackEXT vkCmdBeginTransformFeedbackEXT_volkImpl;
PFN_vkCmdBindTransformFeedbackBuffersEXT vkCmdBindTransformFeedbackBuffersEXT_volkImpl;
PFN_vkCmdDrawIndirectByteCountEXT vkCmdDrawIndirectByteCountEXT_volkImpl;
PFN_vkCmdEndQueryIndexedEXT vkCmdEndQueryIndexedEXT_volkImpl;
PFN_vkCmdEndTransformFeedbackEXT vkCmdEndTransformFeedbackEXT_volkImpl;
#endif /* defined(VK_EXT_transform_feedback) */
#if defined(VK_EXT_validation_cache)
PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT_volkImpl;
PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT_volkImpl;
PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT_volkImpl;
PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT_volkImpl;
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_FUCHSIA_buffer_collection)
PFN_vkCreateBufferCollectionFUCHSIA vkCreateBufferCollectionFUCHSIA_volkImpl;
PFN_vkDestroyBufferCollectionFUCHSIA vkDestroyBufferCollectionFUCHSIA_volkImpl;
PFN_vkGetBufferCollectionPropertiesFUCHSIA vkGetBufferCollectionPropertiesFUCHSIA_volkImpl;
PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA vkSetBufferCollectionBufferConstraintsFUCHSIA_volkImpl;
PFN_vkSetBufferCollectionImageConstraintsFUCHSIA vkSetBufferCollectionImageConstraintsFUCHSIA_volkImpl;
#endif /* defined(VK_FUCHSIA_buffer_collection) */
#if defined(VK_FUCHSIA_external_memory)
PFN_vkGetMemoryZirconHandleFUCHSIA vkGetMemoryZirconHandleFUCHSIA_volkImpl;
PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA vkGetMemoryZirconHandlePropertiesFUCHSIA_volkImpl;
#endif /* defined(VK_FUCHSIA_external_memory) */
#if defined(VK_FUCHSIA_external_semaphore)
PFN_vkGetSemaphoreZirconHandleFUCHSIA vkGetSemaphoreZirconHandleFUCHSIA_volkImpl;
PFN_vkImportSemaphoreZirconHandleFUCHSIA vkImportSemaphoreZirconHandleFUCHSIA_volkImpl;
#endif /* defined(VK_FUCHSIA_external_semaphore) */
#if defined(VK_FUCHSIA_imagepipe_surface)
PFN_vkCreateImagePipeSurfaceFUCHSIA vkCreateImagePipeSurfaceFUCHSIA_volkImpl;
#endif /* defined(VK_FUCHSIA_imagepipe_surface) */
#if defined(VK_GGP_stream_descriptor_surface)
PFN_vkCreateStreamDescriptorSurfaceGGP vkCreateStreamDescriptorSurfaceGGP_volkImpl;
#endif /* defined(VK_GGP_stream_descriptor_surface) */
#if defined(VK_GOOGLE_display_timing)
PFN_vkGetPastPresentationTimingGOOGLE vkGetPastPresentationTimingGOOGLE_volkImpl;
PFN_vkGetRefreshCycleDurationGOOGLE vkGetRefreshCycleDurationGOOGLE_volkImpl;
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_HUAWEI_cluster_culling_shader)
PFN_vkCmdDrawClusterHUAWEI vkCmdDrawClusterHUAWEI_volkImpl;
PFN_vkCmdDrawClusterIndirectHUAWEI vkCmdDrawClusterIndirectHUAWEI_volkImpl;
#endif /* defined(VK_HUAWEI_cluster_culling_shader) */
#if defined(VK_HUAWEI_invocation_mask)
PFN_vkCmdBindInvocationMaskHUAWEI vkCmdBindInvocationMaskHUAWEI_volkImpl;
#endif /* defined(VK_HUAWEI_invocation_mask) */
#if defined(VK_HUAWEI_subpass_shading)
PFN_vkCmdSubpassShadingHUAWEI vkCmdSubpassShadingHUAWEI_volkImpl;
PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI_volkImpl;
#endif /* defined(VK_HUAWEI_subpass_shading) */
#if defined(VK_INTEL_performance_query)
PFN_vkAcquirePerformanceConfigurationINTEL vkAcquirePerformanceConfigurationINTEL_volkImpl;
PFN_vkCmdSetPerformanceMarkerINTEL vkCmdSetPerformanceMarkerINTEL_volkImpl;
PFN_vkCmdSetPerformanceOverrideINTEL vkCmdSetPerformanceOverrideINTEL_volkImpl;
PFN_vkCmdSetPerformanceStreamMarkerINTEL vkCmdSetPerformanceStreamMarkerINTEL_volkImpl;
PFN_vkGetPerformanceParameterINTEL vkGetPerformanceParameterINTEL_volkImpl;
PFN_vkInitializePerformanceApiINTEL vkInitializePerformanceApiINTEL_volkImpl;
PFN_vkQueueSetPerformanceConfigurationINTEL vkQueueSetPerformanceConfigurationINTEL_volkImpl;
PFN_vkReleasePerformanceConfigurationINTEL vkReleasePerformanceConfigurationINTEL_volkImpl;
PFN_vkUninitializePerformanceApiINTEL vkUninitializePerformanceApiINTEL_volkImpl;
#endif /* defined(VK_INTEL_performance_query) */
#if defined(VK_KHR_acceleration_structure)
PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR_volkImpl;
PFN_vkCmdBuildAccelerationStructuresIndirectKHR vkCmdBuildAccelerationStructuresIndirectKHR_volkImpl;
PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR_volkImpl;
PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR_volkImpl;
PFN_vkCmdCopyAccelerationStructureToMemoryKHR vkCmdCopyAccelerationStructureToMemoryKHR_volkImpl;
PFN_vkCmdCopyMemoryToAccelerationStructureKHR vkCmdCopyMemoryToAccelerationStructureKHR_volkImpl;
PFN_vkCmdWriteAccelerationStructuresPropertiesKHR vkCmdWriteAccelerationStructuresPropertiesKHR_volkImpl;
PFN_vkCopyAccelerationStructureKHR vkCopyAccelerationStructureKHR_volkImpl;
PFN_vkCopyAccelerationStructureToMemoryKHR vkCopyAccelerationStructureToMemoryKHR_volkImpl;
PFN_vkCopyMemoryToAccelerationStructureKHR vkCopyMemoryToAccelerationStructureKHR_volkImpl;
PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR_volkImpl;
PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR_volkImpl;
PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR_volkImpl;
PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR_volkImpl;
PFN_vkGetDeviceAccelerationStructureCompatibilityKHR vkGetDeviceAccelerationStructureCompatibilityKHR_volkImpl;
PFN_vkWriteAccelerationStructuresPropertiesKHR vkWriteAccelerationStructuresPropertiesKHR_volkImpl;
#endif /* defined(VK_KHR_acceleration_structure) */
#if defined(VK_KHR_android_surface)
PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR_volkImpl;
#endif /* defined(VK_KHR_android_surface) */
#if defined(VK_KHR_bind_memory2)
PFN_vkBindBufferMemory2KHR vkBindBufferMemory2KHR_volkImpl;
PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR_volkImpl;
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_buffer_device_address)
PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR_volkImpl;
PFN_vkGetBufferOpaqueCaptureAddressKHR vkGetBufferOpaqueCaptureAddressKHR_volkImpl;
PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR vkGetDeviceMemoryOpaqueCaptureAddressKHR_volkImpl;
#endif /* defined(VK_KHR_buffer_device_address) */
#if defined(VK_KHR_calibrated_timestamps)
PFN_vkGetCalibratedTimestampsKHR vkGetCalibratedTimestampsKHR_volkImpl;
PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR vkGetPhysicalDeviceCalibrateableTimeDomainsKHR_volkImpl;
#endif /* defined(VK_KHR_calibrated_timestamps) */
#if defined(VK_KHR_cooperative_matrix)
PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR_volkImpl;
#endif /* defined(VK_KHR_cooperative_matrix) */
#if defined(VK_KHR_copy_commands2)
PFN_vkCmdBlitImage2KHR vkCmdBlitImage2KHR_volkImpl;
PFN_vkCmdCopyBuffer2KHR vkCmdCopyBuffer2KHR_volkImpl;
PFN_vkCmdCopyBufferToImage2KHR vkCmdCopyBufferToImage2KHR_volkImpl;
PFN_vkCmdCopyImage2KHR vkCmdCopyImage2KHR_volkImpl;
PFN_vkCmdCopyImageToBuffer2KHR vkCmdCopyImageToBuffer2KHR_volkImpl;
PFN_vkCmdResolveImage2KHR vkCmdResolveImage2KHR_volkImpl;
#endif /* defined(VK_KHR_copy_commands2) */
#if defined(VK_KHR_create_renderpass2)
PFN_vkCmdBeginRenderPass2KHR vkCmdBeginRenderPass2KHR_volkImpl;
PFN_vkCmdEndRenderPass2KHR vkCmdEndRenderPass2KHR_volkImpl;
PFN_vkCmdNextSubpass2KHR vkCmdNextSubpass2KHR_volkImpl;
PFN_vkCreateRenderPass2KHR vkCreateRenderPass2KHR_volkImpl;
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_deferred_host_operations)
PFN_vkCreateDeferredOperationKHR vkCreateDeferredOperationKHR_volkImpl;
PFN_vkDeferredOperationJoinKHR vkDeferredOperationJoinKHR_volkImpl;
PFN_vkDestroyDeferredOperationKHR vkDestroyDeferredOperationKHR_volkImpl;
PFN_vkGetDeferredOperationMaxConcurrencyKHR vkGetDeferredOperationMaxConcurrencyKHR_volkImpl;
PFN_vkGetDeferredOperationResultKHR vkGetDeferredOperationResultKHR_volkImpl;
#endif /* defined(VK_KHR_deferred_host_operations) */
#if defined(VK_KHR_descriptor_update_template)
PFN_vkCreateDescriptorUpdateTemplateKHR vkCreateDescriptorUpdateTemplateKHR_volkImpl;
PFN_vkDestroyDescriptorUpdateTemplateKHR vkDestroyDescriptorUpdateTemplateKHR_volkImpl;
PFN_vkUpdateDescriptorSetWithTemplateKHR vkUpdateDescriptorSetWithTemplateKHR_volkImpl;
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
PFN_vkCmdDispatchBaseKHR vkCmdDispatchBaseKHR_volkImpl;
PFN_vkCmdSetDeviceMaskKHR vkCmdSetDeviceMaskKHR_volkImpl;
PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR_volkImpl;
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_device_group_creation)
PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR_volkImpl;
#endif /* defined(VK_KHR_device_group_creation) */
#if defined(VK_KHR_display)
PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR_volkImpl;
PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR_volkImpl;
PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR_volkImpl;
PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR_volkImpl;
PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR_volkImpl;
PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR_volkImpl;
PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR_volkImpl;
#endif /* defined(VK_KHR_display) */
#if defined(VK_KHR_display_swapchain)
PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR_volkImpl;
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
PFN_vkCmdDrawIndexedIndirectCountKHR vkCmdDrawIndexedIndirectCountKHR_volkImpl;
PFN_vkCmdDrawIndirectCountKHR vkCmdDrawIndirectCountKHR_volkImpl;
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_dynamic_rendering)
PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR_volkImpl;
PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR_volkImpl;
#endif /* defined(VK_KHR_dynamic_rendering) */
#if defined(VK_KHR_dynamic_rendering_local_read)
PFN_vkCmdSetRenderingAttachmentLocationsKHR vkCmdSetRenderingAttachmentLocationsKHR_volkImpl;
PFN_vkCmdSetRenderingInputAttachmentIndicesKHR vkCmdSetRenderingInputAttachmentIndicesKHR_volkImpl;
#endif /* defined(VK_KHR_dynamic_rendering_local_read) */
#if defined(VK_KHR_external_fence_capabilities)
PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR vkGetPhysicalDeviceExternalFencePropertiesKHR_volkImpl;
#endif /* defined(VK_KHR_external_fence_capabilities) */
#if defined(VK_KHR_external_fence_fd)
PFN_vkGetFenceFdKHR vkGetFenceFdKHR_volkImpl;
PFN_vkImportFenceFdKHR vkImportFenceFdKHR_volkImpl;
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
PFN_vkGetFenceWin32HandleKHR vkGetFenceWin32HandleKHR_volkImpl;
PFN_vkImportFenceWin32HandleKHR vkImportFenceWin32HandleKHR_volkImpl;
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_capabilities)
PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR vkGetPhysicalDeviceExternalBufferPropertiesKHR_volkImpl;
#endif /* defined(VK_KHR_external_memory_capabilities) */
#if defined(VK_KHR_external_memory_fd)
PFN_vkGetMemoryFdKHR vkGetMemoryFdKHR_volkImpl;
PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR_volkImpl;
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
PFN_vkGetMemoryWin32HandleKHR vkGetMemoryWin32HandleKHR_volkImpl;
PFN_vkGetMemoryWin32HandlePropertiesKHR vkGetMemoryWin32HandlePropertiesKHR_volkImpl;
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_capabilities)
PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR vkGetPhysicalDeviceExternalSemaphorePropertiesKHR_volkImpl;
#endif /* defined(VK_KHR_external_semaphore_capabilities) */
#if defined(VK_KHR_external_semaphore_fd)
PFN_vkGetSemaphoreFdKHR vkGetSemaphoreFdKHR_volkImpl;
PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR_volkImpl;
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
PFN_vkGetSemaphoreWin32HandleKHR vkGetSemaphoreWin32HandleKHR_volkImpl;
PFN_vkImportSemaphoreWin32HandleKHR vkImportSemaphoreWin32HandleKHR_volkImpl;
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_fragment_shading_rate)
PFN_vkCmdSetFragmentShadingRateKHR vkCmdSetFragmentShadingRateKHR_volkImpl;
PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR vkGetPhysicalDeviceFragmentShadingRatesKHR_volkImpl;
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_display_properties2)
PFN_vkGetDisplayModeProperties2KHR vkGetDisplayModeProperties2KHR_volkImpl;
PFN_vkGetDisplayPlaneCapabilities2KHR vkGetDisplayPlaneCapabilities2KHR_volkImpl;
PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR vkGetPhysicalDeviceDisplayPlaneProperties2KHR_volkImpl;
PFN_vkGetPhysicalDeviceDisplayProperties2KHR vkGetPhysicalDeviceDisplayProperties2KHR_volkImpl;
#endif /* defined(VK_KHR_get_display_properties2) */
#if defined(VK_KHR_get_memory_requirements2)
PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR_volkImpl;
PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR_volkImpl;
PFN_vkGetImageSparseMemoryRequirements2KHR vkGetImageSparseMemoryRequirements2KHR_volkImpl;
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_get_physical_device_properties2)
PFN_vkGetPhysicalDeviceFeatures2KHR vkGetPhysicalDeviceFeatures2KHR_volkImpl;
PFN_vkGetPhysicalDeviceFormatProperties2KHR vkGetPhysicalDeviceFormatProperties2KHR_volkImpl;
PFN_vkGetPhysicalDeviceImageFormatProperties2KHR vkGetPhysicalDeviceImageFormatProperties2KHR_volkImpl;
PFN_vkGetPhysicalDeviceMemoryProperties2KHR vkGetPhysicalDeviceMemoryProperties2KHR_volkImpl;
PFN_vkGetPhysicalDeviceProperties2KHR vkGetPhysicalDeviceProperties2KHR_volkImpl;
PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR vkGetPhysicalDeviceQueueFamilyProperties2KHR_volkImpl;
PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR vkGetPhysicalDeviceSparseImageFormatProperties2KHR_volkImpl;
#endif /* defined(VK_KHR_get_physical_device_properties2) */
#if defined(VK_KHR_get_surface_capabilities2)
PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR_volkImpl;
PFN_vkGetPhysicalDeviceSurfaceFormats2KHR vkGetPhysicalDeviceSurfaceFormats2KHR_volkImpl;
#endif /* defined(VK_KHR_get_surface_capabilities2) */
#if defined(VK_KHR_line_rasterization)
PFN_vkCmdSetLineStippleKHR vkCmdSetLineStippleKHR_volkImpl;
#endif /* defined(VK_KHR_line_rasterization) */
#if defined(VK_KHR_maintenance1)
PFN_vkTrimCommandPoolKHR vkTrimCommandPoolKHR_volkImpl;
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance3)
PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR_volkImpl;
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_maintenance4)
PFN_vkGetDeviceBufferMemoryRequirementsKHR vkGetDeviceBufferMemoryRequirementsKHR_volkImpl;
PFN_vkGetDeviceImageMemoryRequirementsKHR vkGetDeviceImageMemoryRequirementsKHR_volkImpl;
PFN_vkGetDeviceImageSparseMemoryRequirementsKHR vkGetDeviceImageSparseMemoryRequirementsKHR_volkImpl;
#endif /* defined(VK_KHR_maintenance4) */
#if defined(VK_KHR_maintenance5)
PFN_vkCmdBindIndexBuffer2KHR vkCmdBindIndexBuffer2KHR_volkImpl;
PFN_vkGetDeviceImageSubresourceLayoutKHR vkGetDeviceImageSubresourceLayoutKHR_volkImpl;
PFN_vkGetImageSubresourceLayout2KHR vkGetImageSubresourceLayout2KHR_volkImpl;
PFN_vkGetRenderingAreaGranularityKHR vkGetRenderingAreaGranularityKHR_volkImpl;
#endif /* defined(VK_KHR_maintenance5) */
#if defined(VK_KHR_maintenance6)
PFN_vkCmdBindDescriptorSets2KHR vkCmdBindDescriptorSets2KHR_volkImpl;
PFN_vkCmdPushConstants2KHR vkCmdPushConstants2KHR_volkImpl;
#endif /* defined(VK_KHR_maintenance6) */
#if defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor)
PFN_vkCmdPushDescriptorSet2KHR vkCmdPushDescriptorSet2KHR_volkImpl;
PFN_vkCmdPushDescriptorSetWithTemplate2KHR vkCmdPushDescriptorSetWithTemplate2KHR_volkImpl;
#endif /* defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer)
PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT vkCmdBindDescriptorBufferEmbeddedSamplers2EXT_volkImpl;
PFN_vkCmdSetDescriptorBufferOffsets2EXT vkCmdSetDescriptorBufferOffsets2EXT_volkImpl;
#endif /* defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer) */
#if defined(VK_KHR_map_memory2)
PFN_vkMapMemory2KHR vkMapMemory2KHR_volkImpl;
PFN_vkUnmapMemory2KHR vkUnmapMemory2KHR_volkImpl;
#endif /* defined(VK_KHR_map_memory2) */
#if defined(VK_KHR_performance_query)
PFN_vkAcquireProfilingLockKHR vkAcquireProfilingLockKHR_volkImpl;
PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR_volkImpl;
PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR_volkImpl;
PFN_vkReleaseProfilingLockKHR vkReleaseProfilingLockKHR_volkImpl;
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_pipeline_executable_properties)
PFN_vkGetPipelineExecutableInternalRepresentationsKHR vkGetPipelineExecutableInternalRepresentationsKHR_volkImpl;
PFN_vkGetPipelineExecutablePropertiesKHR vkGetPipelineExecutablePropertiesKHR_volkImpl;
PFN_vkGetPipelineExecutableStatisticsKHR vkGetPipelineExecutableStatisticsKHR_volkImpl;
#endif /* defined(VK_KHR_pipeline_executable_properties) */
#if defined(VK_KHR_present_wait)
PFN_vkWaitForPresentKHR vkWaitForPresentKHR_volkImpl;
#endif /* defined(VK_KHR_present_wait) */
#if defined(VK_KHR_push_descriptor)
PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR_volkImpl;
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline)
PFN_vkCmdTraceRaysIndirect2KHR vkCmdTraceRaysIndirect2KHR_volkImpl;
#endif /* defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_ray_tracing_pipeline)
PFN_vkCmdSetRayTracingPipelineStackSizeKHR vkCmdSetRayTracingPipelineStackSizeKHR_volkImpl;
PFN_vkCmdTraceRaysIndirectKHR vkCmdTraceRaysIndirectKHR_volkImpl;
PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR_volkImpl;
PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR_volkImpl;
PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR vkGetRayTracingCaptureReplayShaderGroupHandlesKHR_volkImpl;
PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR_volkImpl;
PFN_vkGetRayTracingShaderGroupStackSizeKHR vkGetRayTracingShaderGroupStackSizeKHR_volkImpl;
#endif /* defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
PFN_vkCreateSamplerYcbcrConversionKHR vkCreateSamplerYcbcrConversionKHR_volkImpl;
PFN_vkDestroySamplerYcbcrConversionKHR vkDestroySamplerYcbcrConversionKHR_volkImpl;
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR_volkImpl;
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_surface)
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR_volkImpl;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR_volkImpl;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR_volkImpl;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR_volkImpl;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR_volkImpl;
#endif /* defined(VK_KHR_surface) */
#if defined(VK_KHR_swapchain)
PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR_volkImpl;
PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR_volkImpl;
PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR_volkImpl;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR_volkImpl;
PFN_vkQueuePresentKHR vkQueuePresentKHR_volkImpl;
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_synchronization2)
PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR_volkImpl;
PFN_vkCmdResetEvent2KHR vkCmdResetEvent2KHR_volkImpl;
PFN_vkCmdSetEvent2KHR vkCmdSetEvent2KHR_volkImpl;
PFN_vkCmdWaitEvents2KHR vkCmdWaitEvents2KHR_volkImpl;
PFN_vkCmdWriteTimestamp2KHR vkCmdWriteTimestamp2KHR_volkImpl;
PFN_vkQueueSubmit2KHR vkQueueSubmit2KHR_volkImpl;
#endif /* defined(VK_KHR_synchronization2) */
#if defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker)
PFN_vkCmdWriteBufferMarker2AMD vkCmdWriteBufferMarker2AMD_volkImpl;
#endif /* defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker) */
#if defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints)
PFN_vkGetQueueCheckpointData2NV vkGetQueueCheckpointData2NV_volkImpl;
#endif /* defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_KHR_timeline_semaphore)
PFN_vkGetSemaphoreCounterValueKHR vkGetSemaphoreCounterValueKHR_volkImpl;
PFN_vkSignalSemaphoreKHR vkSignalSemaphoreKHR_volkImpl;
PFN_vkWaitSemaphoresKHR vkWaitSemaphoresKHR_volkImpl;
#endif /* defined(VK_KHR_timeline_semaphore) */
#if defined(VK_KHR_video_decode_queue)
PFN_vkCmdDecodeVideoKHR vkCmdDecodeVideoKHR_volkImpl;
#endif /* defined(VK_KHR_video_decode_queue) */
#if defined(VK_KHR_video_encode_queue)
PFN_vkCmdEncodeVideoKHR vkCmdEncodeVideoKHR_volkImpl;
PFN_vkGetEncodedVideoSessionParametersKHR vkGetEncodedVideoSessionParametersKHR_volkImpl;
PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR_volkImpl;
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
PFN_vkBindVideoSessionMemoryKHR vkBindVideoSessionMemoryKHR_volkImpl;
PFN_vkCmdBeginVideoCodingKHR vkCmdBeginVideoCodingKHR_volkImpl;
PFN_vkCmdControlVideoCodingKHR vkCmdControlVideoCodingKHR_volkImpl;
PFN_vkCmdEndVideoCodingKHR vkCmdEndVideoCodingKHR_volkImpl;
PFN_vkCreateVideoSessionKHR vkCreateVideoSessionKHR_volkImpl;
PFN_vkCreateVideoSessionParametersKHR vkCreateVideoSessionParametersKHR_volkImpl;
PFN_vkDestroyVideoSessionKHR vkDestroyVideoSessionKHR_volkImpl;
PFN_vkDestroyVideoSessionParametersKHR vkDestroyVideoSessionParametersKHR_volkImpl;
PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR vkGetPhysicalDeviceVideoCapabilitiesKHR_volkImpl;
PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR vkGetPhysicalDeviceVideoFormatPropertiesKHR_volkImpl;
PFN_vkGetVideoSessionMemoryRequirementsKHR vkGetVideoSessionMemoryRequirementsKHR_volkImpl;
PFN_vkUpdateVideoSessionParametersKHR vkUpdateVideoSessionParametersKHR_volkImpl;
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_KHR_wayland_surface)
PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR_volkImpl;
PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR_volkImpl;
#endif /* defined(VK_KHR_wayland_surface) */
#if defined(VK_KHR_win32_surface)
PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR_volkImpl;
PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR_volkImpl;
#endif /* defined(VK_KHR_win32_surface) */
#if defined(VK_KHR_xcb_surface)
PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR_volkImpl;
PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR_volkImpl;
#endif /* defined(VK_KHR_xcb_surface) */
#if defined(VK_KHR_xlib_surface)
PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR_volkImpl;
PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR_volkImpl;
#endif /* defined(VK_KHR_xlib_surface) */
#if defined(VK_MVK_ios_surface)
PFN_vkCreateIOSSurfaceMVK vkCreateIOSSurfaceMVK_volkImpl;
#endif /* defined(VK_MVK_ios_surface) */
#if defined(VK_MVK_macos_surface)
PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK_volkImpl;
#endif /* defined(VK_MVK_macos_surface) */
#if defined(VK_NN_vi_surface)
PFN_vkCreateViSurfaceNN vkCreateViSurfaceNN_volkImpl;
#endif /* defined(VK_NN_vi_surface) */
#if defined(VK_NVX_binary_import)
PFN_vkCmdCuLaunchKernelNVX vkCmdCuLaunchKernelNVX_volkImpl;
PFN_vkCreateCuFunctionNVX vkCreateCuFunctionNVX_volkImpl;
PFN_vkCreateCuModuleNVX vkCreateCuModuleNVX_volkImpl;
PFN_vkDestroyCuFunctionNVX vkDestroyCuFunctionNVX_volkImpl;
PFN_vkDestroyCuModuleNVX vkDestroyCuModuleNVX_volkImpl;
#endif /* defined(VK_NVX_binary_import) */
#if defined(VK_NVX_image_view_handle)
PFN_vkGetImageViewAddressNVX vkGetImageViewAddressNVX_volkImpl;
PFN_vkGetImageViewHandleNVX vkGetImageViewHandleNVX_volkImpl;
#endif /* defined(VK_NVX_image_view_handle) */
#if defined(VK_NV_acquire_winrt_display)
PFN_vkAcquireWinrtDisplayNV vkAcquireWinrtDisplayNV_volkImpl;
PFN_vkGetWinrtDisplayNV vkGetWinrtDisplayNV_volkImpl;
#endif /* defined(VK_NV_acquire_winrt_display) */
#if defined(VK_NV_clip_space_w_scaling)
PFN_vkCmdSetViewportWScalingNV vkCmdSetViewportWScalingNV_volkImpl;
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_cooperative_matrix)
PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV vkGetPhysicalDeviceCooperativeMatrixPropertiesNV_volkImpl;
#endif /* defined(VK_NV_cooperative_matrix) */
#if defined(VK_NV_copy_memory_indirect)
PFN_vkCmdCopyMemoryIndirectNV vkCmdCopyMemoryIndirectNV_volkImpl;
PFN_vkCmdCopyMemoryToImageIndirectNV vkCmdCopyMemoryToImageIndirectNV_volkImpl;
#endif /* defined(VK_NV_copy_memory_indirect) */
#if defined(VK_NV_coverage_reduction_mode)
PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV_volkImpl;
#endif /* defined(VK_NV_coverage_reduction_mode) */
#if defined(VK_NV_cuda_kernel_launch)
PFN_vkCmdCudaLaunchKernelNV vkCmdCudaLaunchKernelNV_volkImpl;
PFN_vkCreateCudaFunctionNV vkCreateCudaFunctionNV_volkImpl;
PFN_vkCreateCudaModuleNV vkCreateCudaModuleNV_volkImpl;
PFN_vkDestroyCudaFunctionNV vkDestroyCudaFunctionNV_volkImpl;
PFN_vkDestroyCudaModuleNV vkDestroyCudaModuleNV_volkImpl;
PFN_vkGetCudaModuleCacheNV vkGetCudaModuleCacheNV_volkImpl;
#endif /* defined(VK_NV_cuda_kernel_launch) */
#if defined(VK_NV_device_diagnostic_checkpoints)
PFN_vkCmdSetCheckpointNV vkCmdSetCheckpointNV_volkImpl;
PFN_vkGetQueueCheckpointDataNV vkGetQueueCheckpointDataNV_volkImpl;
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_device_generated_commands)
PFN_vkCmdBindPipelineShaderGroupNV vkCmdBindPipelineShaderGroupNV_volkImpl;
PFN_vkCmdExecuteGeneratedCommandsNV vkCmdExecuteGeneratedCommandsNV_volkImpl;
PFN_vkCmdPreprocessGeneratedCommandsNV vkCmdPreprocessGeneratedCommandsNV_volkImpl;
PFN_vkCreateIndirectCommandsLayoutNV vkCreateIndirectCommandsLayoutNV_volkImpl;
PFN_vkDestroyIndirectCommandsLayoutNV vkDestroyIndirectCommandsLayoutNV_volkImpl;
PFN_vkGetGeneratedCommandsMemoryRequirementsNV vkGetGeneratedCommandsMemoryRequirementsNV_volkImpl;
#endif /* defined(VK_NV_device_generated_commands) */
#if defined(VK_NV_device_generated_commands_compute)
PFN_vkCmdUpdatePipelineIndirectBufferNV vkCmdUpdatePipelineIndirectBufferNV_volkImpl;
PFN_vkGetPipelineIndirectDeviceAddressNV vkGetPipelineIndirectDeviceAddressNV_volkImpl;
PFN_vkGetPipelineIndirectMemoryRequirementsNV vkGetPipelineIndirectMemoryRequirementsNV_volkImpl;
#endif /* defined(VK_NV_device_generated_commands_compute) */
#if defined(VK_NV_external_memory_capabilities)
PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV vkGetPhysicalDeviceExternalImageFormatPropertiesNV_volkImpl;
#endif /* defined(VK_NV_external_memory_capabilities) */
#if defined(VK_NV_external_memory_rdma)
PFN_vkGetMemoryRemoteAddressNV vkGetMemoryRemoteAddressNV_volkImpl;
#endif /* defined(VK_NV_external_memory_rdma) */
#if defined(VK_NV_external_memory_win32)
PFN_vkGetMemoryWin32HandleNV vkGetMemoryWin32HandleNV_volkImpl;
#endif /* defined(VK_NV_external_memory_win32) */
#if defined(VK_NV_fragment_shading_rate_enums)
PFN_vkCmdSetFragmentShadingRateEnumNV vkCmdSetFragmentShadingRateEnumNV_volkImpl;
#endif /* defined(VK_NV_fragment_shading_rate_enums) */
#if defined(VK_NV_low_latency2)
PFN_vkGetLatencyTimingsNV vkGetLatencyTimingsNV_volkImpl;
PFN_vkLatencySleepNV vkLatencySleepNV_volkImpl;
PFN_vkQueueNotifyOutOfBandNV vkQueueNotifyOutOfBandNV_volkImpl;
PFN_vkSetLatencyMarkerNV vkSetLatencyMarkerNV_volkImpl;
PFN_vkSetLatencySleepModeNV vkSetLatencySleepModeNV_volkImpl;
#endif /* defined(VK_NV_low_latency2) */
#if defined(VK_NV_memory_decompression)
PFN_vkCmdDecompressMemoryIndirectCountNV vkCmdDecompressMemoryIndirectCountNV_volkImpl;
PFN_vkCmdDecompressMemoryNV vkCmdDecompressMemoryNV_volkImpl;
#endif /* defined(VK_NV_memory_decompression) */
#if defined(VK_NV_mesh_shader)
PFN_vkCmdDrawMeshTasksIndirectCountNV vkCmdDrawMeshTasksIndirectCountNV_volkImpl;
PFN_vkCmdDrawMeshTasksIndirectNV vkCmdDrawMeshTasksIndirectNV_volkImpl;
PFN_vkCmdDrawMeshTasksNV vkCmdDrawMeshTasksNV_volkImpl;
#endif /* defined(VK_NV_mesh_shader) */
#if defined(VK_NV_optical_flow)
PFN_vkBindOpticalFlowSessionImageNV vkBindOpticalFlowSessionImageNV_volkImpl;
PFN_vkCmdOpticalFlowExecuteNV vkCmdOpticalFlowExecuteNV_volkImpl;
PFN_vkCreateOpticalFlowSessionNV vkCreateOpticalFlowSessionNV_volkImpl;
PFN_vkDestroyOpticalFlowSessionNV vkDestroyOpticalFlowSessionNV_volkImpl;
PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV vkGetPhysicalDeviceOpticalFlowImageFormatsNV_volkImpl;
#endif /* defined(VK_NV_optical_flow) */
#if defined(VK_NV_ray_tracing)
PFN_vkBindAccelerationStructureMemoryNV vkBindAccelerationStructureMemoryNV_volkImpl;
PFN_vkCmdBuildAccelerationStructureNV vkCmdBuildAccelerationStructureNV_volkImpl;
PFN_vkCmdCopyAccelerationStructureNV vkCmdCopyAccelerationStructureNV_volkImpl;
PFN_vkCmdTraceRaysNV vkCmdTraceRaysNV_volkImpl;
PFN_vkCmdWriteAccelerationStructuresPropertiesNV vkCmdWriteAccelerationStructuresPropertiesNV_volkImpl;
PFN_vkCompileDeferredNV vkCompileDeferredNV_volkImpl;
PFN_vkCreateAccelerationStructureNV vkCreateAccelerationStructureNV_volkImpl;
PFN_vkCreateRayTracingPipelinesNV vkCreateRayTracingPipelinesNV_volkImpl;
PFN_vkDestroyAccelerationStructureNV vkDestroyAccelerationStructureNV_volkImpl;
PFN_vkGetAccelerationStructureHandleNV vkGetAccelerationStructureHandleNV_volkImpl;
PFN_vkGetAccelerationStructureMemoryRequirementsNV vkGetAccelerationStructureMemoryRequirementsNV_volkImpl;
PFN_vkGetRayTracingShaderGroupHandlesNV vkGetRayTracingShaderGroupHandlesNV_volkImpl;
#endif /* defined(VK_NV_ray_tracing) */
#if defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2
PFN_vkCmdSetExclusiveScissorEnableNV vkCmdSetExclusiveScissorEnableNV_volkImpl;
#endif /* defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2 */
#if defined(VK_NV_scissor_exclusive)
PFN_vkCmdSetExclusiveScissorNV vkCmdSetExclusiveScissorNV_volkImpl;
#endif /* defined(VK_NV_scissor_exclusive) */
#if defined(VK_NV_shading_rate_image)
PFN_vkCmdBindShadingRateImageNV vkCmdBindShadingRateImageNV_volkImpl;
PFN_vkCmdSetCoarseSampleOrderNV vkCmdSetCoarseSampleOrderNV_volkImpl;
PFN_vkCmdSetViewportShadingRatePaletteNV vkCmdSetViewportShadingRatePaletteNV_volkImpl;
#endif /* defined(VK_NV_shading_rate_image) */
#if defined(VK_QCOM_tile_properties)
PFN_vkGetDynamicRenderingTilePropertiesQCOM vkGetDynamicRenderingTilePropertiesQCOM_volkImpl;
PFN_vkGetFramebufferTilePropertiesQCOM vkGetFramebufferTilePropertiesQCOM_volkImpl;
#endif /* defined(VK_QCOM_tile_properties) */
#if defined(VK_QNX_external_memory_screen_buffer)
PFN_vkGetScreenBufferPropertiesQNX vkGetScreenBufferPropertiesQNX_volkImpl;
#endif /* defined(VK_QNX_external_memory_screen_buffer) */
#if defined(VK_QNX_screen_surface)
PFN_vkCreateScreenSurfaceQNX vkCreateScreenSurfaceQNX_volkImpl;
PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX vkGetPhysicalDeviceScreenPresentationSupportQNX_volkImpl;
#endif /* defined(VK_QNX_screen_surface) */
#if defined(VK_VALVE_descriptor_set_host_mapping)
PFN_vkGetDescriptorSetHostMappingVALVE vkGetDescriptorSetHostMappingVALVE_volkImpl;
PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE vkGetDescriptorSetLayoutHostMappingInfoVALVE_volkImpl;
#endif /* defined(VK_VALVE_descriptor_set_host_mapping) */
#if (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object))
PFN_vkCmdBindVertexBuffers2EXT vkCmdBindVertexBuffers2EXT_volkImpl;
PFN_vkCmdSetCullModeEXT vkCmdSetCullModeEXT_volkImpl;
PFN_vkCmdSetDepthBoundsTestEnableEXT vkCmdSetDepthBoundsTestEnableEXT_volkImpl;
PFN_vkCmdSetDepthCompareOpEXT vkCmdSetDepthCompareOpEXT_volkImpl;
PFN_vkCmdSetDepthTestEnableEXT vkCmdSetDepthTestEnableEXT_volkImpl;
PFN_vkCmdSetDepthWriteEnableEXT vkCmdSetDepthWriteEnableEXT_volkImpl;
PFN_vkCmdSetFrontFaceEXT vkCmdSetFrontFaceEXT_volkImpl;
PFN_vkCmdSetPrimitiveTopologyEXT vkCmdSetPrimitiveTopologyEXT_volkImpl;
PFN_vkCmdSetScissorWithCountEXT vkCmdSetScissorWithCountEXT_volkImpl;
PFN_vkCmdSetStencilOpEXT vkCmdSetStencilOpEXT_volkImpl;
PFN_vkCmdSetStencilTestEnableEXT vkCmdSetStencilTestEnableEXT_volkImpl;
PFN_vkCmdSetViewportWithCountEXT vkCmdSetViewportWithCountEXT_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object))
PFN_vkCmdSetDepthBiasEnableEXT vkCmdSetDepthBiasEnableEXT_volkImpl;
PFN_vkCmdSetLogicOpEXT vkCmdSetLogicOpEXT_volkImpl;
PFN_vkCmdSetPatchControlPointsEXT vkCmdSetPatchControlPointsEXT_volkImpl;
PFN_vkCmdSetPrimitiveRestartEnableEXT vkCmdSetPrimitiveRestartEnableEXT_volkImpl;
PFN_vkCmdSetRasterizerDiscardEnableEXT vkCmdSetRasterizerDiscardEnableEXT_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object))
PFN_vkCmdSetAlphaToCoverageEnableEXT vkCmdSetAlphaToCoverageEnableEXT_volkImpl;
PFN_vkCmdSetAlphaToOneEnableEXT vkCmdSetAlphaToOneEnableEXT_volkImpl;
PFN_vkCmdSetColorBlendEnableEXT vkCmdSetColorBlendEnableEXT_volkImpl;
PFN_vkCmdSetColorBlendEquationEXT vkCmdSetColorBlendEquationEXT_volkImpl;
PFN_vkCmdSetColorWriteMaskEXT vkCmdSetColorWriteMaskEXT_volkImpl;
PFN_vkCmdSetDepthClampEnableEXT vkCmdSetDepthClampEnableEXT_volkImpl;
PFN_vkCmdSetLogicOpEnableEXT vkCmdSetLogicOpEnableEXT_volkImpl;
PFN_vkCmdSetPolygonModeEXT vkCmdSetPolygonModeEXT_volkImpl;
PFN_vkCmdSetRasterizationSamplesEXT vkCmdSetRasterizationSamplesEXT_volkImpl;
PFN_vkCmdSetSampleMaskEXT vkCmdSetSampleMaskEXT_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object))
PFN_vkCmdSetTessellationDomainOriginEXT vkCmdSetTessellationDomainOriginEXT_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback))
PFN_vkCmdSetRasterizationStreamEXT vkCmdSetRasterizationStreamEXT_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization))
PFN_vkCmdSetConservativeRasterizationModeEXT vkCmdSetConservativeRasterizationModeEXT_volkImpl;
PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT vkCmdSetExtraPrimitiveOverestimationSizeEXT_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable))
PFN_vkCmdSetDepthClipEnableEXT vkCmdSetDepthClipEnableEXT_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations))
PFN_vkCmdSetSampleLocationsEnableEXT vkCmdSetSampleLocationsEnableEXT_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced))
PFN_vkCmdSetColorBlendAdvancedEXT vkCmdSetColorBlendAdvancedEXT_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex))
PFN_vkCmdSetProvokingVertexModeEXT vkCmdSetProvokingVertexModeEXT_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization))
PFN_vkCmdSetLineRasterizationModeEXT vkCmdSetLineRasterizationModeEXT_volkImpl;
PFN_vkCmdSetLineStippleEnableEXT vkCmdSetLineStippleEnableEXT_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control))
PFN_vkCmdSetDepthClipNegativeOneToOneEXT vkCmdSetDepthClipNegativeOneToOneEXT_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling))
PFN_vkCmdSetViewportWScalingEnableNV vkCmdSetViewportWScalingEnableNV_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle))
PFN_vkCmdSetViewportSwizzleNV vkCmdSetViewportSwizzleNV_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color))
PFN_vkCmdSetCoverageToColorEnableNV vkCmdSetCoverageToColorEnableNV_volkImpl;
PFN_vkCmdSetCoverageToColorLocationNV vkCmdSetCoverageToColorLocationNV_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples))
PFN_vkCmdSetCoverageModulationModeNV vkCmdSetCoverageModulationModeNV_volkImpl;
PFN_vkCmdSetCoverageModulationTableEnableNV vkCmdSetCoverageModulationTableEnableNV_volkImpl;
PFN_vkCmdSetCoverageModulationTableNV vkCmdSetCoverageModulationTableNV_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image))
PFN_vkCmdSetShadingRateImageEnableNV vkCmdSetShadingRateImageEnableNV_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test))
PFN_vkCmdSetRepresentativeFragmentTestEnableNV vkCmdSetRepresentativeFragmentTestEnableNV_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode))
PFN_vkCmdSetCoverageReductionModeNV vkCmdSetCoverageReductionModeNV_volkImpl;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode)) */
#if (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1))
PFN_vkGetDeviceGroupSurfacePresentModes2EXT vkGetDeviceGroupSurfacePresentModes2EXT_volkImpl;
#endif /* (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1)) */
#if (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control))
PFN_vkGetImageSubresourceLayout2EXT vkGetImageSubresourceLayout2EXT_volkImpl;
#endif /* (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control)) */
#if (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state))
PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT_volkImpl;
#endif /* (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state)) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template))
PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR_volkImpl;
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR_volkImpl;
PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR_volkImpl;
PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR_volkImpl;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR_volkImpl;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
/* VOLK_GENERATE_PROTOTYPES_C */

#ifdef __GNUC__
#	pragma GCC visibility pop
#endif

#ifdef __cplusplus
}
#endif
/* clang-format on */