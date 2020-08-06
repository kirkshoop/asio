//
// set_done.cpp
// ~~~~~~~~~~~~
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
#include "asio/execution/set_done.hpp"

#include "asio/error_code.hpp"
#include "../unit_test.hpp"

namespace exec = asio::execution;

static int call_count = 0;

struct no_set_done
{
};

struct free_set_done_const_receiver
{
  friend void tag_invoke(decltype(exec::set_done), const free_set_done_const_receiver&) ASIO_NOEXCEPT
  {
    ++call_count;
  }
};

struct free_set_done_non_const_receiver
{
  friend void tag_invoke(decltype(exec::set_done), free_set_done_non_const_receiver&) ASIO_NOEXCEPT
  {
    ++call_count;
  }
};

void test_can_set_done()
{
  ASIO_CONSTEXPR bool b1 = exec::can_set_done<
      no_set_done&>::value;
  ASIO_CHECK(b1 == false);

  ASIO_CONSTEXPR bool b2 = exec::can_set_done<
      const no_set_done&>::value;
  ASIO_CHECK(b2 == false);

  ASIO_CONSTEXPR bool b5 = exec::can_set_done<
      free_set_done_const_receiver&>::value;
  ASIO_CHECK(b5 == true);

  ASIO_CONSTEXPR bool b6 = exec::can_set_done<
      const free_set_done_const_receiver&>::value;
  ASIO_CHECK(b6 == true);

  ASIO_CONSTEXPR bool b9 = exec::can_set_done<
      free_set_done_non_const_receiver&>::value;
  ASIO_CHECK(b9 == true);

  ASIO_CONSTEXPR bool b10 = exec::can_set_done<
      const free_set_done_non_const_receiver&>::value;
  ASIO_CHECK(b10 == false);
}

void increment(int* count)
{
  ++(*count);
}

void test_set_done()
{
  call_count = 0;
  free_set_done_const_receiver ex3 = {};
  exec::set_done(ex3);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  const free_set_done_const_receiver ex4 = {};
  exec::set_done(ex4);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  exec::set_done(free_set_done_const_receiver());
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  free_set_done_non_const_receiver ex6 = {};
  exec::set_done(ex6);
  ASIO_CHECK(call_count == 1);
}

ASIO_TEST_SUITE
(
  "set_done",
  ASIO_TEST_CASE(test_can_set_done)
  ASIO_TEST_CASE(test_set_done)
)
