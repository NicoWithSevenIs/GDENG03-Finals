#include "SphereRenderer.h"
#include "../Entities/Entity.h"

void SphereRenderer::Load()
{
	vertex list[121] = {};

	unsigned int index_list[600] = {};

	this->MakeSphere(list, index_list);

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

	constant cc;
	std::string texture_path = "brick.png";
	m_tex = new Texture();
	if (!texture_path.empty()) {
		cc.hasTex = m_tex->load(texture_path);
		isTextured = cc.hasTex;
		std::cout << "Has Tex Bool: " << cc.hasTex << std::endl;
	}
	else {
		cc.hasTex = false;
	}

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void SphereRenderer::Draw()
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

void SphereRenderer::MakeSphere(vertex* list, unsigned int* index_list)
{
	float radius = 1.0f;
	float pi = 3.14;
	Vector3D center = this->owner->m_transform.m_translation;

	int stacks = 10;
	int slices = 10;

	int index = 0;

	for (int i = 0; i <= stacks; i++) {
		float stack_angle = (float)i / stacks * pi;
		for (int j = 0; j <= slices; j++) {
			float slice_angle = (float)j / slices * 2 * pi;

			float x = radius * sin(stack_angle) * cos(slice_angle);
			float y = radius * cos(stack_angle);
			float z = radius * sin(stack_angle) * sin(slice_angle);

			Vector3D pos = center + Vector3D(x, y, z);
			Vector3D color1 = Vector3D(1, 0, 1);

			float u = (float)j / slices;
			float v = (float)i / stacks;
			Vector2D texCoord = Vector2D(u, v);

			list[index] = { pos, texCoord, color1 };
			index++;
		}
	}

	index = 0;
	for (int i = 0; i < stacks; i++) {
		for (int j = 0; j < slices; j++) {
			int top_left = i * (slices + 1) + j;
			int bot_left = top_left + (slices + 1);

			index_list[index + 0] = top_left;
			index_list[index + 1] = top_left + 1;
			index_list[index + 2] = bot_left;

			index_list[index + 3] = bot_left;
			index_list[index + 4] = top_left + 1;
			index_list[index + 5] = bot_left + 1;

			index = index + 6;
		}
	}
}

void SphereRenderer::TextureChange(std::string texture_path)
{
	if (std::filesystem::exists(texture_path)) {
		Texture* newTexture = new Texture();
		newTexture->load(texture_path);
		m_tex = newTexture;
		this->owner->cc.hasTex = true;
	}
	
}
