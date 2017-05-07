#pragma once
#include <d3d11.h>
#include <stdio.h>

class TextureClass
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	unsigned char* _targaData;
	ID3D11Texture2D* _texture;
	ID3D11ShaderResourceView* _textureView;

	bool LoadTarga(char*, int&, int&);
public:
	TextureClass();
	TextureClass(const TextureClass& other);
	~TextureClass();

	bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture()const { return _textureView; }
};

