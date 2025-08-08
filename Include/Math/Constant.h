#pragma once
#include <Math/Matrix4x4.h>
#include <Math/Vector3D.h>
#include <d3d11.h>

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_angle;
	Vector3D m_color;
	UINT isRandom;
	UINT hasTex;
};
