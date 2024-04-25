#include <volk.h>

VkResult ACTIAS_ABI vkCreateInstance(VkInstanceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    return vkCreateInstance_volkImpl(pCreateInfo, pAllocator, pInstance);
}

void ACTIAS_ABI vkDestroyInstance(VkInstance instance, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyInstance_volkImpl(instance, pAllocator);
}

VkResult ACTIAS_ABI vkEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices)
{
    return vkEnumeratePhysicalDevices_volkImpl(instance, pPhysicalDeviceCount, pPhysicalDevices);
}

PFN_vkVoidFunction ACTIAS_ABI vkGetDeviceProcAddr(VkDevice device, char* pName)
{
    return vkGetDeviceProcAddr_volkImpl(device, pName);
}

PFN_vkVoidFunction ACTIAS_ABI vkGetInstanceProcAddr(VkInstance instance, char* pName)
{
    return vkGetInstanceProcAddr_volkImpl(instance, pName);
}

void ACTIAS_ABI vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties)
{
    return vkGetPhysicalDeviceProperties_volkImpl(physicalDevice, pProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties)
{
    return vkGetPhysicalDeviceQueueFamilyProperties_volkImpl(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties)
{
    return vkGetPhysicalDeviceMemoryProperties_volkImpl(physicalDevice, pMemoryProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures)
{
    return vkGetPhysicalDeviceFeatures_volkImpl(physicalDevice, pFeatures);
}

void ACTIAS_ABI vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties)
{
    return vkGetPhysicalDeviceFormatProperties_volkImpl(physicalDevice, format, pFormatProperties);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties)
{
    return vkGetPhysicalDeviceImageFormatProperties_volkImpl(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}

VkResult ACTIAS_ABI vkCreateDevice(VkPhysicalDevice physicalDevice, VkDeviceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    return vkCreateDevice_volkImpl(physicalDevice, pCreateInfo, pAllocator, pDevice);
}

VkResult ACTIAS_ABI vkCreateDevice(VkPhysicalDevice physicalDevice, VkDeviceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    return vkCreateDevice_volkImpl(physicalDevice, pCreateInfo, pAllocator, pDevice);
}

void ACTIAS_ABI vkDestroyDevice(VkDevice device, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyDevice_volkImpl(device, pAllocator);
}

VkResult ACTIAS_ABI vkEnumerateInstanceVersion(uint32_t* pApiVersion)
{
    return vkEnumerateInstanceVersion_volkImpl(pApiVersion);
}

VkResult ACTIAS_ABI vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    return vkEnumerateInstanceLayerProperties_volkImpl(pPropertyCount, pProperties);
}

VkResult ACTIAS_ABI vkEnumerateInstanceExtensionProperties(char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    return vkEnumerateInstanceExtensionProperties_volkImpl(pLayerName, pPropertyCount, pProperties);
}

VkResult ACTIAS_ABI vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    return vkEnumerateDeviceLayerProperties_volkImpl(physicalDevice, pPropertyCount, pProperties);
}

VkResult ACTIAS_ABI vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    return vkEnumerateDeviceLayerProperties_volkImpl(physicalDevice, pPropertyCount, pProperties);
}

VkResult ACTIAS_ABI vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    return vkEnumerateDeviceExtensionProperties_volkImpl(physicalDevice, pLayerName, pPropertyCount, pProperties);
}

void ACTIAS_ABI vkGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
    return vkGetDeviceQueue_volkImpl(device, queueFamilyIndex, queueIndex, pQueue);
}

VkResult ACTIAS_ABI vkQueueSubmit(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence)
{
    return vkQueueSubmit_volkImpl(queue, submitCount, pSubmits, fence);
}

VkResult ACTIAS_ABI vkQueueWaitIdle(VkQueue queue)
{
    return vkQueueWaitIdle_volkImpl(queue);
}

VkResult ACTIAS_ABI vkDeviceWaitIdle(VkDevice device)
{
    return vkDeviceWaitIdle_volkImpl(device);
}

VkResult ACTIAS_ABI vkAllocateMemory(VkDevice device, VkMemoryAllocateInfo* pAllocateInfo, VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
    return vkAllocateMemory_volkImpl(device, pAllocateInfo, pAllocator, pMemory);
}

void ACTIAS_ABI vkFreeMemory(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* pAllocator)
{
    return vkFreeMemory_volkImpl(device, memory, pAllocator);
}

VkResult ACTIAS_ABI vkMapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void* ppData)
{
    return vkMapMemory_volkImpl(device, memory, offset, size, flags, ppData);
}

void ACTIAS_ABI vkUnmapMemory(VkDevice device, VkDeviceMemory memory)
{
    return vkUnmapMemory_volkImpl(device, memory);
}

VkResult ACTIAS_ABI vkFlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, VkMappedMemoryRange* pMemoryRanges)
{
    return vkFlushMappedMemoryRanges_volkImpl(device, memoryRangeCount, pMemoryRanges);
}

VkResult ACTIAS_ABI vkInvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, VkMappedMemoryRange* pMemoryRanges)
{
    return vkInvalidateMappedMemoryRanges_volkImpl(device, memoryRangeCount, pMemoryRanges);
}

void ACTIAS_ABI vkGetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes)
{
    return vkGetDeviceMemoryCommitment_volkImpl(device, memory, pCommittedMemoryInBytes);
}

void ACTIAS_ABI vkGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
{
    return vkGetBufferMemoryRequirements_volkImpl(device, buffer, pMemoryRequirements);
}

VkResult ACTIAS_ABI vkBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    return vkBindBufferMemory_volkImpl(device, buffer, memory, memoryOffset);
}

void ACTIAS_ABI vkGetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements)
{
    return vkGetImageMemoryRequirements_volkImpl(device, image, pMemoryRequirements);
}

VkResult ACTIAS_ABI vkBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    return vkBindImageMemory_volkImpl(device, image, memory, memoryOffset);
}

void ACTIAS_ABI vkGetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements)
{
    return vkGetImageSparseMemoryRequirements_volkImpl(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

void ACTIAS_ABI vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties)
{
    return vkGetPhysicalDeviceSparseImageFormatProperties_volkImpl(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
}

VkResult ACTIAS_ABI vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, VkBindSparseInfo* pBindInfo, VkFence fence)
{
    return vkQueueBindSparse_volkImpl(queue, bindInfoCount, pBindInfo, fence);
}

VkResult ACTIAS_ABI vkCreateFence(VkDevice device, VkFenceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    return vkCreateFence_volkImpl(device, pCreateInfo, pAllocator, pFence);
}

void ACTIAS_ABI vkDestroyFence(VkDevice device, VkFence fence, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyFence_volkImpl(device, fence, pAllocator);
}

VkResult ACTIAS_ABI vkResetFences(VkDevice device, uint32_t fenceCount, VkFence* pFences)
{
    return vkResetFences_volkImpl(device, fenceCount, pFences);
}

VkResult ACTIAS_ABI vkGetFenceStatus(VkDevice device, VkFence fence)
{
    return vkGetFenceStatus_volkImpl(device, fence);
}

VkResult ACTIAS_ABI vkWaitForFences(VkDevice device, uint32_t fenceCount, VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
{
    return vkWaitForFences_volkImpl(device, fenceCount, pFences, waitAll, timeout);
}

VkResult ACTIAS_ABI vkCreateSemaphore(VkDevice device, VkSemaphoreCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
{
    return vkCreateSemaphore_volkImpl(device, pCreateInfo, pAllocator, pSemaphore);
}

void ACTIAS_ABI vkDestroySemaphore(VkDevice device, VkSemaphore semaphore, VkAllocationCallbacks* pAllocator)
{
    return vkDestroySemaphore_volkImpl(device, semaphore, pAllocator);
}

VkResult ACTIAS_ABI vkCreateEvent(VkDevice device, VkEventCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    return vkCreateEvent_volkImpl(device, pCreateInfo, pAllocator, pEvent);
}

void ACTIAS_ABI vkDestroyEvent(VkDevice device, VkEvent event, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyEvent_volkImpl(device, event, pAllocator);
}

VkResult ACTIAS_ABI vkGetEventStatus(VkDevice device, VkEvent event)
{
    return vkGetEventStatus_volkImpl(device, event);
}

VkResult ACTIAS_ABI vkSetEvent(VkDevice device, VkEvent event)
{
    return vkSetEvent_volkImpl(device, event);
}

VkResult ACTIAS_ABI vkResetEvent(VkDevice device, VkEvent event)
{
    return vkResetEvent_volkImpl(device, event);
}

VkResult ACTIAS_ABI vkCreateQueryPool(VkDevice device, VkQueryPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool)
{
    return vkCreateQueryPool_volkImpl(device, pCreateInfo, pAllocator, pQueryPool);
}

void ACTIAS_ABI vkDestroyQueryPool(VkDevice device, VkQueryPool queryPool, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyQueryPool_volkImpl(device, queryPool, pAllocator);
}

VkResult ACTIAS_ABI vkGetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
{
    return vkGetQueryPoolResults_volkImpl(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}

void ACTIAS_ABI vkResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    return vkResetQueryPool_volkImpl(device, queryPool, firstQuery, queryCount);
}

VkResult ACTIAS_ABI vkCreateBuffer(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
    return vkCreateBuffer_volkImpl(device, pCreateInfo, pAllocator, pBuffer);
}

void ACTIAS_ABI vkDestroyBuffer(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyBuffer_volkImpl(device, buffer, pAllocator);
}

VkResult ACTIAS_ABI vkCreateBufferView(VkDevice device, VkBufferViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBufferView* pView)
{
    return vkCreateBufferView_volkImpl(device, pCreateInfo, pAllocator, pView);
}

void ACTIAS_ABI vkDestroyBufferView(VkDevice device, VkBufferView bufferView, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyBufferView_volkImpl(device, bufferView, pAllocator);
}

VkResult ACTIAS_ABI vkCreateImage(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    return vkCreateImage_volkImpl(device, pCreateInfo, pAllocator, pImage);
}

void ACTIAS_ABI vkDestroyImage(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyImage_volkImpl(device, image, pAllocator);
}

void ACTIAS_ABI vkGetImageSubresourceLayout(VkDevice device, VkImage image, VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
{
    return vkGetImageSubresourceLayout_volkImpl(device, image, pSubresource, pLayout);
}

VkResult ACTIAS_ABI vkCreateImageView(VkDevice device, VkImageViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImageView* pView)
{
    return vkCreateImageView_volkImpl(device, pCreateInfo, pAllocator, pView);
}

void ACTIAS_ABI vkDestroyImageView(VkDevice device, VkImageView imageView, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyImageView_volkImpl(device, imageView, pAllocator);
}

VkResult ACTIAS_ABI vkCreateShaderModule(VkDevice device, VkShaderModuleCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
{
    return vkCreateShaderModule_volkImpl(device, pCreateInfo, pAllocator, pShaderModule);
}

void ACTIAS_ABI vkDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyShaderModule_volkImpl(device, shaderModule, pAllocator);
}

VkResult ACTIAS_ABI vkCreatePipelineCache(VkDevice device, VkPipelineCacheCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
{
    return vkCreatePipelineCache_volkImpl(device, pCreateInfo, pAllocator, pPipelineCache);
}

VkResult ACTIAS_ABI vkCreatePipelineCache(VkDevice device, VkPipelineCacheCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
{
    return vkCreatePipelineCache_volkImpl(device, pCreateInfo, pAllocator, pPipelineCache);
}

void ACTIAS_ABI vkDestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyPipelineCache_volkImpl(device, pipelineCache, pAllocator);
}

VkResult ACTIAS_ABI vkGetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData)
{
    return vkGetPipelineCacheData_volkImpl(device, pipelineCache, pDataSize, pData);
}

VkResult ACTIAS_ABI vkMergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, VkPipelineCache* pSrcCaches)
{
    return vkMergePipelineCaches_volkImpl(device, dstCache, srcCacheCount, pSrcCaches);
}

VkResult ACTIAS_ABI vkCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkGraphicsPipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    return vkCreateGraphicsPipelines_volkImpl(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VkResult ACTIAS_ABI vkCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkGraphicsPipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    return vkCreateGraphicsPipelines_volkImpl(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VkResult ACTIAS_ABI vkCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkComputePipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    return vkCreateComputePipelines_volkImpl(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VkResult ACTIAS_ABI vkCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkComputePipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    return vkCreateComputePipelines_volkImpl(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VkResult ACTIAS_ABI vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize)
{
    return vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI_volkImpl(device, renderpass, pMaxWorkgroupSize);
}

void ACTIAS_ABI vkDestroyPipeline(VkDevice device, VkPipeline pipeline, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyPipeline_volkImpl(device, pipeline, pAllocator);
}

VkResult ACTIAS_ABI vkCreatePipelineLayout(VkDevice device, VkPipelineLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
{
    return vkCreatePipelineLayout_volkImpl(device, pCreateInfo, pAllocator, pPipelineLayout);
}

void ACTIAS_ABI vkDestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyPipelineLayout_volkImpl(device, pipelineLayout, pAllocator);
}

VkResult ACTIAS_ABI vkCreateSampler(VkDevice device, VkSamplerCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
{
    return vkCreateSampler_volkImpl(device, pCreateInfo, pAllocator, pSampler);
}

void ACTIAS_ABI vkDestroySampler(VkDevice device, VkSampler sampler, VkAllocationCallbacks* pAllocator)
{
    return vkDestroySampler_volkImpl(device, sampler, pAllocator);
}

VkResult ACTIAS_ABI vkCreateDescriptorSetLayout(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
{
    return vkCreateDescriptorSetLayout_volkImpl(device, pCreateInfo, pAllocator, pSetLayout);
}

void ACTIAS_ABI vkDestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyDescriptorSetLayout_volkImpl(device, descriptorSetLayout, pAllocator);
}

VkResult ACTIAS_ABI vkCreateDescriptorPool(VkDevice device, VkDescriptorPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
{
    return vkCreateDescriptorPool_volkImpl(device, pCreateInfo, pAllocator, pDescriptorPool);
}

void ACTIAS_ABI vkDestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyDescriptorPool_volkImpl(device, descriptorPool, pAllocator);
}

VkResult ACTIAS_ABI vkResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
{
    return vkResetDescriptorPool_volkImpl(device, descriptorPool, flags);
}

VkResult ACTIAS_ABI vkAllocateDescriptorSets(VkDevice device, VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
{
    return vkAllocateDescriptorSets_volkImpl(device, pAllocateInfo, pDescriptorSets);
}

VkResult ACTIAS_ABI vkFreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets)
{
    return vkFreeDescriptorSets_volkImpl(device, descriptorPool, descriptorSetCount, pDescriptorSets);
}

void ACTIAS_ABI vkUpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, VkCopyDescriptorSet* pDescriptorCopies)
{
    return vkUpdateDescriptorSets_volkImpl(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}

VkResult ACTIAS_ABI vkCreateFramebuffer(VkDevice device, VkFramebufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
{
    return vkCreateFramebuffer_volkImpl(device, pCreateInfo, pAllocator, pFramebuffer);
}

void ACTIAS_ABI vkDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyFramebuffer_volkImpl(device, framebuffer, pAllocator);
}

VkResult ACTIAS_ABI vkCreateRenderPass(VkDevice device, VkRenderPassCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    return vkCreateRenderPass_volkImpl(device, pCreateInfo, pAllocator, pRenderPass);
}

void ACTIAS_ABI vkDestroyRenderPass(VkDevice device, VkRenderPass renderPass, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyRenderPass_volkImpl(device, renderPass, pAllocator);
}

void ACTIAS_ABI vkGetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity)
{
    return vkGetRenderAreaGranularity_volkImpl(device, renderPass, pGranularity);
}

void ACTIAS_ABI vkGetRenderingAreaGranularityKHR(VkDevice device, VkRenderingAreaInfoKHR* pRenderingAreaInfo, VkExtent2D* pGranularity)
{
    return vkGetRenderingAreaGranularityKHR_volkImpl(device, pRenderingAreaInfo, pGranularity);
}

VkResult ACTIAS_ABI vkCreateCommandPool(VkDevice device, VkCommandPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
    return vkCreateCommandPool_volkImpl(device, pCreateInfo, pAllocator, pCommandPool);
}

void ACTIAS_ABI vkDestroyCommandPool(VkDevice device, VkCommandPool commandPool, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyCommandPool_volkImpl(device, commandPool, pAllocator);
}

VkResult ACTIAS_ABI vkResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags)
{
    return vkResetCommandPool_volkImpl(device, commandPool, flags);
}

VkResult ACTIAS_ABI vkAllocateCommandBuffers(VkDevice device, VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
    return vkAllocateCommandBuffers_volkImpl(device, pAllocateInfo, pCommandBuffers);
}

void ACTIAS_ABI vkFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers)
{
    return vkFreeCommandBuffers_volkImpl(device, commandPool, commandBufferCount, pCommandBuffers);
}

VkResult ACTIAS_ABI vkBeginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo* pBeginInfo)
{
    return vkBeginCommandBuffer_volkImpl(commandBuffer, pBeginInfo);
}

VkResult ACTIAS_ABI vkEndCommandBuffer(VkCommandBuffer commandBuffer)
{
    return vkEndCommandBuffer_volkImpl(commandBuffer);
}

VkResult ACTIAS_ABI vkResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags)
{
    return vkResetCommandBuffer_volkImpl(commandBuffer, flags);
}

void ACTIAS_ABI vkCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
    return vkCmdBindPipeline_volkImpl(commandBuffer, pipelineBindPoint, pipeline);
}

void ACTIAS_ABI vkCmdSetAttachmentFeedbackLoopEnableEXT(VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask)
{
    return vkCmdSetAttachmentFeedbackLoopEnableEXT_volkImpl(commandBuffer, aspectMask);
}

void ACTIAS_ABI vkCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewport* pViewports)
{
    return vkCmdSetViewport_volkImpl(commandBuffer, firstViewport, viewportCount, pViewports);
}

