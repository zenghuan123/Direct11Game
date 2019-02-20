#include "Texture.h"


namespace Direct11Game {
	void Texture::SetSampler_(vector<ID3D11SamplerState*> sampler)
	{
		for (int i = 0; i < this->sampler_.size(); i++)
		{
			if (sampler_[i])
				sampler[i]->Release();
		}
		sampler_.clear();
		this->sampler_ = sampler;
		this->sampler_;
		for (int i = 0; i < this->sampler_.size(); i++)
		{
			if (sampler_[i])
				sampler[i]->AddRef();
		}

	}
	void Texture::SetUp(ID3D11DeviceContext * context)
	{
		context->PSSetShaderResources(0, (UINT)resource_.size(), &resource_[0]);
		context->PSSetSamplers(0, (UINT)sampler_.size(), &sampler_[0]);
	}
	Texture::Texture()
	{
	}

	Texture::~Texture()
	{
		for (int i = 0; i < this->sampler_.size(); i++)
		{
			if (sampler_[i])
				sampler_[i]->Release();
		}
		sampler_.clear();
	}
}
