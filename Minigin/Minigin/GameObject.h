#pragma once
#include <memory>
#include "Texture2D.h"
#include <vector>
#include "Transform.h"

class BaseComponent;

	class GameObject final
	{
	public:
		GameObject() = default;
		virtual ~GameObject() = default;

		virtual void SetPosition(float x, float y);
		Transform GetTransform();

		void AddComponent(std::shared_ptr<BaseComponent> component);

		void EnableRender(bool enable)
		{
			m_Render = enable;
		}

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

		virtual void Initialize() {}
		virtual void Render();
		void Update();

		void NeedsUpdate(bool needsUpdate);


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
	};

