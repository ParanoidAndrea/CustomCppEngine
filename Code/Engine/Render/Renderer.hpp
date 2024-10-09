#pragma once
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Render/Camera.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Render/Texture.hpp"
#include "Engine/Math/IntVec2.hpp"

#include "Game/EngineBuildPreferences.hpp"
#define DX_SAFE_RELEASE(dxobject) \
{\
if (dxobject != nullptr)\
(dxobject)->Release();\
(dxobject) = nullptr;\
}

#include <vector>
#if defined(OPAQUE)
#undef OPAQUE
#endif

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11RasterizerState;
struct ID3D11BlendState;
struct ID3D11SamplerState;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilState;

struct RenderConfig
{
	Window* m_window;
};
struct LightingDebug
{
	int	   RenderAmbient    = true;
	int	   RenderDiffuse    = true;
	int	   RenderSpecular   = true;
	int	   RenderEmissive   = true;
	int	   UseDiffuseMap    = true;
	int	   UseNormalMap    = true;
	int	   UseSpecularMap   = true;
	int	   UseGlossinessMap = true;
	int	   UseEmissiveMap   = true;

	float Padding[3] = {0.f,0.f,0.f};
};
struct LightConstants
{
	Vec3 SunDirection = Vec3(0.f, 0.f, -1.f);
	float SunIntensity = 1.f;
	float AmbientIntensity = 0.f;
	Vec3 WorldEyePosition = Vec3();

	float MinFalloff = 0.f;
	float MaxFalloff = 0.1f;
	float MinFalloffMultiplier = 0.f;
	float MaxFalloffMultiplier = 1.f;
	LightingDebug LightingDebugs;
};
static const int k_lightingConstantsSlot = 1;
static const int k_cameraConstantsSlot = 2;
static const int k_modelConstantsSlot = 3;
static const int k_blurConstantsSlot = 5;
static const int k_blurMaxSamples = 64;
struct CameraConstants
{
	Mat44 ViewMatrix;
	Mat44 ProjectionMatrix;
};
struct ModelConstants
{
	Mat44 ModelMatrix;
	float ModelColor[4] = {0.f,0.f,0.f,1.f};
};
struct BlurSample
{
	Vec2 Offset = Vec2();
	float Weight = 0.f;
	int Padding = 0;
};
struct BlurConstants
{
	Vec2 TexelSize = Vec2();
	float LerpT = 0.f;
	int NumSamples = 0;
	BlurSample Samples[k_blurMaxSamples];
};

enum class VertexType
{
	Vertex_PCU,
	Vertex_PCUTBN,
	COUNT
};
enum class BlendMode
{
	ALPHA,
	ADDITIVE,
	OPAQUE,
	COUNT
};

enum class SamplerMode
{
	POINT_CLAMP,
	BILINEAR_WRAP,
	BILINEAR_CLAMP,
	COUNT
};

enum class RasterizerMode
{
	SOLID_CULL_NONE,
	SOLID_CULL_BACK,
	WIREFRAME_CULL_NONE,
	WIREFRAME_CULL_BACK,
	COUNT
};

enum class DepthMode
{
	DISABLED,
	ENABLED,
	COUNT
};
class BitmapFont;
class Texture;
class Shader;
class VertexBuffer;
class ConstantBuffer;
class Image;
class IndexBuffer;
class Renderer
{
public:
	Renderer(RenderConfig const& config);
	~Renderer();
	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();
	RenderConfig const GetRenderConfig() const;
	void ClearScreen(Rgba8 const& clearColor);
	void SetColor(Rgba8 const& color);
	void BeginCamera(Camera const& camera);
	void EndCamera(Camera const& camera);
	void DrawVertexArray(int numVertexes, const Vertex_PCU* vertexes);
	void DrawVertexArray(int numVertexes, const Vertex_PCUTBN* vertexes);
	void DrawVertexArrayWithIBO(int indexCount, VertexBuffer* vbo, IndexBuffer* ibo);
	void DrawVertexTBN(int indexCount, VertexBuffer* vbo, IndexBuffer* ibo);
	void DrawVertexBuffer(VertexBuffer* vbo, VertexType type = VertexType::Vertex_PCU, int vertexOffset = 0);
	void DrawVertexBuffer(VertexBuffer* vbo, int vertexCount, VertexType type = VertexType::Vertex_PCU, int vertexOffset = 0);