void ACTIAS_ABI vkCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, VkRect2D* pScissors)
{
    return vkCmdSetScissor_volkImpl(commandBuffer, firstScissor, scissorCount, pScissors);
}

void ACTIAS_ABI vkCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth)
{
    return vkCmdSetLineWidth_volkImpl(commandBuffer, lineWidth);
}

void ACTIAS_ABI vkCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
    return vkCmdSetDepthBias_volkImpl(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

void ACTIAS_ABI vkCmdSetBlendConstants(VkCommandBuffer commandBuffer, float blendConstants)
{
    return vkCmdSetBlendConstants_volkImpl(commandBuffer, blendConstants);
}

void ACTIAS_ABI vkCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
    return vkCmdSetDepthBounds_volkImpl(commandBuffer, minDepthBounds, maxDepthBounds);
}

void ACTIAS_ABI vkCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
{
    return vkCmdSetStencilCompareMask_volkImpl(commandBuffer, faceMask, compareMask);
}

void ACTIAS_ABI vkCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
{
    return vkCmdSetStencilWriteMask_volkImpl(commandBuffer, faceMask, writeMask);
}

void ACTIAS_ABI vkCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
{
    return vkCmdSetStencilReference_volkImpl(commandBuffer, faceMask, reference);
}

void ACTIAS_ABI vkCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, uint32_t* pDynamicOffsets)
{
    return vkCmdBindDescriptorSets_volkImpl(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

void ACTIAS_ABI vkCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
    return vkCmdBindIndexBuffer_volkImpl(commandBuffer, buffer, offset, indexType);
}

void ACTIAS_ABI vkCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets)
{
    return vkCmdBindVertexBuffers_volkImpl(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

void ACTIAS_ABI vkCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    return vkCmdDraw_volkImpl(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void ACTIAS_ABI vkCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
    return vkCmdDrawIndexed_volkImpl(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void ACTIAS_ABI vkCmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride)
{
    return vkCmdDrawMultiEXT_volkImpl(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}

void ACTIAS_ABI vkCmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, int32_t* pVertexOffset)
{
    return vkCmdDrawMultiIndexedEXT_volkImpl(commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
}

void ACTIAS_ABI vkCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    return vkCmdDrawIndirect_volkImpl(commandBuffer, buffer, offset, drawCount, stride);
}

void ACTIAS_ABI vkCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    return vkCmdDrawIndexedIndirect_volkImpl(commandBuffer, buffer, offset, drawCount, stride);
}

void ACTIAS_ABI vkCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    return vkCmdDispatch_volkImpl(commandBuffer, groupCountX, groupCountY, groupCountZ);
}

void ACTIAS_ABI vkCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
    return vkCmdDispatchIndirect_volkImpl(commandBuffer, buffer, offset);
}

void ACTIAS_ABI vkCmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer)
{
    return vkCmdSubpassShadingHUAWEI_volkImpl(commandBuffer);
}

void ACTIAS_ABI vkCmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    return vkCmdDrawClusterHUAWEI_volkImpl(commandBuffer, groupCountX, groupCountY, groupCountZ);
}

void ACTIAS_ABI vkCmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
    return vkCmdDrawClusterIndirectHUAWEI_volkImpl(commandBuffer, buffer, offset);
}

void ACTIAS_ABI vkCmdUpdatePipelineIndirectBufferNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
    return vkCmdUpdatePipelineIndirectBufferNV_volkImpl(commandBuffer, pipelineBindPoint, pipeline);
}

void ACTIAS_ABI vkCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions)
{
    return vkCmdCopyBuffer_volkImpl(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

void ACTIAS_ABI vkCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions)
{
    return vkCmdCopyImage_volkImpl(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void ACTIAS_ABI vkCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageBlit* pRegions, VkFilter filter)
{
    return vkCmdBlitImage_volkImpl(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}

void ACTIAS_ABI vkCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions)
{
    return vkCmdCopyBufferToImage_volkImpl(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

void ACTIAS_ABI vkCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions)
{
    return vkCmdCopyImageToBuffer_volkImpl(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

void ACTIAS_ABI vkCmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride)
{
    return vkCmdCopyMemoryIndirectNV_volkImpl(commandBuffer, copyBufferAddress, copyCount, stride);
}

void ACTIAS_ABI vkCmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, VkImageSubresourceLayers* pImageSubresources)
{
    return vkCmdCopyMemoryToImageIndirectNV_volkImpl(commandBuffer, copyBufferAddress, copyCount, stride, dstImage, dstImageLayout, pImageSubresources);
}

void ACTIAS_ABI vkCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, void* pData)
{
    return vkCmdUpdateBuffer_volkImpl(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}

void ACTIAS_ABI vkCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
{
    return vkCmdFillBuffer_volkImpl(commandBuffer, dstBuffer, dstOffset, size, data);
}

void ACTIAS_ABI vkCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearColorValue* pColor, uint32_t rangeCount, VkImageSubresourceRange* pRanges)
{
    return vkCmdClearColorImage_volkImpl(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}

void ACTIAS_ABI vkCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, VkImageSubresourceRange* pRanges)
{
    return vkCmdClearDepthStencilImage_volkImpl(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}

void ACTIAS_ABI vkCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, VkClearAttachment* pAttachments, uint32_t rectCount, VkClearRect* pRects)
{
    return vkCmdClearAttachments_volkImpl(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}

void ACTIAS_ABI vkCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageResolve* pRegions)
{
    return vkCmdResolveImage_volkImpl(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void ACTIAS_ABI vkCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    return vkCmdSetEvent_volkImpl(commandBuffer, event, stageMask);
}

void ACTIAS_ABI vkCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    return vkCmdResetEvent_volkImpl(commandBuffer, event, stageMask);
}

void ACTIAS_ABI vkCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers)
{
    return vkCmdWaitEvents_volkImpl(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void ACTIAS_ABI vkCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers)
{
    return vkCmdPipelineBarrier_volkImpl(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void ACTIAS_ABI vkCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
{
    return vkCmdBeginQuery_volkImpl(commandBuffer, queryPool, query, flags);
}

void ACTIAS_ABI vkCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
{
    return vkCmdEndQuery_volkImpl(commandBuffer, queryPool, query);
}

void ACTIAS_ABI vkCmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin)
{
    return vkCmdBeginConditionalRenderingEXT_volkImpl(commandBuffer, pConditionalRenderingBegin);
}

void ACTIAS_ABI vkCmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer)
{
    return vkCmdEndConditionalRenderingEXT_volkImpl(commandBuffer);
}

void ACTIAS_ABI vkCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    return vkCmdResetQueryPool_volkImpl(commandBuffer, queryPool, firstQuery, queryCount);
}

void ACTIAS_ABI vkCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
{
    return vkCmdWriteTimestamp_volkImpl(commandBuffer, pipelineStage, queryPool, query);
}

void ACTIAS_ABI vkCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
{
    return vkCmdCopyQueryPoolResults_volkImpl(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}

void ACTIAS_ABI vkCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, void* pValues)
{
    return vkCmdPushConstants_volkImpl(commandBuffer, layout, stageFlags, offset, size, pValues);
}

void ACTIAS_ABI vkCmdBeginRenderPass(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents)
{
    return vkCmdBeginRenderPass_volkImpl(commandBuffer, pRenderPassBegin, contents);
}

void ACTIAS_ABI vkCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents)
{
    return vkCmdNextSubpass_volkImpl(commandBuffer, contents);
}

void ACTIAS_ABI vkCmdEndRenderPass(VkCommandBuffer commandBuffer)
{
    return vkCmdEndRenderPass_volkImpl(commandBuffer);
}

void ACTIAS_ABI vkCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers)
{
    return vkCmdExecuteCommands_volkImpl(commandBuffer, commandBufferCount, pCommandBuffers);
}

VkResult ACTIAS_ABI vkCreateAndroidSurfaceKHR(VkInstance instance, VkAndroidSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateAndroidSurfaceKHR_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties)
{
    return vkGetPhysicalDeviceDisplayPropertiesKHR_volkImpl(physicalDevice, pPropertyCount, pProperties);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties)
{
    return vkGetPhysicalDeviceDisplayPlanePropertiesKHR_volkImpl(physicalDevice, pPropertyCount, pProperties);
}

VkResult ACTIAS_ABI vkGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays)
{
    return vkGetDisplayPlaneSupportedDisplaysKHR_volkImpl(physicalDevice, planeIndex, pDisplayCount, pDisplays);
}

VkResult ACTIAS_ABI vkGetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties)
{
    return vkGetDisplayModePropertiesKHR_volkImpl(physicalDevice, display, pPropertyCount, pProperties);
}

VkResult ACTIAS_ABI vkCreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, VkDisplayModeCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode)
{
    return vkCreateDisplayModeKHR_volkImpl(physicalDevice, display, pCreateInfo, pAllocator, pMode);
}

VkResult ACTIAS_ABI vkGetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities)
{
    return vkGetDisplayPlaneCapabilitiesKHR_volkImpl(physicalDevice, mode, planeIndex, pCapabilities);
}

VkResult ACTIAS_ABI vkCreateDisplayPlaneSurfaceKHR(VkInstance instance, VkDisplaySurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateDisplayPlaneSurfaceKHR_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

VkResult ACTIAS_ABI vkCreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, VkSwapchainCreateInfoKHR* pCreateInfos, VkSwapchainCreateInfoKHR* pCreateInfos, VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains)
{
    return vkCreateSharedSwapchainsKHR_volkImpl(device, swapchainCount, pCreateInfos, pCreateInfos, pAllocator, pSwapchains);
}

void ACTIAS_ABI vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, VkAllocationCallbacks* pAllocator)
{
    return vkDestroySurfaceKHR_volkImpl(instance, surface, pAllocator);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
    return vkGetPhysicalDeviceSurfaceSupportKHR_volkImpl(physicalDevice, queueFamilyIndex, surface, pSupported);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
    return vkGetPhysicalDeviceSurfaceCapabilitiesKHR_volkImpl(physicalDevice, surface, pSurfaceCapabilities);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats)
{
    return vkGetPhysicalDeviceSurfaceFormatsKHR_volkImpl(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
    return vkGetPhysicalDeviceSurfacePresentModesKHR_volkImpl(physicalDevice, surface, pPresentModeCount, pPresentModes);
}

VkResult ACTIAS_ABI vkCreateSwapchainKHR(VkDevice device, VkSwapchainCreateInfoKHR* pCreateInfo, VkSwapchainCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
{
    return vkCreateSwapchainKHR_volkImpl(device, pCreateInfo, pCreateInfo, pAllocator, pSwapchain);
}

void ACTIAS_ABI vkDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, VkAllocationCallbacks* pAllocator)
{
    return vkDestroySwapchainKHR_volkImpl(device, swapchain, pAllocator);
}

VkResult ACTIAS_ABI vkGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
{
    return vkGetSwapchainImagesKHR_volkImpl(device, swapchain, pSwapchainImageCount, pSwapchainImages);
}

VkResult ACTIAS_ABI vkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
    return vkAcquireNextImageKHR_volkImpl(device, swapchain, timeout, semaphore, fence, pImageIndex);
}

VkResult ACTIAS_ABI vkQueuePresentKHR(VkQueue queue, VkPresentInfoKHR* pPresentInfo)
{
    return vkQueuePresentKHR_volkImpl(queue, pPresentInfo);
}

