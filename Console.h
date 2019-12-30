#pragma once

#include <iostream>
#include <string>
#include <type_traits>


struct RGB {
	enum {
		Black, Red, Green, Yellow, Blue, Magenta, Cyan, White, Reset
	};
	unsigned short int r, g, b;
};


template <typename T, 
	typename = std::enable_if<std::is_same_v<T, RGB> or std::is_same_v<T, decltype(RGB::Black)>>>
class TextColor {
public:
	T color;
	TextColor (T color) : color(color){}
};

std::ostream& operator << (std::ostream& out, const TextColor<RGB> Tcolor) {
	std::cout << "\033[38;2" <<
		";" << std::to_string(Tcolor.color.r) <<
		";" << std::to_string(Tcolor.color.g) <<
		";" << std::to_string(Tcolor.color.b) << "m";
	return out;
}

std::ostream& operator << (std::ostream& out, const TextColor<decltype(RGB::Black)>& Tcolor) {
	const std::string colorID = std::to_string(static_cast<int>(Tcolor.color));
	std::cout << "\033[3" << colorID << "m";
	return out;
}

