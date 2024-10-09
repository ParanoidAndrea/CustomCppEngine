#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <windows.h>
#include "Engine/Render/Renderer.hpp"
#include "Engine/Render/Camera.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Render/BitmapFont.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "ThirdParty/stb/stb_image.h"
#include "Engine/Render/Shader.hpp"
#include "Engine/Render/DefaultShader.hpp"
#include "Engine/Render/VertexBuffer.hpp"
#include "Engine/Render/ConstantBuffer.hpp"
#include "Engine/Core/Image.hpp"
#include "Engine/Render/IndexBuffer.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")


Renderer::Renderer(RenderConfig const& config)
	:m_config(config) 
{

}

Renderer::~Renderer()
{

}

void Renderer::Startup()
{
	CreateDebugModule();
	CreateDeviceAndSwapChain();
	m_defaultShader = CreateShader("Default", defaultShaderSource);
	BindShader(m_currentShader);
	CreateBuffers();
	SetRasterizerStates();
	CreateBlendStates();
	CreateAndBindDefaultTexture();
	CreateSamplerStates();
	CreateDepthStencilTextureAndView();
	SetStatesIfChanged();
	SetModelConstants();
	CreateEmissiveBloomTextures();
}

void Renderer::BeginFrame()
{
	SetStatesIfChanged();
	ID3D11RenderTargetView* RTVs[] =
	{
		m_backbufferRTV,
		m_emissiveRenderTexture->m_renderTargetView
	};
	m_deviceContext->OMSetRenderTargets(2, RTVs, m_depthStencilView);
	
	//m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
	//m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}

void Renderer::EndFrame()
{
	//Present
	HRESULT hr;
	hr = m_swapChain->Present(0, 0);
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		ERROR_AND_DIE("Device has been lost, application will now termniate.");
	}

}

void Renderer::Shutdown()
{

	DX_SAFE_RELEASE(m_backbufferRTV);
	DX_SAFE_RELEASE(m_swapChain);
	DX_SAFE_RELEASE(m_deviceContext);
	DX_SAFE_RELEASE(m_device);
	DX_SAFE_RELEASE(m_depthStencilTexture);
	DX_SAFE_RELEASE(m_depthStencilView);
	for (int i = 0; i < (int)BlendMode::COUNT; ++i)
    {
		DX_SAFE_RELEASE(m_blendStates[i]);
    }
	for (int i = 0; i < (int)SamplerMode::COUNT; ++i)
	{
		DX_SAFE_RELEASE(m_samplerStates[i]);
	}
	for (int i = 0; i < (int)RasterizerMode::COUNT; ++i)
	{
		DX_SAFE_RELEASE(m_rasterizerStates[i]);
	}
	for (int i = 0; i < (int)DepthMode::COUNT; ++i)
	{
		DX_SAFE_RELEASE(m_depthStencilStates[i]);
	}

	delete m_immediateVBO;
	m_immediateVBO = nullptr;
	delete m_lightingCBO;
	m_lightingCBO = nullptr;
	delete m_cameraCBO;
	m_cameraCBO = nullptr;
	delete m_modelCBO;
	m_modelCBO = nullptr;
	delete m_blurCBO;
	m_blurCBO = nullptr;
	delete m_fullScreenQuadVBO_PCU;
	m_fullScreenQuadVBO_PCU = nullptr;

 	for (int i = 0; i < (int)m_loadedTextures.size(); ++i)
 	{
 		if (m_loadedTextures[i])
 		{
 			delete m_loadedTextures[i];
 			m_loadedTextures[i] = nullptr;
 		}
 	}
	for (int i = 0; i < (int)m_blurUpTextures.size(); ++i)
	{
		if (m_blurUpTextures[i])
		{
			delete m_blurUpTextures[i];
			m_blurUpTextures[i] = nullptr;
		}
	}
	for (int i = 0; i < (int)m_blurDownTextures.size(); ++i)
	{
		if (m_blurDownTextures[i])
		{
			delete m_blurDownTextures[i];
			m_blurDownTextures[i] = nullptr;
		}
	}

	for (int i = 0; i <(int)m_loadedShaders.size() ; ++i)
	{
		if (m_loadedShaders[i])
		{
			delete m_loadedShaders[i];
			m_loadedShaders[i] = nullptr;
		}

	}
	m_defaultTexture = nullptr;
	m_currentTexture = nullptr;
	delete m_emissiveRenderTexture;
	m_emissiveRenderTexture = nullptr;
	delete m_emissiveBlurredRenderTexture;
	m_emissiveBlurredRenderTexture = nullptr;

	//Report error leaks and release debug module
#if defined(ENGINE_DEBUG_RENDER)

	((IDXGIDebug*)m_dxgiDebug)->ReportLiveObjects(DXGI_DEBUG_ALL, (DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL)
	);

	((IDXGIDebug*)m_dxgiDebug)->Release();
	m_dxgiDebug = nullptr;

	::FreeLibrary((HMODULE)m_dxgiDebugModule);
	m_dxgiDebugModule = nullptr;
#endif
}

RenderConfig const Renderer::GetRenderConfig() const
{
	return m_config;
}

void Renderer::ClearScreen( Rgba8 const& clearColor)
{
	float colorAsFloats[4];
	clearColor.GetAsFloats(colorAsFloats);
	m_deviceContext->ClearRenderTargetView(m_backbufferRTV, colorAsFloats);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	float  blackAsFloat[4] = { 0.f,0.f,0.f,1.f };
	m_deviceContext->ClearRenderTargetView(m_emissiveRenderTexture->m_renderTargetView, blackAsFloat);
	m_deviceContext->ClearRenderTargetView(m_emissiveBlurredRenderTexture->m_renderTargetView, blackAsFloat);
	for (int i = 0; i < (int)m_blurUpTextures.size(); ++i)
	{
		m_deviceContext->ClearRenderTargetView(m_blurUpTextures[i]->m_renderTargetView, blackAsFloat);
	}
	for (int i = 0; i < (int)m_blurDownTextures.size(); ++i)
	{
		m_deviceContext->ClearRenderTargetView(m_blurDownTextures[i]->m_renderTargetView, blackAsFloat);
	}
}

