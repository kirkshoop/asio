//
// tag_invokes/tag_invoke.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_TAG_INVOKE_HPP
#define ASIO_EXECUTION_TAG_INVOKE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"
#include "asio/detail/variadic_templates.hpp"
#include "asio/traits/tag_invoke_member.hpp"
#include "asio/traits/tag_invoke_free.hpp"

#include "asio/detail/push_options.hpp"

#if defined(GENERATING_DOCUMENTATION)

namespace asio {
namespace tag_invokes {

/// A customisation point that delivers a value to a receiver.
/**
 * The name <tt>tag_invokes::tag_invoke</tt> denotes a customisation point object.
 * The expression <tt>tag_invokes::tag_invoke(CPO, T, Vs...)</tt> for some
 * subexpressions <tt>T</tt> and <tt>Vs...</tt> is expression-equivalent to:
 *
 * @li <tt>T.tag_invoke(CPO, Vs...)</tt>, if that expression is valid. If the
 *   function selected does not send the value(s) <tt>Vs...</tt> to the target
 *   <tt>T</tt>'s customization of tag_invoke, the program is ill-formed with no 
 *   diagnostic required.
 *
 * @li Otherwise, <tt>tag_invoke(CPO, T, Vs...)</tt>, if that expression is valid,
 *   with overload resolution performed in a context that includes the
 *   declaration <tt>void tag_invoke();</tt> and that does not include a
 *   declaration of <tt>tag_invokes::tag_invoke</tt>. If the function selected by
 *   overload resolution does not send the value(s) <tt>Vs...</tt> to the
 *   target <tt>T</tt>'s customisation of tag_invoke, the program is ill-formed with 
 *   no diagnostic required.
 *
 * @li Otherwise, <tt>tag_invokes::tag_invoke(CPO, T, Vs...)</tt> is ill-formed.
 */
inline constexpr unspecified tag_invoke = unspecified;

/// A type trait that determines whether a @c tag_invoke expression is
/// well-formed.
/**
 * Class template @c can_tag_invoke is a trait that is derived from
 * @c true_type if the expression <tt>tag_invokes::tag_invoke(std::declval<CPO>(),
 * std::declval<T>(), std::declval<Vs>()...)</tt> is well formed; otherwise  
 * @c false_type.
 */
template <typename CPO, typename T, typename... Vs>
struct can_tag_invoke :
  integral_constant<bool, automatically_determined>
{
};

/// A type trait to determine the result of a @c tag_invoke expression.
template <typename CPO, typename T, typename... Vs>
struct tag_invoke_result
{
  /// The type of the tag_invoke expression.
  /**
   * The type of the expression <tt>tag_invokes::tag_invoke(std::declval<CPO>(),
   * std::declval<T>(), std::declval<Vs>()...)</tt>.
   */
  typedef automatically_determined type;
};

/// A type alias to determine the result of a @c tag_invoke expression.
template <typename CPO, typename T, typename... Vs>
using tag_invoke_t = typename tag_invoke_result<CPO, T, Vs...>::type;

/// The result of a @c tag_invoke expression at compile-time.
inline constexpr unspecified static_tag_invoke_v = unspecified;

/// A type trait that determines whether a @c tag_invoke expression is
/// well-formed at compile-time.
/**
 * Class template @c can_tag_invoke is a trait that is derived from
 * @c true_type if the expression <tt>tag_invokes::tag_invoke(std::declval<CPO>(),
 * std::declval<T>(), std::declval<Vs>()...)</tt> is well formed; otherwise  
 * @c false_type.
 */
template <typename CPO, typename T, typename... Vs>
struct can_static_tag_invoke :
  integral_constant<bool, automatically_determined>
{
};

/// A type trait to determine the result of a @c tag_invoke expression at compile-time.
template <typename CPO, typename T, typename... Vs>
struct static_tag_invoke_result
{
  /// The type of the tag_invoke expression.
  typedef automatically_determined type;
};

/// A type alias to determine the result of a @c tag_invoke expression at compile-time.
template <typename CPO, typename T, typename... Vs>
using static_tag_invoke_result_t = typename static_tag_invoke_result<CPO, T, Vs...>::type;

} // namespace tag_invokes
} // namespace asio

#else // defined(GENERATING_DOCUMENTATION)

