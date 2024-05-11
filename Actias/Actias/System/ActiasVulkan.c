#include <Actias/System/ActiasVulkan.h>

VkResult ACTIAS_ABI vkCreateInstance(VkInstanceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    return vkCreateInstance_volkImpl(pCreateInfo, pAllocator, pInstance);
}

void ACTIAS_ABI vkDestroyInstance(VkInstance instance, VkAllocationCallbacks* pAllocator)
{
    vkDestroyInstance_volkImpl(instance, pAllocator);
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
    vkGetPhysicalDeviceProperties_volkImpl(physicalDevice, pProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties)
{
    vkGetPhysicalDeviceQueueFamilyProperties_volkImpl(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties)
{
    vkGetPhysicalDeviceMemoryProperties_volkImpl(physicalDevice, pMemoryProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures)
{
    vkGetPhysicalDeviceFeatures_volkImpl(physicalDevice, pFeatures);
}

void ACTIAS_ABI vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties)
{
    vkGetPhysicalDeviceFormatProperties_volkImpl(physicalDevice, format, pFormatProperties);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties)
{
    return vkGetPhysicalDeviceImageFormatProperties_volkImpl(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}

VkResult ACTIAS_ABI vkCreateDevice(VkPhysicalDevice physicalDevice, VkDeviceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    return vkCreateDevice_volkImpl(physicalDevice, pCreateInfo, pAllocator, pDevice);
}

void ACTIAS_ABI vkDestroyDevice(VkDevice device, VkAllocationCallbacks* pAllocator)
{
    vkDestroyDevice_volkImpl(device, pAllocator);
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

VkResult ACTIAS_ABI vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    return vkEnumerateDeviceExtensionProperties_volkImpl(physicalDevice, pLayerName, pPropertyCount, pProperties);
}

void ACTIAS_ABI vkGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
    vkGetDeviceQueue_volkImpl(device, queueFamilyIndex, queueIndex, pQueue);
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
    vkFreeMemory_volkImpl(device, memory, pAllocator);
}

VkResult ACTIAS_ABI vkMapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void* ppData)
{
    return vkMapMemory_volkImpl(device, memory, offset, size, flags, ppData);
}

void ACTIAS_ABI vkUnmapMemory(VkDevice device, VkDeviceMemory memory)
{
    vkUnmapMemory_volkImpl(device, memory);
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
    vkGetDeviceMemoryCommitment_volkImpl(device, memory, pCommittedMemoryInBytes);
}

void ACTIAS_ABI vkGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
{
    vkGetBufferMemoryRequirements_volkImpl(device, buffer, pMemoryRequirements);
}

VkResult ACTIAS_ABI vkBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    return vkBindBufferMemory_volkImpl(device, buffer, memory, memoryOffset);
}

void ACTIAS_ABI vkGetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements)
{
    vkGetImageMemoryRequirements_volkImpl(device, image, pMemoryRequirements);
}

VkResult ACTIAS_ABI vkBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    return vkBindImageMemory_volkImpl(device, image, memory, memoryOffset);
}

