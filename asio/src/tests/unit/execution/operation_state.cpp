//
// operation_state.cpp
// ~~~~~~~~~~~~~~~~~~~
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
#include "asio/execution/operation_state.hpp"

#include <string>
#include "asio/error_code.hpp"
#include "../unit_test.hpp"

struct not_an_operation_state_1
{
};

struct operation_state
{
  friend void tag_invoke(decltype(asio::execution::start), operation_state&) ASIO_NOEXCEPT
  {
  }
};

void is_operation_state_test()
{
  ASIO_CHECK((
      !asio::execution::is_operation_state<
        void
      >::value));

  ASIO_CHECK((
      !asio::execution::is_operation_state<
        not_an_operation_state_1
      >::value));

  ASIO_CHECK((
      asio::execution::is_operation_state<
        operation_state
      >::value));
}

ASIO_TEST_SUITE
(
  "operation_state",
  ASIO_TEST_CASE(is_operation_state_test)
)
