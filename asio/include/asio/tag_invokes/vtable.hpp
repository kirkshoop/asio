//
// tag_invokes/vtable.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_TAG_INVOKES_VTABLE_HPP
#define ASIO_TAG_INVOKES_VTABLE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {
namespace tag_invokes {

struct target_ {};

template <typename CPO, typename Sig = typename CPO::type_erased_signature_t>
struct vtable_entry;

#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename CPO, typename Ret, typename TargetPlaceholder, typename... ArgN>
struct vtable_entry<CPO, Ret(TargetPlaceholder, ArgN...)> {
  typedef typename base_cpo<CPO>::type cpo_t;
  typedef typename std::conditional<std::is_const<typename std::remove_reference<TargetPlaceholder>::type>::value, const void, void>::type void_target_t;
  typedef Ret(*fn_t)(cpo_t, void_target_t*, ArgN...);

  static_assert(
    std::is_same<target_, typename remove_cvref<TargetPlaceholder>::type>::value, 
    "The polymorphic function signature is missing the target_ placeholder argument");

  ASIO_CONSTEXPR fn_t get() const ASIO_NOEXCEPT {
    return fn_;
  }

  static Ret void_entry_fn(cpo_t cpo, void_target_t* targetPointer, ArgN... argN) {
    std::terminate();
  }

  template <typename Target>
  static Ret entry_fn(cpo_t cpo, void_target_t* targetPointer, ArgN... argN) {
    Target targetRef = static_cast<Target>(*static_cast<typename remove_reference<Target>::type*>(targetPointer));
    return std::move(cpo)((Target&&)targetRef, argN...);
  }

  template <typename Target>
  static ASIO_CONSTEXPR auto create() ASIO_NOEXCEPT
    -> typename enable_if<std::is_void<Target>::value, tag_invokes::vtable_entry<CPO>>::type {
    ASIO_CONSTEXPR fn_t f = &void_entry_fn;
    return tag_invokes::vtable_entry<CPO>{f};
  }
  template <typename Target>
  static ASIO_CONSTEXPR auto create() ASIO_NOEXCEPT
    -> typename enable_if<!std::is_void<Target>::value, tag_invokes::vtable_entry<CPO>>::type {
    typedef typename remove_cvref<Target>::type target_t;
    typedef typename std::conditional<std::is_const<typename std::remove_reference<TargetPlaceholder>::type>::value, const target_t, target_t>::type c_target_t;
    typedef typename std::conditional<std::is_rvalue_reference<TargetPlaceholder>::value, c_target_t&&, c_target_t&>::type ref_target_t;
    ASIO_CONSTEXPR fn_t f = &entry_fn<ref_target_t>;
    return tag_invokes::vtable_entry<CPO>{f};
  }

  template <typename Target>
  static auto check_target_fn(nullptr_t) 
  -> typename conditional<true, true_type, 
      typename result_of<
        decltype(&vtable_entry::entry_fn<Target>)(cpo_t, void_target_t*, ArgN...)
      >::type
    >::type;
  template <typename Target>
  static false_type check_target_fn(...);

  template <typename Target>
  struct is_valid_target : std::integral_constant<bool, decltype(vtable_entry::check_target_fn<Target>(nullptr))::value> {};

 protected:

  static auto vtable_invoke(const cpo_t& cpo, const vtable_entry& self, void_target_t* targetPointer, ArgN... argN)
  {
    return self.fn_(cpo, targetPointer, argN...);
  }

 private:

  explicit ASIO_CONSTEXPR vtable_entry(fn_t fn) ASIO_NOEXCEPT
    : fn_(fn) {}

  fn_t fn_;
};

#if defined(ASIO_HAS_NOEXCEPT)
template <typename CPO, typename Ret, typename TargetPlaceholder, typename... ArgN>
struct vtable_entry<CPO, Ret(TargetPlaceholder, ArgN...) noexcept> {
  typedef typename base_cpo<CPO>::type cpo_t;
  typedef typename std::conditional<std::is_const<typename std::remove_reference<TargetPlaceholder>::type>::value, const void, void>::type void_target_t;
  typedef Ret(*fn_t)(cpo_t, void_target_t*, ArgN...) noexcept;

  static_assert(
    std::is_same<target_, typename remove_cvref<TargetPlaceholder>::type>::value, 
    "The polymorphic function signature is missing the target_ placeholder argument");

  ASIO_CONSTEXPR fn_t get() const ASIO_NOEXCEPT {
    return fn_;
  }

  static Ret void_entry_fn(cpo_t cpo, void_target_t* targetPointer, ArgN... argN) noexcept {
    std::terminate();
  }

