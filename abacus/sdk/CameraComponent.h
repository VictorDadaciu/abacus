#pragma once

#include "Component.h"

namespace abc
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(GameObject* gameObject);
		virtual ~CameraComponent();

		glm::mat4 view{};
		glm::mat4 proj{};
	};
}

