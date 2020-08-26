//
// execution/get_mapping.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_GET_MAPPING_HPP
#define ASIO_EXECUTION_GET_MAPPING_HPP

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

/// A customisation point that queries a target to describe what guarantees the 
/// target makes about the mapping of execution agents on to threads of execution.
/**
 * The name <tt>execution::get_mapping</tt> denotes a customisation point object.
 * The expression <tt>execution::get_mapping(T)</tt> for some subexpression
 * <tt>T</tt> is expression-equivalent to:
 *
 * @li <tt>get_mapping(T)</tt>, if that expression is valid, and if the expression 
 *   <tt>asio::tag_invokes::tag_invoke(get_mapping, T)</tt> is valid.
 *
 * @li Otherwise, <tt>execution::get_mapping(R)</tt> is ill-formed.
 */
inline constexpr unspecified get_mapping = unspecified;

/// A type trait that determines whether a @c get_mapping expression is
/// well-formed.
/**
 * Class template @c can_get_mapping is a trait that is derived from
 * @c true_type if the expression <tt>execution::get_mapping(std::declval<T>())</tt> 
 * is well formed; otherwise @c false_type.
 */
template <typename R>
struct can_get_mapping :
  integral_constant<bool, automatically_determined>
{
};

} // namespace execution
} // namespace asio

#else // defined(GENERATING_DOCUMENTATION)

namespace asio_execution_get_mapping_fn {

using asio::decay;
using asio::declval;
using asio::enable_if;
using asio::tag_invokes::can_tag_invoke;
using asio::tag_invokes::tag_invoke_result;
using asio::tag_invokes::is_nothrow_tag_invoke;

struct other_t {
  friend ASIO_CONSTEXPR bool operator==(
      const other_t&, const other_t&)
  {
    return true;
  }
  friend ASIO_CONSTEXPR bool operator!=(
      const other_t&, const other_t&)
  {
    return false;
  }
};
struct new_thread_t {
  friend ASIO_CONSTEXPR bool operator==(
      const new_thread_t&, const new_thread_t&)
  {
    return true;
  }
  friend ASIO_CONSTEXPR bool operator!=(
      const new_thread_t&, const new_thread_t&)
  {
    return false;
  }
};
struct thread_t {
  friend ASIO_CONSTEXPR bool operator==(
      const thread_t&, const thread_t&)
  {
    return true;
  }
  friend ASIO_CONSTEXPR bool operator!=(
      const thread_t&, const thread_t&)
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

} // namespace asio_execution_get_mapping_fn
namespace asio {
namespace execution {
namespace {

static ASIO_CONSTEXPR const asio_execution_get_mapping_fn::impl&
  get_mapping = asio_execution_get_mapping_fn::static_instance<>::instance;

static ASIO_CONSTEXPR const asio_execution_get_mapping_fn::other_t&
  other_mapping = asio_execution_get_mapping_fn::static_instance<
    asio_execution_get_mapping_fn::other_t>::instance;

static ASIO_CONSTEXPR const asio_execution_get_mapping_fn::new_thread_t&
  new_thread_mapping = asio_execution_get_mapping_fn::static_instance<
    asio_execution_get_mapping_fn::new_thread_t>::instance;

static ASIO_CONSTEXPR const asio_execution_get_mapping_fn::thread_t&
  thread_mapping = asio_execution_get_mapping_fn::static_instance<
    asio_execution_get_mapping_fn::thread_t>::instance;

} // namespace

template <typename R>
struct can_get_mapping :
  integral_constant<bool,
    asio::tag_invokes::can_tag_invoke<asio_execution_get_mapping_fn::impl, R>::value>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R>
constexpr bool can_get_mapping_v = can_get_mapping<R>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R>
struct is_nothrow_get_mapping :
  integral_constant<bool,
    asio::tag_invokes::is_nothrow_tag_invoke<asio_execution_get_mapping_fn::impl, R>::value>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R>
constexpr bool is_nothrow_get_mapping_v
  = is_nothrow_get_mapping<R>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R>
struct get_mapping_result : 
  asio::tag_invokes::tag_invoke_result<asio_execution_get_mapping_fn::impl, R>
{
};

#if defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename R>
using get_mapping_result_t = typename get_mapping_result<R>::type;

#endif // defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename Result = tag_invokes::any_ref<>>
struct get_mapping_o
{
  typedef asio::tag_invokes::overload<asio_execution_get_mapping_fn::impl, 
    Result(const asio::tag_invokes::target_&)> type;
};

} // namespace execution

namespace traits {

#if !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

template <>
struct equality_comparable<
  asio_execution_get_mapping_fn::other_t >
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);
};

#endif // !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

#if !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

template <>
struct equality_comparable<
  asio_execution_get_mapping_fn::new_thread_t >
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);
};

#endif // !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

#if !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

template <>
struct equality_comparable<
  asio_execution_get_mapping_fn::thread_t >
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);
};

#endif // !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

} // namespace traits
} // namespace asio

#endif // defined(GENERATING_DOCUMENTATION)

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_GET_MAPPING_HPP
