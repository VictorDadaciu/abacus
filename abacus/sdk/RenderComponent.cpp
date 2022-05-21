#include "pch.h"
#include "RenderComponent.h"

#include "Model.h"
#include "Texture.h"

namespace abc
{
	RenderComponent::RenderComponent(GameObject* gameObject, const std::string& meshPath, const std::string& texturePath) : Component(gameObject, ComponentType::RENDER)
	{
		m_model = new Model(meshPath);
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