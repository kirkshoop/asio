//
// execution/set_allocator.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_SET_ALLOCATOR_HPP
#define ASIO_EXECUTION_SET_ALLOCATOR_HPP

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

/// A customisation point that supplies a target with an allocator value.
/**
 * The name <tt>execution::set_allocator</tt> denotes a customisation point object.
 * The expression <tt>execution::set_allocator(T, A)</tt> for some subexpression
 * <tt>T</tt> is expression-equivalent to:
 *
 * @li <tt>set_allocator(T, A)</tt>, if that expression is valid, and if the expression 
 *   <tt>asio::tag_invokes::tag_invoke(set_allocator, T, A)</tt> is valid. If the function 
 *   selected by overload resolution does not return a new object that combines <tt>T</tt>'s 
 *   implementation with the supplied <tt>A</tt> or the function selected by overload 
 *   resolution does not return the same object with all existing allocations replaced 
 *   with allocations from <tt>A</tt>, the program is ill-formed with no diagnostic 
 *   required.
 *
 * @li Otherwise, <tt>execution::set_allocator(R, A)</tt> is ill-formed.
 */
inline constexpr unspecified set_allocator = unspecified;

/// A type trait that determines whether a @c set_allocator expression is
/// well-formed.
/**
 * Class template @c can_set_allocator is a trait that is derived from
 * @c true_type if the expression <tt>execution::set_allocator(std::declval<T>())</tt> 
 * is well formed; otherwise @c false_type.
 */
template <typename R>
struct can_set_allocator :
  integral_constant<bool, automatically_determined>
{
};

} // namespace execution
} // namespace asio

#else // defined(GENERATING_DOCUMENTATION)

namespace asio_execution_set_allocator_fn {

using asio::decay;
using asio::declval;
using asio::enable_if;
using asio::tag_invokes::can_tag_invoke;
using asio::tag_invokes::tag_invoke_result;
using asio::tag_invokes::is_nothrow_tag_invoke;

struct impl
{
#if defined(ASIO_HAS_MOVE)
  template <typename R, typename Allocator>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, R, Allocator>::value,
    typename tag_invoke_result<impl, R, Allocator>::type
  >::type
  operator()(R&& r, Allocator&& f) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, R, Allocator>::value))
  {
    return asio::tag_invokes::tag_invoke(*this, ASIO_MOVE_CAST(R)(r), ASIO_MOVE_CAST(Allocator)(f));
  }
#else // defined(ASIO_HAS_MOVE)
  template <typename R, typename Allocator>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, R&, Allocator>::value,
    typename tag_invoke_result<impl, R&, Allocator>::type
  >::type
  operator()(R& r, Allocator f) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, R&, Allocator>::value))
  {
    return asio::tag_invokes::tag_invoke(*this, r, f);
  }

  template <typename R, typename Allocator>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, const R&, Allocator>::value,
    typename tag_invoke_result<impl, const R&, Allocator>::type
  >::type
  operator()(const R& r, Allocator f) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, const R&, Allocator>::value))
  {
    return asio::tag_invokes::tag_invoke(*this, r, f);
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

} // namespace asio_execution_set_allocator_fn
namespace asio {
namespace execution {
namespace {

static ASIO_CONSTEXPR const asio_execution_set_allocator_fn::impl&
  set_allocator = asio_execution_set_allocator_fn::static_instance<>::instance;

} // namespace

template <typename R, typename Allocator>
struct can_set_allocator :
  integral_constant<bool,
    asio::tag_invokes::can_tag_invoke<asio_execution_set_allocator_fn::impl, R, Allocator>::value>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R, typename Allocator>
constexpr bool can_set_allocator_v = can_set_allocator<R, Allocator>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R, typename Allocator>
struct is_nothrow_set_allocator :
  integral_constant<bool,
    asio::tag_invokes::is_nothrow_tag_invoke<asio_execution_set_allocator_fn::impl, R, Allocator>::value>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R, typename Allocator>
constexpr bool is_nothrow_set_allocator_v
  = is_nothrow_set_allocator<R, Allocator>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R, typename Allocator>
struct set_allocator_result : 
  asio::tag_invokes::tag_invoke_result<asio_execution_set_allocator_fn::impl, R, Allocator>
{
};

#if defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename R, typename Allocator>
using set_allocator_result_t = typename set_allocator_result<R, Allocator>::type;

#endif // defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename NewR, typename Allocator, typename Target = const asio::tag_invokes::target_&>
struct set_allocator_o
{
  typedef asio::tag_invokes::overload<asio_execution_set_allocator_fn::impl, 
    NewR(Target, Allocator)> type;
};

} // namespace execution
} // namespace asio

#endif // defined(GENERATING_DOCUMENTATION)

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_SET_ALLOCATOR_HPP
