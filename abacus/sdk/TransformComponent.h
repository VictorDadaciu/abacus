#pragma once

#include "Component.h"

namespace abc
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent(GameObject* gameObject);
		virtual ~TransformComponent();

		const glm::mat4& GetMat();
		void Rotate(float angle, const glm::vec3& axis);

		virtual void Destroy() override;

		glm::vec3 pos{};
		glm::vec3 euler{};
		glm::quat rot{};
		glm::vec3 scale{};
		glm::mat4 mat{};

	protected:
	};
}

