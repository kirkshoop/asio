//
// tag_invokes/any_ref.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_ANY_REF_HPP
#define ASIO_EXECUTION_ANY_REF_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include <new>
#include <typeinfo>
#include "asio/detail/assert.hpp"
#include "asio/detail/cstddef.hpp"
#include "asio/detail/memory.hpp"
#include "asio/detail/non_const_lvalue.hpp"
#include "asio/detail/scoped_ptr.hpp"
#include "asio/detail/type_traits.hpp"
#include "asio/detail/throw_exception.hpp"
#include "asio/detail/variadic_templates.hpp"
#include "asio/tag_invokes/bad_target.hpp"
#include "asio/tag_invokes/tag_invoke.hpp"
#include "asio/tag_invokes/any_base.hpp"
#include "asio/traits/equality_comparable.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {

#if defined(GENERATING_DOCUMENTATION)

namespace tag_invokes {

/// Polymorphic wrapper.
template <typename... Cpos>
class any_ref
{
public:
  /// Default constructor.
  any_ref() noexcept;

  /// Construct in an empty state. Equivalent effects to default constructor.
  any_ref(nullptr_t) noexcept;

  /// Copy constructor.
  any_ref(const any_ref& e) noexcept;

  /// Move constructor.
  any_ref(any_ref&& e) noexcept;

  /// Construct to point to the same target as another any_ref.
  template <class... OtherCpos>
    any_ref(any_ref<OtherCpos...> e);

  /// Construct a polymorphic wrapper for the specified target.
  template <typename Target>
  any_ref(Target e);

  /// Assignment operator.
  any_ref& operator=(const any_ref& e) noexcept;

  /// Move assignment operator.
  any_ref& operator=(any_ref&& e) noexcept;

  /// Assignment operator that sets the polymorphic wrapper to the empty state.
  any_ref& operator=(nullptr_t);

  /// Assignment operator to create a polymorphic wrapper for the specified
  /// target.
  template <typename Target>
  any_ref& operator=(Target e);

  /// Destructor.
  ~any_ref();

  /// Swap targets with another polymorphic wrapper.
  void swap(any_ref& other) noexcept;

  /// Obtain the value associated with the specified cpo.
  /**
   * Do not call this function directly. It is intended for use with the
   * asio::tag_invokes::tag_invoke customisation point.
   *
   * For example:
   * @code tag_invokes::any_ref<tag_invokes::overload<execution::execute, void (std::function<void()>)>> ex = ...;
   * tag_invokes::tag_invoke(execution::execute, target, [](){}); @endcode
   */
  template <typename Cpo, typename... ArgN>
  friend typename asio::tag_invokes::tag_invoke_result<Cpo, any_ref, ArgN...>::type tag_invoke(Cpo, any_ref, ArgN...);


  /// Determine whether the wrapper has a target.
  /**
   * @returns @c true if the polymorphic wrapper has a target,
   * otherwise false.
   */
  explicit operator bool() const noexcept;
};

/// Equality operator.
/**
 * @relates any_ref
 */
template <typename... Cpos>
bool operator==(const any_ref<Cpos...>& a,
    const any_ref<Cpos...>& b) noexcept;

/// Equality operator.
/**
 * @relates any_ref
 */
template <typename... Cpos>
bool operator==(const any_ref<Cpos...>& a,
    nullptr_t) noexcept;

/// Equality operator.
/**
 * @relates any_ref
 */
template <typename... Cpos>
bool operator==(nullptr_t,
    const any_ref<Cpos...>& b) noexcept;

/// Inequality operator.
/**
 * @relates any_ref
 */
template <typename... Cpos>
bool operator!=(const any_ref<Cpos...>& a,
    const any_ref<Cpos...>& b) noexcept;

/// Inequality operator.
/**
 * @relates any_ref
 */
template <typename... Cpos>
bool operator!=(const any_ref<Cpos...>& a,
    nullptr_t) noexcept;

/// Inequality operator.
/**
 * @relates any_ref
 */
template <typename... Cpos>
bool operator!=(nullptr_t,
    const any_ref<Cpos...>& b) noexcept;

} // namespace tag_invokes

#else // defined(GENERATING_DOCUMENTATION)

namespace tag_invokes {

#if !defined(ASIO_EXECUTION_ANY_REF_FWD_DECL)
#define ASIO_EXECUTION_ANY_REF_FWD_DECL

#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename... Cpos>
class any_ref;

#else // defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename = void, typename = void, typename = void,
    typename = void, typename = void, typename = void,
    typename = void, typename = void, typename = void>
class any_ref;

#endif // defined(ASIO_HAS_VARIADIC_TEMPLATES)

#endif // !defined(ASIO_EXECUTION_ANY_REF_FWD_DECL)

namespace detail {

// Helper template to:
// - Check if a target can supply the required overloads.
// - Find the first convertible-from-T cpo in the list.

template <std::size_t I, typename Cpos>
struct can_tag_invoke;

template <std::size_t I, typename Cpo>
struct can_tag_invoke<I, void(Cpo)>
{
  template <typename T>
  struct is_valid_target : integral_constant<bool,
      asio::tag_invokes::vtable_entry<Cpo>::template is_valid_target<T>::value
    >
  {
  };

  struct found
  {
    ASIO_STATIC_CONSTEXPR(bool, value = true);
    typedef Cpo type;
    // typedef typename asio::tag_invokes::tag_invoke_result<Cpo>::type tag_invoke_result_type;
    ASIO_STATIC_CONSTEXPR(std::size_t, index = I);
  };