  template <typename Target>
  static Ret entry_fn(cpo_t cpo, void_target_t* targetPointer, ArgN... argN) noexcept {
    Target targetRef = static_cast<Target>(*static_cast<typename remove_reference<Target>::type*>(targetPointer));
    return std::move(cpo)((Target&&)targetRef, argN...);
  }

  template <typename Target>
  static ASIO_CONSTEXPR auto create() ASIO_NOEXCEPT
    -> typename enable_if<std::is_void<Target>::value, tag_invokes::vtable_entry<CPO>>::type {
    ASIO_CONSTEXPR fn_t f = &void_entry_fn;
    return tag_invokes::vtable_entry<CPO>{f};
  }
  template <typename Target>
  static ASIO_CONSTEXPR auto create() ASIO_NOEXCEPT
    -> typename enable_if<!std::is_void<Target>::value, tag_invokes::vtable_entry<CPO>>::type {
    typedef typename remove_cvref<Target>::type target_t;
    typedef typename std::conditional<std::is_const<typename std::remove_reference<TargetPlaceholder>::type>::value, const target_t, target_t>::type c_target_t;
    typedef typename std::conditional<std::is_rvalue_reference<TargetPlaceholder>::value, c_target_t&&, c_target_t&>::type ref_target_t;
    ASIO_CONSTEXPR fn_t f = &entry_fn<ref_target_t>;
    return tag_invokes::vtable_entry<CPO>{f};
  }

  template <typename Target>
  static auto check_target_fn(nullptr_t) 
  -> typename conditional<true, true_type, 
      typename result_of<
        decltype(&vtable_entry::entry_fn<Target>)(cpo_t, void_target_t*, ArgN...)
      >::type
    >::type;
  template <typename Target>
  static false_type check_target_fn(...);

  template <typename Target>
  struct is_valid_target : std::integral_constant<bool, decltype(vtable_entry::check_target_fn<Target>(nullptr))::value> {};

 protected:

  static auto vtable_invoke(const cpo_t& cpo, const vtable_entry& self, void_target_t* targetPointer, ArgN... argN) noexcept
  {
    return self.fn_(cpo, targetPointer, argN...);
  }

 private:

  explicit ASIO_CONSTEXPR vtable_entry(fn_t fn) ASIO_NOEXCEPT
    : fn_(fn) {}

  fn_t fn_;
};
#endif // defined(ASIO_HAS_NOEXCEPT)

#else // defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename CPO, typename Ret, typename TargetPlaceholder>
struct vtable_entry<CPO, Ret(TargetPlaceholder)> {
  typedef typename base_cpo<CPO>::type cpo_t;
  typedef typename std::conditional<std::is_const<typename std::remove_reference<TargetPlaceholder>::type>::value, const void*, void*>::type void_target_t;
  typedef Ret(*fn_t)(cpo_t, void_target_t*, ArgN...);

  static_assert(
    std::is_same<target_, typename remove_cvref<TargetPlaceholder>::type>::value, 
    "The polymorphic function signature is missing the target_ placeholder argument");

  ASIO_CONSTEXPR fn_t get() const ASIO_NOEXCEPT {
    return fn_;
  }

  template <typename Target>
  static Ret entry_fn(cpo_t cpo, void* targetPointer) {
    Target& targetRef = *static_cast<Target*>(targetPointer);
    return std::move(cpo)(targetRef;
  }

  template <typename Target>
  static ASIO_CONSTEXPR tag_invokes::vtable_entry<CPO> create() ASIO_NOEXCEPT {
      ASIO_CONSTEXPR fn_t f = &entry_fn<Target>;
    return tag_invokes::vtable_entry<CPO>{f};
  }

  template <typename Target>
  static auto check_target_fn(nullptr_t) 
  -> typename conditional<true, true_type, 
      typename result_of<
        decltype(&vtable_entry::entry_fn<Target>)(cpo_t, void_target_t*)
      >::type
    >::type;
  template <typename Target>
  static false_type check_target_fn(...);

  template <typename Target>
  struct is_valid_target : std::integral_constant<bool, decltype(vtable_entry::check_target_fn<Target>(nullptr))::value> {};

 private:
  explicit ASIO_CONSTEXPR vtable_entry(fn_t fn) ASIO_NOEXCEPT
    : fn_(fn) {}

  fn_t fn_;
};

#if defined(ASIO_HAS_NOEXCEPT)
template <typename CPO, typename Ret, typename TargetPlaceholder>
struct vtable_entry<CPO, Ret(TargetPlaceholder) noexcept> {
  typedef typename base_cpo<CPO>::type cpo_t;
  typedef typename std::conditional<std::is_const<typename std::remove_reference<TargetPlaceholder>::type>::value, const void*, void*>::type void_target_t;
  typedef Ret(*fn_t)(cpo_t, void_target_t*, ArgN...) noexcept;

