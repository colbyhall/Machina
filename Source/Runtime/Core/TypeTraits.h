/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Core.h>
#include <type_traits>
#include <utility>

// Aliasing type traits found in the C++20 standard library to match our naming conventions
// https://en.cppreference.com/w/cpp/header/type_traits
namespace Grizzly::Core {
	// https://en.cppreference.com/w/cpp/types/is_void
	template <typename T>
	inline constexpr bool is_void = std::is_void_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_null_pointer
	template <typename T>
	inline constexpr bool is_null_pointer = std::is_null_pointer_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_integral
	template <typename T>
	inline constexpr bool is_integral = std::is_integral_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_floating_point
	template <typename T>
	inline constexpr bool is_floating_point = std::is_floating_point_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_array
	template <typename T>
	inline constexpr bool is_array = std::is_array_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_enum
	template <typename T>
	inline constexpr bool is_enum = std::is_enum_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_union
	template <typename T>
	inline constexpr bool is_union = std::is_union_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_class
	template <typename T>
	inline constexpr bool is_class = std::is_class_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_function
	template <typename T>
	inline constexpr bool is_function = std::is_function_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_pointer
	template <typename T>
	inline constexpr bool is_pointer = std::is_pointer_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_lvalue_reference
	template <typename T>
	inline constexpr bool is_lvalue_reference = std::is_lvalue_reference_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_rvalue_reference
	template <typename T>
	inline constexpr bool is_rvalue_reference = std::is_rvalue_reference_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_member_object_pointer
	template <typename T>
	inline constexpr bool is_member_object_pointer = std::is_member_object_pointer_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_member_function_pointer
	template <typename T>
	inline constexpr bool is_member_function_pointer = std::is_member_function_pointer_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_fundamental
	template <typename T>
	inline constexpr bool is_fundamental = std::is_fundamental_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_arithmetic
	template <typename T>
	inline constexpr bool is_arithmetic = std::is_arithmetic_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_scalar
	template <typename T>
	inline constexpr bool is_scalar = std::is_scalar_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_object
	template <typename T>
	inline constexpr bool is_object = std::is_object_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_compound
	template <typename T>
	inline constexpr bool is_compound = std::is_compound_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_reference
	template <typename T>
	inline constexpr bool is_reference = std::is_reference_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_member_pointer
	template <typename T>
	inline constexpr bool is_member_pointer = std::is_member_pointer_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_const
	template <typename T>
	inline constexpr bool is_const = std::is_const_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_volatile
	template <typename T>
	inline constexpr bool is_volatile = std::is_volatile_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_trivial
	template <typename T>
	inline constexpr bool is_trivial = std::is_trivial_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_trivially_copyable
	template <typename T>
	inline constexpr bool is_trivially_copyable = std::is_trivially_copyable_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_standard_layout
	template <typename T>
	inline constexpr bool is_standard_layout = std::is_standard_layout_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_pod
	template <typename T>
	inline constexpr bool is_pod = std::is_pod_v<T>;

	// https://en.cppreference.com/w/cpp/types/has_unique_object_representations
	template <typename T>
	inline constexpr bool has_unique_object_representations = std::has_unique_object_representations_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_empty
	template <typename T>
	inline constexpr bool is_empty = std::is_empty_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_polymorphic
	template <typename T>
	inline constexpr bool is_polymorphic = std::is_polymorphic_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_abstract
	template <typename T>
	inline constexpr bool is_abstract = std::is_abstract_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_final
	template <typename T>
	inline constexpr bool is_final = std::is_final_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_aggregate
	template <typename T>
	inline constexpr bool is_aggregate = std::is_aggregate_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_signed
	template <typename T>
	inline constexpr bool is_signed = std::is_signed_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_unsigned
	template <typename T>
	inline constexpr bool is_unsigned = std::is_unsigned_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_bounded_array
	template <typename T>
	inline constexpr bool is_bounded_array = std::is_bounded_array_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_unbounded_array
	template <typename T>
	inline constexpr bool is_unbounded_array = std::is_unbounded_array_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_constructible
	template <typename T, typename... Args>
	inline constexpr bool is_constructible = std::is_constructible_v<T, Args...>;