  struct not_found
  {
    ASIO_STATIC_CONSTEXPR(bool, value = false);
  };

  template <typename T>
  struct find_convertible_cpo :
      conditional<
        is_same<T, Cpo>::value || is_convertible<T, Cpo>::value,
        found,
        not_found
      >::type {};
};

#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <std::size_t I, typename Head, typename... Tail>
struct can_tag_invoke<I, void(Head, Tail...)>
{
  template <typename T>
  struct is_valid_target : integral_constant<bool,
      (
        can_tag_invoke<I,
          void(Head)>::template is_valid_target<T>::value
        &&
        can_tag_invoke<I + 1,
          void(Tail...)>::template is_valid_target<T>::value
      )
    >
  {
  };

  template <typename T>
  struct find_convertible_cpo :
      conditional<
        is_convertible<T, Head>::value,
        typename can_tag_invoke<I, void(Head)>::found,
        typename can_tag_invoke<I + 1,
            void(Tail...)>::template find_convertible_cpo<T>
      >::type {};
};

#else // defined(ASIO_HAS_VARIADIC_TEMPLATES)

#define ASIO_PRIVATE_ANY_REF_PROPS_BASE_DEF(n) \
  template <std::size_t I, \
    typename Head, ASIO_VARIADIC_TPARAMS(n)> \
  struct can_tag_invoke<I, \
      void(Head, ASIO_VARIADIC_TARGS(n))> \
  { \
    template <typename T> \
    struct is_valid_target : integral_constant<bool, \
        ( \
          can_tag_invoke<I, \
            void(Head)>::template is_valid_target<T>::value \
          && \
          can_tag_invoke<I + 1, \
              void(ASIO_VARIADIC_TARGS(n))>::template \
                is_valid_target<T>::value \
        ) \
      > \
    { \
    }; \
  \
    template <typename T> \
    struct find_convertible_cpo : \
        conditional< \
          is_convertible<T, Head>::value, \
          typename can_tag_invoke<I, void(Head)>::found, \
          typename can_tag_invoke<I + 1, \
              void(ASIO_VARIADIC_TARGS(n))>::template \
                find_convertible_cpo<T> \
        >::type {}; \
  }; \
  /**/
ASIO_VARIADIC_GENERATE(ASIO_PRIVATE_ANY_REF_PROPS_BASE_DEF)
#undef ASIO_PRIVATE_ANY_REF_PROPS_BASE_DEF

#endif // defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename T, typename Cpos>
struct is_valid_target :
    can_tag_invoke<0, Cpos>::template is_valid_target<T>
{
};

class any_ref_base
{
public:
  any_ref_base() ASIO_NOEXCEPT
    : object_fns_(object_fns_table<void>()),
      target_(0),
      target_fns_(target_fns_table<void>())
  {
  }

  template <typename Target>
  any_ref_base(Target target, false_type)
    : target_fns_(target_fns_table<Target>())
  {
    any_ref_base::construct_object(target,
        integral_constant<bool,
          sizeof(Target) <= sizeof(object_type)
            && alignment_of<Target>::value <= alignment_of<object_type>::value
        >());
  }
  
  template <typename Target>
  any_ref_base(Target other, true_type)
    : object_fns_(object_fns_table<asio::detail::shared_ptr<void> >()),
      target_fns_(other.target_fns_)
  {
    asio::detail::shared_ptr<Target> p =
      asio::detail::make_shared<Target>(
          ASIO_MOVE_CAST(Target)(other));
    target_ = p->template target<void>();
    new (&object_) asio::detail::shared_ptr<void>(
        ASIO_MOVE_CAST(asio::detail::shared_ptr<Target>)(p));
  }

  any_ref_base(const any_ref_base& other) ASIO_NOEXCEPT
    : object_fns_(other.object_fns_),
      target_fns_(other.target_fns_)
  {
    object_fns_->copy(*this, other);
  }

  ~any_ref_base() ASIO_NOEXCEPT
  {
    object_fns_->destroy(*this);
  }

  any_ref_base& operator=(
      const any_ref_base& other) ASIO_NOEXCEPT
  {
    if (this != &other)
    {
      object_fns_->destroy(*this);
      object_fns_ = other.object_fns_;
      target_fns_ = other.target_fns_;
      object_fns_->copy(*this, other);
    }
    return *this;
  }

  any_ref_base& operator=(nullptr_t) ASIO_NOEXCEPT
  {
    object_fns_->destroy(*this);
    target_ = 0;
    object_fns_ = object_fns_table<void>();
    target_fns_ = target_fns_table<void>();
    return *this;
  }

#if defined(ASIO_HAS_MOVE)

  any_ref_base(any_ref_base&& other) ASIO_NOEXCEPT
    : object_fns_(other.object_fns_),
      target_fns_(other.target_fns_)
  {
    other.object_fns_ = object_fns_table<void>();
    other.target_fns_ = target_fns_table<void>();
    object_fns_->move(*this, other);
    other.target_ = 0;
  }

  any_ref_base& operator=(
      any_ref_base&& other) ASIO_NOEXCEPT
  {
    if (this != &other)
    {
      object_fns_->destroy(*this);
      object_fns_ = other.object_fns_;
      other.object_fns_ = object_fns_table<void>();
      target_fns_ = other.target_fns_;
      other.target_fns_ = target_fns_table<void>();
      object_fns_->move(*this, other);
      other.target_ = 0;
    }
    return *this;
  }

#endif // defined(ASIO_HAS_MOVE)

