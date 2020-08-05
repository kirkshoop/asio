//
// execution/detail/as_operation.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_DETAIL_AS_OPERATION_HPP
#define ASIO_EXECUTION_DETAIL_AS_OPERATION_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/memory.hpp"
#include "asio/detail/type_traits.hpp"
#include "asio/execution/detail/as_invocable.hpp"
#include "asio/execution/execute.hpp"
#include "asio/execution/set_error.hpp"
#include "asio/execution/start.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {
namespace execution {
namespace detail {

using asio::enable_if;
using asio::is_same;
using asio::remove_cvref;

template <typename Executor, typename Receiver>
struct as_operation
{
  typename remove_cvref<Executor>::type ex_;
  typename remove_cvref<Receiver>::type receiver_;
#if !defined(ASIO_HAS_MOVE)
  asio::detail::shared_ptr<asio::detail::atomic_count> ref_count_;
#endif // !defined(ASIO_HAS_MOVE)

  template <typename E, typename R>
  explicit as_operation(ASIO_MOVE_ARG(E) e, ASIO_MOVE_ARG(R) r)
    : ex_(ASIO_MOVE_CAST(E)(e)),
      receiver_(ASIO_MOVE_CAST(R)(r))
#if !defined(ASIO_HAS_MOVE)
      , ref_count_(new asio::detail::atomic_count(1))
#endif // !defined(ASIO_HAS_MOVE)
  {
  }

  void tag_invoke(decltype(asio::execution::start)) ASIO_NOEXCEPT
  {
#if !defined(ASIO_NO_EXCEPTIONS)
    try
    {
#endif // !defined(ASIO_NO_EXCEPTIONS)
      execution::execute(
          ASIO_MOVE_CAST(typename remove_cvref<Executor>::type)(ex_),
          as_invocable<typename remove_cvref<Receiver>::type,
              Executor>(receiver_
#if !defined(ASIO_HAS_MOVE)
                , ref_count_
#endif // !defined(ASIO_HAS_MOVE)
              ));
#if !defined(ASIO_NO_EXCEPTIONS)
    }
    catch (...)
    {
#if defined(ASIO_HAS_STD_EXCEPTION_PTR)
      execution::set_error(
          ASIO_MOVE_OR_LVALUE(
            typename remove_cvref<Receiver>::type)(
              receiver_),
          std::current_exception());
#else // defined(ASIO_HAS_STD_EXCEPTION_PTR)
      std::terminate();
#endif // defined(ASIO_HAS_STD_EXCEPTION_PTR)
    }
#endif // !defined(ASIO_NO_EXCEPTIONS)
  }
};

} // namespace detail
} // namespace execution
} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_DETAIL_AS_OPERATION_HPP
