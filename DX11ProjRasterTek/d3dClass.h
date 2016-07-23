#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;

class d3dClass
{
public:
	d3dClass();
	d3dClass(const d3dClass&);
	~d3dClass();

	bool Init(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjMat(XMMATRIX&);
	void GetWorldMat(XMMATRIX&);
	void GetOrthoMat(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

private:
	bool _vsync;
	int _videoCardMem;
	char _videoCardDesc[128];
	IDXGISwapChain*			_swapchain;
	ID3D11Device*			_device;
	ID3D11DeviceContext*	_devContext;
	ID3D11RenderTargetView*	_rtv;
	ID3D11Texture2D*		_depthStencilBuffer;
	ID3D11DepthStencilState* _depthStencilState;
	ID3D11DepthStencilView*	_depthStencilView;
	ID3D11RasterizerState*	_rasterState;
	XMMATRIX _proj;
	XMMATRIX _world;
	XMMATRIX _ortho;
};

