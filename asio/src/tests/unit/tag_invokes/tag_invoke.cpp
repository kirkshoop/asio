//
// tag_invoke.cpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Disable autolinking for unit tests.
#if !defined(BOOST_ALL_NO_LIB)
#define BOOST_ALL_NO_LIB 1
#endif // !defined(BOOST_ALL_NO_LIB)

// Test that header file is self-contained.
#include "asio/tag_invokes/tag_invoke.hpp"

#include <string>
#include "../unit_test.hpp"

namespace tinvk = asio::tag_invokes;

static int call_count = 0;

namespace test_cpo_fn_impl {
struct impl {
  template <typename T>
  ASIO_CONSTEXPR typename asio::enable_if<
    tinvk::can_tag_invoke<impl, T>::value,
    typename tinvk::tag_invoke_result<impl, T>::type
  >::type
  operator()(T&& t) const
    ASIO_NOEXCEPT_IF((
      tinvk::is_nothrow_tag_invoke<impl, T>::value))
  {
    return tinvk::tag_invoke(*this, ASIO_MOVE_CAST(T)(t));
  }

  template <typename T, typename V1>
  ASIO_CONSTEXPR typename asio::enable_if<
    tinvk::can_tag_invoke<impl, T, V1>::value,
    typename tinvk::tag_invoke_result<impl, T, V1>::type
  >::type
  operator()(T&& t, V1&& v1) const
    ASIO_NOEXCEPT_IF((
      tinvk::is_nothrow_tag_invoke<impl, T, V1>::value))
  {
    return tinvk::tag_invoke(*this, ASIO_MOVE_CAST(T)(t), ASIO_MOVE_CAST(V1)(v1));
  }

  template <typename T, typename V1, typename V2>
  ASIO_CONSTEXPR typename asio::enable_if<
    tinvk::can_tag_invoke<impl, T, V1, V2>::value,
    typename tinvk::tag_invoke_result<impl, T, V1, V2>::type
  >::type
  operator()(T&& t, V1&& v1, V2&& v2) const
    ASIO_NOEXCEPT_IF((
      tinvk::is_nothrow_tag_invoke<impl, T, V1, V2>::value))
  {
    return tinvk::tag_invoke(*this, ASIO_MOVE_CAST(T)(t), ASIO_MOVE_CAST(V1)(v1), ASIO_MOVE_CAST(V2)(v2));
  }
};

template <typename T = impl>
struct static_instance
{
  static const T instance;
};

template <typename T>
const T static_instance<T>::instance = {};
}

static ASIO_CONSTEXPR const test_cpo_fn_impl::impl&
  test_cpo = test_cpo_fn_impl::static_instance<>::instance;

struct no_tag_invoke
{
};

struct const_member_tag_invoke_0
{
  int tag_invoke(test_cpo_fn_impl::impl) const
  {
    ++call_count;
    return call_count;
  }
};

#if !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

