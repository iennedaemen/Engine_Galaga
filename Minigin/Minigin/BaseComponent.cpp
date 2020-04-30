#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "Transform.h"


BaseComponent::BaseComponent()
	: m_pGameObject(nullptr)
	, m_IsInitialized(false)
	, m_NeedsUpdate(true)
{
}

void BaseComponent::RootInitialize()
{
	if (m_IsInitialized) return;

	Initialize();

	m_IsInitialized = true;
}

void BaseComponent::RootUpdate()
{
	if(m_NeedsUpdate)
		Update();
}

GameObject* BaseComponent::GetGameObject() const
{
	if(m_pGameObject == nullptr)
	{
		throw("GameObject is a nullptr -> BaseComponent::GetGameObject()");
	}
	return m_pGameObject;
}

Transform BaseComponent::GetTransform() const
{
	if (m_pGameObject == nullptr)
	{
		throw("Gameobject is a nullptr -> BaseComponent::GetTransform()");
	}
	return m_pGameObject->GetTransform();
}

void BaseComponent::SetGameObject(GameObject* object)
{
	m_pGameObject = object;
}

void BaseComponent::NeedsUpdates(bool needsUpdate)
{
	m_NeedsUpdate = needsUpdate;
}

