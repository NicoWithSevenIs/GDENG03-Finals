#pragma once
#include <vector>
#include <functional>

namespace Utilities {

	template<typename T>
	std::vector<T> Where(std::vector<T> vec, std::function<bool(T)> predicate) {
		std::vector<T> v;
		for (auto i : vec) {
			if (predicate(i))
				v.push_back(i);
		}
		return v;
	}
	template<typename T>
	T Select(std::vector<T> vec, std::function<bool(T)> predicate) {
		T selected{};
		for (auto i : vec) {
			if (predicate(i)) {
				selected = i;
				break;
			}
			
		}
		return selected;
	}
}