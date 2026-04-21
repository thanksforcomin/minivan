#pragma once
#include <vulkan/vulkan.h>

#include "vulkan_context.hpp"

namespace engine {
  class Swapchain {
    VkSwapchainKHR swapchain_;
    VkFormat swapchain_image_format_;
    VkExtent2D swapchain_extent_;
    std::vector<VkImage> swapchain_images_;
    std::vector<VkImageView> swapchain_image_views_;

    // non-owning resources
    VulkanDevice device_;
    VkSurfaceKHR surface_;

  public:
    Swapchain(const VulkanContext &context);

    Swapchain(const Swapchain &) = delete;
    auto operator=(const Swapchain &) -> Swapchain & = delete;
    
  private:
    auto create_swapchain(uint32_t width, uint32_t height) -> void;
    auto destroy_swapchain() -> void;
  };
  
}
