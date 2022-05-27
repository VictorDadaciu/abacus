#include "pch.h"
#include "InputManager.h"

namespace abc
{
	InputManager* InputManager::st_instance = nullptr;

	InputManager::InputManager()
	{
	}

	InputManager* InputManager::GetInstance()
	{
		if (!st_instance)
		{
			st_instance = new InputManager();
		}
		return st_instance;
	}

	void InputManager::Initialise()
	{
		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	void InputManager::ClearInputs()
	{
		window.Clear();
		keyboard.Clear();
		mouse.Clear();
	}

	void InputManager::Update()
	{
		ClearInputs();

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				window.quit = true;
			}
			
			if (e.type == SDL_WINDOWEVENT)
			{
				if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					window.resized = true;
				}
			}

			if (e.type == SDL_KEYDOWN) 
			{
				SDL_Keycode code = e.key.keysym.sym;
				if (code == SDLK_ESCAPE)
				{
					window.quit = true;
				}

				keyboard.keys[code].justPressed = true;
				keyboard.keys[code].pressed = true;
			}

			if (e.type == SDL_KEYUP) 
			{
				SDL_Keycode code = e.key.keysym.sym;

				keyboard.keys[code].justReleased = true;
				keyboard.keys[code].pressed = false;
			}

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					mouse.left.justPressed = true;
					mouse.left.pressed = true;
				}

				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					mouse.right.justPressed = true;
					mouse.right.pressed = true;
				}

				if (e.button.button == SDL_BUTTON_MIDDLE)
				{
					mouse.middle.justPressed = true;
					mouse.middle.pressed = true;
				}
			}

			if (e.type == SDL_MOUSEBUTTONUP)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					mouse.left.justReleased = true;
					mouse.left.pressed = false;
				}

				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					mouse.right.justReleased = true;
					mouse.right.pressed = false;
				}

				if (e.button.button == SDL_BUTTON_MIDDLE)
				{
					mouse.middle.justReleased = true;
					mouse.middle.pressed = false;
				}
			}

			if (e.type == SDL_MOUSEMOTION)
			{
				mouse.x = e.motion.x;
				mouse.y = e.motion.y;
				mouse.dx = e.motion.xrel;
				mouse.dy = e.motion.yrel;
			}
		}
	}

	void WindowEvents::Clear()
	{
		quit = false;
		resized = false;
	}

	void KeyboardEvents::Clear()
	{
		for (auto& key : keys)
		{
			key.second.justPressed = false;
			key.second.justReleased = false;
		}
	}

	void MouseEvents::Clear()
	{
		left.justPressed = false;
		left.justReleased = false;
		right.justPressed = false;
		right.justReleased = false;
		middle.justPressed = false;
		middle.justReleased = false;

		scroll = 0.f;
		dx = 0.f;
		dy = 0.f;
	}

	void InputManager::Destroy()
	{

	}

	InputManager::~InputManager()
	{
	}
}
