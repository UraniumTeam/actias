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
	vkGetInstanceProcAddr_volkImpl = (PFN_vkGetInstanceProcAddr)(void(*)(void))GetProcAddress(module, "vkGetInstanceProcAddr_volkImpl");
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

	vkGetInstanceProcAddr_volkImpl = (PFN_vkGetInstanceProcAddr)dlsym(module, "vkGetInstanceProcAddr_volkImpl");
#else
	void* module = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
	if (!module)
		module = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
	if (!module)
		return VK_ERROR_INITIALIZATION_FAILED;

	vkGetInstanceProcAddr_volkImpl = (PFN_vkGetInstanceProcAddr)dlsym(module, "vkGetInstanceProcAddr_volkImpl");
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
	vkCreateInstance_volkImpl = (PFN_vkCreateInstance)load(context, "vkCreateInstance_volkImpl");
	vkEnumerateInstanceExtensionProperties_volkImpl = (PFN_vkEnumerateInstanceExtensionProperties)load(context, "vkEnumerateInstanceExtensionProperties_volkImpl");
	vkEnumerateInstanceLayerProperties_volkImpl = (PFN_vkEnumerateInstanceLayerProperties)load(context, "vkEnumerateInstanceLayerProperties_volkImpl");
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
	vkEnumerateInstanceVersion_volkImpl = (PFN_vkEnumerateInstanceVersion)load(context, "vkEnumerateInstanceVersion_volkImpl");
#endif /* defined(VK_VERSION_1_1) */
	/* VOLK_GENERATE_LOAD_LOADER */
}

static void volkGenLoadInstance(void* context, PFN_vkVoidFunction (*load)(void*, const char*))
{
	/* VOLK_GENERATE_LOAD_INSTANCE */
#if defined(VK_VERSION_1_0)
	vkCreateDevice_volkImpl = (PFN_vkCreateDevice)load(context, "vkCreateDevice_volkImpl");
	vkDestroyInstance_volkImpl = (PFN_vkDestroyInstance)load(context, "vkDestroyInstance_volkImpl");
	vkEnumerateDeviceExtensionProperties_volkImpl = (PFN_vkEnumerateDeviceExtensionProperties)load(context, "vkEnumerateDeviceExtensionProperties_volkImpl");
	vkEnumerateDeviceLayerProperties_volkImpl = (PFN_vkEnumerateDeviceLayerProperties)load(context, "vkEnumerateDeviceLayerProperties_volkImpl");
	vkEnumeratePhysicalDevices_volkImpl = (PFN_vkEnumeratePhysicalDevices)load(context, "vkEnumeratePhysicalDevices_volkImpl");
	vkGetDeviceProcAddr_volkImpl = (PFN_vkGetDeviceProcAddr)load(context, "vkGetDeviceProcAddr_volkImpl");
	vkGetPhysicalDeviceFeatures_volkImpl = (PFN_vkGetPhysicalDeviceFeatures)load(context, "vkGetPhysicalDeviceFeatures_volkImpl");
	vkGetPhysicalDeviceFormatProperties_volkImpl = (PFN_vkGetPhysicalDeviceFormatProperties)load(context, "vkGetPhysicalDeviceFormatProperties_volkImpl");
	vkGetPhysicalDeviceImageFormatProperties_volkImpl = (PFN_vkGetPhysicalDeviceImageFormatProperties)load(context, "vkGetPhysicalDeviceImageFormatProperties_volkImpl");
	vkGetPhysicalDeviceMemoryProperties_volkImpl = (PFN_vkGetPhysicalDeviceMemoryProperties)load(context, "vkGetPhysicalDeviceMemoryProperties_volkImpl");
	vkGetPhysicalDeviceProperties_volkImpl = (PFN_vkGetPhysicalDeviceProperties)load(context, "vkGetPhysicalDeviceProperties_volkImpl");
	vkGetPhysicalDeviceQueueFamilyProperties_volkImpl = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)load(context, "vkGetPhysicalDeviceQueueFamilyProperties_volkImpl");
	vkGetPhysicalDeviceSparseImageFormatProperties_volkImpl = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties)load(context, "vkGetPhysicalDeviceSparseImageFormatProperties_volkImpl");
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
	vkEnumeratePhysicalDeviceGroups_volkImpl = (PFN_vkEnumeratePhysicalDeviceGroups)load(context, "vkEnumeratePhysicalDeviceGroups_volkImpl");
	vkGetPhysicalDeviceExternalBufferProperties_volkImpl = (PFN_vkGetPhysicalDeviceExternalBufferProperties)load(context, "vkGetPhysicalDeviceExternalBufferProperties_volkImpl");
	vkGetPhysicalDeviceExternalFenceProperties_volkImpl = (PFN_vkGetPhysicalDeviceExternalFenceProperties)load(context, "vkGetPhysicalDeviceExternalFenceProperties_volkImpl");
	vkGetPhysicalDeviceExternalSemaphoreProperties_volkImpl = (PFN_vkGetPhysicalDeviceExternalSemaphoreProperties)load(context, "vkGetPhysicalDeviceExternalSemaphoreProperties_volkImpl");
	vkGetPhysicalDeviceFeatures2_volkImpl = (PFN_vkGetPhysicalDeviceFeatures2)load(context, "vkGetPhysicalDeviceFeatures2_volkImpl");
	vkGetPhysicalDeviceFormatProperties2_volkImpl = (PFN_vkGetPhysicalDeviceFormatProperties2)load(context, "vkGetPhysicalDeviceFormatProperties2_volkImpl");
	vkGetPhysicalDeviceImageFormatProperties2_volkImpl = (PFN_vkGetPhysicalDeviceImageFormatProperties2)load(context, "vkGetPhysicalDeviceImageFormatProperties2_volkImpl");
	vkGetPhysicalDeviceMemoryProperties2_volkImpl = (PFN_vkGetPhysicalDeviceMemoryProperties2)load(context, "vkGetPhysicalDeviceMemoryProperties2_volkImpl");
	vkGetPhysicalDeviceProperties2_volkImpl = (PFN_vkGetPhysicalDeviceProperties2)load(context, "vkGetPhysicalDeviceProperties2_volkImpl");
	vkGetPhysicalDeviceQueueFamilyProperties2_volkImpl = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2)load(context, "vkGetPhysicalDeviceQueueFamilyProperties2_volkImpl");
	vkGetPhysicalDeviceSparseImageFormatProperties2_volkImpl = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2)load(context, "vkGetPhysicalDeviceSparseImageFormatProperties2_volkImpl");
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_3)
	vkGetPhysicalDeviceToolProperties_volkImpl = (PFN_vkGetPhysicalDeviceToolProperties)load(context, "vkGetPhysicalDeviceToolProperties_volkImpl");
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_EXT_acquire_drm_display)
	vkAcquireDrmDisplayEXT_volkImpl = (PFN_vkAcquireDrmDisplayEXT)load(context, "vkAcquireDrmDisplayEXT_volkImpl");
	vkGetDrmDisplayEXT_volkImpl = (PFN_vkGetDrmDisplayEXT)load(context, "vkGetDrmDisplayEXT_volkImpl");
#endif /* defined(VK_EXT_acquire_drm_display) */
#if defined(VK_EXT_acquire_xlib_display)
	vkAcquireXlibDisplayEXT_volkImpl = (PFN_vkAcquireXlibDisplayEXT)load(context, "vkAcquireXlibDisplayEXT_volkImpl");
	vkGetRandROutputDisplayEXT_volkImpl = (PFN_vkGetRandROutputDisplayEXT)load(context, "vkGetRandROutputDisplayEXT_volkImpl");
#endif /* defined(VK_EXT_acquire_xlib_display) */
#if defined(VK_EXT_calibrated_timestamps)
	vkGetPhysicalDeviceCalibrateableTimeDomainsEXT_volkImpl = (PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT)load(context, "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT_volkImpl");
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_debug_report)
	vkCreateDebugReportCallbackEXT_volkImpl = (PFN_vkCreateDebugReportCallbackEXT)load(context, "vkCreateDebugReportCallbackEXT_volkImpl");
	vkDebugReportMessageEXT_volkImpl = (PFN_vkDebugReportMessageEXT)load(context, "vkDebugReportMessageEXT_volkImpl");
	vkDestroyDebugReportCallbackEXT_volkImpl = (PFN_vkDestroyDebugReportCallbackEXT)load(context, "vkDestroyDebugReportCallbackEXT_volkImpl");
#endif /* defined(VK_EXT_debug_report) */
#if defined(VK_EXT_debug_utils)
	vkCmdBeginDebugUtilsLabelEXT_volkImpl = (PFN_vkCmdBeginDebugUtilsLabelEXT)load(context, "vkCmdBeginDebugUtilsLabelEXT_volkImpl");
	vkCmdEndDebugUtilsLabelEXT_volkImpl = (PFN_vkCmdEndDebugUtilsLabelEXT)load(context, "vkCmdEndDebugUtilsLabelEXT_volkImpl");
	vkCmdInsertDebugUtilsLabelEXT_volkImpl = (PFN_vkCmdInsertDebugUtilsLabelEXT)load(context, "vkCmdInsertDebugUtilsLabelEXT_volkImpl");
	vkCreateDebugUtilsMessengerEXT_volkImpl = (PFN_vkCreateDebugUtilsMessengerEXT)load(context, "vkCreateDebugUtilsMessengerEXT_volkImpl");
	vkDestroyDebugUtilsMessengerEXT_volkImpl = (PFN_vkDestroyDebugUtilsMessengerEXT)load(context, "vkDestroyDebugUtilsMessengerEXT_volkImpl");
	vkQueueBeginDebugUtilsLabelEXT_volkImpl = (PFN_vkQueueBeginDebugUtilsLabelEXT)load(context, "vkQueueBeginDebugUtilsLabelEXT_volkImpl");
	vkQueueEndDebugUtilsLabelEXT_volkImpl = (PFN_vkQueueEndDebugUtilsLabelEXT)load(context, "vkQueueEndDebugUtilsLabelEXT_volkImpl");
	vkQueueInsertDebugUtilsLabelEXT_volkImpl = (PFN_vkQueueInsertDebugUtilsLabelEXT)load(context, "vkQueueInsertDebugUtilsLabelEXT_volkImpl");
	vkSetDebugUtilsObjectNameEXT_volkImpl = (PFN_vkSetDebugUtilsObjectNameEXT)load(context, "vkSetDebugUtilsObjectNameEXT_volkImpl");
	vkSetDebugUtilsObjectTagEXT_volkImpl = (PFN_vkSetDebugUtilsObjectTagEXT)load(context, "vkSetDebugUtilsObjectTagEXT_volkImpl");
	vkSubmitDebugUtilsMessageEXT_volkImpl = (PFN_vkSubmitDebugUtilsMessageEXT)load(context, "vkSubmitDebugUtilsMessageEXT_volkImpl");
#endif /* defined(VK_EXT_debug_utils) */
#if defined(VK_EXT_direct_mode_display)
	vkReleaseDisplayEXT_volkImpl = (PFN_vkReleaseDisplayEXT)load(context, "vkReleaseDisplayEXT_volkImpl");
#endif /* defined(VK_EXT_direct_mode_display) */
#if defined(VK_EXT_directfb_surface)
	vkCreateDirectFBSurfaceEXT_volkImpl = (PFN_vkCreateDirectFBSurfaceEXT)load(context, "vkCreateDirectFBSurfaceEXT_volkImpl");
	vkGetPhysicalDeviceDirectFBPresentationSupportEXT_volkImpl = (PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT)load(context, "vkGetPhysicalDeviceDirectFBPresentationSupportEXT_volkImpl");
#endif /* defined(VK_EXT_directfb_surface) */
#if defined(VK_EXT_display_surface_counter)
	vkGetPhysicalDeviceSurfaceCapabilities2EXT_volkImpl = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT)load(context, "vkGetPhysicalDeviceSurfaceCapabilities2EXT_volkImpl");
#endif /* defined(VK_EXT_display_surface_counter) */
#if defined(VK_EXT_full_screen_exclusive)
	vkGetPhysicalDeviceSurfacePresentModes2EXT_volkImpl = (PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT)load(context, "vkGetPhysicalDeviceSurfacePresentModes2EXT_volkImpl");
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_headless_surface)
	vkCreateHeadlessSurfaceEXT_volkImpl = (PFN_vkCreateHeadlessSurfaceEXT)load(context, "vkCreateHeadlessSurfaceEXT_volkImpl");
#endif /* defined(VK_EXT_headless_surface) */
#if defined(VK_EXT_metal_surface)
	vkCreateMetalSurfaceEXT_volkImpl = (PFN_vkCreateMetalSurfaceEXT)load(context, "vkCreateMetalSurfaceEXT_volkImpl");
#endif /* defined(VK_EXT_metal_surface) */
#if defined(VK_EXT_sample_locations)
	vkGetPhysicalDeviceMultisamplePropertiesEXT_volkImpl = (PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT)load(context, "vkGetPhysicalDeviceMultisamplePropertiesEXT_volkImpl");
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_tooling_info)
	vkGetPhysicalDeviceToolPropertiesEXT_volkImpl = (PFN_vkGetPhysicalDeviceToolPropertiesEXT)load(context, "vkGetPhysicalDeviceToolPropertiesEXT_volkImpl");
#endif /* defined(VK_EXT_tooling_info) */
#if defined(VK_FUCHSIA_imagepipe_surface)
	vkCreateImagePipeSurfaceFUCHSIA_volkImpl = (PFN_vkCreateImagePipeSurfaceFUCHSIA)load(context, "vkCreateImagePipeSurfaceFUCHSIA_volkImpl");
#endif /* defined(VK_FUCHSIA_imagepipe_surface) */
#if defined(VK_GGP_stream_descriptor_surface)
	vkCreateStreamDescriptorSurfaceGGP_volkImpl = (PFN_vkCreateStreamDescriptorSurfaceGGP)load(context, "vkCreateStreamDescriptorSurfaceGGP_volkImpl");
#endif /* defined(VK_GGP_stream_descriptor_surface) */
#if defined(VK_KHR_android_surface)
	vkCreateAndroidSurfaceKHR_volkImpl = (PFN_vkCreateAndroidSurfaceKHR)load(context, "vkCreateAndroidSurfaceKHR_volkImpl");
#endif /* defined(VK_KHR_android_surface) */
#if defined(VK_KHR_calibrated_timestamps)
	vkGetPhysicalDeviceCalibrateableTimeDomainsKHR_volkImpl = (PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR)load(context, "vkGetPhysicalDeviceCalibrateableTimeDomainsKHR_volkImpl");
#endif /* defined(VK_KHR_calibrated_timestamps) */
#if defined(VK_KHR_cooperative_matrix)
	vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR)load(context, "vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR_volkImpl");
#endif /* defined(VK_KHR_cooperative_matrix) */
#if defined(VK_KHR_device_group_creation)
	vkEnumeratePhysicalDeviceGroupsKHR_volkImpl = (PFN_vkEnumeratePhysicalDeviceGroupsKHR)load(context, "vkEnumeratePhysicalDeviceGroupsKHR_volkImpl");
#endif /* defined(VK_KHR_device_group_creation) */
#if defined(VK_KHR_display)
	vkCreateDisplayModeKHR_volkImpl = (PFN_vkCreateDisplayModeKHR)load(context, "vkCreateDisplayModeKHR_volkImpl");
	vkCreateDisplayPlaneSurfaceKHR_volkImpl = (PFN_vkCreateDisplayPlaneSurfaceKHR)load(context, "vkCreateDisplayPlaneSurfaceKHR_volkImpl");
	vkGetDisplayModePropertiesKHR_volkImpl = (PFN_vkGetDisplayModePropertiesKHR)load(context, "vkGetDisplayModePropertiesKHR_volkImpl");
	vkGetDisplayPlaneCapabilitiesKHR_volkImpl = (PFN_vkGetDisplayPlaneCapabilitiesKHR)load(context, "vkGetDisplayPlaneCapabilitiesKHR_volkImpl");
	vkGetDisplayPlaneSupportedDisplaysKHR_volkImpl = (PFN_vkGetDisplayPlaneSupportedDisplaysKHR)load(context, "vkGetDisplayPlaneSupportedDisplaysKHR_volkImpl");
	vkGetPhysicalDeviceDisplayPlanePropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR)load(context, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR_volkImpl");
	vkGetPhysicalDeviceDisplayPropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceDisplayPropertiesKHR)load(context, "vkGetPhysicalDeviceDisplayPropertiesKHR_volkImpl");
#endif /* defined(VK_KHR_display) */
#if defined(VK_KHR_external_fence_capabilities)
	vkGetPhysicalDeviceExternalFencePropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR)load(context, "vkGetPhysicalDeviceExternalFencePropertiesKHR_volkImpl");
#endif /* defined(VK_KHR_external_fence_capabilities) */
#if defined(VK_KHR_external_memory_capabilities)
	vkGetPhysicalDeviceExternalBufferPropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR)load(context, "vkGetPhysicalDeviceExternalBufferPropertiesKHR_volkImpl");
#endif /* defined(VK_KHR_external_memory_capabilities) */
#if defined(VK_KHR_external_semaphore_capabilities)
	vkGetPhysicalDeviceExternalSemaphorePropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR)load(context, "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR_volkImpl");
#endif /* defined(VK_KHR_external_semaphore_capabilities) */
#if defined(VK_KHR_fragment_shading_rate)
	vkGetPhysicalDeviceFragmentShadingRatesKHR_volkImpl = (PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR)load(context, "vkGetPhysicalDeviceFragmentShadingRatesKHR_volkImpl");
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_display_properties2)
	vkGetDisplayModeProperties2KHR_volkImpl = (PFN_vkGetDisplayModeProperties2KHR)load(context, "vkGetDisplayModeProperties2KHR_volkImpl");
	vkGetDisplayPlaneCapabilities2KHR_volkImpl = (PFN_vkGetDisplayPlaneCapabilities2KHR)load(context, "vkGetDisplayPlaneCapabilities2KHR_volkImpl");
	vkGetPhysicalDeviceDisplayPlaneProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR)load(context, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR_volkImpl");
	vkGetPhysicalDeviceDisplayProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceDisplayProperties2KHR)load(context, "vkGetPhysicalDeviceDisplayProperties2KHR_volkImpl");
#endif /* defined(VK_KHR_get_display_properties2) */
#if defined(VK_KHR_get_physical_device_properties2)
	vkGetPhysicalDeviceFeatures2KHR_volkImpl = (PFN_vkGetPhysicalDeviceFeatures2KHR)load(context, "vkGetPhysicalDeviceFeatures2KHR_volkImpl");
	vkGetPhysicalDeviceFormatProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceFormatProperties2KHR)load(context, "vkGetPhysicalDeviceFormatProperties2KHR_volkImpl");
	vkGetPhysicalDeviceImageFormatProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceImageFormatProperties2KHR)load(context, "vkGetPhysicalDeviceImageFormatProperties2KHR_volkImpl");
	vkGetPhysicalDeviceMemoryProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceMemoryProperties2KHR)load(context, "vkGetPhysicalDeviceMemoryProperties2KHR_volkImpl");
	vkGetPhysicalDeviceProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceProperties2KHR)load(context, "vkGetPhysicalDeviceProperties2KHR_volkImpl");
	vkGetPhysicalDeviceQueueFamilyProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR)load(context, "vkGetPhysicalDeviceQueueFamilyProperties2KHR_volkImpl");
	vkGetPhysicalDeviceSparseImageFormatProperties2KHR_volkImpl = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR)load(context, "vkGetPhysicalDeviceSparseImageFormatProperties2KHR_volkImpl");
#endif /* defined(VK_KHR_get_physical_device_properties2) */
#if defined(VK_KHR_get_surface_capabilities2)
	vkGetPhysicalDeviceSurfaceCapabilities2KHR_volkImpl = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR)load(context, "vkGetPhysicalDeviceSurfaceCapabilities2KHR_volkImpl");
	vkGetPhysicalDeviceSurfaceFormats2KHR_volkImpl = (PFN_vkGetPhysicalDeviceSurfaceFormats2KHR)load(context, "vkGetPhysicalDeviceSurfaceFormats2KHR_volkImpl");
#endif /* defined(VK_KHR_get_surface_capabilities2) */
#if defined(VK_KHR_performance_query)
	vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR_volkImpl = (PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR)load(context, "vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR_volkImpl");
	vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR_volkImpl = (PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR)load(context, "vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR_volkImpl");
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_surface)
	vkDestroySurfaceKHR_volkImpl = (PFN_vkDestroySurfaceKHR)load(context, "vkDestroySurfaceKHR_volkImpl");
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)load(context, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR_volkImpl");
	vkGetPhysicalDeviceSurfaceFormatsKHR_volkImpl = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)load(context, "vkGetPhysicalDeviceSurfaceFormatsKHR_volkImpl");
	vkGetPhysicalDeviceSurfacePresentModesKHR_volkImpl = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)load(context, "vkGetPhysicalDeviceSurfacePresentModesKHR_volkImpl");
	vkGetPhysicalDeviceSurfaceSupportKHR_volkImpl = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)load(context, "vkGetPhysicalDeviceSurfaceSupportKHR_volkImpl");
#endif /* defined(VK_KHR_surface) */
#if defined(VK_KHR_video_encode_queue)
	vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR)load(context, "vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR_volkImpl");
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
	vkGetPhysicalDeviceVideoCapabilitiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR)load(context, "vkGetPhysicalDeviceVideoCapabilitiesKHR_volkImpl");
	vkGetPhysicalDeviceVideoFormatPropertiesKHR_volkImpl = (PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR)load(context, "vkGetPhysicalDeviceVideoFormatPropertiesKHR_volkImpl");
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_KHR_wayland_surface)
	vkCreateWaylandSurfaceKHR_volkImpl = (PFN_vkCreateWaylandSurfaceKHR)load(context, "vkCreateWaylandSurfaceKHR_volkImpl");
	vkGetPhysicalDeviceWaylandPresentationSupportKHR_volkImpl = (PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR)load(context, "vkGetPhysicalDeviceWaylandPresentationSupportKHR_volkImpl");
#endif /* defined(VK_KHR_wayland_surface) */
#if defined(VK_KHR_win32_surface)
	vkCreateWin32SurfaceKHR_volkImpl = (PFN_vkCreateWin32SurfaceKHR)load(context, "vkCreateWin32SurfaceKHR_volkImpl");
	vkGetPhysicalDeviceWin32PresentationSupportKHR_volkImpl = (PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)load(context, "vkGetPhysicalDeviceWin32PresentationSupportKHR_volkImpl");
#endif /* defined(VK_KHR_win32_surface) */
#if defined(VK_KHR_xcb_surface)
	vkCreateXcbSurfaceKHR_volkImpl = (PFN_vkCreateXcbSurfaceKHR)load(context, "vkCreateXcbSurfaceKHR_volkImpl");
	vkGetPhysicalDeviceXcbPresentationSupportKHR_volkImpl = (PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)load(context, "vkGetPhysicalDeviceXcbPresentationSupportKHR_volkImpl");
#endif /* defined(VK_KHR_xcb_surface) */
#if defined(VK_KHR_xlib_surface)
	vkCreateXlibSurfaceKHR_volkImpl = (PFN_vkCreateXlibSurfaceKHR)load(context, "vkCreateXlibSurfaceKHR_volkImpl");
	vkGetPhysicalDeviceXlibPresentationSupportKHR_volkImpl = (PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)load(context, "vkGetPhysicalDeviceXlibPresentationSupportKHR_volkImpl");
#endif /* defined(VK_KHR_xlib_surface) */
#if defined(VK_MVK_ios_surface)
	vkCreateIOSSurfaceMVK_volkImpl = (PFN_vkCreateIOSSurfaceMVK)load(context, "vkCreateIOSSurfaceMVK_volkImpl");
#endif /* defined(VK_MVK_ios_surface) */
#if defined(VK_MVK_macos_surface)
	vkCreateMacOSSurfaceMVK_volkImpl = (PFN_vkCreateMacOSSurfaceMVK)load(context, "vkCreateMacOSSurfaceMVK_volkImpl");
#endif /* defined(VK_MVK_macos_surface) */
#if defined(VK_NN_vi_surface)
	vkCreateViSurfaceNN_volkImpl = (PFN_vkCreateViSurfaceNN)load(context, "vkCreateViSurfaceNN_volkImpl");
#endif /* defined(VK_NN_vi_surface) */
#if defined(VK_NV_acquire_winrt_display)
	vkAcquireWinrtDisplayNV_volkImpl = (PFN_vkAcquireWinrtDisplayNV)load(context, "vkAcquireWinrtDisplayNV_volkImpl");
	vkGetWinrtDisplayNV_volkImpl = (PFN_vkGetWinrtDisplayNV)load(context, "vkGetWinrtDisplayNV_volkImpl");
#endif /* defined(VK_NV_acquire_winrt_display) */
#if defined(VK_NV_cooperative_matrix)
	vkGetPhysicalDeviceCooperativeMatrixPropertiesNV_volkImpl = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV)load(context, "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV_volkImpl");
#endif /* defined(VK_NV_cooperative_matrix) */
#if defined(VK_NV_coverage_reduction_mode)
	vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV_volkImpl = (PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV)load(context, "vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV_volkImpl");
#endif /* defined(VK_NV_coverage_reduction_mode) */
#if defined(VK_NV_external_memory_capabilities)
	vkGetPhysicalDeviceExternalImageFormatPropertiesNV_volkImpl = (PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV)load(context, "vkGetPhysicalDeviceExternalImageFormatPropertiesNV_volkImpl");
#endif /* defined(VK_NV_external_memory_capabilities) */
#if defined(VK_NV_optical_flow)
	vkGetPhysicalDeviceOpticalFlowImageFormatsNV_volkImpl = (PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV)load(context, "vkGetPhysicalDeviceOpticalFlowImageFormatsNV_volkImpl");
#endif /* defined(VK_NV_optical_flow) */
#if defined(VK_QNX_screen_surface)
	vkCreateScreenSurfaceQNX_volkImpl = (PFN_vkCreateScreenSurfaceQNX)load(context, "vkCreateScreenSurfaceQNX_volkImpl");
	vkGetPhysicalDeviceScreenPresentationSupportQNX_volkImpl = (PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX)load(context, "vkGetPhysicalDeviceScreenPresentationSupportQNX_volkImpl");
#endif /* defined(VK_QNX_screen_surface) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	vkGetPhysicalDevicePresentRectanglesKHR_volkImpl = (PFN_vkGetPhysicalDevicePresentRectanglesKHR)load(context, "vkGetPhysicalDevicePresentRectanglesKHR_volkImpl");
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
	/* VOLK_GENERATE_LOAD_INSTANCE */
}

