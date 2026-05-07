#pragma once

#include <expected>
#include <string_view>
#include <vulkan/vulkan.h>

namespace utils {
  template <typename T> using Result = std::expected<T, std::string_view>;
}