	// https://en.cppreference.com/w/cpp/types/is_constructible
	template <typename T, typename... Args>
	inline constexpr bool is_trivially_constructible = std::is_trivially_constructible_v<T, Args...>;

	// https://en.cppreference.com/w/cpp/types/is_constructible
	template <typename T, typename... Args>
	inline constexpr bool is_nothrow_constructible = std::is_nothrow_constructible_v<T, Args...>;

	// https://en.cppreference.com/w/cpp/types/is_default_constructible
	template <typename T>
	inline constexpr bool is_default_constructible = std::is_default_constructible_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_default_constructible
	template <typename T>
	inline constexpr bool is_trivially_default_constructible = std::is_trivially_default_constructible_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_default_constructible
	template <typename T>
	inline constexpr bool is_nothrow_default_constructible = std::is_nothrow_default_constructible_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_copy_constructible
	template <typename T>
	inline constexpr bool is_copy_constructible = std::is_copy_constructible_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_copy_constructible
	template <typename T>
	inline constexpr bool is_trivially_copy_constructible = std::is_trivially_copy_constructible_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_copy_constructible
	template <typename T>
	inline constexpr bool is_nothrow_copy_constructible = std::is_nothrow_copy_constructible_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_move_constructible
	template <typename T>
	inline constexpr bool is_move_constructible = std::is_move_constructible_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_move_constructible
	template <typename T>
	inline constexpr bool is_trivially_move_constructible = std::is_trivially_move_constructible_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_move_constructible
	template <typename T>
	inline constexpr bool is_nothrow_move_constructible = std::is_nothrow_move_constructible_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_assignable
	template <typename T, typename U>
	inline constexpr bool is_assignable = std::is_assignable_v<T, U>;

	// https://en.cppreference.com/w/cpp/types/is_assignable
	template <typename T, typename U>
	inline constexpr bool is_trivially_assignable = std::is_trivially_assignable_v<T, U>;

	// https://en.cppreference.com/w/cpp/types/is_assignable
	template <typename T, typename U>
	inline constexpr bool is_nothrow_assignable = std::is_nothrow_assignable_v<T, U>;

	// https://en.cppreference.com/w/cpp/types/is_copy_assignable
	template <typename T>
	inline constexpr bool is_copy_assignable = std::is_copy_assignable_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_copy_assignable
	template <typename T>
	inline constexpr bool is_trivially_copy_assignable = std::is_trivially_copy_assignable_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_copy_assignable
	template <typename T>
	inline constexpr bool is_nothrow_copy_assignable = std::is_nothrow_copy_assignable_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_move_assignable
	template <typename T>
	inline constexpr bool is_move_assignable = std::is_move_assignable_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_move_assignable
	template <typename T>
	inline constexpr bool is_trivially_move_assignable = std::is_trivially_move_assignable_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_move_assignable
	template <typename T>
	inline constexpr bool is_nothrow_move_assignable = std::is_nothrow_move_assignable_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_destructible
	template <typename T>
	inline constexpr bool is_destructible = std::is_destructible_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_destructible
	template <typename T>
	inline constexpr bool is_trivially_destructible = std::is_trivially_destructible_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_destructible
	template <typename T>
	inline constexpr bool is_nothrow_destructible = std::is_nothrow_destructible_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_destructible
	template <typename T>
	inline constexpr bool has_virtual_destructor = std::has_virtual_destructor_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_swappable
	template <typename T>
	inline constexpr bool is_swappable = std::is_swappable_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_swappable
	template <typename T, typename U>
	inline constexpr bool is_swappable_with = std::is_swappable_with_v<T, U>;

	// https://en.cppreference.com/w/cpp/types/is_swappable
	template <typename T>
	inline constexpr bool is_nothrow_swappable = std::is_nothrow_swappable_v<T>;

	// https://en.cppreference.com/w/cpp/types/is_swappable
	template <typename T, typename U>
	inline constexpr bool is_nothrow_swappable_with = std::is_nothrow_swappable_with_v<T, U>;

