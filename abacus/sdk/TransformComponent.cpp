#include "pch.h"
#include "TransformComponent.h"

#include "GameObject.h"

namespace abc
{
	TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject, ComponentType::TRANSFORM)
	{
		pos = glm::vec3();
		rot = glm::quat();
		scale = glm::vec3(1.0f);
	}

	const glm::mat4& TransformComponent::GetMat()
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), pos);
		glm::mat4 rotate = glm::toMat4(rot);
		glm::mat4 scal = glm::scale(glm::mat4(1.0f), scale);
		mat = translate * rotate * scal;

		return mat;
	}

	void TransformComponent::Rotate(float angle, const glm::vec3& axis)
	{
		rot = glm::angleAxis(glm::radians(angle), axis);
	}

	TransformComponent::~TransformComponent()
	{
	}

	void TransformComponent::Destroy()
	{
	}
}