void Renderer::SetColor(Rgba8 const& color)
{
	UNUSED(color);
}

void Renderer::BeginCamera( Camera const& camera)
{
	CameraConstants cameraConstant;
	cameraConstant.ProjectionMatrix = camera.GetProjectionMatrix();
	cameraConstant.ViewMatrix = camera.GetViewMatrix();
	CopyCPUToGPU(&cameraConstant, sizeof(cameraConstant), m_cameraCBO);
	BindConstantBuffer(k_cameraConstantsSlot, m_cameraCBO);

	AABB2 dx11viewport = camera.GetDXNormalizedViewport();

	D3D11_VIEWPORT viewport = { };
	viewport.TopLeftX = dx11viewport.m_mins.x * (float)m_config.m_window->GetClientDimensions().x;
	viewport.TopLeftY = dx11viewport.m_mins.y * (float)m_config.m_window->GetClientDimensions().y;
	viewport.Width = (float)m_config.m_window->GetClientDimensions().x * dx11viewport.GetDimensions().x;
	viewport.Height = (float)m_config.m_window->GetClientDimensions().y * dx11viewport.GetDimensions().y;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	m_deviceContext->RSSetViewports(1, &viewport);

}

void Renderer::EndCamera(const Camera& camera)
{
	UNUSED(camera);
}


Texture* Renderer::CreateOrGetTextureFromFile(char const* imageFilePath)
{
	// See if we already have this texture previously loaded
	Texture* existingTexture = GetTextureForFileName(imageFilePath);
	if (existingTexture)
	{
		return existingTexture;
	}

	// Never seen this texture before!  Let's load it.
	Texture* newTexture = CreateTextureFromFile(imageFilePath);
	return newTexture;
}

BitmapFont* Renderer::CreateOrGetBitmapFont(const char* bitmapFontFilePathWithNoExtension)
{
	BitmapFont* existingBitmapFont = GetBitmapFontForFileName(bitmapFontFilePathWithNoExtension);
	if (existingBitmapFont)
	{
		return existingBitmapFont;
	}

	BitmapFont* newBitmapFont = CreateBitmapFont(bitmapFontFilePathWithNoExtension);
	return newBitmapFont;
}

Shader* Renderer::CreateOrGetShaderFromFile(const char* filePath, VertexType type)
{
	Shader* existingShader = GetShaderFromFileName(filePath);
	if (existingShader)
	{
		return existingShader;
	}
	Shader* newShader = CreateShader(filePath, type);
	return newShader;
}

BitmapFont* Renderer::CreateBitmapFont(const char* bitmapFontFilePathWithNoExtension)
{
	std::string bitmapFontPath = std::string(bitmapFontFilePathWithNoExtension);
	bitmapFontPath += ".png";
	Texture* existingTexture = CreateOrGetTextureFromFile(bitmapFontPath.c_str());
	BitmapFont* newBitmapFont = new BitmapFont(bitmapFontPath.c_str(), *existingTexture);
	m_loadedFonts.push_back(newBitmapFont);
	return newBitmapFont;
}

BitmapFont* Renderer::GetBitmapFontForFileName(const char* bitmapFontFilePathWithNoExtension)
{
	for (int index = 0; index < (int)m_loadedFonts.size(); index++)
	{
		if (!strcmp(m_loadedFonts[index]->m_fontFilePathNameWithNoExtension.c_str(), bitmapFontFilePathWithNoExtension))
		{
			return m_loadedFonts[index];
		}
	}
	return nullptr;
}

Texture* Renderer::GetTextureForFileName(char const* imageFilePath)
{
	for (int index = 0; index < (int)m_loadedTextures.size(); index++)
    {
        if (!strcmp(m_loadedTextures[index]->m_name.c_str(), imageFilePath))
        {
            return m_loadedTextures[index];
        }
    }
	return nullptr;
}

Texture* Renderer::CreateTextureFromFile(char const* imageFilePath)
{

	Image* image = CreateImageFromFile(imageFilePath);
	// Check if the load was successful
	GUARANTEE_OR_DIE(image, Stringf("Failed to load image \"%s\"", imageFilePath));
	
	Texture* newTexture = CreateTextureFromImage(*image);
	//Texture* newTexture = CreateTextureFromData(imageFilePath, dimensions, bytesPerTexel, texelData);

	delete image;
	image = nullptr;
	return newTexture;
}


Image* Renderer::CreateImageFromFile(char const* imageFilePath)
{
	Image* image = new Image(imageFilePath);
	return image;
}

//------------------------------------------------------------------------------------------------
Texture* Renderer::CreateTextureFromData(char const* name, IntVec2 dimensions, int bytesPerTexel, uint8_t* texelData)
{
	// Check if the load was successful
	GUARANTEE_OR_DIE(texelData, Stringf("CreateTextureFromData failed for \"%s\" - texelData was null!", name));
	GUARANTEE_OR_DIE(bytesPerTexel >= 3 && bytesPerTexel <= 4, Stringf("CreateTextureFromData failed for \"%s\" - unsupported BPP=%i (must be 3 or 4)", name, bytesPerTexel));
	GUARANTEE_OR_DIE(dimensions.x > 0 && dimensions.y > 0, Stringf("CreateTextureFromData failed for \"%s\" - illegal texture dimensions (%i x %i)", name, dimensions.x, dimensions.y));

	Texture* newTexture = new Texture();
 	newTexture->m_name = name; // NOTE: m_name must be a std::string, otherwise it may point to temporary data!
 	newTexture->m_dimensions = dimensions;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = newTexture->m_dimensions.x;
	textureDesc.Height = newTexture->m_dimensions.y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	D3D11_SUBRESOURCE_DATA textureData;
	textureData.pSysMem = texelData;
	textureData.SysMemPitch = 4 * newTexture->m_dimensions.x;

	HRESULT hr;
	hr = m_device->CreateTexture2D(&textureDesc, &textureData, &newTexture->m_texture);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("CreateTextureFromImage failed for image file \"%s\".", newTexture->m_name.c_str()));
	}

	hr = m_device->CreateShaderResourceView(newTexture->m_texture, NULL, &newTexture->m_shaderResourceView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("CreateShaderResourceView failed for image file \"%s\".", newTexture->m_name.c_str()));
	}

	m_loadedTextures.push_back(newTexture);
	return newTexture;
}

