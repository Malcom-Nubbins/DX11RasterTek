#include "ColorShaderClass.h"


ColorShaderClass::ColorShaderClass()
{
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass &)
{
}


ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::Init(ID3D11Device *, HWND)
{
	return false;
}

void ColorShaderClass::Shutdown()
{
}

bool ColorShaderClass::Render(ID3D11DeviceContext *, int, XMMATRIX, XMMATRIX, XMMATRIX)
{
	return false;
}

bool ColorShaderClass::InitShader(ID3D11Device *, HWND, WCHAR *, WCHAR *)
{
	return false;
}

void ColorShaderClass::ShutdownShader()
{
}

void ColorShaderClass::OutputShaderErr(ID3D10Blob *, HWND, WCHAR *)
{
}

bool ColorShaderClass::SetShaderParams(ID3D11DeviceContext *, XMMATRIX, XMMATRIX, XMMATRIX)
{
	return false;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext *, int)
{
}
