#pragma once
#include "stdafx.h"
#include "Texture.h"
#include "meshRenderer.h"
#include "shader.h"
namespace Direct11Game {
	class Camera
	{
	private:
		

		XMVECTOR up_;
		XMFLOAT3 position_;
		XMFLOAT3 target_;

		Texture *top_;
		Texture *left_;
		Texture*right_;
		Texture*front_;
		Texture*back_;
		MeshRenderer*meshRender_;
		Shader*shader_;

		float destance;
		
		


	public:
		Camera();
		virtual ~Camera();

		void SetShader(Shader*shader);
		void SetMeshRender(MeshRenderer*mesh);

		void SetSkyBox(Texture*top, Texture *left, Texture*right, Texture*front, Texture*back);

		void SetPosition(float x,float y,float z);
		void SetTarget(float x, float y, float z);

		void SetPosition(FXMVECTOR ve);
		void SetTarget(FXMVECTOR ve);

		inline XMFLOAT3 GetPosition()
		{
			return this->position_;
		}
		void Update(float t);
		void Render(ID3D11DeviceContext * context);

		XMMATRIX GetViewMatrix();


	};
}

