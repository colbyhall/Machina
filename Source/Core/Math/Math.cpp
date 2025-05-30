/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#include <Core/Math/Math.hpp>
#include <cmath>

namespace Mach::Core::Math {
	f32 cos(f32 x) { return std::cos(x); }
	f64 cos(f64 x) { return std::cos(x); }

	f32 sin(f32 x) { return std::sin(x); }
	f64 sin(f64 x) { return std::sin(x); }

	f32 tan(f32 x) { return std::tan(x); }
	f64 tan(f64 x) { return std::tan(x); }

	f32 acos(f32 x) { return std::acos(x); }
	f64 acos(f64 x) { return std::acos(x); }

	f32 asin(f32 x) { return std::asin(x); }
	f64 asin(f64 x) { return std::asin(x); }

	f32 atan(f32 x) { return std::atan(x); }
	f64 atan(f64 x) { return std::atan(x); }

	f32 atan2(f32 y, f32 x) { return std::atan2(y, x); }
	f64 atan2(f64 y, f64 x) { return std::atan2(y, x); }

	f32 sqrt(f32 x) { return std::sqrt(x); }
	f64 sqrt(f64 x) { return std::sqrt(x); }

	f32 fmod(f32 numerator, f32 denominator) { return std::fmod(numerator, denominator); }
	f64 fmod(f64 numerator, f64 denominator) { return std::fmod(numerator, denominator); }

	f32 powf(f32 x, f32 y) { return std::powf(x, y); }
	f64 pow(f64 x, f64 y) { return std::pow(x, y); }
} // namespace Mach::Core::Math

#include <Core/Debug/Test.hpp>

MACH_TEST_SUITE("Math") {
	using namespace Mach;

	MACH_TEST_CASE("min") {
		MACH_CHECK(Math::min(0.f, 1.f) == 0.f);
		MACH_CHECK(Math::min(1.f, 0.f) == 0.f);
	}

	MACH_TEST_CASE("max") {
		MACH_CHECK(Math::max(0.f, 1.f) == 1.f);
		MACH_CHECK(Math::max(1.f, 0.f) == 1.f);
	}

	MACH_TEST_CASE("clamp") {
		MACH_CHECK(Math::clamp(-5.f, 0.f, 1.f) == 0.f);
		MACH_CHECK(Math::clamp(5.f, 0.f, 1.f) == 1.f);
		MACH_CHECK(Math::clamp(0.5f, 0.f, 1.f) == 0.5f);
	}

	MACH_TEST_CASE("abs") {
		MACH_CHECK(Math::abs(-1.f) == 1.f);
		MACH_CHECK(Math::abs(1.f) == 1.f);
	}

	MACH_TEST_CASE("lerp") {
		MACH_CHECK(Math::lerp(0.f, 1.f, 0.5f) == .5f);
		MACH_CHECK(Math::lerp(0.f, 10.f, 0.5f) == 5.f);
	}

	MACH_TEST_CASE("square") {
		MACH_CHECK(Math::square(1.f) == 1.f);
		MACH_CHECK(Math::square(2.f) == 4.f);
		MACH_CHECK(Math::square(3.f) == 9.f);
	}

	MACH_TEST_CASE("equals") {
		MACH_CHECK(Math::equals(1.f, 1.f));
		MACH_CHECK(Math::equals(2.f, 2.f));
	}
}
