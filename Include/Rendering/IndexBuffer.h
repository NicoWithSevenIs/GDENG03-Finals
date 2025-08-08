#pragma once

#include <d3d11.h>

class DeviceContext;

class IndexBuffer {
private:
	UINT m_size_list;

	ID3D11Buffer* m_buffer;

private:
	friend class DeviceContext;

public:
	IndexBuffer();
	~IndexBuffer();

	bool load(void* list_incides, UINT size_list);
	bool release();

	UINT getSizeIndexList();
};