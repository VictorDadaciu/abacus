#pragma once

#include "Component.h"

namespace abc
{
	class Texture;
	class Model;
	class Shader;
	class RenderComponent : public Component
	{
	public:
		RenderComponent(GameObject* gameObject, const std::string& meshPath, const std::string& texturePath, Shader* shader);

		const Model* GetModel() const { return m_model; }
		const Texture* GetTexture() const { return m_tex; }

		virtual void Destroy() override;

		virtual ~RenderComponent();

	protected:
		Model* m_model{};
		Texture* m_tex{};
	};
}

