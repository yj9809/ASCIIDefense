#include "Input.h"

#include <Windows.h>
#include <iostream>

namespace Wanted
{
	Input* Input::instance = nullptr;

	Input::Input()
	{
		instance = this;

		HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
		if (hIn != INVALID_HANDLE_VALUE)
		{
			DWORD mode = 0;
			if (GetConsoleMode(hIn, &mode))
			{
				mode |= ENABLE_EXTENDED_FLAGS;   // Quick Edit 제어 가능
				mode |= ENABLE_MOUSE_INPUT;      // 마우스 이벤트 수신
				mode &= ~ENABLE_QUICK_EDIT_MODE; // Quick Edit 끄기
				SetConsoleMode(hIn, mode);
			}
		}
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
		static Vector2 lastPos(-1, -1);

		HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
		if (hIn == INVALID_HANDLE_VALUE)
		{
			return lastPos;
		}

		DWORD eventsCount = 0;
		if (!GetNumberOfConsoleInputEvents(hIn, &eventsCount))
		{
			return lastPos;
		}

		while (eventsCount > 0)
		{
			INPUT_RECORD rec[64];
			DWORD read = 0;
			DWORD toRead = (eventsCount < 64) ? eventsCount : 64;

			if (!ReadConsoleInput(hIn, rec, toRead, &read))
			{
				break;
			}

			for (DWORD i = 0; i < read; ++i)
			{
				if (rec[i].EventType == MOUSE_EVENT)
				{
					const MOUSE_EVENT_RECORD& m = rec[i].Event.MouseEvent;
					lastPos.x = m.dwMousePosition.X;
					lastPos.y = m.dwMousePosition.Y;
				}
			}

			if (!GetNumberOfConsoleInputEvents(hIn, &eventsCount))
			{
				break;
			}
		}

		return lastPos;
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
