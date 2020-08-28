//
// any_ref.cpp
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
#include "asio/tag_invokes/any_ref.hpp"

#include <cstring>
#include "asio/thread_pool.hpp"
#include "asio/execution/get_occupancy.hpp"
#include "../unit_test.hpp"

#if defined(ASIO_HAS_BOOST_BIND)
# include <boost/bind/bind.hpp>
#else // defined(ASIO_HAS_BOOST_BIND)
# include <functional>
#endif // defined(ASIO_HAS_BOOST_BIND)

using namespace asio;

#if defined(ASIO_HAS_BOOST_BIND)
namespace bindns = boost;
#else // defined(ASIO_HAS_BOOST_BIND)
namespace bindns = std;
#endif

struct fat_executor
{
  fat_executor(int id)
    : id_(id)
  {
    std::memset(data_, 0, sizeof(data_));
  }

  template <typename F>
  void tag_invoke(decltype(execution::execute), const F&) const
  {
  }

  std::size_t tag_invoke(decltype(execution::get_occupancy)) const
  {
      return 1;
  }

  decltype(execution::always_blocking) tag_invoke(decltype(execution::get_blocking)) const
  {
      return execution::always_blocking;
  }

  friend bool operator==(const fat_executor& a,
      const fat_executor& b) ASIO_NOEXCEPT
  {
    return a.id_ == b.id_;
  }

  friend bool operator!=(const fat_executor& a,
      const fat_executor& b) ASIO_NOEXCEPT
  {
    return a.id_ != b.id_;
  }

  int id_;
  unsigned char data_[1024];
};

namespace asio {
namespace traits {

#if !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

template <>
struct equality_comparable<fat_executor>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);
};

#endif // !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

} // namespace traits
} // namespace asio

void increment(int* count)
{
  ++(*count);
}

struct test_receiver {
  test_receiver(int* count) : count_(count) {}
  int* count_;
  friend void tag_invoke(decltype(execution::set_value), test_receiver&& self, int, long, std::string)
  {
    increment(self.count_);
  }
  friend void tag_invoke(decltype(execution::set_error), test_receiver&& self, std::exception_ptr) noexcept
  {
    increment(self.count_);
  }
  friend void tag_invoke(decltype(execution::set_done), test_receiver&& self) noexcept
  {
    increment(self.count_);
  }
};

