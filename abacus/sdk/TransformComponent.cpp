#include "pch.h"
#include "TransformComponent.h"

#include "GameObject.h"

namespace abc
{
	const glm::vec3 TransformComponent::fowardAxis = glm::vec3(0, 0, 1);
	const glm::vec3 TransformComponent::upAxis = glm::vec3(0, 1, 0);
	const glm::vec3 TransformComponent::rightAxis = glm::vec3(1, 0, 0);

	TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject, ComponentType::TRANSFORM)
	{
		pos = glm::vec3();
		rot = glm::vec3();
		scale = glm::vec3(1.0f);
	}

	void TransformComponent::UpdateMats()
	{
		glm::mat4 parentMat = glm::mat4(1.0f);
		if (gameObject->GetParent())
		{
			parentMat = gameObject->GetParent()->GetTransformComponent()->mat;
		}

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), worldPos);
		glm::mat4 rotate = glm::yawPitchRoll(worldRot.y, worldRot.x, worldRot.z);
		glm::mat4 scal = glm::scale(glm::mat4(1.0f), scale);
		mat = parentMat * translate * rotate * scal;

		forward = glm::normalize(glm::vec3(rotate * glm::vec4(fowardAxis, 1.0)));
		up = glm::normalize(glm::vec3(rotate * glm::vec4(upAxis, 1.0)));
		right = glm::normalize(glm::vec3(rotate * glm::vec4(rightAxis, 1.0)));

		for (auto child : gameObject->GetChildren())
		{
			child->GetTransformComponent()->UpdateMats();
		}
	}

	void TransformComponent::Init()
	{
		worldPos = pos;
		worldRot = rot;
		if (GameObject* parent = gameObject->GetParent())
		{
			worldPos += parent->GetTransformComponent()->worldPos;
			worldRot += parent->GetTransformComponent()->worldRot;
		}

		for (auto child : gameObject->GetChildren())
		{
			child->GetTransformComponent()->Init();
		}
	}

	void TransformComponent::Translate(const glm::vec3& translate)
	{
		pos += translate;
		TranslateWorld(translate);
	}

	void TransformComponent::TranslateWorld(const glm::vec3& translate)
	{
		worldPos += translate;
		for (auto child : gameObject->GetChildren())
		{
			child->GetTransformComponent()->TranslateWorld(translate);
		}
	}

	void TransformComponent::Rotate(const glm::vec3& rotate)
	{
		rot += rotate;
		RotateWorld(rotate);
	}

	void TransformComponent::RotateWorld(const glm::vec3& rotate)
	{
		worldRot += rotate;
		for (auto child : gameObject->GetChildren())
		{
			child->GetTransformComponent()->RotateWorld(rotate);
		}
	}

	const glm::vec3& TransformComponent::GetForward()
	{
		forward = glm::normalize(glm::vec3(glm::yawPitchRoll(worldRot.y, worldRot.x, worldRot.z) * glm::vec4(fowardAxis, 1.0)));
		return forward;
	}

	const glm::vec3& TransformComponent::GetUp()
	{
		up = glm::normalize(glm::vec3(glm::yawPitchRoll(worldRot.y, worldRot.x, worldRot.z) * glm::vec4(upAxis, 1.0)));
		return up;
	}

	const glm::vec3& TransformComponent::GetRight()
	{
		right = glm::normalize(glm::vec3(glm::yawPitchRoll(worldRot.y, worldRot.x, worldRot.z) * glm::vec4(rightAxis, 1.0)));
		return right;
	}

	TransformComponent::~TransformComponent()
	{
	}

	void TransformComponent::Destroy()
	{
	}
}