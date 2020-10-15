//////////////////////////// Notes ///////////////////////////////////////////
// function with [Ex] - extended version (Newer)
// functions with [A] - multibyte (ANSI) version
// functions with [w] - unicode (Wide) version
//////////////////////////// Notes ///////////////////////////////////////////

#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <windows.h>
#include <dxerr.h>

#pragma region Foward declaration

HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#pragma endregion

int CALLBACK WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	HpervInstance,	// Always null
	LPSTR		lpCmdLine,		// Recive cm line as a string
	int			nCmdShow)		// Defines how windows is shown
{
	UNREFERENCED_PARAMETER(HpervInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(InitialiseWindow(hInstance, nCmdShow)))
	{
		DXTRACE_MSG("Failed to create Windows");
		return 0;
	}

	//Main message loop
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		//Get windows actions
		//PeekMessage - good for 3d aplications, cpu consumming
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//do somthing
		}
	}
	return (int)msg.wParam;
}

HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow)
{
	const auto pClassName = "Mario Fernandes";

	//register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;		//Get msg
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);

	//create window

	//Window size
	RECT rc = { 0, 0, 640, 480 };

	// Windows Title
	char g_TutorialName[100] = "Tutorial 01 Exercice 01\0";

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hWnd = CreateWindowEx(
		0,						//Extended styles
		pClassName,				//Class name
		g_TutorialName,			//Window name
		WS_OVERLAPPEDWINDOW,	//Basic styles
		0, 0,					//Start position of windows
		rc.right - rc.left,		//Resolution
		rc.bottom - rc.top,		//Resolution
		nullptr, nullptr, hInstance, nullptr
	);

	if (!hWnd)
		return E_FAIL;

	ShowWindow(hWnd, nCmdShow);
}

//   Call every time the aplication recives a message
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

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam); //Mantain the normal windows behavior
	}

	return 0;
}