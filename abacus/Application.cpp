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
            SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN
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

    void Application::Start()
    {
        m_activeWindow = new Window();
        m_renderer = new Renderer(this);

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
        }
    }

    void Application::Quit()
    {
        m_renderer->Destroy();
        delete m_renderer;

        m_activeWindow->Close();
        delete m_activeWindow;

        delete st_instance;
    }
}