	// https://en.cppreference.com/w/cpp/types/alignment_of
	template <typename T>
	inline constexpr std::size_t alignment_of = std::alignment_of_v<T>;

	// https://en.cppreference.com/w/cpp/types/rank
	template <typename T>
	inline constexpr std::size_t rank = std::rank_v<T>;

	// https://en.cppreference.com/w/cpp/extent
	template <typename T, std::size_t N = 0>
	inline constexpr std::size_t extent = std::extent_v<T, N>;

	// https://en.cppreference.com/w/cpp/types/is_same
	template <typename T, typename U>
	inline constexpr bool is_same = std::is_same_v<T, U>;

	// https://en.cppreference.com/w/cpp/types/is_base_of
	template <typename Base, typename Derived>
	inline constexpr bool is_base_of = std::is_base_of_v<Base, Derived>;

	// https://en.cppreference.com/w/cpp/types/is_convertible
	template <typename From, typename To>
	inline constexpr bool is_convertible = std::is_convertible_v<From, To>;

	// https://en.cppreference.com/w/cpp/types/is_convertible
	template <typename From, typename To>
	inline constexpr bool is_nothrow_convertible = std::is_nothrow_convertible_v<From, To>;

	// https://en.cppreference.com/w/cpp/types/is_invocable
	template <typename F, typename... Args>
	inline constexpr bool is_invocable = std::is_invocable_v<F, Args...>;

	// https://en.cppreference.com/w/cpp/types/is_invocable
	template <typename R, typename F, typename... Args>
	inline constexpr bool is_invocable_r = std::is_invocable_r_v<R, F, Args...>;

	// https://en.cppreference.com/w/cpp/types/is_invocable
	template <typename F, typename... Args>
	inline constexpr bool is_nothrow_invocable = std::is_nothrow_invocable_v<F, Args...>;

	// https://en.cppreference.com/w/cpp/types/is_invocable
	template <typename R, typename F, typename... Args>
	inline constexpr bool is_nothrow_invocable_r = std::is_nothrow_invocable_r_v<R, F, Args...>;

	// https://en.cppreference.com/w/cpp/types/remove_cv
	template <typename T>
	using RemoveCV = std::remove_cv_t<T>;

	// https://en.cppreference.com/w/cpp/types/remove_cv
	template <typename T>
	using RemoveConst = std::remove_const_t<T>;

	// https://en.cppreference.com/w/cpp/types/remove_cv
	template <typename T>
	using RemoveVolatile = std::remove_volatile_t<T>;

	// https://en.cppreference.com/w/cpp/types/add_cv
	template <typename T>
	using AddCV = std::add_cv_t<T>;

	// https://en.cppreference.com/w/cpp/types/add_cv
	template <typename T>
	using AddConst = std::add_const_t<T>;

	// https://en.cppreference.com/w/cpp/types/add_cv
	template <typename T>
	using AddVolatile = std::add_volatile_t<T>;

	// https://en.cppreference.com/w/cpp/types/remove_reference
	template <typename T>
	using RemoveReference = std::remove_reference_t<T>;

	// https://en.cppreference.com/w/cpp/types/add_reference
	template <typename T>
	using AddLvalueReference = std::add_lvalue_reference_t<T>;

	// https://en.cppreference.com/w/cpp/types/add_reference
	template <typename T>
	using AddRvalueReference = std::add_rvalue_reference_t<T>;

	// https://en.cppreference.com/w/cpp/types/remove_pointer
	template <typename T>
	using RemovePointer = std::remove_pointer_t<T>;

	// https://en.cppreference.com/w/cpp/types/add_pointer
	template <typename T>
	using AddPointer = std::add_pointer_t<T>;

	// https://en.cppreference.com/w/cpp/types/make_signed
	template <typename T>
	using MakeSigned = std::make_signed_t<T>;

	// https://en.cppreference.com/w/cpp/types/make_unsigned
	template <typename T>
	using MakeUnsigned = std::make_unsigned_t<T>;