void any_ref_construction_test()
{
  typedef tag_invokes::any_ref<> ex_no_props_t;

  typedef tag_invokes::any_ref<
      typename execution::get_blocking_o<>::type
    > ex_one_prop_t;

  typedef tag_invokes::any_ref<
      typename execution::get_blocking_o<>::type,
      typename execution::get_occupancy_o<>::type
    > ex_two_props_t;

  thread_pool pool(1);
  asio::nullptr_t null_ptr = asio::nullptr_t();

  ex_two_props_t ex_two_props_1;

  ASIO_CHECK(!ex_two_props_1);
  ASIO_CHECK(ex_two_props_1 == null_ptr);

  ex_two_props_t ex_two_props_2(null_ptr);

  ASIO_CHECK(!ex_two_props_2);
  ASIO_CHECK(ex_two_props_2 == null_ptr);
  ASIO_CHECK(ex_two_props_2 == ex_two_props_1);

  ex_two_props_t ex_two_props_3(pool.executor());

  ASIO_CHECK(!!ex_two_props_3);
  ASIO_CHECK(ex_two_props_3 != null_ptr);
  ASIO_CHECK(ex_two_props_3 != ex_two_props_1);

  ex_two_props_t ex_two_props_4(ex_two_props_1);

  ASIO_CHECK(!ex_two_props_4);
  ASIO_CHECK(ex_two_props_4 == null_ptr);
  ASIO_CHECK(ex_two_props_4 == ex_two_props_1);

  ex_two_props_t ex_two_props_5(ex_two_props_3);

  ASIO_CHECK(!!ex_two_props_5);
  ASIO_CHECK(ex_two_props_5 != null_ptr);
  ASIO_CHECK(ex_two_props_5 == ex_two_props_3);

  ex_two_props_t ex_two_props_6 = fat_executor(1);

  ASIO_CHECK(!!ex_two_props_6);
  ASIO_CHECK(ex_two_props_6 != null_ptr);
  ASIO_CHECK(ex_two_props_6 != ex_two_props_1);

  ex_two_props_t ex_two_props_7 = fat_executor(1);

  ASIO_CHECK(!!ex_two_props_7);
  ASIO_CHECK(ex_two_props_7 != null_ptr);
  ASIO_CHECK(ex_two_props_7 != ex_two_props_1);
  ASIO_CHECK(ex_two_props_7 == ex_two_props_6);

  ex_two_props_t ex_two_props_8 = fat_executor(2);

  ASIO_CHECK(!!ex_two_props_8);
  ASIO_CHECK(ex_two_props_8 != null_ptr);
  ASIO_CHECK(ex_two_props_8 != ex_two_props_1);
  ASIO_CHECK(ex_two_props_8 != ex_two_props_6);
  ASIO_CHECK(ex_two_props_8 != ex_two_props_7);

  ex_two_props_t ex_two_props_9(ex_two_props_6);

  ASIO_CHECK(!!ex_two_props_9);
  ASIO_CHECK(ex_two_props_9 != null_ptr);
  ASIO_CHECK(ex_two_props_9 != ex_two_props_1);
  ASIO_CHECK(ex_two_props_9 == ex_two_props_6);
  ASIO_CHECK(ex_two_props_9 == ex_two_props_7);
  ASIO_CHECK(ex_two_props_9 != ex_two_props_8);

#if defined(ASIO_HAS_MOVE)
  ex_two_props_t ex_two_props_10(std::move(ex_two_props_1));

  ASIO_CHECK(!ex_two_props_10);
  ASIO_CHECK(ex_two_props_10 == null_ptr);
  ASIO_CHECK(!ex_two_props_1);
  ASIO_CHECK(ex_two_props_1 == null_ptr);

  ex_two_props_t ex_two_props_11(std::move(ex_two_props_3));

  ASIO_CHECK(!!ex_two_props_11);
  ASIO_CHECK(ex_two_props_11 != null_ptr);
  ASIO_CHECK(!ex_two_props_3);
  ASIO_CHECK(ex_two_props_3 == null_ptr);
  ASIO_CHECK(ex_two_props_11 == ex_two_props_5);

  ex_two_props_t ex_two_props_12(std::move(ex_two_props_7));

  ASIO_CHECK(!!ex_two_props_12);
  ASIO_CHECK(ex_two_props_12 != null_ptr);
  ASIO_CHECK(!ex_two_props_7);
  ASIO_CHECK(ex_two_props_7 == null_ptr);
  ASIO_CHECK(ex_two_props_12 == ex_two_props_6);
  ASIO_CHECK(ex_two_props_12 != ex_two_props_8);
#endif // defined(ASIO_HAS_MOVE)

  ex_one_prop_t ex_one_prop_1;

  ASIO_CHECK(!ex_one_prop_1);
  ASIO_CHECK(ex_one_prop_1 == null_ptr);

  ex_one_prop_t ex_one_prop_2(null_ptr);

  ASIO_CHECK(!ex_one_prop_2);
  ASIO_CHECK(ex_one_prop_2 == null_ptr);
  ASIO_CHECK(ex_one_prop_2 == ex_one_prop_1);

  ex_one_prop_t ex_one_prop_3(pool.executor());

  ASIO_CHECK(!!ex_one_prop_3);
  ASIO_CHECK(ex_one_prop_3 != null_ptr);
  ASIO_CHECK(ex_one_prop_3 != ex_one_prop_1);

  ex_one_prop_t ex_one_prop_4(ex_one_prop_1);

  ASIO_CHECK(!ex_one_prop_4);
  ASIO_CHECK(ex_one_prop_4 == null_ptr);
  ASIO_CHECK(ex_one_prop_4 == ex_one_prop_1);

  ex_one_prop_t ex_one_prop_5(ex_one_prop_3);

  ASIO_CHECK(!!ex_one_prop_5);
  ASIO_CHECK(ex_one_prop_5 != null_ptr);
  ASIO_CHECK(ex_one_prop_5 == ex_one_prop_3);

  ex_one_prop_t ex_one_prop_6 = fat_executor(1);

  ASIO_CHECK(!!ex_one_prop_6);
  ASIO_CHECK(ex_one_prop_6 != null_ptr);
  ASIO_CHECK(ex_one_prop_6 != ex_one_prop_1);

  ex_one_prop_t ex_one_prop_7 = fat_executor(1);

  ASIO_CHECK(!!ex_one_prop_7);
  ASIO_CHECK(ex_one_prop_7 != null_ptr);
  ASIO_CHECK(ex_one_prop_7 != ex_one_prop_1);
  ASIO_CHECK(ex_one_prop_7 == ex_one_prop_6);

  ex_one_prop_t ex_one_prop_8 = fat_executor(2);

  ASIO_CHECK(!!ex_one_prop_8);
  ASIO_CHECK(ex_one_prop_8 != null_ptr);
  ASIO_CHECK(ex_one_prop_8 != ex_one_prop_1);
  ASIO_CHECK(ex_one_prop_8 != ex_one_prop_6);
  ASIO_CHECK(ex_one_prop_8 != ex_one_prop_7);

  ex_one_prop_t ex_one_prop_9(ex_one_prop_6);

  ASIO_CHECK(!!ex_one_prop_9);
  ASIO_CHECK(ex_one_prop_9 != null_ptr);
  ASIO_CHECK(ex_one_prop_9 != ex_one_prop_1);
  ASIO_CHECK(ex_one_prop_9 == ex_one_prop_6);
  ASIO_CHECK(ex_one_prop_9 == ex_one_prop_7);
  ASIO_CHECK(ex_one_prop_9 != ex_one_prop_8);

#if defined(ASIO_HAS_MOVE)
  ex_one_prop_t ex_one_prop_10(std::move(ex_one_prop_1));

  ASIO_CHECK(!ex_one_prop_10);
  ASIO_CHECK(ex_one_prop_10 == null_ptr);
  ASIO_CHECK(!ex_one_prop_1);
  ASIO_CHECK(ex_one_prop_1 == null_ptr);

  ex_one_prop_t ex_one_prop_11(std::move(ex_one_prop_3));

  ASIO_CHECK(!!ex_one_prop_11);
  ASIO_CHECK(ex_one_prop_11 != null_ptr);
  ASIO_CHECK(!ex_one_prop_3);
  ASIO_CHECK(ex_one_prop_3 == null_ptr);
  ASIO_CHECK(ex_one_prop_11 == ex_one_prop_5);

  ex_one_prop_t ex_one_prop_12(std::move(ex_one_prop_7));

  ASIO_CHECK(!!ex_one_prop_12);
  ASIO_CHECK(ex_one_prop_12 != null_ptr);
  ASIO_CHECK(!ex_one_prop_7);
  ASIO_CHECK(ex_one_prop_7 == null_ptr);
  ASIO_CHECK(ex_one_prop_12 == ex_one_prop_6);
  ASIO_CHECK(ex_one_prop_12 != ex_one_prop_8);
#endif // defined(ASIO_HAS_MOVE)

  ex_one_prop_t ex_one_prop_13(ex_two_props_1);

  ASIO_CHECK(!ex_one_prop_13);
  ASIO_CHECK(ex_one_prop_13 == null_ptr);

  ex_one_prop_t ex_one_prop_14(ex_two_props_5);

  ASIO_CHECK(!!ex_one_prop_14);
  ASIO_CHECK(ex_one_prop_14 != null_ptr);

  ex_one_prop_t ex_one_prop_15(ex_two_props_9);

  ASIO_CHECK(!!ex_one_prop_15);
  ASIO_CHECK(ex_one_prop_15 != null_ptr);

  ex_no_props_t ex_no_props_1;

   ASIO_CHECK(!ex_no_props_1);
  ASIO_CHECK(ex_no_props_1 == null_ptr);

  ex_no_props_t ex_no_props_2(null_ptr);

   ASIO_CHECK(!ex_no_props_2);
  ASIO_CHECK(ex_no_props_2 == null_ptr);
  ASIO_CHECK(ex_no_props_2 == ex_no_props_1);

  ex_no_props_t ex_no_props_3(pool.executor());

   ASIO_CHECK(!!ex_no_props_3);
  ASIO_CHECK(ex_no_props_3 != null_ptr);
  ASIO_CHECK(ex_no_props_3 != ex_no_props_1);

  ex_no_props_t ex_no_props_4(ex_no_props_1);

   ASIO_CHECK(!ex_no_props_4);
  ASIO_CHECK(ex_no_props_4 == null_ptr);
  ASIO_CHECK(ex_no_props_4 == ex_no_props_1);

  ex_no_props_t ex_no_props_5(ex_no_props_3);

   ASIO_CHECK(!!ex_no_props_5);
  ASIO_CHECK(ex_no_props_5 != null_ptr);
  ASIO_CHECK(ex_no_props_5 == ex_no_props_3);

  ex_no_props_t ex_no_props_6 = fat_executor(1);

   ASIO_CHECK(!!ex_no_props_6);
  ASIO_CHECK(ex_no_props_6 != null_ptr);
  ASIO_CHECK(ex_no_props_6 != ex_no_props_1);

  ex_no_props_t ex_no_props_7 = fat_executor(1);

   ASIO_CHECK(!!ex_no_props_7);
  ASIO_CHECK(ex_no_props_7 != null_ptr);
  ASIO_CHECK(ex_no_props_7 != ex_no_props_1);
  ASIO_CHECK(ex_no_props_7 == ex_no_props_6);

  ex_no_props_t ex_no_props_8 = fat_executor(2);

   ASIO_CHECK(!!ex_no_props_8);
  ASIO_CHECK(ex_no_props_8 != null_ptr);
  ASIO_CHECK(ex_no_props_8 != ex_no_props_1);
  ASIO_CHECK(ex_no_props_8 != ex_no_props_6);
  ASIO_CHECK(ex_no_props_8 != ex_no_props_7);

  ex_no_props_t ex_no_props_9(ex_no_props_6);

   ASIO_CHECK(!!ex_no_props_9);
  ASIO_CHECK(ex_no_props_9 != null_ptr);
  ASIO_CHECK(ex_no_props_9 != ex_no_props_1);
  ASIO_CHECK(ex_no_props_9 == ex_no_props_6);
  ASIO_CHECK(ex_no_props_9 == ex_no_props_7);
  ASIO_CHECK(ex_no_props_9 != ex_no_props_8);

#if defined(ASIO_HAS_MOVE)
  ex_no_props_t ex_no_props_10(std::move(ex_no_props_1));

   ASIO_CHECK(!ex_no_props_10);
  ASIO_CHECK(ex_no_props_10 == null_ptr);
   ASIO_CHECK(!ex_no_props_1);
  ASIO_CHECK(ex_no_props_1 == null_ptr);

  ex_no_props_t ex_no_props_11(std::move(ex_no_props_3));

   ASIO_CHECK(!!ex_no_props_11);
  ASIO_CHECK(ex_no_props_11 != null_ptr);
   ASIO_CHECK(!ex_no_props_3);
  ASIO_CHECK(ex_no_props_3 == null_ptr);
  ASIO_CHECK(ex_no_props_11 == ex_no_props_5);

  ex_no_props_t ex_no_props_12(std::move(ex_no_props_7));

   ASIO_CHECK(!!ex_no_props_12);
  ASIO_CHECK(ex_no_props_12 != null_ptr);
   ASIO_CHECK(!ex_no_props_7);
  ASIO_CHECK(ex_no_props_7 == null_ptr);
  ASIO_CHECK(ex_no_props_12 == ex_no_props_6);
  ASIO_CHECK(ex_no_props_12 != ex_no_props_8);
#endif // defined(ASIO_HAS_MOVE)

  ex_no_props_t ex_no_props_13(ex_two_props_1);

   ASIO_CHECK(!ex_no_props_13);
  ASIO_CHECK(ex_no_props_13 == null_ptr);

  ex_no_props_t ex_no_props_14(ex_two_props_5);

   ASIO_CHECK(!!ex_no_props_14);
  ASIO_CHECK(ex_no_props_14 != null_ptr);

  ex_no_props_t ex_no_props_15(ex_two_props_9);

   ASIO_CHECK(!!ex_no_props_15);
  ASIO_CHECK(ex_no_props_15 != null_ptr);

  ex_no_props_t ex_no_props_16(ex_one_prop_1);

   ASIO_CHECK(!ex_no_props_16);
  ASIO_CHECK(ex_no_props_16 == null_ptr);

  ex_no_props_t ex_no_props_17(ex_one_prop_5);

   ASIO_CHECK(!!ex_no_props_17);
  ASIO_CHECK(ex_no_props_17 != null_ptr);

  ex_no_props_t ex_no_props_18(ex_one_prop_9);

   ASIO_CHECK(!!ex_no_props_18);
  ASIO_CHECK(ex_no_props_18 != null_ptr);
}

