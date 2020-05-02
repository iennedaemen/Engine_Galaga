#pragma once
#include "Singleton.h"

	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void AddScene(const std::shared_ptr<Scene> scene);
		void SetActiveScene(const std::string name);
		std::shared_ptr<Scene> GetActiveScene();

		void Update();
		void Render();
		
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
		std::shared_ptr<Scene> m_ActiveScene;
		bool m_IsInitialized = false;
	};
