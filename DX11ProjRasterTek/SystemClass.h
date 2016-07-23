#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "InputClass.h"
#include "GraphicsClass.h"

class SystemClass
{
private:
	bool Frame();
	void InitWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR			_appName;
	HINSTANCE		_hInstance;
	HWND			_hWnd;

	InputClass*		_input;
	GraphicsClass*	_graphics;
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Init();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass*		_appHandle = 0;