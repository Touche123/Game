#pragma once
#include <d3d11.h>

class GraphicsEngine;
class DeviceContext;

class PixelShader
{
public:
	PixelShader();
	void release();
	~PixelShader();

private:
	bool init(const void* shaderByteCode, size_t byteCodeSize);
private:
	ID3D11PixelShader* m_ps;
private:
	friend class GraphicsEngine;
	friend class DeviceContext;
};
