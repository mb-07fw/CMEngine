#pragma once

#include <source_location>
#include <exception>

#include "Log.hpp"

namespace Platform
{
	[[noreturn]] inline void RuntimeFailure(
		std::string_view condition,
		std::string_view msg,
		ILogger* pLoggerOpt = nullptr,
		std::source_location loc = std::source_location::current()
	)
	{
		constexpr std::string_view MsgFmt = "Runtime Failure: [{}:{}:{}] `{}` => \'{}\'";

		const char* fileName = loc.file_name();
		uint_least32_t line = loc.line();
		const char* functionName = loc.function_name();

		std::string formatted = std::vformat(
			MsgFmt,
			std::make_format_args(
				fileName,
				line,
				functionName,
				condition,
				msg
			));

		if (pLoggerOpt)
			pLoggerOpt->LogFatal(formatted.c_str());

		std::cerr << formatted.c_str() << '\n';

		__debugbreak();

		std::terminate();
	}

	template <typename... Args>
	[[noreturn]] inline void RuntimeFailureV(
		std::string_view condition,
		std::source_location loc,
		ILogger* pLoggerOpt,
		std::string_view fmt,
		Args&&... args
	)
	{
		std::string formatted = std::vformat(
			fmt,
			std::make_format_args(
				std::forward<Args>(args)...
			)
		);

		RuntimeFailure(condition, formatted, pLoggerOpt, loc);
	}

	inline void RuntimeFailureIf(
		bool condition,
		std::string_view conditionStr,
		std::string_view msg,
		ILogger* pLoggerOpt,
		std::source_location loc = std::source_location::current()
	)
	{
		if (condition)
			RuntimeFailure(conditionStr, msg, pLoggerOpt, loc);
	}

	template <typename... Args>
	inline void RuntimeFailureIfV(
		bool condition,
		std::string_view conditionStr,
		std::source_location loc,
		ILogger* pLoggerOpt,
		std::string_view fmt,
		Args&&... args
	)
	{
		if (condition)
			RuntimeFailureV(conditionStr, loc, pLoggerOpt, fmt, std::forward<Args>(args)...);
	}
}

#define LOC_CURRENT() std::source_location::current()

// `...` is used to optionally provide a ILogger instance.
#define RUNTIME_FAILURE(msg, ...)    				  RuntimeFailure("false", msg, __VA_ARGS__)

#define RUNTIME_FAILURE_V(pLoggerOpt, fmt, ...)       RuntimeFailureV("false", LOC_CURRENT(), pLoggerOpt, fmt, __VA_ARGS__)
#define RUNTIME_FAILURE_IF(x, msg, pLoggerOpt)		  RuntimeFailureIf(x, #x, msg, pLoggerOpt)
#define RUNTIME_FAILURE_IF_V(x, pLoggerOpt, fmt, ...) RuntimeFailureIfV(x, #x, LOC_CURRENT(), pLoggerOpt, fmt, __VA_ARGS__)