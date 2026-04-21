#pragma once

#include <VkBootstrap.h>
#include <vulkan/vulkan_core.h>

namespace engine {
  struct VulkanDevice {
    VkPhysicalDevice phy_device;
    VkDevice device;
  };

  struct ContextOptions {
    bool use_validation_layers;
  };

  class VulkanContext {
  public:
    VkInstance instance_;
    VkSurfaceKHR surface_;
    VulkanDevice device_;
    VkQueue graphics_queue_;
    VkQueue present_queue_;
    VkDebugUtilsMessengerEXT debug_messenger_;
    uint32_t graphics_queue_family_;

  public:
    VulkanContext(struct SDL_Window* window, const ContextOptions &options);

    VulkanContext(const VulkanContext &) = delete;
    auto operator=(const VulkanContext &) -> VulkanContext & = delete;

  private:
    auto init(struct SDL_Window* window, bool use_validation_layers = true) -> void;
  };
}