VkResult ACTIAS_ABI vkCreateViSurfaceNN(VkInstance instance, VkViSurfaceCreateInfoNN* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateViSurfaceNN_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

VkResult ACTIAS_ABI vkCreateWaylandSurfaceKHR(VkInstance instance, VkWaylandSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateWaylandSurfaceKHR_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

VkBool32 ACTIAS_ABI vkGetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, wl_display* display)
{
    return vkGetPhysicalDeviceWaylandPresentationSupportKHR_volkImpl(physicalDevice, queueFamilyIndex, display);
}

VkResult ACTIAS_ABI vkCreateWin32SurfaceKHR(VkInstance instance, VkWin32SurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateWin32SurfaceKHR_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

VkBool32 ACTIAS_ABI vkGetPhysicalDeviceWin32PresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex)
{
    return vkGetPhysicalDeviceWin32PresentationSupportKHR_volkImpl(physicalDevice, queueFamilyIndex);
}

VkResult ACTIAS_ABI vkCreateXlibSurfaceKHR(VkInstance instance, VkXlibSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateXlibSurfaceKHR_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

VkBool32 ACTIAS_ABI vkGetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID)
{
    return vkGetPhysicalDeviceXlibPresentationSupportKHR_volkImpl(physicalDevice, queueFamilyIndex, dpy, visualID);
}

VkResult ACTIAS_ABI vkCreateXcbSurfaceKHR(VkInstance instance, VkXcbSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateXcbSurfaceKHR_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

VkBool32 ACTIAS_ABI vkGetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id)
{
    return vkGetPhysicalDeviceXcbPresentationSupportKHR_volkImpl(physicalDevice, queueFamilyIndex, connection, visual_id);
}

VkResult ACTIAS_ABI vkCreateDirectFBSurfaceEXT(VkInstance instance, VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateDirectFBSurfaceEXT_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

VkBool32 ACTIAS_ABI vkGetPhysicalDeviceDirectFBPresentationSupportEXT(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb)
{
    return vkGetPhysicalDeviceDirectFBPresentationSupportEXT_volkImpl(physicalDevice, queueFamilyIndex, dfb);
}

VkResult ACTIAS_ABI vkCreateImagePipeSurfaceFUCHSIA(VkInstance instance, VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateImagePipeSurfaceFUCHSIA_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

VkResult ACTIAS_ABI vkCreateStreamDescriptorSurfaceGGP(VkInstance instance, VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateStreamDescriptorSurfaceGGP_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

VkResult ACTIAS_ABI vkCreateScreenSurfaceQNX(VkInstance instance, VkScreenSurfaceCreateInfoQNX* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateScreenSurfaceQNX_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

VkBool32 ACTIAS_ABI vkGetPhysicalDeviceScreenPresentationSupportQNX(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, _screen_window* window)
{
    return vkGetPhysicalDeviceScreenPresentationSupportQNX_volkImpl(physicalDevice, queueFamilyIndex, window);
}

VkResult ACTIAS_ABI vkCreateDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
    return vkCreateDebugReportCallbackEXT_volkImpl(instance, pCreateInfo, pAllocator, pCallback);
}

void ACTIAS_ABI vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyDebugReportCallbackEXT_volkImpl(instance, callback, pAllocator);
}

void ACTIAS_ABI vkDebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, char* pLayerPrefix, char* pMessage)
{
    return vkDebugReportMessageEXT_volkImpl(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}

VkResult ACTIAS_ABI vkDebugMarkerSetObjectNameEXT(VkDevice device, VkDebugMarkerObjectNameInfoEXT* pNameInfo)
{
    return vkDebugMarkerSetObjectNameEXT_volkImpl(device, pNameInfo);
}

VkResult ACTIAS_ABI vkDebugMarkerSetObjectTagEXT(VkDevice device, VkDebugMarkerObjectTagInfoEXT* pTagInfo)
{
    return vkDebugMarkerSetObjectTagEXT_volkImpl(device, pTagInfo);
}

void ACTIAS_ABI vkCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
    return vkCmdDebugMarkerBeginEXT_volkImpl(commandBuffer, pMarkerInfo);
}

void ACTIAS_ABI vkCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer)
{
    return vkCmdDebugMarkerEndEXT_volkImpl(commandBuffer);
}

void ACTIAS_ABI vkCmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
    return vkCmdDebugMarkerInsertEXT_volkImpl(commandBuffer, pMarkerInfo);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties)
{
    return vkGetPhysicalDeviceExternalImageFormatPropertiesNV_volkImpl(physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
}

VkResult ACTIAS_ABI vkGetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle)
{
    return vkGetMemoryWin32HandleNV_volkImpl(device, memory, handleType, pHandle);
}

void ACTIAS_ABI vkCmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo)
{
    return vkCmdExecuteGeneratedCommandsNV_volkImpl(commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
}

void ACTIAS_ABI vkCmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo)
{
    return vkCmdPreprocessGeneratedCommandsNV_volkImpl(commandBuffer, pGeneratedCommandsInfo);
}

void ACTIAS_ABI vkCmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex)
{
    return vkCmdBindPipelineShaderGroupNV_volkImpl(commandBuffer, pipelineBindPoint, pipeline, groupIndex);
}

void ACTIAS_ABI vkGetGeneratedCommandsMemoryRequirementsNV(VkDevice device, VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return vkGetGeneratedCommandsMemoryRequirementsNV_volkImpl(device, pInfo, pMemoryRequirements);
}

VkResult ACTIAS_ABI vkCreateIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout)
{
    return vkCreateIndirectCommandsLayoutNV_volkImpl(device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
}

void ACTIAS_ABI vkDestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyIndirectCommandsLayoutNV_volkImpl(device, indirectCommandsLayout, pAllocator);
}

void ACTIAS_ABI vkGetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
    return vkGetPhysicalDeviceFeatures2_volkImpl(physicalDevice, pFeatures);
}

void ACTIAS_ABI vkGetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
    return vkGetPhysicalDeviceProperties2_volkImpl(physicalDevice, pProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
    return vkGetPhysicalDeviceFormatProperties2_volkImpl(physicalDevice, format, pFormatProperties);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
    return vkGetPhysicalDeviceImageFormatProperties2_volkImpl(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties)
{
    return vkGetPhysicalDeviceQueueFamilyProperties2_volkImpl(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
    return vkGetPhysicalDeviceMemoryProperties2_volkImpl(physicalDevice, pMemoryProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties)
{
    return vkGetPhysicalDeviceSparseImageFormatProperties2_volkImpl(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
}

void ACTIAS_ABI vkCmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, VkWriteDescriptorSet* pDescriptorWrites)
{
    return vkCmdPushDescriptorSetKHR_volkImpl(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}

void ACTIAS_ABI vkTrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
    return vkTrimCommandPool_volkImpl(device, commandPool, flags);
}

void ACTIAS_ABI vkGetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
    return vkGetPhysicalDeviceExternalBufferProperties_volkImpl(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}

VkResult ACTIAS_ABI vkGetMemoryWin32HandleKHR(VkDevice device, VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    return vkGetMemoryWin32HandleKHR_volkImpl(device, pGetWin32HandleInfo, pHandle);
}

VkResult ACTIAS_ABI vkGetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties)
{
    return vkGetMemoryWin32HandlePropertiesKHR_volkImpl(device, handleType, handle, pMemoryWin32HandleProperties);
}

VkResult ACTIAS_ABI vkGetMemoryFdKHR(VkDevice device, VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    return vkGetMemoryFdKHR_volkImpl(device, pGetFdInfo, pFd);
}

VkResult ACTIAS_ABI vkGetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties)
{
    return vkGetMemoryFdPropertiesKHR_volkImpl(device, handleType, fd, pMemoryFdProperties);
}

VkResult ACTIAS_ABI vkGetMemoryZirconHandleFUCHSIA(VkDevice device, VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle)
{
    return vkGetMemoryZirconHandleFUCHSIA_volkImpl(device, pGetZirconHandleInfo, pZirconHandle);
}

VkResult ACTIAS_ABI vkGetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties)
{
    return vkGetMemoryZirconHandlePropertiesFUCHSIA_volkImpl(device, handleType, zirconHandle, pMemoryZirconHandleProperties);
}

VkResult ACTIAS_ABI vkGetMemoryRemoteAddressNV(VkDevice device, VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress)
{
    return vkGetMemoryRemoteAddressNV_volkImpl(device, pMemoryGetRemoteAddressInfo, pAddress);
}

VkResult ACTIAS_ABI vkGetMemorySciBufNV(VkDevice device, VkMemoryGetSciBufInfoNV* pGetSciBufInfo, NvSciBufObj* pHandle)
{
    return vkGetMemorySciBufNV_volkImpl(device, pGetSciBufInfo, pHandle);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV(VkPhysicalDevice physicalDevice, VkExternalMemoryHandleTypeFlagBits handleType, NvSciBufObj handle, VkMemorySciBufPropertiesNV* pMemorySciBufProperties)
{
    return vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV_volkImpl(physicalDevice, handleType, handle, pMemorySciBufProperties);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceSciBufAttributesNV(VkPhysicalDevice physicalDevice, NvSciBufAttrList pAttributes)
{
    return vkGetPhysicalDeviceSciBufAttributesNV_volkImpl(physicalDevice, pAttributes);
}

void ACTIAS_ABI vkGetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
    return vkGetPhysicalDeviceExternalSemaphoreProperties_volkImpl(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

VkResult ACTIAS_ABI vkGetSemaphoreWin32HandleKHR(VkDevice device, VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    return vkGetSemaphoreWin32HandleKHR_volkImpl(device, pGetWin32HandleInfo, pHandle);
}

VkResult ACTIAS_ABI vkImportSemaphoreWin32HandleKHR(VkDevice device, VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo)
{
    return vkImportSemaphoreWin32HandleKHR_volkImpl(device, pImportSemaphoreWin32HandleInfo);
}

VkResult ACTIAS_ABI vkGetSemaphoreFdKHR(VkDevice device, VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    return vkGetSemaphoreFdKHR_volkImpl(device, pGetFdInfo, pFd);
}

VkResult ACTIAS_ABI vkImportSemaphoreFdKHR(VkDevice device, VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo)
{
    return vkImportSemaphoreFdKHR_volkImpl(device, pImportSemaphoreFdInfo);
}

VkResult ACTIAS_ABI vkGetSemaphoreZirconHandleFUCHSIA(VkDevice device, VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle)
{
    return vkGetSemaphoreZirconHandleFUCHSIA_volkImpl(device, pGetZirconHandleInfo, pZirconHandle);
}

VkResult ACTIAS_ABI vkImportSemaphoreZirconHandleFUCHSIA(VkDevice device, VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo)
{
    return vkImportSemaphoreZirconHandleFUCHSIA_volkImpl(device, pImportSemaphoreZirconHandleInfo);
}

void ACTIAS_ABI vkGetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
    return vkGetPhysicalDeviceExternalFenceProperties_volkImpl(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}

VkResult ACTIAS_ABI vkGetFenceWin32HandleKHR(VkDevice device, VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    return vkGetFenceWin32HandleKHR_volkImpl(device, pGetWin32HandleInfo, pHandle);
}

VkResult ACTIAS_ABI vkImportFenceWin32HandleKHR(VkDevice device, VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo)
{
    return vkImportFenceWin32HandleKHR_volkImpl(device, pImportFenceWin32HandleInfo);
}

VkResult ACTIAS_ABI vkGetFenceFdKHR(VkDevice device, VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    return vkGetFenceFdKHR_volkImpl(device, pGetFdInfo, pFd);
}

VkResult ACTIAS_ABI vkImportFenceFdKHR(VkDevice device, VkImportFenceFdInfoKHR* pImportFenceFdInfo)
{
    return vkImportFenceFdKHR_volkImpl(device, pImportFenceFdInfo);
}

VkResult ACTIAS_ABI vkGetFenceSciSyncFenceNV(VkDevice device, VkFenceGetSciSyncInfoNV* pGetSciSyncHandleInfo, void* pHandle)
{
    return vkGetFenceSciSyncFenceNV_volkImpl(device, pGetSciSyncHandleInfo, pHandle);
}

VkResult ACTIAS_ABI vkGetFenceSciSyncObjNV(VkDevice device, VkFenceGetSciSyncInfoNV* pGetSciSyncHandleInfo, void* pHandle)
{
    return vkGetFenceSciSyncObjNV_volkImpl(device, pGetSciSyncHandleInfo, pHandle);
}

VkResult ACTIAS_ABI vkImportFenceSciSyncFenceNV(VkDevice device, VkImportFenceSciSyncInfoNV* pImportFenceSciSyncInfo)
{
    return vkImportFenceSciSyncFenceNV_volkImpl(device, pImportFenceSciSyncInfo);
}

VkResult ACTIAS_ABI vkImportFenceSciSyncObjNV(VkDevice device, VkImportFenceSciSyncInfoNV* pImportFenceSciSyncInfo)
{
    return vkImportFenceSciSyncObjNV_volkImpl(device, pImportFenceSciSyncInfo);
}

VkResult ACTIAS_ABI vkGetSemaphoreSciSyncObjNV(VkDevice device, VkSemaphoreGetSciSyncInfoNV* pGetSciSyncInfo, void* pHandle)
{
    return vkGetSemaphoreSciSyncObjNV_volkImpl(device, pGetSciSyncInfo, pHandle);
}

VkResult ACTIAS_ABI vkImportSemaphoreSciSyncObjNV(VkDevice device, VkImportSemaphoreSciSyncInfoNV* pImportSemaphoreSciSyncInfo)
{
    return vkImportSemaphoreSciSyncObjNV_volkImpl(device, pImportSemaphoreSciSyncInfo);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceSciSyncAttributesNV(VkPhysicalDevice physicalDevice, VkSciSyncAttributesInfoNV* pSciSyncAttributesInfo, NvSciSyncAttrList pAttributes)
{
    return vkGetPhysicalDeviceSciSyncAttributesNV_volkImpl(physicalDevice, pSciSyncAttributesInfo, pAttributes);
}

VkResult ACTIAS_ABI vkCreateSemaphoreSciSyncPoolNV(VkDevice device, VkSemaphoreSciSyncPoolCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSemaphoreSciSyncPoolNV* pSemaphorePool)
{
    return vkCreateSemaphoreSciSyncPoolNV_volkImpl(device, pCreateInfo, pAllocator, pSemaphorePool);
}

void ACTIAS_ABI vkDestroySemaphoreSciSyncPoolNV(VkDevice device, VkSemaphoreSciSyncPoolNV semaphorePool, VkAllocationCallbacks* pAllocator)
{
    return vkDestroySemaphoreSciSyncPoolNV_volkImpl(device, semaphorePool, pAllocator);
}

VkResult ACTIAS_ABI vkReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
    return vkReleaseDisplayEXT_volkImpl(physicalDevice, display);
}

VkResult ACTIAS_ABI vkAcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display)
{
    return vkAcquireXlibDisplayEXT_volkImpl(physicalDevice, dpy, display);
}

VkResult ACTIAS_ABI vkGetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay)
{
    return vkGetRandROutputDisplayEXT_volkImpl(physicalDevice, dpy, rrOutput, pDisplay);
}

VkResult ACTIAS_ABI vkAcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
    return vkAcquireWinrtDisplayNV_volkImpl(physicalDevice, display);
}

VkResult ACTIAS_ABI vkGetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay)
{
    return vkGetWinrtDisplayNV_volkImpl(physicalDevice, deviceRelativeId, pDisplay);
}

VkResult ACTIAS_ABI vkDisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, VkDisplayPowerInfoEXT* pDisplayPowerInfo)
{
    return vkDisplayPowerControlEXT_volkImpl(device, display, pDisplayPowerInfo);
}

VkResult ACTIAS_ABI vkRegisterDeviceEventEXT(VkDevice device, VkDeviceEventInfoEXT* pDeviceEventInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    return vkRegisterDeviceEventEXT_volkImpl(device, pDeviceEventInfo, pAllocator, pFence);
}

VkResult ACTIAS_ABI vkRegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, VkDisplayEventInfoEXT* pDisplayEventInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    return vkRegisterDisplayEventEXT_volkImpl(device, display, pDisplayEventInfo, pAllocator, pFence);
}

