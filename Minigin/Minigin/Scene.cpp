#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "ColliderComponent.h"

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name)
	: m_Name(name)
{
	
}

Scene::~Scene() = default;

void Scene::RootInitialize()
{
	if(m_IsInitialized)
		return;

	Initialize();
	
	for (std::weak_ptr<GameObject> pObject : m_Objects)
	{
		pObject.lock()->RootInitialize();
	}

	m_IsInitialized = true;
}


void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	m_Objects.push_back(object);
	object->RootInitialize();
}

void Scene::Remove(const std::shared_ptr<GameObject>& object)
{
	auto it = std::find(m_Objects.begin(), m_Objects.end(), object);
	if (it != m_Objects.end())
	{
		if (*it != m_Objects.back())
			std::swap(*it, m_Objects.back());
		m_Objects.pop_back();
	}
}

void Scene::RootUpdate()
{
	Update();
	
	for(auto& object : m_Objects)
	{		
		object->RootUpdate();
	}
}

void Scene::RootRender() const
{
	Render();
	
	for (const auto& object : m_Objects)
	{
		object->RootRender();
	}
}

std::string Scene::GetName() const
{
	return m_Name;
}

bool Scene::GetInitialized()
{
	return m_IsInitialized;
}
