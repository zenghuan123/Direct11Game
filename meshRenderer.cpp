
#include "meshRenderer.h"


namespace Direct11Game {

	MeshRenderer::MeshRenderer()
	{
	}

	void MeshRenderer::Release()
	{
		for (int i = 0; i < this->vertexBuffer_.size(); i++)
		{
			while (this->vertexBuffer_[i]->Release() > 0);
		}
		while (this->indexBuffer_->Release()>0);
		

	}

	void MeshRenderer::Draw(ID3D11DeviceContext * immediateContext)
	{
		immediateContext->IASetVertexBuffers(0,(UINT) this->vertexBuffer_.size(),&this->vertexBuffer_[0],&this->strides_[0], &this->offsets_[0]);
		immediateContext->IASetIndexBuffer(this->indexBuffer_, DXGI_FORMAT_R16_UINT, 0);
		
		
		immediateContext->DrawIndexed(this->indexNum,0,0);
	}




	MeshRenderer::~MeshRenderer()
	{
		

	}
	
}
