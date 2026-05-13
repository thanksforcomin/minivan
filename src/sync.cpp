#include "sync.hpp"
#include "src/utility.hpp"
#include "src/vulkan_context.hpp"
#include <vulkan/vulkan_core.h>

namespace engine {
  always_inline_attr auto clear_frames(VkDevice device,
                                       std::vector<FrameSyncData> &frames,
                                       uint32_t count) -> void {
    for (uint32_t i = 0; i < count; ++i) {
      if (frames[i].fence != VK_NULL_HANDLE) {
            vkDestroyFence(device, frames[i].fence, nullptr);
            frames[i].fence = VK_NULL_HANDLE;
        }
        if (frames[i].swapchain_semop != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, frames[i].swapchain_semop, nullptr);
            frames[i].swapchain_semop = VK_NULL_HANDLE;
        }
        if (frames[i].render_semop != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, frames[i].render_semop, nullptr);
            frames[i].render_semop = VK_NULL_HANDLE;
        }
    }
  }
  
  auto SyncManager::init(const VulkanContext &context,
                         uint32_t frames_in_flight) noexcept
    -> Result<SyncManager> {
    std::vector<FrameSyncData> frames(frames_in_flight);

    VkFenceCreateInfo fence_info{.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                                 .flags = VK_FENCE_CREATE_SIGNALED_BIT};

    VkSemaphoreCreateInfo sem_info{.sType =
                                       VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

    for (uint32_t i = 0; i < frames_in_flight; ++i) {
      auto ret = vkCreateFence(context.device_.device, &fence_info, nullptr,
                               &frames[i].fence);
      if (ret != VK_SUCCESS) [[unlikely]] {
        clear_frames(context.device_.device, frames, i);
        return std::unexpected("Failed to create a fence");
      }

      ret = vkCreateSemaphore(context.device_.device, &sem_info, nullptr,
                              &frames[i].swapchain_semop);
      if (ret != VK_SUCCESS) [[unlikely]] {
        clear_frames(context.device_.device, frames, i);
        return std::unexpected("Failed to create a semaphore");
      }

      ret = vkCreateSemaphore(context.device_.device, &sem_info, nullptr,
                              &frames[i].render_semop);
      if (ret != VK_SUCCESS) [[unlikely]] {
        clear_frames(context.device_.device, frames, i);
        return std::unexpected("Failed to create a semaphore");
      }
    }

    return SyncManager(std::move(frames), frames_in_flight, context.device_);
  }

  SyncManager::SyncManager(std::vector<FrameSyncData> &&frames,
                           uint32_t frames_in_flight,
                           VulkanDevice device) noexcept
      : frames_(std::forward<decltype(frames)>(frames)),
        frames_in_flight_(frames_in_flight), device_(device) {}

  SyncManager::SyncManager(SyncManager &&other) noexcept
  : frames_(std::move(other.frames_)),
    frames_in_flight_(other.frames_in_flight_),
    current_frame_(other.current_frame_),
    device_(other.device_)
  {
    other.device_.device = VK_NULL_HANDLE;
    other.device_.phy_device = VK_NULL_HANDLE;
    other.frames_in_flight_ = 0;
    other.current_frame_ = 0;
  }

  auto SyncManager::operator=(SyncManager&& other) noexcept -> SyncManager& {
    if (this != &other) {
      cleanup();
      
      frames_ = std::move(other.frames_);
      device_ = other.device_;
      frames_in_flight_ = other.frames_in_flight_;
      current_frame_ = other.current_frame_;
      
      other.device_.device = VK_NULL_HANDLE;
      other.device_.phy_device = VK_NULL_HANDLE;
      other.frames_in_flight_ = 0;
      other.current_frame_ = 0;
      other.frames_.clear();
    }
    return *this;
  }

  SyncManager::~SyncManager() { cleanup(); }

  auto SyncManager::advance() noexcept -> void {
    current_frame_ = (current_frame_ + 1) % frames_in_flight_;
  }
  

  auto SyncManager::current() noexcept -> FrameSyncData {
    return frames_[current_frame_];
  }
  
  auto SyncManager::cleanup() noexcept -> void {
    if (device_.device != VK_NULL_HANDLE) {
      vkDeviceWaitIdle(device_.device);
      clear_frames(device_.device, frames_, frames_in_flight_);
    }
  }
  
  
}