static void volkGenLoadDevice(void* context, PFN_vkVoidFunction (*load)(void*, const char*))
{
	/* VOLK_GENERATE_LOAD_DEVICE */
#if defined(VK_VERSION_1_0)
	vkAllocateCommandBuffers_volkImpl = (PFN_vkAllocateCommandBuffers)load(context, "vkAllocateCommandBuffers_volkImpl");
	vkAllocateDescriptorSets_volkImpl = (PFN_vkAllocateDescriptorSets)load(context, "vkAllocateDescriptorSets_volkImpl");
	vkAllocateMemory_volkImpl = (PFN_vkAllocateMemory)load(context, "vkAllocateMemory_volkImpl");
	vkBeginCommandBuffer_volkImpl = (PFN_vkBeginCommandBuffer)load(context, "vkBeginCommandBuffer_volkImpl");
	vkBindBufferMemory_volkImpl = (PFN_vkBindBufferMemory)load(context, "vkBindBufferMemory_volkImpl");
	vkBindImageMemory_volkImpl = (PFN_vkBindImageMemory)load(context, "vkBindImageMemory_volkImpl");
	vkCmdBeginQuery_volkImpl = (PFN_vkCmdBeginQuery)load(context, "vkCmdBeginQuery_volkImpl");
	vkCmdBeginRenderPass_volkImpl = (PFN_vkCmdBeginRenderPass)load(context, "vkCmdBeginRenderPass_volkImpl");
	vkCmdBindDescriptorSets_volkImpl = (PFN_vkCmdBindDescriptorSets)load(context, "vkCmdBindDescriptorSets_volkImpl");
	vkCmdBindIndexBuffer_volkImpl = (PFN_vkCmdBindIndexBuffer)load(context, "vkCmdBindIndexBuffer_volkImpl");
	vkCmdBindPipeline_volkImpl = (PFN_vkCmdBindPipeline)load(context, "vkCmdBindPipeline_volkImpl");
	vkCmdBindVertexBuffers_volkImpl = (PFN_vkCmdBindVertexBuffers)load(context, "vkCmdBindVertexBuffers_volkImpl");
	vkCmdBlitImage_volkImpl = (PFN_vkCmdBlitImage)load(context, "vkCmdBlitImage_volkImpl");
	vkCmdClearAttachments_volkImpl = (PFN_vkCmdClearAttachments)load(context, "vkCmdClearAttachments_volkImpl");
	vkCmdClearColorImage_volkImpl = (PFN_vkCmdClearColorImage)load(context, "vkCmdClearColorImage_volkImpl");
	vkCmdClearDepthStencilImage_volkImpl = (PFN_vkCmdClearDepthStencilImage)load(context, "vkCmdClearDepthStencilImage_volkImpl");
	vkCmdCopyBuffer_volkImpl = (PFN_vkCmdCopyBuffer)load(context, "vkCmdCopyBuffer_volkImpl");
	vkCmdCopyBufferToImage_volkImpl = (PFN_vkCmdCopyBufferToImage)load(context, "vkCmdCopyBufferToImage_volkImpl");
	vkCmdCopyImage_volkImpl = (PFN_vkCmdCopyImage)load(context, "vkCmdCopyImage_volkImpl");
	vkCmdCopyImageToBuffer_volkImpl = (PFN_vkCmdCopyImageToBuffer)load(context, "vkCmdCopyImageToBuffer_volkImpl");
	vkCmdCopyQueryPoolResults_volkImpl = (PFN_vkCmdCopyQueryPoolResults)load(context, "vkCmdCopyQueryPoolResults_volkImpl");
	vkCmdDispatch_volkImpl = (PFN_vkCmdDispatch)load(context, "vkCmdDispatch_volkImpl");
	vkCmdDispatchIndirect_volkImpl = (PFN_vkCmdDispatchIndirect)load(context, "vkCmdDispatchIndirect_volkImpl");
	vkCmdDraw_volkImpl = (PFN_vkCmdDraw)load(context, "vkCmdDraw_volkImpl");
	vkCmdDrawIndexed_volkImpl = (PFN_vkCmdDrawIndexed)load(context, "vkCmdDrawIndexed_volkImpl");
	vkCmdDrawIndexedIndirect_volkImpl = (PFN_vkCmdDrawIndexedIndirect)load(context, "vkCmdDrawIndexedIndirect_volkImpl");
	vkCmdDrawIndirect_volkImpl = (PFN_vkCmdDrawIndirect)load(context, "vkCmdDrawIndirect_volkImpl");
	vkCmdEndQuery_volkImpl = (PFN_vkCmdEndQuery)load(context, "vkCmdEndQuery_volkImpl");
	vkCmdEndRenderPass_volkImpl = (PFN_vkCmdEndRenderPass)load(context, "vkCmdEndRenderPass_volkImpl");
	vkCmdExecuteCommands_volkImpl = (PFN_vkCmdExecuteCommands)load(context, "vkCmdExecuteCommands_volkImpl");
	vkCmdFillBuffer_volkImpl = (PFN_vkCmdFillBuffer)load(context, "vkCmdFillBuffer_volkImpl");
	vkCmdNextSubpass_volkImpl = (PFN_vkCmdNextSubpass)load(context, "vkCmdNextSubpass_volkImpl");
	vkCmdPipelineBarrier_volkImpl = (PFN_vkCmdPipelineBarrier)load(context, "vkCmdPipelineBarrier_volkImpl");
	vkCmdPushConstants_volkImpl = (PFN_vkCmdPushConstants)load(context, "vkCmdPushConstants_volkImpl");
	vkCmdResetEvent_volkImpl = (PFN_vkCmdResetEvent)load(context, "vkCmdResetEvent_volkImpl");
	vkCmdResetQueryPool_volkImpl = (PFN_vkCmdResetQueryPool)load(context, "vkCmdResetQueryPool_volkImpl");
	vkCmdResolveImage_volkImpl = (PFN_vkCmdResolveImage)load(context, "vkCmdResolveImage_volkImpl");
	vkCmdSetBlendConstants_volkImpl = (PFN_vkCmdSetBlendConstants)load(context, "vkCmdSetBlendConstants_volkImpl");
	vkCmdSetDepthBias_volkImpl = (PFN_vkCmdSetDepthBias)load(context, "vkCmdSetDepthBias_volkImpl");
	vkCmdSetDepthBounds_volkImpl = (PFN_vkCmdSetDepthBounds)load(context, "vkCmdSetDepthBounds_volkImpl");
	vkCmdSetEvent_volkImpl = (PFN_vkCmdSetEvent)load(context, "vkCmdSetEvent_volkImpl");
	vkCmdSetLineWidth_volkImpl = (PFN_vkCmdSetLineWidth)load(context, "vkCmdSetLineWidth_volkImpl");
	vkCmdSetScissor_volkImpl = (PFN_vkCmdSetScissor)load(context, "vkCmdSetScissor_volkImpl");
	vkCmdSetStencilCompareMask_volkImpl = (PFN_vkCmdSetStencilCompareMask)load(context, "vkCmdSetStencilCompareMask_volkImpl");
	vkCmdSetStencilReference_volkImpl = (PFN_vkCmdSetStencilReference)load(context, "vkCmdSetStencilReference_volkImpl");
	vkCmdSetStencilWriteMask_volkImpl = (PFN_vkCmdSetStencilWriteMask)load(context, "vkCmdSetStencilWriteMask_volkImpl");
	vkCmdSetViewport_volkImpl = (PFN_vkCmdSetViewport)load(context, "vkCmdSetViewport_volkImpl");
	vkCmdUpdateBuffer_volkImpl = (PFN_vkCmdUpdateBuffer)load(context, "vkCmdUpdateBuffer_volkImpl");
	vkCmdWaitEvents_volkImpl = (PFN_vkCmdWaitEvents)load(context, "vkCmdWaitEvents_volkImpl");
	vkCmdWriteTimestamp_volkImpl = (PFN_vkCmdWriteTimestamp)load(context, "vkCmdWriteTimestamp_volkImpl");
	vkCreateBuffer_volkImpl = (PFN_vkCreateBuffer)load(context, "vkCreateBuffer_volkImpl");
	vkCreateBufferView_volkImpl = (PFN_vkCreateBufferView)load(context, "vkCreateBufferView_volkImpl");
	vkCreateCommandPool_volkImpl = (PFN_vkCreateCommandPool)load(context, "vkCreateCommandPool_volkImpl");
	vkCreateComputePipelines_volkImpl = (PFN_vkCreateComputePipelines)load(context, "vkCreateComputePipelines_volkImpl");
	vkCreateDescriptorPool_volkImpl = (PFN_vkCreateDescriptorPool)load(context, "vkCreateDescriptorPool_volkImpl");
	vkCreateDescriptorSetLayout_volkImpl = (PFN_vkCreateDescriptorSetLayout)load(context, "vkCreateDescriptorSetLayout_volkImpl");
	vkCreateEvent_volkImpl = (PFN_vkCreateEvent)load(context, "vkCreateEvent_volkImpl");
	vkCreateFence_volkImpl = (PFN_vkCreateFence)load(context, "vkCreateFence_volkImpl");
	vkCreateFramebuffer_volkImpl = (PFN_vkCreateFramebuffer)load(context, "vkCreateFramebuffer_volkImpl");
	vkCreateGraphicsPipelines_volkImpl = (PFN_vkCreateGraphicsPipelines)load(context, "vkCreateGraphicsPipelines_volkImpl");
	vkCreateImage_volkImpl = (PFN_vkCreateImage)load(context, "vkCreateImage_volkImpl");
	vkCreateImageView_volkImpl = (PFN_vkCreateImageView)load(context, "vkCreateImageView_volkImpl");
	vkCreatePipelineCache_volkImpl = (PFN_vkCreatePipelineCache)load(context, "vkCreatePipelineCache_volkImpl");
	vkCreatePipelineLayout_volkImpl = (PFN_vkCreatePipelineLayout)load(context, "vkCreatePipelineLayout_volkImpl");
	vkCreateQueryPool_volkImpl = (PFN_vkCreateQueryPool)load(context, "vkCreateQueryPool_volkImpl");
	vkCreateRenderPass_volkImpl = (PFN_vkCreateRenderPass)load(context, "vkCreateRenderPass_volkImpl");
	vkCreateSampler_volkImpl = (PFN_vkCreateSampler)load(context, "vkCreateSampler_volkImpl");
	vkCreateSemaphore_volkImpl = (PFN_vkCreateSemaphore)load(context, "vkCreateSemaphore_volkImpl");
	vkCreateShaderModule_volkImpl = (PFN_vkCreateShaderModule)load(context, "vkCreateShaderModule_volkImpl");
	vkDestroyBuffer_volkImpl = (PFN_vkDestroyBuffer)load(context, "vkDestroyBuffer_volkImpl");
	vkDestroyBufferView_volkImpl = (PFN_vkDestroyBufferView)load(context, "vkDestroyBufferView_volkImpl");
	vkDestroyCommandPool_volkImpl = (PFN_vkDestroyCommandPool)load(context, "vkDestroyCommandPool_volkImpl");
	vkDestroyDescriptorPool_volkImpl = (PFN_vkDestroyDescriptorPool)load(context, "vkDestroyDescriptorPool_volkImpl");
	vkDestroyDescriptorSetLayout_volkImpl = (PFN_vkDestroyDescriptorSetLayout)load(context, "vkDestroyDescriptorSetLayout_volkImpl");
	vkDestroyDevice_volkImpl = (PFN_vkDestroyDevice)load(context, "vkDestroyDevice_volkImpl");
	vkDestroyEvent_volkImpl = (PFN_vkDestroyEvent)load(context, "vkDestroyEvent_volkImpl");
	vkDestroyFence_volkImpl = (PFN_vkDestroyFence)load(context, "vkDestroyFence_volkImpl");
	vkDestroyFramebuffer_volkImpl = (PFN_vkDestroyFramebuffer)load(context, "vkDestroyFramebuffer_volkImpl");
	vkDestroyImage_volkImpl = (PFN_vkDestroyImage)load(context, "vkDestroyImage_volkImpl");
	vkDestroyImageView_volkImpl = (PFN_vkDestroyImageView)load(context, "vkDestroyImageView_volkImpl");
	vkDestroyPipeline_volkImpl = (PFN_vkDestroyPipeline)load(context, "vkDestroyPipeline_volkImpl");
	vkDestroyPipelineCache_volkImpl = (PFN_vkDestroyPipelineCache)load(context, "vkDestroyPipelineCache_volkImpl");
	vkDestroyPipelineLayout_volkImpl = (PFN_vkDestroyPipelineLayout)load(context, "vkDestroyPipelineLayout_volkImpl");
	vkDestroyQueryPool_volkImpl = (PFN_vkDestroyQueryPool)load(context, "vkDestroyQueryPool_volkImpl");
	vkDestroyRenderPass_volkImpl = (PFN_vkDestroyRenderPass)load(context, "vkDestroyRenderPass_volkImpl");
	vkDestroySampler_volkImpl = (PFN_vkDestroySampler)load(context, "vkDestroySampler_volkImpl");
	vkDestroySemaphore_volkImpl = (PFN_vkDestroySemaphore)load(context, "vkDestroySemaphore_volkImpl");
	vkDestroyShaderModule_volkImpl = (PFN_vkDestroyShaderModule)load(context, "vkDestroyShaderModule_volkImpl");
	vkDeviceWaitIdle_volkImpl = (PFN_vkDeviceWaitIdle)load(context, "vkDeviceWaitIdle_volkImpl");
	vkEndCommandBuffer_volkImpl = (PFN_vkEndCommandBuffer)load(context, "vkEndCommandBuffer_volkImpl");
	vkFlushMappedMemoryRanges_volkImpl = (PFN_vkFlushMappedMemoryRanges)load(context, "vkFlushMappedMemoryRanges_volkImpl");
	vkFreeCommandBuffers_volkImpl = (PFN_vkFreeCommandBuffers)load(context, "vkFreeCommandBuffers_volkImpl");
	vkFreeDescriptorSets_volkImpl = (PFN_vkFreeDescriptorSets)load(context, "vkFreeDescriptorSets_volkImpl");
	vkFreeMemory_volkImpl = (PFN_vkFreeMemory)load(context, "vkFreeMemory_volkImpl");
	vkGetBufferMemoryRequirements_volkImpl = (PFN_vkGetBufferMemoryRequirements)load(context, "vkGetBufferMemoryRequirements_volkImpl");
	vkGetDeviceMemoryCommitment_volkImpl = (PFN_vkGetDeviceMemoryCommitment)load(context, "vkGetDeviceMemoryCommitment_volkImpl");
	vkGetDeviceQueue_volkImpl = (PFN_vkGetDeviceQueue)load(context, "vkGetDeviceQueue_volkImpl");
	vkGetEventStatus_volkImpl = (PFN_vkGetEventStatus)load(context, "vkGetEventStatus_volkImpl");
	vkGetFenceStatus_volkImpl = (PFN_vkGetFenceStatus)load(context, "vkGetFenceStatus_volkImpl");
	vkGetImageMemoryRequirements_volkImpl = (PFN_vkGetImageMemoryRequirements)load(context, "vkGetImageMemoryRequirements_volkImpl");
	vkGetImageSparseMemoryRequirements_volkImpl = (PFN_vkGetImageSparseMemoryRequirements)load(context, "vkGetImageSparseMemoryRequirements_volkImpl");
	vkGetImageSubresourceLayout_volkImpl = (PFN_vkGetImageSubresourceLayout)load(context, "vkGetImageSubresourceLayout_volkImpl");
	vkGetPipelineCacheData_volkImpl = (PFN_vkGetPipelineCacheData)load(context, "vkGetPipelineCacheData_volkImpl");
	vkGetQueryPoolResults_volkImpl = (PFN_vkGetQueryPoolResults)load(context, "vkGetQueryPoolResults_volkImpl");
	vkGetRenderAreaGranularity_volkImpl = (PFN_vkGetRenderAreaGranularity)load(context, "vkGetRenderAreaGranularity_volkImpl");
	vkInvalidateMappedMemoryRanges_volkImpl = (PFN_vkInvalidateMappedMemoryRanges)load(context, "vkInvalidateMappedMemoryRanges_volkImpl");
	vkMapMemory_volkImpl = (PFN_vkMapMemory)load(context, "vkMapMemory_volkImpl");
	vkMergePipelineCaches_volkImpl = (PFN_vkMergePipelineCaches)load(context, "vkMergePipelineCaches_volkImpl");
	vkQueueBindSparse_volkImpl = (PFN_vkQueueBindSparse)load(context, "vkQueueBindSparse_volkImpl");
	vkQueueSubmit_volkImpl = (PFN_vkQueueSubmit)load(context, "vkQueueSubmit_volkImpl");
	vkQueueWaitIdle_volkImpl = (PFN_vkQueueWaitIdle)load(context, "vkQueueWaitIdle_volkImpl");
	vkResetCommandBuffer_volkImpl = (PFN_vkResetCommandBuffer)load(context, "vkResetCommandBuffer_volkImpl");
	vkResetCommandPool_volkImpl = (PFN_vkResetCommandPool)load(context, "vkResetCommandPool_volkImpl");
	vkResetDescriptorPool_volkImpl = (PFN_vkResetDescriptorPool)load(context, "vkResetDescriptorPool_volkImpl");
	vkResetEvent_volkImpl = (PFN_vkResetEvent)load(context, "vkResetEvent_volkImpl");
	vkResetFences_volkImpl = (PFN_vkResetFences)load(context, "vkResetFences_volkImpl");
	vkSetEvent_volkImpl = (PFN_vkSetEvent)load(context, "vkSetEvent_volkImpl");
	vkUnmapMemory_volkImpl = (PFN_vkUnmapMemory)load(context, "vkUnmapMemory_volkImpl");
	vkUpdateDescriptorSets_volkImpl = (PFN_vkUpdateDescriptorSets)load(context, "vkUpdateDescriptorSets_volkImpl");
	vkWaitForFences_volkImpl = (PFN_vkWaitForFences)load(context, "vkWaitForFences_volkImpl");
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
	vkBindBufferMemory2_volkImpl = (PFN_vkBindBufferMemory2)load(context, "vkBindBufferMemory2_volkImpl");
	vkBindImageMemory2_volkImpl = (PFN_vkBindImageMemory2)load(context, "vkBindImageMemory2_volkImpl");
	vkCmdDispatchBase_volkImpl = (PFN_vkCmdDispatchBase)load(context, "vkCmdDispatchBase_volkImpl");
	vkCmdSetDeviceMask_volkImpl = (PFN_vkCmdSetDeviceMask)load(context, "vkCmdSetDeviceMask_volkImpl");
	vkCreateDescriptorUpdateTemplate_volkImpl = (PFN_vkCreateDescriptorUpdateTemplate)load(context, "vkCreateDescriptorUpdateTemplate_volkImpl");
	vkCreateSamplerYcbcrConversion_volkImpl = (PFN_vkCreateSamplerYcbcrConversion)load(context, "vkCreateSamplerYcbcrConversion_volkImpl");
	vkDestroyDescriptorUpdateTemplate_volkImpl = (PFN_vkDestroyDescriptorUpdateTemplate)load(context, "vkDestroyDescriptorUpdateTemplate_volkImpl");
	vkDestroySamplerYcbcrConversion_volkImpl = (PFN_vkDestroySamplerYcbcrConversion)load(context, "vkDestroySamplerYcbcrConversion_volkImpl");
	vkGetBufferMemoryRequirements2_volkImpl = (PFN_vkGetBufferMemoryRequirements2)load(context, "vkGetBufferMemoryRequirements2_volkImpl");
	vkGetDescriptorSetLayoutSupport_volkImpl = (PFN_vkGetDescriptorSetLayoutSupport)load(context, "vkGetDescriptorSetLayoutSupport_volkImpl");
	vkGetDeviceGroupPeerMemoryFeatures_volkImpl = (PFN_vkGetDeviceGroupPeerMemoryFeatures)load(context, "vkGetDeviceGroupPeerMemoryFeatures_volkImpl");
	vkGetDeviceQueue2_volkImpl = (PFN_vkGetDeviceQueue2)load(context, "vkGetDeviceQueue2_volkImpl");
	vkGetImageMemoryRequirements2_volkImpl = (PFN_vkGetImageMemoryRequirements2)load(context, "vkGetImageMemoryRequirements2_volkImpl");
	vkGetImageSparseMemoryRequirements2_volkImpl = (PFN_vkGetImageSparseMemoryRequirements2)load(context, "vkGetImageSparseMemoryRequirements2_volkImpl");
	vkTrimCommandPool_volkImpl = (PFN_vkTrimCommandPool)load(context, "vkTrimCommandPool_volkImpl");
	vkUpdateDescriptorSetWithTemplate_volkImpl = (PFN_vkUpdateDescriptorSetWithTemplate)load(context, "vkUpdateDescriptorSetWithTemplate_volkImpl");
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_2)
	vkCmdBeginRenderPass2_volkImpl = (PFN_vkCmdBeginRenderPass2)load(context, "vkCmdBeginRenderPass2_volkImpl");
	vkCmdDrawIndexedIndirectCount_volkImpl = (PFN_vkCmdDrawIndexedIndirectCount)load(context, "vkCmdDrawIndexedIndirectCount_volkImpl");
	vkCmdDrawIndirectCount_volkImpl = (PFN_vkCmdDrawIndirectCount)load(context, "vkCmdDrawIndirectCount_volkImpl");
	vkCmdEndRenderPass2_volkImpl = (PFN_vkCmdEndRenderPass2)load(context, "vkCmdEndRenderPass2_volkImpl");
	vkCmdNextSubpass2_volkImpl = (PFN_vkCmdNextSubpass2)load(context, "vkCmdNextSubpass2_volkImpl");
	vkCreateRenderPass2_volkImpl = (PFN_vkCreateRenderPass2)load(context, "vkCreateRenderPass2_volkImpl");
	vkGetBufferDeviceAddress_volkImpl = (PFN_vkGetBufferDeviceAddress)load(context, "vkGetBufferDeviceAddress_volkImpl");
	vkGetBufferOpaqueCaptureAddress_volkImpl = (PFN_vkGetBufferOpaqueCaptureAddress)load(context, "vkGetBufferOpaqueCaptureAddress_volkImpl");
	vkGetDeviceMemoryOpaqueCaptureAddress_volkImpl = (PFN_vkGetDeviceMemoryOpaqueCaptureAddress)load(context, "vkGetDeviceMemoryOpaqueCaptureAddress_volkImpl");
	vkGetSemaphoreCounterValue_volkImpl = (PFN_vkGetSemaphoreCounterValue)load(context, "vkGetSemaphoreCounterValue_volkImpl");
	vkResetQueryPool_volkImpl = (PFN_vkResetQueryPool)load(context, "vkResetQueryPool_volkImpl");
	vkSignalSemaphore_volkImpl = (PFN_vkSignalSemaphore)load(context, "vkSignalSemaphore_volkImpl");
	vkWaitSemaphores_volkImpl = (PFN_vkWaitSemaphores)load(context, "vkWaitSemaphores_volkImpl");
#endif /* defined(VK_VERSION_1_2) */
#if defined(VK_VERSION_1_3)
	vkCmdBeginRendering_volkImpl = (PFN_vkCmdBeginRendering)load(context, "vkCmdBeginRendering_volkImpl");
	vkCmdBindVertexBuffers2_volkImpl = (PFN_vkCmdBindVertexBuffers2)load(context, "vkCmdBindVertexBuffers2_volkImpl");
	vkCmdBlitImage2_volkImpl = (PFN_vkCmdBlitImage2)load(context, "vkCmdBlitImage2_volkImpl");
	vkCmdCopyBuffer2_volkImpl = (PFN_vkCmdCopyBuffer2)load(context, "vkCmdCopyBuffer2_volkImpl");
	vkCmdCopyBufferToImage2_volkImpl = (PFN_vkCmdCopyBufferToImage2)load(context, "vkCmdCopyBufferToImage2_volkImpl");
	vkCmdCopyImage2_volkImpl = (PFN_vkCmdCopyImage2)load(context, "vkCmdCopyImage2_volkImpl");
	vkCmdCopyImageToBuffer2_volkImpl = (PFN_vkCmdCopyImageToBuffer2)load(context, "vkCmdCopyImageToBuffer2_volkImpl");
	vkCmdEndRendering_volkImpl = (PFN_vkCmdEndRendering)load(context, "vkCmdEndRendering_volkImpl");
	vkCmdPipelineBarrier2_volkImpl = (PFN_vkCmdPipelineBarrier2)load(context, "vkCmdPipelineBarrier2_volkImpl");
	vkCmdResetEvent2_volkImpl = (PFN_vkCmdResetEvent2)load(context, "vkCmdResetEvent2_volkImpl");
	vkCmdResolveImage2_volkImpl = (PFN_vkCmdResolveImage2)load(context, "vkCmdResolveImage2_volkImpl");
	vkCmdSetCullMode_volkImpl = (PFN_vkCmdSetCullMode)load(context, "vkCmdSetCullMode_volkImpl");
	vkCmdSetDepthBiasEnable_volkImpl = (PFN_vkCmdSetDepthBiasEnable)load(context, "vkCmdSetDepthBiasEnable_volkImpl");
	vkCmdSetDepthBoundsTestEnable_volkImpl = (PFN_vkCmdSetDepthBoundsTestEnable)load(context, "vkCmdSetDepthBoundsTestEnable_volkImpl");
	vkCmdSetDepthCompareOp_volkImpl = (PFN_vkCmdSetDepthCompareOp)load(context, "vkCmdSetDepthCompareOp_volkImpl");
	vkCmdSetDepthTestEnable_volkImpl = (PFN_vkCmdSetDepthTestEnable)load(context, "vkCmdSetDepthTestEnable_volkImpl");
	vkCmdSetDepthWriteEnable_volkImpl = (PFN_vkCmdSetDepthWriteEnable)load(context, "vkCmdSetDepthWriteEnable_volkImpl");
	vkCmdSetEvent2_volkImpl = (PFN_vkCmdSetEvent2)load(context, "vkCmdSetEvent2_volkImpl");
	vkCmdSetFrontFace_volkImpl = (PFN_vkCmdSetFrontFace)load(context, "vkCmdSetFrontFace_volkImpl");
	vkCmdSetPrimitiveRestartEnable_volkImpl = (PFN_vkCmdSetPrimitiveRestartEnable)load(context, "vkCmdSetPrimitiveRestartEnable_volkImpl");
	vkCmdSetPrimitiveTopology_volkImpl = (PFN_vkCmdSetPrimitiveTopology)load(context, "vkCmdSetPrimitiveTopology_volkImpl");
	vkCmdSetRasterizerDiscardEnable_volkImpl = (PFN_vkCmdSetRasterizerDiscardEnable)load(context, "vkCmdSetRasterizerDiscardEnable_volkImpl");
	vkCmdSetScissorWithCount_volkImpl = (PFN_vkCmdSetScissorWithCount)load(context, "vkCmdSetScissorWithCount_volkImpl");
	vkCmdSetStencilOp_volkImpl = (PFN_vkCmdSetStencilOp)load(context, "vkCmdSetStencilOp_volkImpl");
	vkCmdSetStencilTestEnable_volkImpl = (PFN_vkCmdSetStencilTestEnable)load(context, "vkCmdSetStencilTestEnable_volkImpl");
	vkCmdSetViewportWithCount_volkImpl = (PFN_vkCmdSetViewportWithCount)load(context, "vkCmdSetViewportWithCount_volkImpl");
	vkCmdWaitEvents2_volkImpl = (PFN_vkCmdWaitEvents2)load(context, "vkCmdWaitEvents2_volkImpl");
	vkCmdWriteTimestamp2_volkImpl = (PFN_vkCmdWriteTimestamp2)load(context, "vkCmdWriteTimestamp2_volkImpl");
	vkCreatePrivateDataSlot_volkImpl = (PFN_vkCreatePrivateDataSlot)load(context, "vkCreatePrivateDataSlot_volkImpl");
	vkDestroyPrivateDataSlot_volkImpl = (PFN_vkDestroyPrivateDataSlot)load(context, "vkDestroyPrivateDataSlot_volkImpl");
	vkGetDeviceBufferMemoryRequirements_volkImpl = (PFN_vkGetDeviceBufferMemoryRequirements)load(context, "vkGetDeviceBufferMemoryRequirements_volkImpl");
	vkGetDeviceImageMemoryRequirements_volkImpl = (PFN_vkGetDeviceImageMemoryRequirements)load(context, "vkGetDeviceImageMemoryRequirements_volkImpl");
	vkGetDeviceImageSparseMemoryRequirements_volkImpl = (PFN_vkGetDeviceImageSparseMemoryRequirements)load(context, "vkGetDeviceImageSparseMemoryRequirements_volkImpl");
	vkGetPrivateData_volkImpl = (PFN_vkGetPrivateData)load(context, "vkGetPrivateData_volkImpl");
	vkQueueSubmit2_volkImpl = (PFN_vkQueueSubmit2)load(context, "vkQueueSubmit2_volkImpl");
	vkSetPrivateData_volkImpl = (PFN_vkSetPrivateData)load(context, "vkSetPrivateData_volkImpl");
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_AMDX_shader_enqueue)
	vkCmdDispatchGraphAMDX_volkImpl = (PFN_vkCmdDispatchGraphAMDX)load(context, "vkCmdDispatchGraphAMDX_volkImpl");
	vkCmdDispatchGraphIndirectAMDX_volkImpl = (PFN_vkCmdDispatchGraphIndirectAMDX)load(context, "vkCmdDispatchGraphIndirectAMDX_volkImpl");
	vkCmdDispatchGraphIndirectCountAMDX_volkImpl = (PFN_vkCmdDispatchGraphIndirectCountAMDX)load(context, "vkCmdDispatchGraphIndirectCountAMDX_volkImpl");
	vkCmdInitializeGraphScratchMemoryAMDX_volkImpl = (PFN_vkCmdInitializeGraphScratchMemoryAMDX)load(context, "vkCmdInitializeGraphScratchMemoryAMDX_volkImpl");
	vkCreateExecutionGraphPipelinesAMDX_volkImpl = (PFN_vkCreateExecutionGraphPipelinesAMDX)load(context, "vkCreateExecutionGraphPipelinesAMDX_volkImpl");
	vkGetExecutionGraphPipelineNodeIndexAMDX_volkImpl = (PFN_vkGetExecutionGraphPipelineNodeIndexAMDX)load(context, "vkGetExecutionGraphPipelineNodeIndexAMDX_volkImpl");
	vkGetExecutionGraphPipelineScratchSizeAMDX_volkImpl = (PFN_vkGetExecutionGraphPipelineScratchSizeAMDX)load(context, "vkGetExecutionGraphPipelineScratchSizeAMDX_volkImpl");
#endif /* defined(VK_AMDX_shader_enqueue) */
#if defined(VK_AMD_buffer_marker)
	vkCmdWriteBufferMarkerAMD_volkImpl = (PFN_vkCmdWriteBufferMarkerAMD)load(context, "vkCmdWriteBufferMarkerAMD_volkImpl");
#endif /* defined(VK_AMD_buffer_marker) */
#if defined(VK_AMD_display_native_hdr)
	vkSetLocalDimmingAMD_volkImpl = (PFN_vkSetLocalDimmingAMD)load(context, "vkSetLocalDimmingAMD_volkImpl");
#endif /* defined(VK_AMD_display_native_hdr) */
#if defined(VK_AMD_draw_indirect_count)
	vkCmdDrawIndexedIndirectCountAMD_volkImpl = (PFN_vkCmdDrawIndexedIndirectCountAMD)load(context, "vkCmdDrawIndexedIndirectCountAMD_volkImpl");
	vkCmdDrawIndirectCountAMD_volkImpl = (PFN_vkCmdDrawIndirectCountAMD)load(context, "vkCmdDrawIndirectCountAMD_volkImpl");
#endif /* defined(VK_AMD_draw_indirect_count) */
#if defined(VK_AMD_shader_info)
	vkGetShaderInfoAMD_volkImpl = (PFN_vkGetShaderInfoAMD)load(context, "vkGetShaderInfoAMD_volkImpl");
#endif /* defined(VK_AMD_shader_info) */
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
	vkGetAndroidHardwareBufferPropertiesANDROID_volkImpl = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID)load(context, "vkGetAndroidHardwareBufferPropertiesANDROID_volkImpl");
	vkGetMemoryAndroidHardwareBufferANDROID_volkImpl = (PFN_vkGetMemoryAndroidHardwareBufferANDROID)load(context, "vkGetMemoryAndroidHardwareBufferANDROID_volkImpl");
#endif /* defined(VK_ANDROID_external_memory_android_hardware_buffer) */
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state)
	vkCmdSetAttachmentFeedbackLoopEnableEXT_volkImpl = (PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT)load(context, "vkCmdSetAttachmentFeedbackLoopEnableEXT_volkImpl");
#endif /* defined(VK_EXT_attachment_feedback_loop_dynamic_state) */
#if defined(VK_EXT_buffer_device_address)
	vkGetBufferDeviceAddressEXT_volkImpl = (PFN_vkGetBufferDeviceAddressEXT)load(context, "vkGetBufferDeviceAddressEXT_volkImpl");
#endif /* defined(VK_EXT_buffer_device_address) */
#if defined(VK_EXT_calibrated_timestamps)
	vkGetCalibratedTimestampsEXT_volkImpl = (PFN_vkGetCalibratedTimestampsEXT)load(context, "vkGetCalibratedTimestampsEXT_volkImpl");
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_color_write_enable)
	vkCmdSetColorWriteEnableEXT_volkImpl = (PFN_vkCmdSetColorWriteEnableEXT)load(context, "vkCmdSetColorWriteEnableEXT_volkImpl");
#endif /* defined(VK_EXT_color_write_enable) */
#if defined(VK_EXT_conditional_rendering)
	vkCmdBeginConditionalRenderingEXT_volkImpl = (PFN_vkCmdBeginConditionalRenderingEXT)load(context, "vkCmdBeginConditionalRenderingEXT_volkImpl");
	vkCmdEndConditionalRenderingEXT_volkImpl = (PFN_vkCmdEndConditionalRenderingEXT)load(context, "vkCmdEndConditionalRenderingEXT_volkImpl");
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_debug_marker)
	vkCmdDebugMarkerBeginEXT_volkImpl = (PFN_vkCmdDebugMarkerBeginEXT)load(context, "vkCmdDebugMarkerBeginEXT_volkImpl");
	vkCmdDebugMarkerEndEXT_volkImpl = (PFN_vkCmdDebugMarkerEndEXT)load(context, "vkCmdDebugMarkerEndEXT_volkImpl");
	vkCmdDebugMarkerInsertEXT_volkImpl = (PFN_vkCmdDebugMarkerInsertEXT)load(context, "vkCmdDebugMarkerInsertEXT_volkImpl");
	vkDebugMarkerSetObjectNameEXT_volkImpl = (PFN_vkDebugMarkerSetObjectNameEXT)load(context, "vkDebugMarkerSetObjectNameEXT_volkImpl");
	vkDebugMarkerSetObjectTagEXT_volkImpl = (PFN_vkDebugMarkerSetObjectTagEXT)load(context, "vkDebugMarkerSetObjectTagEXT_volkImpl");
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_depth_bias_control)
	vkCmdSetDepthBias2EXT_volkImpl = (PFN_vkCmdSetDepthBias2EXT)load(context, "vkCmdSetDepthBias2EXT_volkImpl");
