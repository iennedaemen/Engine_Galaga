#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "Transform.h"
#include "GameObject.h"


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

GameObject* BaseComponent::GetGameObject()
{
	if(m_pGameObject == nullptr)
	{
		throw("GameObject is a nullptr -> BaseComponent::GetGameObject()");
	}
	return m_pGameObject;
}

void BaseComponent::SetGameObject(GameObject* object)
{
	m_pGameObject = object;
}

void BaseComponent::NeedsUpdates(bool needsUpdate)
{
	m_NeedsUpdate = needsUpdate;
}

