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

		void Add(std::shared_ptr<GameObject> object)
		{
			m_pChildren.push_back(object);
			object->Initialize();
		}

		void Remove(std::shared_ptr<GameObject> object)
		{
			auto it = std::find(m_pChildren.begin(), m_pChildren.end(), object);
			if (it != m_pChildren.end())
			{
				if (*it != m_pChildren.back())
					std::swap(*it, m_pChildren.back());
				m_pChildren.pop_back();
			}
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
			m_Transform.SetPosition(float(rect.x), float(rect.y), 0.0f);
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

	//protected:
		SDL_Rect m_Rect = { 0,0,0,0 };

	private:
		std::vector<std::shared_ptr<BaseComponent>> m_pComponents;
		std::vector<std::shared_ptr<GameObject>> m_pChildren;
		Transform m_Transform;
		std::shared_ptr<Texture2D> m_Texture;
		bool m_IsInitialized = false;
	};

