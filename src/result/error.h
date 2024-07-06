#pragma once

#include <string>

#include "string/strfmt.h"

/**
 * @brief Project standard error datatype.
 */
struct error {
	const char* info;
	//std::string msg;
	//std::string loc;

	error(const char* msg) : info(msg) {}

	//error(std::string msg, const char* file, const char* func, const int line) {
	//	std::string r = strfmt("%s [%s:%s(...):%i]", msg.data(), file, func, line);
	//	info = (char*)malloc(r.length());
	//	memcpy(info, r.data(), r.length());
	//}
};

//#define ERR(MSG) (error(MSG, __FILE__, __func__, __LINE__))
