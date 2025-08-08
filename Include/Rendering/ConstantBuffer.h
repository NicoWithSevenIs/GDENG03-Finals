#pragma once
#include <d3d11.h>

class DeviceContext;

class ConstantBuffer {

	private:
		ID3D11Buffer* m_buffer;

	public:
		ConstantBuffer();
		~ConstantBuffer();

		bool load(void* buffer, UINT size_buffer);
		bool release();
		void update(DeviceContext* context, void* buffer);

	private:
		friend class DeviceContext;
};