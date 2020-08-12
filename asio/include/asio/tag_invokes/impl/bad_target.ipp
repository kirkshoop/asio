//
// tag_invokes/impl/bad_target.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_IMPL_BAD_TARGET_IPP
#define ASIO_EXECUTION_IMPL_BAD_TARGET_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/tag_invokes/bad_target.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {
namespace tag_invokes {

bad_target::bad_target() ASIO_NOEXCEPT
{
}

const char* bad_target::what() const ASIO_NOEXCEPT_OR_NOTHROW
{
  return "bad target";
}

} // namespace execution
} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_IMPL_BAD_TARGET_IPP
