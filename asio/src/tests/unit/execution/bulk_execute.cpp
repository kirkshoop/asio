//
// bulk_execute.cpp
// ~~~~~~~~~~~~~~~~
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
#include "asio/execution/bulk_execute.hpp"

#include "asio/execution.hpp"
#include "../unit_test.hpp"

namespace exec = asio::execution;

int call_count = 0;

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
};

struct no_bulk_execute
{
};

struct free_bulk_execute
{
  free_bulk_execute()
  {
  }

  template <typename F>
  friend sender tag_invoke(decltype(exec::bulk_execute), const free_bulk_execute&,
      ASIO_MOVE_ARG(F), std::size_t)
  {
    ++call_count;
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
  void tag_invoke(decltype(exec::execute), ASIO_MOVE_ARG(F) f) const ASIO_NOEXCEPT
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

void test_can_bulk_execute()
{
  ASIO_CONSTEXPR bool b1 = exec::can_bulk_execute<
      no_bulk_execute&, exec::invocable_archetype, std::size_t>::value;
  ASIO_CHECK(b1 == false);

  ASIO_CONSTEXPR bool b2 = exec::can_bulk_execute<
      const no_bulk_execute&, exec::invocable_archetype, std::size_t>::value;
  ASIO_CHECK(b2 == false);

  ASIO_CONSTEXPR bool b5 = exec::can_bulk_execute<
      free_bulk_execute&, exec::invocable_archetype, std::size_t>::value;
  ASIO_CHECK(b5 == true);

  ASIO_CONSTEXPR bool b6 = exec::can_bulk_execute<
      const free_bulk_execute&, exec::invocable_archetype, std::size_t>::value;
  ASIO_CHECK(b6 == true);

  ASIO_CONSTEXPR bool b7 = exec::can_bulk_execute<
      executor&, exec::invocable_archetype, std::size_t>::value;
  ASIO_CHECK(b7 == true);

  ASIO_CONSTEXPR bool b8 = exec::can_bulk_execute<
      const executor&, exec::invocable_archetype, std::size_t>::value;
  ASIO_CHECK(b8 == true);
}

void handler(std::size_t)
{
}

void counting_handler(std::size_t)
{
  ++call_count;
}

void completion_handler()
{
  ++call_count;
}

void test_bulk_execute()
{
  call_count = 0;
  free_bulk_execute ex3;
  exec::bulk_execute(ex3, handler, 2);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  const free_bulk_execute ex4;
  exec::bulk_execute(ex4, handler, 2);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  exec::bulk_execute(free_bulk_execute(), handler, 2);
  ASIO_CHECK(call_count == 1);

  call_count = 0;
  executor ex5;
  exec::execute(
      exec::bulk_execute(ex5, counting_handler, 10u),
      completion_handler);
  ASIO_CHECK(call_count == 11);

  call_count = 0;
  const executor ex6;
  exec::execute(
      exec::bulk_execute(ex6, counting_handler, 10u),
      completion_handler);
  ASIO_CHECK(call_count == 11);

  call_count = 0;
  exec::execute(
      exec::bulk_execute(executor(), counting_handler, 10u),
      completion_handler);
  ASIO_CHECK(call_count == 11);
}

ASIO_TEST_SUITE
(
  "bulk_execute",
  ASIO_TEST_CASE(test_can_bulk_execute)
  ASIO_TEST_CASE(test_bulk_execute)
)
