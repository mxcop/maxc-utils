#pragma once

#include <string>

#include "string/strfmt.h"

/**
 * @brief Project standard error datatype.
 */
struct error {
	std::string msg;
	std::string loc;

	error(std::string msg, const char* file, const char* func, const int line) : msg(msg), loc(strfmt("%s:%s(...):%i", file, func, line)) {

	}
};
