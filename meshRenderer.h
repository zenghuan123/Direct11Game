#pragma once
#include "stdafx.h"
namespace Direct11Game {

	class MeshRenderer
	{
		friend class ResourceManager;
	private:
		/*
		默认1 位置,贴图坐标,法线向量
		下面五个数据由ResourceManager回收内存
		*/
		std::vector<ID3D11Buffer*>vertexBuffer_;
		std::vector<UINT>strides_;
		std::vector<UINT>offsets_;
		UINT indexNum;
		ID3D11Buffer* indexBuffer_;

		MeshRenderer();

		/*
		有ResourceManager调用来回收内存
		*/
		void Release();
	

	public:
		void Draw(ID3D11DeviceContext *context);
		

		virtual ~MeshRenderer();
		
		

		

	};
}