  void swap(any_ref_base& other) ASIO_NOEXCEPT
  {
    if (this != &other)
    {
      any_ref_base tmp(ASIO_MOVE_CAST(any_ref_base)(other));
      other = ASIO_MOVE_CAST(any_ref_base)(*this);
      *this = ASIO_MOVE_CAST(any_ref_base)(tmp);
    }
  }

  struct unspecified_bool_type_t {};
  typedef void (*unspecified_bool_type)(unspecified_bool_type_t);
  static void unspecified_bool_true(unspecified_bool_type_t) {}

  operator unspecified_bool_type() const ASIO_NOEXCEPT
  {
    return target_ ? &any_ref_base::unspecified_bool_true : 0;
  }

  bool operator!() const ASIO_NOEXCEPT
  {
    return target_ == 0;
  }

protected:
  template <typename StaticTarget>
  StaticTarget* target()
  {
    return static_cast<StaticTarget*>(target_);
  }

  template <typename StaticTarget>
  const StaticTarget* target() const
  {
    return static_cast<StaticTarget*>(target_);
  }

  bool equality_helper(const any_ref_base& other) const ASIO_NOEXCEPT
  {
    if (target_ == other.target_)
      return true;
    if (target_ && !other.target_)
      return false;
    if (!target_ && other.target_)
      return false;
    if (target_fns_ != other.target_fns_)
      return false;
    return target_fns_->equal(*this, other);
  }

  template <typename Tgt>
  Tgt& object()
  {
    return *static_cast<Tgt*>(static_cast<void*>(&object_));
  }

  template <typename Tgt>
  const Tgt& object() const
  {
    return *static_cast<const Tgt*>(static_cast<const void*>(&object_));
  }

  struct object_fns
  {
    void (*destroy)(any_ref_base&);
    void (*copy)(any_ref_base&, const any_ref_base&);
    void (*move)(any_ref_base&, any_ref_base&);
    const void* (*target)(const any_ref_base&);
  };

  static void destroy_void(any_ref_base&)
  {
  }

  static void copy_void(any_ref_base& target1, const any_ref_base&)
  {
    target1.target_ = 0;
  }

  static void move_void(any_ref_base& target1, any_ref_base&)
  {
    target1.target_ = 0;
  }

  static const void* target_void(const any_ref_base&)
  {
    return 0;
  }

  template <typename Obj>
  static const object_fns* object_fns_table(
      typename enable_if<
        is_same<Obj, void>::value
      >::type* = 0)
  {
    static const object_fns fns =
    {
      &any_ref_base::destroy_void,
      &any_ref_base::copy_void,
      &any_ref_base::move_void,
      &any_ref_base::target_void
    };
    return &fns;
  }

  static void destroy_shared(any_ref_base& target)
  {
    typedef asio::detail::shared_ptr<void> type;
    target.object<type>().~type();
  }

  static void copy_shared(any_ref_base& target1, const any_ref_base& target2)
  {
    typedef asio::detail::shared_ptr<void> type;
    new (&target1.object_) type(target2.object<type>());
    target1.target_ = target2.target_;
  }

  static void move_shared(any_ref_base& target1, any_ref_base& target2)
  {
    typedef asio::detail::shared_ptr<void> type;
    new (&target1.object_) type(ASIO_MOVE_CAST(type)(target2.object<type>()));
    target1.target_ = target2.target_;
    target2.object<type>().~type();
  }

  static const void* target_shared(const any_ref_base& target)
  {
    typedef asio::detail::shared_ptr<void> type;
    return target.object<type>().get();
  }

  template <typename Obj>
  static const object_fns* object_fns_table(
      typename enable_if<
        is_same<Obj, asio::detail::shared_ptr<void> >::value
      >::type* = 0)
  {
    static const object_fns fns =
    {
      &any_ref_base::destroy_shared,
      &any_ref_base::copy_shared,
      &any_ref_base::move_shared,
      &any_ref_base::target_shared
    };
    return &fns;
  }

  template <typename Obj>
  static void destroy_object(any_ref_base& target)
  {
    target.object<Obj>().~Obj();
  }

  template <typename Obj>
  static void copy_object(any_ref_base& target1, const any_ref_base& target2)
  {
    new (&target1.object_) Obj(target2.object<Obj>());
    target1.target_ = &target1.object<Obj>();
  }

  template <typename Obj>
  static void move_object(any_ref_base& target1, any_ref_base& target2)
  {
    new (&target1.object_) Obj(ASIO_MOVE_CAST(Obj)(target2.object<Obj>()));
    target1.target_ = &target1.object<Obj>();
    target2.object<Obj>().~Obj();
  }

  template <typename Obj>
  static const void* target_object(const any_ref_base& target)
  {
    return &target.object<Obj>();
  }

  template <typename Obj>
  static const object_fns* object_fns_table(
      typename enable_if<
        !is_same<Obj, void>::value
          && !is_same<Obj, asio::detail::shared_ptr<void> >::value
      >::type* = 0)
  {
    static const object_fns fns =
    {
      &any_ref_base::destroy_object<Obj>,
      &any_ref_base::copy_object<Obj>,
      &any_ref_base::move_object<Obj>,
      &any_ref_base::target_object<Obj>
    };
    return &fns;
  }

  struct target_fns
  {
    bool (*equal)(const any_ref_base&, const any_ref_base&);
  };

  static bool equal_void(const any_ref_base&, const any_ref_base&)
  {
    return true;
  }

  template <typename Tgt>
  static const target_fns* target_fns_table(
      typename enable_if<
        is_same<Tgt, void>::value
      >::type* = 0)
  {
    static const target_fns fns =
    {
      &any_ref_base::equal_void,
    };
    return &fns;
  }

