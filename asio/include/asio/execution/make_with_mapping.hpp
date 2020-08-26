//
// execution/make_with_mapping.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_MAKE_WITH_MAPPING_HPP
#define ASIO_EXECUTION_MAKE_WITH_MAPPING_HPP

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
 * The name <tt>execution::make_with_mapping</tt> denotes a customisation point object.
 * The expression <tt>execution::make_with_mapping(T, A)</tt> for some subexpression
 * <tt>T</tt> is expression-equivalent to:
 *
 * @li <tt>make_with_mapping(T, A)</tt>, if that expression is valid, and if the expression 
 *   <tt>asio::tag_invokes::tag_invoke(make_with_mapping, T, A)</tt> is valid. If the function 
 *   selected by overload resolution does not return a new object that combines <tt>T</tt>'s 
 *   implementation with the supplied <tt>A</tt> and that satisfies the mapping denoted by 
 *   <tt>A</tt>, the program is ill-formed with no diagnostic required.
 *
 * @li Otherwise, <tt>execution::make_with_mapping(R, A)</tt> is ill-formed.
 */
inline constexpr unspecified make_with_mapping = unspecified;

/// A type trait that determines whether a @c make_with_mapping expression is
/// well-formed.
/**
 * Class template @c can_make_with_mapping is a trait that is derived from
 * @c true_type if the expression <tt>execution::make_with_mapping(std::declval<T>(), 
 * std::declval<A>())</tt> is well formed; otherwise @c false_type.
 */
template <typename R, typename B>
struct can_make_with_mapping :
  integral_constant<bool, automatically_determined>
{
};

} // namespace execution
} // namespace asio

#else // defined(GENERATING_DOCUMENTATION)

namespace asio_execution_make_with_mapping_fn {

using asio::decay;
using asio::declval;
using asio::enable_if;
using asio::tag_invokes::can_tag_invoke;
using asio::tag_invokes::tag_invoke_result;
using asio::tag_invokes::is_nothrow_tag_invoke;

struct impl
{
#if defined(ASIO_HAS_MOVE)
  template <typename R, typename Mapping>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, R, Mapping>::value,
    typename tag_invoke_result<impl, R, Mapping>::type
  >::type
  operator()(R&& r, Mapping&& f) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, R, Mapping>::value))
  {
    return asio::tag_invokes::tag_invoke(*this, ASIO_MOVE_CAST(R)(r), ASIO_MOVE_CAST(Mapping)(f));
  }
#else // defined(ASIO_HAS_MOVE)
  template <typename R, typename Mapping>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, R&, Mapping>::value,
    typename tag_invoke_result<impl, R&, Mapping>::type
  >::type
  operator()(R& r, Mapping f) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, R&, Mapping>::value))
  {
    return asio::tag_invokes::tag_invoke(*this, r, f);
  }

  template <typename R, typename Mapping>
  ASIO_CONSTEXPR typename enable_if<
    can_tag_invoke<impl, const R&, Mapping>::value,
    typename tag_invoke_result<impl, const R&, Mapping>::type
  >::type
  operator()(const R& r, Mapping f) const
    ASIO_NOEXCEPT_IF((
      is_nothrow_tag_invoke<impl, const R&, Mapping>::value))
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

} // namespace asio_execution_make_with_mapping_fn
namespace asio {
namespace execution {
namespace {

static ASIO_CONSTEXPR const asio_execution_make_with_mapping_fn::impl&
  make_with_mapping = asio_execution_make_with_mapping_fn::static_instance<>::instance;

} // namespace

template <typename R, typename Mapping>
struct can_make_with_mapping :
  integral_constant<bool,
    asio::tag_invokes::can_tag_invoke<asio_execution_make_with_mapping_fn::impl, R, Mapping>::value>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R, typename Mapping>
constexpr bool can_make_with_mapping_v = can_make_with_mapping<R, Mapping>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R, typename Mapping>
struct is_nothrow_make_with_mapping :
  integral_constant<bool,
    asio::tag_invokes::is_nothrow_tag_invoke<asio_execution_make_with_mapping_fn::impl, R, Mapping>::value>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R, typename Mapping>
constexpr bool is_nothrow_make_with_mapping_v
  = is_nothrow_make_with_mapping<R, Mapping>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename R, typename Mapping>
struct make_with_mapping_result : 
  asio::tag_invokes::tag_invoke_result<asio_execution_make_with_mapping_fn::impl, R, Mapping>
{
};

#if defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename R, typename Mapping>
using make_with_mapping_result_t = typename make_with_mapping_result<R, Mapping>::type;

#endif // defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename NewTarget, typename Mapping, typename Target = const asio::tag_invokes::target_&>
struct make_with_mapping_o
{
  typedef asio::tag_invokes::overload<asio_execution_make_with_mapping_fn::impl, 
    NewTarget(Target, Mapping)> type;
};

} // namespace execution
} // namespace asio

#endif // defined(GENERATING_DOCUMENTATION)

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_MAKE_WITH_MAPPING_HPP
