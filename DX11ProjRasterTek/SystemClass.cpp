#include "SystemClass.h"

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
	switch (umsg)
	{
	case WM_KEYDOWN:
		_input->KeyDown((unsigned int)wParam);
		return 0;
		break;

	case WM_KEYUP:
		_input->KeyUp((unsigned int)wParam);
		return 0;
		break;

	default:
		return DefWindowProc(hWnd, umsg, wParam, lParam);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT umessage, WPARAM wParam, LPARAM lParam)
{
	switch (umessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;

	default:
		return _appHandle->MessageHandler(hWnd, umessage, wParam, lParam);
	}
}

SystemClass::SystemClass()
{
	_input = 0;
	_graphics = 0;
}

SystemClass::SystemClass(const SystemClass &)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Init()
{
	int screenWidth, screenHeight;
	bool result;

	screenHeight = 0;
	screenWidth = 0;

	InitWindows(screenWidth, screenHeight);

	// Create Input object, to handle keyboard input
	_input = new InputClass();
	if (!_input)
		return false;

	_input->Init();

	//Init graphics object
	_graphics = new GraphicsClass();
	if (!_graphics)
		return false;

	result = _graphics->Init(screenWidth, screenHeight, _hWnd);
	if (!result)
		return false;


	return true;
}

void SystemClass::Shutdown()
{
	if (_graphics)
	{
		_graphics->Shutdown();
		delete _graphics;
		_graphics = 0;
	}

	if (_input)
	{
		delete _input;
		_input = 0;
	}

	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}

bool SystemClass::Frame()
{
	bool result;

	if (_input->isKeyDown(VK_ESCAPE))
	{
		return false;
	}

	result = _graphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::InitWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	_appHandle = this;

	_hInstance = GetModuleHandle(nullptr);

	_appName = L"DX11 Proj";

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = _appName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		screenWidth = 1280;
		screenHeight = 720;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	_hWnd = CreateWindowEx(0, _appName, _appName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, screenWidth, screenHeight, nullptr, nullptr, _hInstance, nullptr);

	ShowWindow(_hWnd, SW_SHOW);
	//SetForegroundWindow(_hWnd);
	//SetFocus(_hWnd);
	UpdateWindow(_hWnd);
	//ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(nullptr, 0);
	}

	DestroyWindow(_hWnd);
	_hWnd = nullptr;

	UnregisterClass(_appName, _hInstance);
	_hInstance = nullptr;

	_appHandle = nullptr;

	return;
}