#endif /* defined(VK_EXT_depth_bias_control) */
#if defined(VK_EXT_descriptor_buffer)
	vkCmdBindDescriptorBufferEmbeddedSamplersEXT_volkImpl = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT)load(context, "vkCmdBindDescriptorBufferEmbeddedSamplersEXT_volkImpl");
	vkCmdBindDescriptorBuffersEXT_volkImpl = (PFN_vkCmdBindDescriptorBuffersEXT)load(context, "vkCmdBindDescriptorBuffersEXT_volkImpl");
	vkCmdSetDescriptorBufferOffsetsEXT_volkImpl = (PFN_vkCmdSetDescriptorBufferOffsetsEXT)load(context, "vkCmdSetDescriptorBufferOffsetsEXT_volkImpl");
	vkGetBufferOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT)load(context, "vkGetBufferOpaqueCaptureDescriptorDataEXT_volkImpl");
	vkGetDescriptorEXT_volkImpl = (PFN_vkGetDescriptorEXT)load(context, "vkGetDescriptorEXT_volkImpl");
	vkGetDescriptorSetLayoutBindingOffsetEXT_volkImpl = (PFN_vkGetDescriptorSetLayoutBindingOffsetEXT)load(context, "vkGetDescriptorSetLayoutBindingOffsetEXT_volkImpl");
	vkGetDescriptorSetLayoutSizeEXT_volkImpl = (PFN_vkGetDescriptorSetLayoutSizeEXT)load(context, "vkGetDescriptorSetLayoutSizeEXT_volkImpl");
	vkGetImageOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetImageOpaqueCaptureDescriptorDataEXT)load(context, "vkGetImageOpaqueCaptureDescriptorDataEXT_volkImpl");
	vkGetImageViewOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT)load(context, "vkGetImageViewOpaqueCaptureDescriptorDataEXT_volkImpl");
	vkGetSamplerOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT)load(context, "vkGetSamplerOpaqueCaptureDescriptorDataEXT_volkImpl");
#endif /* defined(VK_EXT_descriptor_buffer) */
#if defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing))
	vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT)load(context, "vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT_volkImpl");
#endif /* defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)) */
#if defined(VK_EXT_device_fault)
	vkGetDeviceFaultInfoEXT_volkImpl = (PFN_vkGetDeviceFaultInfoEXT)load(context, "vkGetDeviceFaultInfoEXT_volkImpl");
#endif /* defined(VK_EXT_device_fault) */
#if defined(VK_EXT_discard_rectangles)
	vkCmdSetDiscardRectangleEXT_volkImpl = (PFN_vkCmdSetDiscardRectangleEXT)load(context, "vkCmdSetDiscardRectangleEXT_volkImpl");
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2
	vkCmdSetDiscardRectangleEnableEXT_volkImpl = (PFN_vkCmdSetDiscardRectangleEnableEXT)load(context, "vkCmdSetDiscardRectangleEnableEXT_volkImpl");
	vkCmdSetDiscardRectangleModeEXT_volkImpl = (PFN_vkCmdSetDiscardRectangleModeEXT)load(context, "vkCmdSetDiscardRectangleModeEXT_volkImpl");
#endif /* defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2 */
#if defined(VK_EXT_display_control)
	vkDisplayPowerControlEXT_volkImpl = (PFN_vkDisplayPowerControlEXT)load(context, "vkDisplayPowerControlEXT_volkImpl");
	vkGetSwapchainCounterEXT_volkImpl = (PFN_vkGetSwapchainCounterEXT)load(context, "vkGetSwapchainCounterEXT_volkImpl");
	vkRegisterDeviceEventEXT_volkImpl = (PFN_vkRegisterDeviceEventEXT)load(context, "vkRegisterDeviceEventEXT_volkImpl");
	vkRegisterDisplayEventEXT_volkImpl = (PFN_vkRegisterDisplayEventEXT)load(context, "vkRegisterDisplayEventEXT_volkImpl");
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_external_memory_host)
	vkGetMemoryHostPointerPropertiesEXT_volkImpl = (PFN_vkGetMemoryHostPointerPropertiesEXT)load(context, "vkGetMemoryHostPointerPropertiesEXT_volkImpl");
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_full_screen_exclusive)
	vkAcquireFullScreenExclusiveModeEXT_volkImpl = (PFN_vkAcquireFullScreenExclusiveModeEXT)load(context, "vkAcquireFullScreenExclusiveModeEXT_volkImpl");
	vkReleaseFullScreenExclusiveModeEXT_volkImpl = (PFN_vkReleaseFullScreenExclusiveModeEXT)load(context, "vkReleaseFullScreenExclusiveModeEXT_volkImpl");
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_hdr_metadata)
	vkSetHdrMetadataEXT_volkImpl = (PFN_vkSetHdrMetadataEXT)load(context, "vkSetHdrMetadataEXT_volkImpl");
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_host_image_copy)
	vkCopyImageToImageEXT_volkImpl = (PFN_vkCopyImageToImageEXT)load(context, "vkCopyImageToImageEXT_volkImpl");
	vkCopyImageToMemoryEXT_volkImpl = (PFN_vkCopyImageToMemoryEXT)load(context, "vkCopyImageToMemoryEXT_volkImpl");
	vkCopyMemoryToImageEXT_volkImpl = (PFN_vkCopyMemoryToImageEXT)load(context, "vkCopyMemoryToImageEXT_volkImpl");
	vkTransitionImageLayoutEXT_volkImpl = (PFN_vkTransitionImageLayoutEXT)load(context, "vkTransitionImageLayoutEXT_volkImpl");
#endif /* defined(VK_EXT_host_image_copy) */
#if defined(VK_EXT_host_query_reset)
	vkResetQueryPoolEXT_volkImpl = (PFN_vkResetQueryPoolEXT)load(context, "vkResetQueryPoolEXT_volkImpl");
#endif /* defined(VK_EXT_host_query_reset) */
#if defined(VK_EXT_image_drm_format_modifier)
	vkGetImageDrmFormatModifierPropertiesEXT_volkImpl = (PFN_vkGetImageDrmFormatModifierPropertiesEXT)load(context, "vkGetImageDrmFormatModifierPropertiesEXT_volkImpl");
#endif /* defined(VK_EXT_image_drm_format_modifier) */
#if defined(VK_EXT_line_rasterization)
	vkCmdSetLineStippleEXT_volkImpl = (PFN_vkCmdSetLineStippleEXT)load(context, "vkCmdSetLineStippleEXT_volkImpl");
#endif /* defined(VK_EXT_line_rasterization) */
#if defined(VK_EXT_mesh_shader)
	vkCmdDrawMeshTasksEXT_volkImpl = (PFN_vkCmdDrawMeshTasksEXT)load(context, "vkCmdDrawMeshTasksEXT_volkImpl");
	vkCmdDrawMeshTasksIndirectCountEXT_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectCountEXT)load(context, "vkCmdDrawMeshTasksIndirectCountEXT_volkImpl");
	vkCmdDrawMeshTasksIndirectEXT_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectEXT)load(context, "vkCmdDrawMeshTasksIndirectEXT_volkImpl");
#endif /* defined(VK_EXT_mesh_shader) */
#if defined(VK_EXT_metal_objects)
	vkExportMetalObjectsEXT_volkImpl = (PFN_vkExportMetalObjectsEXT)load(context, "vkExportMetalObjectsEXT_volkImpl");
#endif /* defined(VK_EXT_metal_objects) */
#if defined(VK_EXT_multi_draw)
	vkCmdDrawMultiEXT_volkImpl = (PFN_vkCmdDrawMultiEXT)load(context, "vkCmdDrawMultiEXT_volkImpl");
	vkCmdDrawMultiIndexedEXT_volkImpl = (PFN_vkCmdDrawMultiIndexedEXT)load(context, "vkCmdDrawMultiIndexedEXT_volkImpl");
#endif /* defined(VK_EXT_multi_draw) */
#if defined(VK_EXT_opacity_micromap)
	vkBuildMicromapsEXT_volkImpl = (PFN_vkBuildMicromapsEXT)load(context, "vkBuildMicromapsEXT_volkImpl");
	vkCmdBuildMicromapsEXT_volkImpl = (PFN_vkCmdBuildMicromapsEXT)load(context, "vkCmdBuildMicromapsEXT_volkImpl");
	vkCmdCopyMemoryToMicromapEXT_volkImpl = (PFN_vkCmdCopyMemoryToMicromapEXT)load(context, "vkCmdCopyMemoryToMicromapEXT_volkImpl");
	vkCmdCopyMicromapEXT_volkImpl = (PFN_vkCmdCopyMicromapEXT)load(context, "vkCmdCopyMicromapEXT_volkImpl");
	vkCmdCopyMicromapToMemoryEXT_volkImpl = (PFN_vkCmdCopyMicromapToMemoryEXT)load(context, "vkCmdCopyMicromapToMemoryEXT_volkImpl");
	vkCmdWriteMicromapsPropertiesEXT_volkImpl = (PFN_vkCmdWriteMicromapsPropertiesEXT)load(context, "vkCmdWriteMicromapsPropertiesEXT_volkImpl");
	vkCopyMemoryToMicromapEXT_volkImpl = (PFN_vkCopyMemoryToMicromapEXT)load(context, "vkCopyMemoryToMicromapEXT_volkImpl");
	vkCopyMicromapEXT_volkImpl = (PFN_vkCopyMicromapEXT)load(context, "vkCopyMicromapEXT_volkImpl");
	vkCopyMicromapToMemoryEXT_volkImpl = (PFN_vkCopyMicromapToMemoryEXT)load(context, "vkCopyMicromapToMemoryEXT_volkImpl");
	vkCreateMicromapEXT_volkImpl = (PFN_vkCreateMicromapEXT)load(context, "vkCreateMicromapEXT_volkImpl");
	vkDestroyMicromapEXT_volkImpl = (PFN_vkDestroyMicromapEXT)load(context, "vkDestroyMicromapEXT_volkImpl");
	vkGetDeviceMicromapCompatibilityEXT_volkImpl = (PFN_vkGetDeviceMicromapCompatibilityEXT)load(context, "vkGetDeviceMicromapCompatibilityEXT_volkImpl");
	vkGetMicromapBuildSizesEXT_volkImpl = (PFN_vkGetMicromapBuildSizesEXT)load(context, "vkGetMicromapBuildSizesEXT_volkImpl");
	vkWriteMicromapsPropertiesEXT_volkImpl = (PFN_vkWriteMicromapsPropertiesEXT)load(context, "vkWriteMicromapsPropertiesEXT_volkImpl");
#endif /* defined(VK_EXT_opacity_micromap) */
#if defined(VK_EXT_pageable_device_local_memory)
	vkSetDeviceMemoryPriorityEXT_volkImpl = (PFN_vkSetDeviceMemoryPriorityEXT)load(context, "vkSetDeviceMemoryPriorityEXT_volkImpl");
#endif /* defined(VK_EXT_pageable_device_local_memory) */
#if defined(VK_EXT_pipeline_properties)
	vkGetPipelinePropertiesEXT_volkImpl = (PFN_vkGetPipelinePropertiesEXT)load(context, "vkGetPipelinePropertiesEXT_volkImpl");
#endif /* defined(VK_EXT_pipeline_properties) */
#if defined(VK_EXT_private_data)
	vkCreatePrivateDataSlotEXT_volkImpl = (PFN_vkCreatePrivateDataSlotEXT)load(context, "vkCreatePrivateDataSlotEXT_volkImpl");
	vkDestroyPrivateDataSlotEXT_volkImpl = (PFN_vkDestroyPrivateDataSlotEXT)load(context, "vkDestroyPrivateDataSlotEXT_volkImpl");
	vkGetPrivateDataEXT_volkImpl = (PFN_vkGetPrivateDataEXT)load(context, "vkGetPrivateDataEXT_volkImpl");
	vkSetPrivateDataEXT_volkImpl = (PFN_vkSetPrivateDataEXT)load(context, "vkSetPrivateDataEXT_volkImpl");
#endif /* defined(VK_EXT_private_data) */
#if defined(VK_EXT_sample_locations)
	vkCmdSetSampleLocationsEXT_volkImpl = (PFN_vkCmdSetSampleLocationsEXT)load(context, "vkCmdSetSampleLocationsEXT_volkImpl");
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_shader_module_identifier)
	vkGetShaderModuleCreateInfoIdentifierEXT_volkImpl = (PFN_vkGetShaderModuleCreateInfoIdentifierEXT)load(context, "vkGetShaderModuleCreateInfoIdentifierEXT_volkImpl");
	vkGetShaderModuleIdentifierEXT_volkImpl = (PFN_vkGetShaderModuleIdentifierEXT)load(context, "vkGetShaderModuleIdentifierEXT_volkImpl");
#endif /* defined(VK_EXT_shader_module_identifier) */
#if defined(VK_EXT_shader_object)
	vkCmdBindShadersEXT_volkImpl = (PFN_vkCmdBindShadersEXT)load(context, "vkCmdBindShadersEXT_volkImpl");
	vkCreateShadersEXT_volkImpl = (PFN_vkCreateShadersEXT)load(context, "vkCreateShadersEXT_volkImpl");
	vkDestroyShaderEXT_volkImpl = (PFN_vkDestroyShaderEXT)load(context, "vkDestroyShaderEXT_volkImpl");
	vkGetShaderBinaryDataEXT_volkImpl = (PFN_vkGetShaderBinaryDataEXT)load(context, "vkGetShaderBinaryDataEXT_volkImpl");
#endif /* defined(VK_EXT_shader_object) */
#if defined(VK_EXT_swapchain_maintenance1)
	vkReleaseSwapchainImagesEXT_volkImpl = (PFN_vkReleaseSwapchainImagesEXT)load(context, "vkReleaseSwapchainImagesEXT_volkImpl");
#endif /* defined(VK_EXT_swapchain_maintenance1) */
#if defined(VK_EXT_transform_feedback)
	vkCmdBeginQueryIndexedEXT_volkImpl = (PFN_vkCmdBeginQueryIndexedEXT)load(context, "vkCmdBeginQueryIndexedEXT_volkImpl");
	vkCmdBeginTransformFeedbackEXT_volkImpl = (PFN_vkCmdBeginTransformFeedbackEXT)load(context, "vkCmdBeginTransformFeedbackEXT_volkImpl");
	vkCmdBindTransformFeedbackBuffersEXT_volkImpl = (PFN_vkCmdBindTransformFeedbackBuffersEXT)load(context, "vkCmdBindTransformFeedbackBuffersEXT_volkImpl");
	vkCmdDrawIndirectByteCountEXT_volkImpl = (PFN_vkCmdDrawIndirectByteCountEXT)load(context, "vkCmdDrawIndirectByteCountEXT_volkImpl");
	vkCmdEndQueryIndexedEXT_volkImpl = (PFN_vkCmdEndQueryIndexedEXT)load(context, "vkCmdEndQueryIndexedEXT_volkImpl");
	vkCmdEndTransformFeedbackEXT_volkImpl = (PFN_vkCmdEndTransformFeedbackEXT)load(context, "vkCmdEndTransformFeedbackEXT_volkImpl");
#endif /* defined(VK_EXT_transform_feedback) */
#if defined(VK_EXT_validation_cache)
	vkCreateValidationCacheEXT_volkImpl = (PFN_vkCreateValidationCacheEXT)load(context, "vkCreateValidationCacheEXT_volkImpl");
	vkDestroyValidationCacheEXT_volkImpl = (PFN_vkDestroyValidationCacheEXT)load(context, "vkDestroyValidationCacheEXT_volkImpl");
	vkGetValidationCacheDataEXT_volkImpl = (PFN_vkGetValidationCacheDataEXT)load(context, "vkGetValidationCacheDataEXT_volkImpl");
	vkMergeValidationCachesEXT_volkImpl = (PFN_vkMergeValidationCachesEXT)load(context, "vkMergeValidationCachesEXT_volkImpl");
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_FUCHSIA_buffer_collection)
	vkCreateBufferCollectionFUCHSIA_volkImpl = (PFN_vkCreateBufferCollectionFUCHSIA)load(context, "vkCreateBufferCollectionFUCHSIA_volkImpl");
	vkDestroyBufferCollectionFUCHSIA_volkImpl = (PFN_vkDestroyBufferCollectionFUCHSIA)load(context, "vkDestroyBufferCollectionFUCHSIA_volkImpl");
	vkGetBufferCollectionPropertiesFUCHSIA_volkImpl = (PFN_vkGetBufferCollectionPropertiesFUCHSIA)load(context, "vkGetBufferCollectionPropertiesFUCHSIA_volkImpl");
	vkSetBufferCollectionBufferConstraintsFUCHSIA_volkImpl = (PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA)load(context, "vkSetBufferCollectionBufferConstraintsFUCHSIA_volkImpl");
	vkSetBufferCollectionImageConstraintsFUCHSIA_volkImpl = (PFN_vkSetBufferCollectionImageConstraintsFUCHSIA)load(context, "vkSetBufferCollectionImageConstraintsFUCHSIA_volkImpl");
#endif /* defined(VK_FUCHSIA_buffer_collection) */
#if defined(VK_FUCHSIA_external_memory)
	vkGetMemoryZirconHandleFUCHSIA_volkImpl = (PFN_vkGetMemoryZirconHandleFUCHSIA)load(context, "vkGetMemoryZirconHandleFUCHSIA_volkImpl");
	vkGetMemoryZirconHandlePropertiesFUCHSIA_volkImpl = (PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA)load(context, "vkGetMemoryZirconHandlePropertiesFUCHSIA_volkImpl");
#endif /* defined(VK_FUCHSIA_external_memory) */
#if defined(VK_FUCHSIA_external_semaphore)
	vkGetSemaphoreZirconHandleFUCHSIA_volkImpl = (PFN_vkGetSemaphoreZirconHandleFUCHSIA)load(context, "vkGetSemaphoreZirconHandleFUCHSIA_volkImpl");
	vkImportSemaphoreZirconHandleFUCHSIA_volkImpl = (PFN_vkImportSemaphoreZirconHandleFUCHSIA)load(context, "vkImportSemaphoreZirconHandleFUCHSIA_volkImpl");
#endif /* defined(VK_FUCHSIA_external_semaphore) */
#if defined(VK_GOOGLE_display_timing)
	vkGetPastPresentationTimingGOOGLE_volkImpl = (PFN_vkGetPastPresentationTimingGOOGLE)load(context, "vkGetPastPresentationTimingGOOGLE_volkImpl");
	vkGetRefreshCycleDurationGOOGLE_volkImpl = (PFN_vkGetRefreshCycleDurationGOOGLE)load(context, "vkGetRefreshCycleDurationGOOGLE_volkImpl");
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_HUAWEI_cluster_culling_shader)
	vkCmdDrawClusterHUAWEI_volkImpl = (PFN_vkCmdDrawClusterHUAWEI)load(context, "vkCmdDrawClusterHUAWEI_volkImpl");
	vkCmdDrawClusterIndirectHUAWEI_volkImpl = (PFN_vkCmdDrawClusterIndirectHUAWEI)load(context, "vkCmdDrawClusterIndirectHUAWEI_volkImpl");
#endif /* defined(VK_HUAWEI_cluster_culling_shader) */
#if defined(VK_HUAWEI_invocation_mask)
	vkCmdBindInvocationMaskHUAWEI_volkImpl = (PFN_vkCmdBindInvocationMaskHUAWEI)load(context, "vkCmdBindInvocationMaskHUAWEI_volkImpl");
#endif /* defined(VK_HUAWEI_invocation_mask) */
#if defined(VK_HUAWEI_subpass_shading)
	vkCmdSubpassShadingHUAWEI_volkImpl = (PFN_vkCmdSubpassShadingHUAWEI)load(context, "vkCmdSubpassShadingHUAWEI_volkImpl");
	vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI_volkImpl = (PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI)load(context, "vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI_volkImpl");
#endif /* defined(VK_HUAWEI_subpass_shading) */
#if defined(VK_INTEL_performance_query)
	vkAcquirePerformanceConfigurationINTEL_volkImpl = (PFN_vkAcquirePerformanceConfigurationINTEL)load(context, "vkAcquirePerformanceConfigurationINTEL_volkImpl");
	vkCmdSetPerformanceMarkerINTEL_volkImpl = (PFN_vkCmdSetPerformanceMarkerINTEL)load(context, "vkCmdSetPerformanceMarkerINTEL_volkImpl");
	vkCmdSetPerformanceOverrideINTEL_volkImpl = (PFN_vkCmdSetPerformanceOverrideINTEL)load(context, "vkCmdSetPerformanceOverrideINTEL_volkImpl");
	vkCmdSetPerformanceStreamMarkerINTEL_volkImpl = (PFN_vkCmdSetPerformanceStreamMarkerINTEL)load(context, "vkCmdSetPerformanceStreamMarkerINTEL_volkImpl");
	vkGetPerformanceParameterINTEL_volkImpl = (PFN_vkGetPerformanceParameterINTEL)load(context, "vkGetPerformanceParameterINTEL_volkImpl");
	vkInitializePerformanceApiINTEL_volkImpl = (PFN_vkInitializePerformanceApiINTEL)load(context, "vkInitializePerformanceApiINTEL_volkImpl");
	vkQueueSetPerformanceConfigurationINTEL_volkImpl = (PFN_vkQueueSetPerformanceConfigurationINTEL)load(context, "vkQueueSetPerformanceConfigurationINTEL_volkImpl");
	vkReleasePerformanceConfigurationINTEL_volkImpl = (PFN_vkReleasePerformanceConfigurationINTEL)load(context, "vkReleasePerformanceConfigurationINTEL_volkImpl");
	vkUninitializePerformanceApiINTEL_volkImpl = (PFN_vkUninitializePerformanceApiINTEL)load(context, "vkUninitializePerformanceApiINTEL_volkImpl");
#endif /* defined(VK_INTEL_performance_query) */
#if defined(VK_KHR_acceleration_structure)
	vkBuildAccelerationStructuresKHR_volkImpl = (PFN_vkBuildAccelerationStructuresKHR)load(context, "vkBuildAccelerationStructuresKHR_volkImpl");
	vkCmdBuildAccelerationStructuresIndirectKHR_volkImpl = (PFN_vkCmdBuildAccelerationStructuresIndirectKHR)load(context, "vkCmdBuildAccelerationStructuresIndirectKHR_volkImpl");
	vkCmdBuildAccelerationStructuresKHR_volkImpl = (PFN_vkCmdBuildAccelerationStructuresKHR)load(context, "vkCmdBuildAccelerationStructuresKHR_volkImpl");
	vkCmdCopyAccelerationStructureKHR_volkImpl = (PFN_vkCmdCopyAccelerationStructureKHR)load(context, "vkCmdCopyAccelerationStructureKHR_volkImpl");
	vkCmdCopyAccelerationStructureToMemoryKHR_volkImpl = (PFN_vkCmdCopyAccelerationStructureToMemoryKHR)load(context, "vkCmdCopyAccelerationStructureToMemoryKHR_volkImpl");
	vkCmdCopyMemoryToAccelerationStructureKHR_volkImpl = (PFN_vkCmdCopyMemoryToAccelerationStructureKHR)load(context, "vkCmdCopyMemoryToAccelerationStructureKHR_volkImpl");
	vkCmdWriteAccelerationStructuresPropertiesKHR_volkImpl = (PFN_vkCmdWriteAccelerationStructuresPropertiesKHR)load(context, "vkCmdWriteAccelerationStructuresPropertiesKHR_volkImpl");
	vkCopyAccelerationStructureKHR_volkImpl = (PFN_vkCopyAccelerationStructureKHR)load(context, "vkCopyAccelerationStructureKHR_volkImpl");
	vkCopyAccelerationStructureToMemoryKHR_volkImpl = (PFN_vkCopyAccelerationStructureToMemoryKHR)load(context, "vkCopyAccelerationStructureToMemoryKHR_volkImpl");
	vkCopyMemoryToAccelerationStructureKHR_volkImpl = (PFN_vkCopyMemoryToAccelerationStructureKHR)load(context, "vkCopyMemoryToAccelerationStructureKHR_volkImpl");
	vkCreateAccelerationStructureKHR_volkImpl = (PFN_vkCreateAccelerationStructureKHR)load(context, "vkCreateAccelerationStructureKHR_volkImpl");
	vkDestroyAccelerationStructureKHR_volkImpl = (PFN_vkDestroyAccelerationStructureKHR)load(context, "vkDestroyAccelerationStructureKHR_volkImpl");
	vkGetAccelerationStructureBuildSizesKHR_volkImpl = (PFN_vkGetAccelerationStructureBuildSizesKHR)load(context, "vkGetAccelerationStructureBuildSizesKHR_volkImpl");
	vkGetAccelerationStructureDeviceAddressKHR_volkImpl = (PFN_vkGetAccelerationStructureDeviceAddressKHR)load(context, "vkGetAccelerationStructureDeviceAddressKHR_volkImpl");
	vkGetDeviceAccelerationStructureCompatibilityKHR_volkImpl = (PFN_vkGetDeviceAccelerationStructureCompatibilityKHR)load(context, "vkGetDeviceAccelerationStructureCompatibilityKHR_volkImpl");
	vkWriteAccelerationStructuresPropertiesKHR_volkImpl = (PFN_vkWriteAccelerationStructuresPropertiesKHR)load(context, "vkWriteAccelerationStructuresPropertiesKHR_volkImpl");
#endif /* defined(VK_KHR_acceleration_structure) */
#if defined(VK_KHR_bind_memory2)
	vkBindBufferMemory2KHR_volkImpl = (PFN_vkBindBufferMemory2KHR)load(context, "vkBindBufferMemory2KHR_volkImpl");
	vkBindImageMemory2KHR_volkImpl = (PFN_vkBindImageMemory2KHR)load(context, "vkBindImageMemory2KHR_volkImpl");
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_buffer_device_address)
	vkGetBufferDeviceAddressKHR_volkImpl = (PFN_vkGetBufferDeviceAddressKHR)load(context, "vkGetBufferDeviceAddressKHR_volkImpl");
	vkGetBufferOpaqueCaptureAddressKHR_volkImpl = (PFN_vkGetBufferOpaqueCaptureAddressKHR)load(context, "vkGetBufferOpaqueCaptureAddressKHR_volkImpl");
	vkGetDeviceMemoryOpaqueCaptureAddressKHR_volkImpl = (PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR)load(context, "vkGetDeviceMemoryOpaqueCaptureAddressKHR_volkImpl");
#endif /* defined(VK_KHR_buffer_device_address) */
#if defined(VK_KHR_calibrated_timestamps)
	vkGetCalibratedTimestampsKHR_volkImpl = (PFN_vkGetCalibratedTimestampsKHR)load(context, "vkGetCalibratedTimestampsKHR_volkImpl");
#endif /* defined(VK_KHR_calibrated_timestamps) */
#if defined(VK_KHR_copy_commands2)
	vkCmdBlitImage2KHR_volkImpl = (PFN_vkCmdBlitImage2KHR)load(context, "vkCmdBlitImage2KHR_volkImpl");
	vkCmdCopyBuffer2KHR_volkImpl = (PFN_vkCmdCopyBuffer2KHR)load(context, "vkCmdCopyBuffer2KHR_volkImpl");
	vkCmdCopyBufferToImage2KHR_volkImpl = (PFN_vkCmdCopyBufferToImage2KHR)load(context, "vkCmdCopyBufferToImage2KHR_volkImpl");
	vkCmdCopyImage2KHR_volkImpl = (PFN_vkCmdCopyImage2KHR)load(context, "vkCmdCopyImage2KHR_volkImpl");
	vkCmdCopyImageToBuffer2KHR_volkImpl = (PFN_vkCmdCopyImageToBuffer2KHR)load(context, "vkCmdCopyImageToBuffer2KHR_volkImpl");
	vkCmdResolveImage2KHR_volkImpl = (PFN_vkCmdResolveImage2KHR)load(context, "vkCmdResolveImage2KHR_volkImpl");
#endif /* defined(VK_KHR_copy_commands2) */
#if defined(VK_KHR_create_renderpass2)
	vkCmdBeginRenderPass2KHR_volkImpl = (PFN_vkCmdBeginRenderPass2KHR)load(context, "vkCmdBeginRenderPass2KHR_volkImpl");
	vkCmdEndRenderPass2KHR_volkImpl = (PFN_vkCmdEndRenderPass2KHR)load(context, "vkCmdEndRenderPass2KHR_volkImpl");
	vkCmdNextSubpass2KHR_volkImpl = (PFN_vkCmdNextSubpass2KHR)load(context, "vkCmdNextSubpass2KHR_volkImpl");
	vkCreateRenderPass2KHR_volkImpl = (PFN_vkCreateRenderPass2KHR)load(context, "vkCreateRenderPass2KHR_volkImpl");
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_deferred_host_operations)
	vkCreateDeferredOperationKHR_volkImpl = (PFN_vkCreateDeferredOperationKHR)load(context, "vkCreateDeferredOperationKHR_volkImpl");
	vkDeferredOperationJoinKHR_volkImpl = (PFN_vkDeferredOperationJoinKHR)load(context, "vkDeferredOperationJoinKHR_volkImpl");
	vkDestroyDeferredOperationKHR_volkImpl = (PFN_vkDestroyDeferredOperationKHR)load(context, "vkDestroyDeferredOperationKHR_volkImpl");
	vkGetDeferredOperationMaxConcurrencyKHR_volkImpl = (PFN_vkGetDeferredOperationMaxConcurrencyKHR)load(context, "vkGetDeferredOperationMaxConcurrencyKHR_volkImpl");
	vkGetDeferredOperationResultKHR_volkImpl = (PFN_vkGetDeferredOperationResultKHR)load(context, "vkGetDeferredOperationResultKHR_volkImpl");
#endif /* defined(VK_KHR_deferred_host_operations) */
#if defined(VK_KHR_descriptor_update_template)
	vkCreateDescriptorUpdateTemplateKHR_volkImpl = (PFN_vkCreateDescriptorUpdateTemplateKHR)load(context, "vkCreateDescriptorUpdateTemplateKHR_volkImpl");
	vkDestroyDescriptorUpdateTemplateKHR_volkImpl = (PFN_vkDestroyDescriptorUpdateTemplateKHR)load(context, "vkDestroyDescriptorUpdateTemplateKHR_volkImpl");
	vkUpdateDescriptorSetWithTemplateKHR_volkImpl = (PFN_vkUpdateDescriptorSetWithTemplateKHR)load(context, "vkUpdateDescriptorSetWithTemplateKHR_volkImpl");
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
	vkCmdDispatchBaseKHR_volkImpl = (PFN_vkCmdDispatchBaseKHR)load(context, "vkCmdDispatchBaseKHR_volkImpl");
	vkCmdSetDeviceMaskKHR_volkImpl = (PFN_vkCmdSetDeviceMaskKHR)load(context, "vkCmdSetDeviceMaskKHR_volkImpl");
	vkGetDeviceGroupPeerMemoryFeaturesKHR_volkImpl = (PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR)load(context, "vkGetDeviceGroupPeerMemoryFeaturesKHR_volkImpl");
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_display_swapchain)
	vkCreateSharedSwapchainsKHR_volkImpl = (PFN_vkCreateSharedSwapchainsKHR)load(context, "vkCreateSharedSwapchainsKHR_volkImpl");
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
	vkCmdDrawIndexedIndirectCountKHR_volkImpl = (PFN_vkCmdDrawIndexedIndirectCountKHR)load(context, "vkCmdDrawIndexedIndirectCountKHR_volkImpl");
	vkCmdDrawIndirectCountKHR_volkImpl = (PFN_vkCmdDrawIndirectCountKHR)load(context, "vkCmdDrawIndirectCountKHR_volkImpl");
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_dynamic_rendering)
	vkCmdBeginRenderingKHR_volkImpl = (PFN_vkCmdBeginRenderingKHR)load(context, "vkCmdBeginRenderingKHR_volkImpl");
	vkCmdEndRenderingKHR_volkImpl = (PFN_vkCmdEndRenderingKHR)load(context, "vkCmdEndRenderingKHR_volkImpl");
#endif /* defined(VK_KHR_dynamic_rendering) */
#if defined(VK_KHR_dynamic_rendering_local_read)
	vkCmdSetRenderingAttachmentLocationsKHR_volkImpl = (PFN_vkCmdSetRenderingAttachmentLocationsKHR)load(context, "vkCmdSetRenderingAttachmentLocationsKHR_volkImpl");
	vkCmdSetRenderingInputAttachmentIndicesKHR_volkImpl = (PFN_vkCmdSetRenderingInputAttachmentIndicesKHR)load(context, "vkCmdSetRenderingInputAttachmentIndicesKHR_volkImpl");
