#include "PixelShader.h"
#include "GraphicsEngine.h"

PixelShader::PixelShader()
{
}

void PixelShader::release()
{
    m_ps->Release();
    delete this;
}

PixelShader::~PixelShader()
{

}

bool PixelShader::init(const void* shaderByteCode, size_t byteCodeSize)
{
    if (!SUCCEEDED(GraphicsEngine::get()->m_d3d_device->CreatePixelShader(shaderByteCode, byteCodeSize, nullptr, &m_ps)))
        return false;

    return true;
}