Texture* Renderer::CreateTextureFromImage(const Image& image)
{
	Texture* newTexture = new Texture();
	newTexture->m_name = image.GetImageFilePath();
	newTexture->m_dimensions = image.GetDimensions();
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = image.GetDimensions().x;
	textureDesc.Height = image.GetDimensions().y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA textureData;
	textureData.pSysMem = image.GetRawData();
	textureData.SysMemPitch = 4 * image.GetDimensions().x;

	HRESULT hr;
	hr = m_device->CreateTexture2D(&textureDesc, &textureData, &newTexture->m_texture);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("CreateTextureFromImage failed for image file \"%s\".", image.GetImageFilePath().c_str()));
	}

	hr = m_device->CreateShaderResourceView(newTexture->m_texture, NULL, &newTexture->m_shaderResourceView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("CreateShaderResourceView failed for image file \"%s\".", image.GetImageFilePath().c_str()));
	}
	m_loadedTextures.push_back(newTexture);
	return newTexture;
}

Texture* Renderer::CreateRenderTexture(IntVec2 const& dimensions, char const* name)
{
	Texture* newRenderTexture = new Texture();
	//newRenderTexture->m_renderTargetView = m_renderTargetView;
	newRenderTexture->m_name = name;
	newRenderTexture->m_dimensions = dimensions;
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width  = dimensions.x;
	textureDesc.Height = dimensions.y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT hr;
	hr = m_device->CreateTexture2D(&textureDesc, NULL, &newRenderTexture->m_texture);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("CreateRenderTexture failed for \"%s\".", name));
	}
	hr = m_device->CreateRenderTargetView(newRenderTexture->m_texture, NULL, &newRenderTexture->m_renderTargetView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Create render target view failed for \"%s\".", name));
	}

	hr = m_device->CreateShaderResourceView(newRenderTexture->m_texture, NULL, &newRenderTexture->m_shaderResourceView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Create shader resouce view failed for \"%s\".", name));
	}
	return newRenderTexture;
}

void Renderer::CreateDebugModule()
{
#if defined(ENGINE_DEBUG_RENDER)
	m_dxgiDebugModule = (void*)::LoadLibraryA("dxgidebug.dll");
	if (m_dxgiDebugModule == nullptr)
	{
		ERROR_AND_DIE("Could not load dxgidebug.dll.");
	}
	typedef HRESULT(WINAPI* GetDebugModuleCB)(REFIID, void**);
	((GetDebugModuleCB)::GetProcAddress((HMODULE)m_dxgiDebugModule, "DXGIGetDebugInterface"))(__uuidof(IDXGIDebug), &m_dxgiDebug);

	if (m_dxgiDebug == nullptr)
	{
		ERROR_AND_DIE("Could not load debug module.");
	}

#endif
}

void Renderer::CreateDeviceAndSwapChain()
{
	//Render Startup
	unsigned int deviceFlags = 0;
#if defined(ENGINE_DEBUG_RENDER)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//Create device and swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferDesc.Width = m_config.m_window->GetClientDimensions().x;
	swapChainDesc.BufferDesc.Height = m_config.m_window->GetClientDimensions().y;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = (HWND)m_config.m_window->GetHwnd();
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceFlags, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, nullptr, &m_deviceContext);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create D3D 11 device and swap chain");
	}

	//Get back buffer texture
	ID3D11Texture2D* backBuffer;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not get swap chain buffer.");
	}

	hr = m_device->CreateRenderTargetView(backBuffer, NULL, &m_backbufferRTV);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could create render target view for swap chain buffer.");
	}

	backBuffer->Release();
}

void Renderer::CreateBuffers()
{
	Vertex_PCU vertices[1];
	size_t vertexBuffersize = sizeof(vertices);
	size_t cameraConstantsSize = sizeof(CameraConstants);
	size_t modelConstantsSize = sizeof(ModelConstants);
	m_immediateVBO = CreateVertexBuffer(vertexBuffersize);
	size_t lightingConstantsSize = sizeof(LightConstants);
	m_lightingCBO = CreateConstantBuffer(lightingConstantsSize);
	m_cameraCBO = CreateConstantBuffer(cameraConstantsSize);
	m_modelCBO = CreateConstantBuffer(modelConstantsSize);
	m_blurCBO = CreateConstantBuffer(sizeof(BlurConstants));


	// Create fullscreen quad vertices in NDC (normalized device coordinates)
	std::vector<Vertex_PCU> fullscreenVerts;


	fullscreenVerts.push_back(Vertex_PCU(Vec3(-1.f, -1.f, 0.f), Rgba8::WHITE, Vec2(0.f, 1.f)));
	fullscreenVerts.push_back(Vertex_PCU(Vec3(1.f, -1.f, 0.f), Rgba8::WHITE, Vec2(1.f, 1.f)));
	fullscreenVerts.push_back(Vertex_PCU(Vec3(1.f, 1.f, 0.f), Rgba8::WHITE, Vec2(1.f, 0.f)));
	fullscreenVerts.push_back(Vertex_PCU(Vec3(-1.f, -1.f, 0.f), Rgba8::WHITE, Vec2(0.f, 1.f)));
	fullscreenVerts.push_back(Vertex_PCU(Vec3(1.f, 1.f, 0.f), Rgba8::WHITE, Vec2(1.f, 0.f)));
	fullscreenVerts.push_back(Vertex_PCU(Vec3(-1.f, 1.f, 0.f), Rgba8::WHITE, Vec2(0.f, 0.f)));

	// Create the vertex buffer for the fullscreen quad
	m_fullScreenQuadVBO_PCU = CreateVertexBuffer(fullscreenVerts.size() * sizeof(Vertex_PCU));

	// Copy the vertex data to the GPU
	CopyCPUToGPU(fullscreenVerts.data(), fullscreenVerts.size() * sizeof(Vertex_PCU), m_fullScreenQuadVBO_PCU);
}

