#include "tyre.h"
#include "resourceManager.h"
namespace Direct11Game {
	void Tyre::SetTurn(float turn)
	{
		this->turn_ = turn;
	}
	void Tyre::SetSpeed(float speed)
	{
		this->speed_ = speed;
	}
	Tyre::Tyre(GameObject* parent):GameObject(parent),turn_(0),speed_(0)
	{
		this->shader = ResourceManager::CreateShader(L"default.fx");
		
		this->meshRender_ = ResourceManager::CreateCylinderMesh2();
		this->meshRender2_ = ResourceManager::CreateCircleMesh();

		this->transform->SetPosition(0, 0, 0);

		wstring name = L"texture/1.jpg";

		this->texture = ResourceManager::CreateTexture(&name,1);
		name = L"texture/tyre.png";
		this->texture2_ = ResourceManager::CreateTexture(&name, 1);
		this->transform->SetRotation(0, 0, 0);
		
	}




	Tyre::~Tyre()
	{

	}
	void Tyre::Update(float t)
	{
		//弧长等于半径乘以弧度
		//turn 是偏的角度
		//speed是转速
		GetTransform()->SetRotation(0, turn_, GetTransform()->GetRotation().z + speed_ / 0.3f*t);
		
	}
	void Tyre::Render(ID3D11DeviceContext * context)
	{

		GameObject::Render(context);
		XMMATRIX world = XMMatrixTranspose(XMMatrixTranslation(0, 0, 0.1f)*this->transform->GetWorldMatrix());
		ID3D11Buffer* worldBuffer = ResourceManager::GetSharedWorldBuffer();
		context->UpdateSubresource(worldBuffer, 0, nullptr, &world, 0, 0);
		context->VSSetConstantBuffers(0, 1, &worldBuffer);

		texture2_->SetUp(context);
		meshRender2_->Draw(context);

		world = XMMatrixTranspose(XMMatrixRotationY(XM_PI)*XMMatrixTranslation(0, 0, -0.1f)*this->transform->GetWorldMatrix());
		
		context->UpdateSubresource(worldBuffer, 0, nullptr, &world, 0, 0);
		context->VSSetConstantBuffers(0, 1, &worldBuffer);
		meshRender2_->Draw(context);
	}
}