void any_ref_assignment_test()
{
  typedef tag_invokes::any_ref<> ex_no_props_t;

  typedef tag_invokes::any_ref<
      typename execution::get_blocking_o<>::type
    > ex_one_prop_t;

  typedef tag_invokes::any_ref<
      typename execution::get_blocking_o<>::type,
      typename execution::get_occupancy_o<>::type
    > ex_two_props_t;

  thread_pool pool(1);
  asio::nullptr_t null_ptr = asio::nullptr_t();

  ex_two_props_t ex_two_props_1;

  ex_two_props_t ex_two_props_2;
  ex_two_props_2 = null_ptr;

  ASIO_CHECK(!ex_two_props_2);

  ex_two_props_t ex_two_props_3;
  ex_two_props_3 = pool.executor();

  ASIO_CHECK(!!ex_two_props_3);

  ex_two_props_t ex_two_props_4;
  ex_two_props_4 = ex_two_props_1;

  ASIO_CHECK(!ex_two_props_4);
  ASIO_CHECK(ex_two_props_4 == ex_two_props_1);

  ex_two_props_4 = ex_two_props_3;

  ASIO_CHECK(!!ex_two_props_4);
  ASIO_CHECK(ex_two_props_4 == ex_two_props_3);

  ex_two_props_t ex_two_props_5;
  ex_two_props_5 = fat_executor(1);

  ASIO_CHECK(!!ex_two_props_5);
  ASIO_CHECK(ex_two_props_5 != null_ptr);
  ASIO_CHECK(ex_two_props_5 != ex_two_props_1);

  ex_two_props_t ex_two_props_6;
  ex_two_props_6 = fat_executor(1);

  ASIO_CHECK(!!ex_two_props_6);
  ASIO_CHECK(ex_two_props_6 != null_ptr);
  ASIO_CHECK(ex_two_props_6 != ex_two_props_1);
  ASIO_CHECK(ex_two_props_6 == ex_two_props_5);

  ex_two_props_6 = fat_executor(2);

  ASIO_CHECK(!!ex_two_props_6);
  ASIO_CHECK(ex_two_props_6 != null_ptr);
  ASIO_CHECK(ex_two_props_6 != ex_two_props_1);
  ASIO_CHECK(ex_two_props_6 != ex_two_props_5);

  ex_two_props_t ex_two_props_7;
  ex_two_props_7 = ex_two_props_5;

  ASIO_CHECK(!!ex_two_props_7);
  ASIO_CHECK(ex_two_props_7 != null_ptr);
  ASIO_CHECK(ex_two_props_7 != ex_two_props_1);
  ASIO_CHECK(ex_two_props_7 == ex_two_props_5);
  ASIO_CHECK(ex_two_props_7 != ex_two_props_6);

#if defined(ASIO_HAS_MOVE)
  ex_two_props_t ex_two_props_8;
  ex_two_props_8 = std::move(ex_two_props_1);

  ASIO_CHECK(!ex_two_props_8);
  ASIO_CHECK(!ex_two_props_1);

  ex_two_props_8 = std::move(ex_two_props_3);

  ASIO_CHECK(!!ex_two_props_8);
  ASIO_CHECK(!ex_two_props_3);
  ASIO_CHECK(ex_two_props_8 == ex_two_props_4);

  ex_two_props_8 = std::move(ex_two_props_5);

  ASIO_CHECK(!!ex_two_props_8);
  ASIO_CHECK(!ex_two_props_5);
  ASIO_CHECK(ex_two_props_8 == ex_two_props_7);
#endif // defined(ASIO_HAS_MOVE)

  ex_one_prop_t ex_one_prop_1;

  ex_one_prop_t ex_one_prop_2;
  ex_one_prop_2 = null_ptr;

  ASIO_CHECK(!ex_one_prop_2);

  ex_one_prop_t ex_one_prop_3;
  ex_one_prop_3 = pool.executor();

  ASIO_CHECK(!!ex_one_prop_3);

  ex_one_prop_t ex_one_prop_4;
  ex_one_prop_4 = ex_one_prop_1;

  ASIO_CHECK(!ex_one_prop_4);
  ASIO_CHECK(ex_one_prop_4 == ex_one_prop_1);

  ex_one_prop_4 = ex_one_prop_3;

  ASIO_CHECK(!!ex_one_prop_4);
  ASIO_CHECK(ex_one_prop_4 == ex_one_prop_3);

  ex_one_prop_t ex_one_prop_5;
  ex_one_prop_5 = fat_executor(1);

  ASIO_CHECK(!!ex_one_prop_5);
  ASIO_CHECK(ex_one_prop_5 != null_ptr);
  ASIO_CHECK(ex_one_prop_5 != ex_one_prop_1);

  ex_one_prop_t ex_one_prop_6;
  ex_one_prop_6 = fat_executor(1);

  ASIO_CHECK(!!ex_one_prop_6);
  ASIO_CHECK(ex_one_prop_6 != null_ptr);
  ASIO_CHECK(ex_one_prop_6 != ex_one_prop_1);
  ASIO_CHECK(ex_one_prop_6 == ex_one_prop_5);

  ex_one_prop_6 = fat_executor(2);

  ASIO_CHECK(!!ex_one_prop_6);
  ASIO_CHECK(ex_one_prop_6 != null_ptr);
  ASIO_CHECK(ex_one_prop_6 != ex_one_prop_1);
  ASIO_CHECK(ex_one_prop_6 != ex_one_prop_5);

  ex_one_prop_t ex_one_prop_7;
  ex_one_prop_7 = ex_one_prop_5;

  ASIO_CHECK(!!ex_one_prop_7);
  ASIO_CHECK(ex_one_prop_7 != null_ptr);
  ASIO_CHECK(ex_one_prop_7 != ex_one_prop_1);
  ASIO_CHECK(ex_one_prop_7 == ex_one_prop_5);
  ASIO_CHECK(ex_one_prop_7 != ex_one_prop_6);

#if defined(ASIO_HAS_MOVE)
  ex_one_prop_t ex_one_prop_8;
  ex_one_prop_8 = std::move(ex_one_prop_1);

  ASIO_CHECK(!ex_one_prop_8);
  ASIO_CHECK(!ex_one_prop_1);

  ex_one_prop_8 = std::move(ex_one_prop_3);

  ASIO_CHECK(!!ex_one_prop_8);
  ASIO_CHECK(!ex_one_prop_3);
  ASIO_CHECK(ex_one_prop_8 == ex_one_prop_4);

  ex_one_prop_8 = std::move(ex_one_prop_5);

  ASIO_CHECK(!!ex_one_prop_8);
  ASIO_CHECK(!ex_one_prop_5);
  ASIO_CHECK(ex_one_prop_8 == ex_one_prop_7);
#endif // defined(ASIO_HAS_MOVE)

  ex_one_prop_t ex_one_prop_9;
  ex_one_prop_9 = ex_two_props_1;

  ASIO_CHECK(!ex_one_prop_9);

  ex_one_prop_9 = ex_two_props_4;

  ASIO_CHECK(!!ex_one_prop_9);

  ex_one_prop_9 = ex_two_props_7;

  ASIO_CHECK(!!ex_one_prop_9);

  ex_no_props_t ex_no_props_1;

  ex_no_props_t ex_no_props_2;
  ex_no_props_2 = null_ptr;

   ASIO_CHECK(!ex_no_props_2);

  ex_no_props_t ex_no_props_3;
  ex_no_props_3 = pool.executor();

   ASIO_CHECK(!!ex_no_props_3);

  ex_no_props_t ex_no_props_4;
  ex_no_props_4 = ex_no_props_1;

   ASIO_CHECK(!ex_no_props_4);
  ASIO_CHECK(ex_no_props_4 == ex_no_props_1);

  ex_no_props_4 = ex_no_props_3;

   ASIO_CHECK(!!ex_no_props_4);
  ASIO_CHECK(ex_no_props_4 == ex_no_props_3);

  ex_no_props_t ex_no_props_5;
  ex_no_props_5 = fat_executor(1);

   ASIO_CHECK(!!ex_no_props_5);
  ASIO_CHECK(ex_no_props_5 != null_ptr);
  ASIO_CHECK(ex_no_props_5 != ex_no_props_1);

  ex_no_props_t ex_no_props_6;
  ex_no_props_6 = fat_executor(1);

   ASIO_CHECK(!!ex_no_props_6);
  ASIO_CHECK(ex_no_props_6 != null_ptr);
  ASIO_CHECK(ex_no_props_6 != ex_no_props_1);
  ASIO_CHECK(ex_no_props_6 == ex_no_props_5);

  ex_no_props_6 = fat_executor(2);

   ASIO_CHECK(!!ex_no_props_6);
  ASIO_CHECK(ex_no_props_6 != null_ptr);
  ASIO_CHECK(ex_no_props_6 != ex_no_props_1);
  ASIO_CHECK(ex_no_props_6 != ex_no_props_5);

  ex_no_props_t ex_no_props_7;
  ex_no_props_7 = ex_no_props_5;

   ASIO_CHECK(!!ex_no_props_7);
  ASIO_CHECK(ex_no_props_7 != null_ptr);
  ASIO_CHECK(ex_no_props_7 != ex_no_props_1);
  ASIO_CHECK(ex_no_props_7 == ex_no_props_5);
  ASIO_CHECK(ex_no_props_7 != ex_no_props_6);

#if defined(ASIO_HAS_MOVE)
  ex_no_props_t ex_no_props_8;
  ex_no_props_8 = std::move(ex_no_props_1);

   ASIO_CHECK(!ex_no_props_8);
   ASIO_CHECK(!ex_no_props_1);

  ex_no_props_8 = std::move(ex_no_props_3);

   ASIO_CHECK(!!ex_no_props_8);
   ASIO_CHECK(!ex_no_props_3);
  ASIO_CHECK(ex_no_props_8 == ex_no_props_4);

  ex_no_props_8 = std::move(ex_no_props_5);

   ASIO_CHECK(!!ex_no_props_8);
   ASIO_CHECK(!ex_no_props_5);
  ASIO_CHECK(ex_no_props_8 == ex_no_props_7);
#endif // defined(ASIO_HAS_MOVE)

  ex_no_props_t ex_no_props_9;
  ex_no_props_9 = ex_two_props_1;

   ASIO_CHECK(!ex_no_props_9);

  ex_no_props_9 = ex_two_props_4;

   ASIO_CHECK(!!ex_no_props_9);

  ex_no_props_9 = ex_two_props_7;

   ASIO_CHECK(!!ex_no_props_9);

  ex_no_props_9 = ex_one_prop_1;

   ASIO_CHECK(!ex_no_props_9);

  ex_no_props_9 = ex_one_prop_4;

   ASIO_CHECK(!!ex_no_props_9);

  ex_no_props_9 = ex_one_prop_7;

   ASIO_CHECK(!!ex_no_props_9);
}

