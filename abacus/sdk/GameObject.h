#pragma once

#include <string>
#include <vector>

namespace abc
{
	class Component;
	class GameObject
	{
	public:
		GameObject(const std::string& name="Untitled");
		~GameObject();

		const uint32_t GetID() const { return m_id; }
		const std::string& GetName() const { return m_name; }

		void AttachComponent(Component* component);

	protected:
		std::string m_name{};
		uint32_t m_id{};

		std::vector<Component*> m_components{};

		static uint32_t st_nextId;
	};
}
