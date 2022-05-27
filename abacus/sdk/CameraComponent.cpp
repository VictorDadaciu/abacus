#include "pch.h"
#include "CameraComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "GraphicsRenderer.h"

namespace abc
{
	CameraComponent* CameraComponent::activeCamera = nullptr;

	CameraComponent::CameraComponent(GameObject* gameObject, bool primaryCam) : Component(gameObject, ComponentType::CAMERA), primary(primaryCam)
	{
		if (primary)
		{
			SetActiveCamera(this);
		}
	}

	void CameraComponent::SetActiveCamera(CameraComponent* camera) 
	{ 
		if (activeCamera)
			activeCamera->primary = false; 
		activeCamera = camera; 
		activeCamera->primary = true; 
	}

	void CameraComponent::UpdateView()
	{
		TransformComponent* transform = gameObject->GetTransformComponent();
		view = glm::lookAt(transform->worldPos, transform->worldPos + transform->GetForward(), TransformComponent::upAxis);
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