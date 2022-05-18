#pragma once

#include "Component.h"

namespace abc
{
	class Transform : public Component
	{
	public:
		Transform(GameObject* gameObject = nullptr);
		virtual ~Transform();

		glm::vec3 pos{};
		glm::vec3 euler{};
		glm::quat rot{};
		glm::vec3 scale{};
	};
}

