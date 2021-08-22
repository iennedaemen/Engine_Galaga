#pragma once
#include "BaseComponent.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)

	class Transform final: public BaseComponent
	{
	public:
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
		const float& GetRotation() const { return m_RotationAngle; }
		void SetRotation(float angle);
		const float& GetScale() const { return m_Scale; }
		void SetScale(float scale);

	protected:
		virtual void Initialize() {}
		virtual void Render() {}
		virtual void Update() {}

	private:
		glm::vec3 m_Position = { 0,0,0 };
		float m_RotationAngle = 0.0f;
		float m_Scale = 1.0f;
	};
