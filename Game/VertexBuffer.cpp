#include "VertexBuffer.h"
#include "GraphicsEngine.h"

VertexBuffer::VertexBuffer():m_layout(0),m_buffer(0)
{
}

bool VertexBuffer::load(void* listVertices, UINT sizeVertex, UINT sizeList, void* shaderByteCode, UINT sizeByteShader)
{
	if (m_buffer)m_buffer->Release();
	if (m_layout)m_layout->Release();

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeVertex * sizeList;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = listVertices;

	m_SizeVertex = sizeVertex;
	m_SizeList = sizeList;


	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateBuffer(&bufferDesc, &initData, &m_buffer)))
	{
		return false;
	}

	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT sizeLayout = ARRAYSIZE(layout);

	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateInputLayout(layout, sizeLayout, shaderByteCode, sizeByteShader, &m_layout)))
	{
		return false;
	}

	return true;
}

UINT VertexBuffer::getVertexListSize()
{
	return this->m_SizeList;
}

bool VertexBuffer::release()
{
	m_layout->Release();
	m_buffer->Release();
	delete this;

	return true;
}

VertexBuffer::~VertexBuffer()
{
}
