#pragma once
#include "stdafx.h"
#include "scene.h"
#include "resourceManager.h"
namespace Direct11Game {
	
	/*
	初始化窗口和direct11 环境
	提供direct11 各种数据的访问
	并且是程序运行的入口
	*/
	class Application
	{

	public:		
		/*单例模式*/
		static Application* GetInstance() { return staticInstancePtr_; };
		/*初始化*/
		bool static Initialize(HINSTANCE hInstance, int nCmdShow, int height, int width);
		
		inline int GetWidth() { return width_; }
		inline int GetHeight() { return height_; }
		/*运行*/
		bool Run();
		/*
		释放资源
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


