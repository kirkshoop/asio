//
// execution/detail/void_receiver.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_DETAIL_VOID_RECEIVER_HPP
#define ASIO_EXECUTION_DETAIL_VOID_RECEIVER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/execution/set_done.hpp"
#include "asio/execution/set_error.hpp"
#include "asio/execution/set_value.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {
namespace execution {
namespace detail {

struct void_receiver
{
  friend void tag_invoke(decltype(execution::set_value), ASIO_MOVE_ARG(void_receiver)) ASIO_NOEXCEPT
  {
  }

  template <typename E>
  friend void tag_invoke(decltype(asio::execution::set_error), ASIO_MOVE_ARG(void_receiver), ASIO_MOVE_ARG(E)) ASIO_NOEXCEPT
  {
  }

  friend void tag_invoke(decltype(execution::set_done), ASIO_MOVE_ARG(void_receiver)) ASIO_NOEXCEPT
  {
  }
};

} // namespace detail
} // namespace execution
} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_DETAIL_VOID_RECEIVER_HPP
