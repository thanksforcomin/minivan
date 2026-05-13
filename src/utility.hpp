#pragma once

#include <expected>
#include <string_view>
#include <vulkan/vulkan.h>

#define always_inline_attr  __attribute__((always_inline))

namespace utils {
  template <typename T> using Result = std::expected<T, std::string_view>;
}
