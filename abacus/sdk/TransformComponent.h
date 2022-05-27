#pragma once

#include "Component.h"

namespace abc
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent(GameObject* gameObject);
		virtual ~TransformComponent();

		void Init();
		void Translate(const glm::vec3& translate);
		void Rotate(const glm::vec3& rotate);
		const glm::vec3& GetForward();
		const glm::vec3& GetUp();
		const glm::vec3& GetRight();
		void UpdateMats();

		virtual void Destroy() override;

		glm::vec3 worldPos{};
		glm::vec3 worldRot{};
		glm::vec3 worldScale{};
		glm::vec3 pos{};
		glm::vec3 rot{};
		glm::vec3 scale{};
		glm::mat4 mat{};

		const static glm::vec3 fowardAxis;
		const static glm::vec3 upAxis;
		const static glm::vec3 rightAxis;

	protected:
		glm::vec3 forward = glm::vec3(0, 0, 1);
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = glm::vec3(1, 0, 0);

		void TranslateWorld(const glm::vec3& translate);
		void RotateWorld(const glm::vec3& rotate);
	};
}

