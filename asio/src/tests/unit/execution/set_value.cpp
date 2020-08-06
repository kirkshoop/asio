//
// set_value.cpp
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
#include "asio/execution/set_value.hpp"

#include <string>
#include "../unit_test.hpp"

namespace exec = asio::execution;

static int call_count = 0;

struct no_set_value
{
};

struct free_set_value_const_receiver_0
{
  friend void tag_invoke(decltype(exec::set_value), const free_set_value_const_receiver_0&)
  {
    ++call_count;
  }
};

struct free_set_value_const_receiver_1
{
  template <typename V1>
  friend void tag_invoke(decltype(exec::set_value), const free_set_value_const_receiver_1&,
      ASIO_MOVE_ARG(V1) v1)
  {
    typename asio::decay<V1>::type tmp(ASIO_MOVE_CAST(V1)(v1));
    (void)tmp;
    ++call_count;
  }
};

struct free_set_value_const_receiver_2
{
  template <typename V1, typename V2>
  friend void tag_invoke(decltype(exec::set_value), const free_set_value_const_receiver_2&,
      ASIO_MOVE_ARG(V1) v1, ASIO_MOVE_ARG(V2) v2)
  {
    typename asio::decay<V1>::type tmp1(ASIO_MOVE_CAST(V1)(v1));
    (void)tmp1;
    typename asio::decay<V2>::type tmp2(ASIO_MOVE_CAST(V2)(v2));
    (void)tmp2;
    ++call_count;
  }
};

struct free_set_value_non_const_receiver_0
{
  friend void tag_invoke(decltype(exec::set_value), free_set_value_non_const_receiver_0&)
  {
    ++call_count;
  }
};

struct free_set_value_non_const_receiver_1
{
  template <typename V1>
  friend void tag_invoke(decltype(exec::set_value), free_set_value_non_const_receiver_1&,
      ASIO_MOVE_ARG(V1) v1)
  {
    typename asio::decay<V1>::type tmp(ASIO_MOVE_CAST(V1)(v1));
    (void)tmp;
    ++call_count;
  }
};

struct free_set_value_non_const_receiver_2
{
  template <typename V1, typename V2>
  friend void tag_invoke(decltype(exec::set_value), free_set_value_non_const_receiver_2&,
      ASIO_MOVE_ARG(V1) v1, ASIO_MOVE_ARG(V2) v2)
  {
    typename asio::decay<V1>::type tmp1(ASIO_MOVE_CAST(V1)(v1));
    (void)tmp1;
    typename asio::decay<V2>::type tmp2(ASIO_MOVE_CAST(V2)(v2));
    (void)tmp2;
    ++call_count;
  }
};

