#include <iostream>

#include "ConsoleColors.h"

using namespace std;

int main() {
	std::cout << "\033[11";
	std::cout << TextProperty::Undeline;
	std::cout << BackgroundColor{ Color::White };
	std::cout << TextColor{ Color::Red } << "texto en rojo";
	std::cout << TextProperty::UndelineOff;
}