VkResult ACTIAS_ABI vkGetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue)
{
    return vkGetSwapchainCounterEXT_volkImpl(device, swapchain, counter, pCounterValue);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities)
{
    return vkGetPhysicalDeviceSurfaceCapabilities2EXT_volkImpl(physicalDevice, surface, pSurfaceCapabilities);
}

VkResult ACTIAS_ABI vkEnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties)
{
    return vkEnumeratePhysicalDeviceGroups_volkImpl(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}

void ACTIAS_ABI vkGetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
    return vkGetDeviceGroupPeerMemoryFeatures_volkImpl(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}

VkResult ACTIAS_ABI vkBindBufferMemory2(VkDevice device, uint32_t bindInfoCount, VkBindBufferMemoryInfo* pBindInfos)
{
    return vkBindBufferMemory2_volkImpl(device, bindInfoCount, pBindInfos);
}

VkResult ACTIAS_ABI vkBindImageMemory2(VkDevice device, uint32_t bindInfoCount, VkBindImageMemoryInfo* pBindInfos)
{
    return vkBindImageMemory2_volkImpl(device, bindInfoCount, pBindInfos);
}

void ACTIAS_ABI vkCmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask)
{
    return vkCmdSetDeviceMask_volkImpl(commandBuffer, deviceMask);
}

VkResult ACTIAS_ABI vkGetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities)
{
    return vkGetDeviceGroupPresentCapabilitiesKHR_volkImpl(device, pDeviceGroupPresentCapabilities);
}

VkResult ACTIAS_ABI vkGetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes)
{
    return vkGetDeviceGroupSurfacePresentModesKHR_volkImpl(device, surface, pModes);
}

VkResult ACTIAS_ABI vkAcquireNextImage2KHR(VkDevice device, VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex)
{
    return vkAcquireNextImage2KHR_volkImpl(device, pAcquireInfo, pImageIndex);
}

void ACTIAS_ABI vkCmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    return vkCmdDispatchBase_volkImpl(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

VkResult ACTIAS_ABI vkGetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects)
{
    return vkGetPhysicalDevicePresentRectanglesKHR_volkImpl(physicalDevice, surface, pRectCount, pRects);
}

VkResult ACTIAS_ABI vkCreateDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
    return vkCreateDescriptorUpdateTemplate_volkImpl(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}

void ACTIAS_ABI vkDestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyDescriptorUpdateTemplate_volkImpl(device, descriptorUpdateTemplate, pAllocator);
}

void ACTIAS_ABI vkUpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, void* pData)
{
    return vkUpdateDescriptorSetWithTemplate_volkImpl(device, descriptorSet, descriptorUpdateTemplate, pData);
}

void ACTIAS_ABI vkCmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, void* pData)
{
    return vkCmdPushDescriptorSetWithTemplateKHR_volkImpl(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}

void ACTIAS_ABI vkSetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, VkSwapchainKHR* pSwapchains, VkHdrMetadataEXT* pMetadata)
{
    return vkSetHdrMetadataEXT_volkImpl(device, swapchainCount, pSwapchains, pMetadata);
}

VkResult ACTIAS_ABI vkGetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain)
{
    return vkGetSwapchainStatusKHR_volkImpl(device, swapchain);
}

VkResult ACTIAS_ABI vkGetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties)
{
    return vkGetRefreshCycleDurationGOOGLE_volkImpl(device, swapchain, pDisplayTimingProperties);
}

VkResult ACTIAS_ABI vkGetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings)
{
    return vkGetPastPresentationTimingGOOGLE_volkImpl(device, swapchain, pPresentationTimingCount, pPresentationTimings);
}

VkResult ACTIAS_ABI vkCreateIOSSurfaceMVK(VkInstance instance, VkIOSSurfaceCreateInfoMVK* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateIOSSurfaceMVK_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

VkResult ACTIAS_ABI vkCreateMacOSSurfaceMVK(VkInstance instance, VkMacOSSurfaceCreateInfoMVK* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateMacOSSurfaceMVK_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

VkResult ACTIAS_ABI vkCreateMetalSurfaceEXT(VkInstance instance, VkMetalSurfaceCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateMetalSurfaceEXT_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

void ACTIAS_ABI vkCmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewportWScalingNV* pViewportWScalings)
{
    return vkCmdSetViewportWScalingNV_volkImpl(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
}

void ACTIAS_ABI vkCmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, VkRect2D* pDiscardRectangles)
{
    return vkCmdSetDiscardRectangleEXT_volkImpl(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}

void ACTIAS_ABI vkCmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable)
{
    return vkCmdSetDiscardRectangleEnableEXT_volkImpl(commandBuffer, discardRectangleEnable);
}

void ACTIAS_ABI vkCmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode)
{
    return vkCmdSetDiscardRectangleModeEXT_volkImpl(commandBuffer, discardRectangleMode);
}

void ACTIAS_ABI vkCmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, VkSampleLocationsInfoEXT* pSampleLocationsInfo)
{
    return vkCmdSetSampleLocationsEXT_volkImpl(commandBuffer, pSampleLocationsInfo);
}

void ACTIAS_ABI vkGetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties)
{
    return vkGetPhysicalDeviceMultisamplePropertiesEXT_volkImpl(physicalDevice, samples, pMultisampleProperties);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities)
{
    return vkGetPhysicalDeviceSurfaceCapabilities2KHR_volkImpl(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats)
{
    return vkGetPhysicalDeviceSurfaceFormats2KHR_volkImpl(physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties)
{
    return vkGetPhysicalDeviceDisplayProperties2KHR_volkImpl(physicalDevice, pPropertyCount, pProperties);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties)
{
    return vkGetPhysicalDeviceDisplayPlaneProperties2KHR_volkImpl(physicalDevice, pPropertyCount, pProperties);
}

VkResult ACTIAS_ABI vkGetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties)
{
    return vkGetDisplayModeProperties2KHR_volkImpl(physicalDevice, display, pPropertyCount, pProperties);
}

VkResult ACTIAS_ABI vkGetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities)
{
    return vkGetDisplayPlaneCapabilities2KHR_volkImpl(physicalDevice, pDisplayPlaneInfo, pCapabilities);
}

void ACTIAS_ABI vkGetBufferMemoryRequirements2(VkDevice device, VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return vkGetBufferMemoryRequirements2_volkImpl(device, pInfo, pMemoryRequirements);
}

void ACTIAS_ABI vkGetImageMemoryRequirements2(VkDevice device, VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return vkGetImageMemoryRequirements2_volkImpl(device, pInfo, pMemoryRequirements);
}

void ACTIAS_ABI vkGetImageSparseMemoryRequirements2(VkDevice device, VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    return vkGetImageSparseMemoryRequirements2_volkImpl(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

void ACTIAS_ABI vkGetDeviceBufferMemoryRequirements(VkDevice device, VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return vkGetDeviceBufferMemoryRequirements_volkImpl(device, pInfo, pMemoryRequirements);
}

void ACTIAS_ABI vkGetDeviceImageMemoryRequirements(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return vkGetDeviceImageMemoryRequirements_volkImpl(device, pInfo, pMemoryRequirements);
}

void ACTIAS_ABI vkGetDeviceImageSparseMemoryRequirements(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    return vkGetDeviceImageSparseMemoryRequirements_volkImpl(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

VkResult ACTIAS_ABI vkCreateSamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
    return vkCreateSamplerYcbcrConversion_volkImpl(device, pCreateInfo, pAllocator, pYcbcrConversion);
}

void ACTIAS_ABI vkDestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, VkAllocationCallbacks* pAllocator)
{
    return vkDestroySamplerYcbcrConversion_volkImpl(device, ycbcrConversion, pAllocator);
}

void ACTIAS_ABI vkGetDeviceQueue2(VkDevice device, VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue)
{
    return vkGetDeviceQueue2_volkImpl(device, pQueueInfo, pQueue);
}

VkResult ACTIAS_ABI vkCreateValidationCacheEXT(VkDevice device, VkValidationCacheCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache)
{
    return vkCreateValidationCacheEXT_volkImpl(device, pCreateInfo, pAllocator, pValidationCache);
}

void ACTIAS_ABI vkDestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyValidationCacheEXT_volkImpl(device, validationCache, pAllocator);
}

VkResult ACTIAS_ABI vkGetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData)
{
    return vkGetValidationCacheDataEXT_volkImpl(device, validationCache, pDataSize, pData);
}

VkResult ACTIAS_ABI vkMergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, VkValidationCacheEXT* pSrcCaches)
{
    return vkMergeValidationCachesEXT_volkImpl(device, dstCache, srcCacheCount, pSrcCaches);
}

void ACTIAS_ABI vkGetDescriptorSetLayoutSupport(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
    return vkGetDescriptorSetLayoutSupport_volkImpl(device, pCreateInfo, pSupport);
}

VkResult ACTIAS_ABI vkGetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage)
{
    return vkGetSwapchainGrallocUsageANDROID_volkImpl(device, format, imageUsage, grallocUsage);
}

VkResult ACTIAS_ABI vkGetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage)
{
    return vkGetSwapchainGrallocUsage2ANDROID_volkImpl(device, format, imageUsage, swapchainImageUsage, grallocConsumerUsage, grallocProducerUsage);
}

VkResult ACTIAS_ABI vkAcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence)
{
    return vkAcquireImageANDROID_volkImpl(device, image, nativeFenceFd, semaphore, fence);
}

VkResult ACTIAS_ABI vkQueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd)
{
    return vkQueueSignalReleaseImageANDROID_volkImpl(queue, waitSemaphoreCount, pWaitSemaphores, image, pNativeFenceFd);
}

VkResult ACTIAS_ABI vkGetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo)
{
    return vkGetShaderInfoAMD_volkImpl(device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
}

void ACTIAS_ABI vkSetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable)
{
    return vkSetLocalDimmingAMD_volkImpl(device, swapChain, localDimmingEnable);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceCalibrateableTimeDomainsKHR(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains)
{
    return vkGetPhysicalDeviceCalibrateableTimeDomainsKHR_volkImpl(physicalDevice, pTimeDomainCount, pTimeDomains);
}

VkResult ACTIAS_ABI vkGetCalibratedTimestampsKHR(VkDevice device, uint32_t timestampCount, VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation)
{
    return vkGetCalibratedTimestampsKHR_volkImpl(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
}

VkResult ACTIAS_ABI vkSetDebugUtilsObjectNameEXT(VkDevice device, VkDebugUtilsObjectNameInfoEXT* pNameInfo)
{
    return vkSetDebugUtilsObjectNameEXT_volkImpl(device, pNameInfo);
}

VkResult ACTIAS_ABI vkSetDebugUtilsObjectTagEXT(VkDevice device, VkDebugUtilsObjectTagInfoEXT* pTagInfo)
{
    return vkSetDebugUtilsObjectTagEXT_volkImpl(device, pTagInfo);
}

void ACTIAS_ABI vkQueueBeginDebugUtilsLabelEXT(VkQueue queue, VkDebugUtilsLabelEXT* pLabelInfo)
{
    return vkQueueBeginDebugUtilsLabelEXT_volkImpl(queue, pLabelInfo);
}

void ACTIAS_ABI vkQueueEndDebugUtilsLabelEXT(VkQueue queue)
{
    return vkQueueEndDebugUtilsLabelEXT_volkImpl(queue);
}

void ACTIAS_ABI vkQueueInsertDebugUtilsLabelEXT(VkQueue queue, VkDebugUtilsLabelEXT* pLabelInfo)
{
    return vkQueueInsertDebugUtilsLabelEXT_volkImpl(queue, pLabelInfo);
}

void ACTIAS_ABI vkCmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, VkDebugUtilsLabelEXT* pLabelInfo)
{
    return vkCmdBeginDebugUtilsLabelEXT_volkImpl(commandBuffer, pLabelInfo);
}

void ACTIAS_ABI vkCmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer)
{
    return vkCmdEndDebugUtilsLabelEXT_volkImpl(commandBuffer);
}

void ACTIAS_ABI vkCmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, VkDebugUtilsLabelEXT* pLabelInfo)
{
    return vkCmdInsertDebugUtilsLabelEXT_volkImpl(commandBuffer, pLabelInfo);
}

VkResult ACTIAS_ABI vkCreateDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger)
{
    return vkCreateDebugUtilsMessengerEXT_volkImpl(instance, pCreateInfo, pAllocator, pMessenger);
}

void ACTIAS_ABI vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyDebugUtilsMessengerEXT_volkImpl(instance, messenger, pAllocator);
}

void ACTIAS_ABI vkSubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
    return vkSubmitDebugUtilsMessageEXT_volkImpl(instance, messageSeverity, messageTypes, pCallbackData);
}

VkResult ACTIAS_ABI vkGetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties)
{
    return vkGetMemoryHostPointerPropertiesEXT_volkImpl(device, handleType, pHostPointer, pMemoryHostPointerProperties);
}

