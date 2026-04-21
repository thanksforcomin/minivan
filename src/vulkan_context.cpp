#include "vulkan_context.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <VkBootstrap.h>
#include <vulkan/vulkan_core.h>

namespace engine {
  VulkanContext::VulkanContext(struct SDL_Window *window,
                               const ContextOptions &options) 
  {
    init(window, options);
  }
  
  auto VulkanContext::init(struct SDL_Window* window, bool use_validation_layers) -> void {
    vkb::InstanceBuilder builder;

    auto inst_ret = builder.set_app_name("application")
                        .request_validation_layers(use_validation_layers)
                        .use_default_debug_messenger()
                        .require_api_version(1, 3, 0)
                        .build();

    vkb::Instance vkb_instance = inst_ret.value();

    instance_ = vkb_instance.instance;
    debug_messenger_ = vkb_instance.debug_messenger;

    //SDL_Vulkan_CreateSurface(window, instance_, &surface_);

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
                                         .set_surface(surface_)
                                         .select()
                                         .value();

    vkb::DeviceBuilder device_builder{phy_device};
    vkb::Device vkb_device = device_builder.build().value();

    device_.device = vkb_device.device;
    device_.phy_device = phy_device.physical_device;

    graphics_queue_ = vkb_device.get_queue(vkb::QueueType::graphics).value();
    graphics_queue_family_ =
      vkb_device.get_queue_index(vkb::QueueType::graphics).value();
  }
  
}