Shader* Renderer::GetShaderFromFileName(char const* filePath)
{
	for (int index = 0; index < (int)m_loadedShaders.size(); index++)
	{
	    if (!strcmp(m_loadedShaders[index]->GetName().c_str(), filePath))
	    {
	    	return m_loadedShaders[index];
	    }
	}
	return nullptr;
}

Shader* Renderer::CreateShader(char const* shaderName, char const* shaderSource, VertexType vertexType)
{
	ShaderConfig shaderConfig;
	shaderConfig.m_name = shaderName;
	Shader* shader = new Shader(shaderConfig);
	std::vector<uint8_t> vertexShaderByteCode;
	std::vector<uint8_t> pixelShaderByteCode;
	CompileShaderToByteCode(vertexShaderByteCode, "VertexShader", shaderSource, "VertexMain", "vs_5_0");
	CompileShaderToByteCode(pixelShaderByteCode, "PixelShader", shaderSource, "PixelMain", "ps_5_0");

	HRESULT hr;
	//Create vertex shader
	hr = m_device->CreateVertexShader(
		vertexShaderByteCode.data(),
		vertexShaderByteCode.size(),
		NULL, &shader->m_vertexShader
	);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Could not create vertex shader."));
	}

	//Create pixel shader
	hr = m_device->CreatePixelShader(
		pixelShaderByteCode.data(),
		pixelShaderByteCode.size(),
		NULL, &shader->m_pixelShader
	);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Could not create pixel shader."));
	}
	if (vertexType == VertexType::Vertex_PCU)
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		   {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			  0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		   {"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM,
			  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		   {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
			  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		UINT numElements = ARRAYSIZE(inputElementDesc);
		hr = m_device->CreateInputLayout(
			inputElementDesc, numElements,
			vertexShaderByteCode.data(),
			vertexShaderByteCode.size(),
			&shader->m_inputLayout
		);
	}
	else if (vertexType == VertexType::Vertex_PCUTBN)
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		   {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			  0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		   {"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM,
			  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		   {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
			  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		   {"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		   {"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		   {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		UINT numElements = ARRAYSIZE(inputElementDesc);
		hr = m_device->CreateInputLayout(
			inputElementDesc, numElements,
			vertexShaderByteCode.data(),
			vertexShaderByteCode.size(),
			&shader->m_inputLayout
		);
	}


	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create vertex layout.");
	}
	m_loadedShaders.push_back(shader);
	return shader;
}

Shader* Renderer::CreateShader(char const* shaderName, VertexType vertexType)
{
	std::string source;
    FileReadToString(source, (std::string(shaderName) + ".hlsl").c_str());
	char const* shaderSource = source.c_str();
	Shader* shader = CreateShader(shaderName, shaderSource,vertexType);
	return shader;
}

void Renderer::CreateAndBindDefaultTexture()
{
	Image* defaultimage = new Image(IntVec2(2, 2), Rgba8::WHITE);
	m_defaultTexture = CreateTextureFromImage(*defaultimage);
	BindTexture(m_defaultTexture);

}

bool Renderer::CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name, char const* source, char const* entryPoint, char const* target)
{
	DWORD shaderFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#if defined(ENGINE_DEBUG_RENDER)
	shaderFlags = D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	ID3DBlob* shaderBlob = NULL;
	ID3DBlob* errorBlob = NULL;

	HRESULT hr;
	hr = D3DCompile(
		source, strlen(source),
		name, nullptr, nullptr,
		entryPoint, target, shaderFlags, 0,
		&shaderBlob, &errorBlob
	);
	if (SUCCEEDED(hr))
	{
		outByteCode.resize(shaderBlob->GetBufferSize());
		memcpy(
			outByteCode.data(),
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize()
		);
	}
	else
	{
		if (errorBlob != NULL)
		{
			DebuggerPrintf((char*)errorBlob->GetBufferPointer());
		}
		ERROR_AND_DIE("Could not compile the shader.");
	}

	shaderBlob->Release();
	if (errorBlob != NULL)
	{
		errorBlob->Release();
	}

	return false;
}

void Renderer::BindBlurCBO(BlurConstants const& blurConstants)
{
	CopyCPUToGPU(&blurConstants, sizeof(blurConstants), m_blurCBO);
	BindConstantBuffer(k_blurConstantsSlot, m_blurCBO);
}

void Renderer::BindShader(Shader* shader)
{
	if (shader == nullptr)
	{
		shader = m_defaultShader;
	}
	m_deviceContext->VSSetShader(shader->m_vertexShader, nullptr, 0);
	m_deviceContext->PSSetShader(shader->m_pixelShader, nullptr, 0);
	m_deviceContext->IASetInputLayout(shader->m_inputLayout);
}

VertexBuffer* Renderer::CreateVertexBuffer(const size_t size)
{
	D3D11_BUFFER_DESC bufferDesc = { };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HRESULT hr;
	VertexBuffer* vertexBuffer = new VertexBuffer(size);
	hr = m_device->CreateBuffer(&bufferDesc, nullptr, &vertexBuffer->m_buffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create vertex buffer.");
	}
	return vertexBuffer;
}

IndexBuffer* Renderer::CreateIndexBuffer(size_t const size)
{
	D3D11_BUFFER_DESC bufferDesc = { };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HRESULT hr;
	IndexBuffer* indexBuffer = new IndexBuffer((UINT)size);
	hr = m_device->CreateBuffer(&bufferDesc, nullptr, &indexBuffer->m_buffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create index buffer.");
	}
	return indexBuffer;
}



void Renderer::SetModelConstants(Mat44 const& modelMatrix, Rgba8 const& modelColor)
{
	ModelConstants modelConstant;
	modelConstant.ModelMatrix = modelMatrix;
	modelColor.GetAsFloats(modelConstant.ModelColor);
	CopyCPUToGPU(&modelConstant, sizeof(modelConstant), m_modelCBO);
	BindConstantBuffer(k_modelConstantsSlot, m_modelCBO);

}

void Renderer::SetLightingConstants(Vec3 sunDirection, float sunIntensity, float ambientIntensity)
{
	LightConstants lightConstant;
	lightConstant.SunDirection = sunDirection;
	//lightConstant->SunDirection[0] = sunDirection.x;
	//lightConstant->SunDirection[1] = sunDirection.y;
	//lightConstant->SunDirection[2] = sunDirection.z;
	sunIntensity = GetClamped(sunIntensity, 0.f, 1.f);
	ambientIntensity = GetClamped(ambientIntensity, 0.f, 1.f);
	lightConstant.SunIntensity = sunIntensity;
	lightConstant.AmbientIntensity = ambientIntensity;
	CopyCPUToGPU(&lightConstant, sizeof(lightConstant), m_lightingCBO);
	BindConstantBuffer(k_lightingConstantsSlot, m_lightingCBO);

}

void Renderer::SetLightingConstants(LightConstants const& lightConstants)
{
	CopyCPUToGPU(&lightConstants, sizeof(lightConstants), m_lightingCBO);
	BindConstantBuffer(k_lightingConstantsSlot, m_lightingCBO);
}

void Renderer::DrawVertexArray(int numVertexes, const Vertex_PCU* vertexes)
{
	SetStatesIfChanged();
	size_t size = sizeof(Vertex_PCU)*numVertexes;
	CopyCPUToGPU(vertexes, size, m_immediateVBO);
	DrawVertexBuffer(m_immediateVBO, numVertexes,VertexType::Vertex_PCU);
}

void Renderer::DrawVertexArray(int numVertexes, const Vertex_PCUTBN* vertexes)
{
	SetStatesIfChanged();
	size_t size = sizeof(Vertex_PCUTBN) * numVertexes;
	CopyCPUToGPU(vertexes, size, m_immediateVBO);
	DrawVertexBuffer(m_immediateVBO, numVertexes, VertexType::Vertex_PCUTBN);
}

void Renderer::DrawVertexArrayWithIBO(int indexCount, VertexBuffer* vbo, IndexBuffer* ibo)
{
	SetStatesIfChanged();
	BindVertexBuffer(vbo);
	BindIndexBuffer(ibo);

	m_deviceContext->DrawIndexed(indexCount, 0, 0);
}

void Renderer::DrawVertexTBN(int indexCount, VertexBuffer* vbo, IndexBuffer* ibo)
{
	SetStatesIfChanged();
	//size_t size = sizeof(Vertex_PCUTBN) * numVertexes;
	BindVertexBufferTBN(vbo);
	BindIndexBuffer(ibo);
	//CopyCPUToGPU(verts, size, m_immediateVBO);

	m_deviceContext->DrawIndexed(indexCount, 0, 0);
}

void Renderer::CopyCPUToGPU(const void* data, size_t size, VertexBuffer*& vbo)
{
	if ((UINT)vbo->m_size < (UINT)size)
	{
		delete vbo;
		vbo = CreateVertexBuffer(size);
	}
	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(vbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap(vbo->m_buffer, 0);
}


void Renderer::CopyCPUToGPU(const void* data, size_t size, IndexBuffer*& ibo)
{
	if ((UINT)ibo->m_size < (UINT)size)
	{
		delete ibo;
		ibo = CreateIndexBuffer(size);
	}
	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(ibo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap(ibo->m_buffer, 0);
}

void Renderer::DrawVertexBuffer(VertexBuffer* vbo, int vertexCount, VertexType type , int vertexOffset )
{
	SetStatesIfChanged();
	if (type == VertexType::Vertex_PCU)
	{
		BindVertexBuffer(vbo);
	}
	else if (type == VertexType::Vertex_PCUTBN)
	{
		BindVertexBufferTBN(vbo);
	}
	m_deviceContext->Draw(vertexCount, vertexOffset);

}

void Renderer::DrawVertexBuffer(VertexBuffer* vbo, VertexType type, int vertexOffset)
{
	if (type == VertexType::Vertex_PCU)
	{
		BindVertexBuffer(vbo);
	}
	else if (type == VertexType::Vertex_PCUTBN)
	{
		BindVertexBufferTBN(vbo);
	}
	m_deviceContext->Draw((UINT)vbo->m_size, vertexOffset);
}

void Renderer::SetStatesIfChanged()
{
	if (m_blendStates[(int)m_desiredBlendMode] != m_blendState)
	{
		m_blendState = m_blendStates[(int)m_desiredBlendMode];
		float blendFactor[4] = { 0.f,0.f,0.f,0.f };
		UINT sampleMask = 0xffffffff;
		m_deviceContext->OMSetBlendState(m_blendState, blendFactor, sampleMask);
	}
	if (m_samplerStates[(int)m_desiredSamplerMode] != m_samplerState)
	{
		m_samplerState = m_samplerStates[(int)m_desiredSamplerMode];
		m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	}
	if (m_rasterizerStates[(int)m_desiredRasterizerMode] != m_rasterizerState)
	{
		m_rasterizerState = m_rasterizerStates[(int)m_desiredRasterizerMode];
		m_deviceContext->RSSetState(m_rasterizerState);
	}
	if (m_depthStencilStates[(int)m_desiredDepthMode] != m_depthStencilState)
	{
		m_depthStencilState = m_depthStencilStates[(int)m_desiredDepthMode];
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	}
}

void Renderer::BindVertexBuffer(VertexBuffer* vbo)
{
	UINT stride = (UINT)sizeof(Vertex_PCU);
	UINT startOffset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &vbo->m_buffer, &stride, &startOffset);
	m_deviceContext->IASetPrimitiveTopology(vbo->m_isLinePrimitive? D3D11_PRIMITIVE_TOPOLOGY_LINELIST :D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void Renderer::BindVertexBufferTBN(VertexBuffer* vbo)
{
	UINT stride = (UINT)sizeof(Vertex_PCUTBN);
	UINT startOffset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &vbo->m_buffer, &stride, &startOffset);
	m_deviceContext->IASetPrimitiveTopology(vbo->m_isLinePrimitive? D3D11_PRIMITIVE_TOPOLOGY_LINELIST :D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::BindIndexBuffer(IndexBuffer* ibo)
{
	m_deviceContext->IASetIndexBuffer(ibo->m_buffer,DXGI_FORMAT_R32_UINT,0);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::CreateBackBufferRTV()
{
	// Create the back buffer render target view (RTV)
	ID3D11Texture2D* backBuffer = nullptr;
	HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

	if (FAILED(hr)) 
	{
		ERROR_AND_DIE("Could not create back buffer.");
	}

	hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_backbufferRTV);
	if (FAILED(hr))
	{
		ERROR_AND_DIE("Could not create RTV for back buffer.");
	}

	backBuffer->Release(); 
}

void Renderer::CreateEmissiveBloomTextures()
{
	m_emissiveRenderTexture = CreateRenderTexture(m_config.m_window->GetClientDimensions(), "Emissive");
	m_emissiveBlurredRenderTexture = CreateRenderTexture(m_config.m_window->GetClientDimensions(), "Blurred Emissive");

	IntVec2 viewportSize = m_config.m_window->GetClientDimensions();
	float aspectRatio = (float)viewportSize.x / (float)viewportSize.y;

	while (viewportSize.y > 64)
	{
		viewportSize.y /= 2;
		viewportSize.x = static_cast<int>(viewportSize.y * aspectRatio);

		Texture* blurDownTexture = CreateRenderTexture(viewportSize, "BlurDown");
		m_blurDownTextures.push_back(blurDownTexture);
		if (viewportSize.y > 64)
		{
			Texture* blurUpTexture = CreateRenderTexture(viewportSize, "BlurUp");
			m_blurUpTextures.push_back(blurUpTexture);
		}
	}
    std::reverse(m_blurUpTextures.begin(), m_blurUpTextures.end());

}

ConstantBuffer* Renderer::CreateConstantBuffer(const size_t size)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HRESULT hr;
	ConstantBuffer* constantBuffer = new ConstantBuffer(size);
	hr = m_device->CreateBuffer(&bufferDesc, nullptr, &constantBuffer->m_buffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create constant buffer.");
	}
	return constantBuffer;
}


void Renderer::CopyCPUToGPU(const void* data, size_t size, ConstantBuffer* cbo)
{
	if (size > cbo->m_size)
	{
		ERROR_RECOVERABLE("The size of the contant buffer is not large enough");
	}
	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(cbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, (UINT)cbo->m_size);
	m_deviceContext->Unmap(cbo->m_buffer, 0);
}

void Renderer::BindConstantBuffer(int slot, ConstantBuffer* cbo)
{
	m_deviceContext->VSSetConstantBuffers(slot, 1, &cbo->m_buffer);
	m_deviceContext->PSSetConstantBuffers(slot, 1, &cbo->m_buffer);
}

void Renderer::RenderEmissive()
{
	SetDepthMode(DepthMode::DISABLED);
	SetBlendMode(BlendMode::OPAQUE);
	SetSamplerMode(SamplerMode::BILINEAR_CLAMP);
	SetRasterizerMode(RasterizerMode::SOLID_CULL_BACK);
	//SetStatesIfChanged();
	ID3D11RenderTargetView* nullRTVs[2] = { nullptr, nullptr };
	ID3D11ShaderResourceView* nullSRV[2] = { nullptr, nullptr };


	D3D11_VIEWPORT viewport = { };
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;


	//Blur Down
	BlurConstants blurConstants;
	SetBlurConstantsBlurDown(blurConstants);
	BindShader(m_blurDownShader);
	for (int i = 0; i < (int)m_blurDownTextures.size(); ++i)
	{
		Texture* currentBlurDown = m_blurDownTextures[i];
		Texture* nextBlurDown = (i == 0) ? m_emissiveRenderTexture : m_blurDownTextures[i - 1];
		m_deviceContext->OMSetRenderTargets(1, &currentBlurDown->m_renderTargetView, nullptr);
		BindTexture(nextBlurDown);

		blurConstants.TexelSize.x = 1.f / (float)nextBlurDown->m_dimensions.x;
		blurConstants.TexelSize.y = 1.f / (float)nextBlurDown->m_dimensions.y;

		BindBlurCBO(blurConstants);

		viewport.Width  = (float)currentBlurDown->m_dimensions.x;
		viewport.Height = (float)currentBlurDown->m_dimensions.y;

		m_deviceContext->RSSetViewports(1, &viewport);

		DrawVertexBuffer(m_fullScreenQuadVBO_PCU,6);

		m_deviceContext->OMSetRenderTargets(2, nullRTVs, nullptr);
		//m_deviceContext->PSSetShaderResources(0, 2, nullSRV);
	}

	//Blur Up
	SetBlurConstantsBlurUp(blurConstants);
	BindShader(m_blurUpShader);
	for (int i = 0; i < (int)m_blurUpTextures.size(); ++i)
	{
		Texture* currentBlurUp = m_blurUpTextures[i];
		Texture* nextBlurUp = (i == 0) ? m_blurDownTextures.back() : m_blurUpTextures[i - 1];
		Texture* sameSizeBlurDownTexture = m_blurDownTextures[m_blurDownTextures.size()-2-i];

	    m_deviceContext->OMSetRenderTargets(1, &currentBlurUp->m_renderTargetView, nullptr);
		BindTexture(sameSizeBlurDownTexture, 0);
		BindTexture(nextBlurUp, 1);

		blurConstants.TexelSize.x = 1.f / nextBlurUp->m_dimensions.x;
		blurConstants.TexelSize.y = 1.f / nextBlurUp->m_dimensions.y;

		BindBlurCBO(blurConstants);

		viewport.Width  = (float)currentBlurUp->m_dimensions.x;
		viewport.Height = (float)currentBlurUp->m_dimensions.y;

		m_deviceContext->RSSetViewports(1, &viewport);
	    
		DrawVertexBuffer(m_fullScreenQuadVBO_PCU,6);

		m_deviceContext->OMSetRenderTargets(2, nullRTVs, nullptr);
		//m_deviceContext->PSSetShaderResources(0, 2, nullSRV);
		//m_deviceContext->PSSetShaderResources(1, 2, nullSRV);
	}

	m_deviceContext->OMSetRenderTargets(1, &m_emissiveRenderTexture->m_renderTargetView, nullptr);
	BindTexture(m_emissiveBlurredRenderTexture, 0);
	BindTexture(m_blurUpTextures[m_blurUpTextures.size()-1], 1);

	blurConstants.TexelSize.x = 1.f / m_blurUpTextures[m_blurUpTextures.size()-1]->m_dimensions.x;
	blurConstants.TexelSize.y = 1.f / m_blurUpTextures[m_blurUpTextures.size()-1]->m_dimensions.y;

	BindBlurCBO(blurConstants);

	viewport.Width = (float)m_config.m_window->GetClientDimensions().x;
	viewport.Height = (float)m_config.m_window->GetClientDimensions().y;

	m_deviceContext->RSSetViewports(1, &viewport);
	DrawVertexBuffer(m_fullScreenQuadVBO_PCU,6);

	m_deviceContext->OMSetRenderTargets(2, nullRTVs, nullptr);
	//m_deviceContext->PSSetShaderResources(0, 2, nullSRV);

	//Composite
	m_deviceContext->OMSetRenderTargets(1, &m_backbufferRTV, nullptr);
	BindShader(m_compositeShader);
	BindTexture(m_emissiveRenderTexture);
	SetBlendMode(BlendMode::ADDITIVE);
	//SetStatesIfChanged();
	DrawVertexBuffer(m_fullScreenQuadVBO_PCU,6);
	SetBlendMode(BlendMode::OPAQUE);
	BindShader(nullptr);
	BindTexture(nullptr);
	SetSamplerMode(SamplerMode::POINT_CLAMP);
}

ID3D11Device* Renderer::GetDevice() const
{
	return m_device;
}

ID3D11DeviceContext* Renderer::GetDeviceContext() const
{
	return m_deviceContext;
}

void Renderer::SetBlurConstantsBlurDown(BlurConstants &blurConstants)
{
	blurConstants.LerpT = 1.f;
	blurConstants.NumSamples = 13;
	blurConstants.Samples[0].Offset = Vec2(-2.f,2.f);
	blurConstants.Samples[0].Weight = 0.0323f;

	blurConstants.Samples[1].Offset = Vec2(-2.f, -2.f);
	blurConstants.Samples[1].Weight = 0.0323f;

	blurConstants.Samples[2].Offset = Vec2(2.f, -2.f);
	blurConstants.Samples[2].Weight = 0.0323f;

	blurConstants.Samples[3].Offset = Vec2(2.f, 2.f);
	blurConstants.Samples[3].Weight = 0.0323f;

	blurConstants.Samples[4].Offset = Vec2(0.f, 2.f);
	blurConstants.Samples[4].Weight = 0.0645f;

	blurConstants.Samples[5].Offset = Vec2(0.f, -2.f);
	blurConstants.Samples[5].Weight = 0.0645f;

	blurConstants.Samples[6].Offset = Vec2(2.f, 0.f);
	blurConstants.Samples[6].Weight = 0.0645f;

	blurConstants.Samples[7].Offset = Vec2(-2.f, 0.f);
	blurConstants.Samples[7].Weight = 0.0645f;

	blurConstants.Samples[8].Offset = Vec2(-1.f, 1.f);
	blurConstants.Samples[8].Weight = 0.129f;

	blurConstants.Samples[9].Offset = Vec2(-1.f, -1.f);
	blurConstants.Samples[9].Weight = 0.129f;

	blurConstants.Samples[10].Offset = Vec2(1.f, 1.f);
	blurConstants.Samples[10].Weight = 0.129f;

	blurConstants.Samples[11].Offset = Vec2(1.f, -1.f);
	blurConstants.Samples[11].Weight = 0.129f;

	blurConstants.Samples[12].Offset = Vec2(0.f, 0.f);
	blurConstants.Samples[12].Weight = 0.0968f;


}

void Renderer::SetBlurConstantsBlurUp(BlurConstants &blurConstants)
{
	blurConstants.LerpT = 0.85f;
	blurConstants.NumSamples = 9;
	blurConstants.Samples[0].Offset = Vec2(-1.f, 1.f);
	blurConstants.Samples[0].Weight = 0.0625f;

	blurConstants.Samples[1].Offset = Vec2(-1.f, -1.f);
	blurConstants.Samples[1].Weight = 0.0625f;

	blurConstants.Samples[2].Offset = Vec2(1.f, -1.f);
	blurConstants.Samples[2].Weight = 0.0625f;

	blurConstants.Samples[3].Offset = Vec2(1.f, 1.f);
	blurConstants.Samples[3].Weight = 0.0625f;

	blurConstants.Samples[4].Offset = Vec2(0.f, 1.f);
	blurConstants.Samples[4].Weight = 0.125f;

	blurConstants.Samples[5].Offset = Vec2(0.f, -1.f);
	blurConstants.Samples[5].Weight = 0.125f;

	blurConstants.Samples[6].Offset = Vec2(1.f, 0.f);
	blurConstants.Samples[6].Weight = 0.125f;

	blurConstants.Samples[7].Offset = Vec2(-1.f, 0.f);
	blurConstants.Samples[7].Weight = 0.125f;

	blurConstants.Samples[8].Offset = Vec2(0.f, 0.f);
	blurConstants.Samples[8].Weight = 0.25f;
}

void Renderer::SetRasterizerStates()
{
	HRESULT hr;
	//Set rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc = {  };
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.AntialiasedLineEnable = true;
	hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStates[(int)RasterizerMode::SOLID_CULL_NONE]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create the SOLID_CULL_NONE rasterizer state.");
	}

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStates[(int)RasterizerMode::SOLID_CULL_BACK]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create the SOLID_CULL_BACK rasterizer state.");
	}

	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStates[(int)RasterizerMode::WIREFRAME_CULL_NONE]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create the WIREFRAME_CULL_NONE rasterizer state.");
	}

	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStates[(int)RasterizerMode::WIREFRAME_CULL_BACK]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create the WIREFRAME_CULL_BACK rasterizer state.");
	}
	SetRasterizerMode(m_desiredRasterizerMode);
}



void Renderer::CreateBlendStates()
{
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp   = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha  = blendDesc.RenderTarget[0].SrcBlend;
	blendDesc.RenderTarget[0].DestBlendAlpha = blendDesc.RenderTarget[0].DestBlend ;
	blendDesc.RenderTarget[0].BlendOpAlpha   = blendDesc.RenderTarget[0].BlendOp ;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT hr;
	hr = m_device->CreateBlendState(&blendDesc, &m_blendStates[(int)BlendMode::OPAQUE]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Create blend state for BlendMode::OPAQUE failed.");
	}

	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = blendDesc.RenderTarget[0].SrcBlend;
	blendDesc.RenderTarget[0].DestBlendAlpha = blendDesc.RenderTarget[0].DestBlend;
	blendDesc.RenderTarget[0].BlendOpAlpha = blendDesc.RenderTarget[0].BlendOp;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = m_device->CreateBlendState(&blendDesc, &m_blendStates[(int)BlendMode::ALPHA]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Create blend state for BlendMode::ALPHA failed.");
	}
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = blendDesc.RenderTarget[0].SrcBlend;
	blendDesc.RenderTarget[0].DestBlendAlpha = blendDesc.RenderTarget[0].DestBlend;
	blendDesc.RenderTarget[0].BlendOpAlpha = blendDesc.RenderTarget[0].BlendOp;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = m_device->CreateBlendState(&blendDesc, &m_blendStates[(int)BlendMode::ADDITIVE]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Create blend state for BlendMode::ADDITIVE failed.");
	}
	SetBlendMode(m_desiredBlendMode);
}

void Renderer::CreateSamplerStates()
{
	HRESULT hr;
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerStates[(int)SamplerMode::POINT_CLAMP]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateSamplerStates for SamplerMode::POINT_CLAMP failed.");
	}
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerStates[(int)SamplerMode::BILINEAR_CLAMP]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateSamplerStates for SamplerMode::BILINEAR_CLAMP failed.");
	}

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerStates[(int)SamplerMode::BILINEAR_WRAP]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateSamplerStates for SamplerMode::BILINEAR_WRAP failed.");
	}

	SetSamplerMode(m_desiredSamplerMode);
}