namespace asio_tag_invokes_tag_invoke_fn {

using asio::decay;
using asio::declval;
using asio::enable_if;
using asio::traits::tag_invoke_free;
using asio::traits::tag_invoke_member;

void tag_invoke();

enum overload_type
{
  call_member,
  call_free,
  ill_formed
};

template <typename CPO, typename T, typename Vs, typename = void>
struct call_traits
{
  ASIO_STATIC_CONSTEXPR(overload_type, overload = ill_formed);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef void result_type;
};

template <typename CPO, typename T, typename Vs>
struct call_traits<CPO, T, Vs,
  typename enable_if<
    (
      tag_invoke_member<CPO, T, Vs>::is_valid
    )
  >::type> :
  tag_invoke_member<CPO, T, Vs>
{
  ASIO_STATIC_CONSTEXPR(overload_type, overload = call_member);
};

template <typename CPO, typename T, typename Vs>
struct call_traits<CPO, T, Vs,
  typename enable_if<
    (
      !tag_invoke_member<CPO, T, Vs>::is_valid
      &&
      tag_invoke_free<CPO, T, Vs>::is_valid
    )
  >::type> :
  tag_invoke_free<CPO, T, Vs>
{
  ASIO_STATIC_CONSTEXPR(overload_type, overload = call_free);
};

struct impl
{
#if defined(ASIO_HAS_MOVE)

#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

  template <typename CPO, typename T, typename... Vs>
  ASIO_CONSTEXPR typename enable_if<
    call_traits<CPO, T, void(Vs...)>::overload == call_member,
    typename call_traits<CPO, T, void(Vs...)>::result_type
  >::type
  operator()(CPO, T&& t, Vs&&... v) const
    ASIO_NOEXCEPT_IF((
      call_traits<CPO, T, void(Vs...)>::is_noexcept))
  {
    return ASIO_MOVE_CAST(T)(t).tag_invoke(CPO{}, ASIO_MOVE_CAST(Vs)(v)...);
  }

  template <typename CPO, typename T, typename... Vs>
  ASIO_CONSTEXPR typename enable_if<
    call_traits<CPO, T, void(Vs...)>::overload == call_free,
    typename call_traits<CPO, T, void(Vs...)>::result_type
  >::type
  operator()(CPO, T&& t, Vs&&... v) const
    ASIO_NOEXCEPT_IF((
      call_traits<CPO, T, void(Vs...)>::is_noexcept))
  {
    return tag_invoke(CPO{}, ASIO_MOVE_CAST(T)(t),
        ASIO_MOVE_CAST(Vs)(v)...);
  }

#else // defined(ASIO_HAS_VARIADIC_TEMPLATES)

  template <typename T>
  ASIO_CONSTEXPR typename enable_if<
    call_traits<CPO, T, void()>::overload == call_member,
    typename call_traits<CPO, T, void()>::result_type
  >::type
  operator()(CPO, T&& t) const
    ASIO_NOEXCEPT_IF((
      call_traits<CPO, T, void()>::is_noexcept))
  {
    return ASIO_MOVE_CAST(T)(t).tag_invoke(CPO{});
  }

