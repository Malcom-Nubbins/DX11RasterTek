#pragma once
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Init();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool isKeyDown(unsigned int);

private:
	bool _keys[256];
};

