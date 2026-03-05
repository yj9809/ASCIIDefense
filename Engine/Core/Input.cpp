#include "Input.h"
#include <Windows.h>
#include <iostream>

// Ctrl + HomeŰ�� ���� ���� ���� �̵� ����.
// Rider�� ��� �ڵ� �߰� ����.
namespace Wanted
{
	// ���� ���� �ʱ�ȭ.
	Input* Input::instance = nullptr;

	Input::Input()
	{
		// ��ü�� �ʱ�ȭ�Ǹ� �ڱ� �ڽ��� �ּҸ� ����.
		instance = this;
	}
	
	Input::~Input()
	{
	}

	bool Input::GetKeyDown(int keyCode)
	{
		return keyStates[keyCode].isKeyDown
			&& !keyStates[keyCode].wasKeyDown;
	}

	bool Input::GetKeyUp(int keyCode)
	{
		return !keyStates[keyCode].isKeyDown
			&& keyStates[keyCode].wasKeyDown;
	}

	bool Input::GetKey(int keyCode)
	{
		return keyStates[keyCode].isKeyDown;
	}

	Input& Input::Get()
	{
		// �̱���(Singleton).
		// �� �Լ��� ������ ������Ʈ���� ������.
		// ���� ������ �̹� �ʱ�ȭ �Ϸ� ����.
		if (!instance)
		{
			//return *nullptr;
			std::cout << "Error: Input::Get(). instance is null\n";

			// ����� ��忡���� ������.
			// �ڵ����� �ߴ��� �ɸ�.
			__debugbreak();
		}

		// Lazy-Pattern.
		// ����Ƽ�� C++�� ����.
		//static Input instance;
		return *instance;
	}

	void Input::ProcessInput()
	{
		// Ű ������ �Է� �б�.
		// !!! �ü���� �����ϴ� ����� ����� �� �ۿ� ����.
		for (int ix = 0; ix < 255; ++ix)
		{
			keyStates[ix].isKeyDown
				= (GetAsyncKeyState(ix) & 0x8000) > 0 ? true : false;
		}
	}
	
	void Input::SavePreviousInputStates()
	{
		// ���� �Է� ���� ���� �Է� ������ ����.
		for (int ix = 0; ix < 255; ++ix)
		{
			keyStates[ix].wasKeyDown
				= keyStates[ix].isKeyDown;
		}
	}
}
