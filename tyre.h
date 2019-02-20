#pragma once
#include "stdafx.h"
#include "gameObject.h"
namespace Direct11Game {
	class Tyre :
		public GameObject
	{
	private:		
		MeshRenderer*meshRender2_;
		Texture *texture2_;

		float turn_;
		float speed_;

	public:
		void SetTurn(float turn);
		void SetSpeed(float speed);
		Tyre(GameObject* parent=0);
		virtual ~Tyre();
		void Update(float  t)override;
		virtual void Render(ID3D11DeviceContext*immediateContext) override;
	};
}

