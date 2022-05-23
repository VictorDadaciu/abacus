#include "pch.h"
#include "GameObject.h"

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"

namespace abc
{
	uint32_t GameObject::st_nextId = 0;

	GameObject::GameObject(const std::string& name) :
		m_name(name)
	{
		m_id = st_nextId++;
	}

	GameObject::~GameObject()
	{
	}

	RenderComponent* GameObject::GetRenderComponent()
	{
		for (auto component : m_components)
		{
			if (component->type == ComponentType::RENDER)
			{
				return static_cast<RenderComponent*>(component);
			}
		}
		return nullptr;
	}

	TransformComponent* GameObject::GetTransformComponent()
	{
		for (auto component : m_components)
		{
			if (component->type == ComponentType::TRANSFORM)
			{
				return static_cast<TransformComponent*>(component);
			}
		}
		return nullptr;
	}

	CameraComponent* GameObject::GetCameraComponent()
	{
		for (auto component : m_components)
		{
			if (component->type == ComponentType::CAMERA)
			{
				return static_cast<CameraComponent*>(component);
			}
		}
		return nullptr;
	}

	void GameObject::AttachComponent(Component* component)
	{
		m_components.push_back(component);
	}

	void GameObject::Destroy()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			m_components[i]->Destroy();
			delete m_components[i];
		}
	}
}