#endif /* defined(VK_KHR_dynamic_rendering_local_read) */
#if defined(VK_KHR_external_fence_fd)
	vkGetFenceFdKHR_volkImpl = (PFN_vkGetFenceFdKHR)load(context, "vkGetFenceFdKHR_volkImpl");
	vkImportFenceFdKHR_volkImpl = (PFN_vkImportFenceFdKHR)load(context, "vkImportFenceFdKHR_volkImpl");
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
	vkGetFenceWin32HandleKHR_volkImpl = (PFN_vkGetFenceWin32HandleKHR)load(context, "vkGetFenceWin32HandleKHR_volkImpl");
	vkImportFenceWin32HandleKHR_volkImpl = (PFN_vkImportFenceWin32HandleKHR)load(context, "vkImportFenceWin32HandleKHR_volkImpl");
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_fd)
	vkGetMemoryFdKHR_volkImpl = (PFN_vkGetMemoryFdKHR)load(context, "vkGetMemoryFdKHR_volkImpl");
	vkGetMemoryFdPropertiesKHR_volkImpl = (PFN_vkGetMemoryFdPropertiesKHR)load(context, "vkGetMemoryFdPropertiesKHR_volkImpl");
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
	vkGetMemoryWin32HandleKHR_volkImpl = (PFN_vkGetMemoryWin32HandleKHR)load(context, "vkGetMemoryWin32HandleKHR_volkImpl");
	vkGetMemoryWin32HandlePropertiesKHR_volkImpl = (PFN_vkGetMemoryWin32HandlePropertiesKHR)load(context, "vkGetMemoryWin32HandlePropertiesKHR_volkImpl");
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_fd)
	vkGetSemaphoreFdKHR_volkImpl = (PFN_vkGetSemaphoreFdKHR)load(context, "vkGetSemaphoreFdKHR_volkImpl");
	vkImportSemaphoreFdKHR_volkImpl = (PFN_vkImportSemaphoreFdKHR)load(context, "vkImportSemaphoreFdKHR_volkImpl");
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
	vkGetSemaphoreWin32HandleKHR_volkImpl = (PFN_vkGetSemaphoreWin32HandleKHR)load(context, "vkGetSemaphoreWin32HandleKHR_volkImpl");
	vkImportSemaphoreWin32HandleKHR_volkImpl = (PFN_vkImportSemaphoreWin32HandleKHR)load(context, "vkImportSemaphoreWin32HandleKHR_volkImpl");
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_fragment_shading_rate)
	vkCmdSetFragmentShadingRateKHR_volkImpl = (PFN_vkCmdSetFragmentShadingRateKHR)load(context, "vkCmdSetFragmentShadingRateKHR_volkImpl");
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_memory_requirements2)
	vkGetBufferMemoryRequirements2KHR_volkImpl = (PFN_vkGetBufferMemoryRequirements2KHR)load(context, "vkGetBufferMemoryRequirements2KHR_volkImpl");
	vkGetImageMemoryRequirements2KHR_volkImpl = (PFN_vkGetImageMemoryRequirements2KHR)load(context, "vkGetImageMemoryRequirements2KHR_volkImpl");
	vkGetImageSparseMemoryRequirements2KHR_volkImpl = (PFN_vkGetImageSparseMemoryRequirements2KHR)load(context, "vkGetImageSparseMemoryRequirements2KHR_volkImpl");
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_line_rasterization)
	vkCmdSetLineStippleKHR_volkImpl = (PFN_vkCmdSetLineStippleKHR)load(context, "vkCmdSetLineStippleKHR_volkImpl");
#endif /* defined(VK_KHR_line_rasterization) */
#if defined(VK_KHR_maintenance1)
	vkTrimCommandPoolKHR_volkImpl = (PFN_vkTrimCommandPoolKHR)load(context, "vkTrimCommandPoolKHR_volkImpl");
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance3)
	vkGetDescriptorSetLayoutSupportKHR_volkImpl = (PFN_vkGetDescriptorSetLayoutSupportKHR)load(context, "vkGetDescriptorSetLayoutSupportKHR_volkImpl");
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_maintenance4)
	vkGetDeviceBufferMemoryRequirementsKHR_volkImpl = (PFN_vkGetDeviceBufferMemoryRequirementsKHR)load(context, "vkGetDeviceBufferMemoryRequirementsKHR_volkImpl");
	vkGetDeviceImageMemoryRequirementsKHR_volkImpl = (PFN_vkGetDeviceImageMemoryRequirementsKHR)load(context, "vkGetDeviceImageMemoryRequirementsKHR_volkImpl");
	vkGetDeviceImageSparseMemoryRequirementsKHR_volkImpl = (PFN_vkGetDeviceImageSparseMemoryRequirementsKHR)load(context, "vkGetDeviceImageSparseMemoryRequirementsKHR_volkImpl");
#endif /* defined(VK_KHR_maintenance4) */
#if defined(VK_KHR_maintenance5)
	vkCmdBindIndexBuffer2KHR_volkImpl = (PFN_vkCmdBindIndexBuffer2KHR)load(context, "vkCmdBindIndexBuffer2KHR_volkImpl");
	vkGetDeviceImageSubresourceLayoutKHR_volkImpl = (PFN_vkGetDeviceImageSubresourceLayoutKHR)load(context, "vkGetDeviceImageSubresourceLayoutKHR_volkImpl");
	vkGetImageSubresourceLayout2KHR_volkImpl = (PFN_vkGetImageSubresourceLayout2KHR)load(context, "vkGetImageSubresourceLayout2KHR_volkImpl");
	vkGetRenderingAreaGranularityKHR_volkImpl = (PFN_vkGetRenderingAreaGranularityKHR)load(context, "vkGetRenderingAreaGranularityKHR_volkImpl");
#endif /* defined(VK_KHR_maintenance5) */
#if defined(VK_KHR_maintenance6)
	vkCmdBindDescriptorSets2KHR_volkImpl = (PFN_vkCmdBindDescriptorSets2KHR)load(context, "vkCmdBindDescriptorSets2KHR_volkImpl");
	vkCmdPushConstants2KHR_volkImpl = (PFN_vkCmdPushConstants2KHR)load(context, "vkCmdPushConstants2KHR_volkImpl");
#endif /* defined(VK_KHR_maintenance6) */
#if defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor)
	vkCmdPushDescriptorSet2KHR_volkImpl = (PFN_vkCmdPushDescriptorSet2KHR)load(context, "vkCmdPushDescriptorSet2KHR_volkImpl");
	vkCmdPushDescriptorSetWithTemplate2KHR_volkImpl = (PFN_vkCmdPushDescriptorSetWithTemplate2KHR)load(context, "vkCmdPushDescriptorSetWithTemplate2KHR_volkImpl");
#endif /* defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer)
	vkCmdBindDescriptorBufferEmbeddedSamplers2EXT_volkImpl = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT)load(context, "vkCmdBindDescriptorBufferEmbeddedSamplers2EXT_volkImpl");
	vkCmdSetDescriptorBufferOffsets2EXT_volkImpl = (PFN_vkCmdSetDescriptorBufferOffsets2EXT)load(context, "vkCmdSetDescriptorBufferOffsets2EXT_volkImpl");
#endif /* defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer) */
#if defined(VK_KHR_map_memory2)
	vkMapMemory2KHR_volkImpl = (PFN_vkMapMemory2KHR)load(context, "vkMapMemory2KHR_volkImpl");
	vkUnmapMemory2KHR_volkImpl = (PFN_vkUnmapMemory2KHR)load(context, "vkUnmapMemory2KHR_volkImpl");
#endif /* defined(VK_KHR_map_memory2) */
#if defined(VK_KHR_performance_query)
	vkAcquireProfilingLockKHR_volkImpl = (PFN_vkAcquireProfilingLockKHR)load(context, "vkAcquireProfilingLockKHR_volkImpl");
	vkReleaseProfilingLockKHR_volkImpl = (PFN_vkReleaseProfilingLockKHR)load(context, "vkReleaseProfilingLockKHR_volkImpl");
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_pipeline_executable_properties)
	vkGetPipelineExecutableInternalRepresentationsKHR_volkImpl = (PFN_vkGetPipelineExecutableInternalRepresentationsKHR)load(context, "vkGetPipelineExecutableInternalRepresentationsKHR_volkImpl");
	vkGetPipelineExecutablePropertiesKHR_volkImpl = (PFN_vkGetPipelineExecutablePropertiesKHR)load(context, "vkGetPipelineExecutablePropertiesKHR_volkImpl");
	vkGetPipelineExecutableStatisticsKHR_volkImpl = (PFN_vkGetPipelineExecutableStatisticsKHR)load(context, "vkGetPipelineExecutableStatisticsKHR_volkImpl");
#endif /* defined(VK_KHR_pipeline_executable_properties) */
#if defined(VK_KHR_present_wait)
	vkWaitForPresentKHR_volkImpl = (PFN_vkWaitForPresentKHR)load(context, "vkWaitForPresentKHR_volkImpl");
#endif /* defined(VK_KHR_present_wait) */
#if defined(VK_KHR_push_descriptor)
	vkCmdPushDescriptorSetKHR_volkImpl = (PFN_vkCmdPushDescriptorSetKHR)load(context, "vkCmdPushDescriptorSetKHR_volkImpl");
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline)
	vkCmdTraceRaysIndirect2KHR_volkImpl = (PFN_vkCmdTraceRaysIndirect2KHR)load(context, "vkCmdTraceRaysIndirect2KHR_volkImpl");
#endif /* defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_ray_tracing_pipeline)
	vkCmdSetRayTracingPipelineStackSizeKHR_volkImpl = (PFN_vkCmdSetRayTracingPipelineStackSizeKHR)load(context, "vkCmdSetRayTracingPipelineStackSizeKHR_volkImpl");
	vkCmdTraceRaysIndirectKHR_volkImpl = (PFN_vkCmdTraceRaysIndirectKHR)load(context, "vkCmdTraceRaysIndirectKHR_volkImpl");
	vkCmdTraceRaysKHR_volkImpl = (PFN_vkCmdTraceRaysKHR)load(context, "vkCmdTraceRaysKHR_volkImpl");
	vkCreateRayTracingPipelinesKHR_volkImpl = (PFN_vkCreateRayTracingPipelinesKHR)load(context, "vkCreateRayTracingPipelinesKHR_volkImpl");
	vkGetRayTracingCaptureReplayShaderGroupHandlesKHR_volkImpl = (PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR)load(context, "vkGetRayTracingCaptureReplayShaderGroupHandlesKHR_volkImpl");
	vkGetRayTracingShaderGroupHandlesKHR_volkImpl = (PFN_vkGetRayTracingShaderGroupHandlesKHR)load(context, "vkGetRayTracingShaderGroupHandlesKHR_volkImpl");
	vkGetRayTracingShaderGroupStackSizeKHR_volkImpl = (PFN_vkGetRayTracingShaderGroupStackSizeKHR)load(context, "vkGetRayTracingShaderGroupStackSizeKHR_volkImpl");
#endif /* defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
	vkCreateSamplerYcbcrConversionKHR_volkImpl = (PFN_vkCreateSamplerYcbcrConversionKHR)load(context, "vkCreateSamplerYcbcrConversionKHR_volkImpl");
	vkDestroySamplerYcbcrConversionKHR_volkImpl = (PFN_vkDestroySamplerYcbcrConversionKHR)load(context, "vkDestroySamplerYcbcrConversionKHR_volkImpl");
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
	vkGetSwapchainStatusKHR_volkImpl = (PFN_vkGetSwapchainStatusKHR)load(context, "vkGetSwapchainStatusKHR_volkImpl");
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_swapchain)
	vkAcquireNextImageKHR_volkImpl = (PFN_vkAcquireNextImageKHR)load(context, "vkAcquireNextImageKHR_volkImpl");
	vkCreateSwapchainKHR_volkImpl = (PFN_vkCreateSwapchainKHR)load(context, "vkCreateSwapchainKHR_volkImpl");
	vkDestroySwapchainKHR_volkImpl = (PFN_vkDestroySwapchainKHR)load(context, "vkDestroySwapchainKHR_volkImpl");
	vkGetSwapchainImagesKHR_volkImpl = (PFN_vkGetSwapchainImagesKHR)load(context, "vkGetSwapchainImagesKHR_volkImpl");
	vkQueuePresentKHR_volkImpl = (PFN_vkQueuePresentKHR)load(context, "vkQueuePresentKHR_volkImpl");
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_synchronization2)
	vkCmdPipelineBarrier2KHR_volkImpl = (PFN_vkCmdPipelineBarrier2KHR)load(context, "vkCmdPipelineBarrier2KHR_volkImpl");
	vkCmdResetEvent2KHR_volkImpl = (PFN_vkCmdResetEvent2KHR)load(context, "vkCmdResetEvent2KHR_volkImpl");
	vkCmdSetEvent2KHR_volkImpl = (PFN_vkCmdSetEvent2KHR)load(context, "vkCmdSetEvent2KHR_volkImpl");
	vkCmdWaitEvents2KHR_volkImpl = (PFN_vkCmdWaitEvents2KHR)load(context, "vkCmdWaitEvents2KHR_volkImpl");
	vkCmdWriteTimestamp2KHR_volkImpl = (PFN_vkCmdWriteTimestamp2KHR)load(context, "vkCmdWriteTimestamp2KHR_volkImpl");
	vkQueueSubmit2KHR_volkImpl = (PFN_vkQueueSubmit2KHR)load(context, "vkQueueSubmit2KHR_volkImpl");
#endif /* defined(VK_KHR_synchronization2) */
#if defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker)
	vkCmdWriteBufferMarker2AMD_volkImpl = (PFN_vkCmdWriteBufferMarker2AMD)load(context, "vkCmdWriteBufferMarker2AMD_volkImpl");
#endif /* defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker) */
#if defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints)
	vkGetQueueCheckpointData2NV_volkImpl = (PFN_vkGetQueueCheckpointData2NV)load(context, "vkGetQueueCheckpointData2NV_volkImpl");
#endif /* defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_KHR_timeline_semaphore)
	vkGetSemaphoreCounterValueKHR_volkImpl = (PFN_vkGetSemaphoreCounterValueKHR)load(context, "vkGetSemaphoreCounterValueKHR_volkImpl");
	vkSignalSemaphoreKHR_volkImpl = (PFN_vkSignalSemaphoreKHR)load(context, "vkSignalSemaphoreKHR_volkImpl");
	vkWaitSemaphoresKHR_volkImpl = (PFN_vkWaitSemaphoresKHR)load(context, "vkWaitSemaphoresKHR_volkImpl");
#endif /* defined(VK_KHR_timeline_semaphore) */
#if defined(VK_KHR_video_decode_queue)
	vkCmdDecodeVideoKHR_volkImpl = (PFN_vkCmdDecodeVideoKHR)load(context, "vkCmdDecodeVideoKHR_volkImpl");
#endif /* defined(VK_KHR_video_decode_queue) */
#if defined(VK_KHR_video_encode_queue)
	vkCmdEncodeVideoKHR_volkImpl = (PFN_vkCmdEncodeVideoKHR)load(context, "vkCmdEncodeVideoKHR_volkImpl");
	vkGetEncodedVideoSessionParametersKHR_volkImpl = (PFN_vkGetEncodedVideoSessionParametersKHR)load(context, "vkGetEncodedVideoSessionParametersKHR_volkImpl");
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
	vkBindVideoSessionMemoryKHR_volkImpl = (PFN_vkBindVideoSessionMemoryKHR)load(context, "vkBindVideoSessionMemoryKHR_volkImpl");
	vkCmdBeginVideoCodingKHR_volkImpl = (PFN_vkCmdBeginVideoCodingKHR)load(context, "vkCmdBeginVideoCodingKHR_volkImpl");
	vkCmdControlVideoCodingKHR_volkImpl = (PFN_vkCmdControlVideoCodingKHR)load(context, "vkCmdControlVideoCodingKHR_volkImpl");
	vkCmdEndVideoCodingKHR_volkImpl = (PFN_vkCmdEndVideoCodingKHR)load(context, "vkCmdEndVideoCodingKHR_volkImpl");
	vkCreateVideoSessionKHR_volkImpl = (PFN_vkCreateVideoSessionKHR)load(context, "vkCreateVideoSessionKHR_volkImpl");
	vkCreateVideoSessionParametersKHR_volkImpl = (PFN_vkCreateVideoSessionParametersKHR)load(context, "vkCreateVideoSessionParametersKHR_volkImpl");
	vkDestroyVideoSessionKHR_volkImpl = (PFN_vkDestroyVideoSessionKHR)load(context, "vkDestroyVideoSessionKHR_volkImpl");
	vkDestroyVideoSessionParametersKHR_volkImpl = (PFN_vkDestroyVideoSessionParametersKHR)load(context, "vkDestroyVideoSessionParametersKHR_volkImpl");
	vkGetVideoSessionMemoryRequirementsKHR_volkImpl = (PFN_vkGetVideoSessionMemoryRequirementsKHR)load(context, "vkGetVideoSessionMemoryRequirementsKHR_volkImpl");
	vkUpdateVideoSessionParametersKHR_volkImpl = (PFN_vkUpdateVideoSessionParametersKHR)load(context, "vkUpdateVideoSessionParametersKHR_volkImpl");
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_NVX_binary_import)
	vkCmdCuLaunchKernelNVX_volkImpl = (PFN_vkCmdCuLaunchKernelNVX)load(context, "vkCmdCuLaunchKernelNVX_volkImpl");
	vkCreateCuFunctionNVX_volkImpl = (PFN_vkCreateCuFunctionNVX)load(context, "vkCreateCuFunctionNVX_volkImpl");
	vkCreateCuModuleNVX_volkImpl = (PFN_vkCreateCuModuleNVX)load(context, "vkCreateCuModuleNVX_volkImpl");
	vkDestroyCuFunctionNVX_volkImpl = (PFN_vkDestroyCuFunctionNVX)load(context, "vkDestroyCuFunctionNVX_volkImpl");
	vkDestroyCuModuleNVX_volkImpl = (PFN_vkDestroyCuModuleNVX)load(context, "vkDestroyCuModuleNVX_volkImpl");
#endif /* defined(VK_NVX_binary_import) */
#if defined(VK_NVX_image_view_handle)
	vkGetImageViewAddressNVX_volkImpl = (PFN_vkGetImageViewAddressNVX)load(context, "vkGetImageViewAddressNVX_volkImpl");
	vkGetImageViewHandleNVX_volkImpl = (PFN_vkGetImageViewHandleNVX)load(context, "vkGetImageViewHandleNVX_volkImpl");
#endif /* defined(VK_NVX_image_view_handle) */
#if defined(VK_NV_clip_space_w_scaling)
	vkCmdSetViewportWScalingNV_volkImpl = (PFN_vkCmdSetViewportWScalingNV)load(context, "vkCmdSetViewportWScalingNV_volkImpl");
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_copy_memory_indirect)
	vkCmdCopyMemoryIndirectNV_volkImpl = (PFN_vkCmdCopyMemoryIndirectNV)load(context, "vkCmdCopyMemoryIndirectNV_volkImpl");
	vkCmdCopyMemoryToImageIndirectNV_volkImpl = (PFN_vkCmdCopyMemoryToImageIndirectNV)load(context, "vkCmdCopyMemoryToImageIndirectNV_volkImpl");
#endif /* defined(VK_NV_copy_memory_indirect) */
#if defined(VK_NV_cuda_kernel_launch)
	vkCmdCudaLaunchKernelNV_volkImpl = (PFN_vkCmdCudaLaunchKernelNV)load(context, "vkCmdCudaLaunchKernelNV_volkImpl");
	vkCreateCudaFunctionNV_volkImpl = (PFN_vkCreateCudaFunctionNV)load(context, "vkCreateCudaFunctionNV_volkImpl");
	vkCreateCudaModuleNV_volkImpl = (PFN_vkCreateCudaModuleNV)load(context, "vkCreateCudaModuleNV_volkImpl");
	vkDestroyCudaFunctionNV_volkImpl = (PFN_vkDestroyCudaFunctionNV)load(context, "vkDestroyCudaFunctionNV_volkImpl");
	vkDestroyCudaModuleNV_volkImpl = (PFN_vkDestroyCudaModuleNV)load(context, "vkDestroyCudaModuleNV_volkImpl");
	vkGetCudaModuleCacheNV_volkImpl = (PFN_vkGetCudaModuleCacheNV)load(context, "vkGetCudaModuleCacheNV_volkImpl");
#endif /* defined(VK_NV_cuda_kernel_launch) */
#if defined(VK_NV_device_diagnostic_checkpoints)
	vkCmdSetCheckpointNV_volkImpl = (PFN_vkCmdSetCheckpointNV)load(context, "vkCmdSetCheckpointNV_volkImpl");
	vkGetQueueCheckpointDataNV_volkImpl = (PFN_vkGetQueueCheckpointDataNV)load(context, "vkGetQueueCheckpointDataNV_volkImpl");
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_device_generated_commands)
	vkCmdBindPipelineShaderGroupNV_volkImpl = (PFN_vkCmdBindPipelineShaderGroupNV)load(context, "vkCmdBindPipelineShaderGroupNV_volkImpl");
	vkCmdExecuteGeneratedCommandsNV_volkImpl = (PFN_vkCmdExecuteGeneratedCommandsNV)load(context, "vkCmdExecuteGeneratedCommandsNV_volkImpl");
	vkCmdPreprocessGeneratedCommandsNV_volkImpl = (PFN_vkCmdPreprocessGeneratedCommandsNV)load(context, "vkCmdPreprocessGeneratedCommandsNV_volkImpl");
	vkCreateIndirectCommandsLayoutNV_volkImpl = (PFN_vkCreateIndirectCommandsLayoutNV)load(context, "vkCreateIndirectCommandsLayoutNV_volkImpl");
	vkDestroyIndirectCommandsLayoutNV_volkImpl = (PFN_vkDestroyIndirectCommandsLayoutNV)load(context, "vkDestroyIndirectCommandsLayoutNV_volkImpl");
	vkGetGeneratedCommandsMemoryRequirementsNV_volkImpl = (PFN_vkGetGeneratedCommandsMemoryRequirementsNV)load(context, "vkGetGeneratedCommandsMemoryRequirementsNV_volkImpl");
#endif /* defined(VK_NV_device_generated_commands) */
#if defined(VK_NV_device_generated_commands_compute)
	vkCmdUpdatePipelineIndirectBufferNV_volkImpl = (PFN_vkCmdUpdatePipelineIndirectBufferNV)load(context, "vkCmdUpdatePipelineIndirectBufferNV_volkImpl");
	vkGetPipelineIndirectDeviceAddressNV_volkImpl = (PFN_vkGetPipelineIndirectDeviceAddressNV)load(context, "vkGetPipelineIndirectDeviceAddressNV_volkImpl");
	vkGetPipelineIndirectMemoryRequirementsNV_volkImpl = (PFN_vkGetPipelineIndirectMemoryRequirementsNV)load(context, "vkGetPipelineIndirectMemoryRequirementsNV_volkImpl");
#endif /* defined(VK_NV_device_generated_commands_compute) */
#if defined(VK_NV_external_memory_rdma)
	vkGetMemoryRemoteAddressNV_volkImpl = (PFN_vkGetMemoryRemoteAddressNV)load(context, "vkGetMemoryRemoteAddressNV_volkImpl");
#endif /* defined(VK_NV_external_memory_rdma) */
#if defined(VK_NV_external_memory_win32)
	vkGetMemoryWin32HandleNV_volkImpl = (PFN_vkGetMemoryWin32HandleNV)load(context, "vkGetMemoryWin32HandleNV_volkImpl");
#endif /* defined(VK_NV_external_memory_win32) */
#if defined(VK_NV_fragment_shading_rate_enums)
	vkCmdSetFragmentShadingRateEnumNV_volkImpl = (PFN_vkCmdSetFragmentShadingRateEnumNV)load(context, "vkCmdSetFragmentShadingRateEnumNV_volkImpl");
#endif /* defined(VK_NV_fragment_shading_rate_enums) */
#if defined(VK_NV_low_latency2)
	vkGetLatencyTimingsNV_volkImpl = (PFN_vkGetLatencyTimingsNV)load(context, "vkGetLatencyTimingsNV_volkImpl");
	vkLatencySleepNV_volkImpl = (PFN_vkLatencySleepNV)load(context, "vkLatencySleepNV_volkImpl");
	vkQueueNotifyOutOfBandNV_volkImpl = (PFN_vkQueueNotifyOutOfBandNV)load(context, "vkQueueNotifyOutOfBandNV_volkImpl");
	vkSetLatencyMarkerNV_volkImpl = (PFN_vkSetLatencyMarkerNV)load(context, "vkSetLatencyMarkerNV_volkImpl");
	vkSetLatencySleepModeNV_volkImpl = (PFN_vkSetLatencySleepModeNV)load(context, "vkSetLatencySleepModeNV_volkImpl");
#endif /* defined(VK_NV_low_latency2) */
#if defined(VK_NV_memory_decompression)
	vkCmdDecompressMemoryIndirectCountNV_volkImpl = (PFN_vkCmdDecompressMemoryIndirectCountNV)load(context, "vkCmdDecompressMemoryIndirectCountNV_volkImpl");
	vkCmdDecompressMemoryNV_volkImpl = (PFN_vkCmdDecompressMemoryNV)load(context, "vkCmdDecompressMemoryNV_volkImpl");
#endif /* defined(VK_NV_memory_decompression) */
#if defined(VK_NV_mesh_shader)
	vkCmdDrawMeshTasksIndirectCountNV_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectCountNV)load(context, "vkCmdDrawMeshTasksIndirectCountNV_volkImpl");
	vkCmdDrawMeshTasksIndirectNV_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectNV)load(context, "vkCmdDrawMeshTasksIndirectNV_volkImpl");
	vkCmdDrawMeshTasksNV_volkImpl = (PFN_vkCmdDrawMeshTasksNV)load(context, "vkCmdDrawMeshTasksNV_volkImpl");
#endif /* defined(VK_NV_mesh_shader) */
#if defined(VK_NV_optical_flow)
	vkBindOpticalFlowSessionImageNV_volkImpl = (PFN_vkBindOpticalFlowSessionImageNV)load(context, "vkBindOpticalFlowSessionImageNV_volkImpl");
	vkCmdOpticalFlowExecuteNV_volkImpl = (PFN_vkCmdOpticalFlowExecuteNV)load(context, "vkCmdOpticalFlowExecuteNV_volkImpl");
	vkCreateOpticalFlowSessionNV_volkImpl = (PFN_vkCreateOpticalFlowSessionNV)load(context, "vkCreateOpticalFlowSessionNV_volkImpl");
	vkDestroyOpticalFlowSessionNV_volkImpl = (PFN_vkDestroyOpticalFlowSessionNV)load(context, "vkDestroyOpticalFlowSessionNV_volkImpl");
#endif /* defined(VK_NV_optical_flow) */
#if defined(VK_NV_ray_tracing)
	vkBindAccelerationStructureMemoryNV_volkImpl = (PFN_vkBindAccelerationStructureMemoryNV)load(context, "vkBindAccelerationStructureMemoryNV_volkImpl");
	vkCmdBuildAccelerationStructureNV_volkImpl = (PFN_vkCmdBuildAccelerationStructureNV)load(context, "vkCmdBuildAccelerationStructureNV_volkImpl");
	vkCmdCopyAccelerationStructureNV_volkImpl = (PFN_vkCmdCopyAccelerationStructureNV)load(context, "vkCmdCopyAccelerationStructureNV_volkImpl");
	vkCmdTraceRaysNV_volkImpl = (PFN_vkCmdTraceRaysNV)load(context, "vkCmdTraceRaysNV_volkImpl");
	vkCmdWriteAccelerationStructuresPropertiesNV_volkImpl = (PFN_vkCmdWriteAccelerationStructuresPropertiesNV)load(context, "vkCmdWriteAccelerationStructuresPropertiesNV_volkImpl");
	vkCompileDeferredNV_volkImpl = (PFN_vkCompileDeferredNV)load(context, "vkCompileDeferredNV_volkImpl");
	vkCreateAccelerationStructureNV_volkImpl = (PFN_vkCreateAccelerationStructureNV)load(context, "vkCreateAccelerationStructureNV_volkImpl");
	vkCreateRayTracingPipelinesNV_volkImpl = (PFN_vkCreateRayTracingPipelinesNV)load(context, "vkCreateRayTracingPipelinesNV_volkImpl");
	vkDestroyAccelerationStructureNV_volkImpl = (PFN_vkDestroyAccelerationStructureNV)load(context, "vkDestroyAccelerationStructureNV_volkImpl");
	vkGetAccelerationStructureHandleNV_volkImpl = (PFN_vkGetAccelerationStructureHandleNV)load(context, "vkGetAccelerationStructureHandleNV_volkImpl");
	vkGetAccelerationStructureMemoryRequirementsNV_volkImpl = (PFN_vkGetAccelerationStructureMemoryRequirementsNV)load(context, "vkGetAccelerationStructureMemoryRequirementsNV_volkImpl");
	vkGetRayTracingShaderGroupHandlesNV_volkImpl = (PFN_vkGetRayTracingShaderGroupHandlesNV)load(context, "vkGetRayTracingShaderGroupHandlesNV_volkImpl");
#endif /* defined(VK_NV_ray_tracing) */
#if defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2
	vkCmdSetExclusiveScissorEnableNV_volkImpl = (PFN_vkCmdSetExclusiveScissorEnableNV)load(context, "vkCmdSetExclusiveScissorEnableNV_volkImpl");
#endif /* defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2 */
#if defined(VK_NV_scissor_exclusive)
	vkCmdSetExclusiveScissorNV_volkImpl = (PFN_vkCmdSetExclusiveScissorNV)load(context, "vkCmdSetExclusiveScissorNV_volkImpl");
#endif /* defined(VK_NV_scissor_exclusive) */
#if defined(VK_NV_shading_rate_image)
	vkCmdBindShadingRateImageNV_volkImpl = (PFN_vkCmdBindShadingRateImageNV)load(context, "vkCmdBindShadingRateImageNV_volkImpl");
	vkCmdSetCoarseSampleOrderNV_volkImpl = (PFN_vkCmdSetCoarseSampleOrderNV)load(context, "vkCmdSetCoarseSampleOrderNV_volkImpl");
	vkCmdSetViewportShadingRatePaletteNV_volkImpl = (PFN_vkCmdSetViewportShadingRatePaletteNV)load(context, "vkCmdSetViewportShadingRatePaletteNV_volkImpl");
#endif /* defined(VK_NV_shading_rate_image) */
#if defined(VK_QCOM_tile_properties)
	vkGetDynamicRenderingTilePropertiesQCOM_volkImpl = (PFN_vkGetDynamicRenderingTilePropertiesQCOM)load(context, "vkGetDynamicRenderingTilePropertiesQCOM_volkImpl");
	vkGetFramebufferTilePropertiesQCOM_volkImpl = (PFN_vkGetFramebufferTilePropertiesQCOM)load(context, "vkGetFramebufferTilePropertiesQCOM_volkImpl");
#endif /* defined(VK_QCOM_tile_properties) */
#if defined(VK_QNX_external_memory_screen_buffer)
	vkGetScreenBufferPropertiesQNX_volkImpl = (PFN_vkGetScreenBufferPropertiesQNX)load(context, "vkGetScreenBufferPropertiesQNX_volkImpl");
