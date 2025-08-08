#pragma once


#include <vector>

#include "GameObject/GameObject.h"


class Cube: public GameObject {
	

	public:
		IndexBuffer* m_ib;

		std::vector<vertex> cube_list;
		std::vector<unsigned int> index_list;

		float pace = 0.1f;
		float dir = 1.f;
		float m_angle = 0;


	public:
		Cube();
		Cube(std::string name);

		bool load() override;
		bool release() override;
		void Update(float delta_time, Matrix4x4 view_matrix, Matrix4x4 projection_matrix) override;
		void Draw() override;

		static Vector3D getRandom(int cap) {
			Vector3D r;
			r.m_x = rand() % cap;
			r.m_y = rand() % cap;
			r.m_z = rand() % cap;
			return r;
		}
		
};

