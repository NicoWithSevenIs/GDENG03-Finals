#include "Cameras/PerspectiveCamera.h"
#include "Game Engine/EngineTime.h"

PerspectiveCamera::PerspectiveCamera(float fov, float aspect_ratio): 
	Camera(), m_fov(fov), m_aspect_ratio(aspect_ratio)
{
}

Matrix4x4 PerspectiveCamera::GetProjectionMatrix()
{
    Matrix4x4 proj;
    proj.setPerspectiveFovLH(m_fov, m_aspect_ratio, 0.1f, 100.f);
    return proj;
}

void PerspectiveCamera::Update()
{
	Matrix4x4 camera_matrix = this->m_transform.GetTransformationMatrix();
	Vector3D forward = camera_matrix.getLocalZDirection() * EngineTime::deltaTime() * dir;
	Vector3D right = camera_matrix.getLocalXDirection() * EngineTime::deltaTime() * r;

	this->m_transform.m_translation = this->m_transform.m_translation + 
		forward + right + Vector3D(0, EngineTime::deltaTime() * up, 0);
	this->m_transform.m_rotation = Vector3D(xRot, yRot, 0);
}

void PerspectiveCamera::OnKeyDown(int key)
{   //this will be processed via keyboard press or input.//
	switch (key) {
		case 'W': dir = 1.f; break; //FORWARD CAMERA MOVEMENT
		case 'S': dir = -1.f;  break; //BACKWARD CAMERA MOVEMENT

		case 'A': r = -1.f; break; //LEFT CAMERA MOVEMENT
		case 'D': r = 1.f;  break; // RIGHT CAMERA MOVEMENT

		case 'Q': up = -1.f; break; //DOWNWARD MOVEMENT
		case 'E':  up = 1.f;  break; //UPWARD MOVEMENT

		case 'B': xRot -= EngineTime::deltaTime() * 0.5f; break; //ROTATE DOWN
		case 'V': xRot += EngineTime::deltaTime() * 0.5f; break; //ROTATE UP

		case 'M': yRot -= EngineTime::deltaTime() * 0.5f; break; //ROTATE LEFT
		case 'N': yRot += EngineTime::deltaTime() * 0.5f; break; //ROTATE RIGHT
	}
}

void PerspectiveCamera::OnKeyUp(int key)
{
	switch (key) {
		case 'W':
		case 'S': dir = 0.f;  break;
		case 'A':
		case 'D': r = 0.f;  break;
		case 'Q':
		case 'E': up = 0.f;  break;
	}
}

void PerspectiveCamera::OnMouseMove(float x, float y)
{
	//this will be processed via mouse movement.//
	//MOUSE CAMERA MOVEMENT
	xRot -= y * EngineTime::deltaTime() * 0.1f;
	yRot -= x * EngineTime::deltaTime() * 0.1f;
}

