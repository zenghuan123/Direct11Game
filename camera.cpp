#include "camera.h"
#include "resourceManager.h"
namespace Direct11Game {
	XMMATRIX Camera::GetViewMatrix()
	{
		/*
		Transform*transform = targetGameObject->GetTransform();
		XMFLOAT3 position = transform->GetPosition();
		XMFLOAT3 rotation = transform->GetRotation();
		XMMATRIX xm = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)*XMMatrixTranslation(position.x, position.y, position.z);		
		return XMMatrixLookAtLH(XMVector3TransformCoord(this->position_, xm), XMVector3TransformCoord(this->target_, xm),
			XMVector3TransformNormal(this->up_, xm));
		*/
		return XMMatrixLookAtLH(XMLoadFloat3(&this->position_), XMLoadFloat3(&this->target_), this->up_);
	}
	Camera::Camera() :left_(0), right_(0), front_(0), back_(0), top_(0), meshRender_(0), destance(600), shader_(0)
	{
		
		position_ = { 0, 0.f, -10.f };
		target_ = { 0, 0, 0.f };
		up_ = XMVectorSet(0.0f, 1.0f, .0f, 1.0f);
		
		//this->targetGameObject = targetGameObject;
	}
	void Camera::SetShader(Shader * shader)
	{
		if (this->shader_)
			delete this->shader_;
		this->shader_ = shader;
	}
	void Camera::SetMeshRender(MeshRenderer * mesh)
	{
		if (this->meshRender_)
			delete this->meshRender_;
		this->meshRender_ = mesh;

	}
	void Camera::SetSkyBox(Texture * top, Texture * left, Texture * right, Texture * front, Texture * back)
	{
		if (this->top_)
			delete this->top_;
		this->top_ = top;
		if (this->left_)
			delete this->left_;
		this->left_ = left;
		if (this->right_)
			delete this->right_;
		this->right_ = right;
		if (this->front_)
			delete this->front_;
		this->front_ = front;
		if (this->back_)
			delete this->back_;
		this->back_ = back;
	}
	void Camera::SetPosition(float x, float y, float z)
	{
		this->position_.x = x;
		this->position_.y = y;
		this->position_.z = z;
	}
	void Camera::SetTarget(float x, float y, float z)
	{
		this->target_.x = x;
		this->target_.y = y;
		this->target_.z = z;
	}

	void Camera::SetPosition(FXMVECTOR ve)
	{
		XMStoreFloat3(&this->position_, ve);
	}

	void Camera::SetTarget(FXMVECTOR ve)
	{
		XMStoreFloat3(&this->target_, ve);
	}

	void Camera::Update(float t)
	{
		/*
		if (KEYDOWN(GetAsyncKeyState(VK_LEFT)))
		{
			position_ = XMVectorAdd(position_, XMVectorSet(t, 0, 0, 0));
			target_ = XMVectorAdd(target_, XMVectorSet(t, 0, 0, 0));
		}
		else if (KEYDOWN(GetAsyncKeyState(VK_RIGHT)))
		{
			position_ = XMVectorSubtract(position_, XMVectorSet(t, 0, 0, 0));
			target_ = XMVectorSubtract(target_, XMVectorSet(t, 0, 0, 0));
			
		}
		*/
		//GetCursorPos();
	}

	void Camera::Render(ID3D11DeviceContext * context)
	{
		static float t = 1.01f;
		
		XMMATRIX world =XMMatrixScaling(destance*t,destance*t,destance)*XMMatrixTranslation(0, 0, destance)*XMMatrixTranslation(this->position_.x,this->position_.y,this->position_.z);
		world= XMMatrixTranspose(world);
		ID3D11Buffer*worldBuffer = ResourceManager::GetSharedWorldBuffer();
		context->UpdateSubresource(worldBuffer, 0, nullptr, &world, 0, 0);
		context->VSSetConstantBuffers(0, 1, &worldBuffer);
		this->shader_->SetUp(context);
		this->front_->SetUp(context);
		this->meshRender_->Draw(context);

		world = XMMatrixScaling(destance*t, destance*t, destance)*XMMatrixTranslation(0, 0,destance)*XMMatrixRotationY(XM_PI)*XMMatrixTranslation(this->position_.x, this->position_.y, this->position_.z);
		world = XMMatrixTranspose(world);
		context->UpdateSubresource(worldBuffer, 0, nullptr, &world, 0, 0);
		context->VSSetConstantBuffers(0, 1, &worldBuffer);
		this->back_->SetUp(context);
		this->meshRender_->Draw(context);

		world = XMMatrixScaling(destance*t, destance*t, destance)*XMMatrixTranslation(0, 0, destance)*XMMatrixRotationY(-XM_PIDIV2)*XMMatrixTranslation(this->position_.x, this->position_.y, this->position_.z);
		world = XMMatrixTranspose(world);
		context->UpdateSubresource(worldBuffer, 0, nullptr, &world, 0, 0);
		context->VSSetConstantBuffers(0, 1, &worldBuffer);
		this->left_->SetUp(context);
		this->meshRender_->Draw(context);


		world = XMMatrixScaling(destance*t, destance*t, destance)*XMMatrixTranslation(0, 0, destance)*XMMatrixRotationY(XM_PIDIV2)*XMMatrixTranslation(this->position_.x, this->position_.y, this->position_.z);
		world = XMMatrixTranspose(world);
		context->UpdateSubresource(worldBuffer, 0, nullptr, &world, 0, 0);
		context->VSSetConstantBuffers(0, 1, &worldBuffer);
		this->right_->SetUp(context);
		this->meshRender_->Draw(context);

		world = XMMatrixScaling(destance*t, destance*t, destance)*XMMatrixTranslation(0, 0, destance)*XMMatrixRotationX(-XM_PIDIV2)*XMMatrixTranslation(this->position_.x, this->position_.y, this->position_.z);
		world = XMMatrixTranspose(world);
		context->UpdateSubresource(worldBuffer, 0, nullptr, &world, 0, 0);
		context->VSSetConstantBuffers(0, 1, &worldBuffer);
		this->top_->SetUp(context);
		this->meshRender_->Draw(context);


	}
	

	Camera::~Camera()
	{
		if (this->back_)
			delete this->back_;
		if (this->front_)
			delete this->front_;
		if (this->left_)
			delete this->left_;
		if (this->right_)
			delete this->right_;
		if (this->top_)
			delete this->top_;
		if (this->meshRender_)
			delete this->meshRender_;
		if (this->shader_)
			delete this->shader_;
	}
	
}