void any_ref_swap_test()
{
  typedef tag_invokes::any_ref<> ex_no_props_t;

  typedef tag_invokes::any_ref<
      typename execution::get_blocking_o<>::type
    > ex_one_prop_t;

  typedef tag_invokes::any_ref<
      typename execution::get_blocking_o<>::type,
      typename execution::get_occupancy_o<>::type
    > ex_two_props_t;

  thread_pool pool1(1);
  thread_pool pool2(1);

  ex_no_props_t ex_no_props_1(pool1.executor());
  ex_no_props_t ex_no_props_2(pool2.executor());

  ex_no_props_t ex_no_props_3(ex_no_props_1);
  ex_no_props_t ex_no_props_4(ex_no_props_2);

  ASIO_CHECK(ex_no_props_3 == ex_no_props_1);
  ASIO_CHECK(ex_no_props_4 == ex_no_props_2);

  ex_no_props_3.swap(ex_no_props_4);

  ASIO_CHECK(ex_no_props_3 == ex_no_props_2);
  ASIO_CHECK(ex_no_props_4 == ex_no_props_1);

  tag_invokes::swap(ex_no_props_3, ex_no_props_4);

  ASIO_CHECK(ex_no_props_3 == ex_no_props_1);
  ASIO_CHECK(ex_no_props_4 == ex_no_props_2);

  ex_one_prop_t ex_one_prop_1(pool1.executor());
  ex_one_prop_t ex_one_prop_2(pool2.executor());

  ex_one_prop_t ex_one_prop_3(ex_one_prop_1);
  ex_one_prop_t ex_one_prop_4(ex_one_prop_2);

  ASIO_CHECK(ex_one_prop_3 == ex_one_prop_1);
  ASIO_CHECK(ex_one_prop_4 == ex_one_prop_2);

  ex_one_prop_3.swap(ex_one_prop_4);

  ASIO_CHECK(ex_one_prop_3 == ex_one_prop_2);
  ASIO_CHECK(ex_one_prop_4 == ex_one_prop_1);

  tag_invokes::swap(ex_one_prop_3, ex_one_prop_4);

  ASIO_CHECK(ex_one_prop_3 == ex_one_prop_1);
  ASIO_CHECK(ex_one_prop_4 == ex_one_prop_2);

  ex_two_props_t ex_two_props_1(pool1.executor());
  ex_two_props_t ex_two_props_2(pool2.executor());

  ex_two_props_t ex_two_props_3(ex_two_props_1);
  ex_two_props_t ex_two_props_4(ex_two_props_2);

  ASIO_CHECK(ex_two_props_3 == ex_two_props_1);
  ASIO_CHECK(ex_two_props_4 == ex_two_props_2);

  ex_two_props_3.swap(ex_two_props_4);

  ASIO_CHECK(ex_two_props_3 == ex_two_props_2);
  ASIO_CHECK(ex_two_props_4 == ex_two_props_1);

  tag_invokes::swap(ex_two_props_3, ex_two_props_4);

  ASIO_CHECK(ex_two_props_3 == ex_two_props_1);
  ASIO_CHECK(ex_two_props_4 == ex_two_props_2);
}

