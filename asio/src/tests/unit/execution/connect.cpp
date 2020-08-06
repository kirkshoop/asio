//
// connect.cpp
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
#include "asio/execution/connect.hpp"

#include "asio/execution/start.hpp"
#include "asio/error_code.hpp"
#include "../unit_test.hpp"

namespace exec = asio::execution;

static int call_count = 0;

struct operation_state
{
  friend void tag_invoke(decltype(exec::start), operation_state&) ASIO_NOEXCEPT
  {
  }
};

struct no_connect_1
{
};

struct no_connect_2 : exec::sender_base
{
};

struct free_tag_invoke_connect_const_receiver : exec::sender_base
{
  free_tag_invoke_connect_const_receiver()
  {
  }

  template <typename R>
  friend operation_state tag_invoke(decltype(exec::connect), 
      const free_tag_invoke_connect_const_receiver&, ASIO_MOVE_ARG(R) r)
  {
    (void)r;
    ++call_count;
    return operation_state();
  }
};

struct free_tag_invoke_connect_non_const_receiver : exec::sender_base
{
  free_tag_invoke_connect_non_const_receiver()
  {
  }

  template <typename R>
  friend operation_state tag_invoke(decltype(exec::connect), 
      free_tag_invoke_connect_non_const_receiver&, ASIO_MOVE_ARG(R) r)
  {
    (void)r;
    ++call_count;
    return operation_state();
  }
};

struct receiver
{
  receiver()
  {
  }

  receiver(const receiver&)
  {
  }

#if defined(ASIO_HAS_MOVE)
  receiver(receiver&&) ASIO_NOEXCEPT
  {
  }
#endif // defined(ASIO_HAS_MOVE)

  template <typename E>
  friend void tag_invoke(decltype(asio::execution::set_error), ASIO_MOVE_ARG(receiver), ASIO_MOVE_ARG(E) e) ASIO_NOEXCEPT
  {
    (void)e;
  }

  friend void tag_invoke(decltype(exec::set_done), ASIO_MOVE_ARG(receiver) self) ASIO_NOEXCEPT
  {
  }
};

struct executor
{
  executor()
  {
  }

  executor(const executor&) ASIO_NOEXCEPT
  {
  }

#if defined(ASIO_HAS_MOVE)
  executor(executor&&) ASIO_NOEXCEPT
  {
  }
#endif // defined(ASIO_HAS_MOVE)

  template <typename F>
  void execute(ASIO_MOVE_ARG(F) f) const ASIO_NOEXCEPT
  {
    (void)f;
  }

  bool operator==(const executor&) const ASIO_NOEXCEPT
  {
    return true;
  }

  bool operator!=(const executor&) const ASIO_NOEXCEPT
  {
    return false;
  }
};

namespace asio {
namespace traits {

#if !defined(ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)

template <typename F>
struct execute_member<executor, F>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);
  typedef void result_type;
};

#endif // !defined(ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)
#if !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

template <>
struct equality_comparable<executor>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);
};

#endif // !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

} // namespace traits
} // namespace asio

void test_can_connect()
{
  ASIO_CONSTEXPR bool b1 = exec::can_connect<
      no_connect_1&, receiver>::value;
  ASIO_CHECK(b1 == false);

  ASIO_CONSTEXPR bool b2 = exec::can_connect<
      const no_connect_1&, receiver>::value;
  ASIO_CHECK(b2 == false);

  ASIO_CONSTEXPR bool b3 = exec::can_connect<
      no_connect_2&, receiver>::value;
  ASIO_CHECK(b3 == false);

  ASIO_CONSTEXPR bool b4 = exec::can_connect<
      const no_connect_2&, receiver>::value;
  ASIO_CHECK(b4 == false);

  ASIO_CONSTEXPR bool b5 = exec::can_connect<
      free_tag_invoke_connect_const_receiver&, receiver>::value;
  ASIO_CHECK(b5 == true);

  ASIO_CONSTEXPR bool b6 = exec::can_connect<
      const free_tag_invoke_connect_const_receiver&, receiver>::value;
  ASIO_CHECK(b6 == true);

  ASIO_CONSTEXPR bool b7 = exec::can_connect<
      free_tag_invoke_connect_non_const_receiver&, receiver>::value;
  ASIO_CHECK(b7 == true);

  ASIO_CONSTEXPR bool b8 = exec::can_connect<
      const free_tag_invoke_connect_non_const_receiver&, receiver>::value;
  ASIO_CHECK(b8 == false);

  ASIO_CONSTEXPR bool b9 = exec::can_connect<
      executor&, receiver>::value;
  ASIO_CHECK(b9 == true);

  ASIO_CONSTEXPR bool b10 = exec::can_connect<
      const executor&, receiver>::value;
  ASIO_CHECK(b10 == true);
}

void increment(int* count)
{
  ++(*count);
}

void test_connect()
{
  receiver r;

  call_count = 0;
  free_tag_invoke_connect_const_receiver s3;
  operation_state o4 = exec::connect(s3, r);
  ASIO_CHECK(call_count == 1);
  (void)o4;

  call_count = 0;
  const free_tag_invoke_connect_const_receiver s4;
  operation_state o5 = exec::connect(s4, r);
  ASIO_CHECK(call_count == 1);
  (void)o5;

  call_count = 0;
  operation_state o6 = exec::connect(free_tag_invoke_connect_const_receiver(), r);
  ASIO_CHECK(call_count == 1);
  (void)o6;

  call_count = 0;
  free_tag_invoke_connect_non_const_receiver s6;
  operation_state o8 = exec::connect(s6, r);
  ASIO_CHECK(call_count == 1);
  (void)o8;

  executor s7;
  exec::connect_result<executor&,
      receiver&>::type o9 = exec::connect(s7, r);
  ASIO_CHECK((
      exec::is_operation_state<
        exec::connect_result<executor&, receiver&>::type
      >::value));
  (void)o9;

  const executor s8;
  exec::connect_result<const executor&,
      receiver&>::type o10 = exec::connect(s8, r);
  (void)exec::connect(s8, r);
  ASIO_CHECK((
      exec::is_operation_state<
        exec::connect_result<const executor&, receiver&>::type
      >::value));
  (void)o10;
}

ASIO_TEST_SUITE
(
  "connect",
  ASIO_TEST_CASE(test_can_connect)
  ASIO_TEST_CASE(test_connect)
)
