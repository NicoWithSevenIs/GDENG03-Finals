#pragma once

template<typename T>
class Singleton {

	protected:
		Singleton() = default;
		~Singleton() = default;

		Singleton(const Singleton& other) = delete;
		Singleton& operator=(const Singleton&) = delete;

	public:
		static T& get() {
			static T instance;
			return instance;
		}
};