  template <typename T>
  ASIO_CONSTEXPR typename enable_if<
    call_traits<CPO, T, void()>::overload == call_free,
    typename call_traits<CPO, T, void()>::result_type
  >::type
  operator()(CPO, T&& t) const
    ASIO_NOEXCEPT_IF((
      call_traits<CPO, T, void()>::is_noexcept))
  {
    return tag_invoke(CPO{}, ASIO_MOVE_CAST(T)(t));
  }

#define ASIO_PRIVATE_TAG_INVOKE_CALL_DEF(n) \
  template <typename CPO, typename T, ASIO_VARIADIC_TPARAMS(n)> \
  ASIO_CONSTEXPR typename enable_if< \
    call_traits<CPO, T, \
      void(ASIO_VARIADIC_TARGS(n))>::overload == call_member, \
    typename call_traits<CPO, T, void(ASIO_VARIADIC_TARGS(n))>::result_type \
  >::type \
  operator()(CPO, T&& t, ASIO_VARIADIC_MOVE_PARAMS(n)) const \
    ASIO_NOEXCEPT_IF(( \
      call_traits<CPO, T, void(ASIO_VARIADIC_TARGS(n))>::is_noexcept)) \
  { \
    return ASIO_MOVE_CAST(T)(t).tag_invoke( \
        CPO{}, ASIO_VARIADIC_MOVE_ARGS(n)); \
  } \
  \
  template <typename CPO, typename T, ASIO_VARIADIC_TPARAMS(n)> \
  ASIO_CONSTEXPR typename enable_if< \
    call_traits<CPO, T, void(ASIO_VARIADIC_TARGS(n))>::overload == call_free, \
    typename call_traits<CPO, T, void(ASIO_VARIADIC_TARGS(n))>::result_type \
  >::type \
  operator()(CPO, T&& t, ASIO_VARIADIC_MOVE_PARAMS(n)) const \
    ASIO_NOEXCEPT_IF(( \
      call_traits<CPO, T, void(ASIO_VARIADIC_TARGS(n))>::is_noexcept)) \
  { \
    return tag_invoke(CPO{}, ASIO_MOVE_CAST(T)(t), \
        ASIO_VARIADIC_MOVE_ARGS(n)); \
  } \
  /**/
ASIO_VARIADIC_GENERATE(ASIO_PRIVATE_TAG_INVOKE_CALL_DEF)
#undef ASIO_PRIVATE_TAG_INVOKE_CALL_DEF

#endif // defined(ASIO_HAS_VARIADIC_TEMPLATES)

#else // defined(ASIO_HAS_MOVE)

#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

  template <typename CPO, typename T, typename... Vs>
  ASIO_CONSTEXPR typename enable_if<
    call_traits<CPO, T&, void(const Vs&...)>::overload == call_member,
    typename call_traits<CPO, T&, void(const Vs&...)>::result_type
  >::type
  operator()(CPO, T& t, const Vs&... v) const
    ASIO_NOEXCEPT_IF((
      call_traits<CPO, T&, void(const Vs&...)>::is_noexcept))
  {
    return t.tag_invoke(CPO{}, v...);
  }

  template <typename CPO, typename T, typename... Vs>
  ASIO_CONSTEXPR typename enable_if<
    call_traits<CPO, const T&, void(const Vs&...)>::overload == call_member,
    typename call_traits<CPO, const T&, void(const Vs&...)>::result_type
  >::type
  operator()(CPO, const T& t, const Vs&... v) const
    ASIO_NOEXCEPT_IF((
      call_traits<CPO, const T&, void(const Vs&...)>::is_noexcept))
  {
    return t.tag_invoke(CPO{}, v...);
  }

  template <typename CPO, typename T, typename... Vs>
  ASIO_CONSTEXPR typename enable_if<
    call_traits<CPO, T&, void(const Vs&...)>::overload == call_free,
    typename call_traits<CPO, T&, void(const Vs&...)>::result_type
  >::type
  operator()(CPO, T& t, const Vs&... v) const
    ASIO_NOEXCEPT_IF((
      call_traits<CPO, T&, void(const Vs&...)>::is_noexcept))
  {
    return tag_invoke(CPO{}, t, v...);
  }

  template <typename CPO, typename T, typename... Vs>
  ASIO_CONSTEXPR typename enable_if<
    call_traits<CPO, const T&, void(const Vs&...)>::overload == call_free,
    typename call_traits<CPO, const T&, void(const Vs&...)>::result_type
  >::type
  operator()(CPO, const T& t, const Vs&... v) const
    ASIO_NOEXCEPT_IF((
      call_traits<CPO, const T&, void(const Vs&...)>::is_noexcept))
  {
    return tag_invoke(CPO{}, t, v...);
  }

#else // defined(ASIO_HAS_VARIADIC_TEMPLATES)

  template <typename T>
  ASIO_CONSTEXPR typename enable_if<
    call_traits<CPO, T&, void()>::overload == call_member,
    typename call_traits<CPO, T&, void()>::result_type
  >::type
  operator()(CPO, T& t) const
    ASIO_NOEXCEPT_IF((
      call_traits<CPO, T&, void()>::is_noexcept))
  {
    return t.tag_invoke(CPO{});
  }