  static_assert(
    std::is_same<target_, typename remove_cvref<TargetPlaceholder>::type>::value, 
    "The polymorphic function signature is missing the target_ placeholder argument");

  ASIO_CONSTEXPR fn_t get() const ASIO_NOEXCEPT {
    return fn_;
  }

  template <typename Target>
  static Ret entry_fn(cpo_t cpo, void* targetPointer) noexcept {
    Target& targetRef = *static_cast<Target*>(targetPointer);
    return std::move(cpo)(targetRef);
  }

  template <typename Target>
  static ASIO_CONSTEXPR tag_invokes::vtable_entry<CPO> create() ASIO_NOEXCEPT {
      ASIO_CONSTEXPR fn_t f = &entry_fn<Target>;
    return tag_invokes::vtable_entry<CPO>{f};
  }

  template <typename Target>
  static auto check_target_fn(nullptr_t) 
  -> typename conditional<true, true_type, 
      typename result_of<
        decltype(&vtable_entry::entry_fn<Target>)(cpo_t, void_target_t*)
      >::type
    >::type;
  template <typename Target>
  static false_type check_target_fn(...);

  template <typename Target>
  struct is_valid_target : std::integral_constant<bool, decltype(vtable_entry::check_target_fn<Target>(nullptr))::value> {};

 private:
  explicit ASIO_CONSTEXPR vtable_entry(fn_t fn) ASIO_NOEXCEPT
    : fn_(fn) {}

  fn_t fn_;
};
#endif // defined(ASIO_HAS_NOEXCEPT)

#define ASIO_PRIVATE_VTABLE_ENTRY_DEF(n) \
    template <typename CPO, typename Ret, typename TargetPlaceholder, ASIO_VARIADIC_TPARAMS(n)> \
    struct vtable_entry<CPO, Ret(TargetPlaceholder, ASIO_VARIADIC_TARGS(n))> { \
    typedef typename base_cpo<CPO>::type cpo_t; \
    typedef Ret(*fn_t)(cpo_t, void*, ASIO_VARIADIC_TARGS(n)); \
    \
    static_assert( \
        std::is_same<target_, typename remove_cvref<TargetPlaceholder>::type>::value, \
        "The polymorphic function signature is missing the target_ placeholder argument"); \
    \
    ASIO_CONSTEXPR fn_t get() const ASIO_NOEXCEPT { \
        return fn_; \
    } \
    \
    template <typename Target> \
    static Ret entry_fn(cpo_t cpo, void* targetPointer, ASIO_VARIADIC_MOVE_PARAMS(n)) { \
        Target& targetRef = *static_cast<Target*>(targetPointer); \
        return std::move(cpo)(targetRef, ASIO_VARIADIC_MOVE_ARGS(n)); \
    } \
    \
    template <typename Target> \
    static ASIO_CONSTEXPR tag_invokes::vtable_entry<CPO> create() ASIO_NOEXCEPT { \
        ASIO_CONSTEXPR fn_t f = &entry_fn<Target>; \
        return tag_invokes::vtable_entry<CPO>{f}; \
    } \
    \
    private: \
    explicit ASIO_CONSTEXPR vtable_entry(fn_t fn) ASIO_NOEXCEPT \
        : fn_(fn) {} \
    \
    fn_t fn_; \
    }; \
  /**/
ASIO_VARIADIC_GENERATE(ASIO_PRIVATE_VTABLE_ENTRY_DEF)
#undef ASIO_PRIVATE_VTABLE_ENTRY_DEF

#if defined(ASIO_HAS_NOEXCEPT)
#define ASIO_PRIVATE_VTABLE_ENTRY_NOEXCEPT_DEF(n) \
    template <typename CPO, typename Ret, typename TargetPlaceholder, ASIO_VARIADIC_TPARAMS(n)> \
    struct vtable_entry<CPO, Ret(TargetPlaceholder, ASIO_VARIADIC_TARGS(n)) noexcept> { \
    typedef typename base_cpo<CPO>::type cpo_t; \
    typedef Ret(*fn_t)(cpo_t, void*, ASIO_VARIADIC_TARGS(n)) noexcept; \
    \
    static_assert( \
        std::is_same<target_, typename remove_cvref<TargetPlaceholder>::type>::value, \
        "The polymorphic function signature is missing the target_ \
        placeholder argument"); \
    \
    ASIO_CONSTEXPR fn_t get() const ASIO_NOEXCEPT { \
        return fn_; \
    } \
    \
    template <typename Target> \
    static Ret entry_fn(cpo_t cpo, void* targetPointer, ASIO_VARIADIC_MOVE_PARAMS(n)) noexcept { \
        Target& targetRef = *static_cast<Target*>(targetPointer); \
        return std::move(cpo)(targetRef, ASIO_VARIADIC_MOVE_ARGS(n)); \
    } \
    \
    template <typename Target> \
    static ASIO_CONSTEXPR tag_invokes::vtable_entry<CPO> create() ASIO_NOEXCEPT { \
        ASIO_CONSTEXPR fn_t f = &entry_fn<Target>; \
        return tag_invokes::vtable_entry<CPO>{f}; \
    } \
    \
    private: \
    explicit ASIO_CONSTEXPR vtable_entry(fn_t fn) ASIO_NOEXCEPT \
        : fn_(fn) {} \
    \
    fn_t fn_; \
    }; \
  /**/
ASIO_VARIADIC_GENERATE(ASIO_PRIVATE_VTABLE_ENTRY_NOEXCEPT_DEF)
#undef ASIO_PRIVATE_VTABLE_ENTRY_NOEXCEPT_DEF

#endif // defined(ASIO_HAS_NOEXCEPT)

#endif // defined(ASIO_HAS_VARIADIC_TEMPLATES)

#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename... CPOs>
struct vtable : vtable_entry<typename remove_cvref<CPOs>::type>... {
  template <typename T>
  static constexpr vtable create() noexcept {
    return vtable{vtable_entry<typename remove_cvref<CPOs>::type>::template create<T>()...};
  }

