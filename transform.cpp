#include "transform.h"
namespace Direct11Game {

	Transform::Transform(Transform*p) :rotation(XMFLOAT3(0, 0, 0)), scale({ 1,1,1 }), position({ 0, 0, 0 }), parent(p)
	{
	}


	Transform::~Transform()
	{
	}

	void Transform::SetRotation(FXMVECTOR rotation)
	{
		XMStoreFloat3(&this->rotation, rotation);
	}

	void Transform::SetRotation(float x, float y, float z)
	{
		this->rotation.x = x;
		this->rotation.y = y;
		this->rotation.z = z;
	}

	void Transform::SetPosition(FXMVECTOR position)
	{
		XMStoreFloat3(&this->position, position);
	}
	void Transform::SetPosition(float x, float y, float z)
	{
		this->position.x = x;
		this->position.y = y;
		this->position.z = z;
	}
	void Transform::SetScale(FXMVECTOR scale)
	{
		XMStoreFloat3(&this->scale, scale);
	}

	void Transform::SetScale(float x, float y, float z)
	{
		this->scale.x = x;
		this->scale.y = y;
		this->scale.z = z;
	}



	XMFLOAT3 Transform::GetRotation()
	{
		return this->rotation;
	}

	XMFLOAT3 Transform::GetPosition()
	{
		return this->position;
	}

	XMFLOAT3 Transform::GetScale()
	{
		return this->scale;
	}



	XMMATRIX Transform::GetWorldMatrix()
	{
		if (parent)
		{
			return  XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z)*XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z)*
				XMMatrixTranslation(this->position.x, this->position.y, this->position.z)*this->parent->GetWorldMatrix();
		}
		else
		{
			return  XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z)*XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z)*
				XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
		}
	}



}
