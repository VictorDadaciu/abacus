#pragma once

namespace abc
{
	struct WindowEvents
	{
		bool quit{};
		bool resized{};

		void Clear();
	};

	class InputManager
	{
	public:
		~InputManager();

		static InputManager* GetInstance();

		void Initialise();
		void Update();
		void Destroy();

		WindowEvents window{};

	private:
		InputManager();
		static InputManager* st_instance;

		void ClearInputs();
	};

#define INPUT (InputManager::GetInstance())
}

