#pragma once

namespace abc
{
	class Shader;
	class GameObject;
	class Scene
	{
	public:
		Scene(const std::string& path);
		~Scene();

		void SwapchainCleanup();
		void SwapchainRecreation();

		void Draw(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		Shader* FindShader(const std::string& name);
		GameObject* FindGameObject(const std::string& name);

		void Destroy();

	private:
		std::string m_name = "Scene";

		std::vector<Shader*> m_shaders{};

		std::vector<GameObject*> m_allObjects{};
		std::vector<GameObject*> m_rootObjects{};
		std::vector<GameObject*> m_renderObjects{};
		std::vector<GameObject*> m_cameraObjects{}; 
		std::vector<GameObject*> m_lightObjects{};
	};
}

