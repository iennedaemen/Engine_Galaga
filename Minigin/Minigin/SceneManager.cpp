#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void SceneManager::Update()
{
	if (m_ActiveScene != nullptr)
		m_ActiveScene->RootUpdate();
}

void SceneManager::Render()
{
	if (m_ActiveScene != nullptr)
		m_ActiveScene->RootRender();
}

Scene& SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	return *scene;
}

void SceneManager::AddScene(const std::shared_ptr<Scene> scene)
{
	const auto it = std::find(m_Scenes.begin(), m_Scenes.end(), scene);
	if (it == m_Scenes.end())
	{
		m_Scenes.push_back(scene);

		scene->RootInitialize();

		if (m_ActiveScene == nullptr)
			m_ActiveScene = scene;
	}
}

void SceneManager::SetActiveScene(const std::string name)
{
	m_ActiveScene = nullptr;
	for (const auto scene : m_Scenes)
	{
		if (scene->GetName() == name)
		{
			m_ActiveScene = scene;
			if (m_ActiveScene->GetInitialized())
				m_ActiveScene->Reset();
		}
	}
	if (m_ActiveScene == nullptr)
	{
		throw std::exception("Scene not found -> SceneManager::SetActiveScene()\n");
	}
}
