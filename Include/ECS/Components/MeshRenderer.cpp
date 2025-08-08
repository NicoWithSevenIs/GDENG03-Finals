#include "MeshRenderer.h"

void MeshRenderer::Load(std::string model_path, std::string tex_path)
{
	std::cout << "Model Path Out IF: " << model_path << std::endl;
	std::cout << "Texture Path Out IF: " << tex_path << std::endl;
	this->model_path = model_path;
	this->tex_path = tex_path;

	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	bool success = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, model_path.c_str());

	if (!err.empty() || !success || shapes.size() > 1) {
		std::cout << "Mesh Config Error: " << err << std::endl;
		return;
	}

	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;

		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

			for (unsigned char v = 0; v < num_face_verts; v++) {

				tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
				tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
				tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

				auto vert = Vector3D(vx, vy, vz);

				Vector2D tex;
				if (index.texcoord_index >= 0) {
					tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
					tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];
					tex = Vector2D(tx, ty);
				}

				Vector3D norms;
				if (index.normal_index >= 0) {
					tinyobj::real_t nx = attribs.normals[index.normal_index * 3 + 0];
					tinyobj::real_t ny = attribs.normals[index.normal_index * 3 + 1];
					tinyobj::real_t nz = attribs.normals[index.normal_index * 3 + 2];
					norms = Vector3D(nx, ny, nz);
				}

				list_vertices.push_back(MeshVertex(vert, tex, norms));
				list_indices.push_back(index_offset + v);

			}

			index_offset += num_face_verts;
		}

	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	this->m_vb = GraphicsEngine::get()->createVertexBuffer();
	this->m_ib = GraphicsEngine::get()->createIndexBuffer();

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	this->m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

	this->m_vb->load(&list_vertices[0], sizeof(MeshVertex), (UINT)list_vertices.size(), shader_byte_code, (UINT)size_shader);
	this->m_ib->load(&list_indices[0], (UINT)list_indices.size());

	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	this->m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	m_tex = new Texture();
	if (!tex_path.empty()) {
		cc.hasTex = m_tex->load(tex_path);
	}
	else {
		cc.hasTex = false;
	}

	//constant cc;
	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void MeshRenderer::Draw()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);


	GraphicsEngine::get()->getImmediateDeviceContext()->setTexture(m_vs, m_tex);
	GraphicsEngine::get()->getImmediateDeviceContext()->setTexture(m_ps, m_tex);


	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), m_vb->getSizeVertexList(), 0, 0);

}
