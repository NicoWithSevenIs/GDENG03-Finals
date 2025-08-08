#include "UI/UIManager.h"

void UIManager::initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, device_context);

	RECT rc;
	::GetClientRect(hwnd, &rc);

	get().width = rc.right - rc.left;
	get().height = rc.bottom - rc.top;

	auto hierarchy = new HierarchyScreen(get().width, get().height);
	hierarchy->m_screen = &get();
	UIManager::addUIScreen("hierarchy", hierarchy);

	auto inspector = new InspectorScreen(get().width, get().height);
	inspector->m_screen = &get();
	UIManager::addUIScreen("inspector", inspector);

	auto menu = new MenuBarUI(get().width, get().height);
	menu->m_screen = &get();
	UIManager::addUIScreen("menu", menu);

	auto scene_state = new SceneStateUI(get().width, get().height);
	scene_state->m_screen = &get();
	UIManager::addUIScreen("scene state", scene_state);

	auto debug_window = new DebugWindow(get().width, get().height);
	debug_window->m_screen = &get();
	UIManager::addUIScreen("debug", debug_window);

}

void UIManager::draw()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (auto i : get().map_ui_screens) {
		i.second->draw();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::addUIScreen(std::string name, UIScreen* screen)
{
	get().map_ui_screens[name] = screen;
}

void UIManager::Notify(std::string notification, UIScreen* notifier)
{



}

UIScreen* UIManager::Retrieve(std::string name)
{
	return get().map_ui_screens.contains(name) ? get().map_ui_screens[name] : nullptr;
}
