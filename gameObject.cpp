#include "gameObject.h"


namespace Direct11Game {

	GameObject::GameObject(GameObject*p):children(),shader(0),meshRender_(0),parent(0), transform(new Transform()), texture(0)
	{
		if (p)
		{
			p->AddChildren(this);
		}
	}

	GameObject::~GameObject()
	{
		std::set<GameObject*>::iterator it = children.begin();
		while (it != children.end())
		{
			delete (*it);
			it++;
		}
		children.clear();

		if (transform)
		{
			delete transform;
			transform = 0;
		}
		
		if (meshRender_)
		{
			delete meshRender_;
			meshRender_ = 0;
		}
		if (shader)
		{
			delete shader;
			shader = 0;
		}
		if (texture)
		{
			delete texture;
			texture = 0;
		}
	}

	Transform * GameObject::GetTransform()
	{
		return this->transform;
	}

	void GameObject::SetMeshRenderer(MeshRenderer * mesh)
	{

		if (this->meshRender_)
		{
			delete this->meshRender_;
		}
		this->meshRender_ = mesh;

	}

	void GameObject::SetShader(Shader * shader)
	{
		if (this->shader)
			delete this->shader;
		this->shader = shader;
	}

	void GameObject::SetTexture(Texture * texture)
	{
		if (this->texture)
			delete this->texture;
		this->texture = texture;
	}

	void GameObject::AddChildren(GameObject * child)
	{
		if (child)
		{
			if (child->parent)
			{
				child->parent->RemoveChild(child);
			}
			child->parent = this;
			child->transform->parent = this->transform;
			this->children.insert(child);			
		}
	}

	void GameObject::RemoveChild(GameObject * child)
	{
		this->children.erase(child);
		child->transform->parent = 0;
		child->parent = 0;
	}

	

	void GameObject::Update(float interval)
	{

	}

	void GameObject::Render(ID3D11DeviceContext * context)
	{
		XMMATRIX world = XMMatrixTranspose(this->transform->GetWorldMatrix());
		ID3D11Buffer* worldBuffer = ResourceManager::GetSharedWorldBuffer();
		context->UpdateSubresource(worldBuffer, 0, nullptr, &world, 0, 0);
		context->VSSetConstantBuffers(0, 1,&worldBuffer);

		if(this->texture)
			this->texture->SetUp(context);
		if(this->shader)
			this->shader->SetUp(context);
		if(this->meshRender_)
			this->meshRender_->Draw(context);

		std::set<GameObject*>::iterator it = children.begin();
		while (it != children.end())
		{
			(*it)->Render(context);
			it++;
		}
	}


}
