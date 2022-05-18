#include "pch.h"
#include "Transform.h"

#include "GameObject.h"

namespace abc
{
	Transform::Transform(GameObject* go) : Component(go, ComponentType::TRANSFORM)
	{
	}

	Transform::~Transform()
	{
	}
}