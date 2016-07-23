#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR pScmdline, int iCmdShow)
{
	SystemClass* System;
	bool result;

	System = new SystemClass();
	if (!System)
	{
		return 0;
	}
	
	result = System->Init();
	if (result)
	{
		System->Run();
	}

	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}