void ACTIAS_ABI vkCmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker)
{
    return vkCmdWriteBufferMarkerAMD_volkImpl(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}

VkResult ACTIAS_ABI vkCreateRenderPass2(VkDevice device, VkRenderPassCreateInfo2* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    return vkCreateRenderPass2_volkImpl(device, pCreateInfo, pAllocator, pRenderPass);
}

void ACTIAS_ABI vkCmdBeginRenderPass2(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassBeginInfo* pSubpassBeginInfo)
{
    return vkCmdBeginRenderPass2_volkImpl(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}

void ACTIAS_ABI vkCmdNextSubpass2(VkCommandBuffer commandBuffer, VkSubpassBeginInfo* pSubpassBeginInfo, VkSubpassEndInfo* pSubpassEndInfo)
{
    return vkCmdNextSubpass2_volkImpl(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void ACTIAS_ABI vkCmdEndRenderPass2(VkCommandBuffer commandBuffer, VkSubpassEndInfo* pSubpassEndInfo)
{
    return vkCmdEndRenderPass2_volkImpl(commandBuffer, pSubpassEndInfo);
}

VkResult ACTIAS_ABI vkGetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue)
{
    return vkGetSemaphoreCounterValue_volkImpl(device, semaphore, pValue);
}

VkResult ACTIAS_ABI vkWaitSemaphores(VkDevice device, VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout)
{
    return vkWaitSemaphores_volkImpl(device, pWaitInfo, timeout);
}

VkResult ACTIAS_ABI vkSignalSemaphore(VkDevice device, VkSemaphoreSignalInfo* pSignalInfo)
{
    return vkSignalSemaphore_volkImpl(device, pSignalInfo);
}

VkResult ACTIAS_ABI vkGetAndroidHardwareBufferPropertiesANDROID(VkDevice device, AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties)
{
    return vkGetAndroidHardwareBufferPropertiesANDROID_volkImpl(device, buffer, pProperties);
}

VkResult ACTIAS_ABI vkGetMemoryAndroidHardwareBufferANDROID(VkDevice device, VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, AHardwareBuffer* pBuffer)
{
    return vkGetMemoryAndroidHardwareBufferANDROID_volkImpl(device, pInfo, pBuffer);
}

void ACTIAS_ABI vkCmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    return vkCmdDrawIndirectCount_volkImpl(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void ACTIAS_ABI vkCmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    return vkCmdDrawIndexedIndirectCount_volkImpl(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void ACTIAS_ABI vkCmdSetCheckpointNV(VkCommandBuffer commandBuffer, void* pCheckpointMarker)
{
    return vkCmdSetCheckpointNV_volkImpl(commandBuffer, pCheckpointMarker);
}

void ACTIAS_ABI vkGetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData)
{
    return vkGetQueueCheckpointDataNV_volkImpl(queue, pCheckpointDataCount, pCheckpointData);
}

void ACTIAS_ABI vkCmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets, VkDeviceSize* pSizes)
{
    return vkCmdBindTransformFeedbackBuffersEXT_volkImpl(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
}

void ACTIAS_ABI vkCmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, VkBuffer* pCounterBuffers, VkDeviceSize* pCounterBufferOffsets)
{
    return vkCmdBeginTransformFeedbackEXT_volkImpl(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

void ACTIAS_ABI vkCmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, VkBuffer* pCounterBuffers, VkDeviceSize* pCounterBufferOffsets)
{
    return vkCmdEndTransformFeedbackEXT_volkImpl(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

void ACTIAS_ABI vkCmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index)
{
    return vkCmdBeginQueryIndexedEXT_volkImpl(commandBuffer, queryPool, query, flags, index);
}

void ACTIAS_ABI vkCmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index)
{
    return vkCmdEndQueryIndexedEXT_volkImpl(commandBuffer, queryPool, query, index);
}

void ACTIAS_ABI vkCmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride)
{
    return vkCmdDrawIndirectByteCountEXT_volkImpl(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
}

void ACTIAS_ABI vkCmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, VkRect2D* pExclusiveScissors)
{
    return vkCmdSetExclusiveScissorNV_volkImpl(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
}

void ACTIAS_ABI vkCmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, VkBool32* pExclusiveScissorEnables)
{
    return vkCmdSetExclusiveScissorEnableNV_volkImpl(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissorEnables);
}

void ACTIAS_ABI vkCmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
    return vkCmdBindShadingRateImageNV_volkImpl(commandBuffer, imageView, imageLayout);
}

void ACTIAS_ABI vkCmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkShadingRatePaletteNV* pShadingRatePalettes)
{
    return vkCmdSetViewportShadingRatePaletteNV_volkImpl(commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
}

void ACTIAS_ABI vkCmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, VkCoarseSampleOrderCustomNV* pCustomSampleOrders)
{
    return vkCmdSetCoarseSampleOrderNV_volkImpl(commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
}

void ACTIAS_ABI vkCmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask)
{
    return vkCmdDrawMeshTasksNV_volkImpl(commandBuffer, taskCount, firstTask);
}

void ACTIAS_ABI vkCmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    return vkCmdDrawMeshTasksIndirectNV_volkImpl(commandBuffer, buffer, offset, drawCount, stride);
}

void ACTIAS_ABI vkCmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    return vkCmdDrawMeshTasksIndirectCountNV_volkImpl(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void ACTIAS_ABI vkCmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    return vkCmdDrawMeshTasksEXT_volkImpl(commandBuffer, groupCountX, groupCountY, groupCountZ);
}

void ACTIAS_ABI vkCmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    return vkCmdDrawMeshTasksIndirectEXT_volkImpl(commandBuffer, buffer, offset, drawCount, stride);
}

void ACTIAS_ABI vkCmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    return vkCmdDrawMeshTasksIndirectCountEXT_volkImpl(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

VkResult ACTIAS_ABI vkCompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader)
{
    return vkCompileDeferredNV_volkImpl(device, pipeline, shader);
}

VkResult ACTIAS_ABI vkCreateAccelerationStructureNV(VkDevice device, VkAccelerationStructureCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure)
{
    return vkCreateAccelerationStructureNV_volkImpl(device, pCreateInfo, pAllocator, pAccelerationStructure);
}

void ACTIAS_ABI vkCmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
    return vkCmdBindInvocationMaskHUAWEI_volkImpl(commandBuffer, imageView, imageLayout);
}

void ACTIAS_ABI vkDestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyAccelerationStructureKHR_volkImpl(device, accelerationStructure, pAllocator);
}

void ACTIAS_ABI vkDestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyAccelerationStructureNV_volkImpl(device, accelerationStructure, pAllocator);
}

void ACTIAS_ABI vkGetAccelerationStructureMemoryRequirementsNV(VkDevice device, VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements)
{
    return vkGetAccelerationStructureMemoryRequirementsNV_volkImpl(device, pInfo, pMemoryRequirements);
}

VkResult ACTIAS_ABI vkBindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, VkBindAccelerationStructureMemoryInfoNV* pBindInfos)
{
    return vkBindAccelerationStructureMemoryNV_volkImpl(device, bindInfoCount, pBindInfos);
}

void ACTIAS_ABI vkCmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode)
{
    return vkCmdCopyAccelerationStructureNV_volkImpl(commandBuffer, dst, src, mode);
}

void ACTIAS_ABI vkCmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, VkCopyAccelerationStructureInfoKHR* pInfo)
{
    return vkCmdCopyAccelerationStructureKHR_volkImpl(commandBuffer, pInfo);
}

VkResult ACTIAS_ABI vkCopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyAccelerationStructureInfoKHR* pInfo)
{
    return vkCopyAccelerationStructureKHR_volkImpl(device, deferredOperation, pInfo);
}

void ACTIAS_ABI vkCmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, VkCopyAccelerationStructureToMemoryInfoKHR* pInfo)
{
    return vkCmdCopyAccelerationStructureToMemoryKHR_volkImpl(commandBuffer, pInfo);
}

VkResult ACTIAS_ABI vkCopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyAccelerationStructureToMemoryInfoKHR* pInfo)
{
    return vkCopyAccelerationStructureToMemoryKHR_volkImpl(device, deferredOperation, pInfo);
}

void ACTIAS_ABI vkCmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, VkCopyMemoryToAccelerationStructureInfoKHR* pInfo)
{
    return vkCmdCopyMemoryToAccelerationStructureKHR_volkImpl(commandBuffer, pInfo);
}

VkResult ACTIAS_ABI vkCopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMemoryToAccelerationStructureInfoKHR* pInfo)
{
    return vkCopyMemoryToAccelerationStructureKHR_volkImpl(device, deferredOperation, pInfo);
}

void ACTIAS_ABI vkCmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
    return vkCmdWriteAccelerationStructuresPropertiesKHR_volkImpl(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

void ACTIAS_ABI vkCmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
    return vkCmdWriteAccelerationStructuresPropertiesNV_volkImpl(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

void ACTIAS_ABI vkCmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset)
{
    return vkCmdBuildAccelerationStructureNV_volkImpl(commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}

VkResult ACTIAS_ABI vkWriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, size_t dataSize, void* pData, size_t stride)
{
    return vkWriteAccelerationStructuresPropertiesKHR_volkImpl(device, accelerationStructureCount, pAccelerationStructures, queryType, dataSize, pData, stride);
}

void ACTIAS_ABI vkCmdTraceRaysKHR(VkCommandBuffer commandBuffer, VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth)
{
    return vkCmdTraceRaysKHR_volkImpl(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
}

void ACTIAS_ABI vkCmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth)
{
    return vkCmdTraceRaysNV_volkImpl(commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}

VkResult ACTIAS_ABI vkGetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
    return vkGetRayTracingShaderGroupHandlesKHR_volkImpl(device, pipeline, firstGroup, groupCount, dataSize, pData);
}

VkResult ACTIAS_ABI vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
    return vkGetRayTracingCaptureReplayShaderGroupHandlesKHR_volkImpl(device, pipeline, firstGroup, groupCount, dataSize, pData);
}

VkResult ACTIAS_ABI vkGetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData)
{
    return vkGetAccelerationStructureHandleNV_volkImpl(device, accelerationStructure, dataSize, pData);
}

VkResult ACTIAS_ABI vkCreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkRayTracingPipelineCreateInfoNV* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    return vkCreateRayTracingPipelinesNV_volkImpl(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VkResult ACTIAS_ABI vkCreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkRayTracingPipelineCreateInfoNV* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    return vkCreateRayTracingPipelinesNV_volkImpl(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VkResult ACTIAS_ABI vkCreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkRayTracingPipelineCreateInfoKHR* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    return vkCreateRayTracingPipelinesKHR_volkImpl(device, deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VkResult ACTIAS_ABI vkCreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkRayTracingPipelineCreateInfoKHR* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    return vkCreateRayTracingPipelinesKHR_volkImpl(device, deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties)
{
    return vkGetPhysicalDeviceCooperativeMatrixPropertiesNV_volkImpl(physicalDevice, pPropertyCount, pProperties);
}

void ACTIAS_ABI vkCmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress)
{
    return vkCmdTraceRaysIndirectKHR_volkImpl(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
}

void ACTIAS_ABI vkCmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress)
{
    return vkCmdTraceRaysIndirect2KHR_volkImpl(commandBuffer, indirectDeviceAddress);
}

void ACTIAS_ABI vkGetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility)
{
    return vkGetDeviceAccelerationStructureCompatibilityKHR_volkImpl(device, pVersionInfo, pCompatibility);
}

VkDeviceSize ACTIAS_ABI vkGetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader)
{
    return vkGetRayTracingShaderGroupStackSizeKHR_volkImpl(device, pipeline, group, groupShader);
}

void ACTIAS_ABI vkCmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize)
{
    return vkCmdSetRayTracingPipelineStackSizeKHR_volkImpl(commandBuffer, pipelineStackSize);
}

uint32_t ACTIAS_ABI vkGetImageViewHandleNVX(VkDevice device, VkImageViewHandleInfoNVX* pInfo)
{
    return vkGetImageViewHandleNVX_volkImpl(device, pInfo);
}

VkResult ACTIAS_ABI vkGetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties)
{
    return vkGetImageViewAddressNVX_volkImpl(device, imageView, pProperties);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfacePresentModes2EXT(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
    return vkGetPhysicalDeviceSurfacePresentModes2EXT_volkImpl(physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
}

VkResult ACTIAS_ABI vkGetDeviceGroupSurfacePresentModes2EXT(VkDevice device, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes)
{
    return vkGetDeviceGroupSurfacePresentModes2EXT_volkImpl(device, pSurfaceInfo, pModes);
}

VkResult ACTIAS_ABI vkAcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain)
{
    return vkAcquireFullScreenExclusiveModeEXT_volkImpl(device, swapchain);
}

VkResult ACTIAS_ABI vkReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain)
{
    return vkReleaseFullScreenExclusiveModeEXT_volkImpl(device, swapchain);
}

VkResult ACTIAS_ABI vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions)
{
    return vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR_volkImpl(physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions);
}

void ACTIAS_ABI vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses)
{
    return vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR_volkImpl(physicalDevice, pPerformanceQueryCreateInfo, pNumPasses);
}

VkResult ACTIAS_ABI vkAcquireProfilingLockKHR(VkDevice device, VkAcquireProfilingLockInfoKHR* pInfo)
{
    return vkAcquireProfilingLockKHR_volkImpl(device, pInfo);
}

void ACTIAS_ABI vkReleaseProfilingLockKHR(VkDevice device)
{
    return vkReleaseProfilingLockKHR_volkImpl(device);
}

VkResult ACTIAS_ABI vkGetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties)
{
    return vkGetImageDrmFormatModifierPropertiesEXT_volkImpl(device, image, pProperties);
}

uint64_t ACTIAS_ABI vkGetBufferOpaqueCaptureAddress(VkDevice device, VkBufferDeviceAddressInfo* pInfo)
{
    return vkGetBufferOpaqueCaptureAddress_volkImpl(device, pInfo);
}

VkDeviceAddress ACTIAS_ABI vkGetBufferDeviceAddress(VkDevice device, VkBufferDeviceAddressInfo* pInfo)
{
    return vkGetBufferDeviceAddress_volkImpl(device, pInfo);
}

VkResult ACTIAS_ABI vkCreateHeadlessSurfaceEXT(VkInstance instance, VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    return vkCreateHeadlessSurfaceEXT_volkImpl(instance, pCreateInfo, pAllocator, pSurface);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations)
{
    return vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV_volkImpl(physicalDevice, pCombinationCount, pCombinations);
}

VkResult ACTIAS_ABI vkInitializePerformanceApiINTEL(VkDevice device, VkInitializePerformanceApiInfoINTEL* pInitializeInfo)
{
    return vkInitializePerformanceApiINTEL_volkImpl(device, pInitializeInfo);
}

void ACTIAS_ABI vkUninitializePerformanceApiINTEL(VkDevice device)
{
    return vkUninitializePerformanceApiINTEL_volkImpl(device);
}

VkResult ACTIAS_ABI vkCmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, VkPerformanceMarkerInfoINTEL* pMarkerInfo)
{
    return vkCmdSetPerformanceMarkerINTEL_volkImpl(commandBuffer, pMarkerInfo);
}

VkResult ACTIAS_ABI vkCmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo)
{
    return vkCmdSetPerformanceStreamMarkerINTEL_volkImpl(commandBuffer, pMarkerInfo);
}

VkResult ACTIAS_ABI vkCmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, VkPerformanceOverrideInfoINTEL* pOverrideInfo)
{
    return vkCmdSetPerformanceOverrideINTEL_volkImpl(commandBuffer, pOverrideInfo);
}

VkResult ACTIAS_ABI vkAcquirePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration)
{
    return vkAcquirePerformanceConfigurationINTEL_volkImpl(device, pAcquireInfo, pConfiguration);
}

VkResult ACTIAS_ABI vkReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration)
{
    return vkReleasePerformanceConfigurationINTEL_volkImpl(device, configuration);
}

VkResult ACTIAS_ABI vkQueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration)
{
    return vkQueueSetPerformanceConfigurationINTEL_volkImpl(queue, configuration);
}

VkResult ACTIAS_ABI vkGetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue)
{
    return vkGetPerformanceParameterINTEL_volkImpl(device, parameter, pValue);
}

uint64_t ACTIAS_ABI vkGetDeviceMemoryOpaqueCaptureAddress(VkDevice device, VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo)
{
    return vkGetDeviceMemoryOpaqueCaptureAddress_volkImpl(device, pInfo);
}

VkResult ACTIAS_ABI vkGetPipelineExecutablePropertiesKHR(VkDevice device, VkPipelineInfoKHR* pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties)
{
    return vkGetPipelineExecutablePropertiesKHR_volkImpl(device, pPipelineInfo, pExecutableCount, pProperties);
}

VkResult ACTIAS_ABI vkGetPipelineExecutableStatisticsKHR(VkDevice device, VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics)
{
    return vkGetPipelineExecutableStatisticsKHR_volkImpl(device, pExecutableInfo, pStatisticCount, pStatistics);
}