	void BindTexture(Texture const* texture, unsigned int slot = 0);
	void CreateBloomShaders(char const* blurDownFilename, char const* blurUpFilename, char const* compositeFilename);
	void SetBlendMode(BlendMode blendMode);
	void SetSamplerMode(SamplerMode sampleMode);
	void SetRasterizerMode(RasterizerMode rasterizerMode);
	void SetDepthMode(DepthMode depthMode);
	void SetModelConstants(Mat44 const& modelMatrix = Mat44(), Rgba8 const& modelColor = Rgba8::WHITE);
	void SetLightingConstants(Vec3 sunDirection = Vec3(2.f,1.f,-1.f), float sunIntensity = 0.85f, float ambientIntensity = 0.35f);
	void SetLightingConstants(LightConstants const& lightConstants);
	Texture* CreateOrGetTextureFromFile(const char* imageFilePath);
	BitmapFont* CreateOrGetBitmapFont(const char* bitmapFontFilePathWithNoExtension);
	Shader* CreateOrGetShaderFromFile(const char* filePath, VertexType vertexType = VertexType::Vertex_PCUTBN);
	VertexBuffer* CreateVertexBuffer(size_t const size);
	IndexBuffer* CreateIndexBuffer(size_t const size);
	Texture* CreateRenderTexture(IntVec2 const& dimensions, char const* name);
	void CopyCPUToGPU(const void* data, size_t size, VertexBuffer*& vbo);
	void CopyCPUToGPU(const void* data, size_t size, IndexBuffer*& ibo);
	void BindShader(Shader* shader);
	std::vector<Shader*> m_loadedShaders;
	void CopyCPUToGPU(const void* data, size_t size, ConstantBuffer* cbo);
	ConstantBuffer* CreateConstantBuffer(const size_t size);
	void BindConstantBuffer(int slot, ConstantBuffer* cbo);
	void RenderEmissive();
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

private:
	void SetBlurConstantsBlurDown(BlurConstants& blurConstants);
	void SetBlurConstantsBlurUp(BlurConstants& blurConstants);
	BitmapFont* CreateBitmapFont(const char* bitmapFontFilePathWithNoExtension);
	BitmapFont* GetBitmapFontForFileName(const char* bitmapFontFilePathWithNoExtension);
	Texture* GetTextureForFileName(char const* imageFilePath);
	Texture* CreateTextureFromFile(char const* imageFilePath);
	Image* CreateImageFromFile(char const* imageFilePath);
	Texture* CreateTextureFromData(char const* name, IntVec2 dimensions, int bytesPerTexel, uint8_t* texelData);
	Texture* CreateTextureFromImage(const Image& image);
	void CreateDebugModule();
	void CreateDeviceAndSwapChain();
	void CreateBuffers();
	Shader* GetShaderFromFileName(char const* filePath);
	Shader* CreateShader(char const* shaderName, char const* shaderSource, VertexType type = VertexType::Vertex_PCU);
	Shader* CreateShader(char const* shaderName, VertexType type = VertexType::Vertex_PCU);
	void CreateAndBindDefaultTexture();
	bool CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name, char const* source, char const* entryPoint, char const* target);


	void BindBlurCBO(BlurConstants const& blurConstants);
	void BindVertexBuffer(VertexBuffer* vbo);
	void BindVertexBufferTBN(VertexBuffer* vbo);
	void BindIndexBuffer(IndexBuffer* indexBuffer);

	void CreateBackBufferRTV();
	void CreateEmissiveBloomTextures();

	void SetRasterizerStates();
	void CreateBlendStates();
	void CreateSamplerStates();
	void CreateDepthStencilTextureAndView();
	void SetStatesIfChanged();


protected:
	//directX11 data members

	void* m_dxgiDebugModule = nullptr;
	void* m_dxgiDebug = nullptr;

	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;

	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11RenderTargetView* m_backbufferRTV = nullptr;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11Texture2D* m_depthStencilTexture = nullptr;

	ID3D11BlendState* m_blendState = nullptr;
	ID3D11BlendState* m_blendStates[(int)(BlendMode::COUNT)] = {};
	ID3D11SamplerState* m_samplerState = nullptr;
	ID3D11SamplerState* m_samplerStates[(int)(SamplerMode::COUNT)] = {};
	ID3D11RasterizerState* m_rasterizerState = nullptr;
	ID3D11RasterizerState* m_rasterizerStates[(int)(RasterizerMode::COUNT)] = {};
	ID3D11DepthStencilState* m_depthStencilStates[(int)(DepthMode::COUNT)] = {};
	ID3D11DepthStencilState* m_depthStencilState = nullptr;

	BlendMode m_desiredBlendMode = BlendMode::ALPHA;
	SamplerMode m_desiredSamplerMode = SamplerMode::POINT_CLAMP;
	RasterizerMode m_desiredRasterizerMode = RasterizerMode::SOLID_CULL_NONE;
	DepthMode m_desiredDepthMode = DepthMode::DISABLED;

	std::vector<Texture*> m_loadedTextures;
	std::vector<Texture*> m_blurDownTextures;
	std::vector<Texture*> m_blurUpTextures;
	std::vector<BitmapFont* > m_loadedFonts;

	Shader* m_currentShader = nullptr;
	Shader* m_defaultShader = nullptr;
	Shader* m_blurDownShader = nullptr;
	Shader* m_blurUpShader = nullptr;
	Shader* m_compositeShader = nullptr;
	VertexBuffer* m_immediateVBO = nullptr;
	ConstantBuffer* m_lightingCBO = nullptr;
	ConstantBuffer* m_cameraCBO = nullptr;
	ConstantBuffer* m_modelCBO = nullptr;
	RenderConfig m_config;
	Texture* m_defaultTexture = nullptr;
	Texture* m_currentTexture = nullptr;

	VertexBuffer* m_fullScreenQuadVBO_PCU = nullptr;
	ConstantBuffer* m_blurCBO = nullptr;

	Texture* m_emissiveRenderTexture = nullptr;
	Texture* m_emissiveBlurredRenderTexture = nullptr;
	IntVec2 m_savedViewportSize;
};
