#pragma once

#include <functional>

// see http://derekwyatt.org/2011/07/15/functional-map-in-c/
template <typename InType,
          template <typename U, typename alloc = std::allocator<U>>
          class InContainer,
          template <typename V, typename alloc = std::allocator<V>>
          class OutContainer = InContainer,
          typename OutType   = InType>
OutContainer<OutType> mapf(const InContainer<InType>             &input,
                           std::function<OutType(const InType &)> func) {
  OutContainer<OutType> output;
  output.resize(input.size());
  transform(input.begin(), input.end(), output.begin(), func);
  return output;
}
