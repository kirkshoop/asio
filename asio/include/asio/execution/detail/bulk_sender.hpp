//
// execution/detail/bulk_sender.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_DETAIL_BULK_SENDER_HPP
#define ASIO_EXECUTION_DETAIL_BULK_SENDER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"
#include "asio/execution/connect.hpp"
#include "asio/execution/executor.hpp"
#include "asio/execution/set_done.hpp"
#include "asio/execution/set_error.hpp"
#include "asio/traits/set_value_member.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {
namespace execution {
namespace detail {

template <typename Receiver, typename Function, typename Number, typename Index>
struct bulk_receiver
{
  typename remove_cvref<Receiver>::type receiver_;
  typename decay<Function>::type f_;
  typename decay<Number>::type n_;

  template <typename R, typename F, typename N>
  explicit bulk_receiver(ASIO_MOVE_ARG(R) r,
      ASIO_MOVE_ARG(F) f, ASIO_MOVE_ARG(N) n)
    : receiver_(ASIO_MOVE_CAST(R)(r)),
      f_(ASIO_MOVE_CAST(F)(f)),
      n_(ASIO_MOVE_CAST(N)(n))
  {
  }

  void set_value()
  {
    for (Index i = 0; i < n_; ++i)
      f_(i);

    execution::set_value(
        ASIO_MOVE_OR_LVALUE(
          typename remove_cvref<Receiver>::type)(receiver_));
  }

  template <typename Error>
  friend void tag_invoke(decltype(asio::execution::set_error), ASIO_MOVE_ARG(bulk_receiver) self, ASIO_MOVE_ARG(Error) e) ASIO_NOEXCEPT
  {
    execution::set_error(
        ASIO_MOVE_OR_LVALUE(
          typename remove_cvref<Receiver>::type)(self.receiver_),
        ASIO_MOVE_CAST(Error)(e));
  }

  friend void tag_invoke(decltype(execution::set_done), ASIO_MOVE_ARG(bulk_receiver) self) ASIO_NOEXCEPT
  {
    execution::set_done(
        ASIO_MOVE_OR_LVALUE(
          typename remove_cvref<Receiver>::type)(self.receiver_));
  }
};

template <typename Sender, typename Receiver,
  typename Function, typename Number>
struct bulk_receiver_traits
{
  typedef bulk_receiver<
      Receiver, Function, Number,
      typename execution::executor_index<
        typename remove_cvref<Sender>::type
      >::type
    > type;

#if defined(ASIO_HAS_MOVE)
  typedef type arg_type;
#else // defined(ASIO_HAS_MOVE)
  typedef const type& arg_type;
#endif // defined(ASIO_HAS_MOVE)
};

template <typename Sender, typename Function, typename Number>
struct bulk_sender : sender_base
{
  typename remove_cvref<Sender>::type sender_;
  typename decay<Function>::type f_;
  typename decay<Number>::type n_;

  template <typename S, typename F, typename N>
  explicit bulk_sender(ASIO_MOVE_ARG(S) s,
      ASIO_MOVE_ARG(F) f, ASIO_MOVE_ARG(N) n)
    : sender_(ASIO_MOVE_CAST(S)(s)),
      f_(ASIO_MOVE_CAST(F)(f)),
      n_(ASIO_MOVE_CAST(N)(n))
  {
  }

  template <typename Receiver>
  friend
  typename enable_if<
    can_connect<
      typename remove_cvref<Sender>::type,
      typename bulk_receiver_traits<
        Sender, Receiver, Function, Number
      >::arg_type
    >::value,
    typename connect_result<
      ASIO_MOVE_OR_LVALUE_TYPE(typename remove_cvref<Sender>::type),
      typename bulk_receiver_traits<
        Sender, Receiver, Function, Number
      >::arg_type
    >::type
  >::type tag_invoke(decltype(execution::connect), bulk_sender&& that, ASIO_MOVE_ARG(Receiver) r) ASIO_NOEXCEPT
  {
    return execution::connect(
        ASIO_MOVE_OR_LVALUE(typename remove_cvref<Sender>::type)(that.sender_),
        typename bulk_receiver_traits<Sender, Receiver, Function, Number>::type(
          ASIO_MOVE_CAST(Receiver)(r),
          ASIO_MOVE_CAST(typename decay<Function>::type)(that.f_),
          ASIO_MOVE_CAST(typename decay<Number>::type)(that.n_)));
  }

  template <typename Receiver>
  friend
  typename enable_if<
    can_connect<
      const typename remove_cvref<Sender>::type&,
      typename bulk_receiver_traits<
        Sender, Receiver, Function, Number
      >::arg_type
    >::value,
    typename connect_result<
      const typename remove_cvref<Sender>::type&,
      typename bulk_receiver_traits<
        Sender, Receiver, Function, Number
      >::arg_type
    >::type 
  >::type tag_invoke(decltype(execution::connect), const bulk_sender& that, ASIO_MOVE_ARG(Receiver) r) ASIO_NOEXCEPT
  {
    return execution::connect(that.sender_,
        typename bulk_receiver_traits<Sender, Receiver, Function, Number>::type(
          ASIO_MOVE_CAST(Receiver)(r), that.f_, that.n_));
  }
};

} // namespace detail
} // namespace execution
namespace traits {

#if !defined(ASIO_HAS_DEDUCED_SET_VALUE_MEMBER_TRAIT)

template <typename Receiver, typename Function, typename Number, typename Index>
struct set_value_member<
    execution::detail::bulk_receiver<Receiver, Function, Number, Index>,
    void()>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef void result_type;
};

#endif // !defined(ASIO_HAS_DEDUCED_SET_VALUE_MEMBER_TRAIT)

} // namespace traits
} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_DETAIL_BULK_SENDER_HPP
