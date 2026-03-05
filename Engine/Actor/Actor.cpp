#include "Actor.h"
#include "Util/Util.h"
#include "Render/Renderer.h"
#include "Engine/Engine.h"

#include <iostream>
#include <Windows.h>

namespace Wanted
{
	Actor::Actor(
		const char* image,
		const Vector2& position,
		Color color)
		: position(position), color(color)
	{
		// ���ڿ� ����.
		int length = static_cast<int>(strlen(image));
		for (int p = 0; p < length; ++p)
		{
			if (image[p] == '\n')
			{
				width = 0;
				height++;
				continue;
			}
			width++;
		}
		this->image = new char[length + 1];
		strcpy_s(this->image, length + 1, image);
	}

	Actor::~Actor()
	{
		// �޸� ����.
		SafeDeleteArray(image);
	}

	void Actor::BeginPlay()
	{
		// �̺�Ʈ�� ���� �Ŀ��� �÷��� ����.
		hasBeganPlay = true;
	}

	void Actor::Tick(float deltaTime)
	{
	}

	void Actor::Draw()
	{
		//Renderer::Draw(position, color, image);

		// �������� ������ ����.
		Renderer::Get().Submit(image, position, color, sortingOrder);
	}

	void Actor::Destroy()
	{
		// ���� �÷��� ����.
		destroyRequested = true;

		// ���� �̺�Ʈ ȣ��.
		OnDestroy();
	}

	void Actor::OnDestroy()
	{

	}

	void Actor::QuitGame()
	{
		// ���� ���� ��û.
		Engine::Get().QuitEngine();
	}

	bool Actor::TestIntersect(const Actor* const other)
	{
		const int xMin = position.x;
		const int xMax = position.x + width - 1;

		const int yMin = position.y;
		const int yMax = position.y + height - 1;

		const int otherXMin = other->position.x;
		const int otherXMax = other->position.x + other->width - 1;

		const int otherYMin = other->position.y;
		const int otherYMax = other->position.y + other->height - 1;

		// ������ �����ʿ� �ִ� ���.
		if (otherXMin > xMax) return false;
		// ������ ���ʿ� �ִ� ���.
		if (otherXMax < xMin) return false;
		// ������ �Ʒ��� �ִ� ���.
		if (otherYMin > yMax) return false;
		// ������ ���� �ִ� ���.
		if (otherYMax < yMin) return false;

		return true;
	}

	void Actor::SetPosition(const Vector2& newPosition)
	{
		// �������� ��ĭ �׸��� ��û.
		//Renderer::Draw(position, ' ');

		// �����Ϸ��� ��ġ�� ���� ��ġ�� ������ �ǳʶ�.
		if (position == newPosition)
		{
			return;
		}

		// ���ο� ��ġ ����.
		position = newPosition;
	}
}