void test_can_set_value()
{
  ASIO_CONSTEXPR bool b1 = exec::can_set_value<
      no_set_value&>::value;
  ASIO_CHECK(b1 == false);

  ASIO_CONSTEXPR bool b2 = exec::can_set_value<
      const no_set_value&>::value;
  ASIO_CHECK(b2 == false);

  ASIO_CONSTEXPR bool b3 = exec::can_set_value<
      no_set_value&, int>::value;
  ASIO_CHECK(b3 == false);

  ASIO_CONSTEXPR bool b4 = exec::can_set_value<
      const no_set_value&, int>::value;
  ASIO_CHECK(b4 == false);

  ASIO_CONSTEXPR bool b5 = exec::can_set_value<
      no_set_value&, int, std::string>::value;
  ASIO_CHECK(b5 == false);

  ASIO_CONSTEXPR bool b6 = exec::can_set_value<
      const no_set_value&, int, std::string>::value;
  ASIO_CHECK(b6 == false);

  ASIO_CONSTEXPR bool b25 = exec::can_set_value<
      free_set_value_const_receiver_0&>::value;
  ASIO_CHECK(b25 == true);

  ASIO_CONSTEXPR bool b26 = exec::can_set_value<
      const free_set_value_const_receiver_0&>::value;
  ASIO_CHECK(b26 == true);

  ASIO_CONSTEXPR bool b27 = exec::can_set_value<
      free_set_value_const_receiver_0&, int>::value;
  ASIO_CHECK(b27 == false);

  ASIO_CONSTEXPR bool b28 = exec::can_set_value<
      const free_set_value_const_receiver_0&, int>::value;
  ASIO_CHECK(b28 == false);

  ASIO_CONSTEXPR bool b29 = exec::can_set_value<
      free_set_value_const_receiver_0&, int, std::string>::value;
  ASIO_CHECK(b29 == false);

  ASIO_CONSTEXPR bool b30 = exec::can_set_value<
      const free_set_value_const_receiver_0&, int, std::string>::value;
  ASIO_CHECK(b30 == false);

  ASIO_CONSTEXPR bool b31 = exec::can_set_value<
      free_set_value_const_receiver_1&>::value;
  ASIO_CHECK(b31 == false);

  ASIO_CONSTEXPR bool b32 = exec::can_set_value<
      const free_set_value_const_receiver_1&>::value;
  ASIO_CHECK(b32 == false);

  ASIO_CONSTEXPR bool b33 = exec::can_set_value<
      free_set_value_const_receiver_1&, int>::value;
  ASIO_CHECK(b33 == true);

  ASIO_CONSTEXPR bool b34 = exec::can_set_value<
      const free_set_value_const_receiver_1&, int>::value;
  ASIO_CHECK(b34 == true);

  ASIO_CONSTEXPR bool b35 = exec::can_set_value<
      free_set_value_const_receiver_1&, int, std::string>::value;
  ASIO_CHECK(b35 == false);

  ASIO_CONSTEXPR bool b36 = exec::can_set_value<
      const free_set_value_const_receiver_1&, int, std::string>::value;
  ASIO_CHECK(b36 == false);

  ASIO_CONSTEXPR bool b37 = exec::can_set_value<
      free_set_value_const_receiver_2&>::value;
  ASIO_CHECK(b37 == false);

  ASIO_CONSTEXPR bool b38 = exec::can_set_value<
      const free_set_value_const_receiver_2&>::value;
  ASIO_CHECK(b38 == false);

  ASIO_CONSTEXPR bool b39 = exec::can_set_value<
      free_set_value_const_receiver_2&, int>::value;
  ASIO_CHECK(b39 == false);

  ASIO_CONSTEXPR bool b40 = exec::can_set_value<
      const free_set_value_const_receiver_2&, int>::value;
  ASIO_CHECK(b40 == false);

  ASIO_CONSTEXPR bool b41 = exec::can_set_value<
      free_set_value_const_receiver_2&, int, std::string>::value;
  ASIO_CHECK(b41 == true);

  ASIO_CONSTEXPR bool b42 = exec::can_set_value<
      const free_set_value_const_receiver_2&, int, std::string>::value;
  ASIO_CHECK(b42 == true);

  ASIO_CONSTEXPR bool b61 = exec::can_set_value<
      free_set_value_non_const_receiver_0&>::value;
  ASIO_CHECK(b61 == true);

  ASIO_CONSTEXPR bool b62 = exec::can_set_value<
      const free_set_value_non_const_receiver_0&>::value;
  ASIO_CHECK(b62 == false);

  ASIO_CONSTEXPR bool b63 = exec::can_set_value<
      free_set_value_non_const_receiver_0&, int>::value;
  ASIO_CHECK(b63 == false);

  ASIO_CONSTEXPR bool b64 = exec::can_set_value<
      const free_set_value_non_const_receiver_0&, int>::value;
  ASIO_CHECK(b64 == false);

  ASIO_CONSTEXPR bool b65 = exec::can_set_value<
      free_set_value_non_const_receiver_0&, int, std::string>::value;
  ASIO_CHECK(b65 == false);

  ASIO_CONSTEXPR bool b66 = exec::can_set_value<
      const free_set_value_non_const_receiver_0&, int, std::string>::value;
  ASIO_CHECK(b66 == false);

  ASIO_CONSTEXPR bool b67 = exec::can_set_value<
      free_set_value_non_const_receiver_1&>::value;
  ASIO_CHECK(b67 == false);

  ASIO_CONSTEXPR bool b68 = exec::can_set_value<
      const free_set_value_non_const_receiver_1&>::value;
  ASIO_CHECK(b68 == false);

  ASIO_CONSTEXPR bool b69 = exec::can_set_value<
      free_set_value_non_const_receiver_1&, int>::value;
  ASIO_CHECK(b69 == true);

  ASIO_CONSTEXPR bool b70 = exec::can_set_value<
      const free_set_value_non_const_receiver_1&, int>::value;
  ASIO_CHECK(b70 == false);

  ASIO_CONSTEXPR bool b71 = exec::can_set_value<
      free_set_value_non_const_receiver_1&, int, std::string>::value;
  ASIO_CHECK(b71 == false);

  ASIO_CONSTEXPR bool b72 = exec::can_set_value<
      const free_set_value_non_const_receiver_1&, int, std::string>::value;
  ASIO_CHECK(b72 == false);

  ASIO_CONSTEXPR bool b73 = exec::can_set_value<
      free_set_value_non_const_receiver_2&>::value;
  ASIO_CHECK(b73 == false);

  ASIO_CONSTEXPR bool b74 = exec::can_set_value<
      const free_set_value_non_const_receiver_2&>::value;
  ASIO_CHECK(b74 == false);

  ASIO_CONSTEXPR bool b75 = exec::can_set_value<
      free_set_value_non_const_receiver_2&, int>::value;
  ASIO_CHECK(b75 == false);

  ASIO_CONSTEXPR bool b76 = exec::can_set_value<
      const free_set_value_non_const_receiver_2&, int>::value;
  ASIO_CHECK(b76 == false);

  ASIO_CONSTEXPR bool b77 = exec::can_set_value<
      free_set_value_non_const_receiver_2&, int, std::string>::value;
  ASIO_CHECK(b77 == true);

  ASIO_CONSTEXPR bool b78 = exec::can_set_value<
      const free_set_value_non_const_receiver_2&, int, std::string>::value;
  ASIO_CHECK(b78 == false);
}