  template <typename Tgt>
  static auto equal_ex(const any_ref_base& target1,
      const any_ref_base& target2) 
    -> typename enable_if<traits::equality_comparable<Tgt>::is_valid, bool>::type
  {
    return *target1.target<Tgt>() == *target2.target<Tgt>();
  }
  template <typename Tgt>
  static auto equal_ex(const any_ref_base&,
      const any_ref_base&) 
    -> typename enable_if<!traits::equality_comparable<Tgt>::is_valid, bool>::type
  {
    return false;
  }

  template <typename Tgt>
  static const target_fns* target_fns_table(
      typename enable_if<
        !is_same<Tgt, void>::value
      >::type* = 0)
  {
    static const target_fns fns =
    {
      &any_ref_base::equal_ex<Tgt>
    };

    return &fns;
  }

private:

  template <typename Target>
  void construct_object(Target& target, true_type)
  {
    object_fns_ = object_fns_table<Target>();
    target_ = new (&object_) Target(ASIO_MOVE_CAST(Target)(target));
  }

  template <typename Target>
  void construct_object(Target& target, false_type)
  {
    object_fns_ = object_fns_table<asio::detail::shared_ptr<void> >();
    asio::detail::shared_ptr<Target> p =
      asio::detail::make_shared<Target>(
          ASIO_MOVE_CAST(Target)(target));
    target_ = p.get();
    new (&object_) asio::detail::shared_ptr<void>(
        ASIO_MOVE_CAST(asio::detail::shared_ptr<Target>)(p));
  }

/*private:*/public:
//  template <typename...> friend class any_ref;

  typedef aligned_storage<
      sizeof(asio::detail::shared_ptr<void>),
      alignment_of<asio::detail::shared_ptr<void> >::value
    >::type object_type;

  object_type object_;
  const object_fns* object_fns_;
  void* target_;
  const target_fns* target_fns_;
};

} // namespace detail

template <>
class any_ref<> : public detail::any_ref_base
{
public:
  any_ref() ASIO_NOEXCEPT
    : detail::any_ref_base()
  {
  }

  any_ref(nullptr_t) ASIO_NOEXCEPT
    : detail::any_ref_base()
  {
  }

  template <typename Target>
  any_ref(Target target,
      typename enable_if<
        !is_same<Target, any_ref>::value
        && !is_base_of<detail::any_ref_base, Target>::value
      >::type* = 0)
    : detail::any_ref_base(
        ASIO_MOVE_CAST(Target)(target), false_type())
  {
  }

#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

  template <typename... OtherCpos>
  any_ref(any_ref<OtherCpos...> other)
    : detail::any_ref_base(
        static_cast<const detail::any_ref_base&>(other))
  {
  }

#else // defined(ASIO_HAS_VARIADIC_TEMPLATES)

  template <typename U0, typename U1, typename U2, typename U3,
      typename U4, typename U5, typename U6, typename U7>
  any_ref(any_ref<U0, U1, U2, U3, U4, U5, U6, U7> other)
    : detail::any_ref_base(
        static_cast<const detail::any_ref_base&>(other))
  {
  }

#endif // defined(ASIO_HAS_VARIADIC_TEMPLATES)

  any_ref(const any_ref& other) ASIO_NOEXCEPT
    : detail::any_ref_base(
        static_cast<const detail::any_ref_base&>(other))
  {
  }

  any_ref& operator=(const any_ref& other) ASIO_NOEXCEPT
  {
    if (this != &other)
    {
      detail::any_ref_base::operator=(
          static_cast<const detail::any_ref_base&>(other));
    }
    return *this;
  }

  any_ref& operator=(nullptr_t p) ASIO_NOEXCEPT
  {
    detail::any_ref_base::operator=(p);
    return *this;
  }

#if defined(ASIO_HAS_MOVE)

  any_ref(any_ref&& other) ASIO_NOEXCEPT
    : detail::any_ref_base(
        static_cast<any_ref_base&&>(
          static_cast<any_ref_base&>(other)))
  {
  }

  any_ref& operator=(any_ref&& other) ASIO_NOEXCEPT
  {
    if (this != &other)
    {
      detail::any_ref_base::operator=(
          static_cast<detail::any_ref_base&&>(
            static_cast<detail::any_ref_base&>(other)));
    }
    return *this;
  }

#endif // defined(ASIO_HAS_MOVE)

  void swap(any_ref& other) ASIO_NOEXCEPT
  {
    detail::any_ref_base::swap(
        static_cast<detail::any_ref_base&>(other));
  }

  using detail::any_ref_base::operator unspecified_bool_type;
  using detail::any_ref_base::operator!;

  bool equality_helper(const any_ref& other) const ASIO_NOEXCEPT
  {
    return any_ref_base::equality_helper(other);
  }

private:
  using detail::any_ref_base::target;
};

inline bool operator==(const any_ref<>& a,
    const any_ref<>& b) ASIO_NOEXCEPT
{
  return a.equality_helper(b);
}

inline bool operator==(const any_ref<>& a, nullptr_t) ASIO_NOEXCEPT
{
  return !a;
}

inline bool operator==(nullptr_t, const any_ref<>& b) ASIO_NOEXCEPT
{
  return !b;
}

inline bool operator!=(const any_ref<>& a,
    const any_ref<>& b) ASIO_NOEXCEPT
{
  return !a.equality_helper(b);
}

inline bool operator!=(const any_ref<>& a, nullptr_t) ASIO_NOEXCEPT
{
  return !!a;
}

inline bool operator!=(nullptr_t, const any_ref<>& b) ASIO_NOEXCEPT
{
  return !!b;
}

inline void swap(any_ref<>& a, any_ref<>& b) ASIO_NOEXCEPT
{
  return a.swap(b);
}

#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename... Cpos>
class any_ref : public detail::any_ref_base
{
public:
  any_ref() ASIO_NOEXCEPT
    : detail::any_ref_base(),
      cpo_fns_(cpo_fns_table<void>())
  {
  }

