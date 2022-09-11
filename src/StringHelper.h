#pragma once

#include <vector>
#include <string>
#include <charconv>

/**
	* Splits the given string_view along the character del
	* @param s - string_view to split
	* @param del - split character
	*/
inline std::vector<std::string_view> split(std::string_view s, char del) {

	std::vector<std::string_view> result;
	result.reserve(4);
	int start = 0;
	int end = s.find(del);

	while (end != -1) {
		result.emplace_back(s.substr(start, end - start));
		start = end + 1;
		end = s.find(del, start);
	}

	result.emplace_back(s.substr(start, end - start));
	return result;
}