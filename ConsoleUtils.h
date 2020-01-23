#pragma once

#include <iostream>
#include <string>
#include <type_traits>
#include <windows.h>

#include <cstdio>

enum class TextProperty
{
	Reset = 0,
	Underline = 4,
	UndelineOff = 24
};

std::ostream& operator << (std::ostream& out, TextProperty prop) {
	const std::string colorID = std::to_string(static_cast<int>(prop));
	std::cout << "\033[" << colorID << "m";
	return out;
}

struct Color {
	enum {
		Black, Blue, Green, LightBlue, Red, Purple, Gold, Silver, Gray, WeirdBlue, LightGreen, Cyan, Salmon, Magenta, Yellow, White
	};
	unsigned short int r, g, b;
};

template <typename T, 
	typename = std::enable_if<std::is_same_v<T, Color> or std::is_same_v<T, decltype(Color::Black)>>>
class TextColor {
public:
	T color;
	TextColor (T color) : color(color){}
};

std::ostream& operator << (std::ostream& out, const TextColor<Color> Tcolor) {
	std::cout << "\033[38;2" <<
		";" << std::to_string(Tcolor.color.r) <<
		";" << std::to_string(Tcolor.color.g) <<
		";" << std::to_string(Tcolor.color.b) << "m";
	return out;
}

std::ostream& operator << (std::ostream& out, const TextColor<decltype(Color::Black)>& Tcolor) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, static_cast<int>(Tcolor.color));

	return out;
}

template <typename T,
	typename = std::enable_if<std::is_same_v<T, Color> or std::is_same_v<T, decltype(Color::Black)>>>
	class BackgroundColor {
	public:
		T color;
		BackgroundColor(T color) : color(color) {}
};

std::ostream& operator << (std::ostream& out, const BackgroundColor<Color> Tcolor) {
	std::cout << "\033[48;2" <<
		";" << std::to_string(Tcolor.color.r) <<
		";" << std::to_string(Tcolor.color.g) <<
		";" << std::to_string(Tcolor.color.b) << "m";
	return out;
}

std::ostream& operator << (std::ostream& out, const BackgroundColor<decltype(Color::Black)>& Tcolor) {
	const std::string colorID = std::to_string(static_cast<int>(Tcolor.color));
	std::cout << "\033[4" << colorID << "m";
	return out;
}

struct {} cls;

std::ostream& operator <<( std::ostream& out, const decltype(cls)) {
	std::system("cls");
	return out;
}

