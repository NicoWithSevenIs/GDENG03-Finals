#pragma once


#include <d3d11.h>
#include <string>
#include <functional>
#include <DirectXMath.h>
#include <cmath>


#include "Math/Vector3D.h"
#include "Math/Matrix4x4.h"
#include "Math/Transform.h"

#include "Rendering/GraphicsEngine.h"
#include "Rendering/DeviceContext.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexShader.h"
#include "Rendering/PixelShader.h"
#include "Rendering/ConstantBuffer.h"


#include "GameObject/IGUID.h"
#include "GameObject/ParentingManager.h"

#include "Math/Constant.h"

class GameObject: public IGUID
{
	public:
		Transform m_transform;


	protected:

		VertexBuffer* m_vb;

		VertexShader* m_vs;
		PixelShader* m_ps;

		ConstantBuffer* m_cb;

	protected:
		constant cc;

	public:
		Vector3D m_color = Vector3D(1,0,1);
		bool isRainbow = true;
		std::function<void()> doOnUpdate = nullptr;

	public:

		GameObject(std::string name): IGUID(name), m_transform(Transform(this)) {}

		virtual bool load() = 0;
		virtual bool release() = 0;

		virtual void Draw() = 0;

		inline virtual void Update(float delta_time, Matrix4x4 view_matrix, Matrix4x4 projection_matrix) {
			cc.m_world = this->m_transform.GetTransformationMatrix();
			auto parent = (GameObject*)ParentingManager::get().GetParent(this);
			auto child = this;

			while (parent != nullptr) {
				cc.m_world *= parent->m_transform.GetTransformationMatrix();
				child = parent;
				parent = parent = (GameObject*)ParentingManager::get().GetParent(child);
			}	
		}

		inline void OnUnparent() override {
	
			auto descendant = this;
			auto ancestry = (GameObject*)ParentingManager::get().GetParent(descendant);
			Matrix4x4 ancestry_matrix = m_transform.GetTransformationMatrix();

			while (ancestry != nullptr) {
				ancestry_matrix *= ancestry->m_transform.GetTransformationMatrix();
				descendant = ancestry;
				ancestry = (GameObject*)ParentingManager::get().GetParent(descendant);
			}

			auto mat = ancestry_matrix.m_mat;

			m_transform.m_translation = Vector3D(mat[3][0], mat[3][1], mat[3][2]);

			auto row_1 = Vector3D(mat[0][0], mat[0][1], mat[0][2]);
			auto row_2 = Vector3D(mat[1][0], mat[1][1], mat[1][2]);
			auto row_3 = Vector3D(mat[2][0], mat[2][1], mat[2][2]);

			auto scale = Vector3D(
				Vector3D::magnitude(row_1),
				Vector3D::magnitude(row_2),
				Vector3D::magnitude(row_3)
			);

			m_transform.m_scale = scale;
		

			float pitch, yaw, roll;
			
		
			/*
				auto parent_mat = parent->m_transform.GetTransformationMatrix();
				Matrix4x4 temp;

				temp.SetIdentity();
				temp.setRotationX(m_transform.m_rotation.m_x);
				temp *= parent_mat;
				pitch = atan2f(-temp.m_mat[1][2], temp.m_mat[1][1]);


				temp.SetIdentity();
				temp.setRotationY(m_transform.m_rotation.m_y);
				temp *= parent_mat;
				yaw = atan2f(-temp.m_mat[2][0], temp.m_mat[0][0]);


				temp.SetIdentity();
				temp.setRotationZ(m_transform.m_rotation.m_z);
				temp *= parent_mat;
				roll = asinf(temp.m_mat[1][0]);
			*/
			auto parent = (GameObject*)ParentingManager::get().GetParent(this);
			GameObject* child = this;

			pitch = m_transform.m_rotation.m_x;
			yaw = m_transform.m_rotation.m_y;
			roll = m_transform.m_rotation.m_z;
			
			while (parent != nullptr) {
				pitch += parent->m_transform.m_rotation.m_x;
				yaw += parent->m_transform.m_rotation.m_y;
				roll += parent->m_transform.m_rotation.m_z;

				child = parent;
				parent = parent = (GameObject*)ParentingManager::get().GetParent(child);
			}


			m_transform.m_rotation = Vector3D(pitch, yaw, roll);


			std::cout << "New Translate: " <<  mat[3][0] << "," << mat[3][1] << "," << mat[3][2] << std::endl;
			//std::cout << "New Scale: " << scale.m_x << "," << scale.m_y << "," << scale.m_z << std::endl;
			std::cout << "New Rotation: " << pitch << "," << yaw << "," << roll << std::endl;
			
		}

};

