//
// execution/get_occupancy.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_GET_OCCUPANCY_HPP
#define ASIO_EXECUTION_GET_OCCUPANCY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"
#include "asio/tag_invokes/tag_invoke.hpp"
#include "asio/tag_invokes/overload.hpp"
#include "asio/tag_invokes/vtable.hpp"

#include "asio/detail/push_options.hpp"

#if defined(GENERATING_DOCUMENTATION)

namespace asio {
namespace execution {

/// A customisation point that queries a target for an occupancy value.
/**
 * The name <tt>execution::get_occupancy</tt> denotes a customisation point object.
 * The expression <tt>execution::get_occupancy(T)</tt> for some subexpression
 * <tt>T</tt> is expression-equivalent to:
 *
 * @li <tt>get_occupancy(T)</tt>, if that expression is valid, and if the expression 
 *   <tt>asio::tag_invokes::tag_invoke(get_occupancy, T)</tt> is valid. If the function 
 *   selected by overload resolution does not signal the receiver <tt>T</tt>'s done 
 *   channel, the program is ill-formed with no diagnostic required.
 *
 * @li Otherwise, <tt>execution::get_occupancy(R)</tt> is ill-formed.
 */
inline constexpr unspecified get_occupancy = unspecified;

/// A type trait that determines whether a @c get_occupancy expression is
/// well-formed.
/**
 * Class template @c can_get_occupancy is a trait that is derived from
 * @c true_type if the expression <tt>execution::get_occupancy(std::declval<T>())</tt> 
 * is well formed; otherwise @c false_type.
 */
template <typename R>
struct can_get_occupancy :
  integral_constant<bool, automatically_determined>
{
};

} // namespace execution
} // namespace asio

#else // defined(GENERATING_DOCUMENTATION)

namespace asio_execution_get_occupancy_fn {

using asio::decay;
using asio::declval;
using asio::enable_if;
using asio::tag_invokes::can_tag_invoke;
using asio::tag_invokes::tag_invoke_result;
using asio::tag_invokes::is_nothrow_tag_invoke;

struct impl
{
#if defined(ASIO_HAS_MOVE)
  template <typename R>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, R>::value,
    typename tag_invoke_result<impl, R>::type
  >::type
  operator()(R&& r) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, R>::value))
  {
    return asio::tag_invokes::tag_invoke(*this, ASIO_MOVE_CAST(R)(r));
  }
#else // defined(ASIO_HAS_MOVE)
  template <typename R>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, R&>::value,
    typename tag_invoke_result<impl, R&>::type
  >::type
  operator()(R& r) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, R&>::value))
  {
    return asio::tag_invokes::tag_invoke(*this, r);
  }

  template <typename R>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, const R&>::value,
    typename tag_invoke_result<impl, const R&>::type
  >::type
  operator()(const R& r) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, const R&>::value))
  {
    return asio::tag_invokes::tag_invoke(*this, r);
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

} // namespace asio_execution_get_occupancy_fn
namespace asio {
namespace execution {
namespace {

static ASIO_CONSTEXPR const asio_execution_get_occupancy_fn::impl&
  get_occupancy = asio_execution_get_occupancy_fn::static_instance<>::instance;

} // namespace

template <typename R>
struct can_get_occupancy :
  integral_constant<bool,
    asio::tag_invokes::can_tag_invoke<asio_execution_get_occupancy_fn::impl, R>::value>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R>
constexpr bool can_get_occupancy_v = can_get_occupancy<R>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R>
struct is_nothrow_get_occupancy :
  integral_constant<bool,
    asio::tag_invokes::is_nothrow_tag_invoke<asio_execution_get_occupancy_fn::impl, R>::value>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R>
constexpr bool is_nothrow_get_occupancy_v
  = is_nothrow_get_occupancy<R>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R>
struct get_occupancy_result : 
  asio::tag_invokes::tag_invoke_result<asio_execution_get_occupancy_fn::impl, R>
{
};

#if defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename R>
using get_occupancy_result_t = typename get_occupancy_result<R>::type;

#endif // defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename Size = size_t>
struct get_occupancy_o
{
  typedef asio::tag_invokes::overload<asio_execution_get_occupancy_fn::impl, 
    Size(const asio::tag_invokes::target_&)> type;
};

} // namespace execution
} // namespace asio

#endif // defined(GENERATING_DOCUMENTATION)

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_GET_OCCUPANCY_HPP
