#pragma once

#include "Component.h"

namespace abc
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(GameObject* gameObject);
		virtual ~CameraComponent();

		virtual void Destroy() override;

		static void SetActiveCamera(CameraComponent* camera) { activeCamera = camera; }
		static CameraComponent* activeCamera;

		void UpdateProjection();

		glm::mat4 view{};
		glm::mat4 proj{};
	};
}