void ACTIAS_ABI vkGetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements)
{
    vkGetImageSparseMemoryRequirements_volkImpl(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

void ACTIAS_ABI vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties)
{
    vkGetPhysicalDeviceSparseImageFormatProperties_volkImpl(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
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
    vkDestroyFence_volkImpl(device, fence, pAllocator);
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
    vkDestroySemaphore_volkImpl(device, semaphore, pAllocator);
}

VkResult ACTIAS_ABI vkCreateEvent(VkDevice device, VkEventCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    return vkCreateEvent_volkImpl(device, pCreateInfo, pAllocator, pEvent);
}

void ACTIAS_ABI vkDestroyEvent(VkDevice device, VkEvent event, VkAllocationCallbacks* pAllocator)
{
    vkDestroyEvent_volkImpl(device, event, pAllocator);
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
    vkDestroyQueryPool_volkImpl(device, queryPool, pAllocator);
}

VkResult ACTIAS_ABI vkGetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
{
    return vkGetQueryPoolResults_volkImpl(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}

void ACTIAS_ABI vkResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    vkResetQueryPool_volkImpl(device, queryPool, firstQuery, queryCount);
}

VkResult ACTIAS_ABI vkCreateBuffer(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
    return vkCreateBuffer_volkImpl(device, pCreateInfo, pAllocator, pBuffer);
}

void ACTIAS_ABI vkDestroyBuffer(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator)
{
    vkDestroyBuffer_volkImpl(device, buffer, pAllocator);
}

VkResult ACTIAS_ABI vkCreateBufferView(VkDevice device, VkBufferViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBufferView* pView)
{
    return vkCreateBufferView_volkImpl(device, pCreateInfo, pAllocator, pView);
}

void ACTIAS_ABI vkDestroyBufferView(VkDevice device, VkBufferView bufferView, VkAllocationCallbacks* pAllocator)
{
    vkDestroyBufferView_volkImpl(device, bufferView, pAllocator);
}

VkResult ACTIAS_ABI vkCreateImage(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    return vkCreateImage_volkImpl(device, pCreateInfo, pAllocator, pImage);
}

void ACTIAS_ABI vkDestroyImage(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator)
{
    vkDestroyImage_volkImpl(device, image, pAllocator);
}

void ACTIAS_ABI vkGetImageSubresourceLayout(VkDevice device, VkImage image, VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
{
    vkGetImageSubresourceLayout_volkImpl(device, image, pSubresource, pLayout);
}

VkResult ACTIAS_ABI vkCreateImageView(VkDevice device, VkImageViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImageView* pView)
{
    return vkCreateImageView_volkImpl(device, pCreateInfo, pAllocator, pView);
}

void ACTIAS_ABI vkDestroyImageView(VkDevice device, VkImageView imageView, VkAllocationCallbacks* pAllocator)
{
    vkDestroyImageView_volkImpl(device, imageView, pAllocator);
}

VkResult ACTIAS_ABI vkCreateShaderModule(VkDevice device, VkShaderModuleCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
{
    return vkCreateShaderModule_volkImpl(device, pCreateInfo, pAllocator, pShaderModule);
}

void ACTIAS_ABI vkDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, VkAllocationCallbacks* pAllocator)
{
    vkDestroyShaderModule_volkImpl(device, shaderModule, pAllocator);
}

VkResult ACTIAS_ABI vkCreatePipelineCache(VkDevice device, VkPipelineCacheCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
{
    return vkCreatePipelineCache_volkImpl(device, pCreateInfo, pAllocator, pPipelineCache);
}

void ACTIAS_ABI vkDestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, VkAllocationCallbacks* pAllocator)
{
    vkDestroyPipelineCache_volkImpl(device, pipelineCache, pAllocator);
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

VkResult ACTIAS_ABI vkCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkComputePipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    return vkCreateComputePipelines_volkImpl(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}

void ACTIAS_ABI vkDestroyPipeline(VkDevice device, VkPipeline pipeline, VkAllocationCallbacks* pAllocator)
{
    vkDestroyPipeline_volkImpl(device, pipeline, pAllocator);
}

VkResult ACTIAS_ABI vkCreatePipelineLayout(VkDevice device, VkPipelineLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
{
    return vkCreatePipelineLayout_volkImpl(device, pCreateInfo, pAllocator, pPipelineLayout);
}

void ACTIAS_ABI vkDestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, VkAllocationCallbacks* pAllocator)
{
    vkDestroyPipelineLayout_volkImpl(device, pipelineLayout, pAllocator);
}

VkResult ACTIAS_ABI vkCreateSampler(VkDevice device, VkSamplerCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
{
    return vkCreateSampler_volkImpl(device, pCreateInfo, pAllocator, pSampler);
}

void ACTIAS_ABI vkDestroySampler(VkDevice device, VkSampler sampler, VkAllocationCallbacks* pAllocator)
{
    vkDestroySampler_volkImpl(device, sampler, pAllocator);
}

VkResult ACTIAS_ABI vkCreateDescriptorSetLayout(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
{
    return vkCreateDescriptorSetLayout_volkImpl(device, pCreateInfo, pAllocator, pSetLayout);
}

void ACTIAS_ABI vkDestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, VkAllocationCallbacks* pAllocator)
{
    vkDestroyDescriptorSetLayout_volkImpl(device, descriptorSetLayout, pAllocator);
}

VkResult ACTIAS_ABI vkCreateDescriptorPool(VkDevice device, VkDescriptorPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
{
    return vkCreateDescriptorPool_volkImpl(device, pCreateInfo, pAllocator, pDescriptorPool);
}

void ACTIAS_ABI vkDestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkAllocationCallbacks* pAllocator)
{
    vkDestroyDescriptorPool_volkImpl(device, descriptorPool, pAllocator);
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
    vkUpdateDescriptorSets_volkImpl(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}

VkResult ACTIAS_ABI vkCreateFramebuffer(VkDevice device, VkFramebufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
{
    return vkCreateFramebuffer_volkImpl(device, pCreateInfo, pAllocator, pFramebuffer);
}

void ACTIAS_ABI vkDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, VkAllocationCallbacks* pAllocator)
{
    vkDestroyFramebuffer_volkImpl(device, framebuffer, pAllocator);
}

VkResult ACTIAS_ABI vkCreateRenderPass(VkDevice device, VkRenderPassCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    return vkCreateRenderPass_volkImpl(device, pCreateInfo, pAllocator, pRenderPass);
}

void ACTIAS_ABI vkDestroyRenderPass(VkDevice device, VkRenderPass renderPass, VkAllocationCallbacks* pAllocator)
{
    vkDestroyRenderPass_volkImpl(device, renderPass, pAllocator);
}

void ACTIAS_ABI vkGetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity)
{
    vkGetRenderAreaGranularity_volkImpl(device, renderPass, pGranularity);
}

VkResult ACTIAS_ABI vkCreateCommandPool(VkDevice device, VkCommandPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
    return vkCreateCommandPool_volkImpl(device, pCreateInfo, pAllocator, pCommandPool);
}

void ACTIAS_ABI vkDestroyCommandPool(VkDevice device, VkCommandPool commandPool, VkAllocationCallbacks* pAllocator)
{
    vkDestroyCommandPool_volkImpl(device, commandPool, pAllocator);
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
    vkFreeCommandBuffers_volkImpl(device, commandPool, commandBufferCount, pCommandBuffers);
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
    vkCmdBindPipeline_volkImpl(commandBuffer, pipelineBindPoint, pipeline);
}

void ACTIAS_ABI vkCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewport* pViewports)
{
    vkCmdSetViewport_volkImpl(commandBuffer, firstViewport, viewportCount, pViewports);
}

void ACTIAS_ABI vkCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, VkRect2D* pScissors)
{
    vkCmdSetScissor_volkImpl(commandBuffer, firstScissor, scissorCount, pScissors);
}

void ACTIAS_ABI vkCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth)
{
    vkCmdSetLineWidth_volkImpl(commandBuffer, lineWidth);
}

void ACTIAS_ABI vkCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
    vkCmdSetDepthBias_volkImpl(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

void ACTIAS_ABI vkCmdSetBlendConstants(VkCommandBuffer commandBuffer, float* blendConstants)
{
    vkCmdSetBlendConstants_volkImpl(commandBuffer, blendConstants);
}

void ACTIAS_ABI vkCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
    vkCmdSetDepthBounds_volkImpl(commandBuffer, minDepthBounds, maxDepthBounds);
}

void ACTIAS_ABI vkCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
{
    vkCmdSetStencilCompareMask_volkImpl(commandBuffer, faceMask, compareMask);
}

void ACTIAS_ABI vkCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
{
    vkCmdSetStencilWriteMask_volkImpl(commandBuffer, faceMask, writeMask);
}

void ACTIAS_ABI vkCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
{
    vkCmdSetStencilReference_volkImpl(commandBuffer, faceMask, reference);
}

void ACTIAS_ABI vkCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, uint32_t* pDynamicOffsets)
{
    vkCmdBindDescriptorSets_volkImpl(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

void ACTIAS_ABI vkCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
    vkCmdBindIndexBuffer_volkImpl(commandBuffer, buffer, offset, indexType);
}

void ACTIAS_ABI vkCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets)
{
    vkCmdBindVertexBuffers_volkImpl(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

void ACTIAS_ABI vkCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    vkCmdDraw_volkImpl(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void ACTIAS_ABI vkCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
    vkCmdDrawIndexed_volkImpl(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void ACTIAS_ABI vkCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    vkCmdDrawIndirect_volkImpl(commandBuffer, buffer, offset, drawCount, stride);
}

void ACTIAS_ABI vkCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    vkCmdDrawIndexedIndirect_volkImpl(commandBuffer, buffer, offset, drawCount, stride);
}

void ACTIAS_ABI vkCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    vkCmdDispatch_volkImpl(commandBuffer, groupCountX, groupCountY, groupCountZ);
}

void ACTIAS_ABI vkCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
    vkCmdDispatchIndirect_volkImpl(commandBuffer, buffer, offset);
}

void ACTIAS_ABI vkCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions)
{
    vkCmdCopyBuffer_volkImpl(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

void ACTIAS_ABI vkCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions)
{
    vkCmdCopyImage_volkImpl(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void ACTIAS_ABI vkCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageBlit* pRegions, VkFilter filter)
{
    vkCmdBlitImage_volkImpl(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}

void ACTIAS_ABI vkCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions)
{
    vkCmdCopyBufferToImage_volkImpl(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

void ACTIAS_ABI vkCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions)
{
    vkCmdCopyImageToBuffer_volkImpl(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

void ACTIAS_ABI vkCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, void* pData)
{
    vkCmdUpdateBuffer_volkImpl(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}

void ACTIAS_ABI vkCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
{
    vkCmdFillBuffer_volkImpl(commandBuffer, dstBuffer, dstOffset, size, data);
}

void ACTIAS_ABI vkCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearColorValue* pColor, uint32_t rangeCount, VkImageSubresourceRange* pRanges)
{
    vkCmdClearColorImage_volkImpl(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}

void ACTIAS_ABI vkCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, VkImageSubresourceRange* pRanges)
{
    vkCmdClearDepthStencilImage_volkImpl(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}

void ACTIAS_ABI vkCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, VkClearAttachment* pAttachments, uint32_t rectCount, VkClearRect* pRects)
{
    vkCmdClearAttachments_volkImpl(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}

void ACTIAS_ABI vkCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageResolve* pRegions)
{
    vkCmdResolveImage_volkImpl(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void ACTIAS_ABI vkCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    vkCmdSetEvent_volkImpl(commandBuffer, event, stageMask);
}

void ACTIAS_ABI vkCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    vkCmdResetEvent_volkImpl(commandBuffer, event, stageMask);
}

void ACTIAS_ABI vkCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers)
{
    vkCmdWaitEvents_volkImpl(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void ACTIAS_ABI vkCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers)
{
    vkCmdPipelineBarrier_volkImpl(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void ACTIAS_ABI vkCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
{
    vkCmdBeginQuery_volkImpl(commandBuffer, queryPool, query, flags);
}

void ACTIAS_ABI vkCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
{
    vkCmdEndQuery_volkImpl(commandBuffer, queryPool, query);
}

void ACTIAS_ABI vkCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    vkCmdResetQueryPool_volkImpl(commandBuffer, queryPool, firstQuery, queryCount);
}

void ACTIAS_ABI vkCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
{
    vkCmdWriteTimestamp_volkImpl(commandBuffer, pipelineStage, queryPool, query);
}

void ACTIAS_ABI vkCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
{
    vkCmdCopyQueryPoolResults_volkImpl(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}

void ACTIAS_ABI vkCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, void* pValues)
{
    vkCmdPushConstants_volkImpl(commandBuffer, layout, stageFlags, offset, size, pValues);
}

void ACTIAS_ABI vkCmdBeginRenderPass(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents)
{
    vkCmdBeginRenderPass_volkImpl(commandBuffer, pRenderPassBegin, contents);
}

void ACTIAS_ABI vkCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents)
{
    vkCmdNextSubpass_volkImpl(commandBuffer, contents);
}

void ACTIAS_ABI vkCmdEndRenderPass(VkCommandBuffer commandBuffer)
{
    vkCmdEndRenderPass_volkImpl(commandBuffer);
}

void ACTIAS_ABI vkCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers)
{
    vkCmdExecuteCommands_volkImpl(commandBuffer, commandBufferCount, pCommandBuffers);
}

void ACTIAS_ABI vkGetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
    vkGetPhysicalDeviceFeatures2_volkImpl(physicalDevice, pFeatures);
}

void ACTIAS_ABI vkGetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
    vkGetPhysicalDeviceProperties2_volkImpl(physicalDevice, pProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
    vkGetPhysicalDeviceFormatProperties2_volkImpl(physicalDevice, format, pFormatProperties);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
    return vkGetPhysicalDeviceImageFormatProperties2_volkImpl(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties)
{
    vkGetPhysicalDeviceQueueFamilyProperties2_volkImpl(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
    vkGetPhysicalDeviceMemoryProperties2_volkImpl(physicalDevice, pMemoryProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties)
{
    vkGetPhysicalDeviceSparseImageFormatProperties2_volkImpl(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
}

void ACTIAS_ABI vkTrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
    vkTrimCommandPool_volkImpl(device, commandPool, flags);
}

void ACTIAS_ABI vkGetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
    vkGetPhysicalDeviceExternalBufferProperties_volkImpl(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
    vkGetPhysicalDeviceExternalSemaphoreProperties_volkImpl(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

void ACTIAS_ABI vkGetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
    vkGetPhysicalDeviceExternalFenceProperties_volkImpl(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}

VkResult ACTIAS_ABI vkEnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties)
{
    return vkEnumeratePhysicalDeviceGroups_volkImpl(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}

void ACTIAS_ABI vkGetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
    vkGetDeviceGroupPeerMemoryFeatures_volkImpl(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
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
    vkCmdSetDeviceMask_volkImpl(commandBuffer, deviceMask);
}

void ACTIAS_ABI vkCmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    vkCmdDispatchBase_volkImpl(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

VkResult ACTIAS_ABI vkCreateDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
    return vkCreateDescriptorUpdateTemplate_volkImpl(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}

void ACTIAS_ABI vkDestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkAllocationCallbacks* pAllocator)
{
    vkDestroyDescriptorUpdateTemplate_volkImpl(device, descriptorUpdateTemplate, pAllocator);
}

void ACTIAS_ABI vkUpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, void* pData)
{
    vkUpdateDescriptorSetWithTemplate_volkImpl(device, descriptorSet, descriptorUpdateTemplate, pData);
}

void ACTIAS_ABI vkGetBufferMemoryRequirements2(VkDevice device, VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    vkGetBufferMemoryRequirements2_volkImpl(device, pInfo, pMemoryRequirements);
}

void ACTIAS_ABI vkGetImageMemoryRequirements2(VkDevice device, VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    vkGetImageMemoryRequirements2_volkImpl(device, pInfo, pMemoryRequirements);
}

void ACTIAS_ABI vkGetImageSparseMemoryRequirements2(VkDevice device, VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    vkGetImageSparseMemoryRequirements2_volkImpl(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

void ACTIAS_ABI vkGetDeviceBufferMemoryRequirements(VkDevice device, VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    vkGetDeviceBufferMemoryRequirements_volkImpl(device, pInfo, pMemoryRequirements);
}

void ACTIAS_ABI vkGetDeviceImageMemoryRequirements(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    vkGetDeviceImageMemoryRequirements_volkImpl(device, pInfo, pMemoryRequirements);
}

void ACTIAS_ABI vkGetDeviceImageSparseMemoryRequirements(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    vkGetDeviceImageSparseMemoryRequirements_volkImpl(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

VkResult ACTIAS_ABI vkCreateSamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
    return vkCreateSamplerYcbcrConversion_volkImpl(device, pCreateInfo, pAllocator, pYcbcrConversion);
}

void ACTIAS_ABI vkDestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, VkAllocationCallbacks* pAllocator)
{
    vkDestroySamplerYcbcrConversion_volkImpl(device, ycbcrConversion, pAllocator);
}

void ACTIAS_ABI vkGetDeviceQueue2(VkDevice device, VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue)
{
    vkGetDeviceQueue2_volkImpl(device, pQueueInfo, pQueue);
}

void ACTIAS_ABI vkGetDescriptorSetLayoutSupport(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
    vkGetDescriptorSetLayoutSupport_volkImpl(device, pCreateInfo, pSupport);
}

VkResult ACTIAS_ABI vkCreateRenderPass2(VkDevice device, VkRenderPassCreateInfo2* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    return vkCreateRenderPass2_volkImpl(device, pCreateInfo, pAllocator, pRenderPass);
}

void ACTIAS_ABI vkCmdBeginRenderPass2(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassBeginInfo* pSubpassBeginInfo)
{
    vkCmdBeginRenderPass2_volkImpl(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}

void ACTIAS_ABI vkCmdNextSubpass2(VkCommandBuffer commandBuffer, VkSubpassBeginInfo* pSubpassBeginInfo, VkSubpassEndInfo* pSubpassEndInfo)
{
    vkCmdNextSubpass2_volkImpl(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void ACTIAS_ABI vkCmdEndRenderPass2(VkCommandBuffer commandBuffer, VkSubpassEndInfo* pSubpassEndInfo)
{
    vkCmdEndRenderPass2_volkImpl(commandBuffer, pSubpassEndInfo);
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

void ACTIAS_ABI vkCmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    vkCmdDrawIndirectCount_volkImpl(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void ACTIAS_ABI vkCmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    vkCmdDrawIndexedIndirectCount_volkImpl(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

uint64_t ACTIAS_ABI vkGetBufferOpaqueCaptureAddress(VkDevice device, VkBufferDeviceAddressInfo* pInfo)
{
    return vkGetBufferOpaqueCaptureAddress_volkImpl(device, pInfo);
}

VkDeviceAddress ACTIAS_ABI vkGetBufferDeviceAddress(VkDevice device, VkBufferDeviceAddressInfo* pInfo)
{
    return vkGetBufferDeviceAddress_volkImpl(device, pInfo);
}

uint64_t ACTIAS_ABI vkGetDeviceMemoryOpaqueCaptureAddress(VkDevice device, VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo)
{
    return vkGetDeviceMemoryOpaqueCaptureAddress_volkImpl(device, pInfo);
}

VkResult ACTIAS_ABI vkGetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties)
{
    return vkGetPhysicalDeviceToolProperties_volkImpl(physicalDevice, pToolCount, pToolProperties);
}

void ACTIAS_ABI vkCmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode)
{
    vkCmdSetCullMode_volkImpl(commandBuffer, cullMode);
}

void ACTIAS_ABI vkCmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace)
{
    vkCmdSetFrontFace_volkImpl(commandBuffer, frontFace);
}

void ACTIAS_ABI vkCmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology)
{
    vkCmdSetPrimitiveTopology_volkImpl(commandBuffer, primitiveTopology);
}

void ACTIAS_ABI vkCmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, VkViewport* pViewports)
{
    vkCmdSetViewportWithCount_volkImpl(commandBuffer, viewportCount, pViewports);
}

void ACTIAS_ABI vkCmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, VkRect2D* pScissors)
{
    vkCmdSetScissorWithCount_volkImpl(commandBuffer, scissorCount, pScissors);
}

void ACTIAS_ABI vkCmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets, VkDeviceSize* pSizes, VkDeviceSize* pStrides)
{
    vkCmdBindVertexBuffers2_volkImpl(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}

void ACTIAS_ABI vkCmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable)
{
    vkCmdSetDepthTestEnable_volkImpl(commandBuffer, depthTestEnable);
}

void ACTIAS_ABI vkCmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable)
{
    vkCmdSetDepthWriteEnable_volkImpl(commandBuffer, depthWriteEnable);
}

void ACTIAS_ABI vkCmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp)
{
    vkCmdSetDepthCompareOp_volkImpl(commandBuffer, depthCompareOp);
}

void ACTIAS_ABI vkCmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable)
{
    vkCmdSetDepthBoundsTestEnable_volkImpl(commandBuffer, depthBoundsTestEnable);
}

void ACTIAS_ABI vkCmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable)
{
    vkCmdSetStencilTestEnable_volkImpl(commandBuffer, stencilTestEnable);
}

void ACTIAS_ABI vkCmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp)
{
    vkCmdSetStencilOp_volkImpl(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}

void ACTIAS_ABI vkCmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable)
{
    vkCmdSetRasterizerDiscardEnable_volkImpl(commandBuffer, rasterizerDiscardEnable);
}

void ACTIAS_ABI vkCmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable)
{
    vkCmdSetDepthBiasEnable_volkImpl(commandBuffer, depthBiasEnable);
}

void ACTIAS_ABI vkCmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable)
{
    vkCmdSetPrimitiveRestartEnable_volkImpl(commandBuffer, primitiveRestartEnable);
}

