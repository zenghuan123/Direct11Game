#pragma once
#include "stdafx.h"
#include "camera.h"
#include "gameObject.h"
namespace Direct11Game {
	/*
	����������յĽṹ��
	*/
	struct dirLight
	{
		XMFLOAT3  direction;
		XMFLOAT3 radiance;
		XMFLOAT3 viewPos;
		XMFLOAT3 placeholder;
	};

	/*
	��������Ϸ��ͨ����Ҫ�л�������������������ָ��һ�������ġ�
	��������Ḻ�𴴽���������Ϸ�ڵĶ���,���ǲ����ͷ���Դ
	*/
	class Scene
	{
	private:
		dirLight light;

		//light + camara��λ��
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

