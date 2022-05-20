#include "pch.h"
#include "TransformComponent.h"

#include "GameObject.h"

namespace abc
{
	TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject, ComponentType::TRANSFORM)
	{
	}

	TransformComponent::~TransformComponent()
	{
	}
}