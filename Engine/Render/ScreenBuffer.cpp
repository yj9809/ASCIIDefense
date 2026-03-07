#include "ScreenBuffer.h"
#include <iostream>
#include "Renderer.h"

namespace Wanted
{
	ScreenBuffer::ScreenBuffer(const Vector2& screenSize)
		: screenSize(screenSize)
	{
		buffer = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr,
			CONSOLE_TEXTMODE_BUFFER,
			nullptr);

		if(buffer == INVALID_HANDLE_VALUE)
		{
			MessageBoxA(nullptr, "ScreenBuffer - Failed to create buffer.", "Buffer creation error", MB_OK);
			__debugbreak();
		}

		SMALL_RECT rect;
		rect.Left = 0;
		rect.Top = 0;
		rect.Right = static_cast<short>(screenSize.x - 1);
		rect.Bottom = static_cast<short>(screenSize.y - 1);

		if (!SetConsoleWindowInfo(buffer, true, &rect))
		{
			//DWORD errorCode = GetLastError();
			std::cerr << "Failed to set console window info.\n";
			__debugbreak();
		}

		if (!SetConsoleScreenBufferSize(buffer, screenSize))
		{
			std::cerr << "Failed to set console buffer size.\n";
			__debugbreak();
		}

		CONSOLE_CURSOR_INFO info;
		GetConsoleCursorInfo(buffer, &info);

		info.bVisible = false;
		SetConsoleCursorInfo(buffer, &info);
	}

	ScreenBuffer::~ScreenBuffer()
	{
		if (buffer)
		{
			CloseHandle(buffer);
		}
	}

	void ScreenBuffer::Clear()
	{
		DWORD writtenCount = 0;

		FillConsoleOutputCharacterA(buffer, ' ', screenSize.x * screenSize.y, Vector2::Zero, &writtenCount);
	}

	void ScreenBuffer::Draw(CHAR_INFO* charInfo)
	{
		SMALL_RECT writeRegion = {};
		writeRegion.Left = 0;
		writeRegion.Top = 0;
		writeRegion.Right = static_cast<short>(screenSize.x - 1);
		writeRegion.Bottom = static_cast<short>(screenSize.y - 1);

		WriteConsoleOutputA(buffer, charInfo, screenSize, Vector2::Zero, &writeRegion);
	}
}