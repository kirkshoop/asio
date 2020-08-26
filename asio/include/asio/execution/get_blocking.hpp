//
// execution/get_blocking.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_GET_BLOCKING_HPP
#define ASIO_EXECUTION_GET_BLOCKING_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"
#include "asio/tag_invokes/tag_invoke.hpp"
#include "asio/tag_invokes/any_ref.hpp"

#include "asio/detail/push_options.hpp"

#if defined(GENERATING_DOCUMENTATION)

namespace asio {
namespace execution {

/// A customisation point that queries a target for a value that indicates blocking behaviour.
/**
 * The name <tt>execution::get_blocking</tt> denotes a customisation point object.
 * The expression <tt>execution::get_blocking(T)</tt> for some subexpression
 * <tt>T</tt> is expression-equivalent to:
 *
 * @li <tt>get_blocking(T)</tt>, if that expression is valid, and if the expression 
 *   <tt>asio::tag_invokes::tag_invoke(get_blocking, T)</tt> is valid.
 *
 * @li Otherwise, <tt>execution::get_blocking(R)</tt> is ill-formed.
 */
inline constexpr unspecified get_blocking = unspecified;

/// A type trait that determines whether a @c get_blocking expression is
/// well-formed.
/**
 * Class template @c can_get_blocking is a trait that is derived from
 * @c true_type if the expression <tt>execution::get_blocking(std::declval<T>())</tt> 
 * is well formed; otherwise @c false_type.
 */
template <typename R>
struct can_get_blocking :
  integral_constant<bool, automatically_determined>
{
};

} // namespace execution
} // namespace asio

#else // defined(GENERATING_DOCUMENTATION)

namespace asio_execution_get_blocking_fn {

using asio::decay;
using asio::declval;
using asio::enable_if;
using asio::tag_invokes::can_tag_invoke;
using asio::tag_invokes::tag_invoke_result;
using asio::tag_invokes::is_nothrow_tag_invoke;

struct possibly_t {
  friend ASIO_CONSTEXPR bool operator==(
      const possibly_t&, const possibly_t&)
  {
    return true;
  }
  friend ASIO_CONSTEXPR bool operator!=(
      const possibly_t&, const possibly_t&)
  {
    return false;
  }
};
struct always_t {
  friend ASIO_CONSTEXPR bool operator==(
      const always_t&, const always_t&)
  {
    return true;
  }
  friend ASIO_CONSTEXPR bool operator!=(
      const always_t&, const always_t&)
  {
    return false;
  }
};
struct never_t {
  friend ASIO_CONSTEXPR bool operator==(
      const never_t&, const never_t&)
  {
    return true;
  }
  friend ASIO_CONSTEXPR bool operator!=(
      const never_t&, const never_t&)
  {
    return false;
  }
};

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

} // namespace asio_execution_get_blocking_fn
namespace asio {
namespace execution {
namespace {

static ASIO_CONSTEXPR const asio_execution_get_blocking_fn::impl&
  get_blocking = asio_execution_get_blocking_fn::static_instance<>::instance;

static ASIO_CONSTEXPR const asio_execution_get_blocking_fn::possibly_t&
  possibly_blocking = asio_execution_get_blocking_fn::static_instance<
    asio_execution_get_blocking_fn::possibly_t>::instance;

static ASIO_CONSTEXPR const asio_execution_get_blocking_fn::always_t&
  always_blocking = asio_execution_get_blocking_fn::static_instance<
    asio_execution_get_blocking_fn::always_t>::instance;

static ASIO_CONSTEXPR const asio_execution_get_blocking_fn::never_t&
  never_blocking = asio_execution_get_blocking_fn::static_instance<
    asio_execution_get_blocking_fn::never_t>::instance;

} // namespace

template <typename R>
struct can_get_blocking :
  integral_constant<bool,
    asio::tag_invokes::can_tag_invoke<asio_execution_get_blocking_fn::impl, R>::value>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R>
constexpr bool can_get_blocking_v = can_get_blocking<R>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R>
struct is_nothrow_get_blocking :
  integral_constant<bool,
    asio::tag_invokes::is_nothrow_tag_invoke<asio_execution_get_blocking_fn::impl, R>::value>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R>
constexpr bool is_nothrow_get_blocking_v
  = is_nothrow_get_blocking<R>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R>
struct get_blocking_result : 
  asio::tag_invokes::tag_invoke_result<asio_execution_get_blocking_fn::impl, R>
{
};

#if defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename R>
using get_blocking_result_t = typename get_blocking_result<R>::type;

#endif // defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename Result = tag_invokes::any_ref<>>
struct get_blocking_o
{
  typedef asio::tag_invokes::overload<asio_execution_get_blocking_fn::impl, 
    Result(const asio::tag_invokes::target_&)> type;
};

} // namespace execution

namespace traits {

#if !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

template <>
struct equality_comparable<
  asio_execution_get_blocking_fn::possibly_t >
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);
};

#endif // !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

#if !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

template <>
struct equality_comparable<
  asio_execution_get_blocking_fn::always_t >
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);
};

#endif // !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

#if !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

template <>
struct equality_comparable<
  asio_execution_get_blocking_fn::never_t >
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);
};

#endif // !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

} // namespace traits
} // namespace asio

#endif // defined(GENERATING_DOCUMENTATION)

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_GET_BLOCKING_HPP
