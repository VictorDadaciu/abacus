#pragma once

#include "Component.h"

namespace abc
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(GameObject* gameObject, bool primaryCam = false);
		virtual ~CameraComponent();

		virtual void Destroy() override;

		static void SetActiveCamera(CameraComponent* camera);
		static CameraComponent* activeCamera;

		void UpdateView();
		void UpdateProjection();

		glm::mat4 view{};
		glm::mat4 proj{};
		bool primary{};
	};
}

