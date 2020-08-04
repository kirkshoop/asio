//
// traits/tag_invoke_member.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_TRAITS_TAG_INVOKE_MEMBER_HPP
#define ASIO_TRAITS_TAG_INVOKE_MEMBER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"
#include "asio/detail/variadic_templates.hpp"

#if defined(ASIO_HAS_DECLTYPE) \
  && defined(ASIO_HAS_NOEXCEPT) \
  && defined(ASIO_HAS_WORKING_EXPRESSION_SFINAE)
# define ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT 1
#endif // defined(ASIO_HAS_DECLTYPE)
       //   && defined(ASIO_HAS_NOEXCEPT)
       //   && defined(ASIO_HAS_WORKING_EXPRESSION_SFINAE)

#include "asio/detail/push_options.hpp"

namespace asio {
namespace traits {

template <typename CPO, typename T, typename Vs, typename = void>
struct tag_invoke_member_default;

template <typename CPO, typename T, typename Vs, typename = void>
struct tag_invoke_member;

} // namespace traits
namespace detail {

struct no_tag_invoke_member
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#if defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

template <typename CPO, typename T, typename Vs, typename = void>
struct tag_invoke_member_trait : no_tag_invoke_member
{
};

#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename CPO, typename T, typename... Vs>
struct tag_invoke_member_trait<CPO, T, void(Vs...),
  typename void_type<
    decltype(declval<T>().tag_invoke(declval<CPO>(), declval<Vs>()...))
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    declval<T>().tag_invoke(declval<CPO>(), declval<Vs>()...));

  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    declval<T>().tag_invoke(declval<CPO>(), declval<Vs>()...)));
};

#else // defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename CPO, typename T>
struct tag_invoke_member_trait<CPO, T, void(),
  typename void_type<
    decltype(declval<T>().tag_invoke(declval<CPO>()))
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(declval<T>().tag_invoke(declval<CPO>()));

  ASIO_STATIC_CONSTEXPR(bool,
    is_noexcept = noexcept(declval<T>().tag_invoke(declval<CPO>())));
};

#define ASIO_PRIVATE_TAG_INVOKE_MEMBER_TRAIT_DEF(n) \
  template <typename CPO, typename T, ASIO_VARIADIC_TPARAMS(n)> \
  struct tag_invoke_member_trait<CPO, T, void(ASIO_VARIADIC_TARGS(n)), \
    typename void_type< \
      decltype(declval<T>().tag_invoke(declval<CPO>(), ASIO_VARIADIC_DECLVAL(n))) \
    >::type> \
  { \
    ASIO_STATIC_CONSTEXPR(bool, is_valid = true); \
  \
    using result_type = decltype( \
      declval<T>().tag_invoke(declval<CPO>(), ASIO_VARIADIC_DECLVAL(n))); \
  \
    ASIO_STATIC_CONSTEXPR(bool, is_noexcept = noexcept( \
      declval<T>().tag_invoke(declval<CPO>(), ASIO_VARIADIC_DECLVAL(n)))); \
  }; \
  /**/
ASIO_VARIADIC_GENERATE(ASIO_PRIVATE_TAG_INVOKE_MEMBER_TRAIT_DEF)
#undef ASIO_PRIVATE_TAG_INVOKE_MEMBER_TRAIT_DEF

#endif // defined(ASIO_HAS_VARIADIC_TEMPLATES)

#else // defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

template <typename CPO, typename T, typename Vs, typename = void>
struct tag_invoke_member_trait;

#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename CPO, typename T, typename... Vs>
struct tag_invoke_member_trait<CPO, T, void(Vs...)> :
  conditional<
    is_same<T, typename remove_reference<T>::type>::value
      && conjunction<is_same<Vs, typename decay<Vs>::type>...>::value,
    typename conditional<
      is_same<T, typename add_const<T>::type>::value,
      no_tag_invoke_member,
      traits::tag_invoke_member<CPO, typename add_const<T>::type, void(Vs...)>
    >::type,
    traits::tag_invoke_member<
      CPO, 
      typename remove_reference<T>::type,
      void(typename decay<Vs>::type...)>
  >::type
{
};

#else // defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename CPO, typename T>
struct tag_invoke_member_trait<CPO, T, void()> :
  conditional<
    is_same<T, typename remove_reference<T>::type>::value,
    typename conditional<
      is_same<T, typename add_const<T>::type>::value,
      no_tag_invoke_member,
      traits::tag_invoke_member<CPO, typename add_const<T>::type, void()>
    >::type,
    traits::tag_invoke_member<CPO, typename remove_reference<T>::type, void()>
  >::type
{
};

#define ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME(n) \
  ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_##n

#define ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_1 \
  && is_same<T1, typename decay<T1>::type>::value
#define ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_2 \
  ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_1 \
  && is_same<T2, typename decay<T2>::type>::value
#define ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_3 \
  ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_2 \
  && is_same<T3, typename decay<T3>::type>::value
#define ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_4 \
  ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_3 \
  && is_same<T4, typename decay<T4>::type>::value
#define ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_5 \
  ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_4 \
  && is_same<T5, typename decay<T5>::type>::value
#define ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_6 \
  ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_5 \
  && is_same<T6, typename decay<T6>::type>::value
#define ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_7 \
  ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_6 \
  && is_same<T7, typename decay<T7>::type>::value
#define ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_8 \
  ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_7 \
  && is_same<T8, typename decay<T8>::type>::value

#define ASIO_PRIVATE_TAG_INVOKE_MEMBER_TRAIT_DEF(n) \
  template <typename CPO, typename T, ASIO_VARIADIC_TPARAMS(n)> \
  struct tag_invoke_member_trait<CPO, T, void(ASIO_VARIADIC_TARGS(n))> : \
    conditional< \
      is_same<T, typename remove_reference<T>::type>::value \
        ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME(n), \
      typename conditional< \
        is_same<T, typename add_const<T>::type>::value, \
        no_tag_invoke_member, \
        traits::tag_invoke_member< \
          CPO, \
          typename add_const<T>::type, \
          void(ASIO_VARIADIC_TARGS(n))> \
      >::type, \
      traits::tag_invoke_member< \
        CPO,  \
        typename remove_reference<T>::type, \
        void(ASIO_VARIADIC_DECAY(n))> \
    >::type \
  { \
  }; \
  /**/
ASIO_VARIADIC_GENERATE(ASIO_PRIVATE_TAG_INVOKE_MEMBER_TRAIT_DEF)
#undef ASIO_PRIVATE_TAG_INVOKE_MEMBER_TRAIT_DEF
#undef ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME
#undef ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_1
#undef ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_2
#undef ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_3
#undef ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_4
#undef ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_5
#undef ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_6
#undef ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_7
#undef ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME_8

#endif // defined(ASIO_HAS_VARIADIC_TEMPLATES)

#endif // defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

} // namespace detail
namespace traits {

template <typename CPO, typename T, typename Vs, typename>
struct tag_invoke_member_default :
  detail::tag_invoke_member_trait<CPO, T, Vs>
{
};

template <typename CPO, typename T, typename Vs, typename>
struct tag_invoke_member :
  tag_invoke_member_default<CPO, T, Vs>
{
};

} // namespace traits
} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_TRAITS_TAG_INVOKE_MEMBER_HPP
