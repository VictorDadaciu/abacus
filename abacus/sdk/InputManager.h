#pragma once

namespace abc
{
	struct WindowEvents
	{
		bool quit{};
		bool resized{};

		void Clear();
	};

	struct Keys
	{
		bool justPressed{};
		bool pressed{};
		bool justReleased{};
	};

	struct KeyboardEvents
	{
		std::unordered_map<SDL_Keycode, Keys> keys{};

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
		KeyboardEvents keyboard{};

	private:
		InputManager();
		static InputManager* st_instance;

		void ClearInputs();
	};

#define INPUT (InputManager::GetInstance())
}

