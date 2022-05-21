#include "pch.h"
#include "GameObject.h"

#include "RenderComponent.h"

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