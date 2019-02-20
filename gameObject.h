#pragma once
#include "stdafx.h"
#include "Texture.h"
#include "transform.h"
#include "shader.h"
#include "meshRenderer.h"
#include "resourceManager.h"


namespace Direct11Game {


	/*
	������Ϸ���������һ��ʵ��*/
	class GameObject
	{
	protected:

		/*
		��GameObject
		*/
		GameObject*parent;
		/*
		����������ļ��ϣ����Ƴ������ʱ����Ҫ�ı�parent��transform.parent��ֵ
		*/
		std::set<GameObject*>children;
		/*
		λ�ã���ת������
		*/
		Transform*transform;



		
		/*
		�������ݣ�������Ⱦ
		*/
		MeshRenderer * meshRender_;
		/*
		��ɫ��
		*/
		Shader* shader;
		/*
		��ͼ
		*/
		Texture*texture;

		

	public:
		/*
		*/
		GameObject(GameObject* parent=0);
		/*
		���ж���ͨ��ָ�봫��
		*/
		GameObject(const GameObject& ob) = delete;
		GameObject(const GameObject&&ob) = delete;

		virtual ~GameObject();

		Transform  * GetTransform();
		
		void SetMeshRenderer(MeshRenderer*mesh);
		void SetShader(Shader*shader);
		void SetTexture(Texture*texture);
		
		/*
		��������壬�����child�ĸ������RemoveChild����
		*/
		void AddChildren(GameObject*child);
		/*
		�Ƴ������壬���ǲ����ͷ���Դ
		*/
		void RemoveChild(GameObject*child);


		virtual void Update(float interval);
		virtual void Render(ID3D11DeviceContext*context);

		

		/*
		void *operator new(size_t); // allocate an object   ����ԭʼ�ڴ�
		void *operator new[](size_t); // allocate an array  ����ԭʼ�ڴ棬�ڴ���׼���Ŷ������a
		void operator delete(void*); // free an object   //�ͷ�ԭʼ�ڴ�
		void operator delete[](void*); // free an array  //�ͷ�ԭʼ�ڴ�
		*/


	};
}

