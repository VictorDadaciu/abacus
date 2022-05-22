#include "pch.h"
#include "RenderComponent.h"

#include "Model.h"
#include "Texture.h"
#include "Shader.h"

namespace abc
{
	RenderComponent::RenderComponent(GameObject* gameObject, const std::string& meshPath, const std::string& texturePath, Shader* shader) : Component(gameObject, ComponentType::RENDER)
	{
		m_model = new Model(meshPath, shader);
		m_tex = new Texture(texturePath);
	}

	RenderComponent::~RenderComponent()
	{
	}

	void RenderComponent::Destroy()
	{
		m_model->Destroy();
		delete m_model;

		m_tex->Destroy();
		delete m_tex;
	}
}