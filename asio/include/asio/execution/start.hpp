//
// execution/start.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_START_HPP
#define ASIO_EXECUTION_START_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"
#include "asio/tag_invokes/tag_invoke.hpp"

#include "asio/detail/push_options.hpp"

#if defined(GENERATING_DOCUMENTATION)

namespace asio {
namespace execution {

/// A customisation point that notifies an operation state object to start
/// its associated operation.
/**
 * The name <tt>execution::start</tt> denotes a customisation point object.
 * The expression <tt>execution::start(O)</tt> for some subexpression
 * <tt>O</tt> is expression-equivalent to:
 *
 * @li <tt>start(O)</tt>, if that expression is valid, and if the expression 
 *   <tt>asio::tag_invokes::tag_invoke(start, O)</tt> is valid.
 *
 * @li Otherwise, <tt>execution::start(O)</tt> is ill-formed.
 */
inline constexpr unspecified start = unspecified;

/// A type trait that determines whether a @c start expression is
/// well-formed.
/**
 * Class template @c can_start is a trait that is derived from
 * @c true_type if the expression <tt>execution::start(std::declval<O>())</tt> 
 * is well formed; otherwise @c false_type.
 */
template <typename R>
struct can_start :
  integral_constant<bool, automatically_determined>
{
};

} // namespace execution
} // namespace asio

#else // defined(GENERATING_DOCUMENTATION)

namespace asio_execution_start_fn {

using asio::decay;
using asio::declval;
using asio::enable_if;
using asio::tag_invokes::can_tag_invoke;
using asio::tag_invokes::tag_invoke_result;
using asio::tag_invokes::is_nothrow_tag_invoke;

struct impl
{
#if defined(ASIO_HAS_MOVE)
  template <typename O>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, O>::value,
    typename tag_invoke_result<impl, O>::type
  >::type
  operator()(O&& o) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, O>::value))
  {
    return asio::tag_invokes::tag_invoke(*this, ASIO_MOVE_CAST(O)(o));
  }
#else // defined(ASIO_HAS_MOVE)
  template <typename O>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, O&>::value,
    typename tag_invoke_result<impl, O&>::type
  >::type
  operator()(O& o) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, O&>::value))
  {
    return asio::tag_invokes::tag_invoke(*this, o);
  }

  template <typename O>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, const O&>::value,
    typename tag_invoke_result<impl, const O&>::type
  >::type
  operator()(const O& o) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, const O&>::value))
  {
    return asio::tag_invokes::tag_invoke(*this, o);
  }
#endif // defined(ASIO_HAS_MOVE)
};

template <typename T = impl>
struct static_instance
{
  static const T instance;
};

template <typename T>
const T static_instance<T>::instance = {};

} // namespace asio_execution_start_fn
namespace asio {
namespace execution {
namespace {

static ASIO_CONSTEXPR const asio_execution_start_fn::impl&
  start = asio_execution_start_fn::static_instance<>::instance;

} // namespace

template <typename O>
struct can_start :
  integral_constant<bool,
    asio::tag_invokes::can_tag_invoke<asio_execution_start_fn::impl, O>::value>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename O>
constexpr bool can_start_v = can_start<O>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename O>
struct is_nothrow_start :
  integral_constant<bool,
    asio::tag_invokes::is_nothrow_tag_invoke<asio_execution_start_fn::impl, O>::value>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename O>
constexpr bool is_nothrow_start_v
  = is_nothrow_start<O>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

} // namespace execution
} // namespace asio

#endif // defined(GENERATING_DOCUMENTATION)

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_START_HPP
