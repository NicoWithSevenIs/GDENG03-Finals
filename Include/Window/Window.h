#pragma once
#include <Windows.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class Window {

	protected:
		HWND m_hwnd;
		bool m_is_running;

	public:
		Window();
		~Window();

		bool init();
		bool broadcast();
		bool release();
		bool isRunning();

		RECT getClientWindowRect();
		void setHWND(HWND hwnd);

		virtual void OnCreate();
		virtual void OnUpdate();
		virtual void OnDestroy();
		virtual void OnFocus();
		virtual void OnKillFocus();

};