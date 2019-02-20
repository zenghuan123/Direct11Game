#pragma once
#include "stdafx.h"
#include "shader.h"
#include "Texture.h"
#include "meshRenderer.h"
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#pragma comment(lib,"assimp-vc140-mt.lib")

namespace Direct11Game {


	class ResourceManager
	{
		using TextureMap = std::unordered_map<std::wstring, ID3D11ShaderResourceView*>;
		using TextureIterator = TextureMap::iterator;
		using ShaderMap = std::unordered_map<std::wstring, Shader*>;
		using ShaderIterator = ShaderMap::iterator;
		using MeshMap = std::unordered_map<std::string,vector<MeshRenderer*>>;
		using MeshIterator = MeshMap::iterator;

	private:
		static ResourceManager*staticInstance_;
	
		
		TextureMap texureMap_;
		ShaderMap shaderMap_;
		/*
		����ģ�Ͷ�������
		*/
		MeshMap meshMap_;

		

		ID3D11Device*device_;

		/*
		���������ת�����󻺴�
		*/
		ID3D11Buffer*worldBuffer_;



		/*
		���name����Դ����textureMap_����ô������Դ���ڴ�
		
		*/
		static ID3D11ShaderResourceView* LoadResourceView(std::wstring name);
		/*
		���name��shader����shaderMap_����ô���ز�����Ϊ�����Ƭ����ɫ��
		*/
		static Shader*LoadShader(std::wstring name);

		/*
		����ģ�ͣ�
		pathʹ��/�ָ�
		��������ģ����ͼ���ڵ�Ŀ¼,Ĭ��ģ������Ŀ¼
		��Ŀ¼���ģ���б������ͼλ�ð���director+"/"+ģ���ж�����name���
		result��Ҫ�Ը�����
		*/
		static std::vector<MeshRenderer*> LoadModule(std::string path, std::string directory = "");

		


		/*�ٷ�CompileShaderFromFile*/
		static HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	

	public:
		/*
		��ʼ����Application����
		*/
		static void Init(ID3D11Device*device);
		/*
		�ͷ�ȫ����Դ��Application����
		*/
		static void Release();

		/*
		���Ĭ����ͼ
		*/
		static Texture*GetDefaultTexture();
		/*
		��������GameOjbect ��������ת������ʹ��
		*/
		static ID3D11Buffer* GetSharedWorldBuffer();
		/*
		���һ��ģ�͵�ȫ������
		pathΪģ��·����directoryΪ��ͼ���ڵ�Ŀ¼��
		��Ŀ¼���ģ���б������ͼλ�ð���director+"/"+ģ�Ͷ�����name���
		outVector������
		*/
		static std::vector<MeshRenderer*>GetModuleMesh(std::string path, std::string directory = "");
		
		/*
		�½�һ��Texture����,���name��ʾ����Դ�����ڴ��л���ص��ڴ���
		*/
		static Texture*CreateTexture(std::wstring*name, unsigned size);
		/*
		�½�һ��Shader�������name��ʾ��shader�����ڴ��л���ز�����ŵ��ڴ���
		*/
		static Shader*CreateShader(std::wstring name);

		/*
		
		*/
		static MeshRenderer* CreateCubeMesh();
		static MeshRenderer* CreatePlaneMesh();

		static MeshRenderer* CreateCylinderMesh();

		static MeshRenderer* CreateCylinderMesh2();

		static MeshRenderer* CreateCircleMesh();

		ResourceManager();
		virtual ~ResourceManager();
	};
	
}

