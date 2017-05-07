#include "ShaderClass.h"


ShaderClass::ShaderClass()
{
	_vertexShader = 0;
	_pixelShader = 0;
	_layout = 0;
	_matrixBuffer = 0;
	_samplerState = 0;
	_lightBuffer = 0;
}

ShaderClass::ShaderClass(const ShaderClass & other)
{
}

ShaderClass::~ShaderClass()
{
}

bool ShaderClass::Init(ID3D11Device * device, HWND hwnd)
{
	bool result;

	result = InitShader(device, hwnd, L"Shaders/VertexShader.hlsl", L"Shaders/PixelShader.hlsl");
	if (!result)
		return false;


	return true;
}

void ShaderClass::Shutdown()
{
	ShutdownShader();
	return;
}

bool ShaderClass::Render(ID3D11DeviceContext * deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX proj, 
						ID3D11ShaderResourceView * shaderResourceView, XMFLOAT4 diffuse, XMFLOAT3 direction)
{
	bool result;
	
	result = SetShaderParams(deviceContext, world, view, proj, shaderResourceView, diffuse, direction);
	if (!result)
		return false;

	RenderShader(deviceContext, indexCount);
	
	return true;
}

bool ShaderClass::InitShader(ID3D11Device * device, HWND hwnd, WCHAR * vsFilename, WCHAR * psFilename)
{
	HRESULT hr;
	ID3D10Blob* errmsg;
	ID3D10Blob* vertexShaderBuff;
	ID3D10Blob* pixelShaderBuff;
	D3D11_INPUT_ELEMENT_DESC layout[3];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	errmsg = 0;
	vertexShaderBuff = 0;
	pixelShaderBuff = 0;

	hr = D3DCompileFromFile(vsFilename, nullptr, nullptr, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuff, &errmsg);
	if (FAILED(hr))
	{
		if (errmsg)
			OutputShaderErrMsg(errmsg, hwnd, vsFilename);
		else
			MessageBox(hwnd, vsFilename, L"Missing shader file", MB_OK);

		return false;
	}

	hr = D3DCompileFromFile(psFilename, nullptr, nullptr, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuff, &errmsg);
	if (FAILED(hr))
	{
		if (errmsg)
			OutputShaderErrMsg(errmsg, hwnd, psFilename);
		else
			MessageBox(hwnd, psFilename, L"Missing shader file", MB_OK);

		return false;
	}

	hr = device->CreateVertexShader(vertexShaderBuff->GetBufferPointer(), vertexShaderBuff->GetBufferSize(), nullptr, &_vertexShader);
	if (FAILED(hr))
		return false;

	hr = device->CreatePixelShader(pixelShaderBuff->GetBufferPointer(), pixelShaderBuff->GetBufferSize(), nullptr, &_pixelShader);
	if (FAILED(hr))
		return false;

	layout[0].SemanticName = "POSITION";
	layout[0].SemanticIndex = 0;
	layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[0].InputSlot = 0;
	layout[0].AlignedByteOffset = 0;
	layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[0].InstanceDataStepRate = 0;

	layout[1].SemanticName = "TEXCOORD";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	layout[1].InputSlot = 0;
	layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;

	layout[2].SemanticName = "NORMAL";
	layout[2].SemanticIndex = 0;
	layout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[2].InputSlot = 0;
	layout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[2].InstanceDataStepRate = 0;

	numElements = sizeof(layout) / sizeof(layout[0]);

	hr = device->CreateInputLayout(layout, numElements, vertexShaderBuff->GetBufferPointer(), vertexShaderBuff->GetBufferSize(), &_layout);
	if (FAILED(hr))
		return false;

	vertexShaderBuff->Release();
	vertexShaderBuff = 0;

	pixelShaderBuff->Release();
	pixelShaderBuff = 0;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	hr = device->CreateBuffer(&matrixBufferDesc, nullptr, &_matrixBuffer);
	if (FAILED(hr))
		return false;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = device->CreateSamplerState(&samplerDesc, &_samplerState);
	if (FAILED(hr))
		return false;

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	hr = device->CreateBuffer(&lightBufferDesc, nullptr, &_lightBuffer);
	if (FAILED(hr))
		return false;
	
	return true;
}

void ShaderClass::ShutdownShader()
{
	if (_lightBuffer)
	{
		_lightBuffer->Release();
		_lightBuffer = 0;
	}
	
	// Release the sampler state.
	if (_samplerState)
	{
		_samplerState->Release();
		_samplerState = 0;
	}

	// Release the matrix constant buffer.
	if (_matrixBuffer)
	{
		_matrixBuffer->Release();
		_matrixBuffer = 0;
	}

	// Release the layout.
	if (_layout)
	{
		_layout->Release();
		_layout = 0;
	}

	// Release the pixel shader.
	if (_pixelShader)
	{
		_pixelShader->Release();
		_pixelShader = 0;
	}

	// Release the vertex shader.
	if (_vertexShader)
	{
		_vertexShader->Release();
		_vertexShader = 0;
	}

	return;
}

void ShaderClass::OutputShaderErrMsg(ID3D10Blob * errBlob, HWND hwnd, WCHAR * shaderFilename)
{
	char* compileErrors;
	unsigned long long buffersize, i;
	ofstream fout;

	compileErrors = (char*)(errBlob->GetBufferPointer());
	buffersize = errBlob->GetBufferSize();

	fout.open("Shaders/shader_error.txt");

	for (i = 0; i < buffersize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	errBlob->Release();
	errBlob = 0;

	MessageBox(hwnd, L"Error compiling shader. Check shader_error.txt for info.", shaderFilename, MB_OK);

	return;
}

bool ShaderClass::SetShaderParams(ID3D11DeviceContext * deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX proj, 
	ID3D11ShaderResourceView * shaderResourceView, XMFLOAT4 diffuse, XMFLOAT3 direction)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNum;
	LightBufferType* dataPtr2;

	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

	hr = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
		return false;

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->proj = proj;

	deviceContext->Unmap(_matrixBuffer, 0);

	bufferNum = 0;

	deviceContext->VSSetConstantBuffers(bufferNum, 1, &_matrixBuffer);

	deviceContext->PSSetShaderResources(0, 1, &shaderResourceView);

	hr = deviceContext->Map(_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
		return false;

	dataPtr2 = (LightBufferType*)mappedResource.pData;
	dataPtr2->diffuseColour = diffuse;
	dataPtr2->lightDir = direction;
	dataPtr2->padding = 0.0f;

	deviceContext->Unmap(_lightBuffer, 0);

	bufferNum = 0;

	deviceContext->PSSetConstantBuffers(bufferNum, 1, &_lightBuffer);

	return true;
}

void ShaderClass::RenderShader(ID3D11DeviceContext * context, int indexCount)
{
	context->IASetInputLayout(_layout);

	context->VSSetShader(_vertexShader, nullptr, 0);
	context->PSSetShader(_pixelShader, nullptr, 0);

	context->PSSetSamplers(0, 1, &_samplerState);

	context->DrawIndexed(indexCount, 0, 0);

	return;
}
