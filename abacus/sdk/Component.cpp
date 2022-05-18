#include "pch.h"
#include "Component.h"

#include "GameObject.h"

namespace abc
{
	Component::Component(GameObject* gameObject, ComponentType type) :
		m_gameObject(gameObject), m_type(type)
	{
		if (!m_gameObject)
		{
			throw std::runtime_error("Component cannot be unattached!");
		}
		m_gameObject->AttachComponent(this);
	}

	Component::~Component()
	{
	}
}
