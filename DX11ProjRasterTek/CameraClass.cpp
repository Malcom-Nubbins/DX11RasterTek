#include "CameraClass.h"

CameraClass::CameraClass()
{
	_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CameraClass::CameraClass(const CameraClass & other)
{
}


CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(XMFLOAT3 pos)
{
	_pos = pos;
}

void CameraClass::SetRotation(XMFLOAT3 rotation)
{
	_rotation = rotation;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return _pos;
}

XMFLOAT3 CameraClass::GetRotation()
{
	return _rotation;
}

void CameraClass::Render()
{
	XMFLOAT3 eyeF, upF, atF;
	XMVECTOR eyeV, upV, atV;
	float yaw, pitch, roll;
	XMMATRIX rotationMat;

	// Setup Up
	upF.x = 0.0f;
	upF.y = 1.0f;
	upF.z = 0.0f;
	upV = XMLoadFloat3(&upF);

	// Setup eye pos
	eyeF = _pos;
	eyeV = XMLoadFloat3(&eyeF);

	// Setup at
	atF.x = 0.0f;
	atF.y = 0.0f;
	atF.z = 1.0f;
	atV = XMLoadFloat3(&atF);

	pitch = XMConvertToRadians(_rotation.x);
	yaw = XMConvertToRadians(_rotation.y);
	roll = XMConvertToRadians(_rotation.z);

	rotationMat = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	atV = XMVector3TransformCoord(atV, rotationMat);
	upV = XMVector3TransformCoord(upV, rotationMat);

	atV = XMVectorAdd(eyeV, atV);

	_viewMatrix = XMMatrixLookAtLH(eyeV, atV, upV);

	return;
}

void CameraClass::GetViewMatrix(XMMATRIX & viewMatix)
{
	viewMatix = _viewMatrix;
	return;
}
