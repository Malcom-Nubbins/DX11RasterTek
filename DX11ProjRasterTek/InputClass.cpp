#include "InputClass.h"



InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass &)
{
}


InputClass::~InputClass()
{
}

void InputClass::Init()
{
	for (int i = 0; i < sizeof(_keys); i++)
	{
		_keys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int input)
{
	_keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input)
{
	_keys[input] = false;
	return;
}

bool InputClass::isKeyDown(unsigned int key)
{
	return _keys[key];
}
