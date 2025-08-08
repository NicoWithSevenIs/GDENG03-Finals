#include "Rendering/PixelShader.h"
#include "Rendering/GraphicsEngine.h"

PixelShader::PixelShader()
{
}

PixelShader::~PixelShader()
{
}

bool PixelShader::release()
{

	this->m_ps->Release();
	delete this;
	return true;
}

bool PixelShader::init(const void* shader_byte_code, size_t byte_code_size)
{
	if (!SUCCEEDED(GraphicsEngine::get()->m_d3d_device->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &m_ps)))
		return false;
	return true;
}
