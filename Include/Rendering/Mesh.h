#pragma once


#include <tiny_obj_loader.h>

#include <vector>

#include "Rendering/MeshVertex.h"
#include "Rendering/VertexShader.h"
#include "Rendering/Texture.h"

#include "GameObject/GameObject.h"

class Mesh: public GameObject
{

	private:
		std::vector<MeshVertex> list_vertices;
		std::vector<unsigned int> list_indices;

		std::string model_path;
		std::string texture_path;

		IndexBuffer* m_ib;
		Texture* m_tex;

	public:
		Mesh(std::string name, std::string model_path, std::string texture_path);


		bool load() override;
		bool release() override;

		void Draw() override;
		void Update(float delta_time, Matrix4x4 view_matrix, Matrix4x4 projection_matrix) override;

};

