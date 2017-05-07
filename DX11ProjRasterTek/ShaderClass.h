#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;
using namespace std;

class ShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;
	};

	struct LightBufferType
	{
		XMFLOAT4 diffuseColour;
		XMFLOAT3 lightDir;
		float padding;
	};

	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11Buffer* _matrixBuffer;

	ID3D11SamplerState* _samplerState;
	ID3D11Buffer* _lightBuffer;

	bool InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrMsg(ID3D10Blob* errBlob, HWND hwnd, WCHAR*);

	bool SetShaderParams(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX proj, ID3D11ShaderResourceView* shaderResourceView, XMFLOAT4 diffuse, XMFLOAT3 direction);
	void RenderShader(ID3D11DeviceContext* context, int);

public:
	ShaderClass();
	ShaderClass(const ShaderClass&);
	~ShaderClass();

	bool Init(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int, XMMATRIX world, XMMATRIX view, XMMATRIX proj, ID3D11ShaderResourceView* shaderResourceView, XMFLOAT4 diffuse, XMFLOAT3 direction);
};

