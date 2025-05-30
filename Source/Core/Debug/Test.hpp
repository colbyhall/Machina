/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

// Check if MACH_ENABLE_TEST is defined to determine if we should compile the test code.
#ifndef MACH_ENABLE_TEST
	#define MACH_ENABLE_TEST 0
#else
	#define MACH_ENABLE_TEST 1
#endif

#if !MACH_ENABLE_TEST
	#define DOCTEST_CONFIG_DISABLE
#endif

// Remove non doctest macros
#define DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES
#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <Doctest/Doctest.h>

// Move the DOCTEST macros to MACH_ prefixed macros
#define MACH_TEST_CASE(name)							 DOCTEST_TEST_CASE(name)
#define MACH_TEST_CASE_CLASS(name)						 DOCTEST_TEST_CASE_CLASS(name)
#define MACH_TEST_CASE_FIXTURE(x, name)					 DOCTEST_TEST_CASE_FIXTURE(x, name)
#define MACH_TYPE_TO_STRING_AS(str, ...)				 DOCTEST_TYPE_TO_STRING_AS(str, __VA_ARGS__)
#define MACH_TYPE_TO_STRING(...)						 DOCTEST_TYPE_TO_STRING(__VA_ARGS__)
#define MACH_TEST_CASE_TEMPLATE(name, T, ...)			 DOCTEST_TEST_CASE_TEMPLATE(name, T, __VA_ARGS__)
#define MACH_TEST_CASE_TEMPLATE_DEFINE(name, T, id)		 DOCTEST_TEST_CASE_TEMPLATE_DEFINE(name, T, id)
#define MACH_TEST_CASE_TEMPLATE_INVOKE(id, ...)			 DOCTEST_TEST_CASE_TEMPLATE_INVOKE(id, __VA_ARGS__)
#define MACH_TEST_CASE_TEMPLATE_APPLY(id, ...)			 DOCTEST_TEST_CASE_TEMPLATE_APPLY(id, __VA_ARGS__)
#define MACH_SUBCASE(name)								 DOCTEST_SUBCASE(name)
#define MACH_TEST_SUITE(decorators)						 DOCTEST_TEST_SUITE(decorators)
#define MACH_TEST_SUITE_BEGIN(name)						 DOCTEST_TEST_SUITE_BEGIN(name)
#define MACH_TEST_SUITE_END								 DOCTEST_TEST_SUITE_END
#define MACH_REGISTER_EXCEPTION_TRANSLATOR(signature)	 DOCTEST_REGISTER_EXCEPTION_TRANSLATOR(signature)
#define MACH_REGISTER_REPORTER(name, priority, reporter) DOCTEST_REGISTER_REPORTER(name, priority, reporter)
#define MACH_REGISTER_LISTENER(name, priority, reporter) DOCTEST_REGISTER_LISTENER(name, priority, reporter)
#define MACH_INFO(...)									 DOCTEST_INFO(__VA_ARGS__)
#define MACH_CAPTURE(x)									 DOCTEST_CAPTURE(x)
#define MACH_ADD_MESSAGE_AT(file, line, ...)			 DOCTEST_ADD_MESSAGE_AT(file, line, __VA_ARGS__)
#define MACH_ADD_FAIL_CHECK_AT(file, line, ...)			 DOCTEST_ADD_FAIL_CHECK_AT(file, line, __VA_ARGS__)
#define MACH_ADD_FAIL_AT(file, line, ...)				 DOCTEST_ADD_FAIL_AT(file, line, __VA_ARGS__)
#define MACH_MESSAGE(...)								 DOCTEST_MESSAGE(__VA_ARGS__)
#define MACH_FAIL_CHECK(...)							 DOCTEST_FAIL_CHECK(__VA_ARGS__)
#define MACH_FAIL(...)									 DOCTEST_FAIL(__VA_ARGS__)
#define MACH_TO_LVALUE(...)								 DOCTEST_TO_LVALUE(__VA_ARGS__)