  template <typename T>
  ASIO_CONSTEXPR typename enable_if<
    call_traits<CPO, const T&, void()>::overload == call_member,
    typename call_traits<CPO, const T&, void()>::result_type
  >::type
  operator()(CPO, const T& t) const
    ASIO_NOEXCEPT_IF((
      call_traits<CPO, const T&, void()>::is_noexcept))
  {
    return t.tag_invoke(CPO{});
  }

  template <typename T>
  ASIO_CONSTEXPR typename enable_if<
    call_traits<CPO, T&, void()>::overload == call_free,
    typename call_traits<CPO, T&, void()>::result_type
  >::type
  operator()(CPO, T& t) const
    ASIO_NOEXCEPT_IF((
      call_traits<CPO, T&, void()>::is_noexcept))
  {
    return tag_invoke(CPO{}, t);
  }

  template <typename T>
  ASIO_CONSTEXPR typename enable_if<
    call_traits<CPO, const T&, void()>::overload == call_free,
    typename call_traits<CPO, const T&, void()>::result_type
  >::type
  operator()(CPO, const T& t) const
    ASIO_NOEXCEPT_IF((
      call_traits<CPO, const T&, void()>::is_noexcept))
  {
    return tag_invoke(CPO{}, t);
  }

#define ASIO_PRIVATE_TAG_INVOKE_CALL_DEF(n) \
  template <typename CPO, typename T, ASIO_VARIADIC_TPARAMS(n)> \
  ASIO_CONSTEXPR typename enable_if< \
    call_traits<CPO, T&, \
      void(ASIO_VARIADIC_TARGS(n))>::overload == call_member, \
    typename call_traits<CPO, T&, void(ASIO_VARIADIC_TARGS(n))>::result_type \
  >::type \
  operator()(CPO, T& t, ASIO_VARIADIC_MOVE_PARAMS(n)) const \
    ASIO_NOEXCEPT_IF(( \
      call_traits<CPO, T&, void(ASIO_VARIADIC_TARGS(n))>::is_noexcept)) \
  { \
    return t.tag_invoke(CPO{}, ASIO_VARIADIC_MOVE_ARGS(n)); \
  } \
  \
  template <typename CPO, typename T, ASIO_VARIADIC_TPARAMS(n)> \
  ASIO_CONSTEXPR typename enable_if< \
    call_traits<CPO, const T&, \
      void(ASIO_VARIADIC_TARGS(n))>::overload == call_member, \
    typename call_traits<CPO, const T&, \
      void(ASIO_VARIADIC_TARGS(n))>::result_type \
  >::type \
  operator()(CPO, const T& t, ASIO_VARIADIC_MOVE_PARAMS(n)) const \
    ASIO_NOEXCEPT_IF(( \
      call_traits<CPO, const T&, void(ASIO_VARIADIC_TARGS(n))>::is_noexcept)) \
  { \
    return t.tag_invoke(CPO{}, ASIO_VARIADIC_MOVE_ARGS(n)); \
  } \
  \
  template <typename CPO, typename T, ASIO_VARIADIC_TPARAMS(n)> \
  ASIO_CONSTEXPR typename enable_if< \
    call_traits<CPO, T&, \
      void(ASIO_VARIADIC_TARGS(n))>::overload == call_free, \
    typename call_traits<CPO, T&, void(ASIO_VARIADIC_TARGS(n))>::result_type \
  >::type \
  operator()(CPO, T& t, ASIO_VARIADIC_MOVE_PARAMS(n)) const \
    ASIO_NOEXCEPT_IF(( \
      call_traits<CPO, T&, void(ASIO_VARIADIC_TARGS(n))>::is_noexcept)) \
  { \
    return tag_invoke(CPO{}, t, ASIO_VARIADIC_MOVE_ARGS(n)); \
  } \
  \
  template <typename CPO, typename T, ASIO_VARIADIC_TPARAMS(n)> \
  ASIO_CONSTEXPR typename enable_if< \
    call_traits<CPO, const T&, \
      void(ASIO_VARIADIC_TARGS(n))>::overload == call_free, \
    typename call_traits<CPO, const T&, \
      void(ASIO_VARIADIC_TARGS(n))>::result_type \
  >::type \
  operator()(CPO, const T& t, ASIO_VARIADIC_MOVE_PARAMS(n)) const \
    ASIO_NOEXCEPT_IF(( \
      call_traits<CPO, const T&, void(ASIO_VARIADIC_TARGS(n))>::is_noexcept)) \
  { \
    return tag_invoke(CPO{}, t, ASIO_VARIADIC_MOVE_ARGS(n)); \
  } \
  /**/
ASIO_VARIADIC_GENERATE(ASIO_PRIVATE_TAG_INVOKE_CALL_DEF)
#undef ASIO_PRIVATE_TAG_INVOKE_CALL_DEF

#endif // defined(ASIO_HAS_VARIADIC_TEMPLATES)

#endif // defined(ASIO_HAS_MOVE)
};

template<typename T>
struct any_instance_of {
    ASIO_CONSTEXPR any_instance_of() noexcept = default;

