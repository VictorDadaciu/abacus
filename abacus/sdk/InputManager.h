#pragma once

namespace abc
{
	struct WindowEvents
	{
		bool quit{};
		bool resized{};

		void Clear();
	};

	struct Button
	{
		bool justPressed{};
		bool pressed{};
		bool justReleased{};
	};

	struct KeyboardEvents
	{
		std::unordered_map<SDL_Keycode, Button> keys{};

		void Clear();
	};

	struct MouseEvents
	{
		Button right{};
		Button left{};
		Button middle{};

		float scroll{};

		float x{};
		float y{};
		float dx{};
		float dy{};

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
		MouseEvents mouse{};

	private:
		InputManager();
		static InputManager* st_instance;

		void ClearInputs();
	};

#define INPUT (InputManager::GetInstance())
}

