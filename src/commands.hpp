#pragma once

#include "vulkan_context.hpp"
#include <vulkan/vulkan_core.h>

namespace engine {
  class CommandManager {
    struct FrameResources {
      VkCommandPool pool = VK_NULL_HANDLE;
      VkCommandBuffer cmd_buffer = VK_NULL_HANDLE;
    };

    uint32_t frames_in_flight_;
    uint32_t current_frame_;
    std::vector<FrameResources> frames_;
    
    VulkanDevice device_;
    uint32_t queue_family_index_;

  public:
    static auto init(const VulkanContext &context, uint32_t queue_index,
              uint32_t frames_in_flight) noexcept -> Result<CommandManager>;

    CommandManager(std::vector<FrameResources> &&frames,
                   VulkanDevice device, uint32_t queue_family_index, uint32_t frames_in_flight) noexcept;

    CommandManager(const CommandManager &other) = delete;
    auto operator=(const CommandManager &other) -> CommandManager & = delete;

    CommandManager(CommandManager &&other) noexcept;
    auto operator=(CommandManager &&other) noexcept -> CommandManager &;

    ~CommandManager();

    auto advance() noexcept -> void;
    auto begin() noexcept -> Result<VkCommandBuffer>;
    auto end() noexcept -> Result<void>;
    auto get_current() noexcept -> Result<VkCommandBuffer>;

  private:
    auto cleanup() noexcept -> void;
    
  };
  
}
