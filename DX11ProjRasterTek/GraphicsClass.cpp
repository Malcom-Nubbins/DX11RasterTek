#include "GraphicsClass.h"



GraphicsClass::GraphicsClass()
{
	_direct3D = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass &)
{
}


GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	_direct3D = new d3dClass();
	if (!_direct3D)
		return false;

	result = _direct3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not Initialise Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (_direct3D)
	{
		_direct3D->Shutdown();
		delete _direct3D;
		_direct3D = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	result = Render();
	if (!result)
		return false;

	return true;
}

bool GraphicsClass::Render()
{
	_direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	_direct3D->EndScene();
	return true;
}