#define MACH_WARN(...)								 DOCTEST_WARN(__VA_ARGS__)
#define MACH_WARN_FALSE(...)						 DOCTEST_WARN_FALSE(__VA_ARGS__)
#define MACH_WARN_THROWS(...)						 DOCTEST_WARN_THROWS(__VA_ARGS__)
#define MACH_WARN_THROWS_AS(expr, ...)				 DOCTEST_WARN_THROWS_AS(expr, __VA_ARGS__)
#define MACH_WARN_THROWS_WITH(expr, ...)			 DOCTEST_WARN_THROWS_WITH(expr, __VA_ARGS__)
#define MACH_WARN_THROWS_WITH_AS(expr, with, ...)	 DOCTEST_WARN_THROWS_WITH_AS(expr, with, __VA_ARGS__)
#define MACH_WARN_NOTHROW(...)						 DOCTEST_WARN_NOTHROW(__VA_ARGS__)
#define MACH_CHECK(...)								 DOCTEST_CHECK(__VA_ARGS__)
#define MACH_CHECK_FALSE(...)						 DOCTEST_CHECK_FALSE(__VA_ARGS__)
#define MACH_CHECK_THROWS(...)						 DOCTEST_CHECK_THROWS(__VA_ARGS__)
#define MACH_CHECK_THROWS_AS(expr, ...)				 DOCTEST_CHECK_THROWS_AS(expr, __VA_ARGS__)
#define MACH_CHECK_THROWS_WITH(expr, ...)			 DOCTEST_CHECK_THROWS_WITH(expr, __VA_ARGS__)
#define MACH_CHECK_THROWS_WITH_AS(expr, with, ...)	 DOCTEST_CHECK_THROWS_WITH_AS(expr, with, __VA_ARGS__)
#define MACH_CHECK_NOTHROW(...)						 DOCTEST_CHECK_NOTHROW(__VA_ARGS__)
#define MACH_REQUIRE(...)							 DOCTEST_REQUIRE(__VA_ARGS__)
#define MACH_REQUIRE_FALSE(...)						 DOCTEST_REQUIRE_FALSE(__VA_ARGS__)
#define MACH_REQUIRE_THROWS(...)					 DOCTEST_REQUIRE_THROWS(__VA_ARGS__)
#define MACH_REQUIRE_THROWS_AS(expr, ...)			 DOCTEST_REQUIRE_THROWS_AS(expr, __VA_ARGS__)
#define MACH_REQUIRE_THROWS_WITH(expr, ...)			 DOCTEST_REQUIRE_THROWS_WITH(expr, __VA_ARGS__)
#define MACH_REQUIRE_THROWS_WITH_AS(expr, with, ...) DOCTEST_REQUIRE_THROWS_WITH_AS(expr, with, __VA_ARGS__)
#define MACH_REQUIRE_NOTHROW(...)					 DOCTEST_REQUIRE_NOTHROW(__VA_ARGS__)

#define MACH_WARN_MESSAGE(cond, ...)				   DOCTEST_WARN_MESSAGE(cond, __VA_ARGS__)
#define MACH_WARN_FALSE_MESSAGE(cond, ...)			   DOCTEST_WARN_FALSE_MESSAGE(cond, __VA_ARGS__)
#define MACH_WARN_THROWS_MESSAGE(expr, ...)			   DOCTEST_WARN_THROWS_MESSAGE(expr, __VA_ARGS__)
#define MACH_WARN_THROWS_AS_MESSAGE(expr, ex, ...)	   DOCTEST_WARN_THROWS_AS_MESSAGE(expr, ex, __VA_ARGS__)
#define MACH_WARN_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_WARN_THROWS_WITH_MESSAGE(expr, with, __VA_ARGS__)
#define MACH_WARN_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...)                                                          \
	DOCTEST_WARN_THROWS_WITH_AS_MESSAGE(expr, with, ex, __VA_ARGS__)
#define MACH_WARN_NOTHROW_MESSAGE(expr, ...)			DOCTEST_WARN_NOTHROW_MESSAGE(expr, __VA_ARGS__)
#define MACH_CHECK_MESSAGE(cond, ...)					DOCTEST_CHECK_MESSAGE(cond, __VA_ARGS__)
#define MACH_CHECK_FALSE_MESSAGE(cond, ...)				DOCTEST_CHECK_FALSE_MESSAGE(cond, __VA_ARGS__)
#define MACH_CHECK_THROWS_MESSAGE(expr, ...)			DOCTEST_CHECK_THROWS_MESSAGE(expr, __VA_ARGS__)
#define MACH_CHECK_THROWS_AS_MESSAGE(expr, ex, ...)		DOCTEST_CHECK_THROWS_AS_MESSAGE(expr, ex, __VA_ARGS__)
#define MACH_CHECK_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_CHECK_THROWS_WITH_MESSAGE(expr, with, __VA_ARGS__)
#define MACH_CHECK_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...)                                                         \
	DOCTEST_CHECK_THROWS_WITH_AS_MESSAGE(expr, with, ex, __VA_ARGS__)
