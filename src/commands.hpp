#pragma once

#include "vulkan_context.hpp"
#include <vulkan/vulkan_core.h>

namespace engine {
  template <unsigned int FramesInFlight>
  class CommandManager {
    struct FrameResources {
      VkCommandPool pool = VK_NULL_HANDLE;
      VkCommandBuffer cmd_buffer = VK_NULL_HANDLE;
    };

    uint32_t current_frame_;
    std::array<FrameResources, FramesInFlight> frames_;
    
    VulkanDevice device_;
    uint32_t queue_family_index_;

  public:
    auto init(const VulkanContext &context) noexcept -> Result<CommandManager>;
    CommandManager(std::array<FrameResources, FramesInFlight> &&frames,
                   VulkanDevice device, uint32_t queue_family_index) noexcept;

    CommandManager(const CommandManager &other) = delete;
    auto operator=(const CommandManager &other) -> CommandManager & = delete;

    CommandManager(CommandManager &&other) noexcept;
    auto operator=(CommandManager &&other) noexcept -> CommandManager &;

    ~CommandManager();

    auto advance() -> void;
    auto begin() -> Result<VkCommandBuffer>;
    auto end() -> Result<void>;
    auto get_current() -> Result<VkCommandBuffer>;
    
  };
  
}
