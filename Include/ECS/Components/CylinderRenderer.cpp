#include "CylinderRenderer.h"
#include "../Entities/Entity.h"

void CylinderRenderer::Load()
{
	vertex list[146] = {};
	unsigned int index_list[432] = {};
	this->MakeCylinder(list, index_list);

	UINT size_list = ARRAYSIZE(list);
	UINT size_index_list = ARRAYSIZE(index_list);

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	m_ib = GraphicsEngine::get()->get()->createIndexBuffer();

	m_ib->load(index_list, size_index_list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);

	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();

	m_cb = GraphicsEngine::get()->createConstantBuffer();

	std::string texture_path = "brick.png";

	constant cc;
	m_tex = new Texture();
	if (!texture_path.empty()) {
		cc.hasTex = m_tex->load(texture_path);
		isTextured = cc.hasTex;
		std::cout << "Has Tex Bool: " << cc.hasTex << std::endl;
	}
	else {
		cc.hasTex = false;
	}
	m_cb->load(&cc, sizeof(constant));
}

void CylinderRenderer::Draw()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_ps, m_cb);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(this->m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(this->m_ps);

	if (this->owner->cc.hasTex) {
		GraphicsEngine::get()->getImmediateDeviceContext()->setTexture(m_vs, m_tex);
		GraphicsEngine::get()->getImmediateDeviceContext()->setTexture(m_ps, m_tex);
	}

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedList(m_ib->getSizeIndexList(), 0, 0);
}

void CylinderRenderer::MakeCylinder(vertex* list, unsigned int* index_list)
{
	float radius = 1.0f;
	float pi = 3.14f;
	if (this->owner == nullptr) {
		std::cout << "owner is null" << std::endl;
	}
	else {
		std::cout << "owner is not null" << std::endl;
	}

	Vector3D objectPos = this->owner->m_transform.m_translation;

	Vector3D circle1_center = Vector3D(objectPos.m_x, objectPos.m_y + (height / 2), objectPos.m_z);
	Vector3D circle2_center = Vector3D(objectPos.m_x, objectPos.m_y + -(height / 2), objectPos.m_z);

	int slices = 36;
	int curr_vertex = 0;
	int curr_index = 0;

	list[curr_vertex++] = { circle1_center, Vector2D(0.5, 0.5), Vector3D(0, 1, 0) };
	list[curr_vertex++] = { circle2_center, Vector2D(0.5, 0.5), Vector3D(0, 1, 0) };

	for (int i = 0; i < slices; i++) {
		float angle = (float)i / slices * 2.0f * pi;
		float x = radius * cos(angle);
		float z = radius * sin(angle);

		Vector3D circle1 = circle1_center + Vector3D(x, 0, z);
		Vector3D circle2 = circle2_center + Vector3D(x, 0, z);

		Vector2D cap_texcoord = Vector2D(0.5f + 0.5f * cos(angle), 0.5f + 0.5f * sin(angle));

		list[curr_vertex++] = { circle1, cap_texcoord, Vector3D(0, 1, 0) };
		list[curr_vertex++] = { circle2, cap_texcoord, Vector3D(0, 1, 0) };
	}

	int side_start_index = curr_vertex;

	//retrieving uv or tex coords for sides//
	for (int i = 0; i < slices; ++i) {
		float angle = (float)i / slices * 2.0f * pi;
		float x = radius * cos(angle);
		float z = radius * sin(angle);

		Vector3D circle1 = circle1_center + Vector3D(x, 0, z);
		Vector3D circle2 = circle2_center + Vector3D(x, 0, z);

		float u = (float)i / slices;
		Vector2D texcoord1 = Vector2D(u, 0);
		Vector2D texcoord2 = Vector2D(u, 1);

		list[curr_vertex++] = { circle1, texcoord1, Vector3D(0, 1, 0) };
		list[curr_vertex++] = { circle2, texcoord2, Vector3D(0, 1, 0) };
	}

	//connecting circles//
	for (int i = 0; i < slices; ++i) {
		int top_center = 0;
		int top_curr_slice = 2 + i * 2;
		int top_next_slice = 2 + ((i + 1) % slices) * 2;

		index_list[curr_index++] = top_center;
		index_list[curr_index++] = top_next_slice;
		index_list[curr_index++] = top_curr_slice;

		int bottom_center = 1;
		int bottom_curr_slice = 2 + i * 2 + 1;
		int bottom_next_slice = 2 + ((i + 1) % slices) * 2 + 1;

		index_list[curr_index++] = bottom_center;
		index_list[curr_index++] = bottom_curr_slice;
		index_list[curr_index++] = bottom_next_slice;
	}

	//connecting side walls//
	for (int i = 0; i < slices; ++i) {
		int top_curr = side_start_index + i * 2;
		int bottom_curr = side_start_index + i * 2 + 1;
		int top_next = side_start_index + ((i + 1) % slices) * 2;
		int bottom_next = side_start_index + ((i + 1) % slices) * 2 + 1;

		index_list[curr_index++] = top_curr;
		index_list[curr_index++] = bottom_next;
		index_list[curr_index++] = bottom_curr;

		index_list[curr_index++] = top_curr;
		index_list[curr_index++] = top_next;
		index_list[curr_index++] = bottom_next;
	}
}
