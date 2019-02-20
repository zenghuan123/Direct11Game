#pragma once
#include "stdafx.h"
#include "scene.h"
#include "resourceManager.h"
namespace Direct11Game {
	
	/*
	��ʼ�����ں�direct11 ����
	�ṩdirect11 �������ݵķ���
	�����ǳ������е����
	*/
	class Application
	{

	public:		
		/*����ģʽ*/
		static Application* GetInstance() { return staticInstancePtr_; };
		/*��ʼ��*/
		bool static Initialize(HINSTANCE hInstance, int nCmdShow, int height, int width);
		
		inline int GetWidth() { return width_; }
		inline int GetHeight() { return height_; }
		/*����*/
		bool Run();
		/*
		�ͷ���Դ
		*/
		void End();

	private:
		static Application*staticInstancePtr_;


		int width_;
		int height_;

		HINSTANCE hInstance_;
		HWND hWnd_;
		D3D_DRIVER_TYPE driverType_;
		D3D_FEATURE_LEVEL  featureLevel_;

		
		ID3D11Device* d3dDevice_;
		ID3D11Device1* d3dDevice1_;
		ID3D11DeviceContext* immediateContext_;
		ID3D11DeviceContext1* immediateContext1;
		IDXGISwapChain* swapChain;
		IDXGISwapChain1* swapChain1;
		ID3D11RenderTargetView* renderTargetView;
		ID3D11Texture2D*        depthStencil_ ;
		ID3D11DepthStencilView* depthStencilView_;
		Scene* scene;


		Application();
		virtual ~Application();

		
		bool InitializeWindow(HINSTANCE hInstance, int nCmdShow, int height, int width);
		bool InitializeDirect3D();
		void Update();
		void Render();


	};

}