	// https://en.cppreference.com/w/cpp/types/remove_extent
	template <typename T>
	using RemoveExtent = std::remove_extent_t<T>;

	// https://en.cppreference.com/w/cpp/types/remove_all_extents
	template <typename T>
	using RemoveAllExtents = std::remove_all_extents_t<T>;

	// https://en.cppreference.com/w/cpp/types/decay
	template <typename T>
	using Decay = std::decay_t<T>;

	// https://en.cppreference.com/w/cpp/types/remove_cvref
	template <typename T>
	using RemoveCvref = std::remove_cvref_t<T>;

	// https://en.cppreference.com/w/cpp/types/enable_if
	template <bool B, typename T = void>
	using EnableIf = std::enable_if_t<B, T>;

	// https://en.cppreference.com/w/cpp/types/conditional
	template <bool B, typename T, typename F>
	using Conditional = std::conditional_t<B, T, F>;

	// https://en.cppreference.com/w/cpp/types/common_type
	template <typename... T>
	using CommonType = std::common_type_t<T...>;

	// https://en.cppreference.com/w/cpp/types/common_reference
	template <typename... T>
	using CommonReference = std::common_reference_t<T...>;

	// https://en.cppreference.com/w/cpp/types/underlying_type
	template <typename T>
	using UnderlyingType = std::underlying_type_t<T>;

	// https://en.cppreference.com/w/cpp/types/result_of
	template <typename F, typename... Args>
	using InvokeResult = std::invoke_result_t<F, Args...>;

	// https://en.cppreference.com/w/cpp/types/conjunction
	template <typename... B>
	inline constexpr bool conjunction = std::conjunction_v<B...>;

	// https://en.cppreference.com/w/cpp/types/disjunction
	template <typename... B>
	inline constexpr bool disjunction = std::disjunction_v<B...>;

	// https://en.cppreference.com/w/cpp/types/negation
	template <typename B>
	inline constexpr bool negation = std::negation_v<B>;

	// https://en.cppreference.com/w/cpp/utility/forward
	using std::forward;

	// https://en.cppreference.com/w/cpp/utility/move
	using std::move;

	// https://en.cppreference.com/w/cpp/utility/functional/invoke
	using std::invoke;
} // namespace Grizzly::Core

namespace Grizzly {
	using Core::forward;
	using Core::move;
} // namespace Grizzly

#define GRIZZLY_ENUM_CLASS_BITFIELD(Enum)                                                                              \
	inline Enum& operator|=(Enum& A, Enum B) {                                                                         \
		return (Enum&)((Grizzly::Core::UnderlyingType(Enum)&)A |= (Grizzly::Core::UnderlyingType(Enum))B);             \
	}                                                                                                                  \
	inline Enum& operator&=(Enum& A, Enum B) {                                                                         \
		return (Enum&)((Grizzly::Core::UnderlyingType(Enum)&)A &= (Grizzly::Core::UnderlyingType(Enum))B);             \
	}                                                                                                                  \
	inline Enum& operator^=(Enum& A, Enum B) {                                                                         \
		return (Enum&)((Grizzly::Core::UnderlyingType(Enum)&)A ^= (Grizzly::Core::UnderlyingType(Enum))B);             \
	}                                                                                                                  \
	inline Enum operator|(Enum A, Enum B) {                                                                            \
		return (Enum)((Grizzly::Core::UnderlyingType(Enum))A | (Grizzly::Core::UnderlyingType(Enum))B);                \
	}                                                                                                                  \
	inline Enum operator&(Enum A, Enum B) {                                                                            \
		return (Enum)((Grizzly::Core::UnderlyingType(Enum))A & (Grizzly::Core::UnderlyingType(Enum))B);                \
	}                                                                                                                  \
	inline Enum operator^(Enum A, Enum B) {                                                                            \
		return (Enum)((Grizzly::Core::UnderlyingType(Enum))A ^ (Grizzly::Core::UnderlyingType(Enum))B);                \
	}                                                                                                                  \
	inline Enum operator~(Enum A) { return (Enum) ~(Grizzly::Core::UnderlyingType(Enum))A; }