  any_ref(nullptr_t) ASIO_NOEXCEPT
    : detail::any_ref_base(),
      cpo_fns_(cpo_fns_table<void>())
  {
  }

  template <typename Target>
  any_ref(Target target,
      typename enable_if<
        conditional<
          !is_same<Target, any_ref>::value
            && !is_base_of<detail::any_ref_base, Target>::value,
#if 1
          detail::is_valid_target<
            Target, void(Cpos...)>,
#else
          true_type,
#endif
          false_type
        >::type::value
      >::type* = 0)
    : detail::any_ref_base(
        ASIO_MOVE_CAST(Target)(target), false_type()),
      cpo_fns_(cpo_fns_table<Target>())
  {
  }

  template <typename... OtherCpos>
  any_ref(any_ref<OtherCpos...> other,
      typename enable_if<
        conditional<
          !is_same<
            any_ref<OtherCpos...>,
            any_ref
          >::value,
          typename detail::can_tag_invoke<
            0, void(Cpos...)>::template is_valid_target<
              any_ref<OtherCpos...> >,
          false_type
        >::type::value
      >::type* = 0)
    : detail::any_ref_base(ASIO_MOVE_CAST(
          any_ref<OtherCpos...>)(other), true_type()),
      cpo_fns_(cpo_fns_table<any_ref<OtherCpos...> >())
  {
  }

  any_ref(const any_ref& other) ASIO_NOEXCEPT
    : detail::any_ref_base(
        static_cast<const detail::any_ref_base&>(other)),
      cpo_fns_(other.cpo_fns_)
  {
  }

  any_ref& operator=(const any_ref& other) ASIO_NOEXCEPT
  {
    if (this != &other)
    {
      cpo_fns_ = other.cpo_fns_;
      detail::any_ref_base::operator=(
          static_cast<const detail::any_ref_base&>(other));
    }
    return *this;
  }

  any_ref& operator=(nullptr_t p) ASIO_NOEXCEPT
  {
    cpo_fns_ = cpo_fns_table<void>();
    detail::any_ref_base::operator=(p);
    return *this;
  }

#if defined(ASIO_HAS_MOVE)

  any_ref(any_ref&& other) ASIO_NOEXCEPT
    : detail::any_ref_base(
        static_cast<any_ref_base&&>(
          static_cast<any_ref_base&>(other))),
      cpo_fns_(other.cpo_fns_)
  {
    other.cpo_fns_ = cpo_fns_table<void>();
  }

  any_ref& operator=(any_ref&& other) ASIO_NOEXCEPT
  {
    if (this != &other)
    {
      cpo_fns_ = other.cpo_fns_;
      detail::any_ref_base::operator=(
          static_cast<detail::any_ref_base&&>(
            static_cast<detail::any_ref_base&>(other)));
    }
    return *this;
  }

#endif // defined(ASIO_HAS_MOVE)

  void swap(any_ref& other) ASIO_NOEXCEPT
  {
    if (this != &other)
    {
      detail::any_ref_base::swap(
          static_cast<detail::any_ref_base&>(other));
      const vtable<Cpos...>* tmp_cpo_fns = other.cpo_fns_;
      other.cpo_fns_ = cpo_fns_;
      cpo_fns_ = tmp_cpo_fns;
    }
  }

  using detail::any_ref_base::operator unspecified_bool_type;
  using detail::any_ref_base::operator!;

  bool equality_helper(const any_ref& other) const ASIO_NOEXCEPT
  {
    return any_ref_base::equality_helper(other);
  }

  template <typename T>
  struct find_convertible_cpo :
      detail::can_tag_invoke<
        0, void(Cpos...)>::template
          find_convertible_cpo<T> {};

protected:
  const vtable<Cpos...>* get_vtable() {
    return cpo_fns_;
  };

private:

  template <typename Cpo, typename Self, typename... ArgN>
  friend auto tag_invoke(Cpo cpo, Self&& self, ArgN&&... argN) 
    -> typename enable_if<is_base_of<typename decay<Self>::type, any_ref>::value, 
        decltype(
        vtable<Cpos...>::vtable_invoke(
        cpo, 
        *self.get_vtable(), 
        self.template target<void>(),
        argN...))
      >::type
  {
    typedef typename base_cpo<Cpo>::type cpo_t;
    typedef typename std::conditional<std::is_const<typename std::remove_reference<Self>::type>::value, const void, void>::type void_target_t;
    return vtable<Cpos...>::vtable_invoke(
        cpo, 
        *self.get_vtable(), 
        self.template target<void>(), 
        (ArgN&&)argN...);
  }

  template <typename Tgt>
  static const vtable<Cpos...>* cpo_fns_table()
  {
    static const vtable<Cpos...> fns = vtable<Cpos...>::template create<Tgt>();
    return &fns;
  }

  const vtable<Cpos...>* cpo_fns_;

