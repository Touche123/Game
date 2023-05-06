#include "VertexShader.h"
#include "GraphicsEngine.h"

VertexShader::VertexShader()
{
}

void VertexShader::release()
{
    m_vs->Release();
    delete this;
}

VertexShader::~VertexShader()
{
    
}

bool VertexShader::init(const void* shaderByteCode, size_t byteCodeSize)
{
    if (!SUCCEEDED(GraphicsEngine::get()->m_d3d_device->CreateVertexShader(shaderByteCode, byteCodeSize, nullptr, &m_vs)))
        return false;

    return true;
}
