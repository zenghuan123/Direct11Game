#include "resourceManager.h"

static char VS_MAIN[3] = "VS";
static char VS_VERSION[7] = "vs_4_0";
static char PS_MAIN[3] = "PS";
static char PS_VERSION[7] = "ps_4_0";
static D3D11_INPUT_ELEMENT_DESC defaultInputLayout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
static wstring defaultTexture = L"texture/defaultTexture.png";
/*
这个需要析构
*/
static ID3D11SamplerState *sampler=0;

namespace Direct11Game {
	ResourceManager* ResourceManager::staticInstance_ = 0;
	void ResourceManager::Init(ID3D11Device * device)
	{
		if (staticInstance_==nullptr)
		{
			staticInstance_ = new ResourceManager();
		}
		
		staticInstance_->device_ = device;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(XMMATRIX);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		HR(device->CreateBuffer(&bd, nullptr, &staticInstance_->worldBuffer_));
		if (sampler == nullptr)
		{
			D3D11_SAMPLER_DESC colorMapDesc;
			ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
			colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;
			staticInstance_->device_->CreateSamplerState(&colorMapDesc, &sampler);
			sampler->AddRef();
		}

		Texture *t=ResourceManager::CreateTexture(&defaultTexture, 1);



		delete t;
	}
	void ResourceManager::Release()
	{
		delete staticInstance_;
	}

	Texture * ResourceManager::GetDefaultTexture()
	{
		return ResourceManager::CreateTexture(&defaultTexture, 1);
	}

	std::vector<MeshRenderer*>ResourceManager::GetModuleMesh(std::string path, std::string directory)
	{
		
		MeshIterator it = staticInstance_->meshMap_.find(path);
		if (it != staticInstance_->meshMap_.end())
		{
			return it->second;
		}
		else 
		{
			std::vector<MeshRenderer*> load = LoadModule(path, directory);
			staticInstance_->meshMap_.emplace(path, load);
			return load;
		}
		
	}

	Texture * ResourceManager::CreateTexture(std::wstring * name, unsigned size)
	{
		Texture *texture = new Texture();
		for (unsigned i = 0; i < size; i++)
		{
			texture->resource_.push_back(staticInstance_->LoadResourceView(name[i]));
			
			sampler->AddRef();
			texture->sampler_.push_back(sampler);

		}
		return texture;
	}

	Shader * ResourceManager::CreateShader(std::wstring name)
	{
		Shader *shader = new Shader();
		//浅拷贝
		*shader = *(staticInstance_->LoadShader(name));
		return shader;	
	}

	ID3D11Buffer * ResourceManager::GetSharedWorldBuffer()
	{
		return staticInstance_->worldBuffer_;
	}