void any_ref_query_test()
{
  thread_pool pool(1);
  tag_invokes::any_ref<
      typename execution::execute_o<>::type,
#if defined(ASIO_HAS_VARIABLE_TEMPLATES)
      typename execution::get_context_as_o<thread_pool&>::type,
#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)
      typename execution::get_allocator_o<std::allocator<void>>::type,
      typename execution::get_blocking_o<>::type,
      typename execution::get_mapping_o<>::type,
      typename execution::get_relationship_o<>::type,
      typename execution::get_outstanding_work_o<>::type,
      typename execution::get_occupancy_o<>::type>
    ex(pool.executor());

  ASIO_CHECK(
      execution::get_occupancy(ex)
        == 1);

  ASIO_CHECK(
      execution::get_allocator(ex)
        == std::allocator<void>{});

  static_assert(tag_invokes::static_tag_invoke_v<decltype(execution::get_blocking), decltype(execution::make_with_blocking(pool.executor(), execution::always_blocking))>
        == execution::always_blocking);

  static_assert(!tag_invokes::can_static_tag_invoke_v<decltype(execution::get_blocking), decltype(pool.executor())>);

  ASIO_CHECK(
      execution::get_blocking(ex)
        == execution::possibly_blocking);

  static_assert(tag_invokes::static_tag_invoke_v<decltype(execution::get_mapping), decltype(pool.executor())>
        == execution::thread_mapping);

  ASIO_CHECK(
      execution::get_mapping(ex)
        == execution::thread_mapping);

  ASIO_CHECK(
      execution::get_relationship(ex)
        == execution::fork_relationship);

  static_assert(tag_invokes::static_tag_invoke_v<decltype(execution::get_outstanding_work), decltype(pool.executor())>
        == execution::untracked_outstanding_work);

  ASIO_CHECK(
      execution::get_outstanding_work(ex)
        == execution::untracked_outstanding_work);

#if defined(ASIO_HAS_VARIABLE_TEMPLATES)

  ex = execution::get_context_as<thread_pool&>(ex).executor();

#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

}

