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
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					window.quit = true;
				}
			}
		}
	}

	void WindowEvents::Clear()
	{
		quit = false;
		resized = false;
	}

	void InputManager::Destroy()
	{

	}

	InputManager::~InputManager()
	{
	}
}
