#include "commands.hpp"
#include "VkBootstrap.h"
#include "src/vulkan_context.hpp"
#include <vulkan/vulkan_core.h>

namespace engine {
  auto CommandManager::init(const VulkanContext &context, uint32_t queue_index,
                            uint32_t frames_in_flight) noexcept
    -> Result<CommandManager> {
    std::vector<FrameResources> frames(frames_in_flight);
    
    VkCommandPoolCreateInfo command_pool_info {
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, .pNext = nullptr,
      .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
      .queueFamilyIndex = queue_index
    };

    for (size_t i = 0; i < frames_in_flight; ++i) {
      auto ret = vkCreateCommandPool(context.device_.device, &command_pool_info,
                                     nullptr, &frames[i].pool);
      if (!ret) 
        return std::unexpected("Failed to create command pool");

      VkCommandBufferAllocateInfo cmd_alloc_info{
          .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
          .pNext = nullptr,
          .commandPool = frames[i].pool,
          .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
          .commandBufferCount = 1,
      };

      ret = vkAllocateCommandBuffers(context.device_.device, &cmd_alloc_info,
                                     &frames[i].cmd_buffer);
      if (!ret)
        return std::unexpected("Failed to create command buffer");
    }

    return CommandManager(std::move(frames), context.device_,
                          queue_index, frames_in_flight);
  }

  CommandManager::CommandManager(std::vector<FrameResources> &&frames,
                                 VulkanDevice device,
                                 uint32_t queue_family_index,
                                 uint32_t frames_in_flight) noexcept
      : frames_in_flight_(frames_in_flight), current_frame_(0),
        frames_(std::forward<decltype(frames)>(frames)), device_(device),
        queue_family_index_(queue_family_index) {}
  
  CommandManager::CommandManager(CommandManager&& other) noexcept
    : frames_in_flight_(other.frames_in_flight_),
      current_frame_(other.current_frame_),
      frames_(std::move(other.frames_)),
      device_(other.device_),
      queue_family_index_(other.queue_family_index_)
  {
    other.frames_in_flight_ = 0;
    other.current_frame_ = 0;
    other.device_.device = VK_NULL_HANDLE;
    other.device_.phy_device = VK_NULL_HANDLE;
    other.queue_family_index_ = 0;
  }

  auto CommandManager::operator=(CommandManager&& other) noexcept -> CommandManager& {
    if (this != &other) {
      if (device_.device != VK_NULL_HANDLE) {
        for (auto& frame : frames_) {
          if (frame.pool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(device_.device, frame.pool, nullptr);
          }
        }
      }
      
      frames_in_flight_ = other.frames_in_flight_;
      current_frame_ = other.current_frame_;
      frames_ = std::move(other.frames_);
      device_ = other.device_;
      queue_family_index_ = other.queue_family_index_;
      
      other.frames_in_flight_ = 0;
      other.current_frame_ = 0;
      other.frames_.clear();
      other.device_.device = VK_NULL_HANDLE;
      other.device_.phy_device = VK_NULL_HANDLE;
      other.queue_family_index_ = 0;
    }
    return *this;
  }
  
  
}
