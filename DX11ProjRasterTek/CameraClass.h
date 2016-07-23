#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass& other);
	~CameraClass();

	void SetPosition(XMFLOAT3 pos);
	void SetRotation(XMFLOAT3 rotation);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);

private:
	XMFLOAT3 _pos;
	XMFLOAT3 _rotation;
	XMMATRIX _viewMatrix;
};

