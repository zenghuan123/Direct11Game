#include "beginScene.h"
#include "vehicle.h"

namespace Direct11Game {

	BeginScene::BeginScene()
	{
	}

	void BeginScene::CreateScene(ID3D11Device * device, ID3D11DeviceContext * context)
	{
		vector<MeshRenderer*> vec = ResourceManager::GetModuleMesh("model/Island01.FBX");
		Shader*sh = ResourceManager::CreateShader(L"default.fx");
		wstring name = L"model/Ground-grass01.png";
		
		Texture*texture = ResourceManager::CreateTexture(&name, 1);
		for (int i = 0; i < vec.size(); i++)
		{
			GameObject*gameObject = new GameObject();
			gameObject->SetMeshRenderer(vec[i]);
			gameObject->GetTransform()->SetRotation(XM_PIDIV2, 0, 0);

			gameObject->SetShader(sh);
			gameObject->SetTexture(texture);
			this->AddGameObject(gameObject);
		}
		this->GetMainCamera()->SetMeshRender(ResourceManager::CreatePlaneMesh());
		
		std::wstring top = L"texture/bluesky_top.jpg";
		std::wstring left = L"texture/bluesky_left.jpg";
		std::wstring right = L"texture/bluesky_right.jpg";
		std::wstring front = L"texture/bluesky_front.jpg";
		std::wstring back = L"texture/bluesky_back.jpg";
		this->GetMainCamera()->SetSkyBox(
			ResourceManager::CreateTexture(&top, 1),
			ResourceManager::CreateTexture(&left, 1),
			ResourceManager::CreateTexture(&right, 1),
			ResourceManager::CreateTexture(&front, 1),
			ResourceManager::CreateTexture(&back, 1)
			);
		this->GetMainCamera()->SetShader(ResourceManager::CreateShader(L"skybox.fx"));
		GameObject*v = new Vehicle(this->GetMainCamera());
		


		this->AddGameObject(v);
	}


	BeginScene::~BeginScene()
	{
	}
}
