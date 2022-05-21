#include "pch.h"
#include "Component.h"

#include "GameObject.h"

namespace abc
{
	Component::Component(GameObject* go, ComponentType t) :
		gameObject(go), type(t)
	{
		if (!gameObject)
		{
			throw std::runtime_error("Component cannot be unattached!");
		}
	}

	Component::~Component()
	{
	}
}
