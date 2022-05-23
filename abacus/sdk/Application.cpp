#include "pch.h"
#include "Application.h"

#include "GraphicsRenderer.h"
#include "InputManager.h"

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

    void Application::Initialise()
    {
        m_activeWindow = new Window();

        m_running = true;
        INPUT->Initialise();
        RENDERER->Initialise();
    }

    Application::Application()
    {
    }

    void Application::HandleInputSystem()
    {
        INPUT->Update();

        m_running = !INPUT->window.quit;
        if (INPUT->window.resized)
        {
            RENDERER->WindowResized();
        }
    }

    void Application::Update()
    {
        HandleInputSystem();

        RENDERER->DrawFrame();
    }

    void Application::Quit()
    {
        RENDERER->WaitForDeviceToIdle();
        RENDERER->Destroy();
        INPUT->Destroy();

        m_activeWindow->Close();
        delete m_activeWindow;

        delete st_instance;
    }
}