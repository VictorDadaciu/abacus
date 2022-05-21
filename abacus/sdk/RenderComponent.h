#pragma once

#include "Component.h"

namespace abc
{
	class Texture;
	class Model;
	class RenderComponent : public Component
	{
	public:
		RenderComponent(GameObject* gameObject, const std::string& meshPath, const std::string& texturePath);

		const Model* GetModel() const { return m_model; }
		const Texture* GetTexture() const { return m_tex; }

		virtual void Destroy() override;

		virtual ~RenderComponent();

	protected:
		Model* m_model{};
		Texture* m_tex{};
	};
}

