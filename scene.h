#pragma once
#include "stdafx.h"
#include "camera.h"
#include "gameObject.h"
namespace Direct11Game {
	/*
	用作计算光照的结构体
	*/
	struct dirLight
	{
		XMFLOAT3  direction;
		XMFLOAT3 radiance;
		XMFLOAT3 viewPos;
		XMFLOAT3 placeholder;
	};

	/*
	场景，游戏中通常需要切换场景，这个类就是用来指代一个场景的。
	这个场景会负责创建和销毁游戏内的对象,但是不会释放资源
	*/
	class Scene
	{
	private:
		dirLight light;

		//light + camara的位置
		ID3D11Buffer*buffer;

		std::set<GameObject*> objectSet_;
		ID3D11Buffer*viewBuffer_;
		ID3D11Buffer*projectionBuffer_;
		Camera *mainCamera_;
	protected:
		virtual void CreateScene(ID3D11Device* device, ID3D11DeviceContext* context) = 0;
	public:		
		Camera* GetMainCamera();
		Scene();
		virtual ~Scene();

		void AddGameObject(GameObject*ob);
		void Init(ID3D11Device* device, ID3D11DeviceContext* context);
		void Update(float t);
		void Render(ID3D11DeviceContext*context);
	};
}