VkResult ACTIAS_ABI vkGetPipelineExecutableInternalRepresentationsKHR(VkDevice device, VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations)
{
    return vkGetPipelineExecutableInternalRepresentationsKHR_volkImpl(device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
}

void ACTIAS_ABI vkCmdSetLineStippleKHR(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern)
{
    return vkCmdSetLineStippleKHR_volkImpl(commandBuffer, lineStippleFactor, lineStipplePattern);
}

VkResult ACTIAS_ABI vkGetFaultData(VkDevice device, VkFaultQueryBehavior faultQueryBehavior, VkBool32* pUnrecordedFaults, uint32_t* pFaultCount, VkFaultData* pFaults)
{
    return vkGetFaultData_volkImpl(device, faultQueryBehavior, pUnrecordedFaults, pFaultCount, pFaults);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties)
{
    return vkGetPhysicalDeviceToolProperties_volkImpl(physicalDevice, pToolCount, pToolProperties);
}

VkResult ACTIAS_ABI vkCreateAccelerationStructureKHR(VkDevice device, VkAccelerationStructureCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkAccelerationStructureKHR* pAccelerationStructure)
{
    return vkCreateAccelerationStructureKHR_volkImpl(device, pCreateInfo, pAllocator, pAccelerationStructure);
}

void ACTIAS_ABI vkCmdBuildAccelerationStructuresKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* ppBuildRangeInfos)
{
    return vkCmdBuildAccelerationStructuresKHR_volkImpl(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
}

void ACTIAS_ABI vkCmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, VkAccelerationStructureBuildGeometryInfoKHR* pInfos, VkDeviceAddress* pIndirectDeviceAddresses, uint32_t* pIndirectStrides, const uint32_t* ppMaxPrimitiveCounts)
{
    return vkCmdBuildAccelerationStructuresIndirectKHR_volkImpl(commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
}

VkResult ACTIAS_ABI vkBuildAccelerationStructuresKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* ppBuildRangeInfos)
{
    return vkBuildAccelerationStructuresKHR_volkImpl(device, deferredOperation, infoCount, pInfos, ppBuildRangeInfos);
}

VkDeviceAddress ACTIAS_ABI vkGetAccelerationStructureDeviceAddressKHR(VkDevice device, VkAccelerationStructureDeviceAddressInfoKHR* pInfo)
{
    return vkGetAccelerationStructureDeviceAddressKHR_volkImpl(device, pInfo);
}

VkResult ACTIAS_ABI vkCreateDeferredOperationKHR(VkDevice device, VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation)
{
    return vkCreateDeferredOperationKHR_volkImpl(device, pAllocator, pDeferredOperation);
}

void ACTIAS_ABI vkDestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyDeferredOperationKHR_volkImpl(device, operation, pAllocator);
}

uint32_t ACTIAS_ABI vkGetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation)
{
    return vkGetDeferredOperationMaxConcurrencyKHR_volkImpl(device, operation);
}

VkResult ACTIAS_ABI vkGetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation)
{
    return vkGetDeferredOperationResultKHR_volkImpl(device, operation);
}

VkResult ACTIAS_ABI vkDeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation)
{
    return vkDeferredOperationJoinKHR_volkImpl(device, operation);
}

void ACTIAS_ABI vkGetPipelineIndirectMemoryRequirementsNV(VkDevice device, VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    return vkGetPipelineIndirectMemoryRequirementsNV_volkImpl(device, pCreateInfo, pMemoryRequirements);
}

VkDeviceAddress ACTIAS_ABI vkGetPipelineIndirectDeviceAddressNV(VkDevice device, VkPipelineIndirectDeviceAddressInfoNV* pInfo)
{
    return vkGetPipelineIndirectDeviceAddressNV_volkImpl(device, pInfo);
}

void ACTIAS_ABI vkCmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode)
{
    return vkCmdSetCullMode_volkImpl(commandBuffer, cullMode);
}

void ACTIAS_ABI vkCmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace)
{
    return vkCmdSetFrontFace_volkImpl(commandBuffer, frontFace);
}

void ACTIAS_ABI vkCmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology)
{
    return vkCmdSetPrimitiveTopology_volkImpl(commandBuffer, primitiveTopology);
}

void ACTIAS_ABI vkCmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, VkViewport* pViewports)
{
    return vkCmdSetViewportWithCount_volkImpl(commandBuffer, viewportCount, pViewports);
}

void ACTIAS_ABI vkCmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, VkRect2D* pScissors)
{
    return vkCmdSetScissorWithCount_volkImpl(commandBuffer, scissorCount, pScissors);
}

void ACTIAS_ABI vkCmdBindIndexBuffer2KHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType)
{
    return vkCmdBindIndexBuffer2KHR_volkImpl(commandBuffer, buffer, offset, size, indexType);
}

void ACTIAS_ABI vkCmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets, VkDeviceSize* pSizes, VkDeviceSize* pStrides)
{
    return vkCmdBindVertexBuffers2_volkImpl(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}

void ACTIAS_ABI vkCmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable)
{
    return vkCmdSetDepthTestEnable_volkImpl(commandBuffer, depthTestEnable);
}

void ACTIAS_ABI vkCmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable)
{
    return vkCmdSetDepthWriteEnable_volkImpl(commandBuffer, depthWriteEnable);
}

void ACTIAS_ABI vkCmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp)
{
    return vkCmdSetDepthCompareOp_volkImpl(commandBuffer, depthCompareOp);
}

void ACTIAS_ABI vkCmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable)
{
    return vkCmdSetDepthBoundsTestEnable_volkImpl(commandBuffer, depthBoundsTestEnable);
}

void ACTIAS_ABI vkCmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable)
{
    return vkCmdSetStencilTestEnable_volkImpl(commandBuffer, stencilTestEnable);
}

void ACTIAS_ABI vkCmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp)
{
    return vkCmdSetStencilOp_volkImpl(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

void ACTIAS_ABI vkCmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints)
{
    return vkCmdSetPatchControlPointsEXT_volkImpl(commandBuffer, patchControlPoints);
}

void ACTIAS_ABI vkCmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable)
{
    return vkCmdSetRasterizerDiscardEnable_volkImpl(commandBuffer, rasterizerDiscardEnable);
}

void ACTIAS_ABI vkCmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable)
{
    return vkCmdSetDepthBiasEnable_volkImpl(commandBuffer, depthBiasEnable);
}

void ACTIAS_ABI vkCmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp)
{
    return vkCmdSetLogicOpEXT_volkImpl(commandBuffer, logicOp);
}

void ACTIAS_ABI vkCmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable)
{
    return vkCmdSetPrimitiveRestartEnable_volkImpl(commandBuffer, primitiveRestartEnable);
}

void ACTIAS_ABI vkCmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin)
{
    return vkCmdSetTessellationDomainOriginEXT_volkImpl(commandBuffer, domainOrigin);
}

void ACTIAS_ABI vkCmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable)
{
    return vkCmdSetDepthClampEnableEXT_volkImpl(commandBuffer, depthClampEnable);
}

void ACTIAS_ABI vkCmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode)
{
    return vkCmdSetPolygonModeEXT_volkImpl(commandBuffer, polygonMode);
}

void ACTIAS_ABI vkCmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits rasterizationSamples)
{
    return vkCmdSetRasterizationSamplesEXT_volkImpl(commandBuffer, rasterizationSamples);
}

void ACTIAS_ABI vkCmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits samples, VkSampleMask* pSampleMask)
{
    return vkCmdSetSampleMaskEXT_volkImpl(commandBuffer, samples, pSampleMask);
}

void ACTIAS_ABI vkCmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable)
{
    return vkCmdSetAlphaToCoverageEnableEXT_volkImpl(commandBuffer, alphaToCoverageEnable);
}

void ACTIAS_ABI vkCmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable)
{
    return vkCmdSetAlphaToOneEnableEXT_volkImpl(commandBuffer, alphaToOneEnable);
}

void ACTIAS_ABI vkCmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable)
{
    return vkCmdSetLogicOpEnableEXT_volkImpl(commandBuffer, logicOpEnable);
}

void ACTIAS_ABI vkCmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkBool32* pColorBlendEnables)
{
    return vkCmdSetColorBlendEnableEXT_volkImpl(commandBuffer, firstAttachment, attachmentCount, pColorBlendEnables);
}

void ACTIAS_ABI vkCmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkColorBlendEquationEXT* pColorBlendEquations)
{
    return vkCmdSetColorBlendEquationEXT_volkImpl(commandBuffer, firstAttachment, attachmentCount, pColorBlendEquations);
}

void ACTIAS_ABI vkCmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkColorComponentFlags* pColorWriteMasks)
{
    return vkCmdSetColorWriteMaskEXT_volkImpl(commandBuffer, firstAttachment, attachmentCount, pColorWriteMasks);
}

void ACTIAS_ABI vkCmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream)
{
    return vkCmdSetRasterizationStreamEXT_volkImpl(commandBuffer, rasterizationStream);
}

void ACTIAS_ABI vkCmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode)
{
    return vkCmdSetConservativeRasterizationModeEXT_volkImpl(commandBuffer, conservativeRasterizationMode);
}

void ACTIAS_ABI vkCmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize)
{
    return vkCmdSetExtraPrimitiveOverestimationSizeEXT_volkImpl(commandBuffer, extraPrimitiveOverestimationSize);
}

void ACTIAS_ABI vkCmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable)
{
    return vkCmdSetDepthClipEnableEXT_volkImpl(commandBuffer, depthClipEnable);
}

void ACTIAS_ABI vkCmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable)
{
    return vkCmdSetSampleLocationsEnableEXT_volkImpl(commandBuffer, sampleLocationsEnable);
}

void ACTIAS_ABI vkCmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkColorBlendAdvancedEXT* pColorBlendAdvanced)
{
    return vkCmdSetColorBlendAdvancedEXT_volkImpl(commandBuffer, firstAttachment, attachmentCount, pColorBlendAdvanced);
}

void ACTIAS_ABI vkCmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode)
{
    return vkCmdSetProvokingVertexModeEXT_volkImpl(commandBuffer, provokingVertexMode);
}

void ACTIAS_ABI vkCmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode)
{
    return vkCmdSetLineRasterizationModeEXT_volkImpl(commandBuffer, lineRasterizationMode);
}

void ACTIAS_ABI vkCmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable)
{
    return vkCmdSetLineStippleEnableEXT_volkImpl(commandBuffer, stippledLineEnable);
}

void ACTIAS_ABI vkCmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne)
{
    return vkCmdSetDepthClipNegativeOneToOneEXT_volkImpl(commandBuffer, negativeOneToOne);
}

void ACTIAS_ABI vkCmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable)
{
    return vkCmdSetViewportWScalingEnableNV_volkImpl(commandBuffer, viewportWScalingEnable);
}

void ACTIAS_ABI vkCmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewportSwizzleNV* pViewportSwizzles)
{
    return vkCmdSetViewportSwizzleNV_volkImpl(commandBuffer, firstViewport, viewportCount, pViewportSwizzles);
}

void ACTIAS_ABI vkCmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable)
{
    return vkCmdSetCoverageToColorEnableNV_volkImpl(commandBuffer, coverageToColorEnable);
}

void ACTIAS_ABI vkCmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation)
{
    return vkCmdSetCoverageToColorLocationNV_volkImpl(commandBuffer, coverageToColorLocation);
}

void ACTIAS_ABI vkCmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode)
{
    return vkCmdSetCoverageModulationModeNV_volkImpl(commandBuffer, coverageModulationMode);
}

void ACTIAS_ABI vkCmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable)
{
    return vkCmdSetCoverageModulationTableEnableNV_volkImpl(commandBuffer, coverageModulationTableEnable);
}

void ACTIAS_ABI vkCmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, float* pCoverageModulationTable)
{
    return vkCmdSetCoverageModulationTableNV_volkImpl(commandBuffer, coverageModulationTableCount, pCoverageModulationTable);
}

void ACTIAS_ABI vkCmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable)
{
    return vkCmdSetShadingRateImageEnableNV_volkImpl(commandBuffer, shadingRateImageEnable);
}

void ACTIAS_ABI vkCmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode)
{
    return vkCmdSetCoverageReductionModeNV_volkImpl(commandBuffer, coverageReductionMode);
}

void ACTIAS_ABI vkCmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable)
{
    return vkCmdSetRepresentativeFragmentTestEnableNV_volkImpl(commandBuffer, representativeFragmentTestEnable);
}

VkResult ACTIAS_ABI vkCreatePrivateDataSlot(VkDevice device, VkPrivateDataSlotCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot)
{
    return vkCreatePrivateDataSlot_volkImpl(device, pCreateInfo, pAllocator, pPrivateDataSlot);
}

void ACTIAS_ABI vkDestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyPrivateDataSlot_volkImpl(device, privateDataSlot, pAllocator);
}

VkResult ACTIAS_ABI vkSetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data)
{
    return vkSetPrivateData_volkImpl(device, objectType, objectHandle, privateDataSlot, data);
}

void ACTIAS_ABI vkGetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData)
{
    return vkGetPrivateData_volkImpl(device, objectType, objectHandle, privateDataSlot, pData);
}

void ACTIAS_ABI vkCmdCopyBuffer2(VkCommandBuffer commandBuffer, VkCopyBufferInfo2* pCopyBufferInfo)
{
    return vkCmdCopyBuffer2_volkImpl(commandBuffer, pCopyBufferInfo);
}

void ACTIAS_ABI vkCmdCopyImage2(VkCommandBuffer commandBuffer, VkCopyImageInfo2* pCopyImageInfo)
{
    return vkCmdCopyImage2_volkImpl(commandBuffer, pCopyImageInfo);
}

void ACTIAS_ABI vkCmdBlitImage2(VkCommandBuffer commandBuffer, VkBlitImageInfo2* pBlitImageInfo)
{
    return vkCmdBlitImage2_volkImpl(commandBuffer, pBlitImageInfo);
}

void ACTIAS_ABI vkCmdCopyBufferToImage2(VkCommandBuffer commandBuffer, VkCopyBufferToImageInfo2* pCopyBufferToImageInfo)
{
    return vkCmdCopyBufferToImage2_volkImpl(commandBuffer, pCopyBufferToImageInfo);
}

void ACTIAS_ABI vkCmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, VkCopyImageToBufferInfo2* pCopyImageToBufferInfo)
{
    return vkCmdCopyImageToBuffer2_volkImpl(commandBuffer, pCopyImageToBufferInfo);
}

void ACTIAS_ABI vkCmdResolveImage2(VkCommandBuffer commandBuffer, VkResolveImageInfo2* pResolveImageInfo)
{
    return vkCmdResolveImage2_volkImpl(commandBuffer, pResolveImageInfo);
}

