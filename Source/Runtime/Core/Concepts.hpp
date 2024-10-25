/**
 * Copyright (c) 2024 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <concepts>

// Aliasing concepts found in the C++20 standard library to match our naming conventions
// https://en.cppreference.com/w/cpp/concepts
namespace Grizzly::Core {
	// https://en.cppreference.com/w/cpp/concepts/same_as
	template <typename A, typename B>
	concept SameAs = std::same_as<A, B>;

	// https://en.cppreference.com/w/cpp/concepts/derived_from
	template <typename Derived, typename Base>
	concept DerivedFrom = std::derived_from<Derived, Base>;

	// https://en.cppreference.com/w/cpp/concepts/convertible_to
	template <typename From, typename To>
	concept ConvertibleTo = std::convertible_to<From, To>;

	// https://en.cppreference.com/w/cpp/concepts/common_reference_with
	template <typename T, typename U>
	concept CommonReferenceWith = std::common_reference_with<T, U>;

	// https://en.cppreference.com/w/cpp/concepts/common_with
	template <typename T, typename U>
	concept CommonWith = std::common_with<T, U>;

	// https://en.cppreference.com/w/cpp/concepts/integral
	template <typename T>
	concept Integral = std::integral<T>;

	// https://en.cppreference.com/w/cpp/concepts/signed_integral
	template <typename T>
	concept SignedIntegral = std::signed_integral<T>;

	// https://en.cppreference.com/w/cpp/concepts/unsigned_integral
	template <typename T>
	concept UnsignedIntegral = std::unsigned_integral<T>;

	// https://en.cppreference.com/w/cpp/concepts/floating_point
	template <typename T>
	concept FloatingPoint = std::floating_point<T>;

	// https://en.cppreference.com/w/cpp/concepts/assignable_from
	template <typename LHS, typename RHS>
	concept AssignableFrom = std::assignable_from<LHS, RHS>;

	// https://en.cppreference.com/w/cpp/concepts/swappable
	template <typename T>
	concept Swappable = std::swappable<T>;

	// https://en.cppreference.com/w/cpp/concepts/swappable_with
	template <typename T, typename U>
	concept SwappableWith = std::swappable_with<T, U>;

	// https://en.cppreference.com/w/cpp/concepts/destructible
	template <typename T>
	concept Destructible = std::destructible<T>;

	// https://en.cppreference.com/w/cpp/concepts/constructible_from
	template <typename T, typename... Args>
	concept ConstructibleFrom = std::constructible_from<T, Args...>;

	// https://en.cppreference.com/w/cpp/concepts/default_initializable
	template <typename T>
	concept DefaultInitializable = std::default_initializable<T>;

	// https://en.cppreference.com/w/cpp/concepts/move_constructible
	template <typename T>
	concept MoveConstructible = std::move_constructible<T>;

	// https://en.cppreference.com/w/cpp/concepts/copy_constructible
	template <typename T>
	concept CopyConstructible = std::copy_constructible<T>;

	// https://en.cppreference.com/w/cpp/concepts/equality_comparable
	template <typename T>
	concept EqualityComparable = std::equality_comparable<T>;

	// https://en.cppreference.com/w/cpp/concepts/equality_comparable_with
	template <typename T, typename U>
	concept EqualityComparableWith = std::equality_comparable_with<T, U>;

	// https://en.cppreference.com/w/cpp/concepts/totally_ordered
	template <typename T>
	concept TotallyOrdered = std::totally_ordered<T>;

	// https://en.cppreference.com/w/cpp/concepts/totally_ordered_with
	template <typename T, typename U>
	concept TotallyOrderedWith = std::totally_ordered_with<T, U>;

	// https://en.cppreference.com/w/cpp/concepts/movable
	template <typename T>
	concept Movable = std::movable<T>;

	// https://en.cppreference.com/w/cpp/concepts/copyable
	template <typename T>
	concept Copyable = std::copyable<T>;

	// https://en.cppreference.com/w/cpp/concepts/semiregular
	template <typename T>
	concept Semiregular = std::semiregular<T>;

	// https://en.cppreference.com/w/cpp/concepts/regular
	template <typename T>
	concept Regular = std::regular<T>;

	// https://en.cppreference.com/w/cpp/concepts/invocable
	template <typename F, typename... Args>
	concept Invocable = std::invocable<F, Args...>;

	// https://en.cppreference.com/w/cpp/concepts/regular_invocable
	template <typename F, typename... Args>
	concept RegularInvocable = std::regular_invocable<F, Args...>;

	// https://en.cppreference.com/w/cpp/concepts/predicate
	template <typename F, typename... Args>
	concept Predicate = std::predicate<F, Args...>;

	// https://en.cppreference.com/w/cpp/concepts/relation
	template <typename R, typename T, typename U>
	concept Relation = std::relation<R, T, U>;

	// https://en.cppreference.com/w/cpp/concepts/equivalence_relation
	template <typename R, typename T, typename U>
	concept EquivalenceRelation = std::equivalence_relation<R, T, U>;

	// https://en.cppreference.com/w/cpp/concepts/strict_weak_order
	template <typename R, typename T, typename U>
	concept StrictWeakOrder = std::strict_weak_order<R, T, U>;
} // namespace Grizzly::Core
