#pragma once
#include <memory>
#include "Texture2D.h"
#include <vector>
#include "Transform.h"
#include <SDL.h>

class BaseComponent;

	class GameObject
	{
	public:
		GameObject() = default;
		virtual ~GameObject() = default;

		virtual void SetPosition(float x, float y);
		Transform GetTransform();

		void AddComponent(std::shared_ptr<BaseComponent> component);

		template <class T>
		T* GetComponent()
		{
			const type_info& ti = typeid(T);
			for (auto component : m_pComponents)
			{
				if (component.get() && typeid(*component) == ti)
				{
					return static_cast<T*>(component.get());
				}
			}
			return nullptr;
		}

		template <class T>
		std::vector<T*> GetComponents(bool searchChildren = false)
		{
			const type_info& ti = typeid(T);
			std::vector<T*> components;

			for (auto* component : m_pComponents)
			{
				if (component && typeid(*component) == ti)
					components.push_back(static_cast<T*>(component));
			}
			return components;
		}

		void RootInitialize();
		void RootRender();
		void RootUpdate();

		virtual void Initialize() {}
		virtual void Render() {}
		virtual void Update() {}

		void NeedsUpdate(bool needsUpdate);

		void SetRect(SDL_Rect rect)
		{
			m_Rect = rect;
		}

		SDL_Rect GetRect()
		{
			return m_Rect;
		}


		// delete functions
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<std::shared_ptr<BaseComponent>> m_pComponents;
		Transform m_Transform;
		std::shared_ptr<Texture2D> m_Texture;
		bool m_IsInitialized = false;
		bool m_Render = true;
		bool m_NeedsUpdate = true;
		SDL_Rect m_Rect = { 0,0,0,0 };
	};

