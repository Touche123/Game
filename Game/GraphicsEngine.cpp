#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <d3dcompiler.h>

GraphicsEngine::GraphicsEngine()
{
}

bool GraphicsEngine::init()
{
    D3D_DRIVER_TYPE driver_types[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };

    UINT num_driver_types = ARRAYSIZE(driver_types);

    D3D_FEATURE_LEVEL feature_levels[] =
    {
        D3D_FEATURE_LEVEL_11_0
    };
    UINT num_feature_levels = ARRAYSIZE(feature_levels);
    HRESULT res = 0;

    for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
    {
        res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
            num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);

        if (SUCCEEDED(res))
            break;

        ++driver_type_index;
    }

    if (FAILED(res))
    {
        return false;
    }
    
    m_imm_device_context = new DeviceContext(m_imm_context);

    m_d3d_device->QueryInterface(__uuidof(IDXGIDevice),(void**)&m_dxgi_device);
    m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
    m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);
    return true;
}

bool GraphicsEngine::release()
{
    m_dxgi_device->Release();
    m_dxgi_adapter->Release();
    m_dxgi_factory->Release();

    m_imm_device_context->release();
    m_d3d_device->Release();
    return true;
}

GraphicsEngine::~GraphicsEngine()
{
}

SwapChain* GraphicsEngine::createSwapChain()
{
    return new SwapChain();
}

DeviceContext* GraphicsEngine::getImmediateDeviceContext()
{
    return this->m_imm_device_context;
}

VertexBuffer* GraphicsEngine::createVertexBuffer()
{
    return new VertexBuffer();
}

VertexShader* GraphicsEngine::createVertexShader(const void* shaderByteCode, size_t byteCodeSize)
{
    VertexShader* vs = new VertexShader();

    if (!vs->init(shaderByteCode, byteCodeSize))
    {
        vs->release();
        return nullptr;
    }
        
    return vs;
}

PixelShader* GraphicsEngine::createPixelShader(const void* shaderByteCode, size_t byteCodeSize)
{
    PixelShader* ps = new PixelShader();

    if (!ps->init(shaderByteCode, byteCodeSize))
    {
        ps->release();
        return nullptr;
    }

    return ps;
}

bool GraphicsEngine::compileVertexShader(const wchar_t* filename, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize)
{
    ID3DBlob* errorBlob = nullptr;
    if (!SUCCEEDED(::D3DCompileFromFile(filename, nullptr, nullptr, entryPointName, "vs_5_0", 0, 0, &m_blob, &errorBlob)))
    {
        if (errorBlob) errorBlob->Release();
        return false;
    }

    *shaderByteCode = m_blob->GetBufferPointer();
    *byteCodeSize = m_blob->GetBufferSize();

    return false;
}

bool GraphicsEngine::compilePixelShader(const wchar_t* filename, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize)
{
    ID3DBlob* errorBlob = nullptr;
    if (!SUCCEEDED(::D3DCompileFromFile(filename, nullptr, nullptr, entryPointName, "ps_5_0", 0, 0, &m_blob, &errorBlob)))
    {
        if (errorBlob) errorBlob->Release();
        return false;
    }

    *shaderByteCode = m_blob->GetBufferPointer();
    *byteCodeSize = m_blob->GetBufferSize();

    return false;
}

void GraphicsEngine::releaseCompiledShader()
{
    if (m_blob) m_blob->Release();
}

GraphicsEngine* GraphicsEngine::get()
{
    static GraphicsEngine engine;

    return &engine;
}