	MeshRenderer * ResourceManager::CreateCubeMesh()
	{
		static MeshRenderer cubeMesh;
		static bool hasCreate = false;
		if (hasCreate)
		{
			MeshRenderer *mesh = new MeshRenderer(cubeMesh);
			return mesh;
		}
		XMFLOAT3 vertices[] =
		{
		{0,0,0},{0,1,0},{1,1,0},{1,0,0},
		{0,0,0},{0,0,1},{0,1,1 },{0,1,0},
		{0,0,1},{1,0,1},{1,1,1},{0,1,1},
		{1,0,1},{1,0,0},{1,1,0},{1,1,1},
		{1,1,1},{1,1,0},{0,1,0},{0,1,1},

		{0,0,1},{0,0,0},{1,0,0}, {1,0,1},

		};
		XMFLOAT2 texcoords[] =
		{
		{0,0.4969697f}, {0,0},{0.355278093f,0},{0.355278093f,0.4969697f},
		{0,0.907575758f},{0.1861521f,0.907575758f},{0.1861521f,0.500757576f},{0,0.500757576f},
		{0,0.4969697f},{0.355278093f,0.4969697f},{0.355278093f,0},{0,0},
		{0.360196746f,0.503030303f},{0.543700341f,0.503030303f},{0.543700341f,0},{0.360196746f,0},
		{0.544457056f,0.8916666667f},{0.544457056f,0.500757576f},{0.187287174f,0.500757576f},{0.187287174f,0.8916666667f},

		{0.56753689f,1.f - 0.53030303f},{0.870223231f,1.f - 0.53030303f},{0.870223231f,1.f - 0.882352941f},{0.56753689f,1.f - 0.882352941f}
		};
		XMFLOAT3 normals[] = {
		{0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1},
		{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},
		{0,0,1},{0,0,1},{0,0,1},{0,0,1},
		{1,0,0},{1,0,0},{1,0,0},{1,0,0},
		{0,1,0},{0,1,0},{0,1,0},{0,1,0},
		{0,-1,0},{0,-1,0},{0,-1,0},{0,-1,0},
		};
		WORD indices[] =
		{
			0,1,2,2,3,0,
			4,5,6,6,7,4,
			8,9,10,10,11,8,
			12,13,14,14,15,12,
			16,17,18,18,19,16,
			20,21,22,22,23,20

		};



	

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(XMFLOAT3) * 24;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;


		cubeMesh.vertexBuffer_.resize(3);
		cubeMesh.strides_.resize(3);
		cubeMesh.offsets_.resize(3);

		HR(staticInstance_->device_->CreateBuffer(&bd, &InitData, &(cubeMesh.vertexBuffer_[0])));
		bd.ByteWidth = sizeof(XMFLOAT2) * 24;
		InitData.pSysMem = texcoords;
		HR(staticInstance_->device_->CreateBuffer(&bd, &InitData, &(cubeMesh.vertexBuffer_[1])));
		bd.ByteWidth = sizeof(XMFLOAT3) * 24;
		InitData.pSysMem = normals;
		HR(staticInstance_->device_->CreateBuffer(&bd, &InitData, &(cubeMesh.vertexBuffer_[2])));

		

		// Set vertex buffer

		cubeMesh.strides_[0] = { sizeof(XMFLOAT3) };
		cubeMesh.strides_[1] = sizeof(XMFLOAT2);
		cubeMesh.strides_[2] = sizeof(XMFLOAT3);
		cubeMesh.offsets_[0] = 0;
		cubeMesh.offsets_[1] = 0;
		cubeMesh.offsets_[2] = 0;
		cubeMesh.indexNum = 36;


		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * 36;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = indices;
		HR(staticInstance_->device_->CreateBuffer(&bd, &InitData, &cubeMesh.indexBuffer_));
		
		hasCreate = true;
		MeshRenderer *mesh = new MeshRenderer(cubeMesh);
		return mesh;
	}
	MeshRenderer * ResourceManager::CreatePlaneMesh()
	{
		static MeshRenderer mesh;
		static bool hasCreate = false;
		if (hasCreate)
		{
			return new MeshRenderer(mesh);
		}
		XMFLOAT3 vertexs[4];
		XMFLOAT2 texcoords[4];
		XMFLOAT3 normals[4] = {
			{0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1}
		};
		WORD indexs[6] = {
			0,1,2,2,3,0
		};

		vertexs[0] = XMFLOAT3(-1, -1, 0);
		vertexs[1] = XMFLOAT3(-1, 1, 0);
		vertexs[2] = XMFLOAT3(1, 1, 0);
		vertexs[3] = XMFLOAT3(1, -1, 0);

		texcoords[0] = { 0,1 };
		texcoords[1] = { 0,0 };
		texcoords[2] = { 1,0 };
		texcoords[3] = { 1,1 };

		mesh.vertexBuffer_.resize(3);
		mesh.strides_.push_back(sizeof(XMFLOAT3));
		mesh.strides_.push_back(sizeof(XMFLOAT2));
		mesh.strides_.push_back(sizeof(XMFLOAT3));
		mesh.offsets_.push_back(0);
		mesh.offsets_.push_back(0);
		mesh.offsets_.push_back(0);

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(XMFLOAT3) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = vertexs;
		HR(staticInstance_->device_->CreateBuffer(&bd, &initData, &mesh.vertexBuffer_[0]));
		bd.ByteWidth = sizeof(XMFLOAT2) * 4;
		initData.pSysMem = texcoords;
		HR(staticInstance_->device_->CreateBuffer(&bd, &initData, &mesh.vertexBuffer_[1]));
		bd.ByteWidth = sizeof(XMFLOAT3) * 4;
		initData.pSysMem = normals;
		HR(staticInstance_->device_->CreateBuffer(&bd, &initData, &mesh.vertexBuffer_[2]));

		bd.ByteWidth = sizeof(WORD) * 6;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		initData.pSysMem = indexs;
		HR(staticInstance_->device_->CreateBuffer(&bd, &initData, &mesh.indexBuffer_));
		mesh.indexNum = 6;
		
		hasCreate = true;
		return new MeshRenderer(mesh);
	}
	MeshRenderer * ResourceManager::CreateCylinderMesh()
	{
		static MeshRenderer mesh;
		static bool hasCreate = false;
		if (hasCreate)
		{
			MeshRenderer *result= new MeshRenderer(mesh);
			return result;
		}
		float z = 0.1f;
		int precision = 4;
		XMFLOAT3 *vertexs = new XMFLOAT3[precision * 16 + 2];//为了循环好运算 同时最后两个点作为圆心
		WORD *indexs = new WORD[precision * 96];
		for (int i = 0; i <= precision; i++)
		{
			float x = cos(XM_PIDIV4*i / (float)precision);
			float y = sin(XM_PIDIV4*i / (float)precision);
			vertexs[i] = XMFLOAT3(x, y, z);
			vertexs[precision * 2 - i] = XMFLOAT3(y, x, z);
			vertexs[precision * 2 + i] = XMFLOAT3(-y, x, z);
			vertexs[precision * 4 - i] = XMFLOAT3(-x, y, z);
			vertexs[precision * 4 + i] = XMFLOAT3(-x, -y, z);
			vertexs[precision * 6 - i] = XMFLOAT3(-y, -x, z);
			vertexs[precision * 6 + i] = XMFLOAT3(y, -x, z);
			vertexs[precision * 8 - i] = XMFLOAT3(x, -y, z);

			vertexs[precision * 8 + i] = XMFLOAT3(x, y, -z);
			vertexs[precision * 10 - i] = XMFLOAT3(y, x, -z);
			vertexs[precision * 10 + i] = XMFLOAT3(-y, x, -z);
			vertexs[precision * 12 - i] = XMFLOAT3(-x, y, -z);
			vertexs[precision * 12 + i] = XMFLOAT3(-x, -y, -z);
			vertexs[precision * 14 - i] = XMFLOAT3(-y, -x, -z);
			vertexs[precision * 14 + i] = XMFLOAT3(y, -x, -z);
			vertexs[precision * 16 - i] = XMFLOAT3(x, -y, -z);
		}
		vertexs[precision * 16] = XMFLOAT3(0, 0, z);
		vertexs[precision * 16 + 1] = XMFLOAT3(0, 0, -z);
		for (int j = 0; j < precision * 8 - 1; j++)
		{
			indexs[j * 12] = j;
			indexs[j * 12 + 1] = precision * 8 + j;
			indexs[j * 12 + 2] = j + 1;
			indexs[j * 12 + 3] = precision * 8 + j;
			indexs[j * 12 + 4] = precision * 8 + j + 1;
			indexs[j * 12 + 5] = j + 1;
			indexs[j * 12 + 6] = precision * 16;
			indexs[j * 12 + 7] = j;
			indexs[j * 12 + 8] = j + 1;
			indexs[j * 12 + 9] = precision * 16 + 1;
			indexs[j * 12 + 10] = precision * 8 + j + 1;
			indexs[j * 12 + 11] = precision * 8 + j;
		}
		indexs[precision * 96 - 12] = precision * 16;
		indexs[precision * 96 - 11] = precision * 8 - 1;
		indexs[precision * 96 - 10] = 0;
		indexs[precision * 96 - 9] = precision * 16 + 1;
		indexs[precision * 96 - 8] = precision * 8;
		indexs[precision * 96 - 7] = precision * 16 - 1;
		indexs[precision * 96 - 6] = precision * 8 - 1;
		indexs[precision * 96 - 5] = precision * 16 - 1;
		indexs[precision * 96 - 4] = 0;
		indexs[precision * 96 - 3] = precision * 16 - 1;
		indexs[precision * 96 - 2] = precision * 8;
		indexs[precision * 96 - 1] = 0;


		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(XMFLOAT3) *(precision * 16 + 2);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertexs;
		
		mesh.vertexBuffer_.resize(1);
		mesh.strides_.resize(1);
		mesh.offsets_.resize(1);

		HR(staticInstance_->device_->CreateBuffer(&bd, &InitData, &mesh.vertexBuffer_[0]));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) *precision * 96;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = indexs;
		HR(staticInstance_->device_->CreateBuffer(&bd, &InitData, &mesh.indexBuffer_));