#endif /* defined(VK_QNX_external_memory_screen_buffer) */
#if defined(VK_VALVE_descriptor_set_host_mapping)
	vkGetDescriptorSetHostMappingVALVE_volkImpl = (PFN_vkGetDescriptorSetHostMappingVALVE)load(context, "vkGetDescriptorSetHostMappingVALVE_volkImpl");
	vkGetDescriptorSetLayoutHostMappingInfoVALVE_volkImpl = (PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE)load(context, "vkGetDescriptorSetLayoutHostMappingInfoVALVE_volkImpl");
#endif /* defined(VK_VALVE_descriptor_set_host_mapping) */
#if (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object))
	vkCmdBindVertexBuffers2EXT_volkImpl = (PFN_vkCmdBindVertexBuffers2EXT)load(context, "vkCmdBindVertexBuffers2EXT_volkImpl");
	vkCmdSetCullModeEXT_volkImpl = (PFN_vkCmdSetCullModeEXT)load(context, "vkCmdSetCullModeEXT_volkImpl");
	vkCmdSetDepthBoundsTestEnableEXT_volkImpl = (PFN_vkCmdSetDepthBoundsTestEnableEXT)load(context, "vkCmdSetDepthBoundsTestEnableEXT_volkImpl");
	vkCmdSetDepthCompareOpEXT_volkImpl = (PFN_vkCmdSetDepthCompareOpEXT)load(context, "vkCmdSetDepthCompareOpEXT_volkImpl");
	vkCmdSetDepthTestEnableEXT_volkImpl = (PFN_vkCmdSetDepthTestEnableEXT)load(context, "vkCmdSetDepthTestEnableEXT_volkImpl");
	vkCmdSetDepthWriteEnableEXT_volkImpl = (PFN_vkCmdSetDepthWriteEnableEXT)load(context, "vkCmdSetDepthWriteEnableEXT_volkImpl");
	vkCmdSetFrontFaceEXT_volkImpl = (PFN_vkCmdSetFrontFaceEXT)load(context, "vkCmdSetFrontFaceEXT_volkImpl");
	vkCmdSetPrimitiveTopologyEXT_volkImpl = (PFN_vkCmdSetPrimitiveTopologyEXT)load(context, "vkCmdSetPrimitiveTopologyEXT_volkImpl");
	vkCmdSetScissorWithCountEXT_volkImpl = (PFN_vkCmdSetScissorWithCountEXT)load(context, "vkCmdSetScissorWithCountEXT_volkImpl");
	vkCmdSetStencilOpEXT_volkImpl = (PFN_vkCmdSetStencilOpEXT)load(context, "vkCmdSetStencilOpEXT_volkImpl");
	vkCmdSetStencilTestEnableEXT_volkImpl = (PFN_vkCmdSetStencilTestEnableEXT)load(context, "vkCmdSetStencilTestEnableEXT_volkImpl");
	vkCmdSetViewportWithCountEXT_volkImpl = (PFN_vkCmdSetViewportWithCountEXT)load(context, "vkCmdSetViewportWithCountEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object))
	vkCmdSetDepthBiasEnableEXT_volkImpl = (PFN_vkCmdSetDepthBiasEnableEXT)load(context, "vkCmdSetDepthBiasEnableEXT_volkImpl");
	vkCmdSetLogicOpEXT_volkImpl = (PFN_vkCmdSetLogicOpEXT)load(context, "vkCmdSetLogicOpEXT_volkImpl");
	vkCmdSetPatchControlPointsEXT_volkImpl = (PFN_vkCmdSetPatchControlPointsEXT)load(context, "vkCmdSetPatchControlPointsEXT_volkImpl");
	vkCmdSetPrimitiveRestartEnableEXT_volkImpl = (PFN_vkCmdSetPrimitiveRestartEnableEXT)load(context, "vkCmdSetPrimitiveRestartEnableEXT_volkImpl");
	vkCmdSetRasterizerDiscardEnableEXT_volkImpl = (PFN_vkCmdSetRasterizerDiscardEnableEXT)load(context, "vkCmdSetRasterizerDiscardEnableEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object))
	vkCmdSetAlphaToCoverageEnableEXT_volkImpl = (PFN_vkCmdSetAlphaToCoverageEnableEXT)load(context, "vkCmdSetAlphaToCoverageEnableEXT_volkImpl");
	vkCmdSetAlphaToOneEnableEXT_volkImpl = (PFN_vkCmdSetAlphaToOneEnableEXT)load(context, "vkCmdSetAlphaToOneEnableEXT_volkImpl");
	vkCmdSetColorBlendEnableEXT_volkImpl = (PFN_vkCmdSetColorBlendEnableEXT)load(context, "vkCmdSetColorBlendEnableEXT_volkImpl");
	vkCmdSetColorBlendEquationEXT_volkImpl = (PFN_vkCmdSetColorBlendEquationEXT)load(context, "vkCmdSetColorBlendEquationEXT_volkImpl");
	vkCmdSetColorWriteMaskEXT_volkImpl = (PFN_vkCmdSetColorWriteMaskEXT)load(context, "vkCmdSetColorWriteMaskEXT_volkImpl");
	vkCmdSetDepthClampEnableEXT_volkImpl = (PFN_vkCmdSetDepthClampEnableEXT)load(context, "vkCmdSetDepthClampEnableEXT_volkImpl");
	vkCmdSetLogicOpEnableEXT_volkImpl = (PFN_vkCmdSetLogicOpEnableEXT)load(context, "vkCmdSetLogicOpEnableEXT_volkImpl");
	vkCmdSetPolygonModeEXT_volkImpl = (PFN_vkCmdSetPolygonModeEXT)load(context, "vkCmdSetPolygonModeEXT_volkImpl");
	vkCmdSetRasterizationSamplesEXT_volkImpl = (PFN_vkCmdSetRasterizationSamplesEXT)load(context, "vkCmdSetRasterizationSamplesEXT_volkImpl");
	vkCmdSetSampleMaskEXT_volkImpl = (PFN_vkCmdSetSampleMaskEXT)load(context, "vkCmdSetSampleMaskEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object))
	vkCmdSetTessellationDomainOriginEXT_volkImpl = (PFN_vkCmdSetTessellationDomainOriginEXT)load(context, "vkCmdSetTessellationDomainOriginEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback))
	vkCmdSetRasterizationStreamEXT_volkImpl = (PFN_vkCmdSetRasterizationStreamEXT)load(context, "vkCmdSetRasterizationStreamEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization))
	vkCmdSetConservativeRasterizationModeEXT_volkImpl = (PFN_vkCmdSetConservativeRasterizationModeEXT)load(context, "vkCmdSetConservativeRasterizationModeEXT_volkImpl");
	vkCmdSetExtraPrimitiveOverestimationSizeEXT_volkImpl = (PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT)load(context, "vkCmdSetExtraPrimitiveOverestimationSizeEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable))
	vkCmdSetDepthClipEnableEXT_volkImpl = (PFN_vkCmdSetDepthClipEnableEXT)load(context, "vkCmdSetDepthClipEnableEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations))
	vkCmdSetSampleLocationsEnableEXT_volkImpl = (PFN_vkCmdSetSampleLocationsEnableEXT)load(context, "vkCmdSetSampleLocationsEnableEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced))
	vkCmdSetColorBlendAdvancedEXT_volkImpl = (PFN_vkCmdSetColorBlendAdvancedEXT)load(context, "vkCmdSetColorBlendAdvancedEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex))
	vkCmdSetProvokingVertexModeEXT_volkImpl = (PFN_vkCmdSetProvokingVertexModeEXT)load(context, "vkCmdSetProvokingVertexModeEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization))
	vkCmdSetLineRasterizationModeEXT_volkImpl = (PFN_vkCmdSetLineRasterizationModeEXT)load(context, "vkCmdSetLineRasterizationModeEXT_volkImpl");
	vkCmdSetLineStippleEnableEXT_volkImpl = (PFN_vkCmdSetLineStippleEnableEXT)load(context, "vkCmdSetLineStippleEnableEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control))
	vkCmdSetDepthClipNegativeOneToOneEXT_volkImpl = (PFN_vkCmdSetDepthClipNegativeOneToOneEXT)load(context, "vkCmdSetDepthClipNegativeOneToOneEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling))
	vkCmdSetViewportWScalingEnableNV_volkImpl = (PFN_vkCmdSetViewportWScalingEnableNV)load(context, "vkCmdSetViewportWScalingEnableNV_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle))
	vkCmdSetViewportSwizzleNV_volkImpl = (PFN_vkCmdSetViewportSwizzleNV)load(context, "vkCmdSetViewportSwizzleNV_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color))
	vkCmdSetCoverageToColorEnableNV_volkImpl = (PFN_vkCmdSetCoverageToColorEnableNV)load(context, "vkCmdSetCoverageToColorEnableNV_volkImpl");
	vkCmdSetCoverageToColorLocationNV_volkImpl = (PFN_vkCmdSetCoverageToColorLocationNV)load(context, "vkCmdSetCoverageToColorLocationNV_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples))
	vkCmdSetCoverageModulationModeNV_volkImpl = (PFN_vkCmdSetCoverageModulationModeNV)load(context, "vkCmdSetCoverageModulationModeNV_volkImpl");
	vkCmdSetCoverageModulationTableEnableNV_volkImpl = (PFN_vkCmdSetCoverageModulationTableEnableNV)load(context, "vkCmdSetCoverageModulationTableEnableNV_volkImpl");
	vkCmdSetCoverageModulationTableNV_volkImpl = (PFN_vkCmdSetCoverageModulationTableNV)load(context, "vkCmdSetCoverageModulationTableNV_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image))
	vkCmdSetShadingRateImageEnableNV_volkImpl = (PFN_vkCmdSetShadingRateImageEnableNV)load(context, "vkCmdSetShadingRateImageEnableNV_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test))
	vkCmdSetRepresentativeFragmentTestEnableNV_volkImpl = (PFN_vkCmdSetRepresentativeFragmentTestEnableNV)load(context, "vkCmdSetRepresentativeFragmentTestEnableNV_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode))
	vkCmdSetCoverageReductionModeNV_volkImpl = (PFN_vkCmdSetCoverageReductionModeNV)load(context, "vkCmdSetCoverageReductionModeNV_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode)) */
#if (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1))
	vkGetDeviceGroupSurfacePresentModes2EXT_volkImpl = (PFN_vkGetDeviceGroupSurfacePresentModes2EXT)load(context, "vkGetDeviceGroupSurfacePresentModes2EXT_volkImpl");
#endif /* (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1)) */
#if (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control))
	vkGetImageSubresourceLayout2EXT_volkImpl = (PFN_vkGetImageSubresourceLayout2EXT)load(context, "vkGetImageSubresourceLayout2EXT_volkImpl");
#endif /* (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control)) */
#if (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state))
	vkCmdSetVertexInputEXT_volkImpl = (PFN_vkCmdSetVertexInputEXT)load(context, "vkCmdSetVertexInputEXT_volkImpl");
#endif /* (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state)) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template))
	vkCmdPushDescriptorSetWithTemplateKHR_volkImpl = (PFN_vkCmdPushDescriptorSetWithTemplateKHR)load(context, "vkCmdPushDescriptorSetWithTemplateKHR_volkImpl");
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	vkGetDeviceGroupPresentCapabilitiesKHR_volkImpl = (PFN_vkGetDeviceGroupPresentCapabilitiesKHR)load(context, "vkGetDeviceGroupPresentCapabilitiesKHR_volkImpl");
	vkGetDeviceGroupSurfacePresentModesKHR_volkImpl = (PFN_vkGetDeviceGroupSurfacePresentModesKHR)load(context, "vkGetDeviceGroupSurfacePresentModesKHR_volkImpl");
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	vkAcquireNextImage2KHR_volkImpl = (PFN_vkAcquireNextImage2KHR)load(context, "vkAcquireNextImage2KHR_volkImpl");
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
	/* VOLK_GENERATE_LOAD_DEVICE */
}

static void volkGenLoadDeviceTable(struct VolkDeviceTable* table, void* context, PFN_vkVoidFunction (*load)(void*, const char*))
{
	/* VOLK_GENERATE_LOAD_DEVICE_TABLE */
#if defined(VK_VERSION_1_0)
	table->vkAllocateCommandBuffers_volkImpl = (PFN_vkAllocateCommandBuffers)load(context, "vkAllocateCommandBuffers_volkImpl");
	table->vkAllocateDescriptorSets_volkImpl = (PFN_vkAllocateDescriptorSets)load(context, "vkAllocateDescriptorSets_volkImpl");
	table->vkAllocateMemory_volkImpl = (PFN_vkAllocateMemory)load(context, "vkAllocateMemory_volkImpl");
	table->vkBeginCommandBuffer_volkImpl = (PFN_vkBeginCommandBuffer)load(context, "vkBeginCommandBuffer_volkImpl");
	table->vkBindBufferMemory_volkImpl = (PFN_vkBindBufferMemory)load(context, "vkBindBufferMemory_volkImpl");
	table->vkBindImageMemory_volkImpl = (PFN_vkBindImageMemory)load(context, "vkBindImageMemory_volkImpl");
	table->vkCmdBeginQuery_volkImpl = (PFN_vkCmdBeginQuery)load(context, "vkCmdBeginQuery_volkImpl");
	table->vkCmdBeginRenderPass_volkImpl = (PFN_vkCmdBeginRenderPass)load(context, "vkCmdBeginRenderPass_volkImpl");
	table->vkCmdBindDescriptorSets_volkImpl = (PFN_vkCmdBindDescriptorSets)load(context, "vkCmdBindDescriptorSets_volkImpl");
	table->vkCmdBindIndexBuffer_volkImpl = (PFN_vkCmdBindIndexBuffer)load(context, "vkCmdBindIndexBuffer_volkImpl");
	table->vkCmdBindPipeline_volkImpl = (PFN_vkCmdBindPipeline)load(context, "vkCmdBindPipeline_volkImpl");
	table->vkCmdBindVertexBuffers_volkImpl = (PFN_vkCmdBindVertexBuffers)load(context, "vkCmdBindVertexBuffers_volkImpl");
	table->vkCmdBlitImage_volkImpl = (PFN_vkCmdBlitImage)load(context, "vkCmdBlitImage_volkImpl");
	table->vkCmdClearAttachments_volkImpl = (PFN_vkCmdClearAttachments)load(context, "vkCmdClearAttachments_volkImpl");
	table->vkCmdClearColorImage_volkImpl = (PFN_vkCmdClearColorImage)load(context, "vkCmdClearColorImage_volkImpl");
	table->vkCmdClearDepthStencilImage_volkImpl = (PFN_vkCmdClearDepthStencilImage)load(context, "vkCmdClearDepthStencilImage_volkImpl");
	table->vkCmdCopyBuffer_volkImpl = (PFN_vkCmdCopyBuffer)load(context, "vkCmdCopyBuffer_volkImpl");
	table->vkCmdCopyBufferToImage_volkImpl = (PFN_vkCmdCopyBufferToImage)load(context, "vkCmdCopyBufferToImage_volkImpl");
	table->vkCmdCopyImage_volkImpl = (PFN_vkCmdCopyImage)load(context, "vkCmdCopyImage_volkImpl");
	table->vkCmdCopyImageToBuffer_volkImpl = (PFN_vkCmdCopyImageToBuffer)load(context, "vkCmdCopyImageToBuffer_volkImpl");
	table->vkCmdCopyQueryPoolResults_volkImpl = (PFN_vkCmdCopyQueryPoolResults)load(context, "vkCmdCopyQueryPoolResults_volkImpl");
	table->vkCmdDispatch_volkImpl = (PFN_vkCmdDispatch)load(context, "vkCmdDispatch_volkImpl");
	table->vkCmdDispatchIndirect_volkImpl = (PFN_vkCmdDispatchIndirect)load(context, "vkCmdDispatchIndirect_volkImpl");
	table->vkCmdDraw_volkImpl = (PFN_vkCmdDraw)load(context, "vkCmdDraw_volkImpl");
	table->vkCmdDrawIndexed_volkImpl = (PFN_vkCmdDrawIndexed)load(context, "vkCmdDrawIndexed_volkImpl");
	table->vkCmdDrawIndexedIndirect_volkImpl = (PFN_vkCmdDrawIndexedIndirect)load(context, "vkCmdDrawIndexedIndirect_volkImpl");
	table->vkCmdDrawIndirect_volkImpl = (PFN_vkCmdDrawIndirect)load(context, "vkCmdDrawIndirect_volkImpl");
	table->vkCmdEndQuery_volkImpl = (PFN_vkCmdEndQuery)load(context, "vkCmdEndQuery_volkImpl");
	table->vkCmdEndRenderPass_volkImpl = (PFN_vkCmdEndRenderPass)load(context, "vkCmdEndRenderPass_volkImpl");
	table->vkCmdExecuteCommands_volkImpl = (PFN_vkCmdExecuteCommands)load(context, "vkCmdExecuteCommands_volkImpl");
	table->vkCmdFillBuffer_volkImpl = (PFN_vkCmdFillBuffer)load(context, "vkCmdFillBuffer_volkImpl");
	table->vkCmdNextSubpass_volkImpl = (PFN_vkCmdNextSubpass)load(context, "vkCmdNextSubpass_volkImpl");
	table->vkCmdPipelineBarrier_volkImpl = (PFN_vkCmdPipelineBarrier)load(context, "vkCmdPipelineBarrier_volkImpl");
	table->vkCmdPushConstants_volkImpl = (PFN_vkCmdPushConstants)load(context, "vkCmdPushConstants_volkImpl");
	table->vkCmdResetEvent_volkImpl = (PFN_vkCmdResetEvent)load(context, "vkCmdResetEvent_volkImpl");
	table->vkCmdResetQueryPool_volkImpl = (PFN_vkCmdResetQueryPool)load(context, "vkCmdResetQueryPool_volkImpl");
	table->vkCmdResolveImage_volkImpl = (PFN_vkCmdResolveImage)load(context, "vkCmdResolveImage_volkImpl");
	table->vkCmdSetBlendConstants_volkImpl = (PFN_vkCmdSetBlendConstants)load(context, "vkCmdSetBlendConstants_volkImpl");
	table->vkCmdSetDepthBias_volkImpl = (PFN_vkCmdSetDepthBias)load(context, "vkCmdSetDepthBias_volkImpl");
	table->vkCmdSetDepthBounds_volkImpl = (PFN_vkCmdSetDepthBounds)load(context, "vkCmdSetDepthBounds_volkImpl");
	table->vkCmdSetEvent_volkImpl = (PFN_vkCmdSetEvent)load(context, "vkCmdSetEvent_volkImpl");
	table->vkCmdSetLineWidth_volkImpl = (PFN_vkCmdSetLineWidth)load(context, "vkCmdSetLineWidth_volkImpl");
	table->vkCmdSetScissor_volkImpl = (PFN_vkCmdSetScissor)load(context, "vkCmdSetScissor_volkImpl");
	table->vkCmdSetStencilCompareMask_volkImpl = (PFN_vkCmdSetStencilCompareMask)load(context, "vkCmdSetStencilCompareMask_volkImpl");
	table->vkCmdSetStencilReference_volkImpl = (PFN_vkCmdSetStencilReference)load(context, "vkCmdSetStencilReference_volkImpl");
	table->vkCmdSetStencilWriteMask_volkImpl = (PFN_vkCmdSetStencilWriteMask)load(context, "vkCmdSetStencilWriteMask_volkImpl");
	table->vkCmdSetViewport_volkImpl = (PFN_vkCmdSetViewport)load(context, "vkCmdSetViewport_volkImpl");
	table->vkCmdUpdateBuffer_volkImpl = (PFN_vkCmdUpdateBuffer)load(context, "vkCmdUpdateBuffer_volkImpl");
	table->vkCmdWaitEvents_volkImpl = (PFN_vkCmdWaitEvents)load(context, "vkCmdWaitEvents_volkImpl");
	table->vkCmdWriteTimestamp_volkImpl = (PFN_vkCmdWriteTimestamp)load(context, "vkCmdWriteTimestamp_volkImpl");
	table->vkCreateBuffer_volkImpl = (PFN_vkCreateBuffer)load(context, "vkCreateBuffer_volkImpl");
	table->vkCreateBufferView_volkImpl = (PFN_vkCreateBufferView)load(context, "vkCreateBufferView_volkImpl");
	table->vkCreateCommandPool_volkImpl = (PFN_vkCreateCommandPool)load(context, "vkCreateCommandPool_volkImpl");
	table->vkCreateComputePipelines_volkImpl = (PFN_vkCreateComputePipelines)load(context, "vkCreateComputePipelines_volkImpl");
	table->vkCreateDescriptorPool_volkImpl = (PFN_vkCreateDescriptorPool)load(context, "vkCreateDescriptorPool_volkImpl");
	table->vkCreateDescriptorSetLayout_volkImpl = (PFN_vkCreateDescriptorSetLayout)load(context, "vkCreateDescriptorSetLayout_volkImpl");
	table->vkCreateEvent_volkImpl = (PFN_vkCreateEvent)load(context, "vkCreateEvent_volkImpl");
	table->vkCreateFence_volkImpl = (PFN_vkCreateFence)load(context, "vkCreateFence_volkImpl");
	table->vkCreateFramebuffer_volkImpl = (PFN_vkCreateFramebuffer)load(context, "vkCreateFramebuffer_volkImpl");
	table->vkCreateGraphicsPipelines_volkImpl = (PFN_vkCreateGraphicsPipelines)load(context, "vkCreateGraphicsPipelines_volkImpl");
	table->vkCreateImage_volkImpl = (PFN_vkCreateImage)load(context, "vkCreateImage_volkImpl");
	table->vkCreateImageView_volkImpl = (PFN_vkCreateImageView)load(context, "vkCreateImageView_volkImpl");
	table->vkCreatePipelineCache_volkImpl = (PFN_vkCreatePipelineCache)load(context, "vkCreatePipelineCache_volkImpl");
	table->vkCreatePipelineLayout_volkImpl = (PFN_vkCreatePipelineLayout)load(context, "vkCreatePipelineLayout_volkImpl");
	table->vkCreateQueryPool_volkImpl = (PFN_vkCreateQueryPool)load(context, "vkCreateQueryPool_volkImpl");
	table->vkCreateRenderPass_volkImpl = (PFN_vkCreateRenderPass)load(context, "vkCreateRenderPass_volkImpl");
	table->vkCreateSampler_volkImpl = (PFN_vkCreateSampler)load(context, "vkCreateSampler_volkImpl");
	table->vkCreateSemaphore_volkImpl = (PFN_vkCreateSemaphore)load(context, "vkCreateSemaphore_volkImpl");
	table->vkCreateShaderModule_volkImpl = (PFN_vkCreateShaderModule)load(context, "vkCreateShaderModule_volkImpl");
	table->vkDestroyBuffer_volkImpl = (PFN_vkDestroyBuffer)load(context, "vkDestroyBuffer_volkImpl");
	table->vkDestroyBufferView_volkImpl = (PFN_vkDestroyBufferView)load(context, "vkDestroyBufferView_volkImpl");
	table->vkDestroyCommandPool_volkImpl = (PFN_vkDestroyCommandPool)load(context, "vkDestroyCommandPool_volkImpl");
	table->vkDestroyDescriptorPool_volkImpl = (PFN_vkDestroyDescriptorPool)load(context, "vkDestroyDescriptorPool_volkImpl");
	table->vkDestroyDescriptorSetLayout_volkImpl = (PFN_vkDestroyDescriptorSetLayout)load(context, "vkDestroyDescriptorSetLayout_volkImpl");
	table->vkDestroyDevice_volkImpl = (PFN_vkDestroyDevice)load(context, "vkDestroyDevice_volkImpl");
	table->vkDestroyEvent_volkImpl = (PFN_vkDestroyEvent)load(context, "vkDestroyEvent_volkImpl");
	table->vkDestroyFence_volkImpl = (PFN_vkDestroyFence)load(context, "vkDestroyFence_volkImpl");
	table->vkDestroyFramebuffer_volkImpl = (PFN_vkDestroyFramebuffer)load(context, "vkDestroyFramebuffer_volkImpl");
	table->vkDestroyImage_volkImpl = (PFN_vkDestroyImage)load(context, "vkDestroyImage_volkImpl");
	table->vkDestroyImageView_volkImpl = (PFN_vkDestroyImageView)load(context, "vkDestroyImageView_volkImpl");
	table->vkDestroyPipeline_volkImpl = (PFN_vkDestroyPipeline)load(context, "vkDestroyPipeline_volkImpl");
	table->vkDestroyPipelineCache_volkImpl = (PFN_vkDestroyPipelineCache)load(context, "vkDestroyPipelineCache_volkImpl");
	table->vkDestroyPipelineLayout_volkImpl = (PFN_vkDestroyPipelineLayout)load(context, "vkDestroyPipelineLayout_volkImpl");
	table->vkDestroyQueryPool_volkImpl = (PFN_vkDestroyQueryPool)load(context, "vkDestroyQueryPool_volkImpl");
	table->vkDestroyRenderPass_volkImpl = (PFN_vkDestroyRenderPass)load(context, "vkDestroyRenderPass_volkImpl");
	table->vkDestroySampler_volkImpl = (PFN_vkDestroySampler)load(context, "vkDestroySampler_volkImpl");
	table->vkDestroySemaphore_volkImpl = (PFN_vkDestroySemaphore)load(context, "vkDestroySemaphore_volkImpl");
	table->vkDestroyShaderModule_volkImpl = (PFN_vkDestroyShaderModule)load(context, "vkDestroyShaderModule_volkImpl");
	table->vkDeviceWaitIdle_volkImpl = (PFN_vkDeviceWaitIdle)load(context, "vkDeviceWaitIdle_volkImpl");
	table->vkEndCommandBuffer_volkImpl = (PFN_vkEndCommandBuffer)load(context, "vkEndCommandBuffer_volkImpl");
	table->vkFlushMappedMemoryRanges_volkImpl = (PFN_vkFlushMappedMemoryRanges)load(context, "vkFlushMappedMemoryRanges_volkImpl");
	table->vkFreeCommandBuffers_volkImpl = (PFN_vkFreeCommandBuffers)load(context, "vkFreeCommandBuffers_volkImpl");
	table->vkFreeDescriptorSets_volkImpl = (PFN_vkFreeDescriptorSets)load(context, "vkFreeDescriptorSets_volkImpl");
	table->vkFreeMemory_volkImpl = (PFN_vkFreeMemory)load(context, "vkFreeMemory_volkImpl");
	table->vkGetBufferMemoryRequirements_volkImpl = (PFN_vkGetBufferMemoryRequirements)load(context, "vkGetBufferMemoryRequirements_volkImpl");
	table->vkGetDeviceMemoryCommitment_volkImpl = (PFN_vkGetDeviceMemoryCommitment)load(context, "vkGetDeviceMemoryCommitment_volkImpl");
	table->vkGetDeviceQueue_volkImpl = (PFN_vkGetDeviceQueue)load(context, "vkGetDeviceQueue_volkImpl");
	table->vkGetEventStatus_volkImpl = (PFN_vkGetEventStatus)load(context, "vkGetEventStatus_volkImpl");
	table->vkGetFenceStatus_volkImpl = (PFN_vkGetFenceStatus)load(context, "vkGetFenceStatus_volkImpl");
	table->vkGetImageMemoryRequirements_volkImpl = (PFN_vkGetImageMemoryRequirements)load(context, "vkGetImageMemoryRequirements_volkImpl");
	table->vkGetImageSparseMemoryRequirements_volkImpl = (PFN_vkGetImageSparseMemoryRequirements)load(context, "vkGetImageSparseMemoryRequirements_volkImpl");
	table->vkGetImageSubresourceLayout_volkImpl = (PFN_vkGetImageSubresourceLayout)load(context, "vkGetImageSubresourceLayout_volkImpl");
	table->vkGetPipelineCacheData_volkImpl = (PFN_vkGetPipelineCacheData)load(context, "vkGetPipelineCacheData_volkImpl");
	table->vkGetQueryPoolResults_volkImpl = (PFN_vkGetQueryPoolResults)load(context, "vkGetQueryPoolResults_volkImpl");
	table->vkGetRenderAreaGranularity_volkImpl = (PFN_vkGetRenderAreaGranularity)load(context, "vkGetRenderAreaGranularity_volkImpl");
	table->vkInvalidateMappedMemoryRanges_volkImpl = (PFN_vkInvalidateMappedMemoryRanges)load(context, "vkInvalidateMappedMemoryRanges_volkImpl");
	table->vkMapMemory_volkImpl = (PFN_vkMapMemory)load(context, "vkMapMemory_volkImpl");
	table->vkMergePipelineCaches_volkImpl = (PFN_vkMergePipelineCaches)load(context, "vkMergePipelineCaches_volkImpl");
	table->vkQueueBindSparse_volkImpl = (PFN_vkQueueBindSparse)load(context, "vkQueueBindSparse_volkImpl");
	table->vkQueueSubmit_volkImpl = (PFN_vkQueueSubmit)load(context, "vkQueueSubmit_volkImpl");
	table->vkQueueWaitIdle_volkImpl = (PFN_vkQueueWaitIdle)load(context, "vkQueueWaitIdle_volkImpl");
	table->vkResetCommandBuffer_volkImpl = (PFN_vkResetCommandBuffer)load(context, "vkResetCommandBuffer_volkImpl");
	table->vkResetCommandPool_volkImpl = (PFN_vkResetCommandPool)load(context, "vkResetCommandPool_volkImpl");
	table->vkResetDescriptorPool_volkImpl = (PFN_vkResetDescriptorPool)load(context, "vkResetDescriptorPool_volkImpl");
	table->vkResetEvent_volkImpl = (PFN_vkResetEvent)load(context, "vkResetEvent_volkImpl");
	table->vkResetFences_volkImpl = (PFN_vkResetFences)load(context, "vkResetFences_volkImpl");
	table->vkSetEvent_volkImpl = (PFN_vkSetEvent)load(context, "vkSetEvent_volkImpl");
	table->vkUnmapMemory_volkImpl = (PFN_vkUnmapMemory)load(context, "vkUnmapMemory_volkImpl");
	table->vkUpdateDescriptorSets_volkImpl = (PFN_vkUpdateDescriptorSets)load(context, "vkUpdateDescriptorSets_volkImpl");
	table->vkWaitForFences_volkImpl = (PFN_vkWaitForFences)load(context, "vkWaitForFences_volkImpl");
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
	table->vkBindBufferMemory2_volkImpl = (PFN_vkBindBufferMemory2)load(context, "vkBindBufferMemory2_volkImpl");
	table->vkBindImageMemory2_volkImpl = (PFN_vkBindImageMemory2)load(context, "vkBindImageMemory2_volkImpl");
	table->vkCmdDispatchBase_volkImpl = (PFN_vkCmdDispatchBase)load(context, "vkCmdDispatchBase_volkImpl");
	table->vkCmdSetDeviceMask_volkImpl = (PFN_vkCmdSetDeviceMask)load(context, "vkCmdSetDeviceMask_volkImpl");
	table->vkCreateDescriptorUpdateTemplate_volkImpl = (PFN_vkCreateDescriptorUpdateTemplate)load(context, "vkCreateDescriptorUpdateTemplate_volkImpl");
	table->vkCreateSamplerYcbcrConversion_volkImpl = (PFN_vkCreateSamplerYcbcrConversion)load(context, "vkCreateSamplerYcbcrConversion_volkImpl");
	table->vkDestroyDescriptorUpdateTemplate_volkImpl = (PFN_vkDestroyDescriptorUpdateTemplate)load(context, "vkDestroyDescriptorUpdateTemplate_volkImpl");
	table->vkDestroySamplerYcbcrConversion_volkImpl = (PFN_vkDestroySamplerYcbcrConversion)load(context, "vkDestroySamplerYcbcrConversion_volkImpl");
	table->vkGetBufferMemoryRequirements2_volkImpl = (PFN_vkGetBufferMemoryRequirements2)load(context, "vkGetBufferMemoryRequirements2_volkImpl");
	table->vkGetDescriptorSetLayoutSupport_volkImpl = (PFN_vkGetDescriptorSetLayoutSupport)load(context, "vkGetDescriptorSetLayoutSupport_volkImpl");
	table->vkGetDeviceGroupPeerMemoryFeatures_volkImpl = (PFN_vkGetDeviceGroupPeerMemoryFeatures)load(context, "vkGetDeviceGroupPeerMemoryFeatures_volkImpl");
	table->vkGetDeviceQueue2_volkImpl = (PFN_vkGetDeviceQueue2)load(context, "vkGetDeviceQueue2_volkImpl");
	table->vkGetImageMemoryRequirements2_volkImpl = (PFN_vkGetImageMemoryRequirements2)load(context, "vkGetImageMemoryRequirements2_volkImpl");
	table->vkGetImageSparseMemoryRequirements2_volkImpl = (PFN_vkGetImageSparseMemoryRequirements2)load(context, "vkGetImageSparseMemoryRequirements2_volkImpl");
	table->vkTrimCommandPool_volkImpl = (PFN_vkTrimCommandPool)load(context, "vkTrimCommandPool_volkImpl");
	table->vkUpdateDescriptorSetWithTemplate_volkImpl = (PFN_vkUpdateDescriptorSetWithTemplate)load(context, "vkUpdateDescriptorSetWithTemplate_volkImpl");
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_2)
	table->vkCmdBeginRenderPass2_volkImpl = (PFN_vkCmdBeginRenderPass2)load(context, "vkCmdBeginRenderPass2_volkImpl");
	table->vkCmdDrawIndexedIndirectCount_volkImpl = (PFN_vkCmdDrawIndexedIndirectCount)load(context, "vkCmdDrawIndexedIndirectCount_volkImpl");
	table->vkCmdDrawIndirectCount_volkImpl = (PFN_vkCmdDrawIndirectCount)load(context, "vkCmdDrawIndirectCount_volkImpl");
	table->vkCmdEndRenderPass2_volkImpl = (PFN_vkCmdEndRenderPass2)load(context, "vkCmdEndRenderPass2_volkImpl");
	table->vkCmdNextSubpass2_volkImpl = (PFN_vkCmdNextSubpass2)load(context, "vkCmdNextSubpass2_volkImpl");
	table->vkCreateRenderPass2_volkImpl = (PFN_vkCreateRenderPass2)load(context, "vkCreateRenderPass2_volkImpl");
	table->vkGetBufferDeviceAddress_volkImpl = (PFN_vkGetBufferDeviceAddress)load(context, "vkGetBufferDeviceAddress_volkImpl");
	table->vkGetBufferOpaqueCaptureAddress_volkImpl = (PFN_vkGetBufferOpaqueCaptureAddress)load(context, "vkGetBufferOpaqueCaptureAddress_volkImpl");
	table->vkGetDeviceMemoryOpaqueCaptureAddress_volkImpl = (PFN_vkGetDeviceMemoryOpaqueCaptureAddress)load(context, "vkGetDeviceMemoryOpaqueCaptureAddress_volkImpl");
	table->vkGetSemaphoreCounterValue_volkImpl = (PFN_vkGetSemaphoreCounterValue)load(context, "vkGetSemaphoreCounterValue_volkImpl");
	table->vkResetQueryPool_volkImpl = (PFN_vkResetQueryPool)load(context, "vkResetQueryPool_volkImpl");
	table->vkSignalSemaphore_volkImpl = (PFN_vkSignalSemaphore)load(context, "vkSignalSemaphore_volkImpl");
	table->vkWaitSemaphores_volkImpl = (PFN_vkWaitSemaphores)load(context, "vkWaitSemaphores_volkImpl");
#endif /* defined(VK_VERSION_1_2) */
#if defined(VK_VERSION_1_3)
	table->vkCmdBeginRendering_volkImpl = (PFN_vkCmdBeginRendering)load(context, "vkCmdBeginRendering_volkImpl");
	table->vkCmdBindVertexBuffers2_volkImpl = (PFN_vkCmdBindVertexBuffers2)load(context, "vkCmdBindVertexBuffers2_volkImpl");
	table->vkCmdBlitImage2_volkImpl = (PFN_vkCmdBlitImage2)load(context, "vkCmdBlitImage2_volkImpl");
	table->vkCmdCopyBuffer2_volkImpl = (PFN_vkCmdCopyBuffer2)load(context, "vkCmdCopyBuffer2_volkImpl");
	table->vkCmdCopyBufferToImage2_volkImpl = (PFN_vkCmdCopyBufferToImage2)load(context, "vkCmdCopyBufferToImage2_volkImpl");
	table->vkCmdCopyImage2_volkImpl = (PFN_vkCmdCopyImage2)load(context, "vkCmdCopyImage2_volkImpl");
	table->vkCmdCopyImageToBuffer2_volkImpl = (PFN_vkCmdCopyImageToBuffer2)load(context, "vkCmdCopyImageToBuffer2_volkImpl");
	table->vkCmdEndRendering_volkImpl = (PFN_vkCmdEndRendering)load(context, "vkCmdEndRendering_volkImpl");
	table->vkCmdPipelineBarrier2_volkImpl = (PFN_vkCmdPipelineBarrier2)load(context, "vkCmdPipelineBarrier2_volkImpl");
	table->vkCmdResetEvent2_volkImpl = (PFN_vkCmdResetEvent2)load(context, "vkCmdResetEvent2_volkImpl");
	table->vkCmdResolveImage2_volkImpl = (PFN_vkCmdResolveImage2)load(context, "vkCmdResolveImage2_volkImpl");
	table->vkCmdSetCullMode_volkImpl = (PFN_vkCmdSetCullMode)load(context, "vkCmdSetCullMode_volkImpl");
	table->vkCmdSetDepthBiasEnable_volkImpl = (PFN_vkCmdSetDepthBiasEnable)load(context, "vkCmdSetDepthBiasEnable_volkImpl");
	table->vkCmdSetDepthBoundsTestEnable_volkImpl = (PFN_vkCmdSetDepthBoundsTestEnable)load(context, "vkCmdSetDepthBoundsTestEnable_volkImpl");
	table->vkCmdSetDepthCompareOp_volkImpl = (PFN_vkCmdSetDepthCompareOp)load(context, "vkCmdSetDepthCompareOp_volkImpl");
	table->vkCmdSetDepthTestEnable_volkImpl = (PFN_vkCmdSetDepthTestEnable)load(context, "vkCmdSetDepthTestEnable_volkImpl");
	table->vkCmdSetDepthWriteEnable_volkImpl = (PFN_vkCmdSetDepthWriteEnable)load(context, "vkCmdSetDepthWriteEnable_volkImpl");
	table->vkCmdSetEvent2_volkImpl = (PFN_vkCmdSetEvent2)load(context, "vkCmdSetEvent2_volkImpl");
	table->vkCmdSetFrontFace_volkImpl = (PFN_vkCmdSetFrontFace)load(context, "vkCmdSetFrontFace_volkImpl");
	table->vkCmdSetPrimitiveRestartEnable_volkImpl = (PFN_vkCmdSetPrimitiveRestartEnable)load(context, "vkCmdSetPrimitiveRestartEnable_volkImpl");
	table->vkCmdSetPrimitiveTopology_volkImpl = (PFN_vkCmdSetPrimitiveTopology)load(context, "vkCmdSetPrimitiveTopology_volkImpl");
	table->vkCmdSetRasterizerDiscardEnable_volkImpl = (PFN_vkCmdSetRasterizerDiscardEnable)load(context, "vkCmdSetRasterizerDiscardEnable_volkImpl");
	table->vkCmdSetScissorWithCount_volkImpl = (PFN_vkCmdSetScissorWithCount)load(context, "vkCmdSetScissorWithCount_volkImpl");
	table->vkCmdSetStencilOp_volkImpl = (PFN_vkCmdSetStencilOp)load(context, "vkCmdSetStencilOp_volkImpl");
	table->vkCmdSetStencilTestEnable_volkImpl = (PFN_vkCmdSetStencilTestEnable)load(context, "vkCmdSetStencilTestEnable_volkImpl");
	table->vkCmdSetViewportWithCount_volkImpl = (PFN_vkCmdSetViewportWithCount)load(context, "vkCmdSetViewportWithCount_volkImpl");
	table->vkCmdWaitEvents2_volkImpl = (PFN_vkCmdWaitEvents2)load(context, "vkCmdWaitEvents2_volkImpl");
	table->vkCmdWriteTimestamp2_volkImpl = (PFN_vkCmdWriteTimestamp2)load(context, "vkCmdWriteTimestamp2_volkImpl");
	table->vkCreatePrivateDataSlot_volkImpl = (PFN_vkCreatePrivateDataSlot)load(context, "vkCreatePrivateDataSlot_volkImpl");
	table->vkDestroyPrivateDataSlot_volkImpl = (PFN_vkDestroyPrivateDataSlot)load(context, "vkDestroyPrivateDataSlot_volkImpl");
	table->vkGetDeviceBufferMemoryRequirements_volkImpl = (PFN_vkGetDeviceBufferMemoryRequirements)load(context, "vkGetDeviceBufferMemoryRequirements_volkImpl");
	table->vkGetDeviceImageMemoryRequirements_volkImpl = (PFN_vkGetDeviceImageMemoryRequirements)load(context, "vkGetDeviceImageMemoryRequirements_volkImpl");
	table->vkGetDeviceImageSparseMemoryRequirements_volkImpl = (PFN_vkGetDeviceImageSparseMemoryRequirements)load(context, "vkGetDeviceImageSparseMemoryRequirements_volkImpl");
	table->vkGetPrivateData_volkImpl = (PFN_vkGetPrivateData)load(context, "vkGetPrivateData_volkImpl");
	table->vkQueueSubmit2_volkImpl = (PFN_vkQueueSubmit2)load(context, "vkQueueSubmit2_volkImpl");
	table->vkSetPrivateData_volkImpl = (PFN_vkSetPrivateData)load(context, "vkSetPrivateData_volkImpl");
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_AMDX_shader_enqueue)
	table->vkCmdDispatchGraphAMDX_volkImpl = (PFN_vkCmdDispatchGraphAMDX)load(context, "vkCmdDispatchGraphAMDX_volkImpl");
	table->vkCmdDispatchGraphIndirectAMDX_volkImpl = (PFN_vkCmdDispatchGraphIndirectAMDX)load(context, "vkCmdDispatchGraphIndirectAMDX_volkImpl");
	table->vkCmdDispatchGraphIndirectCountAMDX_volkImpl = (PFN_vkCmdDispatchGraphIndirectCountAMDX)load(context, "vkCmdDispatchGraphIndirectCountAMDX_volkImpl");
	table->vkCmdInitializeGraphScratchMemoryAMDX_volkImpl = (PFN_vkCmdInitializeGraphScratchMemoryAMDX)load(context, "vkCmdInitializeGraphScratchMemoryAMDX_volkImpl");
	table->vkCreateExecutionGraphPipelinesAMDX_volkImpl = (PFN_vkCreateExecutionGraphPipelinesAMDX)load(context, "vkCreateExecutionGraphPipelinesAMDX_volkImpl");
	table->vkGetExecutionGraphPipelineNodeIndexAMDX_volkImpl = (PFN_vkGetExecutionGraphPipelineNodeIndexAMDX)load(context, "vkGetExecutionGraphPipelineNodeIndexAMDX_volkImpl");
	table->vkGetExecutionGraphPipelineScratchSizeAMDX_volkImpl = (PFN_vkGetExecutionGraphPipelineScratchSizeAMDX)load(context, "vkGetExecutionGraphPipelineScratchSizeAMDX_volkImpl");
#endif /* defined(VK_AMDX_shader_enqueue) */
#if defined(VK_AMD_buffer_marker)
	table->vkCmdWriteBufferMarkerAMD_volkImpl = (PFN_vkCmdWriteBufferMarkerAMD)load(context, "vkCmdWriteBufferMarkerAMD_volkImpl");
#endif /* defined(VK_AMD_buffer_marker) */
#if defined(VK_AMD_display_native_hdr)
	table->vkSetLocalDimmingAMD_volkImpl = (PFN_vkSetLocalDimmingAMD)load(context, "vkSetLocalDimmingAMD_volkImpl");
#endif /* defined(VK_AMD_display_native_hdr) */
#if defined(VK_AMD_draw_indirect_count)
	table->vkCmdDrawIndexedIndirectCountAMD_volkImpl = (PFN_vkCmdDrawIndexedIndirectCountAMD)load(context, "vkCmdDrawIndexedIndirectCountAMD_volkImpl");
	table->vkCmdDrawIndirectCountAMD_volkImpl = (PFN_vkCmdDrawIndirectCountAMD)load(context, "vkCmdDrawIndirectCountAMD_volkImpl");
#endif /* defined(VK_AMD_draw_indirect_count) */
#if defined(VK_AMD_shader_info)
	table->vkGetShaderInfoAMD_volkImpl = (PFN_vkGetShaderInfoAMD)load(context, "vkGetShaderInfoAMD_volkImpl");
#endif /* defined(VK_AMD_shader_info) */
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
	table->vkGetAndroidHardwareBufferPropertiesANDROID_volkImpl = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID)load(context, "vkGetAndroidHardwareBufferPropertiesANDROID_volkImpl");
	table->vkGetMemoryAndroidHardwareBufferANDROID_volkImpl = (PFN_vkGetMemoryAndroidHardwareBufferANDROID)load(context, "vkGetMemoryAndroidHardwareBufferANDROID_volkImpl");
#endif /* defined(VK_ANDROID_external_memory_android_hardware_buffer) */
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state)
	table->vkCmdSetAttachmentFeedbackLoopEnableEXT_volkImpl = (PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT)load(context, "vkCmdSetAttachmentFeedbackLoopEnableEXT_volkImpl");
#endif /* defined(VK_EXT_attachment_feedback_loop_dynamic_state) */
#if defined(VK_EXT_buffer_device_address)
	table->vkGetBufferDeviceAddressEXT_volkImpl = (PFN_vkGetBufferDeviceAddressEXT)load(context, "vkGetBufferDeviceAddressEXT_volkImpl");
#endif /* defined(VK_EXT_buffer_device_address) */
#if defined(VK_EXT_calibrated_timestamps)
	table->vkGetCalibratedTimestampsEXT_volkImpl = (PFN_vkGetCalibratedTimestampsEXT)load(context, "vkGetCalibratedTimestampsEXT_volkImpl");
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_color_write_enable)
	table->vkCmdSetColorWriteEnableEXT_volkImpl = (PFN_vkCmdSetColorWriteEnableEXT)load(context, "vkCmdSetColorWriteEnableEXT_volkImpl");
#endif /* defined(VK_EXT_color_write_enable) */
#if defined(VK_EXT_conditional_rendering)
	table->vkCmdBeginConditionalRenderingEXT_volkImpl = (PFN_vkCmdBeginConditionalRenderingEXT)load(context, "vkCmdBeginConditionalRenderingEXT_volkImpl");
	table->vkCmdEndConditionalRenderingEXT_volkImpl = (PFN_vkCmdEndConditionalRenderingEXT)load(context, "vkCmdEndConditionalRenderingEXT_volkImpl");
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_debug_marker)
	table->vkCmdDebugMarkerBeginEXT_volkImpl = (PFN_vkCmdDebugMarkerBeginEXT)load(context, "vkCmdDebugMarkerBeginEXT_volkImpl");
	table->vkCmdDebugMarkerEndEXT_volkImpl = (PFN_vkCmdDebugMarkerEndEXT)load(context, "vkCmdDebugMarkerEndEXT_volkImpl");
	table->vkCmdDebugMarkerInsertEXT_volkImpl = (PFN_vkCmdDebugMarkerInsertEXT)load(context, "vkCmdDebugMarkerInsertEXT_volkImpl");
	table->vkDebugMarkerSetObjectNameEXT_volkImpl = (PFN_vkDebugMarkerSetObjectNameEXT)load(context, "vkDebugMarkerSetObjectNameEXT_volkImpl");
	table->vkDebugMarkerSetObjectTagEXT_volkImpl = (PFN_vkDebugMarkerSetObjectTagEXT)load(context, "vkDebugMarkerSetObjectTagEXT_volkImpl");
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_depth_bias_control)
	table->vkCmdSetDepthBias2EXT_volkImpl = (PFN_vkCmdSetDepthBias2EXT)load(context, "vkCmdSetDepthBias2EXT_volkImpl");
#endif /* defined(VK_EXT_depth_bias_control) */
#if defined(VK_EXT_descriptor_buffer)
	table->vkCmdBindDescriptorBufferEmbeddedSamplersEXT_volkImpl = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT)load(context, "vkCmdBindDescriptorBufferEmbeddedSamplersEXT_volkImpl");
	table->vkCmdBindDescriptorBuffersEXT_volkImpl = (PFN_vkCmdBindDescriptorBuffersEXT)load(context, "vkCmdBindDescriptorBuffersEXT_volkImpl");
	table->vkCmdSetDescriptorBufferOffsetsEXT_volkImpl = (PFN_vkCmdSetDescriptorBufferOffsetsEXT)load(context, "vkCmdSetDescriptorBufferOffsetsEXT_volkImpl");
	table->vkGetBufferOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT)load(context, "vkGetBufferOpaqueCaptureDescriptorDataEXT_volkImpl");
	table->vkGetDescriptorEXT_volkImpl = (PFN_vkGetDescriptorEXT)load(context, "vkGetDescriptorEXT_volkImpl");
	table->vkGetDescriptorSetLayoutBindingOffsetEXT_volkImpl = (PFN_vkGetDescriptorSetLayoutBindingOffsetEXT)load(context, "vkGetDescriptorSetLayoutBindingOffsetEXT_volkImpl");
	table->vkGetDescriptorSetLayoutSizeEXT_volkImpl = (PFN_vkGetDescriptorSetLayoutSizeEXT)load(context, "vkGetDescriptorSetLayoutSizeEXT_volkImpl");
	table->vkGetImageOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetImageOpaqueCaptureDescriptorDataEXT)load(context, "vkGetImageOpaqueCaptureDescriptorDataEXT_volkImpl");
	table->vkGetImageViewOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT)load(context, "vkGetImageViewOpaqueCaptureDescriptorDataEXT_volkImpl");
	table->vkGetSamplerOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT)load(context, "vkGetSamplerOpaqueCaptureDescriptorDataEXT_volkImpl");
#endif /* defined(VK_EXT_descriptor_buffer) */
#if defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing))
	table->vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT_volkImpl = (PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT)load(context, "vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT_volkImpl");
#endif /* defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)) */
#if defined(VK_EXT_device_fault)
	table->vkGetDeviceFaultInfoEXT_volkImpl = (PFN_vkGetDeviceFaultInfoEXT)load(context, "vkGetDeviceFaultInfoEXT_volkImpl");
#endif /* defined(VK_EXT_device_fault) */
#if defined(VK_EXT_discard_rectangles)
	table->vkCmdSetDiscardRectangleEXT_volkImpl = (PFN_vkCmdSetDiscardRectangleEXT)load(context, "vkCmdSetDiscardRectangleEXT_volkImpl");
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2
	table->vkCmdSetDiscardRectangleEnableEXT_volkImpl = (PFN_vkCmdSetDiscardRectangleEnableEXT)load(context, "vkCmdSetDiscardRectangleEnableEXT_volkImpl");
	table->vkCmdSetDiscardRectangleModeEXT_volkImpl = (PFN_vkCmdSetDiscardRectangleModeEXT)load(context, "vkCmdSetDiscardRectangleModeEXT_volkImpl");
#endif /* defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2 */
#if defined(VK_EXT_display_control)
	table->vkDisplayPowerControlEXT_volkImpl = (PFN_vkDisplayPowerControlEXT)load(context, "vkDisplayPowerControlEXT_volkImpl");
	table->vkGetSwapchainCounterEXT_volkImpl = (PFN_vkGetSwapchainCounterEXT)load(context, "vkGetSwapchainCounterEXT_volkImpl");
	table->vkRegisterDeviceEventEXT_volkImpl = (PFN_vkRegisterDeviceEventEXT)load(context, "vkRegisterDeviceEventEXT_volkImpl");
	table->vkRegisterDisplayEventEXT_volkImpl = (PFN_vkRegisterDisplayEventEXT)load(context, "vkRegisterDisplayEventEXT_volkImpl");
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_external_memory_host)
	table->vkGetMemoryHostPointerPropertiesEXT_volkImpl = (PFN_vkGetMemoryHostPointerPropertiesEXT)load(context, "vkGetMemoryHostPointerPropertiesEXT_volkImpl");
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_full_screen_exclusive)
	table->vkAcquireFullScreenExclusiveModeEXT_volkImpl = (PFN_vkAcquireFullScreenExclusiveModeEXT)load(context, "vkAcquireFullScreenExclusiveModeEXT_volkImpl");
	table->vkReleaseFullScreenExclusiveModeEXT_volkImpl = (PFN_vkReleaseFullScreenExclusiveModeEXT)load(context, "vkReleaseFullScreenExclusiveModeEXT_volkImpl");
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_hdr_metadata)
	table->vkSetHdrMetadataEXT_volkImpl = (PFN_vkSetHdrMetadataEXT)load(context, "vkSetHdrMetadataEXT_volkImpl");
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_host_image_copy)
	table->vkCopyImageToImageEXT_volkImpl = (PFN_vkCopyImageToImageEXT)load(context, "vkCopyImageToImageEXT_volkImpl");
	table->vkCopyImageToMemoryEXT_volkImpl = (PFN_vkCopyImageToMemoryEXT)load(context, "vkCopyImageToMemoryEXT_volkImpl");
	table->vkCopyMemoryToImageEXT_volkImpl = (PFN_vkCopyMemoryToImageEXT)load(context, "vkCopyMemoryToImageEXT_volkImpl");
	table->vkTransitionImageLayoutEXT_volkImpl = (PFN_vkTransitionImageLayoutEXT)load(context, "vkTransitionImageLayoutEXT_volkImpl");
#endif /* defined(VK_EXT_host_image_copy) */
#if defined(VK_EXT_host_query_reset)
	table->vkResetQueryPoolEXT_volkImpl = (PFN_vkResetQueryPoolEXT)load(context, "vkResetQueryPoolEXT_volkImpl");
#endif /* defined(VK_EXT_host_query_reset) */
#if defined(VK_EXT_image_drm_format_modifier)
	table->vkGetImageDrmFormatModifierPropertiesEXT_volkImpl = (PFN_vkGetImageDrmFormatModifierPropertiesEXT)load(context, "vkGetImageDrmFormatModifierPropertiesEXT_volkImpl");
#endif /* defined(VK_EXT_image_drm_format_modifier) */
#if defined(VK_EXT_line_rasterization)
	table->vkCmdSetLineStippleEXT_volkImpl = (PFN_vkCmdSetLineStippleEXT)load(context, "vkCmdSetLineStippleEXT_volkImpl");
#endif /* defined(VK_EXT_line_rasterization) */
#if defined(VK_EXT_mesh_shader)
	table->vkCmdDrawMeshTasksEXT_volkImpl = (PFN_vkCmdDrawMeshTasksEXT)load(context, "vkCmdDrawMeshTasksEXT_volkImpl");
	table->vkCmdDrawMeshTasksIndirectCountEXT_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectCountEXT)load(context, "vkCmdDrawMeshTasksIndirectCountEXT_volkImpl");
	table->vkCmdDrawMeshTasksIndirectEXT_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectEXT)load(context, "vkCmdDrawMeshTasksIndirectEXT_volkImpl");
#endif /* defined(VK_EXT_mesh_shader) */
#if defined(VK_EXT_metal_objects)
	table->vkExportMetalObjectsEXT_volkImpl = (PFN_vkExportMetalObjectsEXT)load(context, "vkExportMetalObjectsEXT_volkImpl");
#endif /* defined(VK_EXT_metal_objects) */
#if defined(VK_EXT_multi_draw)
	table->vkCmdDrawMultiEXT_volkImpl = (PFN_vkCmdDrawMultiEXT)load(context, "vkCmdDrawMultiEXT_volkImpl");
	table->vkCmdDrawMultiIndexedEXT_volkImpl = (PFN_vkCmdDrawMultiIndexedEXT)load(context, "vkCmdDrawMultiIndexedEXT_volkImpl");
#endif /* defined(VK_EXT_multi_draw) */
#if defined(VK_EXT_opacity_micromap)
	table->vkBuildMicromapsEXT_volkImpl = (PFN_vkBuildMicromapsEXT)load(context, "vkBuildMicromapsEXT_volkImpl");
	table->vkCmdBuildMicromapsEXT_volkImpl = (PFN_vkCmdBuildMicromapsEXT)load(context, "vkCmdBuildMicromapsEXT_volkImpl");
	table->vkCmdCopyMemoryToMicromapEXT_volkImpl = (PFN_vkCmdCopyMemoryToMicromapEXT)load(context, "vkCmdCopyMemoryToMicromapEXT_volkImpl");
	table->vkCmdCopyMicromapEXT_volkImpl = (PFN_vkCmdCopyMicromapEXT)load(context, "vkCmdCopyMicromapEXT_volkImpl");
	table->vkCmdCopyMicromapToMemoryEXT_volkImpl = (PFN_vkCmdCopyMicromapToMemoryEXT)load(context, "vkCmdCopyMicromapToMemoryEXT_volkImpl");
	table->vkCmdWriteMicromapsPropertiesEXT_volkImpl = (PFN_vkCmdWriteMicromapsPropertiesEXT)load(context, "vkCmdWriteMicromapsPropertiesEXT_volkImpl");
	table->vkCopyMemoryToMicromapEXT_volkImpl = (PFN_vkCopyMemoryToMicromapEXT)load(context, "vkCopyMemoryToMicromapEXT_volkImpl");
	table->vkCopyMicromapEXT_volkImpl = (PFN_vkCopyMicromapEXT)load(context, "vkCopyMicromapEXT_volkImpl");
	table->vkCopyMicromapToMemoryEXT_volkImpl = (PFN_vkCopyMicromapToMemoryEXT)load(context, "vkCopyMicromapToMemoryEXT_volkImpl");
	table->vkCreateMicromapEXT_volkImpl = (PFN_vkCreateMicromapEXT)load(context, "vkCreateMicromapEXT_volkImpl");
	table->vkDestroyMicromapEXT_volkImpl = (PFN_vkDestroyMicromapEXT)load(context, "vkDestroyMicromapEXT_volkImpl");
	table->vkGetDeviceMicromapCompatibilityEXT_volkImpl = (PFN_vkGetDeviceMicromapCompatibilityEXT)load(context, "vkGetDeviceMicromapCompatibilityEXT_volkImpl");
	table->vkGetMicromapBuildSizesEXT_volkImpl = (PFN_vkGetMicromapBuildSizesEXT)load(context, "vkGetMicromapBuildSizesEXT_volkImpl");
	table->vkWriteMicromapsPropertiesEXT_volkImpl = (PFN_vkWriteMicromapsPropertiesEXT)load(context, "vkWriteMicromapsPropertiesEXT_volkImpl");
#endif /* defined(VK_EXT_opacity_micromap) */
#if defined(VK_EXT_pageable_device_local_memory)
	table->vkSetDeviceMemoryPriorityEXT_volkImpl = (PFN_vkSetDeviceMemoryPriorityEXT)load(context, "vkSetDeviceMemoryPriorityEXT_volkImpl");
#endif /* defined(VK_EXT_pageable_device_local_memory) */
#if defined(VK_EXT_pipeline_properties)
	table->vkGetPipelinePropertiesEXT_volkImpl = (PFN_vkGetPipelinePropertiesEXT)load(context, "vkGetPipelinePropertiesEXT_volkImpl");
#endif /* defined(VK_EXT_pipeline_properties) */
#if defined(VK_EXT_private_data)
	table->vkCreatePrivateDataSlotEXT_volkImpl = (PFN_vkCreatePrivateDataSlotEXT)load(context, "vkCreatePrivateDataSlotEXT_volkImpl");
	table->vkDestroyPrivateDataSlotEXT_volkImpl = (PFN_vkDestroyPrivateDataSlotEXT)load(context, "vkDestroyPrivateDataSlotEXT_volkImpl");
	table->vkGetPrivateDataEXT_volkImpl = (PFN_vkGetPrivateDataEXT)load(context, "vkGetPrivateDataEXT_volkImpl");
	table->vkSetPrivateDataEXT_volkImpl = (PFN_vkSetPrivateDataEXT)load(context, "vkSetPrivateDataEXT_volkImpl");
#endif /* defined(VK_EXT_private_data) */
#if defined(VK_EXT_sample_locations)
	table->vkCmdSetSampleLocationsEXT_volkImpl = (PFN_vkCmdSetSampleLocationsEXT)load(context, "vkCmdSetSampleLocationsEXT_volkImpl");
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_shader_module_identifier)
	table->vkGetShaderModuleCreateInfoIdentifierEXT_volkImpl = (PFN_vkGetShaderModuleCreateInfoIdentifierEXT)load(context, "vkGetShaderModuleCreateInfoIdentifierEXT_volkImpl");
	table->vkGetShaderModuleIdentifierEXT_volkImpl = (PFN_vkGetShaderModuleIdentifierEXT)load(context, "vkGetShaderModuleIdentifierEXT_volkImpl");
