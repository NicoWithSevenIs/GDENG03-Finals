#pragma once
#include "string";

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui.h"

#include "Interfaces/IMediator.h"

class UIScreen 
{
	public:
		virtual void draw() = 0;

	public:
		bool isOpen;
		float height;
		float width;

	public:
		std::string m_name;
		IMediator<UIScreen>* m_screen = nullptr;

};