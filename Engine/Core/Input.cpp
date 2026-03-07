#include "Input.h"
#include <Windows.h>
#include <iostream>

namespace Wanted
{
	Input* Input::instance = nullptr;

	Input::Input()
	{
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

	Vector2 Input::GetMousePosition() const
	{
		POINT point;
		 if (GetCursorPos(&point))
		 {
			 return Vector2(point.x, point.y);
		 }
		 else
		 {
			 std::cerr << "Failed to get cursor position.\n";
			 return Vector2::Zero;
		 }
	}

	Input& Input::Get()
	{
		if (!instance)
		{
			std::cout << "Error: Input::Get(). instance is null\n";

			__debugbreak();
		}

		return *instance;
	}

	void Input::ProcessInput()
	{
		for (int ix = 0; ix < 255; ++ix)
		{
			keyStates[ix].isKeyDown
				= (GetAsyncKeyState(ix) & 0x8000) > 0 ? true : false;
		}
	}
	
	void Input::SavePreviousInputStates()
	{
		for (int ix = 0; ix < 255; ++ix)
		{
			keyStates[ix].wasKeyDown
				= keyStates[ix].isKeyDown;
		}
	}
}