#define MACH_CHECK_NOTHROW_MESSAGE(expr, ...)			  DOCTEST_CHECK_NOTHROW_MESSAGE(expr, __VA_ARGS__)
#define MACH_REQUIRE_MESSAGE(cond, ...)					  DOCTEST_REQUIRE_MESSAGE(cond, __VA_ARGS__)
#define MACH_REQUIRE_FALSE_MESSAGE(cond, ...)			  DOCTEST_REQUIRE_FALSE_MESSAGE(cond, __VA_ARGS__)
#define MACH_REQUIRE_THROWS_MESSAGE(expr, ...)			  DOCTEST_REQUIRE_THROWS_MESSAGE(expr, __VA_ARGS__)
#define MACH_REQUIRE_THROWS_AS_MESSAGE(expr, ex, ...)	  DOCTEST_REQUIRE_THROWS_AS_MESSAGE(expr, ex, __VA_ARGS__)
#define MACH_REQUIRE_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_REQUIRE_THROWS_WITH_MESSAGE(expr, with, __VA_ARGS__)
#define MACH_REQUIRE_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...)                                                       \
	DOCTEST_REQUIRE_THROWS_WITH_AS_MESSAGE(expr, with, ex, __VA_ARGS__)
#define REQUIRE_NOTHROW_MESSAGE(expr, ...) DOCTEST_REQUIRE_NOTHROW_MESSAGE(expr, __VA_ARGS__)

#define MACH_SCENARIO(name)						   DOCTEST_SCENARIO(name)
#define MACH_SCENARIO_CLASS(name)				   DOCTEST_SCENARIO_CLASS(name)
#define MACH_SCENARIO_TEMPLATE(name, T, ...)	   DOCTEST_SCENARIO_TEMPLATE(name, T, __VA_ARGS__)
#define MACH_SCENARIO_TEMPLATE_DEFINE(name, T, id) DOCTEST_SCENARIO_TEMPLATE_DEFINE(name, T, id)
#define MACH_GIVEN(name)						   DOCTEST_GIVEN(name)
#define MACH_WHEN(name)							   DOCTEST_WHEN(name)
#define MACH_AND_WHEN(name)						   DOCTEST_AND_WHEN(name)
#define MACH_THEN(name)							   DOCTEST_THEN(name)
#define MACH_AND_THEN(name)						   DOCTEST_AND_THEN(name)

#define MACH_WARN_EQ(...)			  DOCTEST_WARN_EQ(__VA_ARGS__)
#define MACH_CHECK_EQ(...)			  DOCTEST_CHECK_EQ(__VA_ARGS__)
#define MACH_REQUIRE_EQ(...)		  DOCTEST_REQUIRE_EQ(__VA_ARGS__)
#define MACH_WARN_NE(...)			  DOCTEST_WARN_NE(__VA_ARGS__)
#define MACH_CHECK_NE(...)			  DOCTEST_CHECK_NE(__VA_ARGS__)
#define MACH_REQUIRE_NE(...)		  DOCTEST_REQUIRE_NE(__VA_ARGS__)
#define MACH_WARN_GT(...)			  DOCTEST_WARN_GT(__VA_ARGS__)
#define MACH_CHECK_GT(...)			  DOCTEST_CHECK_GT(__VA_ARGS__)
#define MACH_REQUIRE_GT(...)		  DOCTEST_REQUIRE_GT(__VA_ARGS__)
#define MACH_WARN_LT(...)			  DOCTEST_WARN_LT(__VA_ARGS__)
#define MACH_CHECK_LT(...)			  DOCTEST_CHECK_LT(__VA_ARGS__)
#define MACH_REQUIRE_LT(...)		  DOCTEST_REQUIRE_LT(__VA_ARGS__)
#define MACH_WARN_GE(...)			  DOCTEST_WARN_GE(__VA_ARGS__)
#define MACH_CHECK_GE(...)			  DOCTEST_CHECK_GE(__VA_ARGS__)
#define MACH_REQUIRE_GE(...)		  DOCTEST_REQUIRE_GE(__VA_ARGS__)
#define MACH_WARN_LE(...)			  DOCTEST_WARN_LE(__VA_ARGS__)
#define MACH_CHECK_LE(...)			  DOCTEST_CHECK_LE(__VA_ARGS__)
#define MACH_REQUIRE_LE(...)		  DOCTEST_REQUIRE_LE(__VA_ARGS__)
#define MACH_WARN_UNARY(...)		  DOCTEST_WARN_UNARY(__VA_ARGS__)
#define MACH_CHECK_UNARY(...)		  DOCTEST_CHECK_UNARY(__VA_ARGS__)
#define MACH_REQUIRE_UNARY(...)		  DOCTEST_REQUIRE_UNARY(__VA_ARGS__)
#define MACH_WARN_UNARY_FALSE(...)	  DOCTEST_WARN_UNARY_FALSE(__VA_ARGS__)
#define MACH_CHECK_UNARY_FALSE(...)	  DOCTEST_CHECK_UNARY_FALSE(__VA_ARGS__)
#define MACH_REQUIRE_UNARY_FALSE(...) DOCTEST_REQUIRE_UNARY_FALSE(__VA_ARGS__)
