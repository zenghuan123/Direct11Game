#include"stdafx.h"
#include "Texture.h"
#include "shader.h"
#include "transform.h"
#include "camera.h"
#include "meshRenderer.h"
#include "resourceManager.h"
#include "gameObject.h"
#include "tyre.h"
#include "vehicle.h"
#include "scene.h"
#include "application.h"

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int cmdShow)
{

	Direct11Game::Application::Initialize(hInstance, cmdShow, 1200, 800);
	Direct11Game::Application::GetInstance()->Run();
	Direct11Game::Application::GetInstance()->End();

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

