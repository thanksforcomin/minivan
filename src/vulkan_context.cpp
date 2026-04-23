#include "vulkan_context.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <VkBootstrap.h>
#include <vulkan/vulkan_core.h>

namespace engine {

  auto VulkanContext::init(SDL_Window *window,
                           const ContextOptions &options)
    -> Result<VulkanContext> {
    vkb::InstanceBuilder builder;

    auto inst_ret =
        builder.set_app_name("application")
            .request_validation_layers(options.use_validation_layers)
            .use_default_debug_messenger()
            .require_api_version(1, 3, 0)
            .build();

    vkb::Instance vkb_instance = inst_ret.value();

    VkInstance instance = vkb_instance.instance;
    VkDebugUtilsMessengerEXT debug_messenger = vkb_instance.debug_messenger;

    VkSurfaceKHR surface;
    
    SDL_Vulkan_CreateSurface(window, instance, &surface);

    VkPhysicalDeviceVulkan13Features features13{
        .synchronization2 = true,
        .dynamicRendering = true,
    };

    VkPhysicalDeviceVulkan12Features features12{
      .descriptorIndexing = true, .descriptorBindingPartiallyBound = true,
      .descriptorBindingVariableDescriptorCount = true,
      .runtimeDescriptorArray = true,
      .bufferDeviceAddress = true
    };

    vkb::PhysicalDeviceSelector selector{vkb_instance};
    vkb::PhysicalDevice phy_device = selector.set_minimum_version(1, 3)
                                         .set_required_features_13(features13)
                                         .set_required_features_12(features12)
                                         .set_surface(surface)
                                         .select()
                                         .value();

    vkb::DeviceBuilder device_builder{phy_device};
    vkb::Device vkb_device = device_builder.build().value();

    VulkanDevice device;
    
    device.device = vkb_device.device;
    device.phy_device = phy_device.physical_device;

    VkQueue graphics_queue;
    VkQueue present_queue;
    VkQueue compute_queue;
    VkQueue transport_queue;
    uint32_t graphics_queue_family;
    
    graphics_queue = vkb_device.get_queue(vkb::QueueType::graphics).value();
    graphics_queue_family =
      vkb_device.get_queue_index(vkb::QueueType::graphics).value();
    present_queue = vkb_device.get_queue(vkb::QueueType::present).value();
    compute_queue = vkb_device.get_queue(vkb::QueueType::compute).value();
    transport_queue = vkb_device.get_queue(vkb::QueueType::transfer).value();

    return VulkanContext(instance, surface, device, graphics_queue,
                         present_queue, compute_queue, transport_queue, debug_messenger, graphics_queue_family);
  }

  VulkanContext::VulkanContext(VkInstance instance, VkSurfaceKHR surface,
                               VulkanDevice device, VkQueue graphics_queue,
                               VkQueue present_queue, VkQueue compute_queue,
                               VkQueue transport_queue,
                               VkDebugUtilsMessengerEXT debug_messenger,
                               uint32_t graphics_queue_family) noexcept
      : instance_(instance), surface_(surface), device_(device),
        graphics_queue_(graphics_queue), present_queue_(present_queue),
        compute_queue_(compute_queue), transport_queue_(transport_queue),
        debug_messenger_(debug_messenger),
        graphics_queue_family_(graphics_queue_family) {}
  
  VulkanContext::VulkanContext(VulkanContext&& other) noexcept
  : instance_(other.instance_),
    surface_(other.surface_),
    device_(std::move(other.device_)),
    graphics_queue_(other.graphics_queue_),
    present_queue_(other.present_queue_),
    debug_messenger_(other.debug_messenger_),
    graphics_queue_family_(other.graphics_queue_family_)
  {
    other.instance_ = VK_NULL_HANDLE;
    other.surface_ = VK_NULL_HANDLE;
    other.graphics_queue_ = VK_NULL_HANDLE;
    other.present_queue_ = VK_NULL_HANDLE;
    other.debug_messenger_ = VK_NULL_HANDLE;
    other.graphics_queue_family_ = 0;
  }
  
  VulkanContext& VulkanContext::operator=(VulkanContext&& other) noexcept {
    if (this == &other) return *this;
    
    if (debug_messenger_ != VK_NULL_HANDLE && instance_ != VK_NULL_HANDLE)
      vkDestroyDebugUtilsMessengerEXT(instance_, debug_messenger_, nullptr);
    
    if (surface_ != VK_NULL_HANDLE && instance_ != VK_NULL_HANDLE)
      vkDestroySurfaceKHR(instance_, surface_, nullptr);
    
    if (instance_ != VK_NULL_HANDLE)
      vkDestroyInstance(instance_, nullptr);
    
    instance_ = other.instance_;
    surface_ = other.surface_;
    device_ = std::move(other.device_);
    graphics_queue_ = other.graphics_queue_;
    present_queue_ = other.present_queue_;
    debug_messenger_ = other.debug_messenger_;
    graphics_queue_family_ = other.graphics_queue_family_;

    other.instance_ = VK_NULL_HANDLE;
    other.surface_ = VK_NULL_HANDLE;
    other.graphics_queue_ = VK_NULL_HANDLE;
    other.present_queue_ = VK_NULL_HANDLE;
    other.debug_messenger_ = VK_NULL_HANDLE;
    other.graphics_queue_family_ = 0;

    return *this;
  }
  

  VulkanContext::~VulkanContext() {
    if (debug_messenger_ != VK_NULL_HANDLE && instance_ != VK_NULL_HANDLE) {
        vkDestroyDebugUtilsMessengerEXT(instance_, debug_messenger_, nullptr);
        debug_messenger_ = VK_NULL_HANDLE;
    }

    if (surface_ != VK_NULL_HANDLE && instance_ != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instance_, surface_, nullptr);
        surface_ = VK_NULL_HANDLE;
    }

    if (instance_ != VK_NULL_HANDLE) {
        vkDestroyInstance(instance_, nullptr);
        instance_ = VK_NULL_HANDLE;
    }
    
  }
  
}