#endif /* defined(VK_EXT_shader_module_identifier) */
#if defined(VK_EXT_shader_object)
	table->vkCmdBindShadersEXT_volkImpl = (PFN_vkCmdBindShadersEXT)load(context, "vkCmdBindShadersEXT_volkImpl");
	table->vkCreateShadersEXT_volkImpl = (PFN_vkCreateShadersEXT)load(context, "vkCreateShadersEXT_volkImpl");
	table->vkDestroyShaderEXT_volkImpl = (PFN_vkDestroyShaderEXT)load(context, "vkDestroyShaderEXT_volkImpl");
	table->vkGetShaderBinaryDataEXT_volkImpl = (PFN_vkGetShaderBinaryDataEXT)load(context, "vkGetShaderBinaryDataEXT_volkImpl");
#endif /* defined(VK_EXT_shader_object) */
#if defined(VK_EXT_swapchain_maintenance1)
	table->vkReleaseSwapchainImagesEXT_volkImpl = (PFN_vkReleaseSwapchainImagesEXT)load(context, "vkReleaseSwapchainImagesEXT_volkImpl");
#endif /* defined(VK_EXT_swapchain_maintenance1) */
#if defined(VK_EXT_transform_feedback)
	table->vkCmdBeginQueryIndexedEXT_volkImpl = (PFN_vkCmdBeginQueryIndexedEXT)load(context, "vkCmdBeginQueryIndexedEXT_volkImpl");
	table->vkCmdBeginTransformFeedbackEXT_volkImpl = (PFN_vkCmdBeginTransformFeedbackEXT)load(context, "vkCmdBeginTransformFeedbackEXT_volkImpl");
	table->vkCmdBindTransformFeedbackBuffersEXT_volkImpl = (PFN_vkCmdBindTransformFeedbackBuffersEXT)load(context, "vkCmdBindTransformFeedbackBuffersEXT_volkImpl");
	table->vkCmdDrawIndirectByteCountEXT_volkImpl = (PFN_vkCmdDrawIndirectByteCountEXT)load(context, "vkCmdDrawIndirectByteCountEXT_volkImpl");
	table->vkCmdEndQueryIndexedEXT_volkImpl = (PFN_vkCmdEndQueryIndexedEXT)load(context, "vkCmdEndQueryIndexedEXT_volkImpl");
	table->vkCmdEndTransformFeedbackEXT_volkImpl = (PFN_vkCmdEndTransformFeedbackEXT)load(context, "vkCmdEndTransformFeedbackEXT_volkImpl");
