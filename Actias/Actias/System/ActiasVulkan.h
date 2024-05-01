#pragma once
#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateInstance(VkInstanceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkInstance* pInstance);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyInstance(VkInstance instance, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices);

ACTIAS_SYSTEM_API PFN_vkVoidFunction ACTIAS_ABI vkGetDeviceProcAddr(VkDevice device, char* pName);

ACTIAS_SYSTEM_API PFN_vkVoidFunction ACTIAS_ABI vkGetInstanceProcAddr(VkInstance instance, char* pName);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateDevice(VkPhysicalDevice physicalDevice, VkDeviceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDevice* pDevice);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateDevice(VkPhysicalDevice physicalDevice, VkDeviceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDevice* pDevice);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyDevice(VkDevice device, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkEnumerateInstanceVersion(uint32_t* pApiVersion);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkEnumerateInstanceExtensionProperties(char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkQueueSubmit(VkQueue queue, uint32_t submitCount, VkSubmitInfo* pSubmits, VkFence fence);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkQueueWaitIdle(VkQueue queue);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkDeviceWaitIdle(VkDevice device);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkAllocateMemory(VkDevice device, VkMemoryAllocateInfo* pAllocateInfo, VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkFreeMemory(VkDevice device, VkDeviceMemory memory, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkMapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void* ppData);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkUnmapMemory(VkDevice device, VkDeviceMemory memory);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkFlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, VkMappedMemoryRange* pMemoryRanges);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkInvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, VkMappedMemoryRange* pMemoryRanges);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, VkBindSparseInfo* pBindInfo, VkFence fence);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateFence(VkDevice device, VkFenceCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyFence(VkDevice device, VkFence fence, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkResetFences(VkDevice device, uint32_t fenceCount, VkFence* pFences);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetFenceStatus(VkDevice device, VkFence fence);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkWaitForFences(VkDevice device, uint32_t fenceCount, VkFence* pFences, VkBool32 waitAll, uint64_t timeout);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateSemaphore(VkDevice device, VkSemaphoreCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroySemaphore(VkDevice device, VkSemaphore semaphore, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateEvent(VkDevice device, VkEventCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkEvent* pEvent);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyEvent(VkDevice device, VkEvent event, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetEventStatus(VkDevice device, VkEvent event);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkSetEvent(VkDevice device, VkEvent event);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkResetEvent(VkDevice device, VkEvent event);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateQueryPool(VkDevice device, VkQueryPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyQueryPool(VkDevice device, VkQueryPool queryPool, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateBuffer(VkDevice device, VkBufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyBuffer(VkDevice device, VkBuffer buffer, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateBufferView(VkDevice device, VkBufferViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBufferView* pView);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyBufferView(VkDevice device, VkBufferView bufferView, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateImage(VkDevice device, VkImageCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImage* pImage);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyImage(VkDevice device, VkImage image, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetImageSubresourceLayout(VkDevice device, VkImage image, VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateImageView(VkDevice device, VkImageViewCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkImageView* pView);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyImageView(VkDevice device, VkImageView imageView, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateShaderModule(VkDevice device, VkShaderModuleCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreatePipelineCache(VkDevice device, VkPipelineCacheCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreatePipelineCache(VkDevice device, VkPipelineCacheCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkMergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, VkPipelineCache* pSrcCaches);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkGraphicsPipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkGraphicsPipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkComputePipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkComputePipelineCreateInfo* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyPipeline(VkDevice device, VkPipeline pipeline, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreatePipelineLayout(VkDevice device, VkPipelineLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateSampler(VkDevice device, VkSamplerCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSampler* pSampler);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroySampler(VkDevice device, VkSampler sampler, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateDescriptorSetLayout(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateDescriptorPool(VkDevice device, VkDescriptorPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkAllocateDescriptorSets(VkDevice device, VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkFreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkUpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, VkCopyDescriptorSet* pDescriptorCopies);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateFramebuffer(VkDevice device, VkFramebufferCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateRenderPass(VkDevice device, VkRenderPassCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyRenderPass(VkDevice device, VkRenderPass renderPass, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetRenderingAreaGranularityKHR(VkDevice device, VkRenderingAreaInfoKHR* pRenderingAreaInfo, VkExtent2D* pGranularity);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateCommandPool(VkDevice device, VkCommandPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyCommandPool(VkDevice device, VkCommandPool commandPool, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkAllocateCommandBuffers(VkDevice device, VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkBeginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo* pBeginInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkEndCommandBuffer(VkCommandBuffer commandBuffer);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetAttachmentFeedbackLoopEnableEXT(VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewport* pViewports);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, VkRect2D* pScissors);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetBlendConstants(VkCommandBuffer commandBuffer, float blendConstants);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, uint32_t* pDynamicOffsets);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, int32_t* pVertexOffset);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdUpdatePipelineIndirectBufferNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageBlit* pRegions, VkFilter filter);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, VkImageSubresourceLayers* pImageSubresources);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, void* pData);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearColorValue* pColor, uint32_t rangeCount, VkImageSubresourceRange* pRanges);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, VkImageSubresourceRange* pRanges);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, VkClearAttachment* pAttachments, uint32_t rectCount, VkClearRect* pRects);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageResolve* pRegions);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, VkImageMemoryBarrier* pImageMemoryBarriers);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, void* pValues);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBeginRenderPass(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdEndRenderPass(VkCommandBuffer commandBuffer);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateAndroidSurfaceKHR(VkInstance instance, VkAndroidSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, VkDisplayModeCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateDisplayPlaneSurfaceKHR(VkInstance instance, VkDisplaySurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, VkSwapchainCreateInfoKHR* pCreateInfos, VkSwapchainCreateInfoKHR* pCreateInfos, VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateSwapchainKHR(VkDevice device, VkSwapchainCreateInfoKHR* pCreateInfo, VkSwapchainCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkQueuePresentKHR(VkQueue queue, VkPresentInfoKHR* pPresentInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateViSurfaceNN(VkInstance instance, VkViSurfaceCreateInfoNN* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateWaylandSurfaceKHR(VkInstance instance, VkWaylandSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API VkBool32 ACTIAS_ABI vkGetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, wl_display* display);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateWin32SurfaceKHR(VkInstance instance, VkWin32SurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API VkBool32 ACTIAS_ABI vkGetPhysicalDeviceWin32PresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateXlibSurfaceKHR(VkInstance instance, VkXlibSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API VkBool32 ACTIAS_ABI vkGetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateXcbSurfaceKHR(VkInstance instance, VkXcbSurfaceCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API VkBool32 ACTIAS_ABI vkGetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateDirectFBSurfaceEXT(VkInstance instance, VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API VkBool32 ACTIAS_ABI vkGetPhysicalDeviceDirectFBPresentationSupportEXT(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateImagePipeSurfaceFUCHSIA(VkInstance instance, VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateStreamDescriptorSurfaceGGP(VkInstance instance, VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateScreenSurfaceQNX(VkInstance instance, VkScreenSurfaceCreateInfoQNX* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API VkBool32 ACTIAS_ABI vkGetPhysicalDeviceScreenPresentationSupportQNX(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, _screen_window* window);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, char* pLayerPrefix, char* pMessage);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkDebugMarkerSetObjectNameEXT(VkDevice device, VkDebugMarkerObjectNameInfoEXT* pNameInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkDebugMarkerSetObjectTagEXT(VkDevice device, VkDebugMarkerObjectTagInfoEXT* pTagInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, VkDebugMarkerMarkerInfoEXT* pMarkerInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, VkDebugMarkerMarkerInfoEXT* pMarkerInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetGeneratedCommandsMemoryRequirementsNV(VkDevice device, VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, VkWriteDescriptorSet* pDescriptorWrites);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkTrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetMemoryWin32HandleKHR(VkDevice device, VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetMemoryFdKHR(VkDevice device, VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetMemoryZirconHandleFUCHSIA(VkDevice device, VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetMemoryRemoteAddressNV(VkDevice device, VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetMemorySciBufNV(VkDevice device, VkMemoryGetSciBufInfoNV* pGetSciBufInfo, NvSciBufObj* pHandle);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV(VkPhysicalDevice physicalDevice, VkExternalMemoryHandleTypeFlagBits handleType, NvSciBufObj handle, VkMemorySciBufPropertiesNV* pMemorySciBufProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceSciBufAttributesNV(VkPhysicalDevice physicalDevice, NvSciBufAttrList pAttributes);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetSemaphoreWin32HandleKHR(VkDevice device, VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkImportSemaphoreWin32HandleKHR(VkDevice device, VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetSemaphoreFdKHR(VkDevice device, VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkImportSemaphoreFdKHR(VkDevice device, VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetSemaphoreZirconHandleFUCHSIA(VkDevice device, VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkImportSemaphoreZirconHandleFUCHSIA(VkDevice device, VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetFenceWin32HandleKHR(VkDevice device, VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkImportFenceWin32HandleKHR(VkDevice device, VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetFenceFdKHR(VkDevice device, VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkImportFenceFdKHR(VkDevice device, VkImportFenceFdInfoKHR* pImportFenceFdInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetFenceSciSyncFenceNV(VkDevice device, VkFenceGetSciSyncInfoNV* pGetSciSyncHandleInfo, void* pHandle);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetFenceSciSyncObjNV(VkDevice device, VkFenceGetSciSyncInfoNV* pGetSciSyncHandleInfo, void* pHandle);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkImportFenceSciSyncFenceNV(VkDevice device, VkImportFenceSciSyncInfoNV* pImportFenceSciSyncInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkImportFenceSciSyncObjNV(VkDevice device, VkImportFenceSciSyncInfoNV* pImportFenceSciSyncInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetSemaphoreSciSyncObjNV(VkDevice device, VkSemaphoreGetSciSyncInfoNV* pGetSciSyncInfo, void* pHandle);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkImportSemaphoreSciSyncObjNV(VkDevice device, VkImportSemaphoreSciSyncInfoNV* pImportSemaphoreSciSyncInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceSciSyncAttributesNV(VkPhysicalDevice physicalDevice, VkSciSyncAttributesInfoNV* pSciSyncAttributesInfo, NvSciSyncAttrList pAttributes);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateSemaphoreSciSyncPoolNV(VkDevice device, VkSemaphoreSciSyncPoolCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSemaphoreSciSyncPoolNV* pSemaphorePool);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroySemaphoreSciSyncPoolNV(VkDevice device, VkSemaphoreSciSyncPoolNV semaphorePool, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkAcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkAcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkDisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, VkDisplayPowerInfoEXT* pDisplayPowerInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkRegisterDeviceEventEXT(VkDevice device, VkDeviceEventInfoEXT* pDeviceEventInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkRegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, VkDisplayEventInfoEXT* pDisplayEventInfo, VkAllocationCallbacks* pAllocator, VkFence* pFence);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkEnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkBindBufferMemory2(VkDevice device, uint32_t bindInfoCount, VkBindBufferMemoryInfo* pBindInfos);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkBindImageMemory2(VkDevice device, uint32_t bindInfoCount, VkBindImageMemoryInfo* pBindInfos);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkAcquireNextImage2KHR(VkDevice device, VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkUpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, void* pData);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, void* pData);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkSetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, VkSwapchainKHR* pSwapchains, VkHdrMetadataEXT* pMetadata);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateIOSSurfaceMVK(VkInstance instance, VkIOSSurfaceCreateInfoMVK* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateMacOSSurfaceMVK(VkInstance instance, VkMacOSSurfaceCreateInfoMVK* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateMetalSurfaceEXT(VkInstance instance, VkMetalSurfaceCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewportWScalingNV* pViewportWScalings);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, VkRect2D* pDiscardRectangles);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, VkSampleLocationsInfoEXT* pSampleLocationsInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetBufferMemoryRequirements2(VkDevice device, VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetImageMemoryRequirements2(VkDevice device, VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetImageSparseMemoryRequirements2(VkDevice device, VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceBufferMemoryRequirements(VkDevice device, VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceImageMemoryRequirements(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceImageSparseMemoryRequirements(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateSamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceQueue2(VkDevice device, VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateValidationCacheEXT(VkDevice device, VkValidationCacheCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkMergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, VkValidationCacheEXT* pSrcCaches);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDescriptorSetLayoutSupport(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkAcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkQueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkSetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceCalibrateableTimeDomainsKHR(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetCalibratedTimestampsKHR(VkDevice device, uint32_t timestampCount, VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkSetDebugUtilsObjectNameEXT(VkDevice device, VkDebugUtilsObjectNameInfoEXT* pNameInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkSetDebugUtilsObjectTagEXT(VkDevice device, VkDebugUtilsObjectTagInfoEXT* pTagInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkQueueBeginDebugUtilsLabelEXT(VkQueue queue, VkDebugUtilsLabelEXT* pLabelInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkQueueEndDebugUtilsLabelEXT(VkQueue queue);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkQueueInsertDebugUtilsLabelEXT(VkQueue queue, VkDebugUtilsLabelEXT* pLabelInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, VkDebugUtilsLabelEXT* pLabelInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, VkDebugUtilsLabelEXT* pLabelInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkSubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateRenderPass2(VkDevice device, VkRenderPassCreateInfo2* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBeginRenderPass2(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassBeginInfo* pSubpassBeginInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdNextSubpass2(VkCommandBuffer commandBuffer, VkSubpassBeginInfo* pSubpassBeginInfo, VkSubpassEndInfo* pSubpassEndInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdEndRenderPass2(VkCommandBuffer commandBuffer, VkSubpassEndInfo* pSubpassEndInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkWaitSemaphores(VkDevice device, VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkSignalSemaphore(VkDevice device, VkSemaphoreSignalInfo* pSignalInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetAndroidHardwareBufferPropertiesANDROID(VkDevice device, AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetMemoryAndroidHardwareBufferANDROID(VkDevice device, VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, AHardwareBuffer* pBuffer);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetCheckpointNV(VkCommandBuffer commandBuffer, void* pCheckpointMarker);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets, VkDeviceSize* pSizes);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, VkBuffer* pCounterBuffers, VkDeviceSize* pCounterBufferOffsets);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, VkBuffer* pCounterBuffers, VkDeviceSize* pCounterBufferOffsets);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, VkRect2D* pExclusiveScissors);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, VkBool32* pExclusiveScissorEnables);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkShadingRatePaletteNV* pShadingRatePalettes);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, VkCoarseSampleOrderCustomNV* pCustomSampleOrders);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateAccelerationStructureNV(VkDevice device, VkAccelerationStructureCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetAccelerationStructureMemoryRequirementsNV(VkDevice device, VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkBindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, VkBindAccelerationStructureMemoryInfoNV* pBindInfos);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, VkCopyAccelerationStructureInfoKHR* pInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyAccelerationStructureInfoKHR* pInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyAccelerationStructureToMemoryInfoKHR* pInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMemoryToAccelerationStructureInfoKHR* pInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkWriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, size_t dataSize, void* pData, size_t stride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdTraceRaysKHR(VkCommandBuffer commandBuffer, VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkRayTracingPipelineCreateInfoNV* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkRayTracingPipelineCreateInfoNV* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkRayTracingPipelineCreateInfoKHR* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkRayTracingPipelineCreateInfoKHR* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);

ACTIAS_SYSTEM_API VkDeviceSize ACTIAS_ABI vkGetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize);

ACTIAS_SYSTEM_API uint32_t ACTIAS_ABI vkGetImageViewHandleNVX(VkDevice device, VkImageViewHandleInfoNVX* pInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceSurfacePresentModes2EXT(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetDeviceGroupSurfacePresentModes2EXT(VkDevice device, VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkAcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkAcquireProfilingLockKHR(VkDevice device, VkAcquireProfilingLockInfoKHR* pInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkReleaseProfilingLockKHR(VkDevice device);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties);

ACTIAS_SYSTEM_API uint64_t ACTIAS_ABI vkGetBufferOpaqueCaptureAddress(VkDevice device, VkBufferDeviceAddressInfo* pInfo);

ACTIAS_SYSTEM_API VkDeviceAddress ACTIAS_ABI vkGetBufferDeviceAddress(VkDevice device, VkBufferDeviceAddressInfo* pInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateHeadlessSurfaceEXT(VkInstance instance, VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkInitializePerformanceApiINTEL(VkDevice device, VkInitializePerformanceApiInfoINTEL* pInitializeInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkUninitializePerformanceApiINTEL(VkDevice device);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, VkPerformanceMarkerInfoINTEL* pMarkerInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, VkPerformanceOverrideInfoINTEL* pOverrideInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkAcquirePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkQueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue);

ACTIAS_SYSTEM_API uint64_t ACTIAS_ABI vkGetDeviceMemoryOpaqueCaptureAddress(VkDevice device, VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPipelineExecutablePropertiesKHR(VkDevice device, VkPipelineInfoKHR* pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPipelineExecutableStatisticsKHR(VkDevice device, VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPipelineExecutableInternalRepresentationsKHR(VkDevice device, VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetLineStippleKHR(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetFaultData(VkDevice device, VkFaultQueryBehavior faultQueryBehavior, VkBool32* pUnrecordedFaults, uint32_t* pFaultCount, VkFaultData* pFaults);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateAccelerationStructureKHR(VkDevice device, VkAccelerationStructureCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkAccelerationStructureKHR* pAccelerationStructure);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBuildAccelerationStructuresKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* ppBuildRangeInfos);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, VkAccelerationStructureBuildGeometryInfoKHR* pInfos, VkDeviceAddress* pIndirectDeviceAddresses, uint32_t* pIndirectStrides, const uint32_t* ppMaxPrimitiveCounts);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkBuildAccelerationStructuresKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* ppBuildRangeInfos);

ACTIAS_SYSTEM_API VkDeviceAddress ACTIAS_ABI vkGetAccelerationStructureDeviceAddressKHR(VkDevice device, VkAccelerationStructureDeviceAddressInfoKHR* pInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateDeferredOperationKHR(VkDevice device, VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API uint32_t ACTIAS_ABI vkGetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkDeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPipelineIndirectMemoryRequirementsNV(VkDevice device, VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements);

ACTIAS_SYSTEM_API VkDeviceAddress ACTIAS_ABI vkGetPipelineIndirectDeviceAddressNV(VkDevice device, VkPipelineIndirectDeviceAddressInfoNV* pInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, VkViewport* pViewports);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, VkRect2D* pScissors);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindIndexBuffer2KHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets, VkDeviceSize* pSizes, VkDeviceSize* pStrides);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits rasterizationSamples);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits samples, VkSampleMask* pSampleMask);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkBool32* pColorBlendEnables);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkColorBlendEquationEXT* pColorBlendEquations);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkColorComponentFlags* pColorWriteMasks);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, VkColorBlendAdvancedEXT* pColorBlendAdvanced);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, VkViewportSwizzleNV* pViewportSwizzles);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, float* pCoverageModulationTable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreatePrivateDataSlot(VkDevice device, VkPrivateDataSlotCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkSetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyBuffer2(VkCommandBuffer commandBuffer, VkCopyBufferInfo2* pCopyBufferInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyImage2(VkCommandBuffer commandBuffer, VkCopyImageInfo2* pCopyImageInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBlitImage2(VkCommandBuffer commandBuffer, VkBlitImageInfo2* pBlitImageInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyBufferToImage2(VkCommandBuffer commandBuffer, VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdResolveImage2(VkCommandBuffer commandBuffer, VkResolveImageInfo2* pResolveImageInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdRefreshObjectsKHR(VkCommandBuffer commandBuffer, VkRefreshObjectListKHR* pRefreshObjects);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceRefreshableObjectTypesKHR(VkPhysicalDevice physicalDevice, uint32_t* pRefreshableObjectTypeCount, VkObjectType* pRefreshableObjectTypes);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetFragmentShadingRateKHR(VkCommandBuffer commandBuffer, VkExtent2D* pFragmentSize, VkFragmentShadingRateCombinerOpKHR combinerOps);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetFragmentShadingRateEnumNV(VkCommandBuffer commandBuffer, VkFragmentShadingRateNV shadingRate, VkFragmentShadingRateCombinerOpKHR combinerOps);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetAccelerationStructureBuildSizesKHR(VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo, uint32_t* pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetColorWriteEnableEXT(VkCommandBuffer commandBuffer, uint32_t attachmentCount, VkBool32* pColorWriteEnables);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkDependencyInfo* pDependencyInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdResetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdWaitEvents2(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkDependencyInfo* pDependencyInfos);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdPipelineBarrier2(VkCommandBuffer commandBuffer, VkDependencyInfo* pDependencyInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkQueueSubmit2(VkQueue queue, uint32_t submitCount, VkSubmitInfo2* pSubmits, VkFence fence);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdWriteTimestamp2(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdWriteBufferMarker2AMD(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCopyMemoryToImageEXT(VkDevice device, VkCopyMemoryToImageInfoEXT* pCopyMemoryToImageInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCopyImageToMemoryEXT(VkDevice device, VkCopyImageToMemoryInfoEXT* pCopyImageToMemoryInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCopyImageToImageEXT(VkDevice device, VkCopyImageToImageInfoEXT* pCopyImageToImageInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkTransitionImageLayoutEXT(VkDevice device, uint32_t transitionCount, VkHostImageLayoutTransitionInfoEXT* pTransitions);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetCommandPoolMemoryConsumption(VkDevice device, VkCommandPool commandPool, VkCommandBuffer commandBuffer, VkCommandPoolMemoryConsumption* pConsumption);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceVideoCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceVideoFormatPropertiesKHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR* pQualityLevelInfo, VkVideoEncodeQualityLevelPropertiesKHR* pQualityLevelProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateVideoSessionKHR(VkDevice device, VkVideoSessionCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersCreateInfoKHR* pCreateInfo, VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkUpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetEncodedVideoSessionParametersKHR(VkDevice device, VkVideoEncodeSessionParametersGetInfoKHR* pVideoSessionParametersInfo, VkVideoEncodeSessionParametersFeedbackInfoKHR* pFeedbackInfo, size_t* pDataSize, void* pData);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkBindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDecodeVideoKHR(VkCommandBuffer commandBuffer, VkVideoDecodeInfoKHR* pDecodeInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, VkVideoBeginCodingInfoKHR* pBeginInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, VkVideoCodingControlInfoKHR* pCodingControlInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, VkVideoEndCodingInfoKHR* pEndCodingInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdEncodeVideoKHR(VkCommandBuffer commandBuffer, VkVideoEncodeInfoKHR* pEncodeInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, VkDecompressMemoryRegionNV* pDecompressMemoryRegions);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateCuModuleNVX(VkDevice device, VkCuModuleCreateInfoNVX* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateCuFunctionNVX(VkDevice device, VkCuFunctionCreateInfoNVX* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, VkCuLaunchInfoNVX* pLaunchInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDescriptorEXT(VkDevice device, VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, VkDescriptorBufferBindingInfoEXT* pBindingInfos);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, uint32_t* pBufferIndices, VkDeviceSize* pOffsets);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkSetDeviceMemoryPriorityEXT(VkDevice device, VkDeviceMemory memory, float priority);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkAcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkWaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkSetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkSetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateCudaModuleNV(VkDevice device, VkCudaModuleCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCudaModuleNV* pModule);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetCudaModuleCacheNV(VkDevice device, VkCudaModuleNV module, size_t* pCacheSize, void* pCacheData);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateCudaFunctionNV(VkDevice device, VkCudaFunctionCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCudaFunctionNV* pFunction);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyCudaModuleNV(VkDevice device, VkCudaModuleNV module, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyCudaFunctionNV(VkDevice device, VkCudaFunctionNV function, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCudaLaunchKernelNV(VkCommandBuffer commandBuffer, VkCudaLaunchInfoNV* pLaunchInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBeginRendering(VkCommandBuffer commandBuffer, VkRenderingInfo* pRenderingInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdEndRendering(VkCommandBuffer commandBuffer);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void* ppData);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateMicromapEXT(VkDevice device, VkMicromapCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkMicromapEXT* pMicromap);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBuildMicromapsEXT(VkCommandBuffer commandBuffer, uint32_t infoCount, VkMicromapBuildInfoEXT* pInfos);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkBuildMicromapsEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, VkMicromapBuildInfoEXT* pInfos);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyMicromapEXT(VkCommandBuffer commandBuffer, VkCopyMicromapInfoEXT* pInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMicromapInfoEXT* pInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, VkCopyMicromapToMemoryInfoEXT* pInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMicromapToMemoryInfoEXT* pInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, VkCopyMemoryToMicromapInfoEXT* pInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, VkCopyMemoryToMicromapInfoEXT* pInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkWriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, VkMicromapEXT* pMicromaps, VkQueryType queryType, size_t dataSize, void* pData, size_t stride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceMicromapCompatibilityEXT(VkDevice device, VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetMicromapBuildSizesEXT(VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, VkMicromapBuildInfoEXT* pBuildInfo, VkMicromapBuildSizesInfoEXT* pSizeInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetShaderModuleCreateInfoIdentifierEXT(VkDevice device, VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetImageSubresourceLayout2KHR(VkDevice device, VkImage image, VkImageSubresource2KHR* pSubresource, VkSubresourceLayout2KHR* pLayout);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPipelinePropertiesEXT(VkDevice device, VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetDynamicRenderingTilePropertiesQCOM(VkDevice device, VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceOpticalFlowImageFormatsNV(VkPhysicalDevice physicalDevice, VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionCreateInfoNV* pCreateInfo, VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkBindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, VkOpticalFlowExecuteInfoNV* pExecuteInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthBias2EXT(VkCommandBuffer commandBuffer, VkDepthBiasInfoEXT* pDepthBiasInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkReleaseSwapchainImagesEXT(VkDevice device, VkReleaseSwapchainImagesInfoEXT* pReleaseInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceImageSubresourceLayoutKHR(VkDevice device, VkDeviceImageSubresourceInfoKHR* pInfo, VkSubresourceLayout2KHR* pLayout);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkMapMemory2KHR(VkDevice device, VkMemoryMapInfoKHR* pMemoryMapInfo, void* ppData);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkUnmapMemory2KHR(VkDevice device, VkMemoryUnmapInfoKHR* pMemoryUnmapInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateShadersEXT(VkDevice device, uint32_t createInfoCount, VkShaderCreateInfoEXT* pCreateInfos, VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyShaderEXT(VkDevice device, VkShaderEXT shader, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, VkShaderStageFlagBits* pStages, VkShaderEXT* pShaders);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetScreenBufferPropertiesQNX(VkDevice device, _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetExecutionGraphPipelineScratchSizeAMDX(VkDevice device, VkPipeline executionGraph, VkExecutionGraphPipelineScratchSizeAMDX* pSizeInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetExecutionGraphPipelineNodeIndexAMDX(VkDevice device, VkPipeline executionGraph, VkPipelineShaderStageNodeCreateInfoAMDX* pNodeInfo, uint32_t* pNodeIndex);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateExecutionGraphPipelinesAMDX(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkExecutionGraphPipelineCreateInfoAMDX* pCreateInfos, VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdInitializeGraphScratchMemoryAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDispatchGraphAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDispatchGraphCountInfoAMDX* pCountInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDispatchGraphIndirectAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDispatchGraphCountInfoAMDX* pCountInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDispatchGraphIndirectCountAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDeviceAddress countInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindDescriptorSets2KHR(VkCommandBuffer commandBuffer, VkBindDescriptorSetsInfoKHR* pBindDescriptorSetsInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdPushConstants2KHR(VkCommandBuffer commandBuffer, VkPushConstantsInfoKHR* pPushConstantsInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdPushDescriptorSet2KHR(VkCommandBuffer commandBuffer, VkPushDescriptorSetInfoKHR* pPushDescriptorSetInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdPushDescriptorSetWithTemplate2KHR(VkCommandBuffer commandBuffer, VkPushDescriptorSetWithTemplateInfoKHR* pPushDescriptorSetWithTemplateInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDescriptorBufferOffsets2EXT(VkCommandBuffer commandBuffer, VkSetDescriptorBufferOffsetsInfoEXT* pSetDescriptorBufferOffsetsInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindDescriptorBufferEmbeddedSamplers2EXT(VkCommandBuffer commandBuffer, VkBindDescriptorBufferEmbeddedSamplersInfoEXT* pBindDescriptorBufferEmbeddedSamplersInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkSetLatencySleepModeNV(VkDevice device, VkSwapchainKHR swapchain, VkLatencySleepModeInfoNV* pSleepModeInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkLatencySleepNV(VkDevice device, VkSwapchainKHR swapchain, VkLatencySleepInfoNV* pSleepInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkSetLatencyMarkerNV(VkDevice device, VkSwapchainKHR swapchain, VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetLatencyTimingsNV(VkDevice device, VkSwapchainKHR swapchain, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkQueueNotifyOutOfBandNV(VkQueue queue, VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetRenderingAttachmentLocationsKHR(VkCommandBuffer commandBuffer, VkRenderingAttachmentLocationInfoKHR* pLocationInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetRenderingInputAttachmentIndicesKHR(VkCommandBuffer commandBuffer, VkRenderingInputAttachmentIndexInfoKHR* pLocationInfo);

