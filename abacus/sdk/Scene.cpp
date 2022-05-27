#include "pch.h"
#include "Scene.h"

#include "Shader.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "RenderComponent.h"
#include "InputManager.h"

#define YAML_CPP_STATIC_DEFINE
#include "yaml-cpp/yaml.h"

namespace abc
{
	Scene::Scene(const std::string& path)
	{
		m_shaders.push_back(new Shader("pbr", "shd/triangle.vert", "shd/triangle.frag"));

		YAML::Node config = YAML::LoadFile(path);
		if (config["sceneName"])
		{
			m_name = config["sceneName"].as<std::string>();
			std::cout << "Deserialising scene " << m_name << "...\n";
		}
		else
		{
			throw std::runtime_error("Scene config has no scene name!");
		}

		auto gameObjects = config["gameObjects"];
		if (gameObjects)
		{
			for (auto obj : gameObjects)
			{
				GameObject* go = new GameObject(obj["name"].as<std::string>());
				std::cout << "	Deserialising game object " << go->GetName() << "...\n";

				auto transform = obj["transform"];
				if (transform)
				{
					std::cout << "		Deserialising transform component...\n";
					glm::vec3 pos = glm::vec3();
					glm::vec3 rot = glm::vec3();
					glm::vec3 scale = glm::vec3(1.f);
					{
						auto position = transform["pos"];
						if (position)
						{
							pos = glm::vec3(position[0].as<float>(), position[1].as<float>(), position[2].as<float>());
						}
					}
					{
						auto rotation = transform["rot"];
						if (rotation)
						{
							rot = glm::vec3(glm::radians(rotation[0].as<float>()), glm::radians(rotation[1].as<float>()), glm::radians(rotation[2].as<float>()));
						}
					}
					{
						auto scal = transform["scale"];
						if (scal)
						{
							scale = glm::vec3(scal[0].as<float>(), scal[1].as<float>(), scal[2].as<float>());
						}
					}

					TransformComponent* transformComponent = new TransformComponent(go);
					transformComponent->pos = pos;
					transformComponent->rot = rot;
					transformComponent->scale = scale;
					go->AttachComponent(transformComponent);
				}

				auto camera = obj["camera"];
				if (camera)
				{
					std::cout << "		Deserialising camera component...\n";
					bool primary = false;
					{
						auto primaryCam = camera["primary"];
						if (primaryCam)
						{
							primary = primaryCam.as<bool>();
						}
					}

					CameraComponent* cameraComponent = new CameraComponent(go, primary);
					go->AttachComponent(cameraComponent);
					m_cameraObjects.push_back(go);
				}

				auto render = obj["render"];
				if (render)
				{
					std::cout << "		Deserialising render component...\n";
					std::string shader = "pbr";
					std::string modelPath = "res/mdl/column.obj";
					std::string texPath = "res/img/default.png";
					{
						auto shaderName = render["shader"];
						if (shaderName)
						{
							shader = shaderName.as<std::string>();
						}
					}
					{
						auto model = render["model"];
						if (model)
						{
							modelPath = model.as<std::string>();
						}
					}
					{
						auto texture = render["tex"];
						if (texture)
						{
							texPath = texture.as<std::string>();
						}
					}
					Shader* shaderObj = FindShader(shader);
					RenderComponent* renderComponent = new RenderComponent(go, modelPath, texPath, shaderObj);
					go->AttachComponent(renderComponent);
					shaderObj->AddGameObject(go);
					m_renderObjects.push_back(go);
				}

				auto parent = obj["parent"];
				if (parent)
				{
					std::cout << "		Deserialising parent...\n";
					{
						std::string parentName = parent.as<std::string>();
						if (parentName == "none")
						{
							m_rootObjects.push_back(go);
						}
						else
						{
							go->SetParent(FindGameObject(parentName));
						}
					}
				}

				m_allObjects.push_back(go);
			}
		}

		for (auto root : m_rootObjects)
		{
			root->GetTransformComponent()->Init();
		}
	}

	void Scene::Draw(VkCommandBuffer commandBuffer, uint32_t imageIndex)
	{
		static float angleY = 0.f;
		static auto lastFrame = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastFrame).count();
		lastFrame = currentTime;

		float right = INPUT->keyboard.keys[SDLK_a].pressed - INPUT->keyboard.keys[SDLK_d].pressed; 
		float forward = INPUT->keyboard.keys[SDLK_w].pressed - INPUT->keyboard.keys[SDLK_s].pressed;
		float up = INPUT->keyboard.keys[SDLK_SPACE].pressed - INPUT->keyboard.keys[SDLK_LCTRL].pressed;

		TransformComponent* camParent = CameraComponent::activeCamera->gameObject->GetParent()->GetTransformComponent();
		camParent->Rotate(glm::vec3(0, glm::radians(150.f) * time * (-INPUT->mouse.dx), 0));

		TransformComponent* camTransform = CameraComponent::activeCamera->gameObject->GetTransformComponent();
		camTransform->Rotate(glm::vec3(glm::radians(180.f) * time * (INPUT->mouse.dy), 0, 0));

		camParent->Translate(5.0f * time * (forward * camParent->GetForward() + right * camParent->GetRight() + up * camParent->GetUp()));

		CameraComponent::activeCamera->UpdateView();
		CameraComponent::activeCamera->UpdateProjection();

		for (auto root : m_rootObjects)
		{
			if (root->GetName() == "statue_parent")
			{
				root->GetTransformComponent()->Rotate(glm::vec3(0, glm::radians(90.f) * time * (INPUT->keyboard.keys[SDLK_q].pressed - INPUT->keyboard.keys[SDLK_e].pressed), 0));
			}

			root->GetTransformComponent()->UpdateMats();
		}

		for (int i = 0; i < m_shaders.size(); i++)
		{
			m_shaders[i]->RecordCommandBuffer(commandBuffer, imageIndex);
		}
	}

	Shader* Scene::FindShader(const std::string& name)
	{
		for (int i = 0; i < m_shaders.size(); i++)
		{
			if (m_shaders[i]->GetName() == name)
			{
				return m_shaders[i];
			}
		}
		return nullptr;
	}

	GameObject* Scene::FindGameObject(const std::string& name)
	{
		for (int i = 0; i < m_allObjects.size(); i++)
		{
			if (m_allObjects[i]->GetName() == name)
			{
				return m_allObjects[i];
			}
		}
		return nullptr;
	}

	Scene::~Scene()
	{
	}

	void Scene::SwapchainCleanup()
	{
		for (int i = 0; i < m_shaders.size(); i++)
		{
			m_shaders[i]->SwapchainCleanup();
		}
	}

	void Scene::SwapchainRecreation()
	{
		for (int i = 0; i < m_shaders.size(); i++)
		{
			m_shaders[i]->SwapchainRecreation();
		}
	}

	void Scene::Destroy()
	{
		for (int i = 0; i < m_allObjects.size(); i++)
		{
			m_allObjects[i]->Destroy();
			delete m_allObjects[i];
		}

		for (int i = 0; i < m_shaders.size(); i++)
		{
			m_shaders[i]->Destroy();
			delete m_shaders[i];
		}
	}
}
