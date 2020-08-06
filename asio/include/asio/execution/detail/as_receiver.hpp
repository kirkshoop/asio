//
// execution/detail/as_receiver.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_DETAIL_AS_RECEIVER_HPP
#define ASIO_EXECUTION_DETAIL_AS_RECEIVER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"
#include "asio/execution/set_done.hpp"
#include "asio/execution/set_error.hpp"
#include "asio/execution/set_value.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {
namespace execution {
namespace detail {

template <typename Function, typename>
struct as_receiver
{
  Function f_;

  template <typename F>
  explicit as_receiver(ASIO_MOVE_ARG(F) f, int)
    : f_(ASIO_MOVE_CAST(F)(f))
  {
  }

#if defined(ASIO_MSVC) && defined(ASIO_HAS_MOVE)
  as_receiver(as_receiver&& other)
    : f_(ASIO_MOVE_CAST(Function)(other.f_))
  {
  }
#endif // defined(ASIO_MSVC) && defined(ASIO_HAS_MOVE)

  friend void tag_invoke(decltype(asio::execution::set_value), ASIO_MOVE_ARG(as_receiver) self)
    ASIO_NOEXCEPT_IF(noexcept(declval<Function&>()()))
  {
    self.f_();
  }

  template <typename E>
  friend void tag_invoke(decltype(asio::execution::set_error), ASIO_MOVE_ARG(as_receiver), ASIO_MOVE_ARG(E) e) ASIO_NOEXCEPT
  {
    std::terminate();
  }

  friend void tag_invoke(decltype(asio::execution::set_done), ASIO_MOVE_ARG(as_receiver)) ASIO_NOEXCEPT
  {
  }
};

template <typename T>
struct is_as_receiver : false_type
{
};

template <typename Function, typename T>
struct is_as_receiver<as_receiver<Function, T> > : true_type
{
};

} // namespace detail
} // namespace execution
} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_DETAIL_AS_RECEIVER_HPP
