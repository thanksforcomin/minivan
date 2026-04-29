#pragma once

#include <expected>
#include <string_view>

namespace utils {
  template <typename T> using Result = std::expected<T, std::string_view>;

}
