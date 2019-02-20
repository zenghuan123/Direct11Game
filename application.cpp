#include "application.h"
#include "beginScene.h"

namespace Direct11Game {
	Application* Application::staticInstancePtr_ = new Application();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

			// Note that this tutorial does not handle resizing (WM_SIZE) requests,
			// so we created the window without the resize border.

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}
	Application::Application() :hInstance_(0), hWnd_(0), driverType_(), featureLevel_(),
		d3dDevice_(0), d3dDevice1_(0), immediateContext1(0), immediateContext_(0), swapChain(0), swapChain1(0),
		renderTargetView(0), depthStencilView_(0), depthStencil_(0), scene(0)
	{

	}


	Application::~Application()
	{
		if (d3dDevice1_) 
		{
			d3dDevice1_->Release(); d3dDevice1_ = 0;
		}
		if (d3dDevice_)
		{
			d3dDevice_->Release(); d3dDevice_ = 0;
		}
		if (immediateContext1)
		{
			immediateContext1->Release(); immediateContext1 = 0;
		}
		if (immediateContext_)
		{
			immediateContext_->Release(); immediateContext_ = 0;
		}
		if (swapChain)
		{
			swapChain->Release(); swapChain = 0;
		}
		if (swapChain1)
		{
			swapChain1->Release(); swapChain1 = 0;
		}
		if (renderTargetView)
		{
			renderTargetView->Release(); renderTargetView = 0;
		}
		if (depthStencil_)
		{
			depthStencil_->Release(); depthStencil_ = 0;
		}
		if (depthStencilView_)
		{
			depthStencilView_->Release(); depthStencilView_ = 0;
		}
		if (scene)
		{
			delete scene; scene = 0;
		}
	}

	bool Application::InitializeWindow(HINSTANCE hInstance, int nCmdShow,int height,int width)
	{
		WNDCLASSEX wndClass = { 0 };
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WndProc;
		wndClass.hInstance = hInstance;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = TEXT("DX11Window");
		if (!RegisterClassEx(&wndClass))
			exit(-1);
		this->width_ = width;
		this->height_ = height;
		RECT rc = { 0,0,this->width_,this->height_ };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		this->hInstance_ = hInstance;
		this->hWnd_ = CreateWindow(TEXT("DX11Window"), TEXT("Direct11 Sample"), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
			NULL, NULL, hInstance, NULL);
		if (!hWnd_)
			exit(-1);
		ShowWindow(this->hWnd_, nCmdShow);
		return true;
	}
	bool Application::InitializeDirect3D()
	{
		
		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect(this->hWnd_, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			this->driverType_ = driverTypes[driverTypeIndex];
			hr = D3D11CreateDevice(nullptr, this->driverType_, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &this->d3dDevice_, &this->featureLevel_, &this->immediateContext_);

			if (hr == E_INVALIDARG)
			{
				// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
				hr = D3D11CreateDevice(nullptr, this->driverType_, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
					D3D11_SDK_VERSION, &this->d3dDevice_, &this->featureLevel_, &this->immediateContext_);
			}

			if (SUCCEEDED(hr))
				break;
		}
		if (FAILED(hr))
			return hr;

		// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
		IDXGIFactory1* dxgiFactory = nullptr;
		{
			IDXGIDevice* dxgiDevice = nullptr;
			hr = this->d3dDevice_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
			if (SUCCEEDED(hr))
			{
				IDXGIAdapter* adapter = nullptr;
				hr = dxgiDevice->GetAdapter(&adapter);
				if (SUCCEEDED(hr))
				{
					hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
					adapter->Release();
				}
				dxgiDevice->Release();
			}
		}
		if (FAILED(hr))
			return hr;

		// Create swap chain
		IDXGIFactory2* dxgiFactory2 = nullptr;
		hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
		if (dxgiFactory2)
		{
			// DirectX 11.1 or later
			hr = this->d3dDevice_->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&this->d3dDevice1_));
			if (SUCCEEDED(hr))
			{
				(void)this->immediateContext_->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&this->immediateContext1));
			}

			DXGI_SWAP_CHAIN_DESC1 sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.Width = width;
			sd.Height = height;
			sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.BufferCount = 1;

			hr = dxgiFactory2->CreateSwapChainForHwnd(this->d3dDevice_, this->hWnd_, &sd, nullptr, nullptr, &this->swapChain1);
			if (SUCCEEDED(hr))
			{
				hr = this->swapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&this->swapChain));
			}

			dxgiFactory2->Release();
		}
		else
		{
			// DirectX 11.0 systems
			DXGI_SWAP_CHAIN_DESC sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = 1;
			sd.BufferDesc.Width = width;
			sd.BufferDesc.Height = height;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = this->hWnd_;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = TRUE;

			hr = dxgiFactory->CreateSwapChain(this->d3dDevice_, &sd, &this->swapChain);
		}

		// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
		dxgiFactory->MakeWindowAssociation(this->hWnd_, DXGI_MWA_NO_ALT_ENTER);

		dxgiFactory->Release();

		if (FAILED(hr))
			return hr;

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
		if (FAILED(hr))
			return hr;

		hr = this->d3dDevice_->CreateRenderTargetView(pBackBuffer, nullptr, &this->renderTargetView);
		pBackBuffer->Release();
		if (FAILED(hr))
			return hr;

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = this->d3dDevice_->CreateTexture2D(&descDepth, nullptr, &this->depthStencil_);
		if (FAILED(hr))
			return hr;

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = this->d3dDevice_->CreateDepthStencilView(this->depthStencil_, &descDSV, &this->depthStencilView_);
		if (FAILED(hr))
			return hr;

		this->immediateContext_->OMSetRenderTargets(1, &this->renderTargetView, this->depthStencilView_);

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		this->immediateContext_->RSSetViewports(1, &vp);

		return S_OK;
	}

	void Application::Update()
	{
		static float t = 0.0f;
		static clock_t lastTime = 0;
		clock_t timeCur = clock();
		t = (timeCur - lastTime) / 1000.0f;
		
		/*while (t <1/60)
		{
			timeCur = clock();
			t = (timeCur - lastTime) / 1000.0f;
		}*/
		lastTime = timeCur;
		this->scene->Update(t);
	}

	bool  Application::Initialize(HINSTANCE hInstance, int nCmdShow, int width, int height)
	{

		if (staticInstancePtr_ == nullptr)
		{
			staticInstancePtr_ = new Application();
		}

		if (!staticInstancePtr_->InitializeWindow(hInstance, nCmdShow, height, width))
		{
			MessageBox(0, TEXT("创建窗口失败"), TEXT(__FILE__ + __LINE__), MB_OK);
			return false;
		}
			
		
		if (FAILED(staticInstancePtr_->InitializeDirect3D()))
		{
			MessageBox(0, TEXT("初始化Direct失败"), TEXT(__FILE__+__LINE__), MB_OK);
			return false;
		}
		staticInstancePtr_->immediateContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ResourceManager::Init(staticInstancePtr_->d3dDevice_);

		staticInstancePtr_->scene = new BeginScene();
		staticInstancePtr_->scene->Init(staticInstancePtr_->d3dDevice_,staticInstancePtr_->immediateContext_);
		return true;
	}

	bool Application::Run()
	{
		MSG msg = { 0 };
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			Update();
			Render();
			
		}
		return false;
	}

	void Application::End()
	{
		CloseWindow(staticInstancePtr_->hWnd_);
		delete staticInstancePtr_->scene;
		delete staticInstancePtr_;
	}



	void Application::Render()
	{
		this->immediateContext_->ClearRenderTargetView(this->renderTargetView, Colors::Blue);
		this->immediateContext_->ClearDepthStencilView(this->depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);
		scene->Render(this->immediateContext_);
		this->swapChain->Present(0, 0);
	}

}