#endif /* defined(VK_EXT_transform_feedback) */
#if defined(VK_EXT_validation_cache)
	table->vkCreateValidationCacheEXT_volkImpl = (PFN_vkCreateValidationCacheEXT)load(context, "vkCreateValidationCacheEXT_volkImpl");
	table->vkDestroyValidationCacheEXT_volkImpl = (PFN_vkDestroyValidationCacheEXT)load(context, "vkDestroyValidationCacheEXT_volkImpl");
	table->vkGetValidationCacheDataEXT_volkImpl = (PFN_vkGetValidationCacheDataEXT)load(context, "vkGetValidationCacheDataEXT_volkImpl");
	table->vkMergeValidationCachesEXT_volkImpl = (PFN_vkMergeValidationCachesEXT)load(context, "vkMergeValidationCachesEXT_volkImpl");
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_FUCHSIA_buffer_collection)
	table->vkCreateBufferCollectionFUCHSIA_volkImpl = (PFN_vkCreateBufferCollectionFUCHSIA)load(context, "vkCreateBufferCollectionFUCHSIA_volkImpl");
	table->vkDestroyBufferCollectionFUCHSIA_volkImpl = (PFN_vkDestroyBufferCollectionFUCHSIA)load(context, "vkDestroyBufferCollectionFUCHSIA_volkImpl");
	table->vkGetBufferCollectionPropertiesFUCHSIA_volkImpl = (PFN_vkGetBufferCollectionPropertiesFUCHSIA)load(context, "vkGetBufferCollectionPropertiesFUCHSIA_volkImpl");
	table->vkSetBufferCollectionBufferConstraintsFUCHSIA_volkImpl = (PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA)load(context, "vkSetBufferCollectionBufferConstraintsFUCHSIA_volkImpl");
	table->vkSetBufferCollectionImageConstraintsFUCHSIA_volkImpl = (PFN_vkSetBufferCollectionImageConstraintsFUCHSIA)load(context, "vkSetBufferCollectionImageConstraintsFUCHSIA_volkImpl");
#endif /* defined(VK_FUCHSIA_buffer_collection) */
#if defined(VK_FUCHSIA_external_memory)
	table->vkGetMemoryZirconHandleFUCHSIA_volkImpl = (PFN_vkGetMemoryZirconHandleFUCHSIA)load(context, "vkGetMemoryZirconHandleFUCHSIA_volkImpl");
	table->vkGetMemoryZirconHandlePropertiesFUCHSIA_volkImpl = (PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA)load(context, "vkGetMemoryZirconHandlePropertiesFUCHSIA_volkImpl");
#endif /* defined(VK_FUCHSIA_external_memory) */
#if defined(VK_FUCHSIA_external_semaphore)
	table->vkGetSemaphoreZirconHandleFUCHSIA_volkImpl = (PFN_vkGetSemaphoreZirconHandleFUCHSIA)load(context, "vkGetSemaphoreZirconHandleFUCHSIA_volkImpl");
	table->vkImportSemaphoreZirconHandleFUCHSIA_volkImpl = (PFN_vkImportSemaphoreZirconHandleFUCHSIA)load(context, "vkImportSemaphoreZirconHandleFUCHSIA_volkImpl");
#endif /* defined(VK_FUCHSIA_external_semaphore) */
#if defined(VK_GOOGLE_display_timing)
	table->vkGetPastPresentationTimingGOOGLE_volkImpl = (PFN_vkGetPastPresentationTimingGOOGLE)load(context, "vkGetPastPresentationTimingGOOGLE_volkImpl");
	table->vkGetRefreshCycleDurationGOOGLE_volkImpl = (PFN_vkGetRefreshCycleDurationGOOGLE)load(context, "vkGetRefreshCycleDurationGOOGLE_volkImpl");
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_HUAWEI_cluster_culling_shader)
	table->vkCmdDrawClusterHUAWEI_volkImpl = (PFN_vkCmdDrawClusterHUAWEI)load(context, "vkCmdDrawClusterHUAWEI_volkImpl");
	table->vkCmdDrawClusterIndirectHUAWEI_volkImpl = (PFN_vkCmdDrawClusterIndirectHUAWEI)load(context, "vkCmdDrawClusterIndirectHUAWEI_volkImpl");
#endif /* defined(VK_HUAWEI_cluster_culling_shader) */
#if defined(VK_HUAWEI_invocation_mask)
	table->vkCmdBindInvocationMaskHUAWEI_volkImpl = (PFN_vkCmdBindInvocationMaskHUAWEI)load(context, "vkCmdBindInvocationMaskHUAWEI_volkImpl");
#endif /* defined(VK_HUAWEI_invocation_mask) */
#if defined(VK_HUAWEI_subpass_shading)
	table->vkCmdSubpassShadingHUAWEI_volkImpl = (PFN_vkCmdSubpassShadingHUAWEI)load(context, "vkCmdSubpassShadingHUAWEI_volkImpl");
	table->vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI_volkImpl = (PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI)load(context, "vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI_volkImpl");
#endif /* defined(VK_HUAWEI_subpass_shading) */
#if defined(VK_INTEL_performance_query)
	table->vkAcquirePerformanceConfigurationINTEL_volkImpl = (PFN_vkAcquirePerformanceConfigurationINTEL)load(context, "vkAcquirePerformanceConfigurationINTEL_volkImpl");
	table->vkCmdSetPerformanceMarkerINTEL_volkImpl = (PFN_vkCmdSetPerformanceMarkerINTEL)load(context, "vkCmdSetPerformanceMarkerINTEL_volkImpl");
	table->vkCmdSetPerformanceOverrideINTEL_volkImpl = (PFN_vkCmdSetPerformanceOverrideINTEL)load(context, "vkCmdSetPerformanceOverrideINTEL_volkImpl");
	table->vkCmdSetPerformanceStreamMarkerINTEL_volkImpl = (PFN_vkCmdSetPerformanceStreamMarkerINTEL)load(context, "vkCmdSetPerformanceStreamMarkerINTEL_volkImpl");
	table->vkGetPerformanceParameterINTEL_volkImpl = (PFN_vkGetPerformanceParameterINTEL)load(context, "vkGetPerformanceParameterINTEL_volkImpl");
	table->vkInitializePerformanceApiINTEL_volkImpl = (PFN_vkInitializePerformanceApiINTEL)load(context, "vkInitializePerformanceApiINTEL_volkImpl");
	table->vkQueueSetPerformanceConfigurationINTEL_volkImpl = (PFN_vkQueueSetPerformanceConfigurationINTEL)load(context, "vkQueueSetPerformanceConfigurationINTEL_volkImpl");
	table->vkReleasePerformanceConfigurationINTEL_volkImpl = (PFN_vkReleasePerformanceConfigurationINTEL)load(context, "vkReleasePerformanceConfigurationINTEL_volkImpl");
	table->vkUninitializePerformanceApiINTEL_volkImpl = (PFN_vkUninitializePerformanceApiINTEL)load(context, "vkUninitializePerformanceApiINTEL_volkImpl");
#endif /* defined(VK_INTEL_performance_query) */
#if defined(VK_KHR_acceleration_structure)
	table->vkBuildAccelerationStructuresKHR_volkImpl = (PFN_vkBuildAccelerationStructuresKHR)load(context, "vkBuildAccelerationStructuresKHR_volkImpl");
	table->vkCmdBuildAccelerationStructuresIndirectKHR_volkImpl = (PFN_vkCmdBuildAccelerationStructuresIndirectKHR)load(context, "vkCmdBuildAccelerationStructuresIndirectKHR_volkImpl");
	table->vkCmdBuildAccelerationStructuresKHR_volkImpl = (PFN_vkCmdBuildAccelerationStructuresKHR)load(context, "vkCmdBuildAccelerationStructuresKHR_volkImpl");
	table->vkCmdCopyAccelerationStructureKHR_volkImpl = (PFN_vkCmdCopyAccelerationStructureKHR)load(context, "vkCmdCopyAccelerationStructureKHR_volkImpl");
	table->vkCmdCopyAccelerationStructureToMemoryKHR_volkImpl = (PFN_vkCmdCopyAccelerationStructureToMemoryKHR)load(context, "vkCmdCopyAccelerationStructureToMemoryKHR_volkImpl");
	table->vkCmdCopyMemoryToAccelerationStructureKHR_volkImpl = (PFN_vkCmdCopyMemoryToAccelerationStructureKHR)load(context, "vkCmdCopyMemoryToAccelerationStructureKHR_volkImpl");
	table->vkCmdWriteAccelerationStructuresPropertiesKHR_volkImpl = (PFN_vkCmdWriteAccelerationStructuresPropertiesKHR)load(context, "vkCmdWriteAccelerationStructuresPropertiesKHR_volkImpl");
	table->vkCopyAccelerationStructureKHR_volkImpl = (PFN_vkCopyAccelerationStructureKHR)load(context, "vkCopyAccelerationStructureKHR_volkImpl");
	table->vkCopyAccelerationStructureToMemoryKHR_volkImpl = (PFN_vkCopyAccelerationStructureToMemoryKHR)load(context, "vkCopyAccelerationStructureToMemoryKHR_volkImpl");
	table->vkCopyMemoryToAccelerationStructureKHR_volkImpl = (PFN_vkCopyMemoryToAccelerationStructureKHR)load(context, "vkCopyMemoryToAccelerationStructureKHR_volkImpl");
	table->vkCreateAccelerationStructureKHR_volkImpl = (PFN_vkCreateAccelerationStructureKHR)load(context, "vkCreateAccelerationStructureKHR_volkImpl");
	table->vkDestroyAccelerationStructureKHR_volkImpl = (PFN_vkDestroyAccelerationStructureKHR)load(context, "vkDestroyAccelerationStructureKHR_volkImpl");
	table->vkGetAccelerationStructureBuildSizesKHR_volkImpl = (PFN_vkGetAccelerationStructureBuildSizesKHR)load(context, "vkGetAccelerationStructureBuildSizesKHR_volkImpl");
	table->vkGetAccelerationStructureDeviceAddressKHR_volkImpl = (PFN_vkGetAccelerationStructureDeviceAddressKHR)load(context, "vkGetAccelerationStructureDeviceAddressKHR_volkImpl");
	table->vkGetDeviceAccelerationStructureCompatibilityKHR_volkImpl = (PFN_vkGetDeviceAccelerationStructureCompatibilityKHR)load(context, "vkGetDeviceAccelerationStructureCompatibilityKHR_volkImpl");
	table->vkWriteAccelerationStructuresPropertiesKHR_volkImpl = (PFN_vkWriteAccelerationStructuresPropertiesKHR)load(context, "vkWriteAccelerationStructuresPropertiesKHR_volkImpl");
#endif /* defined(VK_KHR_acceleration_structure) */
#if defined(VK_KHR_bind_memory2)
	table->vkBindBufferMemory2KHR_volkImpl = (PFN_vkBindBufferMemory2KHR)load(context, "vkBindBufferMemory2KHR_volkImpl");
	table->vkBindImageMemory2KHR_volkImpl = (PFN_vkBindImageMemory2KHR)load(context, "vkBindImageMemory2KHR_volkImpl");
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_buffer_device_address)
	table->vkGetBufferDeviceAddressKHR_volkImpl = (PFN_vkGetBufferDeviceAddressKHR)load(context, "vkGetBufferDeviceAddressKHR_volkImpl");
	table->vkGetBufferOpaqueCaptureAddressKHR_volkImpl = (PFN_vkGetBufferOpaqueCaptureAddressKHR)load(context, "vkGetBufferOpaqueCaptureAddressKHR_volkImpl");
	table->vkGetDeviceMemoryOpaqueCaptureAddressKHR_volkImpl = (PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR)load(context, "vkGetDeviceMemoryOpaqueCaptureAddressKHR_volkImpl");
#endif /* defined(VK_KHR_buffer_device_address) */
#if defined(VK_KHR_calibrated_timestamps)
	table->vkGetCalibratedTimestampsKHR_volkImpl = (PFN_vkGetCalibratedTimestampsKHR)load(context, "vkGetCalibratedTimestampsKHR_volkImpl");
#endif /* defined(VK_KHR_calibrated_timestamps) */
#if defined(VK_KHR_copy_commands2)
	table->vkCmdBlitImage2KHR_volkImpl = (PFN_vkCmdBlitImage2KHR)load(context, "vkCmdBlitImage2KHR_volkImpl");
	table->vkCmdCopyBuffer2KHR_volkImpl = (PFN_vkCmdCopyBuffer2KHR)load(context, "vkCmdCopyBuffer2KHR_volkImpl");
	table->vkCmdCopyBufferToImage2KHR_volkImpl = (PFN_vkCmdCopyBufferToImage2KHR)load(context, "vkCmdCopyBufferToImage2KHR_volkImpl");
	table->vkCmdCopyImage2KHR_volkImpl = (PFN_vkCmdCopyImage2KHR)load(context, "vkCmdCopyImage2KHR_volkImpl");
	table->vkCmdCopyImageToBuffer2KHR_volkImpl = (PFN_vkCmdCopyImageToBuffer2KHR)load(context, "vkCmdCopyImageToBuffer2KHR_volkImpl");
	table->vkCmdResolveImage2KHR_volkImpl = (PFN_vkCmdResolveImage2KHR)load(context, "vkCmdResolveImage2KHR_volkImpl");
#endif /* defined(VK_KHR_copy_commands2) */
#if defined(VK_KHR_create_renderpass2)
	table->vkCmdBeginRenderPass2KHR_volkImpl = (PFN_vkCmdBeginRenderPass2KHR)load(context, "vkCmdBeginRenderPass2KHR_volkImpl");
	table->vkCmdEndRenderPass2KHR_volkImpl = (PFN_vkCmdEndRenderPass2KHR)load(context, "vkCmdEndRenderPass2KHR_volkImpl");
	table->vkCmdNextSubpass2KHR_volkImpl = (PFN_vkCmdNextSubpass2KHR)load(context, "vkCmdNextSubpass2KHR_volkImpl");
	table->vkCreateRenderPass2KHR_volkImpl = (PFN_vkCreateRenderPass2KHR)load(context, "vkCreateRenderPass2KHR_volkImpl");
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_deferred_host_operations)
	table->vkCreateDeferredOperationKHR_volkImpl = (PFN_vkCreateDeferredOperationKHR)load(context, "vkCreateDeferredOperationKHR_volkImpl");
	table->vkDeferredOperationJoinKHR_volkImpl = (PFN_vkDeferredOperationJoinKHR)load(context, "vkDeferredOperationJoinKHR_volkImpl");
	table->vkDestroyDeferredOperationKHR_volkImpl = (PFN_vkDestroyDeferredOperationKHR)load(context, "vkDestroyDeferredOperationKHR_volkImpl");
	table->vkGetDeferredOperationMaxConcurrencyKHR_volkImpl = (PFN_vkGetDeferredOperationMaxConcurrencyKHR)load(context, "vkGetDeferredOperationMaxConcurrencyKHR_volkImpl");
	table->vkGetDeferredOperationResultKHR_volkImpl = (PFN_vkGetDeferredOperationResultKHR)load(context, "vkGetDeferredOperationResultKHR_volkImpl");
#endif /* defined(VK_KHR_deferred_host_operations) */
#if defined(VK_KHR_descriptor_update_template)
	table->vkCreateDescriptorUpdateTemplateKHR_volkImpl = (PFN_vkCreateDescriptorUpdateTemplateKHR)load(context, "vkCreateDescriptorUpdateTemplateKHR_volkImpl");
	table->vkDestroyDescriptorUpdateTemplateKHR_volkImpl = (PFN_vkDestroyDescriptorUpdateTemplateKHR)load(context, "vkDestroyDescriptorUpdateTemplateKHR_volkImpl");
	table->vkUpdateDescriptorSetWithTemplateKHR_volkImpl = (PFN_vkUpdateDescriptorSetWithTemplateKHR)load(context, "vkUpdateDescriptorSetWithTemplateKHR_volkImpl");
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
	table->vkCmdDispatchBaseKHR_volkImpl = (PFN_vkCmdDispatchBaseKHR)load(context, "vkCmdDispatchBaseKHR_volkImpl");
	table->vkCmdSetDeviceMaskKHR_volkImpl = (PFN_vkCmdSetDeviceMaskKHR)load(context, "vkCmdSetDeviceMaskKHR_volkImpl");
	table->vkGetDeviceGroupPeerMemoryFeaturesKHR_volkImpl = (PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR)load(context, "vkGetDeviceGroupPeerMemoryFeaturesKHR_volkImpl");
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_display_swapchain)
	table->vkCreateSharedSwapchainsKHR_volkImpl = (PFN_vkCreateSharedSwapchainsKHR)load(context, "vkCreateSharedSwapchainsKHR_volkImpl");
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
	table->vkCmdDrawIndexedIndirectCountKHR_volkImpl = (PFN_vkCmdDrawIndexedIndirectCountKHR)load(context, "vkCmdDrawIndexedIndirectCountKHR_volkImpl");
	table->vkCmdDrawIndirectCountKHR_volkImpl = (PFN_vkCmdDrawIndirectCountKHR)load(context, "vkCmdDrawIndirectCountKHR_volkImpl");
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_dynamic_rendering)
	table->vkCmdBeginRenderingKHR_volkImpl = (PFN_vkCmdBeginRenderingKHR)load(context, "vkCmdBeginRenderingKHR_volkImpl");
	table->vkCmdEndRenderingKHR_volkImpl = (PFN_vkCmdEndRenderingKHR)load(context, "vkCmdEndRenderingKHR_volkImpl");
#endif /* defined(VK_KHR_dynamic_rendering) */
#if defined(VK_KHR_dynamic_rendering_local_read)
	table->vkCmdSetRenderingAttachmentLocationsKHR_volkImpl = (PFN_vkCmdSetRenderingAttachmentLocationsKHR)load(context, "vkCmdSetRenderingAttachmentLocationsKHR_volkImpl");
	table->vkCmdSetRenderingInputAttachmentIndicesKHR_volkImpl = (PFN_vkCmdSetRenderingInputAttachmentIndicesKHR)load(context, "vkCmdSetRenderingInputAttachmentIndicesKHR_volkImpl");
#endif /* defined(VK_KHR_dynamic_rendering_local_read) */
#if defined(VK_KHR_external_fence_fd)
	table->vkGetFenceFdKHR_volkImpl = (PFN_vkGetFenceFdKHR)load(context, "vkGetFenceFdKHR_volkImpl");
	table->vkImportFenceFdKHR_volkImpl = (PFN_vkImportFenceFdKHR)load(context, "vkImportFenceFdKHR_volkImpl");
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
	table->vkGetFenceWin32HandleKHR_volkImpl = (PFN_vkGetFenceWin32HandleKHR)load(context, "vkGetFenceWin32HandleKHR_volkImpl");
	table->vkImportFenceWin32HandleKHR_volkImpl = (PFN_vkImportFenceWin32HandleKHR)load(context, "vkImportFenceWin32HandleKHR_volkImpl");
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_fd)
	table->vkGetMemoryFdKHR_volkImpl = (PFN_vkGetMemoryFdKHR)load(context, "vkGetMemoryFdKHR_volkImpl");
	table->vkGetMemoryFdPropertiesKHR_volkImpl = (PFN_vkGetMemoryFdPropertiesKHR)load(context, "vkGetMemoryFdPropertiesKHR_volkImpl");
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
	table->vkGetMemoryWin32HandleKHR_volkImpl = (PFN_vkGetMemoryWin32HandleKHR)load(context, "vkGetMemoryWin32HandleKHR_volkImpl");
	table->vkGetMemoryWin32HandlePropertiesKHR_volkImpl = (PFN_vkGetMemoryWin32HandlePropertiesKHR)load(context, "vkGetMemoryWin32HandlePropertiesKHR_volkImpl");
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_fd)
	table->vkGetSemaphoreFdKHR_volkImpl = (PFN_vkGetSemaphoreFdKHR)load(context, "vkGetSemaphoreFdKHR_volkImpl");
	table->vkImportSemaphoreFdKHR_volkImpl = (PFN_vkImportSemaphoreFdKHR)load(context, "vkImportSemaphoreFdKHR_volkImpl");
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
	table->vkGetSemaphoreWin32HandleKHR_volkImpl = (PFN_vkGetSemaphoreWin32HandleKHR)load(context, "vkGetSemaphoreWin32HandleKHR_volkImpl");
	table->vkImportSemaphoreWin32HandleKHR_volkImpl = (PFN_vkImportSemaphoreWin32HandleKHR)load(context, "vkImportSemaphoreWin32HandleKHR_volkImpl");
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_fragment_shading_rate)
	table->vkCmdSetFragmentShadingRateKHR_volkImpl = (PFN_vkCmdSetFragmentShadingRateKHR)load(context, "vkCmdSetFragmentShadingRateKHR_volkImpl");
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_memory_requirements2)
	table->vkGetBufferMemoryRequirements2KHR_volkImpl = (PFN_vkGetBufferMemoryRequirements2KHR)load(context, "vkGetBufferMemoryRequirements2KHR_volkImpl");
	table->vkGetImageMemoryRequirements2KHR_volkImpl = (PFN_vkGetImageMemoryRequirements2KHR)load(context, "vkGetImageMemoryRequirements2KHR_volkImpl");
	table->vkGetImageSparseMemoryRequirements2KHR_volkImpl = (PFN_vkGetImageSparseMemoryRequirements2KHR)load(context, "vkGetImageSparseMemoryRequirements2KHR_volkImpl");
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_line_rasterization)
	table->vkCmdSetLineStippleKHR_volkImpl = (PFN_vkCmdSetLineStippleKHR)load(context, "vkCmdSetLineStippleKHR_volkImpl");
#endif /* defined(VK_KHR_line_rasterization) */
#if defined(VK_KHR_maintenance1)
	table->vkTrimCommandPoolKHR_volkImpl = (PFN_vkTrimCommandPoolKHR)load(context, "vkTrimCommandPoolKHR_volkImpl");
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance3)
	table->vkGetDescriptorSetLayoutSupportKHR_volkImpl = (PFN_vkGetDescriptorSetLayoutSupportKHR)load(context, "vkGetDescriptorSetLayoutSupportKHR_volkImpl");
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_maintenance4)
	table->vkGetDeviceBufferMemoryRequirementsKHR_volkImpl = (PFN_vkGetDeviceBufferMemoryRequirementsKHR)load(context, "vkGetDeviceBufferMemoryRequirementsKHR_volkImpl");
	table->vkGetDeviceImageMemoryRequirementsKHR_volkImpl = (PFN_vkGetDeviceImageMemoryRequirementsKHR)load(context, "vkGetDeviceImageMemoryRequirementsKHR_volkImpl");
	table->vkGetDeviceImageSparseMemoryRequirementsKHR_volkImpl = (PFN_vkGetDeviceImageSparseMemoryRequirementsKHR)load(context, "vkGetDeviceImageSparseMemoryRequirementsKHR_volkImpl");
#endif /* defined(VK_KHR_maintenance4) */
#if defined(VK_KHR_maintenance5)
	table->vkCmdBindIndexBuffer2KHR_volkImpl = (PFN_vkCmdBindIndexBuffer2KHR)load(context, "vkCmdBindIndexBuffer2KHR_volkImpl");
	table->vkGetDeviceImageSubresourceLayoutKHR_volkImpl = (PFN_vkGetDeviceImageSubresourceLayoutKHR)load(context, "vkGetDeviceImageSubresourceLayoutKHR_volkImpl");
	table->vkGetImageSubresourceLayout2KHR_volkImpl = (PFN_vkGetImageSubresourceLayout2KHR)load(context, "vkGetImageSubresourceLayout2KHR_volkImpl");
	table->vkGetRenderingAreaGranularityKHR_volkImpl = (PFN_vkGetRenderingAreaGranularityKHR)load(context, "vkGetRenderingAreaGranularityKHR_volkImpl");
#endif /* defined(VK_KHR_maintenance5) */
#if defined(VK_KHR_maintenance6)
	table->vkCmdBindDescriptorSets2KHR_volkImpl = (PFN_vkCmdBindDescriptorSets2KHR)load(context, "vkCmdBindDescriptorSets2KHR_volkImpl");
	table->vkCmdPushConstants2KHR_volkImpl = (PFN_vkCmdPushConstants2KHR)load(context, "vkCmdPushConstants2KHR_volkImpl");
#endif /* defined(VK_KHR_maintenance6) */
#if defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor)
	table->vkCmdPushDescriptorSet2KHR_volkImpl = (PFN_vkCmdPushDescriptorSet2KHR)load(context, "vkCmdPushDescriptorSet2KHR_volkImpl");
	table->vkCmdPushDescriptorSetWithTemplate2KHR_volkImpl = (PFN_vkCmdPushDescriptorSetWithTemplate2KHR)load(context, "vkCmdPushDescriptorSetWithTemplate2KHR_volkImpl");
#endif /* defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer)
	table->vkCmdBindDescriptorBufferEmbeddedSamplers2EXT_volkImpl = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT)load(context, "vkCmdBindDescriptorBufferEmbeddedSamplers2EXT_volkImpl");
	table->vkCmdSetDescriptorBufferOffsets2EXT_volkImpl = (PFN_vkCmdSetDescriptorBufferOffsets2EXT)load(context, "vkCmdSetDescriptorBufferOffsets2EXT_volkImpl");
#endif /* defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer) */
#if defined(VK_KHR_map_memory2)
	table->vkMapMemory2KHR_volkImpl = (PFN_vkMapMemory2KHR)load(context, "vkMapMemory2KHR_volkImpl");
	table->vkUnmapMemory2KHR_volkImpl = (PFN_vkUnmapMemory2KHR)load(context, "vkUnmapMemory2KHR_volkImpl");
#endif /* defined(VK_KHR_map_memory2) */
#if defined(VK_KHR_performance_query)
	table->vkAcquireProfilingLockKHR_volkImpl = (PFN_vkAcquireProfilingLockKHR)load(context, "vkAcquireProfilingLockKHR_volkImpl");
	table->vkReleaseProfilingLockKHR_volkImpl = (PFN_vkReleaseProfilingLockKHR)load(context, "vkReleaseProfilingLockKHR_volkImpl");
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_pipeline_executable_properties)
	table->vkGetPipelineExecutableInternalRepresentationsKHR_volkImpl = (PFN_vkGetPipelineExecutableInternalRepresentationsKHR)load(context, "vkGetPipelineExecutableInternalRepresentationsKHR_volkImpl");
	table->vkGetPipelineExecutablePropertiesKHR_volkImpl = (PFN_vkGetPipelineExecutablePropertiesKHR)load(context, "vkGetPipelineExecutablePropertiesKHR_volkImpl");
	table->vkGetPipelineExecutableStatisticsKHR_volkImpl = (PFN_vkGetPipelineExecutableStatisticsKHR)load(context, "vkGetPipelineExecutableStatisticsKHR_volkImpl");
#endif /* defined(VK_KHR_pipeline_executable_properties) */
#if defined(VK_KHR_present_wait)
	table->vkWaitForPresentKHR_volkImpl = (PFN_vkWaitForPresentKHR)load(context, "vkWaitForPresentKHR_volkImpl");
#endif /* defined(VK_KHR_present_wait) */
#if defined(VK_KHR_push_descriptor)
	table->vkCmdPushDescriptorSetKHR_volkImpl = (PFN_vkCmdPushDescriptorSetKHR)load(context, "vkCmdPushDescriptorSetKHR_volkImpl");
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline)
	table->vkCmdTraceRaysIndirect2KHR_volkImpl = (PFN_vkCmdTraceRaysIndirect2KHR)load(context, "vkCmdTraceRaysIndirect2KHR_volkImpl");
#endif /* defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_ray_tracing_pipeline)
	table->vkCmdSetRayTracingPipelineStackSizeKHR_volkImpl = (PFN_vkCmdSetRayTracingPipelineStackSizeKHR)load(context, "vkCmdSetRayTracingPipelineStackSizeKHR_volkImpl");
	table->vkCmdTraceRaysIndirectKHR_volkImpl = (PFN_vkCmdTraceRaysIndirectKHR)load(context, "vkCmdTraceRaysIndirectKHR_volkImpl");
	table->vkCmdTraceRaysKHR_volkImpl = (PFN_vkCmdTraceRaysKHR)load(context, "vkCmdTraceRaysKHR_volkImpl");
	table->vkCreateRayTracingPipelinesKHR_volkImpl = (PFN_vkCreateRayTracingPipelinesKHR)load(context, "vkCreateRayTracingPipelinesKHR_volkImpl");
	table->vkGetRayTracingCaptureReplayShaderGroupHandlesKHR_volkImpl = (PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR)load(context, "vkGetRayTracingCaptureReplayShaderGroupHandlesKHR_volkImpl");
	table->vkGetRayTracingShaderGroupHandlesKHR_volkImpl = (PFN_vkGetRayTracingShaderGroupHandlesKHR)load(context, "vkGetRayTracingShaderGroupHandlesKHR_volkImpl");
	table->vkGetRayTracingShaderGroupStackSizeKHR_volkImpl = (PFN_vkGetRayTracingShaderGroupStackSizeKHR)load(context, "vkGetRayTracingShaderGroupStackSizeKHR_volkImpl");
#endif /* defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
	table->vkCreateSamplerYcbcrConversionKHR_volkImpl = (PFN_vkCreateSamplerYcbcrConversionKHR)load(context, "vkCreateSamplerYcbcrConversionKHR_volkImpl");
	table->vkDestroySamplerYcbcrConversionKHR_volkImpl = (PFN_vkDestroySamplerYcbcrConversionKHR)load(context, "vkDestroySamplerYcbcrConversionKHR_volkImpl");
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
	table->vkGetSwapchainStatusKHR_volkImpl = (PFN_vkGetSwapchainStatusKHR)load(context, "vkGetSwapchainStatusKHR_volkImpl");
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_swapchain)
	table->vkAcquireNextImageKHR_volkImpl = (PFN_vkAcquireNextImageKHR)load(context, "vkAcquireNextImageKHR_volkImpl");
	table->vkCreateSwapchainKHR_volkImpl = (PFN_vkCreateSwapchainKHR)load(context, "vkCreateSwapchainKHR_volkImpl");
	table->vkDestroySwapchainKHR_volkImpl = (PFN_vkDestroySwapchainKHR)load(context, "vkDestroySwapchainKHR_volkImpl");
	table->vkGetSwapchainImagesKHR_volkImpl = (PFN_vkGetSwapchainImagesKHR)load(context, "vkGetSwapchainImagesKHR_volkImpl");
	table->vkQueuePresentKHR_volkImpl = (PFN_vkQueuePresentKHR)load(context, "vkQueuePresentKHR_volkImpl");
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_synchronization2)
	table->vkCmdPipelineBarrier2KHR_volkImpl = (PFN_vkCmdPipelineBarrier2KHR)load(context, "vkCmdPipelineBarrier2KHR_volkImpl");
	table->vkCmdResetEvent2KHR_volkImpl = (PFN_vkCmdResetEvent2KHR)load(context, "vkCmdResetEvent2KHR_volkImpl");
	table->vkCmdSetEvent2KHR_volkImpl = (PFN_vkCmdSetEvent2KHR)load(context, "vkCmdSetEvent2KHR_volkImpl");
	table->vkCmdWaitEvents2KHR_volkImpl = (PFN_vkCmdWaitEvents2KHR)load(context, "vkCmdWaitEvents2KHR_volkImpl");
	table->vkCmdWriteTimestamp2KHR_volkImpl = (PFN_vkCmdWriteTimestamp2KHR)load(context, "vkCmdWriteTimestamp2KHR_volkImpl");
	table->vkQueueSubmit2KHR_volkImpl = (PFN_vkQueueSubmit2KHR)load(context, "vkQueueSubmit2KHR_volkImpl");
