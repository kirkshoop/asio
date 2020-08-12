//
// tag_invokes/_any_base.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_TAG_INVOKES_ANY_BASE_HPP
#define ASIO_TAG_INVOKES_ANY_BASE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"

#include "asio/tag_invokes/overload.hpp"
#include "asio/tag_invokes/vtable.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {
namespace tag_invokes {

#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <
    typename Derived,
    typename CPO,
    bool NoExcept,
    typename Sig>
struct _any_base;

template <
    typename Derived,
    typename CPO,
    bool NoExcept = false,
    typename Sig = typename CPO::type_erased_signature_t>
using any_base =
    typename _any_base<Derived, CPO, NoExcept, Sig>::type;

template <
    typename Derived,
    typename CPO,
    bool NoExcept,
    typename Ret,
    typename... Args>
struct _any_base<
    Derived,
    CPO,
    NoExcept,
    Ret(Args...)> {
  struct type {
   private:
      template <typename T>
      static void* get_object_address(T&& t) noexcept {
        return static_cast<T&&>(t).get_object_address();
      }
      template <typename T>
      static auto  get_vtable(T&& t) {
        return static_cast<T&&>(t).get_vtable();
      }
   public:
    template <typename DerivedT>
    friend auto tag_invoke(
        base_cpo_t<CPO> cpo,
        DerivedT&& derived,
        Args... args) noexcept(NoExcept) 
        -> std::enable_if_t<
          std::true_type::value,//std::is_base_of_v<std::decay_t<DerivedT>, Derived>, 
          Ret> {
    //   void* objPtr = get_object_address(derived);
    //   auto* fnPtr = get_vtable(derived)->template get<CPO>();
    //   return fnPtr(
    //       std::move(cpo),
    //       objPtr,
    //       (Args&&) args...);
    }
  };
};

template <
    typename Derived,
    typename CPO,
    typename Ret,
    typename... Args>
struct _any_base<
    Derived,
    CPO,
    false,
    Ret(Args...) noexcept>
  : _any_base<Derived, CPO, true, Ret(Args...)> {
};

#endif // defined(ASIO_HAS_VARIADIC_TEMPLATES)

} // namespace tag_invokes
} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_TAG_INVOKES_ANY_BASE_HPP
