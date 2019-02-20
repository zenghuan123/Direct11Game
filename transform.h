#pragma once
#include "stdafx.h"
namespace Direct11Game {
	class Transform
	{
		friend class GameObject;
	private:
		Transform*parent;
		XMFLOAT3 scale;
		XMFLOAT3 rotation;
		XMFLOAT3 position;
		Transform(Transform*parent = 0);
		~Transform();
	public:

		
		void SetRotation(FXMVECTOR rotation);
		void SetRotation(float x, float y, float z);
		void SetPosition(FXMVECTOR position);
		void SetPosition(float x, float y, float z);
		void SetScale(FXMVECTOR scale);
		void SetScale(float x, float y, float z);
		
		XMFLOAT3 GetRotation();
		XMFLOAT3 GetPosition();
		XMFLOAT3 GetScale();


		XMMATRIX GetWorldMatrix();
		
		
		







	};
}

