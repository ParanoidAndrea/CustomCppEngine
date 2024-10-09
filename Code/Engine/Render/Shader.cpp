#include "Shader.hpp"
#include "Engine/Render/Renderer.hpp"
#define WIN32_LEARN_AND_MEAN

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

Shader::Shader(const ShaderConfig& config)
	:m_config(config)
{

}

Shader::~Shader()
{
	m_vertexShader->Release();
	m_pixelShader->Release();
	m_inputLayout->Release();
}

const std::string& Shader::GetName() const
{
	return m_config.m_name;
}

