#include "pch.h"
#include "Application.h"

#include "Renderer.h"

namespace abc
{
	Window::Window(const std::string& name, int width, int height) :
		m_name("abacus"), m_width(width), m_height(height)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
        m_window = SDL_CreateWindow(
            m_name.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_width,
            m_height,
            SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );
	}

    void Window::Close()
    {
        SDL_DestroyWindow(m_window);
        SDL_Quit();

        m_window = NULL;
    }

    //app
    Application* Application::st_instance = nullptr;

    Application* Application::GetInstance()
    {
        if (!st_instance)
        {
            st_instance = new Application();
        }
        return st_instance;
    }

    Application::Application()
    {
        m_activeWindow = new Window();

        m_running = true;
    }

    void Application::Update()
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
            {
                m_running = false;
            }
            else if (e.type == SDL_WINDOWEVENT)
            {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    RENDERER->WindowResized();
                }
            }
        }
        RENDERER->DrawFrame();
    }

    void Application::Quit()
    {
        RENDERER->WaitForDeviceToIdle();
        RENDERER->Destroy();

        m_activeWindow->Close();
        delete m_activeWindow;

        delete st_instance;
    }
}