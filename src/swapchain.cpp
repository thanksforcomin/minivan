#include "swapchain.hpp"
#include "vulkan/vulkan.hpp"
#include <VkBootstrap.h>
#include <vulkan/vulkan_core.h>

namespace engine {

  auto Swapchain::init(const VulkanContext &context, uint32_t width, uint32_t height) -> Result<Swapchain> {
    vkb::SwapchainBuilder swapchain_builder{
      context.device_.phy_device, context.device_.device, context.surface_};
    
    
    VkFormat swapchain_image_format = VK_FORMAT_B8G8R8A8_UNORM;

    vkb::Swapchain vkb_swapchain =
        swapchain_builder
            .set_desired_format(VkSurfaceFormatKHR{
                .format = swapchain_image_format,
                .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR})
            .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
            .set_desired_extent(width, height)
            .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .build()
            .value();

    VkExtent2D swapchain_extent = vkb_swapchain.extent;
    VkSwapchainKHR swapchain = vkb_swapchain.swapchain;
    auto swapchain_images = vkb_swapchain.get_images().value();
    auto swapchain_image_views = vkb_swapchain.get_image_views().value();

    return Swapchain(swapchain, swapchain_image_format, swapchain_extent,
                     std::move(swapchain_images),
                     std::move(swapchain_image_views), context.device_,
                     context.surface_);
  }

  Swapchain::Swapchain(VkSwapchainKHR swapchain, VkFormat image_format,
                       VkExtent2D swapchain_extent,
                       std::vector<VkImage> &&swapchain_images,
                       std::vector<VkImageView> &&swapchain_image_views,
                       VulkanDevice device, VkSurfaceKHR surface)
      : swapchain_(swapchain), swapchain_image_format_(image_format),
        swapchain_extent_(swapchain_extent),
        swapchain_images_(
            std::forward<decltype(swapchain_images)>(swapchain_images)),
        swapchain_image_views_(std::forward<decltype(swapchain_image_views)>(
            swapchain_image_views)),
        device_(device), surface_(surface) {}

  Swapchain::Swapchain(Swapchain &&other) noexcept
      : swapchain_(other.swapchain_),
        swapchain_image_format_(other.swapchain_image_format_),
        swapchain_extent_(other.swapchain_extent_),
        swapchain_images_(std::move(other.swapchain_images_)),
        swapchain_image_views_(std::move(other.swapchain_image_views_)),
        device_(other.device_), surface_(other.surface_)
  {
    other.swapchain_ = VK_NULL_HANDLE;
    other.swapchain_image_format_ = VK_FORMAT_UNDEFINED;
    other.swapchain_extent_ = {0, 0};
    other.swapchain_images_.clear();
    other.swapchain_image_views_.clear();
  }

  auto Swapchain::operator=(Swapchain &&other) noexcept -> Swapchain & {
    if (this != &other) {
      destroy_swapchain();
      
      swapchain_ = other.swapchain_;
      swapchain_image_format_ = other.swapchain_image_format_;
      swapchain_extent_ = other.swapchain_extent_;
      swapchain_images_ = std::move(other.swapchain_images_);
      swapchain_image_views_ = std::move(other.swapchain_image_views_);
      
      device_ = other.device_;
      surface_ = other.surface_;
      
      other.swapchain_ = VK_NULL_HANDLE;
      other.swapchain_image_format_ = VK_FORMAT_UNDEFINED;
      other.swapchain_extent_ = {0, 0};
      other.swapchain_images_.clear();
      other.swapchain_image_views_.clear();
    }
    return *this;
  }
  
  Swapchain::~Swapchain() {
    destroy_swapchain();
  }
  
  auto Swapchain::create_swapchain(uint32_t width, uint32_t height) -> void
  {
  }

  auto Swapchain::destroy_swapchain() -> void {
    if(swapchain_ != VK_NULL_HANDLE && device_.device != VK_NULL_HANDLE)
      vkDestroySwapchainKHR(device_.device, swapchain_, nullptr);

    for (auto const &view : swapchain_image_views_) {
      vkDestroyImageView(device_.device, view, nullptr);
    }
  }
}
