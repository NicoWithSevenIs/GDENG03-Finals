#include "CapsuleRenderer.h"
#include "../Entities/Entity.h"

void CapsuleRenderer::Load()
{
    vertex list[1000];
    unsigned int index_list[4000];

	this->MakeCapsule(list, index_list);

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

void CapsuleRenderer::Draw()
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

void CapsuleRenderer::MakeCapsule(vertex* list, unsigned int* index_list)
{
    const float PI = 3.14159265359f;

    constexpr float radius = 1.0f;
    constexpr float height = 5.0f;
    constexpr int slices = 24;
    constexpr int stacks = 12;

    float cylinderHeight = height - 2 * radius;
    if (cylinderHeight < 0) cylinderHeight = 0;

    int curr_vertex = 0;
    int curr_index = 0;

    // Bottom hemisphere
    {
        Vector3D center(0, -cylinderHeight / 2.0f, 0);
        for (int i = 0; i <= stacks; ++i) {
            float theta = PI / 2.0f + ((float)i / stacks) * (PI / 2.0f); // π/2 to π
            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);

            for (int j = 0; j <= slices; ++j) {
                float phi = ((float)j / slices) * 2.0f * PI;
                float sinPhi = std::sin(phi);
                float cosPhi = std::cos(phi);

                Vector3D normal(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
                Vector3D pos = center + normal * radius;

                Vector2D uv((float)j / slices, 1.0f - ((float)i / stacks));

                list[curr_vertex++] = { pos, uv, normal };
            }
        }

        int baseIndex = 0;
        for (int i = 0; i < stacks; ++i) {
            for (int j = 0; j < slices; ++j) {
                int curr = baseIndex + i * (slices + 1) + j;
                int next = curr + slices + 1;

                index_list[curr_index++] = curr;
                index_list[curr_index++] = next;
                index_list[curr_index++] = curr + 1;

                index_list[curr_index++] = curr + 1;
                index_list[curr_index++] = next;
                index_list[curr_index++] = next + 1;
            }
        }
    }

    // Cylinder
    {
        int baseIndex = curr_vertex;
        for (int i = 0; i <= 1; ++i) {
            float y = cylinderHeight / 2.0f - i * cylinderHeight;
            for (int j = 0; j <= slices; ++j) {
                float phi = ((float)j / slices) * 2.0f * PI;
                float cosPhi = std::cos(phi);
                float sinPhi = std::sin(phi);

                Vector3D normal(cosPhi, 0, sinPhi);
                Vector3D pos = Vector3D(cosPhi * radius, y, sinPhi * radius);

                Vector2D uv((float)j / slices, (float)i);

                list[curr_vertex++] = { pos, uv, normal };
            }
        }

        for (int j = 0; j < slices; ++j) {
            int curr = baseIndex + j;
            int next = baseIndex + j + slices + 1;

            index_list[curr_index++] = curr;
            index_list[curr_index++] = curr + 1;
            index_list[curr_index++] = next;

            index_list[curr_index++] = curr + 1;
            index_list[curr_index++] = next + 1;
            index_list[curr_index++] = next;
        }
    }

    // Top hemisphere
    {
        int baseIndex = curr_vertex;
        Vector3D center(0, cylinderHeight / 2.0f, 0);
        for (int i = 0; i <= stacks; ++i) {
            float theta = ((float)i / stacks) * (PI / 2.0f); // 0 to π/2
            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);

            for (int j = 0; j <= slices; ++j) {
                float phi = ((float)j / slices) * 2.0f * PI;
                float sinPhi = std::sin(phi);
                float cosPhi = std::cos(phi);

                Vector3D normal(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
                Vector3D pos = center + normal * radius;

                Vector2D uv((float)j / slices, (float)i / stacks);

                list[curr_vertex++] = { pos, uv, normal };
            }
        }

        for (int i = 0; i < stacks; ++i) {
            for (int j = 0; j < slices; ++j) {
                int curr = baseIndex + i * (slices + 1) + j;
                int next = curr + slices + 1;

                index_list[curr_index++] = curr;
                index_list[curr_index++] = curr + 1;
                index_list[curr_index++] = next;

                index_list[curr_index++] = curr + 1;
                index_list[curr_index++] = next + 1;
                index_list[curr_index++] = next;
            }
        }
    }
}
