#include "pch.h"
#include "GameObject.h"

#include "Component.h"

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

	void GameObject::AttachComponent(Component* component)
	{
		m_components.push_back(component);
	}
}