void Renderer::CreateDepthStencilTextureAndView()
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = m_config.m_window->GetClientDimensions().x;
	textureDesc.Height = m_config.m_window->GetClientDimensions().y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.SampleDesc.Count = 1;

	HRESULT hr;
	hr = m_device->CreateTexture2D(&textureDesc, nullptr, &m_depthStencilTexture);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create texture for depth stencil");

	}

	hr = m_device->CreateDepthStencilView(m_depthStencilTexture, nullptr, &m_depthStencilView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create depth stencil view");
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStates[(int)DepthMode::DISABLED]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create depth stencil mode DISABLED");
	}

	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStates[(int)DepthMode::ENABLED]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create depth stencil mode ENABLED");
	}
	SetDepthMode(m_desiredDepthMode);
}

void Renderer::BindTexture(Texture const* texture, unsigned int slot)
{
	if (texture)
	{
		m_deviceContext->PSSetShaderResources(slot, 1, &texture->m_shaderResourceView);
	}
	else
	{
		m_deviceContext->PSSetShaderResources(slot, 1, &m_defaultTexture->m_shaderResourceView);
	}
}

void Renderer::CreateBloomShaders(char const* blurDownFilename, char const* blurUpFilename, char const* compositeFilename)
{
	m_blurUpShader = CreateOrGetShaderFromFile(blurUpFilename, VertexType::Vertex_PCU);
	m_blurDownShader = CreateOrGetShaderFromFile(blurDownFilename, VertexType::Vertex_PCU);
	m_compositeShader = CreateOrGetShaderFromFile(compositeFilename, VertexType::Vertex_PCU);

}

