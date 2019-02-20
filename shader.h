#pragma once
#include "stdafx.h"
namespace Direct11Game {
	/*
	��ɫ�������涥����ɫ��������ṹ��������ɫ������
	*/
	class Shader
	{
		friend class ResourceManager;
	private:
		/*
		��Щ������ResourceMananger�������ͷ�
		*/
		ID3D11VertexShader* vertexShader_;
		ID3D11PixelShader* pixelShader;
		ID3D11InputLayout *inputLayout_;


		Shader();
		
		
	public:

		void SetUp(ID3D11DeviceContext * immediateContext);

		
		virtual ~Shader();


	};
}

