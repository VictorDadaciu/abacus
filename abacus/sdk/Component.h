#pragma once

namespace abc
{
	enum class ComponentType
	{
		CUSTOM = 0x0001,
		TRANSFORM = 0x0002,
		RENDER = 0x0004,
	};

	class GameObject;
	class Component
	{
	public:
		Component(GameObject* gameObject = nullptr, ComponentType type = ComponentType::CUSTOM);
		virtual ~Component();

		GameObject* gameObject{};
		ComponentType type{};
	};
}

