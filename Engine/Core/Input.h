#pragma once

#include "Common/Common.h"
#include "Math/Vector2.h"

namespace Wanted
{
	class WANTED_API Input
	{
		friend class Engine;

		// Down/Up/Key
		struct KeyState
		{
			bool isKeyDown = false;

			// ������ Ű�� ���ȴ���.
			bool wasKeyDown = false;
		};

	private:
		Input();
		~Input();

	public:
		bool GetKeyDown(int keyCode);

		bool GetKeyUp(int keyCode);

		bool GetKey(int keyCode);

		Vector2 GetMousePosition() const;

		static Input& Get();

	private:
		void ProcessInput();

		void SavePreviousInputStates();

	private:
		KeyState keyStates[255] = { };

		Vector2 mousePosition;

		bool isMouseLeftDown = false;
		bool previousMouseLeftDown = false;

		static Input* instance;
	};
}
