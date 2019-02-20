#include "shader.h"


namespace Direct11Game {


	Shader::Shader()
	{
	}

	void Shader::SetUp(ID3D11DeviceContext * context)
	{
		context->VSSetShader(this->vertexShader_, nullptr, 0);
		context->PSSetShader(this->pixelShader, nullptr, 0);
		context->IASetInputLayout(this->inputLayout_);
	}


	Shader::~Shader()
	{
	}

	
}