VkResult ACTIAS_ABI vkCreatePrivateDataSlot(VkDevice device, VkPrivateDataSlotCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot)
{
    return vkCreatePrivateDataSlot_volkImpl(device, pCreateInfo, pAllocator, pPrivateDataSlot);
}

void ACTIAS_ABI vkDestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, VkAllocationCallbacks* pAllocator)
{
    vkDestroyPrivateDataSlot_volkImpl(device, privateDataSlot, pAllocator);
}

VkResult ACTIAS_ABI vkSetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data)
{
    return vkSetPrivateData_volkImpl(device, objectType, objectHandle, privateDataSlot, data);
}

void ACTIAS_ABI vkGetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData)
{
    vkGetPrivateData_volkImpl(device, objectType, objectHandle, privateDataSlot, pData);
}

void ACTIAS_ABI vkCmdCopyBuffer2(VkCommandBuffer commandBuffer, VkCopyBufferInfo2* pCopyBufferInfo)
{
    vkCmdCopyBuffer2_volkImpl(commandBuffer, pCopyBufferInfo);
}

void ACTIAS_ABI vkCmdCopyImage2(VkCommandBuffer commandBuffer, VkCopyImageInfo2* pCopyImageInfo)
{
    vkCmdCopyImage2_volkImpl(commandBuffer, pCopyImageInfo);
}

