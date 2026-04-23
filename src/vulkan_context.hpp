#pragma once

#include <VkBootstrap.h>
#include <vulkan/vulkan_core.h>

#include "utility.hpp"

typedef struct SDL_Window SDL_Window;

namespace engine {

  using namespace utils;
  
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
    VkQueue compute_queue_;
    VkQueue transport_queue_;
    VkDebugUtilsMessengerEXT debug_messenger_;
    uint32_t graphics_queue_family_;

  public:
    static auto init(SDL_Window *window, const ContextOptions &options)
        -> Result<VulkanContext>;

    VulkanContext(VkInstance instance, VkSurfaceKHR surface,
                  VulkanDevice device, VkQueue graphics_queue,
                  VkQueue present_queue, VkQueue compute_queue,
                  VkQueue transport_queue,
                  VkDebugUtilsMessengerEXT debug_messenger,
                  uint32_t graphics_queue_family) noexcept;

    VulkanContext(const VulkanContext&) = delete;
    auto operator=(const VulkanContext&) -> VulkanContext& = delete;

    VulkanContext(VulkanContext&& other) noexcept;
    auto operator=(VulkanContext&& other) noexcept -> VulkanContext&;

    ~VulkanContext();
  };
}
