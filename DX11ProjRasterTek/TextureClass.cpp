#include "TextureClass.h"


TextureClass::TextureClass()
{
	_targaData = 0;
	_texture = 0;
	_textureView = 0;
}

TextureClass::TextureClass(const TextureClass & other)
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Init(ID3D11Device * device, ID3D11DeviceContext * deviceContext, char * filename)
{
	bool result;
	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hr;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	result = LoadTarga(filename, height, width);
	if (!result)
		return false;

	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	hr = device->CreateTexture2D(&textureDesc, nullptr, &_texture);
	if (FAILED(hr))
		return false;

	rowPitch = (width * 4) * sizeof(unsigned char);

	deviceContext->UpdateSubresource(_texture, 0, nullptr, _targaData, rowPitch, 0);

	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(_texture, &srvDesc, &_textureView);
	if (FAILED(hr))
		return false;

	deviceContext->GenerateMips(_textureView);

	delete[] _targaData;
	_targaData = 0;

	return true;
}

void TextureClass::Shutdown()
{
	if (_textureView)
	{
		_textureView->Release();
		_textureView = 0;
	}

	if (_texture)
	{
		_texture->Release();
		_texture = 0;
	}

	if (_targaData)
	{
		delete[] _targaData;
		_targaData = 0;
	}

	return;
}

bool TextureClass::LoadTarga(char * filename, int & height, int & width)
{
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
		return false;

	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
		return false;

	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	if (bpp != 32)
		return false;

	imageSize = width * height * 4;

	targaImage = new unsigned char[imageSize];
	if (!targaImage)
		return false;

	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
		return false;

	error = fclose(filePtr);
	if (error != 0)
		return false;

	_targaData = new unsigned char[imageSize];
	if (!_targaData)
		return false;

	index = 0;

	k = (width * height * 4) - (width * 4);

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			_targaData[index + 0] = targaImage[k + 2];
			_targaData[index + 1] = targaImage[k + 1];
			_targaData[index + 2] = targaImage[k + 0];
			_targaData[index + 3] = targaImage[k + 3];

			k += 4;
			index += 4;
		}

		k -= (width * 8);
	}

	delete[] targaImage;
	targaImage = 0;

	return true;
}