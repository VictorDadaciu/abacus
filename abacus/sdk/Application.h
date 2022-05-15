#pragma once

#include <string>

class SDL_Window;

namespace abc
{
	class Renderer;
	class Window
	{
	public:
		Window(const std::string& name="abacus", int width=800, int height=600);

		void Close();

		SDL_Window* Raw() const { return m_window; }
		const std::string& GetName() const { return m_name; }
		const int GetWidth() const { return m_width; }
		const int GetHeight() const { return m_height; }

	protected:
		SDL_Window* m_window;

		std::string m_name{};
		int m_width{};
		int m_height{};
	};

	class Application
	{
	public:
		static Application* GetInstance();

		void Start();
		void Update();
		const bool IsRunning() const { return m_running; };
		void Quit();

		Window* GetActiveWindow() const { return m_activeWindow; }
	protected:
		static Application* st_instance;
		Renderer* m_renderer;

		Window* m_activeWindow;
		bool m_running = false;
	};

#define APP (Application::GetInstance())
}

