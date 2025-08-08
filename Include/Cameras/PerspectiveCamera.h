#pragma once
#include "Cameras/Camera.h"

class PerspectiveCamera: public Camera
{
	private:
		float m_fov;
		float m_aspect_ratio;

	private:
		float dir = 0;
		float r = 0;
		float up = 0;

		float xRot = 0;
		float yRot = 0;

	public:
		PerspectiveCamera(float fov, float aspect_ratio);
		virtual Matrix4x4 GetProjectionMatrix() override;
		virtual void Update() override;

		void OnKeyDown(int key);
		void OnKeyUp(int key);
		void OnMouseMove(float x, float y);
};