namespace asio {
namespace traits {

template <>
struct tag_invoke_member<test_cpo_fn_impl::impl, const const_member_tag_invoke_0, void()>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef int result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

struct const_member_tag_invoke_1
{
  template <typename V1>
  int tag_invoke(test_cpo_fn_impl::impl, ASIO_MOVE_ARG(V1) v1) const
  {
    typename asio::decay<V1>::type tmp(ASIO_MOVE_CAST(V1)(v1));
    (void)tmp;
    ++call_count;
    return call_count;
  }
};

#if !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

namespace asio {
namespace traits {

template <typename V1>
struct tag_invoke_member<test_cpo_fn_impl::impl, const const_member_tag_invoke_1, void(V1)>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef int result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

struct const_member_tag_invoke_2
{
  template <typename V1, typename V2>
  int tag_invoke(test_cpo_fn_impl::impl, ASIO_MOVE_ARG(V1) v1, ASIO_MOVE_ARG(V2) v2) const
  {
    typename asio::decay<V1>::type tmp1(ASIO_MOVE_CAST(V1)(v1));
    (void)tmp1;
    typename asio::decay<V2>::type tmp2(ASIO_MOVE_CAST(V2)(v2));
    (void)tmp2;
    ++call_count;
    return call_count;
  }
};

#if !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

namespace asio {
namespace traits {

template <typename V1, typename V2>
struct tag_invoke_member<test_cpo_fn_impl::impl, const const_member_tag_invoke_2, void(V1, V2)>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef int result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

struct free_tag_invoke_const_target_0
{
  friend int tag_invoke(test_cpo_fn_impl::impl, const free_tag_invoke_const_target_0&)
  {
    ++call_count;
    return call_count;
  }
};

#if !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

namespace asio {
namespace traits {

template <>
struct tag_invoke_free<test_cpo_fn_impl::impl, const free_tag_invoke_const_target_0, void()>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef int result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

struct free_tag_invoke_const_target_1
{
  template <typename V1>
  friend int tag_invoke(test_cpo_fn_impl::impl, const free_tag_invoke_const_target_1&,
      ASIO_MOVE_ARG(V1) v1)
  {
    typename asio::decay<V1>::type tmp(ASIO_MOVE_CAST(V1)(v1));
    (void)tmp;
    ++call_count;
    return call_count;
  }
};

#if !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

namespace asio {
namespace traits {

template <typename V1>
struct tag_invoke_free<test_cpo_fn_impl::impl, const free_tag_invoke_const_target_1, void(V1)>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef int result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

struct free_tag_invoke_const_target_2
{
  template <typename V1, typename V2>
  friend int tag_invoke(test_cpo_fn_impl::impl, const free_tag_invoke_const_target_2&,
      ASIO_MOVE_ARG(V1) v1, ASIO_MOVE_ARG(V2) v2)
  {
    typename asio::decay<V1>::type tmp1(ASIO_MOVE_CAST(V1)(v1));
    (void)tmp1;
    typename asio::decay<V2>::type tmp2(ASIO_MOVE_CAST(V2)(v2));
    (void)tmp2;
    ++call_count;
    return call_count;
  }
};

#if !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

namespace asio {
namespace traits {

template <typename V1, typename V2>
struct tag_invoke_free<test_cpo_fn_impl::impl, const free_tag_invoke_const_target_2, void(V1, V2)>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef int result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

struct non_const_member_tag_invoke_0
{
  int tag_invoke(test_cpo_fn_impl::impl)
  {
    ++call_count;
    return call_count;
  }
};

#if !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

namespace asio {
namespace traits {

template <>
struct tag_invoke_member<test_cpo_fn_impl::impl, non_const_member_tag_invoke_0, void()>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef int result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

struct non_const_member_tag_invoke_1
{
  template <typename V1>
  int tag_invoke(test_cpo_fn_impl::impl, ASIO_MOVE_ARG(V1) v1)
  {
    typename asio::decay<V1>::type tmp(ASIO_MOVE_CAST(V1)(v1));
    (void)tmp;
    ++call_count;
    return call_count;
  }
};

#if !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

namespace asio {
namespace traits {

template <typename V1>
struct tag_invoke_member<test_cpo_fn_impl::impl, non_const_member_tag_invoke_1, void(V1)>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef int result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

struct non_const_member_tag_invoke_2
{
  template <typename V1, typename V2>
  int tag_invoke(test_cpo_fn_impl::impl, ASIO_MOVE_ARG(V1) v1, ASIO_MOVE_ARG(V2) v2)
  {
    typename asio::decay<V1>::type tmp1(ASIO_MOVE_CAST(V1)(v1));
    (void)tmp1;
    typename asio::decay<V2>::type tmp2(ASIO_MOVE_CAST(V2)(v2));
    (void)tmp2;
    ++call_count;
    return call_count;
  }
};

#if !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

namespace asio {
namespace traits {

template <typename V1, typename V2>
struct tag_invoke_member<test_cpo_fn_impl::impl, non_const_member_tag_invoke_2, void(V1, V2)>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef int result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_MEMBER_TRAIT)

struct free_tag_invoke_non_const_target_0
{
  friend int tag_invoke(test_cpo_fn_impl::impl, free_tag_invoke_non_const_target_0&)
  {
    ++call_count;
    return call_count;
  }
};

#if !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

namespace asio {
namespace traits {

template <>
struct tag_invoke_free<test_cpo_fn_impl::impl, free_tag_invoke_non_const_target_0, void()>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef int result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

struct free_tag_invoke_non_const_target_1
{
  template <typename V1>
  friend int tag_invoke(test_cpo_fn_impl::impl, free_tag_invoke_non_const_target_1&,
      ASIO_MOVE_ARG(V1) v1)
  {
    typename asio::decay<V1>::type tmp(ASIO_MOVE_CAST(V1)(v1));
    (void)tmp;
    ++call_count;
    return call_count;
  }
};

#if !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

namespace asio {
namespace traits {

template <typename V1>
struct tag_invoke_free<test_cpo_fn_impl::impl, free_tag_invoke_non_const_target_1, void(V1)>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef int result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

struct free_tag_invoke_non_const_target_2
{
  template <typename V1, typename V2>
  friend int tag_invoke(test_cpo_fn_impl::impl, free_tag_invoke_non_const_target_2&,
      ASIO_MOVE_ARG(V1) v1, ASIO_MOVE_ARG(V2) v2)
  {
    typename asio::decay<V1>::type tmp1(ASIO_MOVE_CAST(V1)(v1));
    (void)tmp1;
    typename asio::decay<V2>::type tmp2(ASIO_MOVE_CAST(V2)(v2));
    (void)tmp2;
    ++call_count;
    return call_count;
  }
};

#if !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

namespace asio {
namespace traits {

template <typename V1, typename V2>
struct tag_invoke_free<test_cpo_fn_impl::impl, free_tag_invoke_non_const_target_2, void(V1, V2)>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef int result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DEDUCED_TAG_INVOKE_FREE_TRAIT)

void test_can_tag_invoke()
{
  ASIO_CONSTEXPR bool b1 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      no_tag_invoke&>::value;
  ASIO_CHECK(b1 == false);

  ASIO_CONSTEXPR bool b2 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const no_tag_invoke&>::value;
  ASIO_CHECK(b2 == false);

  ASIO_CONSTEXPR bool b3 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      no_tag_invoke&, int>::value;
  ASIO_CHECK(b3 == false);

  ASIO_CONSTEXPR bool b4 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const no_tag_invoke&, int>::value;
  ASIO_CHECK(b4 == false);

  ASIO_CONSTEXPR bool b5 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      no_tag_invoke&, int, std::string>::value;
  ASIO_CHECK(b5 == false);

  ASIO_CONSTEXPR bool b6 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const no_tag_invoke&, int, std::string>::value;
  ASIO_CHECK(b6 == false);

  ASIO_CONSTEXPR bool b7 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const_member_tag_invoke_0&>::value;
  ASIO_CHECK(b7 == true);

  ASIO_CONSTEXPR bool b8 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const const_member_tag_invoke_0&>::value;
  ASIO_CHECK(b8 == true);

  ASIO_CONSTEXPR bool b9 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const_member_tag_invoke_0&, int>::value;
  ASIO_CHECK(b9 == false);

  ASIO_CONSTEXPR bool b10 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const const_member_tag_invoke_0&, int>::value;
  ASIO_CHECK(b10 == false);

  ASIO_CONSTEXPR bool b11 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const_member_tag_invoke_0&, int, std::string>::value;
  ASIO_CHECK(b11 == false);

  ASIO_CONSTEXPR bool b12 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const const_member_tag_invoke_0&, int, std::string>::value;
  ASIO_CHECK(b12 == false);

  ASIO_CONSTEXPR bool b13 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const_member_tag_invoke_1&>::value;
  ASIO_CHECK(b13 == false);

  ASIO_CONSTEXPR bool b14 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const const_member_tag_invoke_1&>::value;
  ASIO_CHECK(b14 == false);

  ASIO_CONSTEXPR bool b15 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const_member_tag_invoke_1&, int>::value;
  ASIO_CHECK(b15 == true);

  ASIO_CONSTEXPR bool b16 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const const_member_tag_invoke_1&, int>::value;
  ASIO_CHECK(b16 == true);

  ASIO_CONSTEXPR bool b17 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const_member_tag_invoke_1&, int, std::string>::value;
  ASIO_CHECK(b17 == false);

  ASIO_CONSTEXPR bool b18 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const const_member_tag_invoke_1&, int, std::string>::value;
  ASIO_CHECK(b18 == false);

  ASIO_CONSTEXPR bool b19 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const_member_tag_invoke_2&>::value;
  ASIO_CHECK(b19 == false);

  ASIO_CONSTEXPR bool b20 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const const_member_tag_invoke_2&>::value;
  ASIO_CHECK(b20 == false);

  ASIO_CONSTEXPR bool b21 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const_member_tag_invoke_2&, int>::value;
  ASIO_CHECK(b21 == false);

  ASIO_CONSTEXPR bool b22 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const const_member_tag_invoke_2&, int>::value;
  ASIO_CHECK(b22 == false);

  ASIO_CONSTEXPR bool b23 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const_member_tag_invoke_2&, int, std::string>::value;
  ASIO_CHECK(b23 == true);

  ASIO_CONSTEXPR bool b24 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const const_member_tag_invoke_2&, int, std::string>::value;
  ASIO_CHECK(b24 == true);

  ASIO_CONSTEXPR bool b25 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_const_target_0&>::value;
  ASIO_CHECK(b25 == true);

  ASIO_CONSTEXPR bool b26 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_const_target_0&>::value;
  ASIO_CHECK(b26 == true);

  ASIO_CONSTEXPR bool b27 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_const_target_0&, int>::value;
  ASIO_CHECK(b27 == false);

  ASIO_CONSTEXPR bool b28 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_const_target_0&, int>::value;
  ASIO_CHECK(b28 == false);

  ASIO_CONSTEXPR bool b29 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_const_target_0&, int, std::string>::value;
  ASIO_CHECK(b29 == false);

  ASIO_CONSTEXPR bool b30 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_const_target_0&, int, std::string>::value;
  ASIO_CHECK(b30 == false);

  ASIO_CONSTEXPR bool b31 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_const_target_1&>::value;
  ASIO_CHECK(b31 == false);

  ASIO_CONSTEXPR bool b32 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_const_target_1&>::value;
  ASIO_CHECK(b32 == false);

  ASIO_CONSTEXPR bool b33 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_const_target_1&, int>::value;
  ASIO_CHECK(b33 == true);

  ASIO_CONSTEXPR bool b34 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_const_target_1&, int>::value;
  ASIO_CHECK(b34 == true);

  ASIO_CONSTEXPR bool b35 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_const_target_1&, int, std::string>::value;
  ASIO_CHECK(b35 == false);

  ASIO_CONSTEXPR bool b36 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_const_target_1&, int, std::string>::value;
  ASIO_CHECK(b36 == false);

  ASIO_CONSTEXPR bool b37 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_const_target_2&>::value;
  ASIO_CHECK(b37 == false);

  ASIO_CONSTEXPR bool b38 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_const_target_2&>::value;
  ASIO_CHECK(b38 == false);

  ASIO_CONSTEXPR bool b39 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_const_target_2&, int>::value;
  ASIO_CHECK(b39 == false);

  ASIO_CONSTEXPR bool b40 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_const_target_2&, int>::value;
  ASIO_CHECK(b40 == false);

  ASIO_CONSTEXPR bool b41 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_const_target_2&, int, std::string>::value;
  ASIO_CHECK(b41 == true);

  ASIO_CONSTEXPR bool b42 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_const_target_2&, int, std::string>::value;
  ASIO_CHECK(b42 == true);

  ASIO_CONSTEXPR bool b43 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      non_const_member_tag_invoke_0&>::value;
  ASIO_CHECK(b43 == true);

  ASIO_CONSTEXPR bool b44 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const non_const_member_tag_invoke_0&>::value;
  ASIO_CHECK(b44 == false);

  ASIO_CONSTEXPR bool b45 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      non_const_member_tag_invoke_0&, int>::value;
  ASIO_CHECK(b45 == false);

  ASIO_CONSTEXPR bool b46 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const non_const_member_tag_invoke_0&, int>::value;
  ASIO_CHECK(b46 == false);

  ASIO_CONSTEXPR bool b47 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      non_const_member_tag_invoke_0&, int, std::string>::value;
  ASIO_CHECK(b47 == false);

  ASIO_CONSTEXPR bool b48 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const non_const_member_tag_invoke_0&, int, std::string>::value;
  ASIO_CHECK(b48 == false);

  ASIO_CONSTEXPR bool b49 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      non_const_member_tag_invoke_1&>::value;
  ASIO_CHECK(b49 == false);

  ASIO_CONSTEXPR bool b50 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const non_const_member_tag_invoke_1&>::value;
  ASIO_CHECK(b50 == false);

  ASIO_CONSTEXPR bool b51 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      non_const_member_tag_invoke_1&, int>::value;
  ASIO_CHECK(b51 == true);

  ASIO_CONSTEXPR bool b52 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const non_const_member_tag_invoke_1&, int>::value;
  ASIO_CHECK(b52 == false);

  ASIO_CONSTEXPR bool b53 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      non_const_member_tag_invoke_1&, int, std::string>::value;
  ASIO_CHECK(b53 == false);

  ASIO_CONSTEXPR bool b54 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const non_const_member_tag_invoke_1&, int, std::string>::value;
  ASIO_CHECK(b54 == false);

  ASIO_CONSTEXPR bool b55 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      non_const_member_tag_invoke_2&>::value;
  ASIO_CHECK(b55 == false);

  ASIO_CONSTEXPR bool b56 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const non_const_member_tag_invoke_2&>::value;
  ASIO_CHECK(b56 == false);

  ASIO_CONSTEXPR bool b57 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      non_const_member_tag_invoke_2&, int>::value;
  ASIO_CHECK(b57 == false);

  ASIO_CONSTEXPR bool b58 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const non_const_member_tag_invoke_2&, int>::value;
  ASIO_CHECK(b58 == false);

  ASIO_CONSTEXPR bool b59 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      non_const_member_tag_invoke_2&, int, std::string>::value;
  ASIO_CHECK(b59 == true);

  ASIO_CONSTEXPR bool b60 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const non_const_member_tag_invoke_2&, int, std::string>::value;
  ASIO_CHECK(b60 == false);

  ASIO_CONSTEXPR bool b61 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_non_const_target_0&>::value;
  ASIO_CHECK(b61 == true);

  ASIO_CONSTEXPR bool b62 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_non_const_target_0&>::value;
  ASIO_CHECK(b62 == false);

  ASIO_CONSTEXPR bool b63 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_non_const_target_0&, int>::value;
  ASIO_CHECK(b63 == false);

  ASIO_CONSTEXPR bool b64 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_non_const_target_0&, int>::value;
  ASIO_CHECK(b64 == false);

  ASIO_CONSTEXPR bool b65 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_non_const_target_0&, int, std::string>::value;
  ASIO_CHECK(b65 == false);

  ASIO_CONSTEXPR bool b66 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_non_const_target_0&, int, std::string>::value;
  ASIO_CHECK(b66 == false);

  ASIO_CONSTEXPR bool b67 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_non_const_target_1&>::value;
  ASIO_CHECK(b67 == false);

  ASIO_CONSTEXPR bool b68 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_non_const_target_1&>::value;
  ASIO_CHECK(b68 == false);

  ASIO_CONSTEXPR bool b69 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_non_const_target_1&, int>::value;
  ASIO_CHECK(b69 == true);

  ASIO_CONSTEXPR bool b70 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_non_const_target_1&, int>::value;
  ASIO_CHECK(b70 == false);

  ASIO_CONSTEXPR bool b71 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_non_const_target_1&, int, std::string>::value;
  ASIO_CHECK(b71 == false);

  ASIO_CONSTEXPR bool b72 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_non_const_target_1&, int, std::string>::value;
  ASIO_CHECK(b72 == false);

  ASIO_CONSTEXPR bool b73 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_non_const_target_2&>::value;
  ASIO_CHECK(b73 == false);

  ASIO_CONSTEXPR bool b74 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_non_const_target_2&>::value;
  ASIO_CHECK(b74 == false);

  ASIO_CONSTEXPR bool b75 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_non_const_target_2&, int>::value;
  ASIO_CHECK(b75 == false);

  ASIO_CONSTEXPR bool b76 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_non_const_target_2&, int>::value;
  ASIO_CHECK(b76 == false);

  ASIO_CONSTEXPR bool b77 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      free_tag_invoke_non_const_target_2&, int, std::string>::value;
  ASIO_CHECK(b77 == true);

  ASIO_CONSTEXPR bool b78 = tinvk::can_tag_invoke<test_cpo_fn_impl::impl, 
      const free_tag_invoke_non_const_target_2&, int, std::string>::value;
  ASIO_CHECK(b78 == false);
}

void increment(int* count)
{
  ++(*count);
}

void test_tag_invoke()
{

  call_count = 0;
  const_member_tag_invoke_0 ex1 = {};
  tinvk::tag_invoke(test_cpo, ex1);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      const_member_tag_invoke_0 &>::type c0 = test_cpo(ex1);
  ASIO_CHECK(call_count == 1);
  (void)c0;

  call_count = 0;
  const const_member_tag_invoke_0 ex2 = {};
  tinvk::tag_invoke(test_cpo, ex2);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      const const_member_tag_invoke_0&>::type c1 = test_cpo(ex1);
  ASIO_CHECK(call_count == 1);
  (void)c1;

  call_count = 0;
  tinvk::tag_invoke(test_cpo, const_member_tag_invoke_0());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      const_member_tag_invoke_0>::type c2 = test_cpo(const_member_tag_invoke_0());
  ASIO_CHECK(call_count == 1);
  (void)c2;

  call_count = 0;
  const_member_tag_invoke_1 ex3 = {};
  tinvk::tag_invoke(test_cpo, ex3, 123);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      const_member_tag_invoke_1 &, int>::type c3 = test_cpo(ex3, 123);
  ASIO_CHECK(call_count == 1);
  (void)c3;

  call_count = 0;
  const const_member_tag_invoke_1 ex4 = {};
  tinvk::tag_invoke(test_cpo, ex4, 123);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      const const_member_tag_invoke_1 &, int>::type c4 = test_cpo(ex4, 123);
  ASIO_CHECK(call_count == 1);
  (void)c4;

  call_count = 0;
  tinvk::tag_invoke(test_cpo, const_member_tag_invoke_1(), 123);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      const_member_tag_invoke_1, int>::type c5 = test_cpo(const_member_tag_invoke_1(), 123);
  ASIO_CHECK(call_count == 1);
  (void)c5;

  call_count = 0;
  const_member_tag_invoke_2 ex5 = {};
  tinvk::tag_invoke(test_cpo, ex5, 123, std::string());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      const_member_tag_invoke_2 &, int, 
      std::string>::type c6 = test_cpo(ex5, 123, std::string());
  ASIO_CHECK(call_count == 1);
  (void)c6;

  call_count = 0;
  const const_member_tag_invoke_2 ex6 = {};
  tinvk::tag_invoke(test_cpo, ex6, 123, std::string());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      const const_member_tag_invoke_2 &, int, 
      std::string>::type c7 = test_cpo(ex6, 123, std::string());
  ASIO_CHECK(call_count == 1);
  (void)c7;

  call_count = 0;
  tinvk::tag_invoke(test_cpo, const_member_tag_invoke_2(), 123, std::string());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      const_member_tag_invoke_2, int, 
      std::string>::type c8 = test_cpo(const_member_tag_invoke_2(), 123, std::string());
  ASIO_CHECK(call_count == 1);
  (void)c8;

  call_count = 0;
  free_tag_invoke_const_target_0 ex7 = {};
  tinvk::tag_invoke(test_cpo, ex7);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      free_tag_invoke_const_target_0 &>::type c9 = test_cpo(ex7);
  ASIO_CHECK(call_count == 1);
  (void)c9;

  call_count = 0;
  const free_tag_invoke_const_target_0 ex8 = {};
  tinvk::tag_invoke(test_cpo, ex8);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      const free_tag_invoke_const_target_0 &>::type c10 = test_cpo(ex8);
  ASIO_CHECK(call_count == 1);
  (void)c10;

  call_count = 0;
  tinvk::tag_invoke(test_cpo, free_tag_invoke_const_target_0());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      free_tag_invoke_const_target_0>::type c11 = test_cpo(free_tag_invoke_const_target_0());
  ASIO_CHECK(call_count == 1);
  (void)c11;

  call_count = 0;
  free_tag_invoke_const_target_1 ex9 = {};
  tinvk::tag_invoke(test_cpo, ex9, 123);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      free_tag_invoke_const_target_1 &, int>::type c12 = test_cpo(ex9, 123);
  ASIO_CHECK(call_count == 1);
  (void)c12;

  call_count = 0;
  const free_tag_invoke_const_target_1 ex10 = {};
  tinvk::tag_invoke(test_cpo, ex10, 123);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      const free_tag_invoke_const_target_1 &, int>::type c13 = test_cpo(ex10, 123);
  ASIO_CHECK(call_count == 1);
  (void)c13;

  call_count = 0;
  tinvk::tag_invoke(test_cpo, free_tag_invoke_const_target_1(), 123);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      free_tag_invoke_const_target_1, int>::type c14 = test_cpo(free_tag_invoke_const_target_1(), 123);
  ASIO_CHECK(call_count == 1);
  (void)c14;

  call_count = 0;
  free_tag_invoke_const_target_2 ex11 = {};
  tinvk::tag_invoke(test_cpo, ex11, 123, std::string());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      free_tag_invoke_const_target_2 &, int, 
      std::string>::type c15 = test_cpo(ex11, 123, std::string());
  ASIO_CHECK(call_count == 1);
  (void)c15;

  call_count = 0;
  const free_tag_invoke_const_target_2 ex12 = {};
  tinvk::tag_invoke(test_cpo, ex12, 123, std::string());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      free_tag_invoke_const_target_2 &, int, 
      std::string>::type c16 = test_cpo(ex12, 123, std::string());
  ASIO_CHECK(call_count == 1);
  (void)c16;

  call_count = 0;
  tinvk::tag_invoke(test_cpo, free_tag_invoke_const_target_2(), 123, std::string());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      free_tag_invoke_const_target_2, int, 
      std::string>::type c17 = test_cpo(free_tag_invoke_const_target_2(), 123, std::string());
  ASIO_CHECK(call_count == 1);
  (void)c17;

  call_count = 0;
  non_const_member_tag_invoke_0 ex13 = {};
  tinvk::tag_invoke(test_cpo, ex13);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      non_const_member_tag_invoke_0 &>::type c18 = test_cpo(ex13);
  ASIO_CHECK(call_count == 1);
  (void)c18;

  call_count = 0;
  non_const_member_tag_invoke_1 ex14 = {};
  tinvk::tag_invoke(test_cpo, ex14, 123);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      non_const_member_tag_invoke_1 &, int>::type c19 = test_cpo(ex14, 123);
  ASIO_CHECK(call_count == 1);
  (void)c19;

  call_count = 0;
  non_const_member_tag_invoke_2 ex15 = {};
  tinvk::tag_invoke(test_cpo, ex15, 123, std::string());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      non_const_member_tag_invoke_2 &, int, 
      std::string>::type c20 = test_cpo(ex15, 123, std::string());
  ASIO_CHECK(call_count == 1);
  (void)c20;

  call_count = 0;
  free_tag_invoke_non_const_target_0 ex16 = {};
  tinvk::tag_invoke(test_cpo, ex16);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      free_tag_invoke_non_const_target_0 &>::type c21 = test_cpo(ex16);
  ASIO_CHECK(call_count == 1);
  (void)c21;

  call_count = 0;
  free_tag_invoke_non_const_target_1 ex17 = {};
  tinvk::tag_invoke(test_cpo, ex17, 123);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      free_tag_invoke_non_const_target_1 &, int>::type c22 = test_cpo(ex17, 123);
  ASIO_CHECK(call_count == 1);
  (void)c22;

  call_count = 0;
  free_tag_invoke_non_const_target_2 ex18 = {};
  tinvk::tag_invoke(test_cpo, ex18, 123, std::string());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  tinvk::tag_invoke_result<test_cpo_fn_impl::impl,
      free_tag_invoke_non_const_target_2 &, int, 
      std::string>::type c23 = test_cpo(ex18, 123, std::string());
  ASIO_CHECK(call_count == 1);
  (void)c23;
}

ASIO_TEST_SUITE
(
  "tag_invoke",
  ASIO_TEST_CASE(test_can_tag_invoke)
  ASIO_TEST_CASE(test_tag_invoke)
)
