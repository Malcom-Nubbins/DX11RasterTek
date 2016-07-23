#include "GraphicsClass.h"



GraphicsClass::GraphicsClass()
{
	_direct3D = 0;
	_camera = 0;
	_model = 0;
	_colorShader = 0;
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

	_camera = new CameraClass();
	if (!_camera)
		return false;

	_camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -5.0f));

	_model = new ModelClass();
	if (!_model)
		return false;

	result = _model->Init(_direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not init model", L"Error", MB_OK);
		return false;
	}

	_colorShader = new ColorShaderClass();
	if (!_colorShader)
		return false;

	result = _colorShader->Init(_direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not init shader object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (_colorShader)
	{
		_colorShader->Shutdown();
		delete _colorShader;
		_colorShader = 0;
	}

	// Release the model object.
	if (_model)
	{
		_model->Shutdown();
		delete _model;
		_model = 0;
	}

	// Release the camera object.
	if (_camera)
	{
		delete _camera;
		_camera = 0;
	}

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
	XMMATRIX world, view, proj;
	bool result;

	_direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	_camera->Render();

	_direct3D->GetWorldMat(world);
	_camera->GetViewMatrix(view);
	_direct3D->GetProjMat(proj);

	_model->Render(_direct3D->GetDeviceContext());

	result = _colorShader->Render(_direct3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj);
	if (!result)
		return false;

	_direct3D->EndScene();
	return true;
}
