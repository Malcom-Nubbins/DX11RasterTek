#include "LightClass.h"

LightClass::LightClass()
{
	_diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	_direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

LightClass::LightClass(const LightClass & other)
{
}


LightClass::~LightClass()
{
}

void LightClass::SetDiffuseColour(XMFLOAT4 diffuseColour)
{
	_diffuse = diffuseColour;
}

void LightClass::SetLightDirection(XMFLOAT3 lightDir)
{
	_direction = lightDir;
}