// void Renderer::BindTexture(Texture const* diffuseTexture,Texture const* normalTexture, Texture const* specularTexture)
// {
// 	ID3D11ShaderResourceView* textures[] = { m_defaultTexture->m_shaderResourceView, m_defaultTexture->m_shaderResourceView,m_defaultTexture->m_shaderResourceView };
// 
// 	if (diffuseTexture)
// 	{
// 		textures[0] = diffuseTexture->m_shaderResourceView;
// 	}
// 
// 	if (normalTexture)
// 	{
// 		textures[1] = normalTexture->m_shaderResourceView;
// 	}
// 
// 	if (specularTexture)
// 	{
// 		textures[2] = specularTexture->m_shaderResourceView;
// 	}
// 
// 	m_deviceContext->PSSetShaderResources(0, 3, textures);
// }

void Renderer::SetBlendMode(BlendMode blendMode)
{
	m_desiredBlendMode = blendMode;

}

void Renderer::SetSamplerMode(SamplerMode sampleMode)
{
	m_desiredSamplerMode = sampleMode;
}

void Renderer::SetRasterizerMode(RasterizerMode rasterizerMode)
{
	m_desiredRasterizerMode = rasterizerMode;

}

void Renderer::SetDepthMode(DepthMode depthMode)
{
	m_desiredDepthMode = depthMode;
}