  template <typename CPO>
  constexpr auto get() const noexcept -> typename vtable_entry<typename remove_cvref<CPO>::type>::fn_t {
    const vtable_entry<typename remove_cvref<CPO>::type>& entry = *this;
    return entry.get();
  }

  using vtable_entry<typename remove_cvref<CPOs>::type>::vtable_invoke...;

 private:
  explicit constexpr vtable(vtable_entry<typename remove_cvref<CPOs>::type>... entries) noexcept
    : vtable_entry<typename remove_cvref<CPOs>::type>{entries}... {}
};

#else // defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename = void,
    typename = void, typename = void, typename = void, typename = void,
    typename = void, typename = void, typename = void, typename = void>
struct vtable {
  template <typename T>
  static constexpr vtable create() noexcept {
    return vtable{};
  }

 private:
  explicit constexpr vtable() noexcept {}
};

#define ASIO_PRIVATE_VTABLE_ENTRY_BASE_DEF(n) \
  private vtable_entry<T##n>

#define ASIO_PRIVATE_VTABLE_ENTRY_CREATE(n) \
  vtable_entry<T##n>::template create<T>()

#define ASIO_PRIVATE_VTABLE_ENTRY_PARAM(n) \
  vtable_entry<T##n> e##n

#define ASIO_PRIVATE_VTABLE_ENTRY_BASE_INIT(n) \
  vtable_entry<T##n>(e##n)

#define ASIO_PRIVATE_VTABLE_DEF(n) \
  template <typename R, ASIO_VARIADIC_TPARAMS(n)> \
  struct vtable : ASIO_VARIADIC_APPLY(ASIO_PRIVATE_VTABLE_ENTRY_BASE_DEF, n) { \
    template <typename T> \
    static constexpr vtable create() noexcept { \
        return vtable{ASIO_VARIADIC_APPLY(ASIO_PRIVATE_VTABLE_ENTRY_CREATE, n)}; \
    } \
   \
    template <typename CPO> \
    constexpr auto get() const noexcept -> typename vtable_entry<CPO>::fn_t { \
        const vtable_entry<CPO>& entry = *this; \
        return entry.get(); \
    } \
   \
  private: \
    explicit constexpr vtable(ASIO_VARIADIC_APPLY(ASIO_PRIVATE_VTABLE_ENTRY_PARAM, n)) noexcept \
        : ASIO_VARIADIC_APPLY(ASIO_PRIVATE_VTABLE_ENTRY_BASE_INIT, n) {} \
  }; 
  /**/
ASIO_VARIADIC_GENERATE(ASIO_PRIVATE_VTABLE_DEF)
#undef ASIO_PRIVATE_VTABLE_DEF
#undef ASIO_PRIVATE_VTABLE_ENTRY_BASE_DEF
#undef ASIO_PRIVATE_VTABLE_ENTRY_CREATE
#undef ASIO_PRIVATE_VTABLE_ENTRY_PARAM
#undef ASIO_PRIVATE_VTABLE_ENTRY_BASE_INIT

#endif // defined(ASIO_HAS_VARIADIC_TEMPLATES)

} // namespace tag_invokes
} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_TAG_INVOKES_VTABLE_HPP