void ACTIAS_ABI vkCmdBlitImage2(VkCommandBuffer commandBuffer, VkBlitImageInfo2* pBlitImageInfo)
{
    vkCmdBlitImage2_volkImpl(commandBuffer, pBlitImageInfo);
}

void ACTIAS_ABI vkCmdCopyBufferToImage2(VkCommandBuffer commandBuffer, VkCopyBufferToImageInfo2* pCopyBufferToImageInfo)
{
    vkCmdCopyBufferToImage2_volkImpl(commandBuffer, pCopyBufferToImageInfo);
}

void ACTIAS_ABI vkCmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, VkCopyImageToBufferInfo2* pCopyImageToBufferInfo)
{
    vkCmdCopyImageToBuffer2_volkImpl(commandBuffer, pCopyImageToBufferInfo);
}

void ACTIAS_ABI vkCmdResolveImage2(VkCommandBuffer commandBuffer, VkResolveImageInfo2* pResolveImageInfo)
{
    vkCmdResolveImage2_volkImpl(commandBuffer, pResolveImageInfo);
}

void ACTIAS_ABI vkCmdSetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkDependencyInfo* pDependencyInfo)
{
    vkCmdSetEvent2_volkImpl(commandBuffer, event, pDependencyInfo);
}

void ACTIAS_ABI vkCmdResetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask)
{
    vkCmdResetEvent2_volkImpl(commandBuffer, event, stageMask);
}

