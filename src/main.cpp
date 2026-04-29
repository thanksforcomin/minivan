#include <iostream>
#include <print>

#include "SDL_error.h"
#include "SDL_video.h"
#include "VkBootstrapFeatureChain.h"
#include "vulkan_context.hpp"
#include "swapchain.hpp"
#include "utility.hpp"

int main() {
  unsigned window_flags = SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
  auto *window =
      SDL_CreateWindow("turbo goon", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 100, 100, window_flags);

  if (!window) {
    std::println("Failed to create an SDL window because {}", SDL_GetError());
    return 1;
  }

  auto instance_options = engine::ContextOptions {
    .use_validation_layers = true
  };

  auto vulkan_context = engine::VulkanContext::init(window, instance_options).value();
  auto swapchain = engine::Swapchain::init(vulkan_context, 100, 100);

  std::println("Initialized stuff");

  SDL_DestroyWindow(window);
  
  return 0;
}
