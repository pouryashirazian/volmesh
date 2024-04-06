#pragma once

#include <tuple>

namespace volmesh {

  // Primary template for the Attribute
  template<typename... Args>
  struct CustomAttribute;

  // Partial specialization for the case when there are no custom member variables
  template<>
  struct CustomAttribute<> {
  };

  // Partial specialization for the case when there are custom member variables
  template<typename T, typename... Args>
  struct CustomAttribute<T, Args...> : public CustomAttribute<Args...> {
  public:
    CustomAttribute(const T& value, const Args&... args) : value_(value), value_list_(args...), CustomAttribute<Args...>(args...) {}

    template<size_t Index>
    auto& get() {
      if constexpr(Index == 0) {
        return value_;
      } else {
        return std::get<Index>(value_list_);
      }
    }

    template<size_t Index>
    void set(const T& value) {
      if constexpr(Index == 0) {
        value_ = value;
      } else {
        std::get<Index>(value_list_) = value;
      }
    }

  private:
    T value_;
    std::tuple<T, Args...> value_list_;
  };

}