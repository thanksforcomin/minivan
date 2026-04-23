#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "vulkan_context.hpp"
#include "utility.hpp"

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
    static auto init(const VulkanContext &context, uint32_t width, uint32_t height) -> Result<Swapchain>;

    Swapchain(VkSwapchainKHR swapchain, VkFormat image_format,
              VkExtent2D swapchain_extent,
              std::vector<VkImage> &&swapchain_images,
              std::vector<VkImageView> &&swapchain_image_views,
              VulkanDevice device, VkSurfaceKHR surface);

    Swapchain(const Swapchain &) = delete;
    auto operator=(const Swapchain &) -> Swapchain & = delete;

    Swapchain(Swapchain &&) noexcept;
    auto operator=(Swapchain&&) noexcept -> Swapchain&;
    
  private:
    auto create_swapchain(uint32_t width, uint32_t height) -> void;
    auto destroy_swapchain() -> void;
  };
  
}
