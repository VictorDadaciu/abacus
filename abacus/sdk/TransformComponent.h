#pragma once

#include "Component.h"

namespace abc
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent(GameObject* gameObject);
		virtual ~TransformComponent();

		glm::vec3 pos{};
		glm::vec3 euler{};
		glm::quat rot{};
		glm::vec3 scale{};
		glm::mat4 mat{};
	};
}