#endif /* defined(VK_KHR_synchronization2) */
#if defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker)
	table->vkCmdWriteBufferMarker2AMD_volkImpl = (PFN_vkCmdWriteBufferMarker2AMD)load(context, "vkCmdWriteBufferMarker2AMD_volkImpl");
#endif /* defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker) */
#if defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints)
	table->vkGetQueueCheckpointData2NV_volkImpl = (PFN_vkGetQueueCheckpointData2NV)load(context, "vkGetQueueCheckpointData2NV_volkImpl");
#endif /* defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_KHR_timeline_semaphore)
	table->vkGetSemaphoreCounterValueKHR_volkImpl = (PFN_vkGetSemaphoreCounterValueKHR)load(context, "vkGetSemaphoreCounterValueKHR_volkImpl");
	table->vkSignalSemaphoreKHR_volkImpl = (PFN_vkSignalSemaphoreKHR)load(context, "vkSignalSemaphoreKHR_volkImpl");
	table->vkWaitSemaphoresKHR_volkImpl = (PFN_vkWaitSemaphoresKHR)load(context, "vkWaitSemaphoresKHR_volkImpl");
#endif /* defined(VK_KHR_timeline_semaphore) */
#if defined(VK_KHR_video_decode_queue)
	table->vkCmdDecodeVideoKHR_volkImpl = (PFN_vkCmdDecodeVideoKHR)load(context, "vkCmdDecodeVideoKHR_volkImpl");
#endif /* defined(VK_KHR_video_decode_queue) */
#if defined(VK_KHR_video_encode_queue)
	table->vkCmdEncodeVideoKHR_volkImpl = (PFN_vkCmdEncodeVideoKHR)load(context, "vkCmdEncodeVideoKHR_volkImpl");
	table->vkGetEncodedVideoSessionParametersKHR_volkImpl = (PFN_vkGetEncodedVideoSessionParametersKHR)load(context, "vkGetEncodedVideoSessionParametersKHR_volkImpl");
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
	table->vkBindVideoSessionMemoryKHR_volkImpl = (PFN_vkBindVideoSessionMemoryKHR)load(context, "vkBindVideoSessionMemoryKHR_volkImpl");
	table->vkCmdBeginVideoCodingKHR_volkImpl = (PFN_vkCmdBeginVideoCodingKHR)load(context, "vkCmdBeginVideoCodingKHR_volkImpl");
	table->vkCmdControlVideoCodingKHR_volkImpl = (PFN_vkCmdControlVideoCodingKHR)load(context, "vkCmdControlVideoCodingKHR_volkImpl");
	table->vkCmdEndVideoCodingKHR_volkImpl = (PFN_vkCmdEndVideoCodingKHR)load(context, "vkCmdEndVideoCodingKHR_volkImpl");
	table->vkCreateVideoSessionKHR_volkImpl = (PFN_vkCreateVideoSessionKHR)load(context, "vkCreateVideoSessionKHR_volkImpl");
	table->vkCreateVideoSessionParametersKHR_volkImpl = (PFN_vkCreateVideoSessionParametersKHR)load(context, "vkCreateVideoSessionParametersKHR_volkImpl");
	table->vkDestroyVideoSessionKHR_volkImpl = (PFN_vkDestroyVideoSessionKHR)load(context, "vkDestroyVideoSessionKHR_volkImpl");
	table->vkDestroyVideoSessionParametersKHR_volkImpl = (PFN_vkDestroyVideoSessionParametersKHR)load(context, "vkDestroyVideoSessionParametersKHR_volkImpl");
	table->vkGetVideoSessionMemoryRequirementsKHR_volkImpl = (PFN_vkGetVideoSessionMemoryRequirementsKHR)load(context, "vkGetVideoSessionMemoryRequirementsKHR_volkImpl");
	table->vkUpdateVideoSessionParametersKHR_volkImpl = (PFN_vkUpdateVideoSessionParametersKHR)load(context, "vkUpdateVideoSessionParametersKHR_volkImpl");
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_NVX_binary_import)
	table->vkCmdCuLaunchKernelNVX_volkImpl = (PFN_vkCmdCuLaunchKernelNVX)load(context, "vkCmdCuLaunchKernelNVX_volkImpl");
	table->vkCreateCuFunctionNVX_volkImpl = (PFN_vkCreateCuFunctionNVX)load(context, "vkCreateCuFunctionNVX_volkImpl");
	table->vkCreateCuModuleNVX_volkImpl = (PFN_vkCreateCuModuleNVX)load(context, "vkCreateCuModuleNVX_volkImpl");
	table->vkDestroyCuFunctionNVX_volkImpl = (PFN_vkDestroyCuFunctionNVX)load(context, "vkDestroyCuFunctionNVX_volkImpl");
	table->vkDestroyCuModuleNVX_volkImpl = (PFN_vkDestroyCuModuleNVX)load(context, "vkDestroyCuModuleNVX_volkImpl");
#endif /* defined(VK_NVX_binary_import) */
#if defined(VK_NVX_image_view_handle)
	table->vkGetImageViewAddressNVX_volkImpl = (PFN_vkGetImageViewAddressNVX)load(context, "vkGetImageViewAddressNVX_volkImpl");
	table->vkGetImageViewHandleNVX_volkImpl = (PFN_vkGetImageViewHandleNVX)load(context, "vkGetImageViewHandleNVX_volkImpl");
#endif /* defined(VK_NVX_image_view_handle) */
#if defined(VK_NV_clip_space_w_scaling)
	table->vkCmdSetViewportWScalingNV_volkImpl = (PFN_vkCmdSetViewportWScalingNV)load(context, "vkCmdSetViewportWScalingNV_volkImpl");
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_copy_memory_indirect)
	table->vkCmdCopyMemoryIndirectNV_volkImpl = (PFN_vkCmdCopyMemoryIndirectNV)load(context, "vkCmdCopyMemoryIndirectNV_volkImpl");
	table->vkCmdCopyMemoryToImageIndirectNV_volkImpl = (PFN_vkCmdCopyMemoryToImageIndirectNV)load(context, "vkCmdCopyMemoryToImageIndirectNV_volkImpl");
#endif /* defined(VK_NV_copy_memory_indirect) */
#if defined(VK_NV_cuda_kernel_launch)
	table->vkCmdCudaLaunchKernelNV_volkImpl = (PFN_vkCmdCudaLaunchKernelNV)load(context, "vkCmdCudaLaunchKernelNV_volkImpl");
	table->vkCreateCudaFunctionNV_volkImpl = (PFN_vkCreateCudaFunctionNV)load(context, "vkCreateCudaFunctionNV_volkImpl");
	table->vkCreateCudaModuleNV_volkImpl = (PFN_vkCreateCudaModuleNV)load(context, "vkCreateCudaModuleNV_volkImpl");
	table->vkDestroyCudaFunctionNV_volkImpl = (PFN_vkDestroyCudaFunctionNV)load(context, "vkDestroyCudaFunctionNV_volkImpl");
	table->vkDestroyCudaModuleNV_volkImpl = (PFN_vkDestroyCudaModuleNV)load(context, "vkDestroyCudaModuleNV_volkImpl");
	table->vkGetCudaModuleCacheNV_volkImpl = (PFN_vkGetCudaModuleCacheNV)load(context, "vkGetCudaModuleCacheNV_volkImpl");
#endif /* defined(VK_NV_cuda_kernel_launch) */
#if defined(VK_NV_device_diagnostic_checkpoints)
	table->vkCmdSetCheckpointNV_volkImpl = (PFN_vkCmdSetCheckpointNV)load(context, "vkCmdSetCheckpointNV_volkImpl");
	table->vkGetQueueCheckpointDataNV_volkImpl = (PFN_vkGetQueueCheckpointDataNV)load(context, "vkGetQueueCheckpointDataNV_volkImpl");
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_device_generated_commands)
	table->vkCmdBindPipelineShaderGroupNV_volkImpl = (PFN_vkCmdBindPipelineShaderGroupNV)load(context, "vkCmdBindPipelineShaderGroupNV_volkImpl");
	table->vkCmdExecuteGeneratedCommandsNV_volkImpl = (PFN_vkCmdExecuteGeneratedCommandsNV)load(context, "vkCmdExecuteGeneratedCommandsNV_volkImpl");
	table->vkCmdPreprocessGeneratedCommandsNV_volkImpl = (PFN_vkCmdPreprocessGeneratedCommandsNV)load(context, "vkCmdPreprocessGeneratedCommandsNV_volkImpl");
	table->vkCreateIndirectCommandsLayoutNV_volkImpl = (PFN_vkCreateIndirectCommandsLayoutNV)load(context, "vkCreateIndirectCommandsLayoutNV_volkImpl");
	table->vkDestroyIndirectCommandsLayoutNV_volkImpl = (PFN_vkDestroyIndirectCommandsLayoutNV)load(context, "vkDestroyIndirectCommandsLayoutNV_volkImpl");
	table->vkGetGeneratedCommandsMemoryRequirementsNV_volkImpl = (PFN_vkGetGeneratedCommandsMemoryRequirementsNV)load(context, "vkGetGeneratedCommandsMemoryRequirementsNV_volkImpl");
#endif /* defined(VK_NV_device_generated_commands) */
#if defined(VK_NV_device_generated_commands_compute)
	table->vkCmdUpdatePipelineIndirectBufferNV_volkImpl = (PFN_vkCmdUpdatePipelineIndirectBufferNV)load(context, "vkCmdUpdatePipelineIndirectBufferNV_volkImpl");
	table->vkGetPipelineIndirectDeviceAddressNV_volkImpl = (PFN_vkGetPipelineIndirectDeviceAddressNV)load(context, "vkGetPipelineIndirectDeviceAddressNV_volkImpl");
	table->vkGetPipelineIndirectMemoryRequirementsNV_volkImpl = (PFN_vkGetPipelineIndirectMemoryRequirementsNV)load(context, "vkGetPipelineIndirectMemoryRequirementsNV_volkImpl");
#endif /* defined(VK_NV_device_generated_commands_compute) */
#if defined(VK_NV_external_memory_rdma)
	table->vkGetMemoryRemoteAddressNV_volkImpl = (PFN_vkGetMemoryRemoteAddressNV)load(context, "vkGetMemoryRemoteAddressNV_volkImpl");
#endif /* defined(VK_NV_external_memory_rdma) */
#if defined(VK_NV_external_memory_win32)
	table->vkGetMemoryWin32HandleNV_volkImpl = (PFN_vkGetMemoryWin32HandleNV)load(context, "vkGetMemoryWin32HandleNV_volkImpl");
#endif /* defined(VK_NV_external_memory_win32) */
#if defined(VK_NV_fragment_shading_rate_enums)
	table->vkCmdSetFragmentShadingRateEnumNV_volkImpl = (PFN_vkCmdSetFragmentShadingRateEnumNV)load(context, "vkCmdSetFragmentShadingRateEnumNV_volkImpl");
#endif /* defined(VK_NV_fragment_shading_rate_enums) */
#if defined(VK_NV_low_latency2)
	table->vkGetLatencyTimingsNV_volkImpl = (PFN_vkGetLatencyTimingsNV)load(context, "vkGetLatencyTimingsNV_volkImpl");
	table->vkLatencySleepNV_volkImpl = (PFN_vkLatencySleepNV)load(context, "vkLatencySleepNV_volkImpl");
	table->vkQueueNotifyOutOfBandNV_volkImpl = (PFN_vkQueueNotifyOutOfBandNV)load(context, "vkQueueNotifyOutOfBandNV_volkImpl");
	table->vkSetLatencyMarkerNV_volkImpl = (PFN_vkSetLatencyMarkerNV)load(context, "vkSetLatencyMarkerNV_volkImpl");
	table->vkSetLatencySleepModeNV_volkImpl = (PFN_vkSetLatencySleepModeNV)load(context, "vkSetLatencySleepModeNV_volkImpl");
#endif /* defined(VK_NV_low_latency2) */
#if defined(VK_NV_memory_decompression)
	table->vkCmdDecompressMemoryIndirectCountNV_volkImpl = (PFN_vkCmdDecompressMemoryIndirectCountNV)load(context, "vkCmdDecompressMemoryIndirectCountNV_volkImpl");
	table->vkCmdDecompressMemoryNV_volkImpl = (PFN_vkCmdDecompressMemoryNV)load(context, "vkCmdDecompressMemoryNV_volkImpl");
#endif /* defined(VK_NV_memory_decompression) */
#if defined(VK_NV_mesh_shader)
	table->vkCmdDrawMeshTasksIndirectCountNV_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectCountNV)load(context, "vkCmdDrawMeshTasksIndirectCountNV_volkImpl");
	table->vkCmdDrawMeshTasksIndirectNV_volkImpl = (PFN_vkCmdDrawMeshTasksIndirectNV)load(context, "vkCmdDrawMeshTasksIndirectNV_volkImpl");
	table->vkCmdDrawMeshTasksNV_volkImpl = (PFN_vkCmdDrawMeshTasksNV)load(context, "vkCmdDrawMeshTasksNV_volkImpl");
#endif /* defined(VK_NV_mesh_shader) */
#if defined(VK_NV_optical_flow)
	table->vkBindOpticalFlowSessionImageNV_volkImpl = (PFN_vkBindOpticalFlowSessionImageNV)load(context, "vkBindOpticalFlowSessionImageNV_volkImpl");
	table->vkCmdOpticalFlowExecuteNV_volkImpl = (PFN_vkCmdOpticalFlowExecuteNV)load(context, "vkCmdOpticalFlowExecuteNV_volkImpl");
	table->vkCreateOpticalFlowSessionNV_volkImpl = (PFN_vkCreateOpticalFlowSessionNV)load(context, "vkCreateOpticalFlowSessionNV_volkImpl");
	table->vkDestroyOpticalFlowSessionNV_volkImpl = (PFN_vkDestroyOpticalFlowSessionNV)load(context, "vkDestroyOpticalFlowSessionNV_volkImpl");
#endif /* defined(VK_NV_optical_flow) */
#if defined(VK_NV_ray_tracing)
	table->vkBindAccelerationStructureMemoryNV_volkImpl = (PFN_vkBindAccelerationStructureMemoryNV)load(context, "vkBindAccelerationStructureMemoryNV_volkImpl");
	table->vkCmdBuildAccelerationStructureNV_volkImpl = (PFN_vkCmdBuildAccelerationStructureNV)load(context, "vkCmdBuildAccelerationStructureNV_volkImpl");
	table->vkCmdCopyAccelerationStructureNV_volkImpl = (PFN_vkCmdCopyAccelerationStructureNV)load(context, "vkCmdCopyAccelerationStructureNV_volkImpl");
	table->vkCmdTraceRaysNV_volkImpl = (PFN_vkCmdTraceRaysNV)load(context, "vkCmdTraceRaysNV_volkImpl");
	table->vkCmdWriteAccelerationStructuresPropertiesNV_volkImpl = (PFN_vkCmdWriteAccelerationStructuresPropertiesNV)load(context, "vkCmdWriteAccelerationStructuresPropertiesNV_volkImpl");
	table->vkCompileDeferredNV_volkImpl = (PFN_vkCompileDeferredNV)load(context, "vkCompileDeferredNV_volkImpl");
	table->vkCreateAccelerationStructureNV_volkImpl = (PFN_vkCreateAccelerationStructureNV)load(context, "vkCreateAccelerationStructureNV_volkImpl");
	table->vkCreateRayTracingPipelinesNV_volkImpl = (PFN_vkCreateRayTracingPipelinesNV)load(context, "vkCreateRayTracingPipelinesNV_volkImpl");
	table->vkDestroyAccelerationStructureNV_volkImpl = (PFN_vkDestroyAccelerationStructureNV)load(context, "vkDestroyAccelerationStructureNV_volkImpl");
	table->vkGetAccelerationStructureHandleNV_volkImpl = (PFN_vkGetAccelerationStructureHandleNV)load(context, "vkGetAccelerationStructureHandleNV_volkImpl");
	table->vkGetAccelerationStructureMemoryRequirementsNV_volkImpl = (PFN_vkGetAccelerationStructureMemoryRequirementsNV)load(context, "vkGetAccelerationStructureMemoryRequirementsNV_volkImpl");
	table->vkGetRayTracingShaderGroupHandlesNV_volkImpl = (PFN_vkGetRayTracingShaderGroupHandlesNV)load(context, "vkGetRayTracingShaderGroupHandlesNV_volkImpl");
#endif /* defined(VK_NV_ray_tracing) */
#if defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2
	table->vkCmdSetExclusiveScissorEnableNV_volkImpl = (PFN_vkCmdSetExclusiveScissorEnableNV)load(context, "vkCmdSetExclusiveScissorEnableNV_volkImpl");
#endif /* defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2 */
#if defined(VK_NV_scissor_exclusive)
	table->vkCmdSetExclusiveScissorNV_volkImpl = (PFN_vkCmdSetExclusiveScissorNV)load(context, "vkCmdSetExclusiveScissorNV_volkImpl");
#endif /* defined(VK_NV_scissor_exclusive) */
#if defined(VK_NV_shading_rate_image)
	table->vkCmdBindShadingRateImageNV_volkImpl = (PFN_vkCmdBindShadingRateImageNV)load(context, "vkCmdBindShadingRateImageNV_volkImpl");
	table->vkCmdSetCoarseSampleOrderNV_volkImpl = (PFN_vkCmdSetCoarseSampleOrderNV)load(context, "vkCmdSetCoarseSampleOrderNV_volkImpl");
	table->vkCmdSetViewportShadingRatePaletteNV_volkImpl = (PFN_vkCmdSetViewportShadingRatePaletteNV)load(context, "vkCmdSetViewportShadingRatePaletteNV_volkImpl");
#endif /* defined(VK_NV_shading_rate_image) */
#if defined(VK_QCOM_tile_properties)
	table->vkGetDynamicRenderingTilePropertiesQCOM_volkImpl = (PFN_vkGetDynamicRenderingTilePropertiesQCOM)load(context, "vkGetDynamicRenderingTilePropertiesQCOM_volkImpl");
	table->vkGetFramebufferTilePropertiesQCOM_volkImpl = (PFN_vkGetFramebufferTilePropertiesQCOM)load(context, "vkGetFramebufferTilePropertiesQCOM_volkImpl");
#endif /* defined(VK_QCOM_tile_properties) */
#if defined(VK_QNX_external_memory_screen_buffer)
	table->vkGetScreenBufferPropertiesQNX_volkImpl = (PFN_vkGetScreenBufferPropertiesQNX)load(context, "vkGetScreenBufferPropertiesQNX_volkImpl");
#endif /* defined(VK_QNX_external_memory_screen_buffer) */
#if defined(VK_VALVE_descriptor_set_host_mapping)
	table->vkGetDescriptorSetHostMappingVALVE_volkImpl = (PFN_vkGetDescriptorSetHostMappingVALVE)load(context, "vkGetDescriptorSetHostMappingVALVE_volkImpl");
	table->vkGetDescriptorSetLayoutHostMappingInfoVALVE_volkImpl = (PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE)load(context, "vkGetDescriptorSetLayoutHostMappingInfoVALVE_volkImpl");
#endif /* defined(VK_VALVE_descriptor_set_host_mapping) */
#if (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object))
	table->vkCmdBindVertexBuffers2EXT_volkImpl = (PFN_vkCmdBindVertexBuffers2EXT)load(context, "vkCmdBindVertexBuffers2EXT_volkImpl");
	table->vkCmdSetCullModeEXT_volkImpl = (PFN_vkCmdSetCullModeEXT)load(context, "vkCmdSetCullModeEXT_volkImpl");
	table->vkCmdSetDepthBoundsTestEnableEXT_volkImpl = (PFN_vkCmdSetDepthBoundsTestEnableEXT)load(context, "vkCmdSetDepthBoundsTestEnableEXT_volkImpl");
	table->vkCmdSetDepthCompareOpEXT_volkImpl = (PFN_vkCmdSetDepthCompareOpEXT)load(context, "vkCmdSetDepthCompareOpEXT_volkImpl");
	table->vkCmdSetDepthTestEnableEXT_volkImpl = (PFN_vkCmdSetDepthTestEnableEXT)load(context, "vkCmdSetDepthTestEnableEXT_volkImpl");
	table->vkCmdSetDepthWriteEnableEXT_volkImpl = (PFN_vkCmdSetDepthWriteEnableEXT)load(context, "vkCmdSetDepthWriteEnableEXT_volkImpl");
	table->vkCmdSetFrontFaceEXT_volkImpl = (PFN_vkCmdSetFrontFaceEXT)load(context, "vkCmdSetFrontFaceEXT_volkImpl");
	table->vkCmdSetPrimitiveTopologyEXT_volkImpl = (PFN_vkCmdSetPrimitiveTopologyEXT)load(context, "vkCmdSetPrimitiveTopologyEXT_volkImpl");
	table->vkCmdSetScissorWithCountEXT_volkImpl = (PFN_vkCmdSetScissorWithCountEXT)load(context, "vkCmdSetScissorWithCountEXT_volkImpl");
	table->vkCmdSetStencilOpEXT_volkImpl = (PFN_vkCmdSetStencilOpEXT)load(context, "vkCmdSetStencilOpEXT_volkImpl");
	table->vkCmdSetStencilTestEnableEXT_volkImpl = (PFN_vkCmdSetStencilTestEnableEXT)load(context, "vkCmdSetStencilTestEnableEXT_volkImpl");
	table->vkCmdSetViewportWithCountEXT_volkImpl = (PFN_vkCmdSetViewportWithCountEXT)load(context, "vkCmdSetViewportWithCountEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object))
	table->vkCmdSetDepthBiasEnableEXT_volkImpl = (PFN_vkCmdSetDepthBiasEnableEXT)load(context, "vkCmdSetDepthBiasEnableEXT_volkImpl");
	table->vkCmdSetLogicOpEXT_volkImpl = (PFN_vkCmdSetLogicOpEXT)load(context, "vkCmdSetLogicOpEXT_volkImpl");
	table->vkCmdSetPatchControlPointsEXT_volkImpl = (PFN_vkCmdSetPatchControlPointsEXT)load(context, "vkCmdSetPatchControlPointsEXT_volkImpl");
	table->vkCmdSetPrimitiveRestartEnableEXT_volkImpl = (PFN_vkCmdSetPrimitiveRestartEnableEXT)load(context, "vkCmdSetPrimitiveRestartEnableEXT_volkImpl");
	table->vkCmdSetRasterizerDiscardEnableEXT_volkImpl = (PFN_vkCmdSetRasterizerDiscardEnableEXT)load(context, "vkCmdSetRasterizerDiscardEnableEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object))
	table->vkCmdSetAlphaToCoverageEnableEXT_volkImpl = (PFN_vkCmdSetAlphaToCoverageEnableEXT)load(context, "vkCmdSetAlphaToCoverageEnableEXT_volkImpl");
	table->vkCmdSetAlphaToOneEnableEXT_volkImpl = (PFN_vkCmdSetAlphaToOneEnableEXT)load(context, "vkCmdSetAlphaToOneEnableEXT_volkImpl");
	table->vkCmdSetColorBlendEnableEXT_volkImpl = (PFN_vkCmdSetColorBlendEnableEXT)load(context, "vkCmdSetColorBlendEnableEXT_volkImpl");
	table->vkCmdSetColorBlendEquationEXT_volkImpl = (PFN_vkCmdSetColorBlendEquationEXT)load(context, "vkCmdSetColorBlendEquationEXT_volkImpl");
	table->vkCmdSetColorWriteMaskEXT_volkImpl = (PFN_vkCmdSetColorWriteMaskEXT)load(context, "vkCmdSetColorWriteMaskEXT_volkImpl");
	table->vkCmdSetDepthClampEnableEXT_volkImpl = (PFN_vkCmdSetDepthClampEnableEXT)load(context, "vkCmdSetDepthClampEnableEXT_volkImpl");
	table->vkCmdSetLogicOpEnableEXT_volkImpl = (PFN_vkCmdSetLogicOpEnableEXT)load(context, "vkCmdSetLogicOpEnableEXT_volkImpl");
	table->vkCmdSetPolygonModeEXT_volkImpl = (PFN_vkCmdSetPolygonModeEXT)load(context, "vkCmdSetPolygonModeEXT_volkImpl");
	table->vkCmdSetRasterizationSamplesEXT_volkImpl = (PFN_vkCmdSetRasterizationSamplesEXT)load(context, "vkCmdSetRasterizationSamplesEXT_volkImpl");
	table->vkCmdSetSampleMaskEXT_volkImpl = (PFN_vkCmdSetSampleMaskEXT)load(context, "vkCmdSetSampleMaskEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object))
	table->vkCmdSetTessellationDomainOriginEXT_volkImpl = (PFN_vkCmdSetTessellationDomainOriginEXT)load(context, "vkCmdSetTessellationDomainOriginEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback))
	table->vkCmdSetRasterizationStreamEXT_volkImpl = (PFN_vkCmdSetRasterizationStreamEXT)load(context, "vkCmdSetRasterizationStreamEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization))
	table->vkCmdSetConservativeRasterizationModeEXT_volkImpl = (PFN_vkCmdSetConservativeRasterizationModeEXT)load(context, "vkCmdSetConservativeRasterizationModeEXT_volkImpl");
	table->vkCmdSetExtraPrimitiveOverestimationSizeEXT_volkImpl = (PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT)load(context, "vkCmdSetExtraPrimitiveOverestimationSizeEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable))
	table->vkCmdSetDepthClipEnableEXT_volkImpl = (PFN_vkCmdSetDepthClipEnableEXT)load(context, "vkCmdSetDepthClipEnableEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations))
	table->vkCmdSetSampleLocationsEnableEXT_volkImpl = (PFN_vkCmdSetSampleLocationsEnableEXT)load(context, "vkCmdSetSampleLocationsEnableEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced))
	table->vkCmdSetColorBlendAdvancedEXT_volkImpl = (PFN_vkCmdSetColorBlendAdvancedEXT)load(context, "vkCmdSetColorBlendAdvancedEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex))
	table->vkCmdSetProvokingVertexModeEXT_volkImpl = (PFN_vkCmdSetProvokingVertexModeEXT)load(context, "vkCmdSetProvokingVertexModeEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization))
	table->vkCmdSetLineRasterizationModeEXT_volkImpl = (PFN_vkCmdSetLineRasterizationModeEXT)load(context, "vkCmdSetLineRasterizationModeEXT_volkImpl");
	table->vkCmdSetLineStippleEnableEXT_volkImpl = (PFN_vkCmdSetLineStippleEnableEXT)load(context, "vkCmdSetLineStippleEnableEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control))
	table->vkCmdSetDepthClipNegativeOneToOneEXT_volkImpl = (PFN_vkCmdSetDepthClipNegativeOneToOneEXT)load(context, "vkCmdSetDepthClipNegativeOneToOneEXT_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling))
	table->vkCmdSetViewportWScalingEnableNV_volkImpl = (PFN_vkCmdSetViewportWScalingEnableNV)load(context, "vkCmdSetViewportWScalingEnableNV_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle))
	table->vkCmdSetViewportSwizzleNV_volkImpl = (PFN_vkCmdSetViewportSwizzleNV)load(context, "vkCmdSetViewportSwizzleNV_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color))
	table->vkCmdSetCoverageToColorEnableNV_volkImpl = (PFN_vkCmdSetCoverageToColorEnableNV)load(context, "vkCmdSetCoverageToColorEnableNV_volkImpl");
	table->vkCmdSetCoverageToColorLocationNV_volkImpl = (PFN_vkCmdSetCoverageToColorLocationNV)load(context, "vkCmdSetCoverageToColorLocationNV_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples))
	table->vkCmdSetCoverageModulationModeNV_volkImpl = (PFN_vkCmdSetCoverageModulationModeNV)load(context, "vkCmdSetCoverageModulationModeNV_volkImpl");
	table->vkCmdSetCoverageModulationTableEnableNV_volkImpl = (PFN_vkCmdSetCoverageModulationTableEnableNV)load(context, "vkCmdSetCoverageModulationTableEnableNV_volkImpl");
	table->vkCmdSetCoverageModulationTableNV_volkImpl = (PFN_vkCmdSetCoverageModulationTableNV)load(context, "vkCmdSetCoverageModulationTableNV_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image))
	table->vkCmdSetShadingRateImageEnableNV_volkImpl = (PFN_vkCmdSetShadingRateImageEnableNV)load(context, "vkCmdSetShadingRateImageEnableNV_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test))
	table->vkCmdSetRepresentativeFragmentTestEnableNV_volkImpl = (PFN_vkCmdSetRepresentativeFragmentTestEnableNV)load(context, "vkCmdSetRepresentativeFragmentTestEnableNV_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode))
	table->vkCmdSetCoverageReductionModeNV_volkImpl = (PFN_vkCmdSetCoverageReductionModeNV)load(context, "vkCmdSetCoverageReductionModeNV_volkImpl");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode)) */
#if (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1))
	table->vkGetDeviceGroupSurfacePresentModes2EXT_volkImpl = (PFN_vkGetDeviceGroupSurfacePresentModes2EXT)load(context, "vkGetDeviceGroupSurfacePresentModes2EXT_volkImpl");
#endif /* (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1)) */
#if (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control))
	table->vkGetImageSubresourceLayout2EXT_volkImpl = (PFN_vkGetImageSubresourceLayout2EXT)load(context, "vkGetImageSubresourceLayout2EXT_volkImpl");
#endif /* (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control)) */
#if (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state))
	table->vkCmdSetVertexInputEXT_volkImpl = (PFN_vkCmdSetVertexInputEXT)load(context, "vkCmdSetVertexInputEXT_volkImpl");
#endif /* (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state)) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template))
	table->vkCmdPushDescriptorSetWithTemplateKHR_volkImpl = (PFN_vkCmdPushDescriptorSetWithTemplateKHR)load(context, "vkCmdPushDescriptorSetWithTemplateKHR_volkImpl");
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	table->vkGetDeviceGroupPresentCapabilitiesKHR_volkImpl = (PFN_vkGetDeviceGroupPresentCapabilitiesKHR)load(context, "vkGetDeviceGroupPresentCapabilitiesKHR_volkImpl");
	table->vkGetDeviceGroupSurfacePresentModesKHR_volkImpl = (PFN_vkGetDeviceGroupSurfacePresentModesKHR)load(context, "vkGetDeviceGroupSurfacePresentModesKHR_volkImpl");
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	table->vkAcquireNextImage2KHR_volkImpl = (PFN_vkAcquireNextImage2KHR)load(context, "vkAcquireNextImage2KHR_volkImpl");
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