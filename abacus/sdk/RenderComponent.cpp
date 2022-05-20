#include "pch.h"
#include "RenderComponent.h"

namespace abc
{
	RenderComponent::RenderComponent(GameObject* gameObject, const std::vector<std::string>& meshPaths) : Component(gameObject, ComponentType::RENDER)
	{
	}

	RenderComponent::RenderComponent(GameObject* gameObject, const std::vector<std::string>& meshPaths, const std::vector<std::string>& texturePaths) : Component(gameObject, ComponentType::RENDER)
	{
	}

	RenderComponent::~RenderComponent()
	{
	}
}