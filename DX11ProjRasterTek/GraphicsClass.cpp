#include "GraphicsClass.h"



GraphicsClass::GraphicsClass()
{
	_direct3D = 0;
	_camera = 0;
	_model = 0;
	_shader = 0;
	_light = 0;
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

	result = _model->Init(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), "Textures/grass.tga");
	if (!result)
	{
		MessageBox(hwnd, L"Could not init model", L"Error", MB_OK);
		return false;
	}

	_shader = new ShaderClass();
	if (!_shader)
		return false;

	result = _shader->Init(_direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not init shader object", L"Error", MB_OK);
		return false;
	}

	_light = new LightClass();
	if (!_light)
		return false;

	_light->SetDiffuseColour(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	_light->SetLightDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));

	return true;
}

void GraphicsClass::Shutdown()
{
	if (_light)
	{
		delete _light;
		_light = 0;
	}

	if (_shader)
	{
		_shader->Shutdown();
		delete _shader;
		_shader = 0;
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

	static float rotation = 0.0f;

	rotation += (float)XM_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	result = Render(rotation);
	if (!result)
		return false;

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	XMMATRIX world, view, proj;
	bool result;

	_direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	_camera->Render();

	_direct3D->GetWorldMat(world);
	_camera->GetViewMatrix(view);
	_direct3D->GetProjMat(proj);

	//XMMatrixRotationY(rotation);

	_model->Render(_direct3D->GetDeviceContext());

	result = _shader->Render(_direct3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTexture(), _light->GetDiffuseColour(), _light->GetLightDirection());
	if (!result)
		return false;

	_direct3D->EndScene();
	return true;
}
