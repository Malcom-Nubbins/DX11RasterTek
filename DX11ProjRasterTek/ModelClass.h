#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "TextureClass.h"
using namespace DirectX;

class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 pos;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

private:
	ID3D11Buffer* _vertexBuffer, *_indexBuffer;
	int _vertexCount, _indexCount;

	TextureClass* _texture;

	bool InitBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* context, char* filename);
	void ReleaseTexture();

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Init(ID3D11Device* device, ID3D11DeviceContext* context, char* filename);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);
	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture()const { return _texture->GetTexture(); }
};

