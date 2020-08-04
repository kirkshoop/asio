//
// traits/tag_invoke_free.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_TRAITS_TAG_INVOKE_FREE_HPP
#define ASIO_TRAITS_TAG_INVOKE_FREE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"
#include "asio/detail/variadic_templates.hpp"

#if defined(ASIO_HAS_DECLTYPE) \
  && defined(ASIO_HAS_NOEXCEPT) \
  && defined(ASIO_HAS_WORKING_EXPRESSION_SFINAE)
# define ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT 1
#endif // defined(ASIO_HAS_DECLTYPE)
       //   && defined(ASIO_HAS_NOEXCEPT)
       //   && defined(ASIO_HAS_WORKING_EXPRESSION_SFINAE)

#include "asio/detail/push_options.hpp"

namespace asio {
namespace traits {

template <typename CPO, typename T, typename Vs, typename = void>
struct tag_invoke_free_traitdefault;

template <typename CPO, typename T, typename Vs, typename = void>
struct tag_invoke_free;

} // namespace traits
namespace detail {

struct no_tag_invoke_free
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#if defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

template <typename CPO, typename T, typename Vs, typename = void>
struct tag_invoke_free_trait : no_tag_invoke_free
{
};

#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename CPO, typename T, typename... Vs>
struct tag_invoke_free_trait<CPO, T, void(Vs...),
  typename void_type<
    decltype(tag_invoke(CPO{}, declval<T>(), declval<Vs>()...))
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    tag_invoke(CPO{}, declval<T>(), declval<Vs>()...));

  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    tag_invoke(CPO{}, declval<T>(), declval<Vs>()...)));
};

#else // defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename CPO, typename T>
struct tag_invoke_free_trait<CPO, T, void(),
  typename void_type<
    decltype(tag_invoke(CPO{}, declval<T>()))
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(tag_invoke(CPO{}, declval<T>()));

  ASIO_STATIC_CONSTEXPR(bool,
    is_noexcept = noexcept(tag_invoke(CPO{}, declval<T>())));
};

#define ASIO_PRIVATE_TAG_INVOKE_FREE_TRAIT_DEF(n) \
  template <typename CPO, typename T, ASIO_VARIADIC_TPARAMS(n)> \
  struct tag_invoke_free_trait<CPO, T, void(ASIO_VARIADIC_TARGS(n)), \
    typename void_type< \
      decltype(tag_invoke(CPO{}, declval<T>(), ASIO_VARIADIC_DECLVAL(n))) \
    >::type> \
  { \
    ASIO_STATIC_CONSTEXPR(bool, is_valid = true); \
  \
    using result_type = decltype( \
      tag_invoke(CPO{}, declval<T>(), ASIO_VARIADIC_DECLVAL(n))); \
  \
    ASIO_STATIC_CONSTEXPR(bool, is_noexcept = noexcept( \
      tag_invoke(CPO{}, declval<T>(), ASIO_VARIADIC_DECLVAL(n)))); \
  }; \
  /**/
ASIO_VARIADIC_GENERATE(ASIO_PRIVATE_TAG_INVOKE_FREE_TRAIT_DEF)
#undef ASIO_PRIVATE_TAG_INVOKE_FREE_TRAIT_DEF

#endif // defined(ASIO_HAS_VARIADIC_TEMPLATES)

#else // defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

template <typename CPO, typename T, typename Vs, typename = void>
struct tag_invoke_free_trait;

#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename CPO, typename T, typename... Vs>
struct tag_invoke_free_trait<CPO, T, void(Vs...)> :
  conditional<
    is_same<T, typename remove_reference<T>::type>::value
      && conjunction<is_same<Vs, typename decay<Vs>::type>...>::value,
    typename conditional<
      is_same<T, typename add_const<T>::type>::value,
      no_tag_invoke_free,
      traits::tag_invoke_free<CPO, typename add_const<T>::type, void(Vs...)>
    >::type,
    traits::tag_invoke_free<
      CPO,
      typename remove_reference<T>::type,
      void(typename decay<Vs>::type...)>
  >::type
{
};

#else // defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename CPO, typename T>
struct tag_invoke_free_trait<CPO, T, void()> :
  conditional<
    is_same<T, typename remove_reference<T>::type>::value,
    typename conditional<
      is_same<T, typename add_const<T>::type>::value,
      no_tag_invoke_free,
      traits::tag_invoke_free<CPO, typename add_const<T>::type, void()>
    >::type,
    traits::tag_invoke_free<CPO, typename remove_reference<T>::type, void()>
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
  struct tag_invoke_free_trait<CPO, T, void(ASIO_VARIADIC_TARGS(n))> : \
    conditional< \
      is_same<T, typename remove_reference<T>::type>::value \
        ASIO_PRIVATE_TAG_INVOKE_MEMBER_IS_SAME(n), \
      typename conditional< \
        is_same<T, typename add_const<T>::type>::value, \
        no_tag_invoke_free, \
        traits::tag_invoke_free< \
          CPO, \
          typename add_const<T>::type, \
          void(ASIO_VARIADIC_TARGS(n))> \
      >::type, \
      traits::tag_invoke_free< \
        CPO, \
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

#endif // defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

} // namespace detail
namespace traits {

template <typename CPO, typename T, typename Vs, typename>
struct tag_invoke_free_traitdefault :
  detail::tag_invoke_free_trait<CPO, T, Vs>
{
};

template <typename CPO, typename T, typename Vs, typename>
struct tag_invoke_free :
  tag_invoke_free_traitdefault<CPO, T, Vs>
{
};

} // namespace traits
} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_TRAITS_TAG_INVOKE_FREE_HPP
