#pragma once
#include "Input/InputListener.h"
#include "Math/Point.h"


#include <unordered_set>
class InputSystem {
	
	private:
		std::unordered_set<InputListener*> m_set_listeners;

		unsigned char m_keys_state[256] = {};
		unsigned char m_old_keys_state[256] = {};

		Point m_old_mouse_pos;

	public:
		bool first = true;
		bool cursorShow = false;

	public:
		InputSystem();
		~InputSystem();

		void AddListener(InputListener* listener);
		void RemoveListener(InputListener* listener);
		void Update();
		void ShowCursor(bool value);
		void SetCursorPos(const Point& point);

	public:
		static InputSystem* get();

};