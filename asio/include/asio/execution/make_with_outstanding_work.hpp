//
// execution/make_with_outstanding_work.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_MAKE_WITH_OUTSTANDING_WORK_HPP
#define ASIO_EXECUTION_MAKE_WITH_OUTSTANDING_WORK_HPP

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

/// A customisation point that constructs a type that satisfies the specified mapping.
/**
 * The name <tt>execution::make_with_outstanding_work</tt> denotes a customisation point object.
 * The expression <tt>execution::make_with_outstanding_work(T, A)</tt> for some subexpression
 * <tt>T</tt> is expression-equivalent to:
 *
 * @li <tt>make_with_outstanding_work(T, A)</tt>, if that expression is valid, and if the expression 
 *   <tt>asio::tag_invokes::tag_invoke(make_with_outstanding_work, T, A)</tt> is valid. If the function 
 *   selected by overload resolution does not return a new object that combines <tt>T</tt>'s 
 *   implementation with the supplied <tt>A</tt> and that satisfies the mapping denoted by 
 *   <tt>A</tt>, the program is ill-formed with no diagnostic required.
 *
 * @li Otherwise, <tt>execution::make_with_outstanding_work(R, A)</tt> is ill-formed.
 */
inline constexpr unspecified make_with_outstanding_work = unspecified;

/// A type trait that determines whether a @c make_with_outstanding_work expression is
/// well-formed.
/**
 * Class template @c can_make_with_outstanding_work is a trait that is derived from
 * @c true_type if the expression <tt>execution::make_with_outstanding_work(std::declval<T>(), 
 * std::declval<A>())</tt> is well formed; otherwise @c false_type.
 */
template <typename R, typename B>
struct can_make_with_outstanding_work :
  integral_constant<bool, automatically_determined>
{
};

} // namespace execution
} // namespace asio

#else // defined(GENERATING_DOCUMENTATION)

namespace asio_execution_make_with_outstanding_work_fn {

using asio::decay;
using asio::declval;
using asio::enable_if;
using asio::tag_invokes::can_tag_invoke;
using asio::tag_invokes::tag_invoke_result;
using asio::tag_invokes::is_nothrow_tag_invoke;

struct impl
{
#if defined(ASIO_HAS_MOVE)
  template <typename R, typename OutstandingWork>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, R, OutstandingWork>::value,
    typename tag_invoke_result<impl, R, OutstandingWork>::type
  >::type
  operator()(R&& r, OutstandingWork&& f) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, R, OutstandingWork>::value))
  {
    return asio::tag_invokes::tag_invoke(*this, ASIO_MOVE_CAST(R)(r), ASIO_MOVE_CAST(OutstandingWork)(f));
  }
#else // defined(ASIO_HAS_MOVE)
  template <typename R, typename OutstandingWork>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, R&, OutstandingWork>::value,
    typename tag_invoke_result<impl, R&, OutstandingWork>::type
  >::type
  operator()(R& r, OutstandingWork f) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, R&, OutstandingWork>::value))
  {
    return asio::tag_invokes::tag_invoke(*this, r, f);
  }

  template <typename R, typename OutstandingWork>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, const R&, OutstandingWork>::value,
    typename tag_invoke_result<impl, const R&, OutstandingWork>::type
  >::type
  operator()(const R& r, OutstandingWork f) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, const R&, OutstandingWork>::value))
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

} // namespace asio_execution_make_with_outstanding_work_fn
namespace asio {
namespace execution {
namespace {

static ASIO_CONSTEXPR const asio_execution_make_with_outstanding_work_fn::impl&
  make_with_outstanding_work = asio_execution_make_with_outstanding_work_fn::static_instance<>::instance;

} // namespace

template <typename R, typename OutstandingWork>
struct can_make_with_outstanding_work :
  integral_constant<bool,
    asio::tag_invokes::can_tag_invoke<asio_execution_make_with_outstanding_work_fn::impl, R, OutstandingWork>::value>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R, typename OutstandingWork>
constexpr bool can_make_with_outstanding_work_v = can_make_with_outstanding_work<R, OutstandingWork>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R, typename OutstandingWork>
struct is_nothrow_make_with_outstanding_work :
  integral_constant<bool,
    asio::tag_invokes::is_nothrow_tag_invoke<asio_execution_make_with_outstanding_work_fn::impl, R, OutstandingWork>::value>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R, typename OutstandingWork>
constexpr bool is_nothrow_make_with_outstanding_work_v
  = is_nothrow_make_with_outstanding_work<R, OutstandingWork>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R, typename OutstandingWork>
struct make_with_outstanding_work_result : 
  asio::tag_invokes::tag_invoke_result<asio_execution_make_with_outstanding_work_fn::impl, R, OutstandingWork>
{
};

#if defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename R, typename OutstandingWork>
using make_with_outstanding_work_result_t = typename make_with_outstanding_work_result<R, OutstandingWork>::type;

#endif // defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename NewTarget, typename OutstandingWork, typename Target = const asio::tag_invokes::target_&>
struct make_with_outstanding_work_o
{
  typedef asio::tag_invokes::overload<asio_execution_make_with_outstanding_work_fn::impl, 
    NewTarget(Target, OutstandingWork)> type;
};

} // namespace execution
} // namespace asio

#endif // defined(GENERATING_DOCUMENTATION)

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_MAKE_WITH_OUTSTANDING_WORK_HPP
