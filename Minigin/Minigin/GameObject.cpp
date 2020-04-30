#include "MiniginPCH.h"
#include "GameObject.h"
#include "BaseComponent.h"


void GameObject::RootInitialize()
{
	if (m_IsInitialized)
		return;

	Initialize();

	for (std::weak_ptr<BaseComponent> pComponent : m_pComponents)
	{
		pComponent.lock()->RootInitialize();
	}

	m_IsInitialized = true;
}

void GameObject::Update()
{
	if (m_IsInitialized && m_NeedsUpdate)
	{
		for (std::weak_ptr<BaseComponent> pComponent : m_pComponents)
		{
			pComponent.lock()->Update();
		}
	}
}

void GameObject::Render()
{
	if (m_Render)
	{
		for (std::weak_ptr<BaseComponent> pComponent : m_pComponents)
		{
			pComponent.lock()->Render();
		}
	}
}

void GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

Transform GameObject::GetTransform()
{
	return m_Transform;
}

void GameObject::AddComponent(std::shared_ptr<BaseComponent> component)
{
	m_pComponents.push_back(component);
	component->SetGameObject(this);
}

void GameObject::NeedsUpdate(bool needsUpdate)
{
	m_NeedsUpdate = needsUpdate;
}
