#include "ColorShaderClass.h"


ColorShaderClass::ColorShaderClass()
{
	_vertexShader = 0;
	_pixelShader = 0;
	_layout = 0;
	_matrixBuffer = 0;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass &)
{
}


ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::Init(ID3D11Device * device, HWND hwnd)
{
	bool result;

	result = InitShader(device, hwnd, L"colorVS.hlsl", L"colorPS.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

void ColorShaderClass::Shutdown()
{
	ShutdownShader();
	return;
}

bool ColorShaderClass::Render(ID3D11DeviceContext * deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX proj)
{
	bool result;

	result = SetShaderParams(deviceContext, world, view, proj);
	if (!result)
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}

bool ColorShaderClass::InitShader(ID3D11Device* device, HWND hwnd, WCHAR * vsFilename, WCHAR * psFilename)
{
	HRESULT hr;
	ID3D10Blob* errMsg;
	ID3D10Blob* vsBuff;
	ID3D10Blob* psBuff;
	D3D11_INPUT_ELEMENT_DESC layout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC bufferDesc;

	errMsg = 0;
	vsBuff = 0;
	psBuff = 0;

	hr = D3DCompileFromFile(vsFilename, nullptr, nullptr, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vsBuff, &errMsg);
	if (FAILED(hr))
	{
		if (errMsg)
		{
			OutputShaderErr(errMsg, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing vertex shader file", MB_OK);
		}
		return false;
	}

	hr = D3DCompileFromFile(psFilename, nullptr, nullptr, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &psBuff, &errMsg);
	if (FAILED(hr))
	{
		if (errMsg)
		{
			OutputShaderErr(errMsg, hwnd, psFilename);
		}
		else
		{
			MessageBox(hwnd, psFilename, L"Missing pixel shader file", MB_OK);
		}
		return false;
	}

	hr = device->CreateVertexShader(vsBuff->GetBufferPointer(), vsBuff->GetBufferSize(), nullptr, &_vertexShader);
	if (FAILED(hr))
		return false;

	hr = device->CreatePixelShader(psBuff->GetBufferPointer(), psBuff->GetBufferSize(), nullptr, &_pixelShader);
	if (FAILED(hr))
		return false;

	layout[0].SemanticName = "POSITION";
	layout[0].SemanticIndex = 0;
	layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[0].InputSlot = 0;
	layout[0].AlignedByteOffset = 0;
	layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[0].InstanceDataStepRate = 0;

	layout[1].SemanticName = "COLOR";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	layout[1].InputSlot = 0;
	layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;

	numElements = sizeof(layout) / sizeof(layout[0]);

	hr = device->CreateInputLayout(layout, numElements, vsBuff->GetBufferPointer(), vsBuff->GetBufferSize(), &_layout);
	if (FAILED(hr))
		return false;

	vsBuff->Release();
	vsBuff = 0;

	psBuff->Release();
	psBuff = 0;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(MatrixBufferType);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	hr = device->CreateBuffer(&bufferDesc, nullptr, &_matrixBuffer);
	if (FAILED(hr))
		return false;

	return true;
}

void ColorShaderClass::ShutdownShader()
{
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

void ColorShaderClass::OutputShaderErr(ID3D10Blob * errmsg, HWND hwnd, WCHAR * shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errmsg->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errmsg->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errmsg->Release();
	errmsg = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool ColorShaderClass::SetShaderParams(ID3D11DeviceContext * deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX proj)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int buffNum;

	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

	hr = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
		return false;

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->worldMat = world;
	dataPtr->viewMat = view;
	dataPtr->projMat = proj;

	deviceContext->Unmap(_matrixBuffer, 0);

	buffNum = 0;

	deviceContext->VSSetConstantBuffers(buffNum, 1, &_matrixBuffer);

	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(_layout);

	deviceContext->VSSetShader(_vertexShader, nullptr, 0);
	deviceContext->PSSetShader(_pixelShader, nullptr, 0);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}