  using detail::any_ref_base::target;
};

template <typename... Cpos>
inline bool operator==(const any_ref<Cpos...>& a,
    const any_ref<Cpos...>& b) ASIO_NOEXCEPT
{
  return a.equality_helper(b);
}

template <typename... Cpos>
inline bool operator==(const any_ref<Cpos...>& a,
    nullptr_t) ASIO_NOEXCEPT
{
  return !a;
}

template <typename... Cpos>
inline bool operator==(nullptr_t,
    const any_ref<Cpos...>& b) ASIO_NOEXCEPT
{
  return !b;
}

template <typename... Cpos>
inline bool operator!=(const any_ref<Cpos...>& a,
    const any_ref<Cpos...>& b) ASIO_NOEXCEPT
{
  return !a.equality_helper(b);
}

template <typename... Cpos>
inline bool operator!=(const any_ref<Cpos...>& a,
    nullptr_t) ASIO_NOEXCEPT
{
  return !!a;
}

template <typename... Cpos>
inline bool operator!=(nullptr_t,
    const any_ref<Cpos...>& b) ASIO_NOEXCEPT
{
  return !!b;
}

template <typename... Cpos>
inline void swap(any_ref<Cpos...>& a,
    any_ref<Cpos...>& b) ASIO_NOEXCEPT
{
  return a.swap(b);
}

#else // defined(ASIO_HAS_VARIADIC_TEMPLATES)

#define ASIO_PRIVATE_ANY_REF_PROP_FNS(n) \
  ASIO_PRIVATE_ANY_REF_PROP_FNS_##n

#define ASIO_PRIVATE_ANY_REF_PROP_FNS_1 \
  {  \
    &detail::any_ref_base::tag_invoke_fn<Tgt, T1>, \
  }
#define ASIO_PRIVATE_ANY_REF_PROP_FNS_2 \
  ASIO_PRIVATE_ANY_REF_PROP_FNS_1, \
  { \
    &detail::any_ref_base::tag_invoke_fn<Tgt, T2>, \
  }
#define ASIO_PRIVATE_ANY_REF_PROP_FNS_3 \
  ASIO_PRIVATE_ANY_REF_PROP_FNS_2, \
  { \
    &detail::any_ref_base::tag_invoke_fn<Tgt, T3>, \
  }
#define ASIO_PRIVATE_ANY_REF_PROP_FNS_4 \
  ASIO_PRIVATE_ANY_REF_PROP_FNS_3, \
  { \
    &detail::any_ref_base::tag_invoke_fn<Tgt, T4>, \
  }
#define ASIO_PRIVATE_ANY_REF_PROP_FNS_5 \
  ASIO_PRIVATE_ANY_REF_PROP_FNS_4, \
  { \
    &detail::any_ref_base::tag_invoke_fn<Tgt, T5>, \
  }
#define ASIO_PRIVATE_ANY_REF_PROP_FNS_6 \
  ASIO_PRIVATE_ANY_REF_PROP_FNS_5, \
  { \
    &detail::any_ref_base::tag_invoke_fn<Tgt, T6>, \
  }
#define ASIO_PRIVATE_ANY_REF_PROP_FNS_7 \
  ASIO_PRIVATE_ANY_REF_PROP_FNS_6, \
  { \
    &detail::any_ref_base::tag_invoke_fn<Tgt, T7>, \
  }
#define ASIO_PRIVATE_ANY_REF_PROP_FNS_8 \
  ASIO_PRIVATE_ANY_REF_PROP_FNS_7, \
  { \
    &detail::any_ref_base::tag_invoke_fn<Tgt, T8>, \
  }

#if defined(ASIO_HAS_MOVE)

# define ASIO_PRIVATE_ANY_REF_MOVE_OPS \
  any_ref(any_ref&& other) ASIO_NOEXCEPT \
    : detail::any_ref_base( \
        static_cast<any_ref_base&&>( \
          static_cast<any_ref_base&>(other))), \
      cpo_fns_(other.cpo_fns_) \
  { \
    other.cpo_fns_ = cpo_fns_table<void>(); \
  } \
  \
  any_ref& operator=(any_ref&& other) ASIO_NOEXCEPT \
  { \
    if (this != &other) \
    { \
      cpo_fns_ = other.cpo_fns_; \
      detail::any_ref_base::operator=( \
          static_cast<detail::any_ref_base&&>( \
            static_cast<detail::any_ref_base&>(other))); \
    } \
    return *this; \
  } \
  /**/
#else // defined(ASIO_HAS_MOVE)

# define ASIO_PRIVATE_ANY_REF_MOVE_OPS

#endif // defined(ASIO_HAS_MOVE)

#define ASIO_PRIVATE_ANY_REF_DEF(n) \
  template <ASIO_VARIADIC_TPARAMS(n)> \
  class any_ref<ASIO_VARIADIC_TARGS(n)> : \
    public detail::any_ref_base \
  { \
  public: \
    any_ref() ASIO_NOEXCEPT \
      : detail::any_ref_base(), \
        cpo_fns_(cpo_fns_table<void>()) \
    { \
    } \
    \
    any_ref(nullptr_t) ASIO_NOEXCEPT \
      : detail::any_ref_base(), \
        cpo_fns_(cpo_fns_table<void>()) \
    { \
    } \
    \
    template <ASIO_EXECUTION_EXECUTOR Target> \
    any_ref(Target target, \
        typename enable_if< \
          conditional< \
            !is_same<Target, any_ref>::value \
              && !is_base_of<detail::any_ref_base, Target>::value, \
            detail::is_valid_target< \
              Target, void(ASIO_VARIADIC_TARGS(n))>, \
            false_type \
          >::type::value \
        >::type* = 0) \
      : detail::any_ref_base(ASIO_MOVE_CAST( \
            Target)(target), false_type()), \
        cpo_fns_(cpo_fns_table<Target>()) \
    { \
    } \
    \
    any_ref(const any_ref& other) ASIO_NOEXCEPT \
      : detail::any_ref_base( \
          static_cast<const detail::any_ref_base&>(other)), \
        cpo_fns_(other.cpo_fns_) \
    { \
    } \
    \
    any_ref(any_ref<> other) \
      : detail::any_ref_base(ASIO_MOVE_CAST( \
            any_ref<>)(other), true_type()), \
        cpo_fns_(cpo_fns_table<any_ref<> >()) \
    { \
    } \
    \
    template <typename OtherAnyTarget> \
    any_ref(OtherAnyTarget other, \
        typename enable_if< \
          conditional< \
            !is_same<OtherAnyTarget, any_ref>::value \
              && is_base_of<detail::any_ref_base, \
                OtherAnyTarget>::value, \
            typename detail::can_tag_invoke< \
              0, void(ASIO_VARIADIC_TARGS(n))>::template \
                is_valid_target<OtherAnyTarget>, \
            false_type \
          >::type::value \
        >::type* = 0) \
      : detail::any_ref_base(ASIO_MOVE_CAST( \
            OtherAnyTarget)(other), true_type()), \
        cpo_fns_(cpo_fns_table<OtherAnyTarget>()) \
    { \
    } \
    \
    any_ref& operator=(const any_ref& other) ASIO_NOEXCEPT \
    { \
      if (this != &other) \
      { \
        cpo_fns_ = other.cpo_fns_; \
        detail::any_ref_base::operator=( \
            static_cast<const detail::any_ref_base&>(other)); \
      } \
      return *this; \
    } \
    \
    any_ref& operator=(nullptr_t p) ASIO_NOEXCEPT \
    { \
      cpo_fns_ = cpo_fns_table<void>(); \
      detail::any_ref_base::operator=(p); \
      return *this; \
    } \
    \
    ASIO_PRIVATE_ANY_REF_MOVE_OPS \
    \
    void swap(any_ref& other) ASIO_NOEXCEPT \
    { \
      if (this != &other) \
      { \
        detail::any_ref_base::swap( \
            static_cast<detail::any_ref_base&>(other)); \
        const vtable<ASIO_VARIADIC_TARGS(n)>* tmp_cpo_fns = other.cpo_fns_; \
        other.cpo_fns_ = cpo_fns_; \
        cpo_fns_ = tmp_cpo_fns; \
      } \
    } \
    \
    using detail::any_ref_base::operator unspecified_bool_type; \
    using detail::any_ref_base::operator!; \
    \
    bool equality_helper(const any_ref& other) const ASIO_NOEXCEPT \
    { \
      return any_ref_base::equality_helper(other); \
    } \
    \
    template <typename T> \
    struct find_convertible_cpo : \
        detail::can_tag_invoke< \
          0, void(ASIO_VARIADIC_TARGS(n))>::template \
            find_convertible_cpo<T> {}; \
    \
    template <typename Cpo> \
    void tag_invoke(const Cpo& p, \
        typename enable_if< \
          is_same< \
            typename find_convertible_cpo<Cpo>::tag_invoke_result_type, \
            void \
          >::value \
        >::type* = 0) const \
    { \
      typedef find_convertible_cpo<Cpo> found; \
      cpo_fns_[found::index].tag_invoke(0, object_fns_->target(*this), \
          &static_cast<const typename found::type&>(p)); \
    } \
    \
    template <typename Cpo> \
    typename find_convertible_cpo<Cpo>::tag_invoke_result_type \
    tag_invoke(const Cpo& p, \
        typename enable_if< \
          !is_same< \
            typename find_convertible_cpo<Cpo>::tag_invoke_result_type, \
            void \
          >::value \
          && \
          is_reference< \
            typename find_convertible_cpo<Cpo>::tag_invoke_result_type \
          >::value \
        >::type* = 0) const \
    { \
      typedef find_convertible_cpo<Cpo> found; \
      typename remove_reference< \
        typename found::tag_invoke_result_type>::type* result; \
      cpo_fns_[found::index].tag_invoke(&result, object_fns_->target(*this), \
          &static_cast<const typename found::type&>(p)); \
      return *result; \
    } \
    \
    template <typename Cpo> \
    typename find_convertible_cpo<Cpo>::tag_invoke_result_type \
    tag_invoke(const Cpo& p, \
        typename enable_if< \
          !is_same< \
            typename find_convertible_cpo<Cpo>::tag_invoke_result_type, \
            void \
          >::value \
          && \
          is_scalar< \
            typename find_convertible_cpo<Cpo>::tag_invoke_result_type \
          >::value \
        >::type* = 0) const \
    { \
      typedef find_convertible_cpo<Cpo> found; \
      typename found::tag_invoke_result_type result; \
      cpo_fns_[found::index].tag_invoke(&result, object_fns_->target(*this), \
          &static_cast<const typename found::type&>(p)); \
      return result; \
    } \
    \
    template <typename Cpo> \
    typename find_convertible_cpo<Cpo>::tag_invoke_result_type \
    tag_invoke(const Cpo& p, \
        typename enable_if< \
          !is_same< \
            typename find_convertible_cpo<Cpo>::tag_invoke_result_type, \
            void \
          >::value \
          && \
          !is_reference< \
            typename find_convertible_cpo<Cpo>::tag_invoke_result_type \
          >::value \
          && \
          !is_scalar< \
            typename find_convertible_cpo<Cpo>::tag_invoke_result_type \
          >::value \
        >::type* = 0) const \
    { \
      typedef find_convertible_cpo<Cpo> found; \
      typename found::tag_invoke_result_type* result; \
      cpo_fns_[found::index].tag_invoke(&result, object_fns_->target(*this), \
          &static_cast<const typename found::type&>(p)); \
      return *asio::detail::scoped_ptr< \
        typename found::tag_invoke_result_type>(result); \
    } \
    \
    template <typename Tgt> \
    static const vtable<ASIO_VARIADIC_TARGS(n)>* cpo_fns_table() \
    { \
      static const vtable<ASIO_VARIADIC_TARGS(n)> fns;
      return &fns; \
    } \
    \
    const vtable<ASIO_VARIADIC_TARGS(n)>* cpo_fns_; \
    typedef detail::can_tag_invoke<0, \
        void(ASIO_VARIADIC_TARGS(n))> can_tag_invoke_type; \
    private: \
      using detail::any_ref_base::target; \
  }; \
  \
  template <ASIO_VARIADIC_TPARAMS(n)> \
  inline bool operator==(const any_ref<ASIO_VARIADIC_TARGS(n)>& a, \
      const any_ref<ASIO_VARIADIC_TARGS(n)>& b) ASIO_NOEXCEPT \
  { \
    return a.equality_helper(b); \
  } \
  \
  template <ASIO_VARIADIC_TPARAMS(n)> \
  inline bool operator==(const any_ref<ASIO_VARIADIC_TARGS(n)>& a, \
      nullptr_t) ASIO_NOEXCEPT \
  { \
    return !a; \
  } \
  \
  template <ASIO_VARIADIC_TPARAMS(n)> \
  inline bool operator==(nullptr_t, \
      const any_ref<ASIO_VARIADIC_TARGS(n)>& b) ASIO_NOEXCEPT \
  { \
    return !b; \
  } \
  \
  template <ASIO_VARIADIC_TPARAMS(n)> \
  inline bool operator!=(const any_ref<ASIO_VARIADIC_TARGS(n)>& a, \
      const any_ref<ASIO_VARIADIC_TARGS(n)>& b) ASIO_NOEXCEPT \
  { \
    return !a.equality_helper(b); \
  } \
  \
  template <ASIO_VARIADIC_TPARAMS(n)> \
  inline bool operator!=(const any_ref<ASIO_VARIADIC_TARGS(n)>& a, \
      nullptr_t) ASIO_NOEXCEPT \
  { \
    return !!a; \
  } \
  \
  template <ASIO_VARIADIC_TPARAMS(n)> \
  inline bool operator!=(nullptr_t, \
      const any_ref<ASIO_VARIADIC_TARGS(n)>& b) ASIO_NOEXCEPT \
  { \
    return !!b; \
  } \
  \
  template <ASIO_VARIADIC_TPARAMS(n)> \
  inline void swap(any_ref<ASIO_VARIADIC_TARGS(n)>& a, \
      any_ref<ASIO_VARIADIC_TARGS(n)>& b) ASIO_NOEXCEPT \
  { \
    return a.swap(b); \
  } \
  /**/
  ASIO_VARIADIC_GENERATE(ASIO_PRIVATE_ANY_REF_DEF)
#undef ASIO_PRIVATE_ANY_REF_DEF
#undef ASIO_PRIVATE_ANY_REF_MOVE_OPS
#undef ASIO_PRIVATE_ANY_REF_PROP_FNS
#undef ASIO_PRIVATE_ANY_REF_PROP_FNS_1
#undef ASIO_PRIVATE_ANY_REF_PROP_FNS_2
#undef ASIO_PRIVATE_ANY_REF_PROP_FNS_3
#undef ASIO_PRIVATE_ANY_REF_PROP_FNS_4
#undef ASIO_PRIVATE_ANY_REF_PROP_FNS_5
#undef ASIO_PRIVATE_ANY_REF_PROP_FNS_6
#undef ASIO_PRIVATE_ANY_REF_PROP_FNS_7
#undef ASIO_PRIVATE_ANY_REF_PROP_FNS_8

#endif // if defined(ASIO_HAS_VARIADIC_TEMPLATES)

} // namespace tag_invokes
namespace traits {

#if !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)
#if defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <typename... Cpos>
struct equality_comparable<tag_invokes::any_ref<Cpos...> >
{
  static const bool is_valid = true;
  static const bool is_noexcept = true;
};

#else // defined(ASIO_HAS_VARIADIC_TEMPLATES)

template <>
struct equality_comparable<tag_invokes::any_ref<> >
{
  static const bool is_valid = true;
  static const bool is_noexcept = true;
};

#define ASIO_PRIVATE_ANY_REF_EQUALITY_COMPARABLE_DEF(n) \
  template <ASIO_VARIADIC_TPARAMS(n)> \
  struct equality_comparable< \
      tag_invokes::any_ref<ASIO_VARIADIC_TARGS(n)> > \
  { \
    static const bool is_valid = true; \
    static const bool is_noexcept = true; \
  }; \
  /**/
  ASIO_VARIADIC_GENERATE(
      ASIO_PRIVATE_ANY_REF_EQUALITY_COMPARABLE_DEF)
#undef ASIO_PRIVATE_ANY_REF_EQUALITY_COMPARABLE_DEF

#endif // defined(ASIO_HAS_VARIADIC_TEMPLATES)
#endif // !defined(ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

} // namespace traits

#endif // defined(GENERATING_DOCUMENTATION)

} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_ANY_REF_HPP