		mesh.strides_[0] = sizeof(XMFLOAT3);
		mesh.offsets_[0] = 0;
		mesh.indexNum = precision * 96;

		delete vertexs;
		delete indexs;
		hasCreate = true;
		MeshRenderer *result = new MeshRenderer(mesh);
		return result;
	}
	MeshRenderer * ResourceManager::CreateCylinderMesh2()
	{
		static MeshRenderer mesh;
		static bool hasCreate = false;
		if (hasCreate)
		{
			MeshRenderer *result = new MeshRenderer(mesh);
			return result;
		}
		float z = 0.2f;
		int precision = 4;
		//flaot r=1;

		XMFLOAT3 *vertexs = new XMFLOAT3[precision * 16 + 1];//加1是为了循环好运算
		XMFLOAT2 *texcoords = new XMFLOAT2[precision * 16+1];
		XMFLOAT3*normals = new XMFLOAT3[precision * 16+1];
		WORD *indexs = new WORD[precision * 48];

		double addition = XM_2PI * 1 / (8 * precision);


		for (int i = 0; i < precision * 8; i++)
		{
			texcoords[i] = XMFLOAT2(0, (float)(i*addition));
			texcoords[i + 8 * precision] = XMFLOAT2(1, (float)(i*addition));
		}

		for (int i = 0; i <= precision; i++)
		{
			float x = cos(XM_PIDIV4*i / (float)precision);
			float y = sin(XM_PIDIV4*i / (float)precision);
			vertexs[i] = XMFLOAT3(x, y, z);
			vertexs[precision * 2 - i] = XMFLOAT3(y, x, z);
			vertexs[precision * 2 + i] = XMFLOAT3(-y, x, z);
			vertexs[precision * 4 - i] = XMFLOAT3(-x, y, z);
			vertexs[precision * 4 + i] = XMFLOAT3(-x, -y, z);
			vertexs[precision * 6 - i] = XMFLOAT3(-y, -x, z);
			vertexs[precision * 6 + i] = XMFLOAT3(y, -x, z);
			vertexs[precision * 8 - i] = XMFLOAT3(x, -y, z);

			vertexs[precision * 8 + i] = XMFLOAT3(x, y, -z);
			vertexs[precision * 10 - i] = XMFLOAT3(y, x, -z);
			vertexs[precision * 10 + i] = XMFLOAT3(-y, x, -z);
			vertexs[precision * 12 - i] = XMFLOAT3(-x, y, -z);
			vertexs[precision * 12 + i] = XMFLOAT3(-x, -y, -z);
			vertexs[precision * 14 - i] = XMFLOAT3(-y, -x, -z);
			vertexs[precision * 14 + i] = XMFLOAT3(y, -x, -z);
			vertexs[precision * 16 - i] = XMFLOAT3(x, -y, -z);

			normals[i] = XMFLOAT3(x, y, 0);
			normals[precision * 2 - i] = XMFLOAT3(y, x, 0);
			normals[precision * 2 + i] = XMFLOAT3(-y, x, 0);
			normals[precision * 4 - i] = XMFLOAT3(-x, y, 0);
			normals[precision * 4 + i] = XMFLOAT3(-x, -y, 0);
			normals[precision * 6 - i] = XMFLOAT3(-y, -x, 0);
			normals[precision * 6 + i] = XMFLOAT3(y, -x, 0);
			normals[precision * 8 - i] = XMFLOAT3(x, -y, 0);

			normals[precision * 8 + i] = XMFLOAT3(x, y, -0);
			normals[precision * 10 - i] = XMFLOAT3(y, x, -0);
			normals[precision * 10 + i] = XMFLOAT3(-y, x, -0);
			normals[precision * 12 - i] = XMFLOAT3(-x, y, -0);
			normals[precision * 12 + i] = XMFLOAT3(-x, -y, -0);
			normals[precision * 14 - i] = XMFLOAT3(-y, -x, -0);
			normals[precision * 14 + i] = XMFLOAT3(y, -x, -0);
			normals[precision * 16 - i] = XMFLOAT3(x, -y, -0);

		}
		for (int j = 0; j < precision * 8 - 1; j++)
		{
			indexs[j * 6] = j;
			indexs[j * 6 + 1] = precision * 8 + j;
			indexs[j * 6 + 2] = j + 1;
			indexs[j * 6 + 3] = precision * 8 + j;
			indexs[j * 6 + 4] = precision * 8 + j + 1;
			indexs[j * 6 + 5] = j + 1;

		}

		indexs[precision * 48 - 6] = precision * 8 - 1;
		indexs[precision * 48 - 5] = precision * 16 - 1;
		indexs[precision * 48 - 4] = 0;
		indexs[precision * 48 - 3] = precision * 16 - 1;
		indexs[precision * 48 - 2] = precision * 8;
		indexs[precision * 48 - 1] = 0;


		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(XMFLOAT3) *(precision * 16);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertexs;

		mesh.vertexBuffer_.resize(3);
		mesh.strides_.resize(3);
		mesh.offsets_.resize(3);

		HR(staticInstance_->device_->CreateBuffer(&bd, &InitData, &mesh.vertexBuffer_[0]));
		InitData.pSysMem = normals;
		HR(staticInstance_->device_->CreateBuffer(&bd, &InitData, &mesh.vertexBuffer_[2]));


		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) *precision * 48;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = indexs;
		HR(staticInstance_->device_->CreateBuffer(&bd, &InitData, &mesh.indexBuffer_));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(XMFLOAT2)*precision * 16;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = texcoords;

		HR(staticInstance_->device_->CreateBuffer(&bd, &InitData, &mesh.vertexBuffer_[1]));

		mesh.strides_[0] = sizeof(XMFLOAT3);
		mesh.offsets_[0] = 0;
		mesh.strides_[1] = sizeof(XMFLOAT2);
		mesh.offsets_[1] = 0;
		mesh.strides_[2] = sizeof(XMFLOAT3);
		mesh.offsets_[2] = 0;

		mesh.indexNum = precision * 48;

		delete vertexs;
		delete indexs;
		delete normals;
		hasCreate = true;
		MeshRenderer *result = new MeshRenderer(mesh);
		return result;
	}
	MeshRenderer * ResourceManager::CreateCircleMesh()
	{
		static MeshRenderer mesh;
		static bool hasCreate = false;
		if (hasCreate)
		{
			MeshRenderer *result = new MeshRenderer(mesh);
			return result;
		}
		float z = 0.1f; int precision = 4;
		mesh.vertexBuffer_.resize(3);
		mesh.strides_.resize(3);
		mesh.offsets_.resize(3);

		XMFLOAT3 *vertexs = new XMFLOAT3[precision * 8 + 1];//为了循环好运算,同时保存圆心

		XMFLOAT2 *texcoords = new XMFLOAT2[precision * 8 + 1];
		XMFLOAT3 *normals = new XMFLOAT3[precision * 8 + 1];
		WORD *indexs = new WORD[precision * 24];

		for (int i = 0; i <= precision; i++)
		{
			float x = cos(XM_PIDIV4*i / (float)precision);
			float y = sin(XM_PIDIV4*i / (float)precision);
			vertexs[i] = XMFLOAT3(x, y, z);
			vertexs[precision * 2 - i] = XMFLOAT3(y, x, z);
			vertexs[precision * 2 + i] = XMFLOAT3(-y, x, z);
			vertexs[precision * 4 - i] = XMFLOAT3(-x, y, z);
			vertexs[precision * 4 + i] = XMFLOAT3(-x, -y, z);
			vertexs[precision * 6 - i] = XMFLOAT3(-y, -x, z);
			vertexs[precision * 6 + i] = XMFLOAT3(y, -x, z);
			vertexs[precision * 8 - i] = XMFLOAT3(x, -y, z);

			texcoords[i] = XMFLOAT2((1 + x) / 2.f, (1 + y) / 2.f);
			texcoords[precision * 2 - i] = XMFLOAT2((1 + y) / 2.f, (1 + x) / 2.f);;
			texcoords[precision * 2 + i] = XMFLOAT2((1 - y) / 2.f, (1 + x) / 2.f);
			texcoords[precision * 4 - i] = XMFLOAT2((1 - x) / 2.f, (1 + y) / 2.f);
			texcoords[precision * 4 + i] = XMFLOAT2((1 - x) / 2.f, (1 - y) / 2.f);
			texcoords[precision * 6 - i] = XMFLOAT2((1 - y) / 2.f, (1 - x) / 2.f);
			texcoords[precision * 6 + i] = XMFLOAT2((1 + y) / 2.f, (1 - x) / 2.f);

			texcoords[precision * 8 - i] = XMFLOAT2((1 + x) / 2.f, (1 - y) / 2.f);
			normals[i] = XMFLOAT3(0, 0, 1);
			normals[precision * 2 - i] = XMFLOAT3(0, 0, 1);
			normals[precision * 2 + i] = XMFLOAT3(-0, 0, 1);
			normals[precision * 4 - i] = XMFLOAT3(-0, 0, 1);
			normals[precision * 4 + i] = XMFLOAT3(-0, -0, 1);
			normals[precision * 6 - i] = XMFLOAT3(-0, -0, 1);
			normals[precision * 6 + i] = XMFLOAT3(0, -0, 1);
			normals[precision * 8 - i] = XMFLOAT3(0, -0, 1);
		}
		vertexs[precision * 8] = XMFLOAT3(0, 0, 0);
		texcoords[precision * 8] = XMFLOAT2(0.5, 0.5);

		for (int i = 0; i < precision * 8; i++)
		{
			indexs[i * 3] = precision * 8;
			indexs[i * 3 + 1] = i;
			indexs[i * 3 + 2] = i + 1;
		}
		indexs[precision * 24 - 1] = 0;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(XMFLOAT3) *(precision * 8 + 1);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertexs;
		HR(staticInstance_->device_->CreateBuffer(&bd, &InitData, &mesh.vertexBuffer_[0]));

		InitData.pSysMem = normals;
		HR(staticInstance_->device_->CreateBuffer(&bd, &InitData, &mesh.vertexBuffer_[2]));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) *precision * 24;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = indexs;
		HR(staticInstance_->device_->CreateBuffer(&bd, &InitData, &mesh.indexBuffer_));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(XMFLOAT2)*(precision * 8 + 1);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = texcoords;
		HR(staticInstance_->device_->CreateBuffer(&bd, &InitData, &mesh.vertexBuffer_[1]));


		mesh.strides_[0] = { sizeof(XMFLOAT3) };
		mesh.strides_[1] = { sizeof(XMFLOAT2) };
		mesh.strides_[2] = sizeof(XMFLOAT3);
		mesh.offsets_[0] = 0;
		mesh.offsets_[1] = 0;
		mesh.offsets_[2] = 0;
		mesh.indexNum = precision * 24;


		delete vertexs;
		delete indexs;
		delete texcoords;
		delete normals;
		hasCreate = true;
		MeshRenderer *result = new MeshRenderer(mesh);
		return result;
	}

	ID3D11ShaderResourceView * ResourceManager::LoadResourceView(std::wstring name)
	{

		TextureIterator it = staticInstance_->texureMap_.find(name);
		if (it != staticInstance_->texureMap_.end())
		{
			return it->second;
		}
		ID3D11ShaderResourceView* resource;
		HR(CreateWICTextureFromFile(staticInstance_->device_, name.c_str(), nullptr, &resource));
		staticInstance_->texureMap_.emplace(name, resource);
		return resource;
	}
	Shader * ResourceManager::LoadShader(std::wstring name)
	{
		ShaderIterator it = staticInstance_->shaderMap_.find(name);
		if (it != staticInstance_->shaderMap_.end())
		{
			return it->second;
		}
		Shader*shader=new Shader();
		ID3DBlob* blob = nullptr;
		HR(CompileShaderFromFile(name.c_str(), VS_MAIN, VS_VERSION, &blob));

		HR(staticInstance_->device_->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader->vertexShader_));

		HR(staticInstance_->device_->CreateInputLayout(defaultInputLayout, ARRAYSIZE(defaultInputLayout), blob->GetBufferPointer(), blob->GetBufferSize(), &shader->inputLayout_));

		blob->Release();

		HR(CompileShaderFromFile(name.c_str(), PS_MAIN, PS_VERSION, &blob));

		HR(staticInstance_->device_->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader->pixelShader));
		blob->Release();
		staticInstance_->shaderMap_.emplace(name, shader);
		return shader;
	}

	HRESULT ResourceManager::CompileShaderFromFile(const WCHAR * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;

		// Disable optimizations to further improve shader debugging
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3DBlob* pErrorBlob = nullptr;
		hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
			dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
		if (FAILED(hr))
		{
			if (pErrorBlob)
			{
				OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
				pErrorBlob->Release();
			}
			return hr;
		}
		if (pErrorBlob) pErrorBlob->Release();

		return S_OK;
	}

	std::vector<MeshRenderer*> ResourceManager::LoadModule(std::string path, std::string directory)
	{
		Assimp::Importer importer;

		const aiScene*scene = importer.ReadFile(path,
			aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded);
		std::vector<MeshRenderer*> result;
		
		if (scene)
		{
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(initData));
			

			if (directory.size() == 0)
			{
				directory = path.substr(0, path.find_last_of("/"));
			}
			for (unsigned i = 0; i < scene->mNumMeshes; i++)
			{
				MeshRenderer*meshRenderer = new MeshRenderer();
				meshRenderer->vertexBuffer_.resize(3);
				
				aiMesh*mesh = scene->mMeshes[i];
				
				//float *array =(float*)mesh->mVertices;
				
				bd.ByteWidth = sizeof(float)*3*mesh->mNumVertices;
				initData.pSysMem = mesh->mVertices;
				staticInstance_->device_->CreateBuffer(&bd, &initData, &meshRenderer->vertexBuffer_[0]);

				float *texcoords = (float*)mesh->mTextureCoords[0];
				vector<float>texcoordVec;
				texcoordVec.reserve(mesh->mNumVertices * 2);
				for (unsigned int j = 0; j < mesh->mNumVertices; j++)
				{
					texcoordVec.push_back(mesh->mTextureCoords[0][j].x);
					texcoordVec.push_back(mesh->mTextureCoords[0][j].y);
				}
				bd.ByteWidth = sizeof(float) * 2 * mesh->mNumVertices;
				initData.pSysMem = &texcoordVec[0];
				staticInstance_->device_->CreateBuffer(&bd, &initData, &meshRenderer->vertexBuffer_[1]);

				initData.pSysMem = mesh->mNormals;
				bd.ByteWidth = sizeof(float) * 3 * mesh->mNumVertices;
				HR(staticInstance_->device_->CreateBuffer(&bd, &initData, &meshRenderer->vertexBuffer_[2]));

				vector<unsigned> indexes;
				indexes.reserve(mesh->mNumFaces*3);
				for (unsigned int j = 0; j < mesh->mNumFaces; j++)
				{
					aiFace face = mesh->mFaces[j];
					for (unsigned int m = 0; m < face.mNumIndices; m++)
					{
						indexes.push_back(face.mIndices[m]);
					}
				}
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.ByteWidth =(UINT)indexes.size() * sizeof(unsigned);
				initData.pSysMem = &indexes[0];
				staticInstance_->device_->CreateBuffer(&bd, &initData, &meshRenderer->indexBuffer_);
				
				meshRenderer->indexNum =(UINT) indexes.size();
				meshRenderer->strides_.push_back(sizeof(float) * 3);
				meshRenderer->strides_.push_back(sizeof(float)*2);
				meshRenderer->strides_.push_back(0);
				meshRenderer->offsets_.push_back(0);
				meshRenderer->offsets_.push_back(0);
				meshRenderer->offsets_.push_back(0);
				result.push_back(meshRenderer);
			}
			
		}
		return result;
	}


	ResourceManager::ResourceManager()
	{

	}

	ResourceManager::~ResourceManager()
	{

		TextureIterator it = this->texureMap_.begin();
		while (it != this->texureMap_.end())
		{
			while (it->second->Release() > 0);

			it++;
		}
		this->texureMap_.clear();

		ShaderIterator it2 = this->shaderMap_.begin();
		while (it2 != this->shaderMap_.end())
		{
			while (it2->second->inputLayout_->Release());
			while (it2->second->vertexShader_->Release());
			while (it2->second->pixelShader->Release());
			delete it2->second;
			it2++;
		}
		this->shaderMap_.clear();

		MeshIterator it3 = this->meshMap_.begin();
		while (it3 != this->meshMap_.end())
		{
			for (int i = 0; i < it3->second.size(); i++)
			{
				it3->second[i]->Release();
			}
		}
		
		MeshRenderer*mesh = CreateCubeMesh();
		mesh->Release();
		delete mesh;

		mesh = CreateCylinderMesh();
		mesh->Release();
		delete mesh;

		mesh = CreateCylinderMesh2();
		mesh->Release();
		delete mesh;

		mesh = CreateCircleMesh();
		mesh->Release();
		delete mesh;
		while (sampler->Release() > 0);
		sampler = 0;
	}



}