void increment(int* count)
{
  ++(*count);
}

void test_set_value()
{
  call_count = 0;
  free_set_value_const_receiver_0 ex7 = {};
  exec::set_value(ex7);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  const free_set_value_const_receiver_0 ex8 = {};
  exec::set_value(ex8);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  exec::set_value(free_set_value_const_receiver_0());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  free_set_value_const_receiver_1 ex9 = {};
  exec::set_value(ex9, 123);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  const free_set_value_const_receiver_1 ex10 = {};
  exec::set_value(ex10, 123);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  exec::set_value(free_set_value_const_receiver_1(), 123);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  free_set_value_const_receiver_2 ex11 = {};
  exec::set_value(ex11, 123, std::string());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  const free_set_value_const_receiver_2 ex12 = {};
  exec::set_value(ex12, 123, std::string());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  exec::set_value(free_set_value_const_receiver_2(), 123, std::string());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  free_set_value_non_const_receiver_0 ex16 = {};
  exec::set_value(ex16);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  free_set_value_non_const_receiver_1 ex17 = {};
  exec::set_value(ex17, 123);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  free_set_value_non_const_receiver_2 ex18 = {};
  exec::set_value(ex18, 123, std::string());
  ASIO_CHECK(call_count == 1);
}

ASIO_TEST_SUITE
(
  "set_value",
  ASIO_TEST_CASE(test_can_set_value)
  ASIO_TEST_CASE(test_set_value)
)
