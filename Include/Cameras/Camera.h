#pragma once

#include "Math/Matrix4x4.h"
#include "Math/Transform.h"
#include "GameObject/IGUID.h"


class Camera: public IGUID {

	public:
		Transform m_transform;

	public:
		Camera(): IGUID("Camera"), m_transform(this) {}
		inline virtual Matrix4x4 GetViewMatrix() {
			Matrix4x4 mat = Matrix4x4(this->m_transform.GetTransformationMatrix());
			mat.inverse();
			return mat;
		}
		virtual Matrix4x4 GetProjectionMatrix() = 0;
		virtual void Update() = 0;
};

