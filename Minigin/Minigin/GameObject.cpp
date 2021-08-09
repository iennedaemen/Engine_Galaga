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

	for (std::weak_ptr<GameObject> pChild : m_pChildren)
	{
		pChild.lock()->Initialize();
		for (std::weak_ptr<BaseComponent> pComponent : pChild.lock()->m_pComponents)
		{
			pComponent.lock()->RootInitialize();
		}
		pChild.lock()->m_IsInitialized = true;
	}

	m_IsInitialized = true;
}

void GameObject::RootUpdate()
{
	if (m_IsInitialized)
	{
		Update();

		for (std::weak_ptr<BaseComponent> pComponent : m_pComponents)
		{
			pComponent.lock()->Update();
		}

		for (std::weak_ptr<GameObject> pChild : m_pChildren)
		{
	
				pChild.lock()->Update();
				for (std::weak_ptr<BaseComponent> pComponent : pChild.lock()->m_pComponents)
				{
					pComponent.lock()->Update();
				}
		}
	}
}

void GameObject::RootRender()
{
	Render();

	for (std::weak_ptr<BaseComponent> pComponent : m_pComponents)
	{
		pComponent.lock()->Render();
	}

	for (std::weak_ptr<GameObject> pChild : m_pChildren)
	{
		pChild.lock()->Render();
		for (std::weak_ptr<BaseComponent> pComponent : pChild.lock()->m_pComponents)
		{
			pComponent.lock()->Render();
		}
	}
}

void GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
	m_Rect = { int(x), int(y), m_Rect.w, m_Rect.h };
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