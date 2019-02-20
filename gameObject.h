#pragma once
#include "stdafx.h"
#include "Texture.h"
#include "transform.h"
#include "shader.h"
#include "meshRenderer.h"
#include "resourceManager.h"


namespace Direct11Game {


	/*
	代表游戏当中物体的一个实例*/
	class GameObject
	{
	protected:

		/*
		父GameObject
		*/
		GameObject*parent;
		/*
		保存子物体的集合，在移除和添加时，需要改变parent和transform.parent的值
		*/
		std::set<GameObject*>children;
		/*
		位置，旋转，控制
		*/
		Transform*transform;



		
		/*
		顶点数据，负责渲染
		*/
		MeshRenderer * meshRender_;
		/*
		着色器
		*/
		Shader* shader;
		/*
		贴图
		*/
		Texture*texture;

		

	public:
		/*
		*/
		GameObject(GameObject* parent=0);
		/*
		所有对象通过指针传递
		*/
		GameObject(const GameObject& ob) = delete;
		GameObject(const GameObject&&ob) = delete;

		virtual ~GameObject();

		Transform  * GetTransform();
		
		void SetMeshRenderer(MeshRenderer*mesh);
		void SetShader(Shader*shader);
		void SetTexture(Texture*texture);
		
		/*
		添加子物体，会调用child的父物体的RemoveChild方法
		*/
		void AddChildren(GameObject*child);
		/*
		移除子物体，但是不会释放资源
		*/
		void RemoveChild(GameObject*child);


		virtual void Update(float interval);
		virtual void Render(ID3D11DeviceContext*context);

		

		/*
		void *operator new(size_t); // allocate an object   申请原始内存
		void *operator new[](size_t); // allocate an array  申请原始内存，内存中准备放多个对象a
		void operator delete(void*); // free an object   //释放原始内存
		void operator delete[](void*); // free an array  //释放原始内存
		*/


	};
}

