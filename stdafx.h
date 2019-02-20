#pragma once
#define NOMINMAX
#include <windows.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <tchar.h>
using namespace DirectX;

#include <WICTextureLoader.h>
#include <d3dx11effect.h>
#ifdef _WIN64

	#pragma comment(lib,"DirectXTK.lib")
	#pragma comment(lib,"Effects11d.lib")
#else
	#ifdef _WIN32
		#pragma comment(lib,"E:/direct11/Direct11AllTutorial/directx-sdk-samples/Effects11/Bin/Desktop_2017_Win10/Win32/Debug/Effects11d.lib")
		#pragma comment(lib,"E:/direct11/Direct11AllTutorial/directx-sdk-samples/DirectXTK/Bin/Desktop_2017_Win10/Win32/Debug/DirectXTK.lib")
	#endif _WIN32

#endif // _WIN64





#include<time.h>
#include <memory>
#include <list>
#include <set>
#include <string>
#include <unordered_map>
using namespace std;


#define KEYDOWN(x)  (x&0x8001)

/*
 µ¯´°ÌáÊ¾³ö´í
*/

#ifdef _DEBUG
#ifndef HR
#define HR(x) {	if (FAILED(x)) MessageBox(0, TEXT(__FILE__),std::to_wstring(__LINE__).c_str(), MB_OK);}
#endif
#else
#ifndef HR
#define HR(x) x;
#endif // !HR
#endif 