    // Instance of 'type' is implicitly constructible from any instance of this type.
    /*implicit*/ ASIO_CONSTEXPR any_instance_of(const typename asio::remove_cvref<T>::type&) noexcept {}
};

// from Richard Smith's comment: https://stackoverflow.com/a/50169108
template<int (*p)()> std::true_type is_constexpr_impl(decltype(int{(p(), 0U)}));
template<int (*p)()> std::false_type is_constexpr_impl(...);

// #if defined(ASIO_HAS_ALIAS_TEMPLATES)

// template<int (*p)()> using is_constexpr = decltype(is_constexpr_impl<p>(0));

// #else // defined(ASIO_HAS_ALIAS_TEMPLATES)

template<int (*p)()> struct is_constexpr : decltype(is_constexpr_impl<p>(0)) {};

// #endif // defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename T = impl>
struct static_instance
{
  static const T instance;
};

template <typename T>
const T static_instance<T>::instance = {};

} // namespace asio_tag_invokes_tag_invoke_fn
namespace asio {
namespace tag_invokes {
namespace {

static ASIO_CONSTEXPR const asio_tag_invokes_tag_invoke_fn::impl&
  tag_invoke = asio_tag_invokes_tag_invoke_fn::static_instance<>::instance;

using asio_tag_invokes_tag_invoke_fn::any_instance_of;

} // namespace

#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename CPO, typename T, typename... Vs>
struct can_tag_invoke :
  integral_constant<bool,
    asio_tag_invokes_tag_invoke_fn::call_traits<CPO, T, void(Vs...)>::overload !=
      asio_tag_invokes_tag_invoke_fn::ill_formed>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename CPO, typename T, typename... Vs>
ASIO_CONSTEXPR bool can_tag_invoke_v = can_tag_invoke<CPO, T, Vs...>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename CPO, typename T, typename... Vs>
struct is_nothrow_tag_invoke :
  integral_constant<bool,
    asio_tag_invokes_tag_invoke_fn::call_traits<CPO, T, void(Vs...)>::is_noexcept>
{
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename CPO, typename T, typename... Vs>
ASIO_CONSTEXPR bool is_nothrow_tag_invoke_v
  = is_nothrow_tag_invoke<CPO, T, Vs...>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename CPO, typename T, typename... Vs>
struct tag_invoke_result
{
  typedef typename asio_tag_invokes_tag_invoke_fn::call_traits<
      CPO, T, void(Vs...)>::result_type type;
};

#if defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename CPO, typename T, typename... Vs>
using tag_invoke_result_t = typename tag_invoke_result<CPO, T, Vs...>::type;

#endif // defined(ASIO_HAS_ALIAS_TEMPLATES)

#else // defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename CPO, typename T, typename = void,
    typename = void, typename = void, typename = void, typename = void,
    typename = void, typename = void, typename = void, typename = void>
struct can_tag_invoke;

template <typename CPO, typename T, typename = void,
    typename = void, typename = void, typename = void, typename = void,
    typename = void, typename = void, typename = void, typename = void>
struct is_nothrow_tag_invoke;

template <typename CPO, typename T, typename = void,
    typename = void, typename = void, typename = void, typename = void,
    typename = void, typename = void, typename = void, typename = void>
struct tag_invoke_result;

template <typename CPO, typename T>
struct can_tag_invoke<CPO, T> :
  integral_constant<bool,
    asio_tag_invokes_tag_invoke_fn::call_traits<CPO, T, void()>::overload !=
      asio_tag_invokes_tag_invoke_fn::ill_formed>
{
};

template <typename CPO, typename T>
struct is_nothrow_tag_invoke<CPO, T> :
  integral_constant<bool,
    asio_tag_invokes_tag_invoke_fn::call_traits<CPO, T, void()>::is_noexcept>
{
};

template <typename CPO, typename T>
struct tag_invoke_result<CPO, T>
{
  typedef typename asio_tag_invokes_tag_invoke_fn::call_traits<
      CPO, T, void()>::result_type type;
};

#define ASIO_PRIVATE_TAG_INVOKE_TRAITS_DEF(n) \
  template <typename CPO, typename T, ASIO_VARIADIC_TPARAMS(n)> \
  struct can_tag_invoke<CPO, T, ASIO_VARIADIC_TARGS(n)> : \
    integral_constant<bool, \
      asio_tag_invokes_tag_invoke_fn::call_traits<CPO, T, \
        void(ASIO_VARIADIC_TARGS(n))>::overload != \
          asio_tag_invokes_tag_invoke_fn::ill_formed> \
  { \
  }; \
  \
  template <typename CPO, typename T, ASIO_VARIADIC_TPARAMS(n)> \
  struct is_nothrow_tag_invoke<CPO, T, ASIO_VARIADIC_TARGS(n)> : \
    integral_constant<bool, \
      asio_tag_invokes_tag_invoke_fn::call_traits<CPO, T, \
        void(ASIO_VARIADIC_TARGS(n))>::is_noexcept> \
  { \
  }; \
  \
  template <typename CPO, typename T, ASIO_VARIADIC_TPARAMS(n)> \
  struct tag_invoke_result<CPO, T, ASIO_VARIADIC_TARGS(n)> \
  { \
    typedef typename asio_tag_invokes_tag_invoke_fn::call_traits< \
      CPO, T, void(ASIO_VARIADIC_TARGS(n))>::result_type type; \
  }; \
  /**/
ASIO_VARIADIC_GENERATE(ASIO_PRIVATE_TAG_INVOKE_TRAITS_DEF)
#undef ASIO_PRIVATE_TAG_INVOKE_TRAITS_DEF

#endif // defined(ASIO_HAS_VARIADIC_TEMPLATES)

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template<typename T>
inline ASIO_CONSTEXPR any_instance_of<T> any_instance_of_v{};

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename CPO, typename T, typename... Vs>
struct can_static_tag_invoke {
  template <typename AnyT, typename... AnyVs>
  static ASIO_CONSTEXPR auto check_impl(int) -> decltype(CPO{}(AnyT{}, AnyVs{}...), 0) { 
    return CPO{}(any_instance_of_v<T>, any_instance_of_v<Vs>...), 0;
  }
  template <typename AnyT, typename... AnyVs>
  static int check_impl(...) {
    return 0;
  }
  static ASIO_CONSTEXPR int check() { 
    return can_static_tag_invoke::check_impl<any_instance_of<T>, any_instance_of<Vs>...>(0U); 
  }
  ASIO_STATIC_CONSTEXPR(bool, value = asio_tag_invokes_tag_invoke_fn::is_constexpr<can_static_tag_invoke::check>::value);
};

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename CPO, typename T, typename... Vs>
ASIO_CONSTEXPR bool can_static_tag_invoke_v = can_static_tag_invoke<CPO, T, Vs...>::value;

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename CPO, typename T, typename... Vs>
struct static_tag_invoke_result : tag_invoke_result<CPO, any_instance_of<T>, any_instance_of<Vs>...> {};

#if defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename CPO, typename T, typename... Vs>
using static_tag_invoke_result_t = typename static_tag_invoke_result<CPO, T, Vs...>::type;

#endif // defined(ASIO_HAS_ALIAS_TEMPLATES)

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

template<typename CPO, typename T, typename... Args>
inline ASIO_CONSTEXPR typename enable_if<can_static_tag_invoke_v<CPO, T, Args...>, static_tag_invoke_result<CPO, T, Args...>>::type::type static_tag_invoke_v = CPO{}(any_instance_of_v<T>, any_instance_of_v<Args>...);

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

} // namespace tag_invokes
} // namespace asio

#endif // defined(GENERATING_DOCUMENTATION)

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_TAG_INVOKE_HPP
