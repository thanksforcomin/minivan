#pragma once

#include "vulkan_context.hpp"

namespace engine {
  struct FrameSyncData {
    VkSemaphore swapchain_semop;
    VkSemaphore render_semop;
    VkFence fence;
  };

  class SyncManager {
    std::vector<FrameSyncData> frames_;
    uint32_t frames_in_flight_;
    uint32_t current_frame_ = 0;

    VulkanDevice device_;

  public:
    [[nodiscard]] static auto init(const VulkanContext &context, uint32_t frames_in_flight) noexcept
        -> Result<SyncManager>;

    SyncManager(std::vector<FrameSyncData> &&frames, uint32_t frames_in_flight,
                VulkanDevice device) noexcept;

    SyncManager(const SyncManager &other) = delete;
    auto operator=(const SyncManager &other) -> SyncManager & = delete;

    SyncManager(SyncManager &&other) noexcept;
    auto operator=(SyncManager &&other) noexcept -> SyncManager &;

    ~SyncManager();

    auto advance() noexcept -> void;
    auto current() noexcept -> FrameSyncData;
  };
  
}
