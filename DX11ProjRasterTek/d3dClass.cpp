#include "d3dClass.h"

d3dClass::d3dClass()
{
	_swapchain = 0;
	_device = 0;
	_devContext = 0;
	_rtv = 0;
	_depthStencilBuffer = 0;
	_depthStencilState = 0;
	_depthStencilView = 0;
	_rasterState = 0;
}

d3dClass::d3dClass(const d3dClass &)
{
}


d3dClass::~d3dClass()
{
}

bool d3dClass::Init(int width, int height, bool vsync, HWND hwnd, bool fullscr, float screenDepth, float screenNear)
{
	HRESULT hr;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPrt;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fov, aspect;

	_vsync = vsync;

	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(hr))
		return false;

	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr))
		return false;

	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr))
		return false;

	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(hr))
		return false;

	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
		return false;

	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(hr))
		return false;

	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)width)
		{
			if (displayModeList[i].Height == (unsigned int)height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	hr = adapter->GetDesc(&adapterDesc);
	if (FAILED(hr))
		return false;

	_videoCardMem = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	error = wcstombs_s(&stringLength, _videoCardDesc, 128, adapterDesc.Description, 128);
	if (error != 0)
		false;

	delete[] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (_vsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (fullscr)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &_swapchain, &_device, nullptr, &_devContext);
	if (FAILED(hr))
		return false;

	hr = _swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPrt);
	if (FAILED(hr))
		return false;

	hr = _device->CreateRenderTargetView(backBufferPrt, nullptr, &_rtv);
	if (FAILED(hr))
		return false;

	backBufferPrt->Release();
	backBufferPrt = 0;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = _device->CreateTexture2D(&depthBufferDesc, nullptr, &_depthStencilBuffer);
	if (FAILED(hr))
		return false;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
	if (FAILED(hr))
		return false;

	_devContext->OMSetDepthStencilState(_depthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = _device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
	if (FAILED(hr))
		return false;

	_devContext->OMSetRenderTargets(1, &_rtv, _depthStencilView);

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	hr = _device->CreateRasterizerState(&rasterDesc, &_rasterState);
	if (FAILED(hr))
		return false;

	_devContext->RSSetState(_rasterState);

	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	_devContext->RSSetViewports(1, &viewport);

	fov = XM_PI / 4.0;
	aspect = (float)width / (float)height;

	_proj = XMMatrixPerspectiveFovLH(fov, aspect, screenNear, screenDepth);

	_world = XMMatrixIdentity();

	_ortho = XMMatrixOrthographicLH((float)width, (float)height, screenNear, screenDepth);

	return true;
}

void d3dClass::Shutdown()
{
	if (_swapchain)
		_swapchain->SetFullscreenState(false, nullptr);

	if (_rasterState)
	{
		_rasterState->Release();
		_rasterState = 0;
	}

	if (_depthStencilView)
	{
		_depthStencilView->Release();
		_depthStencilView = 0;
	}

	if (_depthStencilState)
	{
		_depthStencilState->Release();
		_depthStencilState = 0;
	}

	if (_depthStencilBuffer)
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = 0;
	}

	if (_rtv)
	{
		_rtv->Release();
		_rtv = 0;
	}

	if (_devContext)
	{
		_devContext->Release();
		_devContext = 0;
	}

	if (_device)
	{
		_device->Release();
		_device = 0;
	}

	if (_swapchain)
	{
		_swapchain->Release();
		_swapchain = 0;
	}

	return;
}

void d3dClass::BeginScene(float red, float green, float blue, float alpha)
{
	float colour[4];

	colour[0] = red;
	colour[1] = green;
	colour[2] = blue;
	colour[3] = alpha;

	_devContext->ClearRenderTargetView(_rtv, colour);
	_devContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void d3dClass::EndScene()
{
	if (_vsync)
	{
		_swapchain->Present(1, 0);
	}
	else
	{
		_swapchain->Present(0, 0);
	}

	return;
}

ID3D11Device * d3dClass::GetDevice()
{
	return _device;
}

ID3D11DeviceContext * d3dClass::GetDeviceContext()
{
	return _devContext;
}

void d3dClass::GetProjMat(XMMATRIX& projMat)
{
	projMat = _proj;
	return;
}

void d3dClass::GetWorldMat(XMMATRIX& worldMat)
{
	worldMat = _world;
	return;
}

void d3dClass::GetOrthoMat(XMMATRIX& orthoMat)
{
	orthoMat = _ortho;
	return;
}

void d3dClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, _videoCardDesc);
	memory = _videoCardMem;
	return;
}
