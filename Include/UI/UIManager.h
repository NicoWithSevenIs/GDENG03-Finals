#pragma once

#include <Windows.h>
#include <vector>
#include <unordered_map>

#include "Interfaces/Singleton.h"
#include "Interfaces/IMediator.h"

#include "UI/HierarchyScreen.h"
#include "UI/InspectorScreen.h"
#include "UI/MenuBarUI.h"
#include "UI/SceneStateUI.h"
#include "UI/DebugWindow.h"

class UIManager : public Singleton<UIManager>, public IMediator<UIScreen>
{
	public:
		static void initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
		static void draw();

	public:
		static void addUIScreen(std::string name, UIScreen* screen);
		void Notify(std::string notification, UIScreen* notifier) override;
		UIScreen* Retrieve(std::string name) override;

	private:
		std::unordered_map<std::string, UIScreen*> map_ui_screens;

		float width;
		float height;
};

