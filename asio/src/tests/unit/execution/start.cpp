//
// start.cpp
// ~~~~~~~~~
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
#include "asio/execution/start.hpp"

#include "asio/error_code.hpp"
#include "../unit_test.hpp"

namespace exec = asio::execution;

static int call_count = 0;

struct no_start
{
};

struct free_start_const_operation_state
{
  friend void tag_invoke(decltype(exec::start), const free_start_const_operation_state&) ASIO_NOEXCEPT
  {
    ++call_count;
  }
};

struct free_start_non_const_operation_state
{
  friend void tag_invoke(decltype(exec::start), free_start_non_const_operation_state&) ASIO_NOEXCEPT
  {
    ++call_count;
  }
};

void test_can_start()
{
  ASIO_CONSTEXPR bool b1 = exec::can_start<
      no_start&>::value;
  ASIO_CHECK(b1 == false);

  ASIO_CONSTEXPR bool b2 = exec::can_start<
      const no_start&>::value;
  ASIO_CHECK(b2 == false);

  ASIO_CONSTEXPR bool b5 = exec::can_start<
      free_start_const_operation_state&>::value;
  ASIO_CHECK(b5 == true);

  ASIO_CONSTEXPR bool b6 = exec::can_start<
      const free_start_const_operation_state&>::value;
  ASIO_CHECK(b6 == true);

  ASIO_CONSTEXPR bool b9 = exec::can_start<
      free_start_non_const_operation_state&>::value;
  ASIO_CHECK(b9 == true);

  ASIO_CONSTEXPR bool b10 = exec::can_start<
      const free_start_non_const_operation_state&>::value;
  ASIO_CHECK(b10 == false);
}

void increment(int* count)
{
  ++(*count);
}

void test_start()
{
  call_count = 0;
  free_start_const_operation_state ex3 = {};
  exec::start(ex3);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  const free_start_const_operation_state ex4 = {};
  exec::start(ex4);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  exec::start(free_start_const_operation_state());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  free_start_non_const_operation_state ex6 = {};
  exec::start(ex6);
  ASIO_CHECK(call_count == 1);
}

ASIO_TEST_SUITE
(
  "start",
  ASIO_TEST_CASE(test_can_start)
  ASIO_TEST_CASE(test_start)
)
