#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* deviceContext):m_deviceContext(deviceContext)
{
}

void DeviceContext::clearRenderTargetColor(SwapChain* swapChain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red, green, blue, alpha };
	m_deviceContext->ClearRenderTargetView(swapChain->m_renderTargetView, clear_color);
	m_deviceContext->OMSetRenderTargets(1, &swapChain->m_renderTargetView, NULL);
}

void DeviceContext::setVertexBuffer(VertexBuffer* vertexBuffer)
{
	UINT stride = vertexBuffer->m_SizeVertex;
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer->m_buffer, &stride, &offset);

	m_deviceContext->IASetInputLayout(vertexBuffer->m_layout);

}

void DeviceContext::drawTriangleList(UINT vertexCount, UINT startVertexIndex)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->Draw(vertexCount, startVertexIndex);
}

void DeviceContext::drawTriangleStrip(UINT vertexCount, UINT startVertexIndex)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_deviceContext->Draw(vertexCount, startVertexIndex);
}

void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1, &vp);
}

void DeviceContext::setVertexShader(VertexShader* vertexShader)
{
	m_deviceContext->VSSetShader(vertexShader->m_vs, nullptr, 0);
}

void DeviceContext::setPixelShader(PixelShader* pixelShader)
{
	m_deviceContext->PSSetShader(pixelShader->m_ps, nullptr, 0);
}

bool DeviceContext::release()
{
	m_deviceContext->Release();
	delete this;
	return true;
}

DeviceContext::~DeviceContext()
{
}
