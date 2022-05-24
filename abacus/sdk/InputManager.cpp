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

	}

	void InputManager::ClearInputs()
	{
		window.Clear();
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

	void InputManager::Destroy()
	{

	}

	InputManager::~InputManager()
	{
	}
}
