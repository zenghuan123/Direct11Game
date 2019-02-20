#include "scene.h"


namespace Direct11Game {

	Scene::Scene()
	{

	}


	Scene::~Scene()
	{
		std::set<GameObject*>::iterator it = objectSet_.begin();
		while (it != objectSet_.end())
		{
			delete (*it);
			it++;
		}
		objectSet_.clear();

		if (mainCamera_)
		{
			delete mainCamera_;
			mainCamera_ = 0;
		}

	}

	void Scene::AddGameObject(GameObject * ob)
	{
		this->objectSet_.insert(ob);
	}

	void Scene::Init(ID3D11Device* device, ID3D11DeviceContext* context)
	{

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(XMMATRIX);
		desc.CPUAccessFlags = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		XMMATRIX projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV4,1280.0f/ 720.0f, 0.01f, 2000.0f));
		
		device->CreateBuffer(&desc, nullptr, &this->viewBuffer_);



		device->CreateBuffer(&desc, nullptr, &this->projectionBuffer_);
		context->UpdateSubresource(this->projectionBuffer_, 0, nullptr, &projection, 0, 0);
		context->VSSetConstantBuffers(2, 1, &this->projectionBuffer_);
		desc.ByteWidth = sizeof(dirLight);

		HR(device->CreateBuffer(&desc, nullptr, &this->buffer));

		mainCamera_ = new Camera();
		this->CreateScene(device, context);
		
	}

	Camera* Scene::GetMainCamera()
	{
		return this->mainCamera_;
	}


	void Scene::Update(float t)
	{
		std::set<GameObject*>::iterator it = objectSet_.begin();
		while (it != objectSet_.end())
		{
			(*it)->Update(t);
			it++;
		}
		mainCamera_->Update(t);
		light.direction = { 1,-1,-1 };

		static  float time = 0;
		time = time + t * 2;
		if (time <10)
		{
			light.radiance = { 1,1,1 };
		}else if(time<20)
		{
			light.radiance = { 2,0,0 };
		}
		else if (time < 30)
		{
			light.radiance = { 0,2,0 };
		}
		else if (time < 40)
		{
			light.radiance = { 0,0,2 };
		}else if(time<50)
		{
			light.radiance.x =(float) ((int)time%2);
			light.radiance.y =(float) ((int)time%3);
			light.radiance.z = (float)((int)time%5);
			
		}
		else {
			time = 0;
		}
		light.viewPos = mainCamera_->GetPosition();

	}


	void Scene::Render(ID3D11DeviceContext*context)
	{
		XMVECTOR ve[3];
		ve[0]= XMLoadFloat3(&light.direction);
		ve[1] = XMLoadFloat3(&light.radiance);
		ve[2] = XMLoadFloat3(&light.viewPos);
		context->UpdateSubresource(this->buffer,0,nullptr,&ve,0,0);
		context->PSSetConstantBuffers(3,1,&this->buffer);

		XMMATRIX viewMatrix = XMMatrixTranspose(mainCamera_->GetViewMatrix());
		context->UpdateSubresource(this->viewBuffer_, 0, nullptr,&viewMatrix , 0, 0);
		context->VSSetConstantBuffers(1, 1, &this->viewBuffer_);
		std::set<GameObject*>::iterator it = objectSet_.begin();
		while (it!=objectSet_.end())
		{
			(*it)->Render(context);
			it++;
		}
		mainCamera_->Render(context);
		
	}

}