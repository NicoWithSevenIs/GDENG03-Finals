#pragma once


#include "Math/Vector3D.h"
#include "Math/Matrix4x4.h"

#include "GameObject/IGUID.h"
#include <sstream>
#include <string>
class Transform
{
	public:
		Vector3D m_translation;
		Vector3D m_scale;
		Vector3D m_rotation;

	private:
		Matrix4x4 m_transformation_matrix;

	private:
		IGUID* m_guid;


	public:
		Transform(IGUID* guid): m_translation(Vector3D()), m_scale(Vector3D(1,1,1)), m_rotation(Vector3D()), m_guid(guid) {}

		IGUID* Parent() {return m_guid;}

		void BuildTransform() {

			Matrix4x4 m;
			m.SetIdentity();
	
			Matrix4x4 sm;
			sm.SetScale(m_scale);
			m *= sm;

			Matrix4x4 rm;

			Matrix4x4 temp;
			temp.SetIdentity();

			rm.SetIdentity();
			rm.setRotationX(m_rotation.m_x);
			temp *= rm;


			rm.SetIdentity();
			rm.setRotationY(m_rotation.m_y);
			temp *= rm;


			rm.SetIdentity();
			rm.setRotationZ(m_rotation.m_z);
			temp *= rm;

			m *= temp;
		

			Matrix4x4 tm;
			tm.SetTranslation(m_translation);
			m *= tm;
	
		
			this->m_transformation_matrix = m;

		}

		Matrix4x4 GetTransformationMatrix() {
			BuildTransform();
			return m_transformation_matrix;
		}

		static Matrix4x4 CalculateTransform(Vector3D translate, Vector3D scale, Vector3D rotation) {

			Matrix4x4 m;
			m.SetIdentity();

			Matrix4x4 sm;
			sm.SetScale(scale);
			m *= sm;

			Matrix4x4 rm;

			Matrix4x4 temp;
			temp.SetIdentity();

			rm.SetIdentity();
			rm.setRotationX(rotation.m_x);
			temp *= rm;


			rm.SetIdentity();
			rm.setRotationY(rotation.m_y);
			temp *= rm;


			rm.SetIdentity();
			rm.setRotationZ(rotation.m_z);
			temp *= rm;

			m *= temp;


			Matrix4x4 tm;
			tm.SetTranslation(translate);
			m *= tm;


			return m;
		}

		inline std::string ExportToJSON() {
			std::stringstream s;

			float cx = cos(m_rotation.m_x * 0.5f);
			float sx = sin(m_rotation.m_x * 0.5f);
			float cy = cos(m_rotation.m_y * 0.5f);
			float sy = sin(m_rotation.m_y * 0.5f);
			float cz = cos(m_rotation.m_z * 0.5f);
			float sz = sin(m_rotation.m_z * 0.5f);

			float x = sx * cy * cz + cx * sy * sz;
			float y = cx * sy * cz - sx * cy * sz;
			float z = cx * cy * sz + sx * sy * cz;
			float w = cx * cy * cz - sx * sy * sz;



			s << "\"Transform\": {\n"
				<< "\"Translate\": [" << m_translation.m_x << "," << m_translation.m_y << "," << m_translation.m_z << "],\n"
				<< "\"Scale\": [" << m_scale.m_x << "," << m_scale.m_y << "," << m_scale.m_z << "],\n"
				<< "\"Rotation\": [" << x << "," << y << "," << z << "," << w << "]\n}";
		

			return s.str();
		}

};

