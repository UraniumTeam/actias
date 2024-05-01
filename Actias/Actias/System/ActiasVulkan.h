#pragma once
#define VK_NO_PROTOTYPES
#include <Actias/System/Base.h>
#include <Actias/System/Platform/Common/volk.h>

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

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateCommandPool(VkDevice device, VkCommandPoolCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyCommandPool(VkDevice device, VkCommandPool commandPool, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkAllocateCommandBuffers(VkDevice device, VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkBeginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferBeginInfo* pBeginInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkEndCommandBuffer(VkCommandBuffer commandBuffer);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);

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

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, VkBufferCopy* pRegions);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageCopy* pRegions);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkImageBlit* pRegions, VkFilter filter);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, VkBufferImageCopy* pRegions);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, VkBufferImageCopy* pRegions);

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

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, void* pValues);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBeginRenderPass(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdEndRenderPass(VkCommandBuffer commandBuffer);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkTrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkEnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkBindBufferMemory2(VkDevice device, uint32_t bindInfoCount, VkBindBufferMemoryInfo* pBindInfos);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkBindImageMemory2(VkDevice device, uint32_t bindInfoCount, VkBindImageMemoryInfo* pBindInfos);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkUpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, void* pData);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetBufferMemoryRequirements2(VkDevice device, VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetImageMemoryRequirements2(VkDevice device, VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetImageSparseMemoryRequirements2(VkDevice device, VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceBufferMemoryRequirements(VkDevice device, VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceImageMemoryRequirements(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceImageSparseMemoryRequirements(VkDevice device, VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateSamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkDestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, VkAllocationCallbacks* pAllocator);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDeviceQueue2(VkDevice device, VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkGetDescriptorSetLayoutSupport(VkDevice device, VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkCreateRenderPass2(VkDevice device, VkRenderPassCreateInfo2* pCreateInfo, VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBeginRenderPass2(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassBeginInfo* pSubpassBeginInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdNextSubpass2(VkCommandBuffer commandBuffer, VkSubpassBeginInfo* pSubpassBeginInfo, VkSubpassEndInfo* pSubpassEndInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdEndRenderPass2(VkCommandBuffer commandBuffer, VkSubpassEndInfo* pSubpassEndInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkWaitSemaphores(VkDevice device, VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkSignalSemaphore(VkDevice device, VkSemaphoreSignalInfo* pSignalInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);

ACTIAS_SYSTEM_API uint64_t ACTIAS_ABI vkGetBufferOpaqueCaptureAddress(VkDevice device, VkBufferDeviceAddressInfo* pInfo);

ACTIAS_SYSTEM_API VkDeviceAddress ACTIAS_ABI vkGetBufferDeviceAddress(VkDevice device, VkBufferDeviceAddressInfo* pInfo);

ACTIAS_SYSTEM_API uint64_t ACTIAS_ABI vkGetDeviceMemoryOpaqueCaptureAddress(VkDevice device, VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkGetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, VkViewport* pViewports);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, VkRect2D* pScissors);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, VkBuffer* pBuffers, VkDeviceSize* pOffsets, VkDeviceSize* pSizes, VkDeviceSize* pStrides);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable);

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

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdSetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkDependencyInfo* pDependencyInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdResetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdWaitEvents2(VkCommandBuffer commandBuffer, uint32_t eventCount, VkEvent* pEvents, VkDependencyInfo* pDependencyInfos);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdPipelineBarrier2(VkCommandBuffer commandBuffer, VkDependencyInfo* pDependencyInfo);

ACTIAS_SYSTEM_API VkResult ACTIAS_ABI vkQueueSubmit2(VkQueue queue, uint32_t submitCount, VkSubmitInfo2* pSubmits, VkFence fence);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdWriteTimestamp2(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdBeginRendering(VkCommandBuffer commandBuffer, VkRenderingInfo* pRenderingInfo);

ACTIAS_SYSTEM_API void ACTIAS_ABI vkCmdEndRendering(VkCommandBuffer commandBuffer);

