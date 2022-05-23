#pragma once

#include <string>
#include <vector>

#include "Component.h"

namespace abc
{
	class RenderComponent;
	class CameraComponent;
	class TransformComponent;
	class GameObject
	{
	public:
		GameObject(const std::string& name="Untitled");
		~GameObject();

		const uint32_t GetID() const { return m_id; }
		const std::string& GetName() const { return m_name; }

		void AttachComponent(Component* component);

		RenderComponent* GetRenderComponent();
		CameraComponent* GetCameraComponent();
		TransformComponent* GetTransformComponent();

		void Destroy();

	protected:
		std::string m_name{};
		uint32_t m_id{};

		std::vector<Component*> m_components{};

		static uint32_t st_nextId;
	};
}

