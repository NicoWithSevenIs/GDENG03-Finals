#pragma once


#include <vector>

#include "Window/Window.h"
#include "Rendering/GraphicsEngine.h"
#include "Rendering/SwapChain.h"
#include "Rendering/DeviceContext.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexShader.h"
#include "Rendering/PixelShader.h"
#include "Rendering/ConstantBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Input/InputListener.h"

#include "Cameras/PerspectiveCamera.h"

#include "UI/UIManager.h"
#include "UI/HierarchyScreen.h"
#include "UI/InspectorScreen.h"


class AppWindow : public Window, public InputListener {
	
	private:
		SwapChain* m_swap_chain;
		PerspectiveCamera* editor_camera;
		PerspectiveCamera* player_camera;

		bool toggle_camera_movement = true;
		bool is_ctrl_held = false;

	public:
		AppWindow();
		~AppWindow();

		// Inherited via Window
		void OnCreate() override;
		void InitializeSceneStateCallbacks();
		void OnUpdate() override;
		void OnDestroy() override;

		void OnFocus() override;
		void OnKillFocus() override;

	public:
		// Inherited via InputListener
		void onKeyDown(int key) override;
		void onKeyUp(int key) override;

		// Inherited via InputListener
		void onMouseMove(const Point& delta_mouse_point, const Point& mouse_pos) override;

		// Inherited via InputListener
		void onLeftMouseDown(const Point& delta_mouse_point) override;
		void onLeftMouseUp(const Point& delta_mouse_point) override;
		void onRightMouseDown(const Point& delta_mouse_point) override;
		void onRightMouseUp(const Point& delta_mouse_point) override;

};