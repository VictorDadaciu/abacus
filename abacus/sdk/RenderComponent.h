#pragma once

#include "Component.h"

namespace abc
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent(GameObject* gameObject, const std::vector<std::string>& meshPaths);
		RenderComponent(GameObject* gameObject, const std::vector<std::string>& meshPaths, const std::vector<std::string>& texturePaths);

		virtual ~RenderComponent();
	};
}