void any_ref_set_test()
{
  thread_pool pool(1);
  tag_invokes::any_ref<
      typename execution::execute_o<>::type,
      typename execution::get_blocking_o<>::type,
      typename execution::make_with_blocking_o<
        tag_invokes::any_ref<
          typename execution::execute_o<>::type,
          typename execution::get_blocking_o<>::type>,
        decltype(execution::always_blocking)
      >::type,
      typename execution::get_relationship_o<>::type,
      typename execution::make_with_relationship_o<
        tag_invokes::any_ref<
          typename execution::execute_o<>::type,
          typename execution::get_relationship_o<>::type>,
        decltype(execution::continuation_relationship)
      >::type,
      typename execution::get_outstanding_work_o<>::type,
      typename execution::make_with_outstanding_work_o<
        tag_invokes::any_ref<
          typename execution::execute_o<>::type,
          typename execution::get_outstanding_work_o<>::type>,
        decltype(execution::tracked_outstanding_work)
      >::type,
      typename execution::get_allocator_o<std::allocator<void>>::type,
      typename execution::make_with_allocator_o<
        tag_invokes::any_ref<
          typename execution::execute_o<>::type,
          typename execution::get_allocator_o<std::allocator<char>>::type>, 
        std::allocator<char>>::type>
    ex(pool.executor());

  ASIO_CHECK(
      execution::make_with_allocator(ex, std::allocator<char>{})
        == execution::make_with_allocator(pool.executor(), std::allocator<char>{}));

  ASIO_CHECK(
      execution::make_with_blocking(ex, execution::always_blocking)
        == execution::make_with_blocking(pool.executor(), execution::always_blocking));

  ASIO_CHECK(
      execution::get_blocking(execution::make_with_blocking(ex, execution::always_blocking))
        == execution::always_blocking);

  ASIO_CHECK(
      execution::make_with_relationship(ex, execution::continuation_relationship)
        == execution::make_with_relationship(pool.executor(), execution::continuation_relationship));

  ASIO_CHECK(
      execution::get_relationship(execution::make_with_relationship(ex, execution::continuation_relationship))
        == execution::continuation_relationship);

  ASIO_CHECK(
      execution::make_with_outstanding_work(ex, execution::tracked_outstanding_work)
        == execution::make_with_outstanding_work(pool.executor(), execution::tracked_outstanding_work));

  ASIO_CHECK(
      execution::get_outstanding_work(execution::make_with_outstanding_work(ex, execution::tracked_outstanding_work))
        == execution::tracked_outstanding_work);

}