void ACTIAS_ABI vkCmdRefreshObjectsKHR(VkCommandBuffer commandBuffer, VkRefreshObjectListKHR* pRefreshObjects)
{
    return vkCmdRefreshObjectsKHR_volkImpl(commandBuffer, pRefreshObjects);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceRefreshableObjectTypesKHR(VkPhysicalDevice physicalDevice, uint32_t* pRefreshableObjectTypeCount, VkObjectType* pRefreshableObjectTypes)
{
    return vkGetPhysicalDeviceRefreshableObjectTypesKHR_volkImpl(physicalDevice, pRefreshableObjectTypeCount, pRefreshableObjectTypes);
}

void ACTIAS_ABI vkCmdSetFragmentShadingRateKHR(VkCommandBuffer commandBuffer, VkExtent2D* pFragmentSize, VkFragmentShadingRateCombinerOpKHR combinerOps)
{
    return vkCmdSetFragmentShadingRateKHR_volkImpl(commandBuffer, pFragmentSize, combinerOps);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates)
{
    return vkGetPhysicalDeviceFragmentShadingRatesKHR_volkImpl(physicalDevice, pFragmentShadingRateCount, pFragmentShadingRates);
}

void ACTIAS_ABI vkCmdSetFragmentShadingRateEnumNV(VkCommandBuffer commandBuffer, VkFragmentShadingRateNV shadingRate, VkFragmentShadingRateCombinerOpKHR combinerOps)
{
    return vkCmdSetFragmentShadingRateEnumNV_volkImpl(commandBuffer, shadingRate, combinerOps);
}

void ACTIAS_ABI vkGetAccelerationStructureBuildSizesKHR(VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo, uint32_t* pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo)
{
    return vkGetAccelerationStructureBuildSizesKHR_volkImpl(device, buildType, pBuildInfo, pMaxPrimitiveCounts, pSizeInfo);
}

void ACTIAS_ABI vkCmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions)
{
    return vkCmdSetVertexInputEXT_volkImpl(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}

void ACTIAS_ABI vkCmdSetColorWriteEnableEXT(VkCommandBuffer commandBuffer, uint32_t attachmentCount, VkBool32* pColorWriteEnables)
{
    return vkCmdSetColorWriteEnableEXT_volkImpl(commandBuffer, attachmentCount, pColorWriteEnables);
}

void ACTIAS_ABI vkCmdSetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkDependencyInfo* pDependencyInfo)
{
    return vkCmdSetEvent2_volkImpl(commandBuffer, event, pDependencyInfo);
}

void ACTIAS_ABI vkCmdResetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask)
{
    return vkCmdResetEvent2_volkImpl(commandBuffer, event, stageMask);
}

void ACTIAS_ABI vkCmdWaitEvents2(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkDependencyInfo* pDependencyInfos)
{
    return vkCmdWaitEvents2_volkImpl(commandBuffer, eventCount, pEvents, pDependencyInfos);
}

void ACTIAS_ABI vkCmdPipelineBarrier2(VkCommandBuffer commandBuffer, VkDependencyInfo* pDependencyInfo)
{
    return vkCmdPipelineBarrier2_volkImpl(commandBuffer, pDependencyInfo);
}

VkResult ACTIAS_ABI vkQueueSubmit2(VkQueue queue, uint32_t submitCount, VkSubmitInfo2* pSubmits, VkFence fence)
{
    return vkQueueSubmit2_volkImpl(queue, submitCount, pSubmits, fence);
}

void ACTIAS_ABI vkCmdWriteTimestamp2(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query)
{
    return vkCmdWriteTimestamp2_volkImpl(commandBuffer, stage, queryPool, query);
}

void ACTIAS_ABI vkCmdWriteBufferMarker2AMD(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker)
{
    return vkCmdWriteBufferMarker2AMD_volkImpl(commandBuffer, stage, dstBuffer, dstOffset, marker);
}

void ACTIAS_ABI vkGetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData)
{
    return vkGetQueueCheckpointData2NV_volkImpl(queue, pCheckpointDataCount, pCheckpointData);
}

VkResult ACTIAS_ABI vkCopyMemoryToImageEXT(VkDevice device, VkCopyMemoryToImageInfoEXT* pCopyMemoryToImageInfo)
{
    return vkCopyMemoryToImageEXT_volkImpl(device, pCopyMemoryToImageInfo);
}

VkResult ACTIAS_ABI vkCopyImageToMemoryEXT(VkDevice device, VkCopyImageToMemoryInfoEXT* pCopyImageToMemoryInfo)
{
    return vkCopyImageToMemoryEXT_volkImpl(device, pCopyImageToMemoryInfo);
}

VkResult ACTIAS_ABI vkCopyImageToImageEXT(VkDevice device, VkCopyImageToImageInfoEXT* pCopyImageToImageInfo)
{
    return vkCopyImageToImageEXT_volkImpl(device, pCopyImageToImageInfo);
}

VkResult ACTIAS_ABI vkTransitionImageLayoutEXT(VkDevice device, uint32_t transitionCount, VkHostImageLayoutTransitionInfoEXT* pTransitions)
{
    return vkTransitionImageLayoutEXT_volkImpl(device, transitionCount, pTransitions);
}

void ACTIAS_ABI vkGetCommandPoolMemoryConsumption(VkDevice device, VkCommandPool commandPool, VkCommandBuffer commandBuffer, VkCommandPoolMemoryConsumption* pConsumption)
{
    return vkGetCommandPoolMemoryConsumption_volkImpl(device, commandPool, commandBuffer, pConsumption);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceVideoCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities)
{
    return vkGetPhysicalDeviceVideoCapabilitiesKHR_volkImpl(physicalDevice, pVideoProfile, pCapabilities);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceVideoFormatPropertiesKHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties)
{
    return vkGetPhysicalDeviceVideoFormatPropertiesKHR_volkImpl(physicalDevice, pVideoFormatInfo, pVideoFormatPropertyCount, pVideoFormatProperties);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR* pQualityLevelInfo, VkVideoEncodeQualityLevelPropertiesKHR* pQualityLevelProperties)
{
    return vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR_volkImpl(physicalDevice, pQualityLevelInfo, pQualityLevelProperties);
}

VkResult ACTIAS_ABI vkCreateVideoSessionKHR(VkDevice device, VkVideoSessionCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession)
{
    return vkCreateVideoSessionKHR_volkImpl(device, pCreateInfo, pAllocator, pVideoSession);
}

void ACTIAS_ABI vkDestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyVideoSessionKHR_volkImpl(device, videoSession, pAllocator);
}

VkResult ACTIAS_ABI vkCreateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters)
{
    return vkCreateVideoSessionParametersKHR_volkImpl(device, pCreateInfo, pAllocator, pVideoSessionParameters);
}

VkResult ACTIAS_ABI vkUpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo)
{
    return vkUpdateVideoSessionParametersKHR_volkImpl(device, videoSessionParameters, pUpdateInfo);
}

VkResult ACTIAS_ABI vkGetEncodedVideoSessionParametersKHR(VkDevice device, VkVideoEncodeSessionParametersGetInfoKHR* pVideoSessionParametersInfo, VkVideoEncodeSessionParametersFeedbackInfoKHR* pFeedbackInfo, size_t* pDataSize, void* pData)
{
    return vkGetEncodedVideoSessionParametersKHR_volkImpl(device, pVideoSessionParametersInfo, pFeedbackInfo, pDataSize, pData);
}

void ACTIAS_ABI vkDestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyVideoSessionParametersKHR_volkImpl(device, videoSessionParameters, pAllocator);
}

VkResult ACTIAS_ABI vkGetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements)
{
    return vkGetVideoSessionMemoryRequirementsKHR_volkImpl(device, videoSession, pMemoryRequirementsCount, pMemoryRequirements);
}

VkResult ACTIAS_ABI vkBindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos)
{
    return vkBindVideoSessionMemoryKHR_volkImpl(device, videoSession, bindSessionMemoryInfoCount, pBindSessionMemoryInfos);
}

void ACTIAS_ABI vkCmdDecodeVideoKHR(VkCommandBuffer commandBuffer, VkVideoDecodeInfoKHR* pDecodeInfo)
{
    return vkCmdDecodeVideoKHR_volkImpl(commandBuffer, pDecodeInfo);
}

void ACTIAS_ABI vkCmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, VkVideoBeginCodingInfoKHR* pBeginInfo)
{
    return vkCmdBeginVideoCodingKHR_volkImpl(commandBuffer, pBeginInfo);
}

void ACTIAS_ABI vkCmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, VkVideoCodingControlInfoKHR* pCodingControlInfo)
{
    return vkCmdControlVideoCodingKHR_volkImpl(commandBuffer, pCodingControlInfo);
}

void ACTIAS_ABI vkCmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, VkVideoEndCodingInfoKHR* pEndCodingInfo)
{
    return vkCmdEndVideoCodingKHR_volkImpl(commandBuffer, pEndCodingInfo);
}

void ACTIAS_ABI vkCmdEncodeVideoKHR(VkCommandBuffer commandBuffer, VkVideoEncodeInfoKHR* pEncodeInfo)
{
    return vkCmdEncodeVideoKHR_volkImpl(commandBuffer, pEncodeInfo);
}

void ACTIAS_ABI vkCmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, VkDecompressMemoryRegionNV* pDecompressMemoryRegions)
{
    return vkCmdDecompressMemoryNV_volkImpl(commandBuffer, decompressRegionCount, pDecompressMemoryRegions);
}

void ACTIAS_ABI vkCmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride)
{
    return vkCmdDecompressMemoryIndirectCountNV_volkImpl(commandBuffer, indirectCommandsAddress, indirectCommandsCountAddress, stride);
}

VkResult ACTIAS_ABI vkCreateCuModuleNVX(VkDevice device, VkCuModuleCreateInfoNVX* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule)
{
    return vkCreateCuModuleNVX_volkImpl(device, pCreateInfo, pAllocator, pModule);
}

VkResult ACTIAS_ABI vkCreateCuFunctionNVX(VkDevice device, VkCuFunctionCreateInfoNVX* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction)
{
    return vkCreateCuFunctionNVX_volkImpl(device, pCreateInfo, pAllocator, pFunction);
}

void ACTIAS_ABI vkDestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyCuModuleNVX_volkImpl(device, module, pAllocator);
}

void ACTIAS_ABI vkDestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyCuFunctionNVX_volkImpl(device, function, pAllocator);
}

void ACTIAS_ABI vkCmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, VkCuLaunchInfoNVX* pLaunchInfo)
{
    return vkCmdCuLaunchKernelNVX_volkImpl(commandBuffer, pLaunchInfo);
}

void ACTIAS_ABI vkGetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes)
{
    return vkGetDescriptorSetLayoutSizeEXT_volkImpl(device, layout, pLayoutSizeInBytes);
}

void ACTIAS_ABI vkGetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset)
{
    return vkGetDescriptorSetLayoutBindingOffsetEXT_volkImpl(device, layout, binding, pOffset);
}

void ACTIAS_ABI vkGetDescriptorEXT(VkDevice device, VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor)
{
    return vkGetDescriptorEXT_volkImpl(device, pDescriptorInfo, dataSize, pDescriptor);
}

void ACTIAS_ABI vkCmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, VkDescriptorBufferBindingInfoEXT* pBindingInfos)
{
    return vkCmdBindDescriptorBuffersEXT_volkImpl(commandBuffer, bufferCount, pBindingInfos);
}

void ACTIAS_ABI vkCmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, uint32_t* pBufferIndices, VkDeviceSize* pOffsets)
{
    return vkCmdSetDescriptorBufferOffsetsEXT_volkImpl(commandBuffer, pipelineBindPoint, layout, firstSet, setCount, pBufferIndices, pOffsets);
}

void ACTIAS_ABI vkCmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set)
{
    return vkCmdBindDescriptorBufferEmbeddedSamplersEXT_volkImpl(commandBuffer, pipelineBindPoint, layout, set);
}

VkResult ACTIAS_ABI vkGetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData)
{
    return vkGetBufferOpaqueCaptureDescriptorDataEXT_volkImpl(device, pInfo, pData);
}

VkResult ACTIAS_ABI vkGetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData)
{
    return vkGetImageOpaqueCaptureDescriptorDataEXT_volkImpl(device, pInfo, pData);
}

VkResult ACTIAS_ABI vkGetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData)
{
    return vkGetImageViewOpaqueCaptureDescriptorDataEXT_volkImpl(device, pInfo, pData);
}

VkResult ACTIAS_ABI vkGetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData)
{
    return vkGetSamplerOpaqueCaptureDescriptorDataEXT_volkImpl(device, pInfo, pData);
}

VkResult ACTIAS_ABI vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData)
{
    return vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT_volkImpl(device, pInfo, pData);
}

void ACTIAS_ABI vkSetDeviceMemoryPriorityEXT(VkDevice device, VkDeviceMemory memory, float priority)
{
    return vkSetDeviceMemoryPriorityEXT_volkImpl(device, memory, priority);
}

VkResult ACTIAS_ABI vkAcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display)
{
    return vkAcquireDrmDisplayEXT_volkImpl(physicalDevice, drmFd, display);
}

VkResult ACTIAS_ABI vkGetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display)
{
    return vkGetDrmDisplayEXT_volkImpl(physicalDevice, drmFd, connectorId, display);
}

VkResult ACTIAS_ABI vkWaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout)
{
    return vkWaitForPresentKHR_volkImpl(device, swapchain, presentId, timeout);
}

VkResult ACTIAS_ABI vkCreateBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection)
{
    return vkCreateBufferCollectionFUCHSIA_volkImpl(device, pCreateInfo, pAllocator, pCollection);
}

VkResult ACTIAS_ABI vkSetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo)
{
    return vkSetBufferCollectionBufferConstraintsFUCHSIA_volkImpl(device, collection, pBufferConstraintsInfo);
}

VkResult ACTIAS_ABI vkSetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo)
{
    return vkSetBufferCollectionImageConstraintsFUCHSIA_volkImpl(device, collection, pImageConstraintsInfo);
}

void ACTIAS_ABI vkDestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyBufferCollectionFUCHSIA_volkImpl(device, collection, pAllocator);
}

VkResult ACTIAS_ABI vkGetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties)
{
    return vkGetBufferCollectionPropertiesFUCHSIA_volkImpl(device, collection, pProperties);
}

VkResult ACTIAS_ABI vkCreateCudaModuleNV(VkDevice device, VkCudaModuleCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCudaModuleNV* pModule)
{
    return vkCreateCudaModuleNV_volkImpl(device, pCreateInfo, pAllocator, pModule);
}

VkResult ACTIAS_ABI vkGetCudaModuleCacheNV(VkDevice device, VkCudaModuleNV module, size_t* pCacheSize, void* pCacheData)
{
    return vkGetCudaModuleCacheNV_volkImpl(device, module, pCacheSize, pCacheData);
}

VkResult ACTIAS_ABI vkCreateCudaFunctionNV(VkDevice device, VkCudaFunctionCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCudaFunctionNV* pFunction)
{
    return vkCreateCudaFunctionNV_volkImpl(device, pCreateInfo, pAllocator, pFunction);
}

void ACTIAS_ABI vkDestroyCudaModuleNV(VkDevice device, VkCudaModuleNV module, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyCudaModuleNV_volkImpl(device, module, pAllocator);
}

void ACTIAS_ABI vkDestroyCudaFunctionNV(VkDevice device, VkCudaFunctionNV function, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyCudaFunctionNV_volkImpl(device, function, pAllocator);
}

void ACTIAS_ABI vkCmdCudaLaunchKernelNV(VkCommandBuffer commandBuffer, VkCudaLaunchInfoNV* pLaunchInfo)
{
    return vkCmdCudaLaunchKernelNV_volkImpl(commandBuffer, pLaunchInfo);
}

void ACTIAS_ABI vkCmdBeginRendering(VkCommandBuffer commandBuffer, VkRenderingInfo* pRenderingInfo)
{
    return vkCmdBeginRendering_volkImpl(commandBuffer, pRenderingInfo);
}

void ACTIAS_ABI vkCmdEndRendering(VkCommandBuffer commandBuffer)
{
    return vkCmdEndRendering_volkImpl(commandBuffer);
}

