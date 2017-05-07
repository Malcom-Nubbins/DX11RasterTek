#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class LightClass
{
private:
	XMFLOAT4 _diffuse;
	XMFLOAT3 _direction;
public:
	LightClass();
	LightClass(const LightClass& other);
	~LightClass();

	void SetDiffuseColour(XMFLOAT4 diffuseColour);
	void SetLightDirection(XMFLOAT3 lightDir);

	XMFLOAT4 GetDiffuseColour()const { return _diffuse; }
	XMFLOAT3 GetLightDirection()const { return _direction; }
};

