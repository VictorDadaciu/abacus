#include "pch.h"
#include "CameraComponent.h"

#include "GameObject.h"
#include "GraphicsRenderer.h"

namespace abc
{
	CameraComponent* CameraComponent::activeCamera = nullptr;

	CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject, ComponentType::CAMERA)
	{
		if (!activeCamera)
		{
			activeCamera = this;
		}

		view = glm::lookAt(glm::vec3(5.f, 5.f, 5.f), glm::vec3(0.0f, 3.f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		UpdateProjection();
	}

	void CameraComponent::UpdateProjection()
	{
		proj = glm::perspective(glm::radians(45.0f), RENDERER->GetSwapchain().extent.width / (float)RENDERER->GetSwapchain().extent.height, 0.1f, 1000.0f);
		proj[1][1] *= -1;
	}

	CameraComponent::~CameraComponent()
	{
	}

	void CameraComponent::Destroy()
	{
	}
}