void ACTIAS_ABI vkGetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping)
{
    return vkGetDescriptorSetLayoutHostMappingInfoVALVE_volkImpl(device, pBindingReference, pHostMapping);
}

void ACTIAS_ABI vkGetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void* ppData)
{
    return vkGetDescriptorSetHostMappingVALVE_volkImpl(device, descriptorSet, ppData);
}

VkResult ACTIAS_ABI vkCreateMicromapEXT(VkDevice device, VkMicromapCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkMicromapEXT* pMicromap)
{
    return vkCreateMicromapEXT_volkImpl(device, pCreateInfo, pAllocator, pMicromap);
}

void ACTIAS_ABI vkCmdBuildMicromapsEXT(VkCommandBuffer commandBuffer, uint32_t infoCount, VkMicromapBuildInfoEXT* pInfos)
{
    return vkCmdBuildMicromapsEXT_volkImpl(commandBuffer, infoCount, pInfos);
}

VkResult ACTIAS_ABI vkBuildMicromapsEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, VkMicromapBuildInfoEXT* pInfos)
{
    return vkBuildMicromapsEXT_volkImpl(device, deferredOperation, infoCount, pInfos);
}

void ACTIAS_ABI vkDestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyMicromapEXT_volkImpl(device, micromap, pAllocator);
}

void ACTIAS_ABI vkCmdCopyMicromapEXT(VkCommandBuffer commandBuffer, VkCopyMicromapInfoEXT* pInfo)
{
    return vkCmdCopyMicromapEXT_volkImpl(commandBuffer, pInfo);
}

VkResult ACTIAS_ABI vkCopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMicromapInfoEXT* pInfo)
{
    return vkCopyMicromapEXT_volkImpl(device, deferredOperation, pInfo);
}

void ACTIAS_ABI vkCmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, VkCopyMicromapToMemoryInfoEXT* pInfo)
{
    return vkCmdCopyMicromapToMemoryEXT_volkImpl(commandBuffer, pInfo);
}

VkResult ACTIAS_ABI vkCopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMicromapToMemoryInfoEXT* pInfo)
{
    return vkCopyMicromapToMemoryEXT_volkImpl(device, deferredOperation, pInfo);
}

void ACTIAS_ABI vkCmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, VkCopyMemoryToMicromapInfoEXT* pInfo)
{
    return vkCmdCopyMemoryToMicromapEXT_volkImpl(commandBuffer, pInfo);
}

VkResult ACTIAS_ABI vkCopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMemoryToMicromapInfoEXT* pInfo)
{
    return vkCopyMemoryToMicromapEXT_volkImpl(device, deferredOperation, pInfo);
}

void ACTIAS_ABI vkCmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
    return vkCmdWriteMicromapsPropertiesEXT_volkImpl(commandBuffer, micromapCount, pMicromaps, queryType, queryPool, firstQuery);
}

VkResult ACTIAS_ABI vkWriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, VkMicromapEXT* pMicromaps, VkQueryType queryType, size_t dataSize, void* pData, size_t stride)
{
    return vkWriteMicromapsPropertiesEXT_volkImpl(device, micromapCount, pMicromaps, queryType, dataSize, pData, stride);
}

void ACTIAS_ABI vkGetDeviceMicromapCompatibilityEXT(VkDevice device, VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility)
{
    return vkGetDeviceMicromapCompatibilityEXT_volkImpl(device, pVersionInfo, pCompatibility);
}

void ACTIAS_ABI vkGetMicromapBuildSizesEXT(VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, VkMicromapBuildInfoEXT* pBuildInfo, VkMicromapBuildSizesInfoEXT* pSizeInfo)
{
    return vkGetMicromapBuildSizesEXT_volkImpl(device, buildType, pBuildInfo, pSizeInfo);
}

void ACTIAS_ABI vkGetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier)
{
    return vkGetShaderModuleIdentifierEXT_volkImpl(device, shaderModule, pIdentifier);
}

void ACTIAS_ABI vkGetShaderModuleCreateInfoIdentifierEXT(VkDevice device, VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier)
{
    return vkGetShaderModuleCreateInfoIdentifierEXT_volkImpl(device, pCreateInfo, pIdentifier);
}

void ACTIAS_ABI vkGetImageSubresourceLayout2KHR(VkDevice device, VkImage image, VkImageSubresource2KHR* pSubresource, VkSubresourceLayout2KHR* pLayout)
{
    return vkGetImageSubresourceLayout2KHR_volkImpl(device, image, pSubresource, pLayout);
}

VkResult ACTIAS_ABI vkGetPipelinePropertiesEXT(VkDevice device, VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties)
{
    return vkGetPipelinePropertiesEXT_volkImpl(device, pPipelineInfo, pPipelineProperties);
}

void ACTIAS_ABI vkExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo)
{
    return vkExportMetalObjectsEXT_volkImpl(device, pMetalObjectsInfo);
}

VkResult ACTIAS_ABI vkGetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties)
{
    return vkGetFramebufferTilePropertiesQCOM_volkImpl(device, framebuffer, pPropertiesCount, pProperties);
}

VkResult ACTIAS_ABI vkGetDynamicRenderingTilePropertiesQCOM(VkDevice device, VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties)
{
    return vkGetDynamicRenderingTilePropertiesQCOM_volkImpl(device, pRenderingInfo, pProperties);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceOpticalFlowImageFormatsNV(VkPhysicalDevice physicalDevice, VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties)
{
    return vkGetPhysicalDeviceOpticalFlowImageFormatsNV_volkImpl(physicalDevice, pOpticalFlowImageFormatInfo, pFormatCount, pImageFormatProperties);
}

VkResult ACTIAS_ABI vkCreateOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession)
{
    return vkCreateOpticalFlowSessionNV_volkImpl(device, pCreateInfo, pAllocator, pSession);
}

void ACTIAS_ABI vkDestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyOpticalFlowSessionNV_volkImpl(device, session, pAllocator);
}

VkResult ACTIAS_ABI vkBindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout)
{
    return vkBindOpticalFlowSessionImageNV_volkImpl(device, session, bindingPoint, view, layout);
}

void ACTIAS_ABI vkCmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, VkOpticalFlowExecuteInfoNV* pExecuteInfo)
{
    return vkCmdOpticalFlowExecuteNV_volkImpl(commandBuffer, session, pExecuteInfo);
}

VkResult ACTIAS_ABI vkGetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo)
{
    return vkGetDeviceFaultInfoEXT_volkImpl(device, pFaultCounts, pFaultInfo);
}

void ACTIAS_ABI vkCmdSetDepthBias2EXT(VkCommandBuffer commandBuffer, VkDepthBiasInfoEXT* pDepthBiasInfo)
{
    return vkCmdSetDepthBias2EXT_volkImpl(commandBuffer, pDepthBiasInfo);
}

VkResult ACTIAS_ABI vkReleaseSwapchainImagesEXT(VkDevice device, VkReleaseSwapchainImagesInfoEXT* pReleaseInfo)
{
    return vkReleaseSwapchainImagesEXT_volkImpl(device, pReleaseInfo);
}

void ACTIAS_ABI vkGetDeviceImageSubresourceLayoutKHR(VkDevice device, VkDeviceImageSubresourceInfoKHR* pInfo, VkSubresourceLayout2KHR* pLayout)
{
    return vkGetDeviceImageSubresourceLayoutKHR_volkImpl(device, pInfo, pLayout);
}

VkResult ACTIAS_ABI vkMapMemory2KHR(VkDevice device, VkMemoryMapInfoKHR* pMemoryMapInfo, void* ppData)
{
    return vkMapMemory2KHR_volkImpl(device, pMemoryMapInfo, ppData);
}

VkResult ACTIAS_ABI vkUnmapMemory2KHR(VkDevice device, VkMemoryUnmapInfoKHR* pMemoryUnmapInfo)
{
    return vkUnmapMemory2KHR_volkImpl(device, pMemoryUnmapInfo);
}

VkResult ACTIAS_ABI vkCreateShadersEXT(VkDevice device, uint32_t createInfoCount, VkShaderCreateInfoEXT* pCreateInfos, VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders)
{
    return vkCreateShadersEXT_volkImpl(device, createInfoCount, pCreateInfos, pAllocator, pShaders);
}

void ACTIAS_ABI vkDestroyShaderEXT(VkDevice device, VkShaderEXT shader, VkAllocationCallbacks* pAllocator)
{
    return vkDestroyShaderEXT_volkImpl(device, shader, pAllocator);
}

VkResult ACTIAS_ABI vkGetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData)
{
    return vkGetShaderBinaryDataEXT_volkImpl(device, shader, pDataSize, pData);
}

void ACTIAS_ABI vkCmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, VkShaderStageFlagBits* pStages, VkShaderEXT* pShaders)
{
    return vkCmdBindShadersEXT_volkImpl(commandBuffer, stageCount, pStages, pShaders);
}

VkResult ACTIAS_ABI vkGetScreenBufferPropertiesQNX(VkDevice device, _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties)
{
    return vkGetScreenBufferPropertiesQNX_volkImpl(device, buffer, pProperties);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties)
{
    return vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR_volkImpl(physicalDevice, pPropertyCount, pProperties);
}

VkResult ACTIAS_ABI vkGetExecutionGraphPipelineScratchSizeAMDX(VkDevice device, VkPipeline executionGraph, VkExecutionGraphPipelineScratchSizeAMDX* pSizeInfo)
{
    return vkGetExecutionGraphPipelineScratchSizeAMDX_volkImpl(device, executionGraph, pSizeInfo);
}

VkResult ACTIAS_ABI vkGetExecutionGraphPipelineNodeIndexAMDX(VkDevice device, VkPipeline executionGraph, VkPipelineShaderStageNodeCreateInfoAMDX* pNodeInfo, uint32_t* pNodeIndex)
{
    return vkGetExecutionGraphPipelineNodeIndexAMDX_volkImpl(device, executionGraph, pNodeInfo, pNodeIndex);
}

VkResult ACTIAS_ABI vkCreateExecutionGraphPipelinesAMDX(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkExecutionGraphPipelineCreateInfoAMDX* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    return vkCreateExecutionGraphPipelinesAMDX_volkImpl(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

void ACTIAS_ABI vkCmdInitializeGraphScratchMemoryAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch)
{
    return vkCmdInitializeGraphScratchMemoryAMDX_volkImpl(commandBuffer, scratch);
}

void ACTIAS_ABI vkCmdDispatchGraphAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDispatchGraphCountInfoAMDX* pCountInfo)
{
    return vkCmdDispatchGraphAMDX_volkImpl(commandBuffer, scratch, pCountInfo);
}

void ACTIAS_ABI vkCmdDispatchGraphIndirectAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDispatchGraphCountInfoAMDX* pCountInfo)
{
    return vkCmdDispatchGraphIndirectAMDX_volkImpl(commandBuffer, scratch, pCountInfo);
}

void ACTIAS_ABI vkCmdDispatchGraphIndirectCountAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDeviceAddress countInfo)
{
    return vkCmdDispatchGraphIndirectCountAMDX_volkImpl(commandBuffer, scratch, countInfo);
}

void ACTIAS_ABI vkCmdBindDescriptorSets2KHR(VkCommandBuffer commandBuffer, VkBindDescriptorSetsInfoKHR* pBindDescriptorSetsInfo)
{
    return vkCmdBindDescriptorSets2KHR_volkImpl(commandBuffer, pBindDescriptorSetsInfo);
}

void ACTIAS_ABI vkCmdPushConstants2KHR(VkCommandBuffer commandBuffer, VkPushConstantsInfoKHR* pPushConstantsInfo)
{
    return vkCmdPushConstants2KHR_volkImpl(commandBuffer, pPushConstantsInfo);
}

void ACTIAS_ABI vkCmdPushDescriptorSet2KHR(VkCommandBuffer commandBuffer, VkPushDescriptorSetInfoKHR* pPushDescriptorSetInfo)
{
    return vkCmdPushDescriptorSet2KHR_volkImpl(commandBuffer, pPushDescriptorSetInfo);
}

void ACTIAS_ABI vkCmdPushDescriptorSetWithTemplate2KHR(VkCommandBuffer commandBuffer, VkPushDescriptorSetWithTemplateInfoKHR* pPushDescriptorSetWithTemplateInfo)
{
    return vkCmdPushDescriptorSetWithTemplate2KHR_volkImpl(commandBuffer, pPushDescriptorSetWithTemplateInfo);
}

void ACTIAS_ABI vkCmdSetDescriptorBufferOffsets2EXT(VkCommandBuffer commandBuffer, VkSetDescriptorBufferOffsetsInfoEXT* pSetDescriptorBufferOffsetsInfo)
{
    return vkCmdSetDescriptorBufferOffsets2EXT_volkImpl(commandBuffer, pSetDescriptorBufferOffsetsInfo);
}

void ACTIAS_ABI vkCmdBindDescriptorBufferEmbeddedSamplers2EXT(VkCommandBuffer commandBuffer, VkBindDescriptorBufferEmbeddedSamplersInfoEXT* pBindDescriptorBufferEmbeddedSamplersInfo)
{
    return vkCmdBindDescriptorBufferEmbeddedSamplers2EXT_volkImpl(commandBuffer, pBindDescriptorBufferEmbeddedSamplersInfo);
}

VkResult ACTIAS_ABI vkSetLatencySleepModeNV(VkDevice device, VkSwapchainKHR swapchain, VkLatencySleepModeInfoNV* pSleepModeInfo)
{
    return vkSetLatencySleepModeNV_volkImpl(device, swapchain, pSleepModeInfo);
}

VkResult ACTIAS_ABI vkLatencySleepNV(VkDevice device, VkSwapchainKHR swapchain, VkLatencySleepInfoNV* pSleepInfo)
{
    return vkLatencySleepNV_volkImpl(device, swapchain, pSleepInfo);
}

void ACTIAS_ABI vkSetLatencyMarkerNV(VkDevice device, VkSwapchainKHR swapchain, VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo)
{
    return vkSetLatencyMarkerNV_volkImpl(device, swapchain, pLatencyMarkerInfo);
}

void ACTIAS_ABI vkGetLatencyTimingsNV(VkDevice device, VkSwapchainKHR swapchain, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo)
{
    return vkGetLatencyTimingsNV_volkImpl(device, swapchain, pLatencyMarkerInfo);
}

void ACTIAS_ABI vkQueueNotifyOutOfBandNV(VkQueue queue, VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo)
{
    return vkQueueNotifyOutOfBandNV_volkImpl(queue, pQueueTypeInfo);
}

void ACTIAS_ABI vkCmdSetRenderingAttachmentLocationsKHR(VkCommandBuffer commandBuffer, VkRenderingAttachmentLocationInfoKHR* pLocationInfo)
{
    return vkCmdSetRenderingAttachmentLocationsKHR_volkImpl(commandBuffer, pLocationInfo);
}

void ACTIAS_ABI vkCmdSetRenderingInputAttachmentIndicesKHR(VkCommandBuffer commandBuffer, VkRenderingInputAttachmentIndexInfoKHR* pLocationInfo)
{
    return vkCmdSetRenderingInputAttachmentIndicesKHR_volkImpl(commandBuffer, pLocationInfo);
}

