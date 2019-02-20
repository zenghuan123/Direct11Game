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
		保存模型顶点数据
		*/
		MeshMap meshMap_;

		

		ID3D11Device*device_;

		/*
		共享的世界转换矩阵缓存
		*/
		ID3D11Buffer*worldBuffer_;



		/*
		如果name的资源不在textureMap_中那么加载资源到内存
		
		*/
		static ID3D11ShaderResourceView* LoadResourceView(std::wstring name);
		/*
		如果name的shader不在shaderMap_中那么加载并编译为顶点和片段着色器
		*/
		static Shader*LoadShader(std::wstring name);

		/*
		加载模型，
		path使用/分隔
		可以设置模型贴图所在的目录,默认模型所在目录
		该目录会和模型中保存的贴图位置按照director+"/"+模型中读到的name组合
		result需要自个创建
		*/
		static std::vector<MeshRenderer*> LoadModule(std::string path, std::string directory = "");

		


		/*官方CompileShaderFromFile*/
		static HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	

	public:
		/*
		初始化由Application调用
		*/
		static void Init(ID3D11Device*device);
		/*
		释放全部资源由Application调用
		*/
		static void Release();

		/*
		获得默认贴图
		*/
		static Texture*GetDefaultTexture();
		/*
		用于所有GameOjbect 更新世界转换矩阵使用
		*/
		static ID3D11Buffer* GetSharedWorldBuffer();
		/*
		获得一个模型的全部数据
		path为模型路径，directory为贴图所在的目录，
		该目录会和模型中保存的贴图位置按照director+"/"+模型读到的name组合
		outVector是引用
		*/
		static std::vector<MeshRenderer*>GetModuleMesh(std::string path, std::string directory = "");
		
		/*
		新建一个Texture对象,如果name表示的资源不在内存中会加载到内存中
		*/
		static Texture*CreateTexture(std::wstring*name, unsigned size);
		/*
		新建一个Shader对象，如果name表示的shader不在内存中会加载并编译放到内存中
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

