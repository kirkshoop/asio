//
// schedule.cpp
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
#include "asio/execution/schedule.hpp"

#include "asio/error_code.hpp"
#include "asio/execution/sender.hpp"
#include "asio/execution/start.hpp"
#include "asio/execution/submit.hpp"
#include "../unit_test.hpp"

namespace exec = asio::execution;

struct operation_state
{
  friend void tag_invoke(decltype(exec::start), operation_state&) ASIO_NOEXCEPT
  {
  }
};

struct sender : exec::sender_base
{
  sender()
  {
  }

  template <typename R>
  friend
  operation_state tag_invoke(decltype(exec::connect), const sender&, ASIO_MOVE_ARG(R) r)
  {
    (void)r;
    return operation_state();
  }

  template <typename R>
  friend
  void tag_invoke(decltype(exec::submit), const sender&, ASIO_MOVE_ARG(R) r)
  {
    typename asio::decay<R>::type tmp(ASIO_MOVE_CAST(R)(r));
    exec::set_value(tmp);
  }
};

struct no_schedule
{
};

struct free_schedule_const_receiver
{
  friend sender tag_invoke(decltype(exec::schedule),
      const free_schedule_const_receiver&) ASIO_NOEXCEPT
  {
    return sender();
  }
};

struct free_schedule_non_const_receiver
{
  friend sender tag_invoke(decltype(exec::schedule),
      free_schedule_non_const_receiver&) ASIO_NOEXCEPT
  {
    return sender();
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
    typename asio::decay<F>::type tmp(ASIO_MOVE_CAST(F)(f));
    tmp();
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

void test_can_schedule()
{
  ASIO_CONSTEXPR bool b1 = exec::can_schedule<
      no_schedule&>::value;
  ASIO_CHECK(b1 == false);

  ASIO_CONSTEXPR bool b2 = exec::can_schedule<
      const no_schedule&>::value;
  ASIO_CHECK(b2 == false);

  ASIO_CONSTEXPR bool b5 = exec::can_schedule<
      free_schedule_const_receiver&>::value;
  ASIO_CHECK(b5 == true);

  ASIO_CONSTEXPR bool b6 = exec::can_schedule<
      const free_schedule_const_receiver&>::value;
  ASIO_CHECK(b6 == true);

  ASIO_CONSTEXPR bool b9 = exec::can_schedule<
      free_schedule_non_const_receiver&>::value;
  ASIO_CHECK(b9 == true);

  ASIO_CONSTEXPR bool b10 = exec::can_schedule<
      const free_schedule_non_const_receiver&>::value;
  ASIO_CHECK(b10 == false);

  ASIO_CONSTEXPR bool b11 = exec::can_schedule<
      executor&>::value;
  ASIO_CHECK(b11 == true);

  ASIO_CONSTEXPR bool b12 = exec::can_schedule<
      const executor&>::value;
  ASIO_CHECK(b12 == true);
}

struct receiver
{
  int* count_;

  receiver(int* count)
    : count_(count)
  {
  }

  receiver(const receiver& other) ASIO_NOEXCEPT
    : count_(other.count_)
  {
  }

#if defined(ASIO_HAS_MOVE)
  receiver(receiver&& other) ASIO_NOEXCEPT
    : count_(other.count_)
  {
    other.count_ = 0;
  }
#endif // defined(ASIO_HAS_MOVE)

  void set_value() ASIO_NOEXCEPT
  {
    ++(*count_);
  }

  template <typename E>
  friend void tag_invoke(decltype(asio::execution::set_error), ASIO_MOVE_ARG(receiver), ASIO_MOVE_ARG(E) e) ASIO_NOEXCEPT
  {
    (void)e;
  }

  friend void tag_invoke(decltype(exec::set_done), ASIO_MOVE_ARG(receiver)) ASIO_NOEXCEPT
  {
  }
};

namespace asio {
namespace traits {

#if !defined(ASIO_HAS_DEDUCED_SET_VALUE_MEMBER_TRAIT)

template <>
struct set_value_member<receiver, void()>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);
  typedef void result_type;
};

#endif // !defined(ASIO_HAS_DEDUCED_SET_VALUE_MEMBER_TRAIT)

} // namespace traits
} // namespace asio

void test_schedule()
{
  int count = 0;
  free_schedule_const_receiver ex3 = {};
  exec::submit(
      exec::schedule(ex3),
      receiver(&count));
  ASIO_CHECK(count == 1);

  count = 0;
  const free_schedule_const_receiver ex4 = {};
  exec::submit(
      exec::schedule(ex4),
      receiver(&count));
  ASIO_CHECK(count == 1);

  count = 0;
  exec::submit(
      exec::schedule(free_schedule_const_receiver()),
      receiver(&count));
  ASIO_CHECK(count == 1);

  count = 0;
  free_schedule_non_const_receiver ex6 = {};
  exec::submit(
      exec::schedule(ex6),
      receiver(&count));
  ASIO_CHECK(count == 1);

  count = 0;
  executor ex7;
  exec::submit(
      exec::schedule(ex7),
      receiver(&count));
  ASIO_CHECK(count == 1);

  count = 0;
  const executor ex8;
  exec::submit(
      exec::schedule(ex8),
      receiver(&count));
  ASIO_CHECK(count == 1);

  count = 0;
  exec::submit(
      exec::schedule(executor()),
      receiver(&count));
  ASIO_CHECK(count == 1);
}

ASIO_TEST_SUITE
(
  "schedule",
  ASIO_TEST_CASE(test_can_schedule)
  ASIO_TEST_CASE(test_schedule)
)
