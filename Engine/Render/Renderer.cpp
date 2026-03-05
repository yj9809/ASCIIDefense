#include "Renderer.h"
#include "ScreenBuffer.h"
#include "Util/Util.h"

namespace Wanted
{
	Renderer::Frame::Frame(int bufferCount)
	{
		charInfoArray = new CHAR_INFO[bufferCount];
		memset(charInfoArray, 0, sizeof(CHAR_INFO) * bufferCount);

		sortingOrderArray = new int[bufferCount];
		memset(sortingOrderArray, 0, sizeof(int) * bufferCount);
	}

	Renderer::Frame::~Frame()
	{
		SafeDeleteArray(charInfoArray);
		SafeDeleteArray(sortingOrderArray);
	}

	void Renderer::Frame::Clear(const Vector2& screenSize)
	{
		const int width = screenSize.x;
		const int height = screenSize.y;

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				// �迭 �ε��� ���ϱ�.
				const int index = (y * width) + x;

				// ���� �� �� �Ӽ� ����.
				CHAR_INFO& info = charInfoArray[index];
				info.Char.AsciiChar = ' ';
				info.Attributes = 0;

				// �׸��� �켱���� �ʱ�ȭ.
				sortingOrderArray[index] = -1;
			}
		}
	}

	// -------------------- Frame -------------------- //
	Renderer* Renderer::instance = nullptr;

	Renderer::Renderer(const Vector2& screenSize)
		: screenSize(screenSize)
	{
		instance = this;

		// ������ ��ü ����.
		const int bufferCount = screenSize.x * screenSize.y;
		frame = new Frame(bufferCount);

		// ������ �ʱ�ȭ.
		frame->Clear(screenSize);

		// ���� ���� ��ü ����.
		screenBuffers[0] = new ScreenBuffer(screenSize);
		screenBuffers[0]->Clear();

		screenBuffers[1] = new ScreenBuffer(screenSize);
		screenBuffers[1]->Clear();

		// Ȱ��ȭ ���� ����.
		Present();
	}

	Renderer::~Renderer()
	{
		SafeDelete(frame);
		for (ScreenBuffer*& buffer : screenBuffers)
		{
			SafeDelete(buffer);
		}
	}

	void Renderer::Draw()
	{
		// ȭ�� �����.
		Clear();

		// ��������: ������ ��� ���Ͱ� �������� Submit�� �Ϸ��� ����.
		// ���� ť ��ȸ�ϸ鼭 ������ ä���.
		for (const RenderCommand& command : renderQueue)
		{
			RenderLoop(command);
		}

		// �׸���.
		GetCurrentBuffer()->Draw(frame->charInfoArray);

		// ���� ��ȯ.
		Present();

		// ���� ť ����.
		renderQueue.clear();
		uiQueue.clear();
	}

	Renderer& Renderer::Get()
	{
		if (!instance)
		{
			MessageBoxA(nullptr, "Renderer::Get() - instance is null.\n", "Error", MB_OK);
			__debugbreak();
		}

		return *instance;
	}

	void Renderer::RenderLoop(const RenderCommand& command)
	{
		// ȭ�鿡 �׸� �ؽ�Ʈ�� ������ �ǳʶ�.
		if (!command.text)
		{
			return;
		}

		int localX = 0;
		int localY = 0;

		for (const char* p = command.text; *p != '\0'; ++p)
		{
			const char ch = *p;

			if (ch == '\n')
			{
				localY++;
				localX = 0;
				continue;
			}

			if (ch == ' ')
			{
				localX++;
				continue;
			}

			const int drawX = command.position.x + localX;
			const int drawY = command.position.y + localY;

			localX++;

			if (drawX < 0 || drawX >= screenSize.x ||
				drawY < 0 || drawY >= screenSize.y)
			{
				continue;
			}

			const int index = (drawY * screenSize.x) + drawX;

			// �׸��� �켱���� ��.
			if (frame->sortingOrderArray[index] > command.sortingOrder)
			{
				continue;
			}

			// ������ ���.
			frame->charInfoArray[index].Char.AsciiChar = ch;
			frame->charInfoArray[index].Attributes = (WORD)command.color;

			// �켱���� ������Ʈ.
			frame->sortingOrderArray[index] = command.sortingOrder;
		}
	}

	void Renderer::Clear()
	{
		// ȭ�� �����.
		// 1. ������(2���� �迭 ������) �����.
		frame->Clear(screenSize);

		// 2. �ܼ� ���� �����.
		GetCurrentBuffer()->Clear();
	}

	void Renderer::Submit(const char* text, const Vector2& position, Color color, int sortingOrder)
	{
		// ���� ������ ���� �� ť�� �߰�.
		RenderCommand command = {};
		command.text = text;
		command.position = position;
		command.color = color;
		command.sortingOrder = sortingOrder;

		renderQueue.emplace_back(command);
	}

	void Renderer::Present()
	{
		// ���� ��ȯ.
		SetConsoleActiveScreenBuffer(GetCurrentBuffer()->GetBuffer());

		// �ε��� ��ü.
		currentBufferIndex = 1 - currentBufferIndex;
	}

	ScreenBuffer* Renderer::GetCurrentBuffer()
	{
		return screenBuffers[currentBufferIndex];
	}
}
