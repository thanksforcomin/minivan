#include "swapchain.hpp"
#include <VkBootstrap.h>
#include <vulkan/vulkan_core.h>

namespace engine {
  Swapchain::Swapchain(const VulkanContext &context)
  : device_(context.device_), surface_(context.surface_)
  {
    create_swapchain(0, 0);
  }

  auto Swapchain::create_swapchain(uint32_t width, uint32_t height) -> void
  {
    vkb::SwapchainBuilder swapchain_builder{device_.phy_device, device_.device, surface_};
    
    swapchain_image_format_ = VK_FORMAT_B8G8R8A8_UNORM;

    vkb::Swapchain vkb_swapchain =
        swapchain_builder
            .set_desired_format(VkSurfaceFormatKHR{
                .format = swapchain_image_format_,
                .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR})
            .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
            .set_desired_extent(width, height)
            .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .build()
            .value();

    swapchain_extent_ = vkb_swapchain.extent;
    swapchain_ = vkb_swapchain.swapchain;
    swapchain_images_ = vkb_swapchain.get_images().value();
    swapchain_image_views_ = vkb_swapchain.get_image_views().value();
  }

  auto Swapchain::destroy_swapchain() -> void {
    vkDestroySwapchainKHR(device_.device, swapchain_, nullptr);

    for (auto const &view : swapchain_image_views_) {
      vkDestroyImageView(device_.device, view, nullptr);
    }
  }
}
