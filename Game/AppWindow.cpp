#include "AppWindow.h"


struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
	vec3 color;
};

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex list[] =
	{
		{-0.5f, -0.5f, 0.0f, 1,0,0},
		{-0.5f,  0.5f, 0.0f, 0,1,0},
		{ 0.5f, -0.5f, 0.0f, 0,0,1},
		{ 0.5f,  0.5f, 0.0f, 1,1,0},
	};

	m_vertexBuffer = GraphicsEngine::get()->createVertexBuffer();

	UINT sizeList = ARRAYSIZE(list);

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;

	// Compile the vertex shader
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shaderByteCode, &sizeShader);
	m_vertexShader = GraphicsEngine::get()->createVertexShader(shaderByteCode, sizeShader);
	m_vertexBuffer->load(list, sizeof(vertex), sizeList, shaderByteCode, sizeShader);
	GraphicsEngine::get()->releaseCompiledShader();

	// Compile the pixel shader
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shaderByteCode, &sizeShader);
	m_pixelShader = GraphicsEngine::get()->createPixelShader(shaderByteCode, sizeShader);
	GraphicsEngine::get()->releaseCompiledShader();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vertexShader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_pixelShader);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vertexBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vertexBuffer->getVertexListSize(), 0);
	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vertexBuffer->release();
	m_swap_chain->release();
	m_vertexShader->release();
	m_pixelShader->release();
	GraphicsEngine::get()->release();
}
