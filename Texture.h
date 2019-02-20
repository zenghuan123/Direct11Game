#pragma once
#include "stdafx.h"

namespace Direct11Game {
	class Texture
	{
		friend class  ResourceManager;
	private:
		std::vector<ID3D11ShaderResourceView*> resource_;
		std::vector<ID3D11SamplerState*>sampler_;
		Texture();
	public:
		void SetSampler_(std::vector<ID3D11SamplerState*>sampler);
		void SetUp(ID3D11DeviceContext * context);
		~Texture();

	};

}

