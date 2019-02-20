#pragma once
#include "scene.h"
namespace Direct11Game {
	class BeginScene :
		public Scene
	{
	public:
		BeginScene();
		virtual void CreateScene(ID3D11Device* device, ID3D11DeviceContext* context)override;

		virtual ~BeginScene();
	};
}

