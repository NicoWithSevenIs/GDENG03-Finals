#pragma once

#include <string>

template<typename T>
class IMediator {
	public:
		virtual void Notify(std::string notification, T* notifier) = 0;
		virtual T* Retrieve(std::string name) = 0;
};