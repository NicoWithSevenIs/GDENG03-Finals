//#define NOMINMAX
#include "ECS/Systems/PhysicsSystem.h"		
#include "Window/AppWindow.h"
#include <Windows.h>

#include "Math/Vector3D.h"
#include "Math/Matrix4x4.h"
#include "Input/InputSystem.h"

#include <iostream>
#include <utility>
#include <cstdlib>
#include <algorithm>
#include <DirectXMath.h>
#include <corecrt_math_defines.h>

#include "Game Engine/EngineTime.h"
//#include "ECS/Systems/PhysicsSystem.h"
#include "GameObject/ParentingManager.h"
#include "ECS/Systems/EntityManager.h"
#include "ECS/Systems/TimelineManager.h"

#include "ECS/Components/CubeRenderer.h"
#include "ECS/Systems/SceneStateManager.h"
#include "UI/UIManager.h"
#include "UI/DebugWindow.h"

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::OnCreate()
{
	Window::OnCreate();

	srand(time(0));

	GraphicsEngine::get()->init();
	this->m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();

	float width = rc.right - rc.left;
	float height = rc.bottom - rc.top;

	this->m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);
	
	editor_camera = new PerspectiveCamera(1.57f, width/height);
	editor_camera->m_transform.m_translation = Vector3D(0, 2, -3);

	player_camera = new PerspectiveCamera(1.57f, width / height);


	UIManager::initialize(this->m_hwnd, GraphicsEngine::get()->getDevice(), GraphicsEngine::get()->getImmediateDeviceContext()->getContext());
	EntityManager::Initialize();
	TimelineManager::get().CreateSnapshot();
	PhysicsSystem::Initialize();

	//Setting Callbacks
	InitializeSceneStateCallbacks();

	auto InitializePlayModeCamera = [this](SceneState state) {
		if (state == SceneState::PLAY) {
			player_camera->m_transform.m_translation = Vector3D();
			player_camera->m_transform.m_rotation = Vector3D();
		}
	};

	// Test the debug console
	std::cout << "GDENG03 Scene Editor initialized successfully!" << std::endl;
	std::cout << "Use Window menu -> Debug Console or press 'L' key to open debug window" << std::endl;
}

void AppWindow::InitializeSceneStateCallbacks()
{
	SceneStateManager::get().UpdateCallbacks[SceneState::EDIT] = [this]() {
		editor_camera->Update();

		EntityManager::ResetUpdatedFlags();
		EntityManager::Update(editor_camera->GetViewMatrix(), editor_camera->GetProjectionMatrix());
		EntityManager::Draw();

		UIManager::draw();

		if (TimelineManager::get().IsDirty()) {
			TimelineManager::get().CreateSnapshot();
		}
	};

	SceneStateManager::get().UpdateCallbacks[SceneState::PLAY] = [this]() {
		PhysicsSystem::UpdateAllPhysicsComponents();
		player_camera->Update();
		EntityManager::ResetUpdatedFlags();
		EntityManager::Update(player_camera->GetViewMatrix(), player_camera->GetProjectionMatrix());
		EntityManager::Draw();
	};

	SceneStateManager::get().UpdateCallbacks[SceneState::PAUSED] = [this]() {
		EntityManager::Draw();
	};

}


void AppWindow::OnUpdate()
{
	Window::OnUpdate();

	
	InputSystem::get()->Update();
	//InputSystem::get()->ShowCursor(false);

	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,  0, 0.3f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	float width = rc.right - rc.left;
	float height = rc.bottom - rc.top;

	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	SceneStateManager::get().Update();
	UIManager::draw();
	this->m_swap_chain->present(true);
}



void AppWindow::OnDestroy()
{
	Window::OnDestroy();
	this->m_swap_chain->release();

	EntityManager::Release();

	GraphicsEngine::get()->release();
}

void AppWindow::OnFocus()
{
	std::cout << "Gained Focus\n";
	InputSystem::get()->AddListener(this);
}

void AppWindow::OnKillFocus()
{
	std::cout << "Lost Focus\n";
	InputSystem::get()->RemoveListener(this);
	InputSystem::get()->first = true;
}

void AppWindow::onKeyDown(int key)
{
	switch (SceneStateManager::get().CurrentState()) {
	case SceneState::EDIT: 	editor_camera->OnKeyDown(key);  break;
	case SceneState::PLAY:  player_camera->OnKeyDown(key); break;
	}

	if(key == VK_LCONTROL)
		is_ctrl_held = true;

	if (key == VK_LMENU) {
		InputSystem::get()->cursorShow = true;
	}
	
}

void AppWindow::onKeyUp(int key)
{

	switch (SceneStateManager::get().CurrentState()) {
		case SceneState::EDIT: 	editor_camera->OnKeyUp(key);  break;
		case SceneState::PLAY:  player_camera->OnKeyUp(key); break;
	}


	if (key == '1') {
		toggle_camera_movement = false;
	}

	if (key == '2') {
		toggle_camera_movement = true;
	}


	if (key == VK_LCONTROL)
		is_ctrl_held = false;

	if (key == VK_LMENU) {
		InputSystem::get()->cursorShow = false;
	}

	if (is_ctrl_held && key == 'Z') {
		std::cout << "[INFO] Undo operation performed" << std::endl;
		TimelineManager::get().Undo();
	}

	if (is_ctrl_held && key == 'Y') {
		std::cout << "[INFO] Redo operation performed" << std::endl;
		TimelineManager::get().Redo();
	}

	// Toggle debug console with L key
	if (key == VK_RMENU) {
		auto debug_window = (DebugWindow*)UIManager::get().Retrieve("debug");
		if (debug_window) {
			debug_window->isOpen = !debug_window->isOpen;
		}
	}
}

void AppWindow::onMouseMove(const Point& delta_mouse_point,
	const Point& mouse_pos)
{
	if(toggle_camera_movement)
		switch (SceneStateManager::get().CurrentState()) {
			case SceneState::EDIT: 	editor_camera->OnMouseMove(delta_mouse_point.m_x, delta_mouse_point.m_y);  break;
			case SceneState::PLAY:  player_camera->OnMouseMove(delta_mouse_point.m_x, delta_mouse_point.m_y); break;
		}
}

void AppWindow::onLeftMouseDown(const Point& delta_mouse_point)
{
}

void AppWindow::onLeftMouseUp(const Point& delta_mouse_point)
{
}


void AppWindow::onRightMouseDown(const Point& delta_mouse_point)
{
	
}

void AppWindow::onRightMouseUp(const Point& delta_mouse_point)
{
	
}