void ACTIAS_ABI vkCmdWaitEvents2(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkDependencyInfo* pDependencyInfos)
{
    vkCmdWaitEvents2_volkImpl(commandBuffer, eventCount, pEvents, pDependencyInfos);
}

void ACTIAS_ABI vkCmdPipelineBarrier2(VkCommandBuffer commandBuffer, VkDependencyInfo* pDependencyInfo)
{
    vkCmdPipelineBarrier2_volkImpl(commandBuffer, pDependencyInfo);
}

VkResult ACTIAS_ABI vkQueueSubmit2(VkQueue queue, uint32_t submitCount, VkSubmitInfo2* pSubmits, VkFence fence)
{
    return vkQueueSubmit2_volkImpl(queue, submitCount, pSubmits, fence);
}

void ACTIAS_ABI vkCmdWriteTimestamp2(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query)
{
    vkCmdWriteTimestamp2_volkImpl(commandBuffer, stage, queryPool, query);
}

void ACTIAS_ABI vkCmdBeginRendering(VkCommandBuffer commandBuffer, VkRenderingInfo* pRenderingInfo)
{
    vkCmdBeginRendering_volkImpl(commandBuffer, pRenderingInfo);
}

void ACTIAS_ABI vkCmdEndRendering(VkCommandBuffer commandBuffer)
{
    vkCmdEndRendering_volkImpl(commandBuffer);
}

