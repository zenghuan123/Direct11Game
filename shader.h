#pragma once
#include "stdafx.h"
namespace Direct11Game {
	/*
	着色器，保存顶点着色器，输入结构，像素着色器数据
	*/
	class Shader
	{
		friend class ResourceManager;
	private:
		/*
		这些数据由ResourceMananger创建和释放
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

