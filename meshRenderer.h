#pragma once
#include "stdafx.h"
namespace Direct11Game {

	class MeshRenderer
	{
		friend class ResourceManager;
	private:
		/*
		Ĭ��1 λ��,��ͼ����,��������
		�������������ResourceManager�����ڴ�
		*/
		std::vector<ID3D11Buffer*>vertexBuffer_;
		std::vector<UINT>strides_;
		std::vector<UINT>offsets_;
		UINT indexNum;
		ID3D11Buffer* indexBuffer_;

		MeshRenderer();

		/*
		��ResourceManager�����������ڴ�
		*/
		void Release();
	

	public:
		void Draw(ID3D11DeviceContext *context);
		

		virtual ~MeshRenderer();
		
		

		

	};
}

