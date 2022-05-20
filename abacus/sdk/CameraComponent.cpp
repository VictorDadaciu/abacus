#include "pch.h"
#include "CameraComponent.h"

#include "GameObject.h"

namespace abc
{
	CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject, ComponentType::CAMERA)
	{
	}

	CameraComponent::~CameraComponent()
	{
	}
}