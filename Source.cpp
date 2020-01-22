#include <iostream>
#include <vector>
#include <cstdarg>
#include <functional>
#include <algorithm>
#include <tuple>

#include "Ranges.h"


//#include "ConsoleUtils.h"

//using namespace std;

template <typename T>
struct filterBase {
	using value_type = T;
	using container_type = typename T::value_type;
	typedef std::function<bool(container_type&)> predFnc;

	T& container;
	predFnc pred;

	filterBase(T&container, predFnc pred) 
		: container(container), pred(pred) {}

	class iterator {
	public:
		typename T::iterator it;
		typename T::iterator end;
		predFnc pred;
		
		bool operator == (const iterator& filterIt) {
			return it == filterIt.it;
		}
		bool operator != (const iterator& filterIt) {
			return it != filterIt.it;
		}

		void operator++ () {
			for (++it; it != end && !pred(*it); ++it);
		}

		auto operator*() {
			return *it;
		}
	};

	class reverse_iterator : public iterator {
	public:
		typedef typename T::reverse_iterator rIterator;
		rIterator it;
		rIterator end;
		predFnc pred;

		reverse_iterator(rIterator it, rIterator end, predFnc pred)
			: it(it), end(end), pred(pred) {}
	};

	constexpr iterator begin() {
		auto it = container.begin();
		for (; it != container.end() && !pred(*it); ++it);
		return { it, container.end(), pred };
	}

	constexpr iterator end() {
		return { container.end(), container.end(), pred };
	}

	constexpr reverse_iterator rbegin(){
		auto it = container.rbegin();
		for (; it != container.rend() && !pred(*it); ++it);
		return { it, container.rend(), pred };
	}

	constexpr reverse_iterator rend() {
		return { container.rend(), container.rend(), pred };
	}
	
};

template <typename T, typename U, 
	typename = std::enable_if_t<!std::is_same_v<T, filterBase<typename T::value_type>>>>
auto filter(T& container, U pred) {
	return filterBase<T>(container, pred);
}

template <typename T, typename U,
	typename = std::enable_if_t<std::is_same_v<T, filterBase<typename T::value_type>>>>
auto filter(T&& container, U pred) {

	auto newPred = [=](typename T::container_type&val) -> bool {
		return container.pred(val) && pred(val);
	};
	return filterBase<typename T::value_type>(container.container, newPred);
}

int main() {

	std::vector<int> vals = { 1,2,3,4,5,6,7,8,9,100,200,300,400,401,403 };

	auto mult3 = [](int val) -> bool {
		return val % 3 == 0;
	};

	auto odd = [](int val) -> bool {
		return val % 2;
	};
	
	for (const auto& v : reversed(filter(filter(vals, mult3), odd))) {
		std::cout << v << " ";
	}

	//auto handle = GetStdHandle(STD_OUTPUT_HANDLE);


	/*std::cout << TextProperty::Underline;
	std::cout << BackgroundColor{ Color::White };
	std::cout << TextColor{ Color::White } << "texto en rojo";
	std::cout << TextProperty::Reset;
	std::cout << cls << "asd";
	std::cout << std::endl;*/


}

