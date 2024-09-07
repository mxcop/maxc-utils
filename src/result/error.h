#pragma once

#include <string>

#include "string/strfmt.h"

namespace mxc {

/**
 * @brief Error origin trace info.
 */
struct ErrorTrace {
    const size_t line_num = 0;
    const char* file_name = nullptr;

    ErrorTrace() = default;
    ErrorTrace(const size_t line_num, const char* file_name) : line_num(line_num), file_name(file_name) {}
};

/**
 * @brief Project standard error datatype.
 */
struct Error {
	const char* info;
    ErrorTrace trace;

	Error(const char* msg, ErrorTrace trace = {}) : info(msg), trace(trace) {}
};

/** Optional trace for errors. */
#define TRACE mxc::ErrorTrace(__LINE__, __FILE__)

}
