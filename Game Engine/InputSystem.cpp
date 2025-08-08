#include "Input/InputSystem.h"
#include <Windows.h>

InputSystem::InputSystem()
{
	
}

InputSystem::~InputSystem()
{
}

void InputSystem::AddListener(InputListener* listener)
{
	m_set_listeners.insert(listener);
}

void InputSystem::RemoveListener(InputListener* listener)
{
	std::unordered_set<InputListener*>::iterator it = m_set_listeners.find(listener);
	if(it != m_set_listeners.end())
		m_set_listeners.erase(it);
}

void InputSystem::Update()
{

	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	Point mousePos = Point(current_mouse_pos.x, current_mouse_pos.y);

	if (first) {
		m_old_mouse_pos = mousePos;
		first = false;
	}

	this->ShowCursor(true);
	//else {
	//	this->ShowCursor(false);
	//	mousePos = Point(0, 0);
	//}

	if (current_mouse_pos.x != m_old_mouse_pos.m_x || current_mouse_pos.y != m_old_mouse_pos.m_y) {

		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end()) {
			(*it)->onMouseMove(Point(mousePos.m_x - m_old_mouse_pos.m_x, mousePos.m_y - m_old_mouse_pos.m_y), mousePos);
			++it;
		}
	}

	m_old_mouse_pos = mousePos;

	if (::GetKeyboardState(m_keys_state)) {

		for (unsigned int i = 0; i < 256; i++) {

			if (m_keys_state[i] & 0x80) {

				std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

				while (it != m_set_listeners.end()) {

					switch (i) {
						case VK_LBUTTON: 
							if (m_keys_state[i] != m_old_keys_state[i]) (*it)->onLeftMouseDown(mousePos);
							break;
						case VK_RBUTTON: 
							if (m_keys_state[i] != m_old_keys_state[i]) (*it)->onRightMouseDown(mousePos);
							break;
						default:
							(*it)->onKeyDown(i);
					}

					++it;
				}

			}
			else {

				if (m_keys_state[i] != m_old_keys_state[i]) {

					std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

					while (it != m_set_listeners.end()) {

						switch (i) {
							case VK_LBUTTON:
								if (m_keys_state[i] != m_old_keys_state[i]) (*it)->onLeftMouseUp(mousePos);
								break;
							case MK_RBUTTON:
								if (m_keys_state[i] != m_old_keys_state[i]) (*it)->onRightMouseUp(mousePos);
								break;
							default:
								(*it)->onKeyUp(i);
						}

						
						++it;
					}
				}

			}
		}

		::memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char) * 256);

	}
}

void InputSystem::ShowCursor(bool value)
{
	::ShowCursor(value);
}

void InputSystem::SetCursorPos(const Point& point)
{
	::SetCursorPos(point.m_x, point.m_y);
}

InputSystem* InputSystem::get()
{
	static InputSystem system;
	return &system;
}
