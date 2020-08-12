//
// tag_invokes/overload.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_OVERLOAD_HPP
#define ASIO_EXECUTION_OVERLOAD_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {
namespace tag_invokes {

/// overload to add function to the vtable of a polymorphic object.
template <typename Cpo, typename Sig>
struct overload : remove_cvref<Cpo>::type
{
private:
  typedef typename remove_cvref<Cpo>::type cpo_t;
public:
  /// Constructor.
  ASIO_CONSTEXPR overload() ASIO_NOEXCEPT {}
  ASIO_CONSTEXPR overload(cpo_t) ASIO_NOEXCEPT {}

  using cpo_t::operator();

  typedef Sig type_erased_signature_t;
};

template <typename Cpo>
struct base_cpo_impl
{
  typedef Cpo type;
};

template <typename Cpo, typename Sig>
struct base_cpo_impl<overload<Cpo, Sig>>
{
  typedef typename remove_cvref<Cpo>::type type;
};

template <typename Cpo>
struct base_cpo
{
  typedef typename base_cpo_impl<typename remove_cvref<Cpo>::type>::type type;
};

#if defined(ASIO_HAS_ALIAS_TEMPLATES)

template <typename Cpo>
using base_cpo_t = typename base_cpo<Cpo>::type;

#endif // defined(ASIO_HAS_ALIAS_TEMPLATES)

} // namespace tag_invokes
} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_OVERLOAD_HPP