void any_ref_execute_test()
{
  int count = 0;
  thread_pool pool(1);
  asio::tag_invokes::any_ref<
    asio::tag_invokes::overload<
      decltype(asio::execution::execute), 
      void(const asio::tag_invokes::target_&, asio::execution::invocable_archetype)>,
    asio::tag_invokes::overload<
      decltype(asio::execution::execute), 
      void(asio::tag_invokes::target_&, std::function<void()>)>,
    asio::tag_invokes::overload<
      decltype(asio::execution::execute), 
      void(const asio::tag_invokes::target_&, std::function<void()>)>>
    ex;

  ex = pool.executor();

  asio::execution::execute(ex,
      bindns::bind(increment, &count));

  pool.wait();
  ASIO_CHECK(count == 1);
}

void any_ref_receiver_test()
{
  int count = 0;
  asio::tag_invokes::any_ref<
    asio::tag_invokes::overload<
      decltype(asio::execution::set_value), 
      void(asio::tag_invokes::target_&&, int, long, std::string)>,
    asio::tag_invokes::overload<
      decltype(asio::execution::set_error), 
      void(asio::tag_invokes::target_&&, std::exception_ptr) noexcept>,
    asio::tag_invokes::overload<
      decltype(asio::execution::set_done), 
      void(asio::tag_invokes::target_&&) noexcept>>
    r;

  r = test_receiver(&count);
  asio::execution::set_value(std::move(r), 0, 0L, "");
  ASIO_CHECK(count == 1);

  r = test_receiver(&count);
  asio::execution::set_error(std::move(r), nullptr);
  ASIO_CHECK(count == 2);

  r = test_receiver(&count);
  asio::execution::set_done(std::move(r));
  ASIO_CHECK(count == 3);
}

ASIO_TEST_SUITE
(
  "any_ref",
  ASIO_TEST_CASE(any_ref_construction_test)
  ASIO_TEST_CASE(any_ref_assignment_test)
  ASIO_TEST_CASE(any_ref_swap_test)
  ASIO_TEST_CASE(any_ref_set_test)
  ASIO_TEST_CASE(any_ref_query_test)
  ASIO_TEST_CASE(any_ref_execute_test)
  ASIO_TEST_CASE(any_ref_receiver_test)
)
