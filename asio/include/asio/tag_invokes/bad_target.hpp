//
// tag_invokes/bad_target.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_BAD_TARGET_HPP
#define ASIO_EXECUTION_BAD_TARGET_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include <exception>
#include "asio/detail/push_options.hpp"

namespace asio {
namespace tag_invokes {

/// Exception thrown when trying to access an empty polymorphic object.
class bad_target
  : public std::exception
{
public:
  /// Constructor.
  ASIO_DECL bad_target() ASIO_NOEXCEPT;

  /// Obtain message associated with exception.
  ASIO_DECL virtual const char* what() const
    ASIO_NOEXCEPT_OR_NOTHROW;
};

} // namespace tag_invokes
} // namespace asio

#include "asio/detail/pop_options.hpp"

#if defined(ASIO_HEADER_ONLY)
# include "asio/tag_invokes/impl/bad_target.ipp"
#endif // defined(ASIO_HEADER_ONLY)

#endif // ASIO_EXECUTION_BAD_TARGET_HPP
