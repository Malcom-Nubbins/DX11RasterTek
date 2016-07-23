#pragma once
#include <Windows.h>

#include "d3dClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColorShaderClass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Init(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	d3dClass* _direct3D;
	CameraClass* _camera;
	ModelClass* _model;
	ColorShaderClass* _colorShader;
};

